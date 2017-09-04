/********************************************************************************
**
** �ļ���:     leelen_list.c
** ��Ȩ����:   (c) 2015 �������ֿƼ����޹�˾
** �ļ�����:   ʵ�ֶ���������
**
*********************************************************************************
**             �޸���ʷ��¼
**===============================================================================
**| ����       | ����   |  �޸ļ�¼
**===============================================================================
**| 2015/09/28 | zzh    |  �������ļ�
**
*********************************************************************************/


#include "leelen_comdef.h"
#include "leelen_list.h"


#define MAX_LISTITEM                        10000                   /* ��һ����������������ڵ���� */


/*******************************************************************
** ������:     check_list
** ��������:   ����������Ч��
** ����:       [in]  lp:        ����
** ����:       true:    ������Ч
**             false:   ������Ч
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
** ������:     init_list
** ��������:   ��ʼ������
** ����:       [in]  lp:        ����
** ����:       true:    �ɹ�
**             false:   ʧ��
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
** ������:     get_list_item
** ��������:   ��ȡ����ڵ����
** ����:       [in]  lp:        ����
** ����:       ����ڵ����
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
** ������:     get_list_head
** ��������:   ��ȡ����ͷ�ڵ�
** ����:       [in]  lp:        ����
** ����:       ����ͷ�ڵ�; �������޽ڵ�, �򷵻�0
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
** ������:     get_list_tail
** ��������:   ��ȡ����β�ڵ�
** ����:       [in]  lp:        ����
** ����:       ����β�ڵ�; �������޽ڵ�, �򷵻�0
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
** ������:     get_list_ele
** ��������:   ��ȡ��n���ڵ�
** ����:       [in]  lp:        ����
**             [in]  n:        ����ȡ�ڵ�����, ע��: ��Ŵ�0��ʼ���, 0��ʾ����ͷ�ڵ�
** ����:       ��n�ڵ�; �緵��0, ���ʾ��n���ڵ㲻����
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
** ������:     get_list_next_ele
** ��������:   ��ȡָ���ڵ���¸��ڵ�
** ����:       [in]  bp:        ָ���ڵ�
** ����:       ����ָ���ڵ�bp���¸��ڵ�; �緵��0, ���ʾ��������һ�ڵ�
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
** ������:     get_list_prv_ele
** ��������:   ��ȡָ���ڵ��ǰһ�ڵ�
** ����:       [in]  bp:        ָ���ڵ�
** ����:       ����ָ���ڵ�bp��ǰһ�ڵ�; �緵��0, ���ʾ������ǰһ�ڵ�
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
** ������:     del_list_ele
** ��������:   ɾ��ָ���ڵ�
** ����:       [in]  lp:        ����
**             [in]  bp:        ָ���ڵ�
** ����:       ����ָ���ڵ�bp���¸��ڵ�; �緵��0, ���ʾbp��������һ�ڵ�
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
** ������:     del_list_head
** ��������:   ɾ������ͷ�ڵ�
** ����:       [in]  lp:        ����
** ����:       ����ͷ�ڵ�; �緵��0, ���ʾ����������ͷ�ڵ�
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
** ������:     del_list_tail
** ��������:   ɾ������β�ڵ�
** ����:       [in]  lp:        ����
** ����:       ����β�ڵ�; �緵��0, ���ʾ����������β�ڵ�
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
** ������:     append_list_ele
** ��������:   ������β��׷��һ���ڵ�
** ����:       [in]  lp:        ����
**             [in]  bp:        ��׷�ӽڵ�
** ����:       ׷�ӳɹ���ʧ��
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
** ������:     insert_list_ele
** ��������:   ������ͷ����һ���ڵ�
** ����:       [in]  lp:        ����
**             [in]  bp:        ������Ľڵ�
** ����:       ����ɹ���ʧ��
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
** ������:     init_mem_list
** ��������:   ��һ���ڴ��ʼ������������
** ����:       [in]  memlp:     ����
**             [in]  addr:      �ڴ���ʼ��ַ
**             [in]  nblks:     �ڴ�����
**             [in]  blksize:   �ڴ���С
** ����:       �ɹ���ʧ��
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



