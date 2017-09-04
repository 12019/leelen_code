/********************************************************************************
**
** 文件名:     leelen_udp.c
** 版权所有:   (c) 2015 厦门立林科技有限公司
** 文件描述:   实现通讯协议数据UDP发送
**
*********************************************************************************
**             修改历史记录
**===============================================================================
**| 日期       | 作者   |  修改记录
**===============================================================================
**| 2015/09/28 | zzh    |  创建该文件
**
*********************************************************************************/



#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <fcntl.h>


#include "leelen_comdef.h"
#include "leelen_packet.h"
#include "leelen_hdlpkt.h"
#include "leelen_stream.h"
#include "leelen_udp.h"


/*
********************************************************************************
* 参数定义
********************************************************************************
*/
#define DEBUG_UDP                       0 

#define MAX_MSG_LEN                     1024
#define UDP_PORT_17722                  17722
#define SEND_BUF_SIZE                   1500                    /* 发送缓冲区 */
#define RECV_BUF_SIZE                   1500                    /* 接收缓冲区 */
#define DATA_BUF_SIZE                   1500                    /* 一个数据包的最大长度 */

#define PERIOD_SCAN                     20                      /* 扫描定时器周期, 单位: ms */
#define MAX_CONNECT                     (10*1000/PERIOD_SCAN)   /* socket连接超时时间 */
char remote_ip[30];
/*
********************************************************************************
* 定义协议接收处理的步骤
********************************************************************************
*/
#define RECV_HEAD                       0
#define RECV_LENGTH                     1
#define RECV_DATA                       2

/*
********************************************************************************
* 定义数据结构
********************************************************************************
*/
typedef struct {
    int             sock_id;                    /* sock */
    uint32_t        dest_ip;
    uint32_t        dest_port;
    bool_t          connected;                  /* 标识与服务器之间的连接是否已建立 */
    bool_t          sending;                    /* 标志是否在发送数据或已发送完毕等待应答 */
    uint8_t         send_buf[SEND_BUF_SIZE];    /* 发送缓冲区 */
    int             send_len;                   /* 发送数据长度 */
    int             send_pos;                   /* 发送位置 */
    uint8_t         send_ack;                   /* 应答包发送 */ 
    uint8_t         ct_connect;                 /* 统计建立连接时间 */
    uint8_t         recv_step;                  /* 数据接收步骤 */
    int             recv_length;                /* 数据长度 */
    uint8_t         data_buf[DATA_BUF_SIZE];    /* 数据接收缓冲区 */
    int             data_len;                   /* 已接收的数据长度 */
    uint32_t        seq_id;                     /* 流水号 */
    sock_informer   informer;                   /* 发送通知函数 */
} udp_t;

/*
********************************************************************************
* 定义模块局部变量
********************************************************************************
*/
static udp_t      s_ucb;
static uint8_t    s_scantmrid;
static uint8_t    s_recv_buf[RECV_BUF_SIZE];

/*******************************************************************
** 函数名:     handle_recv_data
** 函数描述:   请求协议的处理
** 参数:       [in]data:     接收到的数据；
**             [in]data_len：接收到数据的长度
**             [in]to：      收到对方数据者地址信息
** 返回:       处理接收数据成功true;失败false
** 注意:
********************************************************************/
static bool_t handle_recv_data(uint8_t *data, int data_len, struct sockaddr_in to)
{
    packet_t pkt;
    uint8_t result, *msg;
    int err, msg_len;
    stream_t rstrm, wstrm;
    bool_t param;

    LEE_ASSERT(data != NULL || data_len != 0);

    if (getchksum(data, data_len) != 0){
        LEE_LOG("checksum err");
        return false;
    }
    stream_init(&rstrm, data, data_len);
    stream_readdata(&rstrm, pkt.sync_head, sizeof(pkt.sync_head));
    if (pkt.sync_head[0] != SYNC_HEAD0 || pkt.sync_head[1] != SYNC_HEAD1 || pkt.sync_head[2] != SYNC_HEAD2){
        LEE_LOG("synchead err");
        return false;
    }

    memset(remote_ip,'\0',sizeof(remote_ip));
    memcpy(remote_ip,inet_ntoa(to.sin_addr),strlen(inet_ntoa(to.sin_addr)));

    pkt.prot_ver = stream_readshort(&rstrm);
    pkt.cmd_type = stream_readshort(&rstrm);

    pkt.session_id= stream_readint(&rstrm);

    pkt.req_ack = stream_readbyte(&rstrm);
    pkt.encrypt = stream_readbyte(&rstrm);
    pkt.pkt_length = stream_readint(&rstrm);
#if DEBUG_UDP > 0
    LEE_LOG("data_len %d %d %d %d destip:%x", data_len, pkt.pkt_length, pkt.session_id, s_ucb.seq_id, pkt.dest_ip);
#endif

    if (pkt.pkt_length != data_len){
        LEE_LOG("data_len not equal");
        return false;
    }
    stream_readdata(&rstrm, pkt.src,  sizeof(pkt.src));
    stream_readdata(&rstrm, pkt.dest, sizeof(pkt.dest));
    pkt.dest_ip    = htonl(to.sin_addr.s_addr);

    if (pkt.req_ack == LEE_ACK){                              /* 处理请求后的应答协议 */
        if (pkt.session_id == s_ucb.seq_id){
            if (s_ucb.informer != NULL){
                s_ucb.informer(SOCK_RESULT_ACK, 0);
            }
        } else {
            if (s_ucb.informer != NULL){
                s_ucb.informer(SOCK_RESULT_NAK, 0);      /* 流水号错误 */
            }
            return false;
        }
        result = req_hdlpkt(stream_getptr(&rstrm), data_len - stream_getlen(&rstrm), &pkt);
    } else if (pkt.req_ack == LEE_REQ){                      /* 收到请求后直接应答 */
        result = req_hdlpkt(stream_getptr(&rstrm), data_len - stream_getlen(&rstrm), &pkt);
        if (result == 0){
            result = RESULT_NOTRANS;
        }
        param = req_hdlparam(pkt.cmd_type);              /* 判断是否为有参应答 */
        if(param){
            pkt.pkt_length = packet_head_length(&pkt) + sizeof(result) + pkt.body_len;
        } else {
            pkt.pkt_length = packet_head_length(&pkt) + sizeof(result);
        }
        msg = (uint8_t *)LEE_MALLOC(pkt.pkt_length);
        if (msg == NULL){
            return false;
        }
        stream_init(&wstrm, msg,  pkt.pkt_length);
        stream_writedata(&wstrm,  pkt.sync_head, sizeof(pkt.sync_head));
        stream_writeshort(&wstrm, pkt.prot_ver);
        stream_writeshort(&wstrm, pkt.cmd_type);
        stream_writeint(&wstrm,   pkt.session_id);
        stream_writebyte(&wstrm,  LEE_ACK);
        stream_writebyte(&wstrm,  pkt.encrypt);

        stream_writeint(&wstrm,   pkt.pkt_length);
        stream_writedata(&wstrm,  pkt.dest, sizeof(pkt.dest)); 
        stream_writedata(&wstrm,  pkt.src,  sizeof(pkt.src)); /* 源地址作为目的地址 */
        stream_writebyte(&wstrm,  result);   /* 1成功;2错误;3不支持;4内存不足;5终止传输 */
        if (param && pkt.body_len > 0){
            stream_writedata(&wstrm, pkt.prot_body, pkt.body_len);
            if (pkt.prot_body != NULL){      /* 释放有参应答资源 */
                LEE_FREE(pkt.prot_body);
            }
        }
        stream_writeshort(&wstrm, 0xffff);   /* 预留字节填充 */
        msg_len = stream_getlen(&wstrm);
        pkt.check_sum = getchksum_n(stream_getstartptr(&wstrm), msg_len);
        stream_writebyte(&wstrm,  pkt.check_sum);
        msg_len = stream_getlen(&wstrm);

        if (msg_len != pkt.pkt_length){
            LEE_LOG("pkt_length not equal");
            if (msg != NULL){
                LEE_FREE(msg);
            }
            return false;
        }
        err = sendto(s_ucb.sock_id, (const void*) msg, msg_len, 0, (struct sockaddr *)&to, sizeof(struct sockaddr_in));
#if DEBUG_UDP > 0
        loghex("stream_ack:", msg, msg_len);
        LEE_LOG("sendbig: %d ip:%s", err, inet_ntoa(to.sin_addr));
#endif        
        if (msg != NULL){
            LEE_FREE(msg);
        }
    }

    return true;
}

/*******************************************************************
** 函数名:     scan_tmr_proc
** 函数描述:   扫描定时器处理函数
** 参数:       无
** 返回:       无
********************************************************************/
static void scan_tmr_proc(void)
{
    struct sockaddr_in sockaddr, to;
    struct timeval tv;
    fd_set fdset; 
    int    recv_len, send_len, sock_len, to_len, e;
    uint8_t *ptr;


    if (s_ucb.sock_id == -1) {
        s_ucb.connected  = false;
        //s_ucb.sock_id = (int)socket(AF_INET, SOCK_DGRAM, 0);    /* 创建socket */
        s_ucb.sock_id = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); /* 创建socket */
        if (s_ucb.sock_id != -1) { 
#if DEBUG_UDP > 0
            LEE_LOG("sock create success!");
#endif
            LEE_MEMSET(&sockaddr, 0, sizeof(sockaddr));
            sockaddr.sin_family = AF_INET;
            sockaddr.sin_addr.s_addr = htonl(INADDR_ANY); /* htonl 主机数转换成无符号长整型的网络字节顺序 */
            sockaddr.sin_port = htons(UDP_PORT_17722);

            if (bind(s_ucb.sock_id, (struct sockaddr*) &sockaddr, sizeof(sockaddr))!= 0) {
                close(s_ucb.sock_id);                   /* 释放socket资源 */
                s_ucb.sock_id = -1;
            } else {
#if DEBUG_UDP > 0
                LEE_LOG("sock bind success");
#endif
                s_ucb.ct_connect = 0;
                s_ucb.connected  = true;
                goto br_send;
            }

        }
        LEE_LOG("sock create %d", s_ucb.sock_id);
        return;
    }


#if 0
    if (!s_ucb.connected && s_ucb.send_len > 0) {        /* 未连接成功 */

        LEE_MEMSET(&to, 0, sizeof(to));
        to.sin_family = AF_INET;
        to.sin_addr.s_addr = htonl(s_ucb.dest_ip);
        //to.sin_addr.s_addr = inet_addr(s_ucb.dest_ip);
        to.sin_port   = htons(s_ucb.dest_port);
#if DEBUG_UDP > 0
        char        ip[17];
        iptostr(ip, s_ucb.dest_ip);
        LEE_LOG("ip: %s:%d %d",ip, s_ucb.dest_port, s_ucb.sock_id);
#endif 
        e = connect(s_ucb.sock_id, (struct sockaddr *)&to, sizeof(to));
        if (e >= 0) {                                                    /* 连接建立成功 */
#if DEBUG_UDP > 0
            LEE_LOG("sock connect success!");
#endif
            s_ucb.connected = true;
            goto br_recv;
        } else if (e < 0) {                                             /* 连接建立失败 */
#if DEBUG_UDP > 0
            LEE_LOG("sock can not connect!(e = %d)", e);
#endif
            close(s_ucb.sock_id);                                       /* 释放socket资源 */
            s_ucb.sock_id = -1;
            if (s_ucb.informer != NULL){
                s_ucb.informer(SOCK_RESULT_ERROR_SOCK, 0);
            }
        } else {                                                        /* 连接还在建立当中 */
            if (++s_ucb.ct_connect >= MAX_CONNECT) {                    /* 连接建立时间超时 */
#if DEBUG_UDP > 0
                LEE_LOG("sock connect time overflow!");
#endif
                close(s_ucb.sock_id);                                  /* 释放socket资源 */
                s_ucb.sock_id = -1;
                if (s_ucb.informer != NULL){
                    s_ucb.informer(SOCK_RESULT_ERROR_SOCK, 0);
                }
            }
        }
        return;
    }
#endif

br_recv:

    tv.tv_sec  = 0;
    tv.tv_usec = 10000; // 10 毫秒
    recv_len = 0;

    fcntl(s_ucb.sock_id, F_SETFL, O_NONBLOCK);

    FD_ZERO(&fdset);
    FD_SET(s_ucb.sock_id, &fdset);
    e = select(s_ucb.sock_id + 1, &fdset, NULL, NULL, &tv);
    sock_len = sizeof(sockaddr);
    LEE_MEMSET(&sockaddr, 0, sock_len);
    //recv_len   = recv(s_ucb.sock_id, (void *)s_recv_buf, sizeof(s_recv_buf), 0);
    if (FD_ISSET(s_ucb.sock_id, &fdset)){
        recv_len = recvfrom(s_ucb.sock_id, (void *)s_recv_buf, sizeof(s_recv_buf), 0, (struct sockaddr *)&sockaddr, &sock_len);
#if DEBUG_UDP > 0
        LEE_LOG("sock recv %d", recv_len);
        loghex("rcv:", s_recv_buf, recv_len);
#endif        
    }
    if (recv_len > 0) {                                                 /* 接收到数据 */
        ptr = s_recv_buf;
        if (s_ucb.recv_step == RECV_HEAD || s_ucb.recv_step == RECV_LENGTH) {
            for (; recv_len > 0; ) {
                if (s_ucb.data_len >= sizeof(s_ucb.data_buf)) {
                    s_ucb.data_len  = 0;
                    s_ucb.recv_step = RECV_HEAD;
                }

                s_ucb.data_buf[s_ucb.data_len++] = *ptr++;
                recv_len--;

                if (s_ucb.data_len >= 3) {                              
                    if (s_ucb.recv_step == RECV_HEAD) {             /* 接收同步数据头 */
                        if ( s_ucb.data_buf[s_ucb.data_len - 3] == SYNC_HEAD0
                          && s_ucb.data_buf[s_ucb.data_len - 2] == SYNC_HEAD1
                          && s_ucb.data_buf[s_ucb.data_len - 1] == SYNC_HEAD2) {
                            s_ucb.recv_step = RECV_LENGTH;         /* 转入接收包数据内容长度 */
                        }
                    } else if (s_ucb.recv_step == RECV_LENGTH) {    /* 接收包数据内容长度 */
                        if (s_ucb.data_len < 17) {
                            continue;
                        }
                        s_ucb.recv_length = bytetoint(&s_ucb.data_buf[13]);
                        if (s_ucb.recv_length == 0 
                        ||  s_ucb.recv_length > sizeof(s_ucb.data_buf)) {
#if DEBUG_UDP > 0
                            LEE_LOG("data size is large(%d), can't recv!", s_ucb.recv_length);
#endif
                            s_ucb.data_len  = 0;
                            s_ucb.recv_step = RECV_HEAD;
                            if (s_ucb.informer != NULL){
                                s_ucb.informer(SOCK_RESULT_NAK, 0);
                            }
                            goto br_send;
                        }
                        s_ucb.recv_step = RECV_DATA;                /* 转入接收数据内容 */  
                        break;
                    } 
                }
            }
        }

        if (recv_len > 0) {
            if ((s_ucb.data_len + recv_len) > sizeof(s_ucb.data_buf)) { /* 数据内容太长 */
                s_ucb.data_len  = 0;
                s_ucb.recv_step = RECV_HEAD;
                if (s_ucb.informer != NULL){
                    s_ucb.informer(SOCK_RESULT_NAK, 0);
                }
                goto br_send;
            }

            LEE_MEMCPY(&s_ucb.data_buf[s_ucb.data_len], ptr, recv_len);
            s_ucb.data_len += recv_len;
            if (s_ucb.data_len >= s_ucb.recv_length) {                  /* 数据包接收完毕 */
                if (s_ucb.data_len >= 27) {
                    handle_recv_data(s_ucb.data_buf, s_ucb.data_len, sockaddr);
                } else {
                    if (s_ucb.informer != NULL){
                        s_ucb.informer(SOCK_RESULT_NAK, 0);
                    }
                }

                s_ucb.sending   = false;
                s_ucb.data_len  = 0;
                s_ucb.recv_step = RECV_HEAD;
            }
        }
    } else if (recv_len < 0) {
#if DEBUG_UDP > 0
        LEE_LOG("sock is disconnected!");
#endif
        udp_reset();                                                /* 连接被断开，释放socket资源 */
        if (s_ucb.informer != NULL){
            s_ucb.informer(SOCK_RESULT_ERROR_SOCK, 0);
        }
        goto br_send;
    }

br_send:
    if (s_ucb.sock_id != -1 && s_ucb.connected) {
        if (s_ucb.send_len > 0) {                                       /* 判断是否有待发送的数据 */
            to_len = sizeof(to);
            LEE_MEMSET(&to, 0, sizeof(to));
            to.sin_family = AF_INET;
            to.sin_addr.s_addr = htonl(s_ucb.dest_ip);
            to.sin_port   = htons(s_ucb.dest_port);
#if DEBUG_UDP > 0
            LEE_LOG("ip: %s:%d %d",inet_ntoa(to.sin_addr), s_ucb.dest_port, s_ucb.sock_id);
#endif 
            send_len = sendto(s_ucb.sock_id, (char *)&s_ucb.send_buf[s_ucb.send_pos], (s_ucb.send_len > PKT_MAX_SIZE) ? PKT_MAX_SIZE : s_ucb.send_len, 0, (struct sockaddr *)&to, to_len);
            //send_len = send(s_ucb.sock_id, (char *)&s_ucb.send_buf[s_ucb.send_pos], (s_ucb.send_len > 1024) ? 1024 : s_ucb.send_len, 0);
            if (send_len > 0) {                                         /* 数据发送成功 */
                if (send_len >= s_ucb.send_len) {                       /* 数据包发送完毕 */
                    s_ucb.send_len  = 0;
                } else {
                    s_ucb.send_len -= send_len;
                    s_ucb.send_pos += send_len;
                }
#if DEBUG_UDP > 0
                LEE_LOG("send %d %d", send_len, s_ucb.send_len);
#endif
                if (s_ucb.send_len == 0 && s_ucb.send_ack == LEE_ACK){
                    if (s_ucb.informer != NULL){
                        s_ucb.informer(SOCK_RESULT_ACK, 0);
                    }
                }
            } else {                                                    /* 数据发送失败,意味连接可能已经断开 */
#if DEBUG_UDP > 0
                LEE_LOG("sock is disconnected!");
#endif
                udp_reset();
                if (s_ucb.informer != NULL){
                    s_ucb.informer(SOCK_RESULT_ERROR_SOCK, 0);
                }
            }
        }
    }
}

/*******************************************************************
** 函数名:     udp_is_sending
** 函数描述:   查询udp是否在发送数据
** 参数:       无
** 返回:       无
********************************************************************/
bool_t udp_is_sending(void)
{
    return s_ucb.sending;
}

/*******************************************************************
** 函数名:     udp_reset
** 函数描述:   复位socket, 将已打开的socket关闭掉 
** 参数:       无
** 返回:       无
********************************************************************/
void udp_reset(void)
{
    if (s_ucb.sock_id != -1) {                                       /* 关闭连接 */
        close(s_ucb.sock_id);
        s_ucb.sock_id = -1;
    }

    s_ucb.sending   = false;
    s_ucb.send_len  = 0;
    s_ucb.recv_step = RECV_HEAD;
    s_ucb.data_len  = 0;

}

/*******************************************************************
** 函数名:     udp_stopsend
** 函数描述:   停止继续发送
** 参数:       无
** 返回:       无
********************************************************************/
void udp_stopsend(void)
{
    s_ucb.sending   = false;
    s_ucb.send_len  = 0;
    s_ucb.recv_step = RECV_HEAD;
}

/*******************************************************************
** 函数名:     udp_big_send
** 函数描述:   请求发送数据包
** 参数:       [in]  pkt:              发送数据包
**             [in]  sock_informer:    回调通知函数
** 返回:       true--成功;  false--失败;
********************************************************************/
bool_t udp_send(packet_t *pkt, void (* sock_informer)(SOCK_RESULT_E result, int ackcode))
{
    int          rlen;
    stream_t     wstrm;

    if (pkt->pkt_length > sizeof(s_ucb.send_buf)) {
        LEE_LOG("data length too long");
        return false;
    }
    stream_init(&wstrm, s_ucb.send_buf, sizeof(s_ucb.send_buf));
    stream_writedata(&wstrm,  pkt->sync_head, sizeof(pkt->sync_head));
    stream_writeshort(&wstrm, pkt->prot_ver);
    stream_writeshort(&wstrm, pkt->cmd_type);
    stream_writeint(&wstrm,   pkt->session_id);
    stream_writebyte(&wstrm,  pkt->req_ack);
    stream_writebyte(&wstrm,  pkt->encrypt);
    stream_writeint(&wstrm,   pkt->pkt_length);
    stream_writedata(&wstrm,  pkt->src,  sizeof(pkt->src));
    stream_writedata(&wstrm,  pkt->dest, sizeof(pkt->dest));
    if (pkt->body_len > 0){
        stream_writedata(&wstrm,  pkt->prot_body, pkt->body_len);
    }
    stream_writeshort(&wstrm, pkt->reserved);
    
    rlen = stream_getlen(&wstrm);
    pkt->check_sum = getchksum_n(stream_getstartptr(&wstrm), rlen);
    stream_writebyte(&wstrm,  pkt->check_sum);

    rlen = stream_getlen(&wstrm);
#if DEBUG_UDP > 0
    LEE_LOG("len: %d, %d", rlen, pkt->pkt_length);
    loghex("udp_send:", s_ucb.send_buf, rlen);
#endif    
    if (pkt->pkt_length != rlen){
        LEE_LOG("data length err");
        return false;
    }

    s_ucb.sending   = true;
    s_ucb.send_len  = rlen;
    s_ucb.send_pos  = 0;
    s_ucb.data_len  = 0;
    s_ucb.send_ack  = pkt->req_ack;
    s_ucb.seq_id    = pkt->session_id;
    s_ucb.dest_ip   = pkt->dest_ip;
    s_ucb.informer  = sock_informer;
    s_ucb.dest_port = UDP_PORT_17722; //pkt->dest_port;
    s_ucb.recv_step = RECV_HEAD;


    if (!timer_is_run(s_scantmrid)) {
        start_timer(s_scantmrid, PERIOD_SCAN, true);
    }

    return true;
}

/*******************************************************************
** 函数名:     init_udp
** 函数描述:   模块初始化函数
** 参数:       无
** 返回:       无
********************************************************************/
void init_udp(void)
{
    s_ucb.sock_id    = -1;
    s_ucb.sending    = false;
    s_ucb.connected  = false;
    s_ucb.send_len   = 0;
    s_ucb.recv_step  = RECV_HEAD;
    s_ucb.data_len   = 0;

    s_scantmrid = install_timer(scan_tmr_proc);
    start_timer(s_scantmrid, PERIOD_SCAN, true);
}

/*******************************************************************
** 函数名:     uninit_udp
** 函数描述:   模块去始化函数
** 参数:       无
** 返回:       无
********************************************************************/
void uninit_udp(void)
{
    stop_timer(s_scantmrid);
}

