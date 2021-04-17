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
    
    printf("����ֵΪ:[%d]\n", maxdata);
    
    return maxdata;
}

int main()
{
    int i = 0;
    
    nodelist *list, *rlist;
    nodelist *tmp;
    int pos;
    
    int A[9] = {1,2,3,4,5,6,7,8,9};
    
    /*��ʼ��ͷ���*/
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
    
    /*�������*/
    nodelist *p;
    p = list->next;
    
    while (p)
    {
        printf("%d ", p->data);
        p = p->next;
    }
    
    printf("-->nul\n");
    
    /*����λ�õ�Ԫ��*/
    pos = pos_list(list, 3);
    printf("pos:%d, address:%p\n", pos);
    
    /*���Ҷ���������ֵ*/
    pos = find_max_num_list(list);
    printf("pos:%d, address:%p\n", pos);
    return 0;
}