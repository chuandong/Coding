#include "dong.h"

typedef struct node{
    int data;
    struct node *next;
}nodelist;

void init_list(nodelist **L)
{
    (*L) = (nodelist *) malloc (sizeof(nodelist));
    assert((*L) != NULL);
    (*L)->next = NULL;
    
    return ;
}

int main()
{
    int i = 0;
    
    nodelist *list, *rlist;
    nodelist *tmp;
    
    int A[9] = {1,2,3,4,5,6,7,8,9};
    
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
    
    nodelist *p;
    p = list->next;
    
    while (p)
    {
        printf("%d ", p->data);
        p = p->next;
    }
    
    printf("-->nul\n");
    return 0;
}