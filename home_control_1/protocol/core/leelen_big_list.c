/********************************************************************************
**
** �ļ���:     leelen_big_list.c
** ��Ȩ����:   (c) 2015 �������ֿƼ����޹�˾
** �ļ�����:   ʵ��ͨѶЭ������ݻ��巢�Ͷ���
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
#include "leelen_timer.h"
#include "leelen_big_packet.h"
#include "leelen_big_list.h"
#include "leelen_big_udp.h"





/*
********************************************************************************
* ��������
********************************************************************************
*/
#define DEBUG_BIGLIST                       0

#define MAX_NODE                            20                      /* �������������ݰ����� */

#define MAX_SEND                            10                      /* ����ش��Ĵ��� */
#define PERIOD_SCAN                         100                     /* ɨ������, ��λ:ms */

#define DEF_OVERTIME                        (15*(1000/PERIOD_SCAN)) /* ȱʡӦ��ʱʱ��, ��λ: �� ����Ҫ��socket���ӳ�ʱʱ�䳤 */
#define DEF_SEND                            3                       /* ȱʡ���ʹ��� */


/*
********************************************************************************
* �������ݰ��ڵ����ݽṹ
********************************************************************************
*/
typedef struct {
    uint16_t      keyword;            /* �ؼ��� */
    uint8_t       ct_time;            /* ʱ����� */
    uint8_t       ct_send;            /* �Ѵ��ʹ��� */
    uint8_t       max_time;           /* �����ͳ�ʱʱ�� */
    uint8_t       max_send;           /* ������ش��Ĵ��� */
    uint8_t       file_send;          /* �ļ����ʹ��� */
    big_packet_t  *pkt;               /* ���ݷ��� */
    void          (*informer)(RESULT_E);
} big_node_t;

/*
********************************************************************************
* ����ģ��ֲ�����
********************************************************************************
*/
static struct {
    lee_node_t    res;
    big_node_t   node;
} memblock[MAX_NODE];

static uint32_t       s_session_id;
static lee_list_t     s_freelist, s_usedlist;
static big_node_t    *s_curnode;
static uint8_t        s_scantmrid;

/*******************************************************************
** ������:     free_cur_big_node
** ��������:   �ͷŵ�ǰ����Ľڵ�
** ����:       [in]  stop: ֹͣ����
** ����:       ��
********************************************************************/
static void free_cur_big_node(bool_t stop, RESULT_E result)
{
    void (*informer)(RESULT_E);

    if (s_curnode != NULL) {
        if (result == RESULT_OVER && s_curnode->file_send < DEF_SEND){/* һ���ļ���෢3�� */
            s_curnode->pkt->cur_pktid  = 1;
            s_curnode->pkt->session_id = s_session_id;                /* �ỰID���·��� */
            s_session_id++;
            if (s_session_id == 0){
                s_session_id = 1;
            }
            append_list_ele(&s_usedlist, s_curnode);                 /* ������β���� */
            s_curnode = NULL;
            return;
        }
        
        informer = s_curnode->informer;
        big_packet_free(s_curnode->pkt);                             /* �ͷŵ�ǰ�ڵ�����ݰ� */
        s_curnode->pkt = NULL;
        append_list_ele(&s_freelist, s_curnode);                     /* ����ǰ�ڵ�ҵ����������� */
        s_curnode = NULL;
        if (stop) {                            
#if DEBUG_BIGLIST > 0
            LEE_LOG("free node, stopsend");
#endif
            udp_big_stopsend();                                       /* ֹͣ���� */
        }
        if (informer != NULL) {                                       /* ִ�лص�֪ͨ���� */
            informer(result);
        }
    }
}

/*******************************************************************
** ������:     big_list_informer
** ��������:   ���ݷ���֪ͨ����
** ����:       [in]  result:        ���ͽ��
**             [in]  ackcode:       ��Ӧ����
** ����:       ��
********************************************************************/
static void big_list_informer(SOCK_RESULT_E result, int ackcode)
{
#if DEBUG_BIGLIST > 0
        LEE_LOG("big_list_informer %d", result);
        printf("=========total_pkt=%d,cur_pktid=%d\n",s_curnode->pkt->total_pkt,s_curnode->pkt->cur_pktid);
#endif
    if (s_curnode != NULL && SOCK_RESULT_ACK == result) {
        if (s_curnode->pkt->cur_pktid == s_curnode->pkt->total_pkt){
            free_cur_big_node(false, RESULT_SUCCESS);                      /* �ѽ��յ�Ӧ��,�ͷŵ�ǰ���ͽڵ� */
        } else {
            if ((s_curnode->pkt->cur_pktid + 1)  == s_curnode->pkt->total_pkt){ /* ���һ�� */
                s_curnode->pkt->cur_size = s_curnode->pkt->file_length %PKT_DATA_SIZE;
                if (s_curnode->pkt->cur_size == 0){
                    s_curnode->pkt->cur_size = PKT_DATA_SIZE;
                }
            } else {
                s_curnode->pkt->cur_size = PKT_DATA_SIZE;
            }
            s_curnode->pkt->cur_pktid++;
            s_curnode->ct_time = 0;
            s_curnode->ct_send = 0;
            start_timer(s_scantmrid, 5, true);
        }
    } else {
        if (s_curnode != NULL) {
            s_curnode->ct_time = 2*s_curnode->max_time;                  /* ����ʧ��,�óɳ�ʱ, ��ʾ��Ҫ�ش� */
        }
    }
}

/*******************************************************************
** ������:     scan_timer_proc
** ��������:   ɨ�趨ʱ��������
** ����:       �� 
** ����:       ��
********************************************************************/
static void scan_timer_proc(void)
{
    start_timer(s_scantmrid, PERIOD_SCAN, true);
    if (get_list_item(&s_usedlist) == 0 && s_curnode == NULL) {        /* �������跢�͵����ݽڵ� */
        stop_timer(s_scantmrid);                                       /* ֹͣɨ�趨ʱ�� */
        return;
    }

    if (s_curnode == NULL) {                                          /* ��ǰ����ڵ㲻���� */
        s_curnode = (big_node_t *)del_list_head(&s_usedlist);         /* ��ȡ��ǰ����ڵ� */
        LEE_ASSERT(s_curnode != 0);
        s_curnode->ct_time = 0;                                       /* ��ʱ������ó�0 */
        s_curnode->ct_send = 0;
        s_curnode->file_send++;
    }

    if (s_curnode->ct_time == 0) {                                    /* ʱ�����Ϊ0, ��ʾ��Ҫ���� */
        if (!udp_big_send(s_curnode->pkt, big_list_informer)){
            free_cur_big_node(false, RESULT_NULL);
            return;
        }
    }
    if (++s_curnode->ct_time >= s_curnode->max_time) {                 /* �ѵ���ʱʱ�� */
#if DEBUG_BIGLIST > 0
        LEE_LOG("sendbig time overflow, reset");
#endif
        udp_big_reset();                                              /* ��λsocket */
        start_timer(s_scantmrid, 2*1000, true);                       /* ��λ����ʱ */
        if (++s_curnode->ct_send >= s_curnode->max_send) {            /* ���ʹ����Ѵﵽ�趨ֵ */
            free_cur_big_node(false, RESULT_OVER);
        } else {
            s_curnode->ct_time = 0;
        }
    }
}

/*******************************************************************
** ������:     apply_send_list
** ��������:   ������Э������
** ����:       [in]  keyword:       �����͵����ݰ��ؼ���, ͨ�����Э��������
**             [in]  pkt:           �����͵����ݰ�
**             [in]  over_time:     ָ�����ͳ�ʱʱ��(��λ:��)
**             [in]  max_send:      �ڽ��ղ���Ӧ������µ�����ʹ���
**             [in]  informer:      ���ͽ���ص�����
** ����:       ������ʧ��, �򷵻�false;  �ɹ�, �򷵻�һ����Ϊ0�ı�ʶ
** ע��:       1. ���ݰ�pkt���ڱ�ģ���ڲ������ͷ�, һ�������˸ýӿ�, ���۳ɹ���ʧ��, �����߶������ͷ�pkt;
**             2. ��over_timeΪ0, ���ʾ����ȱʡ��ʱʱ��(��15��)
**             3. ��max_sendΪ0, ���ʾ����ȱʡ���ʹ���(��3��);
********************************************************************/
bool_t apply_big_list(uint16_t keyword, big_packet_t *pkt, uint8_t over_time, uint8_t max_send, void (*informer)(RESULT_E))
{
    big_node_t   *node;

    LEE_ASSERT(pkt != NULL);
    
    node = (big_node_t *)del_list_head(&s_freelist);                  /* ������нڵ� */
    if (node == NULL) {                                               /* ����ʧ�� */
        big_packet_free(pkt);                                         /* �ͷ����ݰ��ռ���Դ */
        return false;                                                 /* ��ʾ������ʧ�� */
    }

    big_packet_assemble(pkt, keyword, s_session_id, PKT_DATA_SIZE);    /* �������֡ */
    if (over_time == 0){
        over_time = DEF_OVERTIME;
    }
    if (max_send == 0) {                                              /* ����ȱʡ���ʹ��� */
        max_send = DEF_SEND;
    }
    if (max_send > MAX_SEND) {
        max_send = MAX_SEND;
    }

    node->keyword  = keyword;
    node->max_time = over_time;
    node->max_send = max_send;
    node->file_send= 0; 
    node->pkt      = pkt;
    node->informer = informer;
    append_list_ele(&s_usedlist, node);

    s_session_id++;
    if (s_session_id == 0){
        s_session_id = 1;
    }
    if (!timer_is_run(s_scantmrid)) {
        start_timer(s_scantmrid, PERIOD_SCAN, true);
    }

    
#if DEBUG_BIGLIST > 0
    LEE_LOG("====  submit packet to send_list success ====");
#endif

    return true;                                                
}

/*******************************************************************
** ������:     cancel_send_list
** ��������:   ɾ�����������е�ָ���ڵ�
** ����:       [in]  keyword:    ɾ��������
** ����:       ��
********************************************************************/
void cancel_big_list(uint16_t keyword)
{
    big_node_t  *node, *next;

    if (s_curnode != NULL) {                                          /* �жϵ�ǰ�ڵ��Ƿ�����Ҫ�� */
        if (s_curnode->keyword == keyword) {
            free_cur_big_node(true, RESULT_NULL);
        }
    }

    node = (big_node_t *)get_list_head(&s_usedlist);                 /* �����������Ƿ�������������Ľڵ� */
    for (;;) {
        if (node == NULL) {
            break;
        }

        if (node->keyword == keyword) {
            big_packet_free(node->pkt);                              /* �ͷŽڵ��б�������ݰ� */
            node->pkt = NULL;
            next = (big_node_t *)del_list_ele(&s_usedlist, node);     /* �ӷ���������ɾ�� */
            append_list_ele(&s_freelist, node);                       /* ���ڵ�ҵ����������� */
            node = next;
        } else {
            node = (big_node_t *)get_list_next_ele(node);
        }
    }
}

/*******************************************************************
** ������:     init_send_list
** ��������:   ��ʼ������
** ����:       ��
** ����:       ��
********************************************************************/
void init_big_list(void)
{
    s_session_id  = 1;
    s_curnode = NULL;
    init_mem_list(&s_freelist, memblock, sizeof(memblock)/sizeof(memblock[0]), sizeof(memblock[0]));
    init_list(&s_usedlist);

    s_scantmrid    = install_timer(scan_timer_proc);
}


