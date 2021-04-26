#include "dong.h"

typedef struct node
{
    int data;
    struct node *next;
}queue_node;

typedef struct queue
{
    queue_node *front;
    queue_node *rear;
    int size;
}queue_list;

void init_queue(queue_list **Q)
{
    (*Q) = (queue_list *) malloc (sizeof(queue_list));
    (*Q)->front = (*Q)->rear = (queue_node *) malloc (sizeof(queue_node));
    
    (*Q)->front->next = NULL;
    (*Q)->size = 0;
    
    return ;
}

void insert_queue(queue_list *Q, int data)
{
    queue_node *p = (queue_node *) malloc (sizeof(queue_node));
    p->data = data;
    p->next = NULL;
    
    Q->rear->next = p; /*指向新加入的节点*/
    Q->rear = p;       /*队列节点往后移*/
    Q->size++;         /*计数*/
    
    return ;
}

int delete_queue(queue_list *Q, int pos)
{
    int i = 1;
    int data;
    
    queue_node *s = (queue_node *) malloc (sizeof(queue_node));
    queue_node *p = Q->front->next;
    
    for (; i < pos; i++)
    {
        p = p->next;
    }
    
    s = p;
    data = s->data;
    
    p->next = s->next;
    
    Q->size--;
    
    free(s);
    
    return data;
}

int pop_queue(queue_list *Q, int *data)
{
    queue_node *p;
    
    if(Q->rear == Q->front)
        return -1;
    
    p = Q->front->next;
    *data = p->data;
    
    if (p == Q->rear)
        Q->rear = Q->front;
        
    Q->front->next = p->next;
    
    free(p);
    
    return 0;
}

void show_queue(queue_list *Q)
{
    queue_node *p = Q->front->next; /*rear节点在尾端, front节点在首端*/
    while (p)
    {
        printf("%d ", p->data);
        p = p->next;
    }
    
    putchar(10);
    return ;
}

int main()
{
    int num = 0;
    
    queue_list *queue;
    
    init_queue(&queue);
    
    insert_queue(queue, 1);
    insert_queue(queue, 2);
    insert_queue(queue, 30);
    insert_queue(queue, 4);
    insert_queue(queue, 5);
    
    show_queue(queue);
    
    printf("queue length:%d\n", queue->size);
    
    pop_queue(queue, &num);
    
    printf("val:%d\n", num);
    
    num = delete_queue(queue, 3);
    
    printf("pos val:%d\n", num);
    printf("queue length:%d\n", queue->size);
    show_queue(queue);
    return 0;
}