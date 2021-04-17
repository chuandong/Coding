#include "dong.h"

typedef struct node{
    int data;
    struct node *next;
}nodelist, *linklist;

void init_list(nodelist **L)
{
    (*L) = (nodelist *) malloc (sizeof(nodelist));
    assert((*L) != NULL);
    (*L)->next = NULL;
    
    return ;
}

int pos_list(nodelist *L, int pos)
{
    int i = 0;
    
    nodelist *p = L;
    
    while (p && i < pos)
    {
        p = p->next;
        i++;
    }
    
    if (i < pos)
    {
        printf("not found\n");
        return -1;
    }
    
    return p->data;
}

int find_max_num_list(nodelist *L)
{
    int maxdata = 0;
    
    nodelist *p = L->next;
    
    while (p)
    {
        if (maxdata < p->data)
            maxdata = p->data;
        p = p->next; 
    }
    
    printf("最大的值为:[%d]\n", maxdata);
    
    return maxdata;
}

int main()
{
    int i = 0;
    
    nodelist *list, *rlist;
    nodelist *tmp;
    int pos;
    
    int A[9] = {1,2,3,4,5,6,7,8,9};
    
    /*初始化头结点*/
    init_list(&list);
    
    rlist = list; /*尾插法使用*/
    for (; i < 9; i++)
    {
        tmp = (nodelist *) malloc (sizeof(nodelist));
        tmp->data = A[i];
        /*头插法
        tmp->next = list->next;
        list->next = tmp;
        */
        //tmp->next = NULL; /*尾插法不能写*/
        /*尾插法*/
        rlist->next = tmp;
        rlist = tmp;
    }
    
    /*输出链表*/
    nodelist *p;
    p = list->next;
    
    while (p)
    {
        printf("%d ", p->data);
        p = p->next;
    }
    
    printf("-->nul\n");
    
    /*查找位置的元素*/
    pos = pos_list(list, 3);
    printf("pos:%d, address:%p\n", pos);
    
    /*查找队列中最大的值*/
    pos = find_max_num_list(list);
    printf("pos:%d, address:%p\n", pos);
    return 0;
}