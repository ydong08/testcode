#include "stdafx.h"
#include "stdio.h"
#include <stdlib.h>
#include "string.h"
 
typedef int elemType ;
 
//定义双向链表的基本结构
typedef struct _DOUBLE_LINK_NODE  
{  
    int data;  
    struct _DOUBLE_LINK_NODE* prev;  
    struct _DOUBLE_LINK_NODE* next;  
}DOUBLE_LINK_NODE;  


//创建双向链表节点
DOUBLE_LINK_NODE* create_double_link_node(int value)  
{  
    DOUBLE_LINK_NODE* pDLinkNode = NULL;  
    pDLinkNode = (DOUBLE_LINK_NODE*)malloc(sizeof(DOUBLE_LINK_NODE));  
    assert(NULL != pDLinkNode);  
  
    memset(pDLinkNode, 0, sizeof(DOUBLE_LINK_NODE));  
    pDLinkNode->data = value;  
    return pDLinkNode;  
}  


//删除双向链表
void delete_all_double_link_node(DOUBLE_LINK_NODE** pDLinkNode)  
{  
    DOUBLE_LINK_NODE* pNode;  
    if(NULL == *pDLinkNode)  
        return ;  
  
    pNode = *pDLinkNode;  
    *pDLinkNode = pNode->next;  
    free(pNode);  
    delete_all_double_link_node(pDLinkNode);  
}  


//在双向链表中查找数据
DOUBLE_LINK_NODE* find_data_in_double_link(const DOUBLE_LINK_NODE* pDLinkNode, int data)  
{  
    DOUBLE_LINK_NODE* pNode = NULL;  
    if(NULL == pDLinkNode)  
        return NULL;  
  
    pNode = (DOUBLE_LINK_NODE*)pDLinkNode;  
    while(NULL != pNode){  
        if(data == pNode->data)  
            return pNode;  
        pNode = pNode ->next;  
    }  
      
    return NULL;  
}  


//双向链表中插入数据
STATUS insert_data_into_double_link(DOUBLE_LINK_NODE** ppDLinkNode, int data)  
{  
    DOUBLE_LINK_NODE* pNode;  
    DOUBLE_LINK_NODE* pIndex;  
  
    if(NULL == ppDLinkNode)  
        return FALSE;  
  
    if(NULL == *ppDLinkNode){  
        pNode = create_double_link_node(data);  
        assert(NULL != pNode);  
        *ppDLinkNode = pNode;  
        (*ppDLinkNode)->prev = (*ppDLinkNode)->next = NULL;  
        return TRUE;  
    }  
  
    if(NULL != find_data_in_double_link(*ppDLinkNode, data))  
        return FALSE;  
  
    pNode = create_double_link_node(data);  
    assert(NULL != pNode);  
  
    pIndex = *ppDLinkNode;  
    while(NULL != pIndex->next)  
        pIndex = pIndex->next;  
  
    pNode->prev = pIndex;  
    pNode->next = pIndex->next;  
    pIndex->next = pNode;  
    return TRUE;  
}  


//双向链表中删除数据
STATUS delete_data_from_double_link(DOUBLE_LINK_NODE** ppDLinkNode, int data)  
{  
    DOUBLE_LINK_NODE* pNode;  
    if(NULL == ppDLinkNode || NULL == *ppDLinkNode)  
        return FALSE;  
  
    pNode = find_data_in_double_link(*ppDLinkNode, data);  
    if(NULL == pNode)  
        return FALSE;  
  
    if(pNode == *ppDLinkNode){  
        if(NULL == (*ppDLinkNode)->next){  
            *ppDLinkNode = NULL;  
        }else{  
            *ppDLinkNode = pNode->next;  
            (*ppDLinkNode)->prev = NULL;  
        }  
  
    }else{  
        if(pNode->next)  
            pNode->next->prev = pNode->prev;  
        pNode->prev->next = pNode->next;  
    }  
  
    free(pNode);  
    return TRUE;  
}  


//统计双向链表中数据的个数
int count_number_in_double_link(const DOUBLE_LINK_NODE* pDLinkNode)  
{  
    int count = 0;  
    DOUBLE_LINK_NODE* pNode = (DOUBLE_LINK_NODE*)pDLinkNode;  
  
    while(NULL != pNode){  
        count ++;  
        pNode = pNode->next;  
    }  
    return count;  
}  

//打印双向链表中数据
void print_double_link_node(const DOUBLE_LINK_NODE* pDLinkNode)  
{  
    DOUBLE_LINK_NODE* pNode = (DOUBLE_LINK_NODE*)pDLinkNode;  
  
    while(NULL != pNode){  
        printf("%d\n", pNode->data);  
        pNode = pNode ->next;  
    }  
}  