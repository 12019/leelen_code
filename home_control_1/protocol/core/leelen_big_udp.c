/********************************************************************************
**
** 文件名:     leelen_big_udp.c
** 版权所有:   (c) 2015 厦门立林科技有限公司
** 文件描述:   实现通讯协议大数据UDP发送
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
#include "leelen_stream.h"
#include "leelen_big_packet.h"
#include "leelen_big_udp.h"



/*
********************************************************************************
* 参数定义
********************************************************************************
*/
#define DEBUG_BIG_UDP                  0 

#define UDP_PORT_17723                  17723
#define SEND_BUF_SIZE                   1500                   /* 发送缓冲区 */
#define RECV_BUF_SIZE                   1500                   /* 接收缓冲区 */
#define DATA_BUF_SIZE                   1500                   /* 一个数据包的最大长度 */

#define PERIOD_SCAN                     15                      /* 扫描定时器周期, 单位: ms */

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
    uint8_t         recv_step;                  /* 数据接收步骤 */
    int             recv_length;                /* 数据长度 */
    uint8_t         data_buf[DATA_BUF_SIZE];    /* 数据接收缓冲区 */
    int             data_len;                   /* 已接收的数据长度 */
    uint32_t        seq_id;                     /* 流水号 */
    big_informer    informer;                   /* 发送通知函数 */
} udp_big_t;

/*
********************************************************************************
* 定义模块局部变量
********************************************************************************
*/
static udp_big_t  s_bcb;
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
    big_packet_t pkt;
    uint8_t result, msg[40], ipbuf[17];
    int err, msg_len;
    stream_t rstrm, wstrm;
    bool_t param;

    LEE_ASSERT(data != NULL || data_len != 0);

    if (getchksum(data, data_len) != 0){
        LEE_LOG("bigchecksum err");
        return false;
    }
    stream_init(&rstrm, data, data_len);
    stream_readdata(&rstrm, pkt.sync_head, sizeof(pkt.sync_head));
    if (pkt.sync_head[0] != SYNC_HEAD0 || pkt.sync_head[1] != SYNC_HEAD1 || pkt.sync_head[2] != SYNC_HEAD2){
        LEE_LOG("bigsynchead err");
        return false;
    }

    pkt.prot_ver = stream_readshort(&rstrm);
    pkt.cmd_type = stream_readshort(&rstrm);

    pkt.session_id= stream_readint(&rstrm);

    pkt.req_ack = stream_readbyte(&rstrm);
    pkt.encrypt = stream_readbyte(&rstrm);
    pkt.pkt_length = stream_readint(&rstrm);
    pkt.dest_ip    = htonl(to.sin_addr.s_addr);

#if DEBUG_BIG_UDP > 0
    iptostr(ipbuf, pkt.dest_ip);
    LEE_LOG("data_len %d %d %d %d destip:%s", data_len, pkt.pkt_length, pkt.session_id, s_bcb.seq_id, ipbuf);
#endif
    if (pkt.pkt_length != data_len){
        LEE_LOG("big_data_len not equal");
        return false;
    }
    stream_readdata(&rstrm, pkt.src,  sizeof(pkt.src)); 
    stream_readdata(&rstrm, pkt.dest, sizeof(pkt.dest));

    if (pkt.req_ack == LEE_ACK){                              /* 处理请求后的应答协议 */
        if (pkt.session_id == s_bcb.seq_id){
            if (s_bcb.informer != NULL){
                s_bcb.informer(SOCK_RESULT_ACK, 0);
            }
        } else {
            if (s_bcb.informer != NULL){
                s_bcb.informer(SOCK_RESULT_NAK, 0);      /* 流水号错误 */
            }
            return false;
        }
        //result = req_hdlpkt(stream_getptr(&rstrm), data_len - stream_getlen(&rstrm), &pkt);
        result = hdl_big_data_0x0601(stream_getptr(&rstrm), data_len - stream_getlen(&rstrm), &pkt);
    } else if (pkt.req_ack == LEE_REQ){                      /* 收到请求后直接应答 */
        //result = req_hdlpkt(stream_getptr(&rstrm), data_len - stream_getlen(&rstrm), &pkt);
        result = hdl_big_data_0x0601(stream_getptr(&rstrm), data_len - stream_getlen(&rstrm), &pkt);
        if (result == 0){
            result = RESULT_NOTRANS;
        }
        
        pkt.pkt_length = 30 + sizeof(result) + 4;        /* 含包序号 */
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
        stream_writebyte(&wstrm,  result);             /* 1成功;2错误;3不支持;4内存不足;5终止传输 */
        stream_writeint(&wstrm,   pkt.cur_pktid);      /* 包序号 */
        stream_writeshort(&wstrm, 0xffff);             /* 预留字节填充 */
        msg_len = stream_getlen(&wstrm);
        pkt.check_sum = getchksum_n(stream_getstartptr(&wstrm), msg_len);
        stream_writebyte(&wstrm,  pkt.check_sum);
        msg_len = stream_getlen(&wstrm);

        if (msg_len != pkt.pkt_length){
            LEE_LOG("big_pkt_len not equal");
            return false;
        }
        err = sendto(s_bcb.sock_id, (const void*) msg, msg_len, 0, (struct sockaddr *)&to, sizeof(struct sockaddr_in));
#if DEBUG_BIG_UDP > 0
        loghex("stream_bigack:", msg, msg_len);
        LEE_LOG("sendbig: %d ip:%s", err, inet_ntoa(to.sin_addr));
#endif        
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


    if (s_bcb.sock_id == -1) {
        s_bcb.connected  = false;
        s_bcb.sock_id = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); /* 创建socket */
        if (s_bcb.sock_id != -1) { 
#if DEBUG_BIG_UDP > 0
            LEE_LOG("sock create success!");
#endif
            LEE_MEMSET(&sockaddr, 0, sizeof(sockaddr));
            sockaddr.sin_family = AF_INET;
            sockaddr.sin_addr.s_addr = htonl(INADDR_ANY); /* htonl 主机数转换成无符号长整型的网络字节顺序 */
            sockaddr.sin_port = htons(UDP_PORT_17723);

            if (bind(s_bcb.sock_id, (struct sockaddr*) &sockaddr, sizeof(sockaddr))!= 0) {
                close(s_bcb.sock_id);                   /* 释放socket资源 */
                s_bcb.sock_id = -1;
            } else {
#if DEBUG_BIG_UDP > 0
                LEE_LOG("sock bind success");
#endif
                s_bcb.connected  = true;
                goto br_recv;
            }

        }
        return;
    }

br_recv:
    tv.tv_sec  = 0;
    tv.tv_usec = 10000; // 10 毫秒
    recv_len = 0;

    fcntl(s_bcb.sock_id, F_SETFL, O_NONBLOCK);

    FD_ZERO(&fdset);
    FD_SET(s_bcb.sock_id, &fdset);
    e = select(s_bcb.sock_id + 1, &fdset, NULL, NULL, &tv);
    sock_len = sizeof(sockaddr);
    LEE_MEMSET(&sockaddr, 0, sock_len);
    if (FD_ISSET(s_bcb.sock_id, &fdset)){
        recv_len = recvfrom(s_bcb.sock_id, (void *)s_recv_buf, sizeof(s_recv_buf), 0, (struct sockaddr *)&sockaddr, &sock_len);
#if DEBUG_BIG_UDP > 0
        LEE_LOG("sock recv %d", recv_len);
        loghex("rcv:", s_recv_buf, recv_len > 100 ? 100 : recv_len);
#endif        
    }
   
    if (recv_len > 0) {                                                 /* 接收到数据 */
        ptr = s_recv_buf;
        if (s_bcb.recv_step == RECV_HEAD || s_bcb.recv_step == RECV_LENGTH) {
            for (; recv_len > 0; ) {
                if (s_bcb.data_len >= sizeof(s_bcb.data_buf)) {
                    s_bcb.data_len  = 0;
                    s_bcb.recv_step = RECV_HEAD;
                }

                s_bcb.data_buf[s_bcb.data_len++] = *ptr++;
                recv_len--;

                if (s_bcb.data_len >= 3) {                              
                    if (s_bcb.recv_step == RECV_HEAD) {             /* 接收同步数据头 */
                        if ( s_bcb.data_buf[s_bcb.data_len - 3] == SYNC_HEAD0
                          && s_bcb.data_buf[s_bcb.data_len - 2] == SYNC_HEAD1
                          && s_bcb.data_buf[s_bcb.data_len - 1] == SYNC_HEAD2) {
                            s_bcb.recv_step = RECV_LENGTH;         /* 转入接收包数据内容长度 */
                        }
                    } else if (s_bcb.recv_step == RECV_LENGTH) {    /* 接收包数据内容长度 */
                        if (s_bcb.data_len < 17) {
                            continue;
                        }
                        s_bcb.recv_length = bytetoint(&s_bcb.data_buf[13]);
                        if (s_bcb.recv_length == 0 
                        ||  s_bcb.recv_length > sizeof(s_bcb.data_buf)) {
#if DEBUG_BIG_UDP > 0
                            LEE_LOG("data size is large(%d), can't recv!", s_bcb.recv_length);
#endif
                            s_bcb.data_len  = 0;
                            s_bcb.recv_step = RECV_HEAD;
                            if (s_bcb.informer != NULL){
                                s_bcb.informer(SOCK_RESULT_NAK, 0);
                            }
                            goto br_send;
                        }
                        s_bcb.recv_step = RECV_DATA;                /* 转入接收数据内容 */  
                        break;
                    } 
                }
            }
        }

        if (recv_len > 0) {
            if ((s_bcb.data_len + recv_len) > sizeof(s_bcb.data_buf)) { /* 数据内容太长 */
                s_bcb.data_len  = 0;
                s_bcb.recv_step = RECV_HEAD;
                if (s_bcb.informer != NULL){
                    s_bcb.informer(SOCK_RESULT_NAK, 0);
                }
                goto br_send;
            }

            LEE_MEMCPY(&s_bcb.data_buf[s_bcb.data_len], ptr, recv_len);
            s_bcb.data_len += recv_len;
            if (s_bcb.data_len >= s_bcb.recv_length) {                  /* 数据包接收完毕 */
                if (s_bcb.data_len >= 27) {
                    handle_recv_data(s_bcb.data_buf, s_bcb.data_len, sockaddr);
                } else {
                    if (s_bcb.informer != NULL){
                        s_bcb.informer(SOCK_RESULT_NAK, 0);
                    }
                }

                s_bcb.sending   = false;
                s_bcb.data_len  = 0;
                s_bcb.recv_step = RECV_HEAD;
            }
        }
    } else if (recv_len < 0) {
#if DEBUG_BIG_UDP > 0
        LEE_LOG("sock is disconnected!");
#endif
        udp_big_reset();                                                /* 连接被断开，释放socket资源 */
        if (s_bcb.informer != NULL){
            s_bcb.informer(SOCK_RESULT_ERROR_SOCK, 0);
        }
        goto br_send;
    }

br_send:
    if (s_bcb.sock_id != -1 && s_bcb.connected) {
        if (s_bcb.send_len > 0) {                                       /* 判断是否有待发送的数据 */
            to_len = sizeof(to);
            LEE_MEMSET(&to, 0, sizeof(to));
            to.sin_family = AF_INET;
            to.sin_addr.s_addr = htonl(s_bcb.dest_ip);
            to.sin_port   = htons(s_bcb.dest_port);
#if DEBUG_BIG_UDP > 0
            LEE_LOG("sendip: %s:%d %d",inet_ntoa(to.sin_addr), s_bcb.dest_port, s_bcb.sock_id);
#endif 
            send_len = sendto(s_bcb.sock_id, (char *)&s_bcb.send_buf[s_bcb.send_pos], (s_bcb.send_len > PKT_MAX_SIZE) ? PKT_MAX_SIZE : s_bcb.send_len, 0, (struct sockaddr *)&to, to_len);
            if (send_len > 0) {                                         /* 数据发送成功 */
                if (send_len >= s_bcb.send_len) {                       /* 数据包发送完毕 */
                    s_bcb.send_len  = 0;
                } else {
                    s_bcb.send_len -= send_len;
                    s_bcb.send_pos += send_len;
                }
#if DEBUG_BIG_UDP > 0
                LEE_LOG("send %d %d", send_len, s_bcb.send_len);
#endif
                if (s_bcb.send_len == 0 && s_bcb.send_ack == LEE_ACK){
                    if (s_bcb.informer != NULL){
                        s_bcb.informer(SOCK_RESULT_ACK, 0);
                    }
                }
            } else {                                                    /* 数据发送失败,意味连接可能已经断开 */
#if DEBUG_BIG_UDP > 0
                LEE_LOG("sock is disconnected!");
#endif
                udp_big_reset();
                if (s_bcb.informer != NULL){
                    s_bcb.informer(SOCK_RESULT_ERROR_SOCK, 0);
                }
            }
        }
    }
}

/*******************************************************************
** 函数名:     udp_big_sending
** 函数描述:   查询udp是否在发送数据
** 参数:       无
** 返回:       无
********************************************************************/
bool_t udp_big_sending(void)
{
    return s_bcb.sending;
}

/*******************************************************************
** 函数名:     udp_big_reset
** 函数描述:   复位socket, 将已打开的socket关闭掉 
** 参数:       无
** 返回:       无
********************************************************************/
void udp_big_reset(void)
{
    if (s_bcb.sock_id != -1) {                                       /* 关闭连接 */
        close(s_bcb.sock_id);
        s_bcb.sock_id = -1;
    }

    s_bcb.sending   = false;
    s_bcb.send_len  = 0;
    s_bcb.recv_step = RECV_HEAD;
    s_bcb.data_len  = 0;
}

/*******************************************************************
** 函数名:     udp_big_stopsend
** 函数描述:   停止继续发送
** 参数:       无
** 返回:       无
********************************************************************/
void udp_big_stopsend(void)
{
    s_bcb.sending   = false;
    s_bcb.send_len  = 0;
    s_bcb.recv_step = RECV_HEAD;
}

/*******************************************************************
** 函数名:     udp_big_send
** 函数描述:   请求发送数据包
** 参数:       [in]  pkt:             发送数据包
**             [in]  big_informer:    回调通知函数
** 返回:       true--成功;  false--失败;
********************************************************************/
bool_t udp_big_send(big_packet_t *pkt, void (* big_informer)(SOCK_RESULT_E result, int ackcode))
{
    int          rlen;
    stream_t     wstrm;
    uint32_t     offset, data_len;
    uint8_t      fullpath[120], dirpath_len;

    pkt->pkt_length = big_packet_head_length(pkt) + pkt->cur_size;
    if (pkt->pkt_length > sizeof(s_bcb.send_buf)) {
        LEE_LOG("data length too long");
        return false;
    }
    if ((pkt->dir_namelen + pkt->file_namelen + 2) > sizeof(fullpath)){
        LEE_LOG("file_namelen too long");
        return false;
    }
    stream_init(&wstrm, s_bcb.send_buf, sizeof(s_bcb.send_buf));
    stream_writedata(&wstrm,  pkt->sync_head, sizeof(pkt->sync_head));
    stream_writeshort(&wstrm, pkt->prot_ver);
    stream_writeshort(&wstrm, pkt->cmd_type);
    stream_writeint(&wstrm,   pkt->session_id);
    stream_writebyte(&wstrm,  pkt->req_ack);
    stream_writebyte(&wstrm,  pkt->encrypt);
    stream_writeint(&wstrm,   pkt->pkt_length);     /* 最大长度 */
    stream_writedata(&wstrm,  pkt->src,  sizeof(pkt->src));
    stream_writedata(&wstrm,  pkt->dest, sizeof(pkt->dest));
    
    stream_writebyte(&wstrm,  pkt->data_type);              
    stream_writebyte(&wstrm,  pkt->dir_namelen);
    stream_writedata(&wstrm,  pkt->dir_name, pkt->dir_namelen);
    stream_writebyte(&wstrm,  pkt->file_namelen);
    stream_writedata(&wstrm,  pkt->file_name, pkt->file_namelen);

    stream_writeint(&wstrm,   pkt->file_length);
    stream_writeint(&wstrm,   pkt->total_pkt);
    stream_writeint(&wstrm,   pkt->cur_pktid);
    stream_writeint(&wstrm,   pkt->cur_size);
    
    if (pkt->cur_size > 0){
        dirpath_len = pkt->dir_namelen;
        if (dirpath_len > 0){
            LEE_MEMCPY(fullpath, pkt->dir_name, pkt->dir_namelen);
            fullpath[dirpath_len] = '/';
            dirpath_len++;
        }
        LEE_MEMCPY(&fullpath[dirpath_len], pkt->file_name, pkt->file_namelen);
        fullpath[dirpath_len + pkt->file_namelen] = '\0';
        offset = (pkt->cur_pktid - 1) * PKT_DATA_SIZE;
#if DEBUG_BIG_UDP > 0
        LEE_LOG("fullpath: %s, %d, %d, %d,%d,%d", fullpath, offset, pkt->cur_size, pkt->pkt_length,pkt->total_pkt,pkt->cur_pktid);
#endif        
        data_len = read_big_data_0x0601(fullpath, offset, stream_getptr(&wstrm), pkt->cur_size);
        if (data_len != pkt->cur_size){
            LEE_LOG("read file err");
            return false;
        }
        stream_movptr(&wstrm, pkt->cur_size);
    } else {
        return false;
    }
    stream_writeshort(&wstrm, pkt->reserved);
    
    rlen = stream_getlen(&wstrm);
    pkt->check_sum = getchksum_n(stream_getstartptr(&wstrm), rlen);
    stream_writebyte(&wstrm,  pkt->check_sum);

    rlen = stream_getlen(&wstrm);
#if DEBUG_BIG_UDP > 0
    loghex("udp_send:", s_bcb.send_buf, rlen > 100 ? 100:rlen);
#endif    
    if (pkt->pkt_length != rlen){
        LEE_LOG("data length err");
        return false;
    }
    
    s_bcb.sending   = true;
    s_bcb.send_len  = rlen;
    s_bcb.send_pos  = 0;
    s_bcb.data_len  = 0;
    s_bcb.send_ack  = pkt->req_ack;
    s_bcb.seq_id    = pkt->session_id;
    s_bcb.dest_ip   = pkt->dest_ip;
    s_bcb.informer  = big_informer;
    s_bcb.dest_port = UDP_PORT_17723;
    s_bcb.recv_step = RECV_HEAD;

    if (!timer_is_run(s_scantmrid)) {
        start_timer(s_scantmrid, PERIOD_SCAN, true);
    }
    return true;
}


/*******************************************************************
** 函数名:     init_big_udp
** 函数描述:   模块初始化函数
** 参数:       无
** 返回:       无
********************************************************************/
void init_big_udp(void)
{
    s_bcb.sock_id    = -1;
    s_bcb.sending    = false;
    s_bcb.connected  = false;
    s_bcb.send_len   = 0;
    s_bcb.recv_step  = RECV_HEAD;
    s_bcb.data_len   = 0;

    s_scantmrid = install_timer(scan_tmr_proc);
    start_timer(s_scantmrid, PERIOD_SCAN, true);
}

/*******************************************************************
** 函数名:     uninit_big_udp
** 函数描述:   模块去始化函数
** 参数:       无
** 返回:       无
********************************************************************/
void uninit_big_udp(void)
{
    stop_timer(s_scantmrid);
}

