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
    
    rlist = list; /*β�巨ʹ��*/
    for (; i < 9; i++)
    {
        tmp = (nodelist *) malloc (sizeof(nodelist));
        tmp->data = A[i];
        /*ͷ�巨
        tmp->next = list->next;
        list->next = tmp;
        */
        //tmp->next = NULL; /*β�巨����д*/
        /*β�巨*/
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