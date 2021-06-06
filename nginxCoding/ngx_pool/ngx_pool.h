#include "ngx_core.h"
#include "ngx_config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

typedef u_char *(*ngx_log_handler_pt) (ngx_log_t *log, u_char *buf, size_t len);
typedef void (*ngx_log_writer_pt) (ngx_log_t *log, ngx_uint_t level, u_char *buf, size_t len);
typedef void (*ngx_pool_cleanup_pt)(void *data);
//typedef AO_t                        ngx_atomic_uint_t;
typedef void *  ngx_atomic_uint_t;
typedef void *  ngx_buf_tag_t;

struct ngx_log_s {
    ngx_uint_t           log_level;
    ngx_open_file_t     *file;

    ngx_atomic_uint_t    connection;

    time_t               disk_full_time;

    ngx_log_handler_pt   handler;
    void                *data;

    ngx_log_writer_pt    writer;
    void                *wdata;

    /*
     * we declare "action" as "char *" because the actions are usually
     * the static strings and in the "u_char *" case we have to override
     * their types all the time
     */

    char                *action;

    ngx_log_t           *next;
};

struct ngx_buf_s {
    u_char          *pos;
    u_char          *last;
    off_t            file_pos;
    off_t            file_last;

    u_char          *start;         /* start of buffer */
    u_char          *end;           /* end of buffer */
    ngx_buf_tag_t    tag;
    ngx_file_t      *file;
    ngx_buf_t       *shadow;


    /* the buf's content could be changed */
    unsigned         temporary:1;

    /*
     * the buf's content is in a memory cache or in a read only memory
     * and must not be changed
     */
    unsigned         memory:1;

    /* the buf's content is mmap()ed and must not be changed */
    unsigned         mmap:1;

    unsigned         recycled:1;
    unsigned         in_file:1;
    unsigned         flush:1;
    unsigned         sync:1;
    unsigned         last_buf:1;
    unsigned         last_in_chain:1;

    unsigned         last_shadow:1;
    unsigned         temp_file:1;

    /* STUB */ int   num;
};

struct ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    void                 *data;
    ngx_pool_cleanup_t   *next;
};

struct ngx_pool_large_s {
    ngx_pool_large_t     *next;
    void                 *alloc;
};


struct ngx_chain_s {
    ngx_buf_t    *buf;
    ngx_chain_t  *next;
};


typedef struct {
    u_char               *last;
    u_char               *end;
    ngx_pool_t           *next;
    ngx_uint_t            failed;
} ngx_pool_data_t;

struct ngx_pool_s {
    ngx_pool_data_t       d;         /*�ڴ���е�����*/
    size_t                max;       /*���������ڴ��С*/
    ngx_pool_t           *current;   /*���ڱ����ڴ������Ŀ���ָ��*/
    ngx_chain_t          *chain;     /* */
    ngx_pool_large_t     *large;     /*��������֯�ϴ���޷����ڴ����������ڴ��**/
    ngx_pool_cleanup_t   *cleanup;   /*�����������ͷ��ڴ��*/
    ngx_log_t            *log;       /*�ڴ������ص���־��Ϣ*/
};