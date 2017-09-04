/********************************************************************************
**
** 文件名:     leelen_list.c
** 版权所有:   (c) 2015 厦门立林科技有限公司
** 文件描述:   实现队列链表功能
**
*********************************************************************************
**             修改历史记录
**===============================================================================
**| 日期       | 作者   |  修改记录
**===============================================================================
**| 2015/09/28 | zzh    |  创建该文件
**
*********************************************************************************/


#include "leelen_comdef.h"
#include "leelen_list.h"


#define MAX_LISTITEM                        10000                   /* 在一个链表中允许的最大节点个数 */


/*******************************************************************
** 函数名:     check_list
** 函数描述:   检测链表的有效性
** 参数:       [in]  lp:        链表
** 返回:       true:    链表有效
**             false:   链表无效
********************************************************************/
bool_t check_list(lee_list_t *lp)
{
    int    count, item;
    lee_node_t *curnode;
    
    if (lp == 0) {
        return false;
    }
    
    count   = 0;
    item    = lp->item;
    curnode = lp->head;
    while(curnode != 0) {
        if (++count > item) {
            return false;
        }
        if (count > MAX_LISTITEM) {
            return false;
        }
        curnode = curnode->next;
    }
    if (count != item) {
        return false;
    }
    
    count   = 0;
    curnode = lp->tail;
    while(curnode != 0) {
        if (++count > item) {
            return false;
        }
        if (count > MAX_LISTITEM) {
            return false;
        }
        curnode = curnode->prv;
    }
    if (count != item) {
        return false;
    } else {
        return true;
    }
}

/*******************************************************************
** 函数名:     init_list
** 函数描述:   初始化链表
** 参数:       [in]  lp:        链表
** 返回:       true:    成功
**             false:   失败
********************************************************************/
bool_t init_list(lee_list_t *lp)
{
    if (lp == 0) {
        return false;
    }

    lp->head = 0;
    lp->tail = 0;
    lp->item = 0;
    return true;
}

/*******************************************************************
** 函数名:     get_list_item
** 函数描述:   获取链表节点个数
** 参数:       [in]  lp:        链表
** 返回:       链表节点个数
********************************************************************/
int get_list_item(lee_list_t *lp)
{
    if (lp == 0) {
        return 0;
    } else {
        return (lp->item);
    }
}

/*******************************************************************
** 函数名:     get_list_head
** 函数描述:   获取链表头节点
** 参数:       [in]  lp:        链表
** 返回:       链表头节点; 如链表无节点, 则返回0
********************************************************************/
void * get_list_head(lee_list_t *lp)
{
    if (lp == 0 || lp->item == 0) {
        return 0;
    } else {
        return ((void *)((uint8_t *)lp->head + sizeof(lee_node_t)));
    }
}

/*******************************************************************
** 函数名:     get_list_tail
** 函数描述:   获取链表尾节点
** 参数:       [in]  lp:        链表
** 返回:       链表尾节点; 如链表无节点, 则返回0
********************************************************************/
void * get_list_tail(lee_list_t *lp)
{
    if (lp == 0 || lp->item == 0) {
        return 0;
    } else {
        return ((void *)((uint8_t *)lp->tail + sizeof(lee_node_t)));
    }
}

/*******************************************************************
** 函数名:     get_list_ele
** 函数描述:   获取第n个节点
** 参数:       [in]  lp:        链表
**             [in]  n:        待获取节点的序号, 注意: 序号从0开始编号, 0表示链表头节点
** 返回:       第n节点; 如返回0, 则表示第n个节点不存在
********************************************************************/
void * get_list_ele(lee_list_t *lp, int n)
{
    lee_node_t *curnode;

    if (lp == 0 || lp->item <= n) {
        return 0;
    }

    curnode = lp->head;
    for (; n > 0; n--) {
        if (curnode == 0) {
            break;
        }
        curnode = curnode->next;
    }
    if (curnode == 0) {
        return 0;
    } else {
        return ((void *)((uint8_t *)curnode + sizeof(lee_node_t)));
    }
}

/*******************************************************************
** 函数名:     get_list_next_ele
** 函数描述:   获取指定节点的下个节点
** 参数:       [in]  bp:        指定节点
** 返回:       返回指定节点bp的下个节点; 如返回0, 则表示不存在下一节点
********************************************************************/
void * get_list_next_ele(void *bp)
{
    lee_node_t *curnode;

    if (bp == 0) {
        return 0;
    } else {
        curnode = (lee_node_t *)((uint8_t *)bp - sizeof(lee_node_t));
        if ((curnode = curnode->next) == 0) {
            return 0;
        } else {
            return ((void *)((uint8_t *)curnode + sizeof(lee_node_t)));
        }
    }
}

/*******************************************************************
** 函数名:     get_list_prv_ele
** 函数描述:   获取指定节点的前一节点
** 参数:       [in]  bp:        指定节点
** 返回:       返回指定节点bp的前一节点; 如返回0, 则表示不存在前一节点
********************************************************************/
void * get_list_prv_ele(void *bp)
{
    lee_node_t *curnode;

    if (bp == 0) {
        return 0;
    } else {
        curnode = (lee_node_t *)((uint8_t *)bp - sizeof(lee_node_t));
        if ((curnode = curnode->prv) == 0) {
            return 0;
        } else {
            return ((void *)((uint8_t *)curnode + sizeof(lee_node_t)));
        }
    }
}

/*******************************************************************
** 函数名:     del_list_ele
** 函数描述:   删除指定节点
** 参数:       [in]  lp:        链表
**             [in]  bp:        指定节点
** 返回:       返回指定节点bp的下个节点; 如返回0, 则表示bp不存在下一节点
********************************************************************/
void * del_list_ele(lee_list_t *lp, void *bp)
{
    lee_node_t *curnode, *prvnode, *nextnode;

    if (lp == 0 || bp == 0) {
        return 0;
    }
    if (lp->item == 0) {
        return 0;
    }
    
    lp->item--;
    curnode  = (lee_node_t *)((uint8_t *)bp - sizeof(lee_node_t));
    prvnode  = curnode->prv;
    nextnode = curnode->next;
    if (prvnode == 0) {
        lp->head = nextnode;
    } else {
        prvnode->next = nextnode;
    }
    if (nextnode == 0) {
        lp->tail = prvnode;
        return 0;
    } else {
        nextnode->prv = prvnode;
        return ((void *)((uint8_t *)nextnode + sizeof(lee_node_t)));
    }
}

/*******************************************************************
** 函数名:     del_list_head
** 函数描述:   删除链表头节点
** 参数:       [in]  lp:        链表
** 返回:       链表头节点; 如返回0, 则表示不存在链表头节点
********************************************************************/
void * del_list_head(lee_list_t *lp)
{
    void *bp;

    if (lp == 0 || lp->item == 0) {
        return 0;
    }
    bp = (void *)((uint8_t *)lp->head + sizeof(lee_node_t));
    del_list_ele(lp, bp);
    return bp;
}

/*******************************************************************
** 函数名:     del_list_tail
** 函数描述:   删除链表尾节点
** 参数:       [in]  lp:        链表
** 返回:       链表尾节点; 如返回0, 则表示不存在链表尾节点
********************************************************************/
void * del_list_tail(lee_list_t *lp)
{
    void *bp;

    if (lp == 0 || lp->item == 0) {
        return 0;
    }
    bp = (void *)((uint8_t *)lp->tail + sizeof(lee_node_t));
    del_list_ele(lp, bp);
    return bp;
}

/*******************************************************************
** 函数名:     append_list_ele
** 函数描述:   在链表尾上追加一个节点
** 参数:       [in]  lp:        链表
**             [in]  bp:        待追加节点
** 返回:       追加成功或失败
********************************************************************/
bool_t append_list_ele(lee_list_t *lp, void *bp)
{
    lee_node_t *curnode;

    if (lp == 0 || bp == 0) {
        return false;
    }

    curnode = (lee_node_t *)((uint8_t *)bp - sizeof(lee_node_t));
    curnode->prv = lp->tail;
    if (lp->item == 0) {
        lp->head = curnode;
    } else {
        lp->tail->next = curnode;
    }
    curnode->next = 0;
    lp->tail = curnode;
    lp->item++;
    return true;
}

/*******************************************************************
** 函数名:     insert_list_ele
** 函数描述:   在链表头插入一个节点
** 参数:       [in]  lp:        链表
**             [in]  bp:        待插入的节点
** 返回:       插入成功或失败
********************************************************************/
bool_t insert_list_ele(lee_list_t *lp, void *bp)
{
    lee_node_t *curnode;

    if (lp == 0 || bp == 0){
        return false;
    }

    curnode = (lee_node_t *)((uint8_t *)bp - sizeof(lee_node_t));
    if (lp->item == 0) {
        lp->tail = curnode;
        curnode->next = 0;
    } else {
        lp->head->prv = curnode;
        curnode->next = lp->head;
    }
    curnode->prv = 0;
    lp->head = curnode;

    lp->item++;
    return true;
}


/*******************************************************************
** 函数名:     init_mem_list
** 函数描述:   将一块内存初始化成链表缓冲区
** 参数:       [in]  memlp:     链表
**             [in]  addr:      内存起始地址
**             [in]  nblks:     内存块个数
**             [in]  blksize:   内存块大小
** 返回:       成功或失败
********************************************************************/
bool_t init_mem_list(lee_list_t *memlp, void *addr, int nblks, int blksize)
{
    if (!init_list(memlp)) {
        return false;
    }

    addr = (void *)((uint8_t *)addr + sizeof(lee_node_t));
    for (; nblks > 0; nblks--) {
        append_list_ele(memlp, addr);
        addr = (void *)((uint8_t *)addr + blksize);
    }
    return true;
}



