/********************************************************************************
**
** 文件名:     leelen_send_list.c
** 版权所有:   (c) 2015 厦门立林科技有限公司
** 文件描述:   实现通讯协议数据缓冲发送队列
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
#include "leelen_packet.h"
#include "leelen_send_list.h"
#include "leelen_list.h"
#include "leelen_timer.h"
#include "leelen_udp.h"





/*
********************************************************************************
* 参数定义
********************************************************************************
*/
#define DEBUG_SENDLIST                      0

#define MAX_NODE                            20                      /* 允许缓冲的最大数据包个数 */

#define MAX_SEND                            10                      /* 最大重传的次数 */
#define PERIOD_SCAN                         100                     /* 扫描周期, 单位:ms */

#define DEF_OVERTIME                        (15*(1000/PERIOD_SCAN)) /* 缺省应答超时时间, 单位: 秒 至少要比socket连接超时时间长 */
#define DEF_SEND                            3                       /* 缺省发送次数 */


/*
********************************************************************************
* 定义数据包节点数据结构
********************************************************************************
*/
typedef struct {
    uint16_t      keyword;            /* 关键字 */
    uint8_t       ct_time;            /* 时间计数 */
    uint8_t       ct_send;            /* 已传送次数 */
    uint8_t       max_time;           /* 超时时间 */
    uint8_t       max_send;           /* 最多重传的次数 */
    packet_t      *pkt;               /* 数据分组 */
    void          (*informer)(RESULT_E);
} send_node_t;

/*
********************************************************************************
* 定义模块局部变量
********************************************************************************
*/
static struct {
    lee_node_t    res;
    send_node_t   node;
} memblock[MAX_NODE];

static uint32_t       s_session_id;
static lee_list_t     s_freelist, s_usedlist;
static send_node_t    *s_curnode;
static uint8_t        s_scantmrid;

/*******************************************************************
** 函数名:     free_cur_node
** 函数描述:   释放当前处理的节点
** 参数:       [in]  reset:     true--需进行复位处理
** 返回:       无
********************************************************************/
static void free_cur_node(bool_t reset, RESULT_E result)
{
    void (*informer)(RESULT_E);

    if (s_curnode != NULL) {
        informer = s_curnode->informer;
        packet_free(s_curnode->pkt);                                 /* 释放当前节点的数据包 */
        s_curnode->pkt = NULL;
        append_list_ele(&s_freelist, s_curnode);                     /* 将当前节点挂到空闲链表中 */
        s_curnode = NULL;
        if (reset && udp_is_sending()) {
#if DEBUG_SENDLIST > 0
            LEE_LOG("free node, reset");
#endif
            udp_stopsend(); 
        }
        if (informer != NULL && result != RESULT_SUCCESS) {         /* 执行回调通知函数 */
            informer(result);
        }
    }
}

/*******************************************************************
** 函数名:     send_informer
** 函数描述:   数据发送通知函数
** 参数:       [in]  result:        发送结果
**             [in]  ackcode:       响应代码
** 返回:       无
********************************************************************/
static void send_informer(SOCK_RESULT_E result, int ackcode)
{
#if DEBUG_SENDLIST > 0
        LEE_LOG("send_informer %d", result);
#endif

    if (SOCK_RESULT_ACK == result) {
        free_cur_node(false, RESULT_SUCCESS);                           /* 已接收到应答,释放当前发送节点 */
    } else {
        if (s_curnode != NULL) {
            s_curnode->ct_time = 2*s_curnode->max_time;                  /* 发送失败,置成超时, 表示需要重传 */
        }
    }
}

/*******************************************************************
** 函数名:     scan_timer_proc
** 函数描述:   扫描定时器处理函数
** 参数:       无 
** 返回:       无
********************************************************************/
static void scan_timer_proc(void)
{
    start_timer(s_scantmrid, PERIOD_SCAN, true);
    if (get_list_item(&s_usedlist) == 0 && s_curnode == NULL) {        /* 不存在需发送的数据节点 */
        stop_timer(s_scantmrid);                                       /* 停止扫描定时器 */
        return;
    }

    if (s_curnode == NULL) {                                          /* 当前处理节点不存在 */
        s_curnode = (send_node_t *)del_list_head(&s_usedlist);        /* 获取当前处理节点 */
        LEE_ASSERT(s_curnode != NULL);
        s_curnode->ct_time = 0;                                       /* 将时间计数置成0 */
        s_curnode->ct_send = 0;
    }

    if (s_curnode->ct_time == 0) {                                    /* 时间计数为0, 表示需要发送 */
        if (!udp_send(s_curnode->pkt, send_informer)){
            free_cur_node(false, RESULT_NULL);
            return;
        }
    }
    if (++s_curnode->ct_time >= s_curnode->max_time) {                 /* 已到超时时间 */
#if DEBUG_SENDLIST > 0
        LEE_LOG("send time overflow, reset");
#endif
        udp_reset();                                                  /* 复位socket */
        if (++s_curnode->ct_send >= s_curnode->max_send) {            /* 发送次数已达到设定值 */
            free_cur_node(false, RESULT_OVER);
        } else {
            s_curnode->ct_time = 0;
        }
    }
}

/*******************************************************************
** 函数名:     apply_send_list
** 函数描述:   请求发送协议数据
** 参数:       [in]  keyword:       请求发送的数据包关键字, 通常填充协议命令字
**             [in]  pkt:           待发送的数据包
**             [in]  over_time:     指定发送超时时间(单位:秒)
**             [in]  max_send:      在接收不到应答情况下的最大传送次数
**             [in]  informer:      发送结果回调函数
** 返回:       如请求失败, 则返回false;  成功, 则返回一个不为0的标识
** 注意:       1. 数据包pkt会在本模块内部进行释放, 一旦调用了该接口, 无论成功或失败, 调用者都无需释放pkt;
**             2. 如over_time为0, 则表示采用缺省超时时间(即15秒)
**             3. 如max_send为0, 则表示采用缺省发送次数(即3次);
********************************************************************/
bool_t apply_send_list(uint16_t keyword, packet_t *pkt, uint8_t over_time, uint8_t max_send, void (*informer)(RESULT_E))
{
    send_node_t   *node;

    LEE_ASSERT(pkt != NULL);
    
    node = (send_node_t *)del_list_head(&s_freelist);                 /* 申请空闲节点 */
    if (node == NULL) {                                               /* 申请失败 */
        packet_free(pkt);                                             /* 释放数据包空间资源 */
        return false;                                                 /* 表示请求发送失败 */
    }

    packet_assemble(pkt, keyword, s_session_id);                      /* 组成完整帧 */
    if (over_time == 0){
        over_time = DEF_OVERTIME;
    }
    if (max_send == 0) {                                              /* 采用缺省发送次数 */
        max_send = DEF_SEND;
    }
    if (max_send > MAX_SEND) {
        max_send = MAX_SEND;
    }

    node->keyword  = keyword;
    node->max_time  = over_time;
    node->max_send = max_send;
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

    
#if DEBUG_SENDLIST > 0
    LEE_LOG("====  submit packet to send_list success ====");
#endif

    return true;                                                
}

/*******************************************************************
** 函数名:     cancel_send_list
** 函数描述:   删除发送链表中的指定节点
** 参数:       [in]  keyword:    删除命令字
** 返回:       无
********************************************************************/
void cancel_send_list(uint16_t keyword)
{
    send_node_t  *node, *next;

    if (s_curnode != NULL) {                                            /* 判断当前节点是否满足要求 */
        if (s_curnode->keyword == keyword) {
            free_cur_node(true, RESULT_NULL);
        }
    }

    node = (send_node_t *)get_list_head(&s_usedlist);                 /* 遍历链表中是否存在满足条件的节点 */
    for (;;) {
        if (node == NULL) {
            break;
        }

        if (node->keyword == keyword) {
            packet_free(node->pkt);                                   /* 释放节点中保存的数据包 */
            node->pkt = NULL;
            next = (send_node_t *)del_list_ele(&s_usedlist, node);    /* 从发送链表中删除 */
            append_list_ele(&s_freelist, node);                       /* 将节点挂到空闲链表上 */
            node = next;
        } else {
            node = (send_node_t *)get_list_next_ele(node);
        }
    }
}

/*******************************************************************
** 函数名:     init_send_list
** 函数描述:   初始化函数
** 参数:       无
** 返回:       无
********************************************************************/
void init_send_list(void)
{
    s_session_id  = 1;
    s_curnode = NULL;
    init_mem_list(&s_freelist, memblock, sizeof(memblock)/sizeof(memblock[0]), sizeof(memblock[0]));
    init_list(&s_usedlist);

    s_scantmrid    = install_timer(scan_timer_proc);
}


