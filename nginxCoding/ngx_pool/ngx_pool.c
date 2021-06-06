#include "ngx_pool.h"

#define NGX_MAX_ALLOC_FROM_POOL  (4096 - 1)
#define ngx_memalign(alignment, size, log)  ngx_alloc(size, log)
#define ngx_free          free


void *
ngx_alloc(size_t size, ngx_log_t *log)
{
    void  *p;

    p = malloc(size);
    if (p == NULL) {
        //ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "malloc(%uz) failed", size);
        printf("malloc(%uz) failed\n", size);
    }

    //ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, log, 0, "malloc: %p:%uz", p, size);
    printf("malloc: %p:%uz\n", p, size);

    return p;
}

ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
    if (p == NULL) {
        return NULL;
    }

    p->d.last = (u_char *) p + sizeof(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = NULL;
    p->d.failed = 0;

    size = size - sizeof(ngx_pool_t);
    printf("size:[%uz], sizeof:[%d]\n", size, sizeof(ngx_pool_t));
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = NULL;
    p->large = NULL;
    p->cleanup = NULL;
    p->log = log;

    return p;
}

void
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    printf("max:[%uz]\n", pool->max);
    for (c = pool->cleanup; c; c = c->next) {
        if (c->handler) {
            //ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "run cleanup: %p", c);
            printf("run cleanup: %p\n", c);
            c->handler(c->data);
        }
    }

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    for (l = pool->large; l; l = l->next) {
        //ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
        printf("free: %p\n", l->alloc);
    }

    for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        //ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p, unused: %uz", p, p->d.end - p->d.last);
        printf("free: %p, unused: %uz\n", p, p->d.end - p->d.last);

        if (n == NULL) {
            break;
        }
    }

    for (l = pool->large; l; l = l->next) {
        if (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        if (n == NULL) {
            break;
        }
    }
}


static void *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    if (m == NULL) {
        return NULL;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = NULL;
    new->d.failed = 0;

    m += sizeof(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;

    for (p = pool->current; p->d.next; p = p->d.next) {
        if (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    return m;
}

/*从pool中分配大小为size的内存，align表示是否做内存对齐*/
static ngx_inline void *
ngx_palloc_small(ngx_pool_t *pool, size_t size, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;
	/*从第一个可用结点开始*/
    p = pool->current;

    do {
        m = p->d.last;

        if (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT); /*内存对齐*/
        }

        printf("large:[%uz], size:[%uz]\n", (size_t) (p->d.end - m), size);
		/*如果内存够用，就直接分配*/
        if ((size_t) (p->d.end - m) >= size) {
			/*记录上一次分配的结束为止*/
            p->d.last = m + size;
            printf("last:[%p], [%u], m:[%p], [%u]\n", p->d.last, (size_t)p->d.last, m, (size_t)m);
            return m;
        }
		/*内存不足，就用下一个*/
        p = p->d.next;

    } while (p);

	/*都失败了，就重新分配一个内存池*/
    return ngx_palloc_block(pool, size);
}

static void *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    void              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size, pool->log);
    if (p == NULL) {
        return NULL;
    }

    n = 0;

    printf("3--->alloc:[%p], [%uz]\n", large->alloc, (size_t)large->alloc);
    for (large = pool->large; large; large = large->next) {
        if (large->alloc == NULL) {
            large->alloc = p;
            printf("1--->alloc:[%p], [%uz]\n", large->alloc, (size_t)large->alloc);
            return p;
        }
        
        printf("2--->alloc:[%p], [%uz]\n", large->alloc, (size_t)large->alloc);
        
        if (n++ > 3) {
            break;
        }
    }

    large = ngx_palloc_small(pool, sizeof(ngx_pool_large_t), 1);
    if (large == NULL) {
        printf("1 ngx_palloc_large alloc:[%p], [%u]\n", large->alloc, (size_t)large->alloc);
        ngx_free(p);
        return NULL;
    }
    
    printf("2 ngx_palloc_large alloc:[%p], [%u]\n", large->alloc, (size_t)large->alloc);
    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}


void *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
    printf("内存大小:[%d]\n", pool->max);
    if (size <= pool->max) {
        printf("小内存分配\n");
        return ngx_palloc_small(pool, size, 1);  /*小内存分配*/
    }
    printf("大内存分配\n");
    return ngx_palloc_large(pool, size);  /*大内存分配*/
}


int main(int argc, char *argv[])
{
    ngx_pool_t *pool;
    ngx_log_t  *logs;
    
    pool = ngx_create_pool(1024, logs);
    if (pool == NULL)
    {
        printf("ngx_create_pool error. errmsg:[%s]\n", strerror(errno));
        return -1;
    }
    
    printf("create pool success\n");
    
    //ngx_destroy_pool(pool);
    
    ngx_palloc(pool, 1024);
    return 0;
}