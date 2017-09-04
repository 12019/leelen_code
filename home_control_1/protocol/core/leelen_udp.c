/********************************************************************************
**
** �ļ���:     leelen_udp.c
** ��Ȩ����:   (c) 2015 �������ֿƼ����޹�˾
** �ļ�����:   ʵ��ͨѶЭ������UDP����
**
*********************************************************************************
**             �޸���ʷ��¼
**===============================================================================
**| ����       | ����   |  �޸ļ�¼
**===============================================================================
**| 2015/09/28 | zzh    |  �������ļ�
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
* ��������
********************************************************************************
*/
#define DEBUG_UDP                       0 

#define MAX_MSG_LEN                     1024
#define UDP_PORT_17722                  17722
#define SEND_BUF_SIZE                   1500                    /* ���ͻ����� */
#define RECV_BUF_SIZE                   1500                    /* ���ջ����� */
#define DATA_BUF_SIZE                   1500                    /* һ�����ݰ�����󳤶� */

#define PERIOD_SCAN                     20                      /* ɨ�趨ʱ������, ��λ: ms */
#define MAX_CONNECT                     (10*1000/PERIOD_SCAN)   /* socket���ӳ�ʱʱ�� */
char remote_ip[30];
/*
********************************************************************************
* ����Э����մ���Ĳ���
********************************************************************************
*/
#define RECV_HEAD                       0
#define RECV_LENGTH                     1
#define RECV_DATA                       2

/*
********************************************************************************
* �������ݽṹ
********************************************************************************
*/
typedef struct {
    int             sock_id;                    /* sock */
    uint32_t        dest_ip;
    uint32_t        dest_port;
    bool_t          connected;                  /* ��ʶ�������֮��������Ƿ��ѽ��� */
    bool_t          sending;                    /* ��־�Ƿ��ڷ������ݻ��ѷ�����ϵȴ�Ӧ�� */
    uint8_t         send_buf[SEND_BUF_SIZE];    /* ���ͻ����� */
    int             send_len;                   /* �������ݳ��� */
    int             send_pos;                   /* ����λ�� */
    uint8_t         send_ack;                   /* Ӧ������� */ 
    uint8_t         ct_connect;                 /* ͳ�ƽ�������ʱ�� */
    uint8_t         recv_step;                  /* ���ݽ��ղ��� */
    int             recv_length;                /* ���ݳ��� */
    uint8_t         data_buf[DATA_BUF_SIZE];    /* ���ݽ��ջ����� */
    int             data_len;                   /* �ѽ��յ����ݳ��� */
    uint32_t        seq_id;                     /* ��ˮ�� */
    sock_informer   informer;                   /* ����֪ͨ���� */
} udp_t;

/*
********************************************************************************
* ����ģ��ֲ�����
********************************************************************************
*/
static udp_t      s_ucb;
static uint8_t    s_scantmrid;
static uint8_t    s_recv_buf[RECV_BUF_SIZE];

/*******************************************************************
** ������:     handle_recv_data
** ��������:   ����Э��Ĵ���
** ����:       [in]data:     ���յ������ݣ�
**             [in]data_len�����յ����ݵĳ���
**             [in]to��      �յ��Է������ߵ�ַ��Ϣ
** ����:       ����������ݳɹ�true;ʧ��false
** ע��:
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

    if (pkt.req_ack == LEE_ACK){                              /* ����������Ӧ��Э�� */
        if (pkt.session_id == s_ucb.seq_id){
            if (s_ucb.informer != NULL){
                s_ucb.informer(SOCK_RESULT_ACK, 0);
            }
        } else {
            if (s_ucb.informer != NULL){
                s_ucb.informer(SOCK_RESULT_NAK, 0);      /* ��ˮ�Ŵ��� */
            }
            return false;
        }
        result = req_hdlpkt(stream_getptr(&rstrm), data_len - stream_getlen(&rstrm), &pkt);
    } else if (pkt.req_ack == LEE_REQ){                      /* �յ������ֱ��Ӧ�� */
        result = req_hdlpkt(stream_getptr(&rstrm), data_len - stream_getlen(&rstrm), &pkt);
        if (result == 0){
            result = RESULT_NOTRANS;
        }
        param = req_hdlparam(pkt.cmd_type);              /* �ж��Ƿ�Ϊ�в�Ӧ�� */
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
        stream_writedata(&wstrm,  pkt.src,  sizeof(pkt.src)); /* Դ��ַ��ΪĿ�ĵ�ַ */
        stream_writebyte(&wstrm,  result);   /* 1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ���� */
        if (param && pkt.body_len > 0){
            stream_writedata(&wstrm, pkt.prot_body, pkt.body_len);
            if (pkt.prot_body != NULL){      /* �ͷ��в�Ӧ����Դ */
                LEE_FREE(pkt.prot_body);
            }
        }
        stream_writeshort(&wstrm, 0xffff);   /* Ԥ���ֽ���� */
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
** ������:     scan_tmr_proc
** ��������:   ɨ�趨ʱ��������
** ����:       ��
** ����:       ��
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
        //s_ucb.sock_id = (int)socket(AF_INET, SOCK_DGRAM, 0);    /* ����socket */
        s_ucb.sock_id = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); /* ����socket */
        if (s_ucb.sock_id != -1) { 
#if DEBUG_UDP > 0
            LEE_LOG("sock create success!");
#endif
            LEE_MEMSET(&sockaddr, 0, sizeof(sockaddr));
            sockaddr.sin_family = AF_INET;
            sockaddr.sin_addr.s_addr = htonl(INADDR_ANY); /* htonl ������ת�����޷��ų����͵������ֽ�˳�� */
            sockaddr.sin_port = htons(UDP_PORT_17722);

            if (bind(s_ucb.sock_id, (struct sockaddr*) &sockaddr, sizeof(sockaddr))!= 0) {
                close(s_ucb.sock_id);                   /* �ͷ�socket��Դ */
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
    if (!s_ucb.connected && s_ucb.send_len > 0) {        /* δ���ӳɹ� */

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
        if (e >= 0) {                                                    /* ���ӽ����ɹ� */
#if DEBUG_UDP > 0
            LEE_LOG("sock connect success!");
#endif
            s_ucb.connected = true;
            goto br_recv;
        } else if (e < 0) {                                             /* ���ӽ���ʧ�� */
#if DEBUG_UDP > 0
            LEE_LOG("sock can not connect!(e = %d)", e);
#endif
            close(s_ucb.sock_id);                                       /* �ͷ�socket��Դ */
            s_ucb.sock_id = -1;
            if (s_ucb.informer != NULL){
                s_ucb.informer(SOCK_RESULT_ERROR_SOCK, 0);
            }
        } else {                                                        /* ���ӻ��ڽ������� */
            if (++s_ucb.ct_connect >= MAX_CONNECT) {                    /* ���ӽ���ʱ�䳬ʱ */
#if DEBUG_UDP > 0
                LEE_LOG("sock connect time overflow!");
#endif
                close(s_ucb.sock_id);                                  /* �ͷ�socket��Դ */
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
    tv.tv_usec = 10000; // 10 ����
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
    if (recv_len > 0) {                                                 /* ���յ����� */
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
                    if (s_ucb.recv_step == RECV_HEAD) {             /* ����ͬ������ͷ */
                        if ( s_ucb.data_buf[s_ucb.data_len - 3] == SYNC_HEAD0
                          && s_ucb.data_buf[s_ucb.data_len - 2] == SYNC_HEAD1
                          && s_ucb.data_buf[s_ucb.data_len - 1] == SYNC_HEAD2) {
                            s_ucb.recv_step = RECV_LENGTH;         /* ת����հ��������ݳ��� */
                        }
                    } else if (s_ucb.recv_step == RECV_LENGTH) {    /* ���հ��������ݳ��� */
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
                        s_ucb.recv_step = RECV_DATA;                /* ת������������� */  
                        break;
                    } 
                }
            }
        }

        if (recv_len > 0) {
            if ((s_ucb.data_len + recv_len) > sizeof(s_ucb.data_buf)) { /* ��������̫�� */
                s_ucb.data_len  = 0;
                s_ucb.recv_step = RECV_HEAD;
                if (s_ucb.informer != NULL){
                    s_ucb.informer(SOCK_RESULT_NAK, 0);
                }
                goto br_send;
            }

            LEE_MEMCPY(&s_ucb.data_buf[s_ucb.data_len], ptr, recv_len);
            s_ucb.data_len += recv_len;
            if (s_ucb.data_len >= s_ucb.recv_length) {                  /* ���ݰ�������� */
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
        udp_reset();                                                /* ���ӱ��Ͽ����ͷ�socket��Դ */
        if (s_ucb.informer != NULL){
            s_ucb.informer(SOCK_RESULT_ERROR_SOCK, 0);
        }
        goto br_send;
    }

br_send:
    if (s_ucb.sock_id != -1 && s_ucb.connected) {
        if (s_ucb.send_len > 0) {                                       /* �ж��Ƿ��д����͵����� */
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
            if (send_len > 0) {                                         /* ���ݷ��ͳɹ� */
                if (send_len >= s_ucb.send_len) {                       /* ���ݰ�������� */
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
            } else {                                                    /* ���ݷ���ʧ��,��ζ���ӿ����Ѿ��Ͽ� */
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
** ������:     udp_is_sending
** ��������:   ��ѯudp�Ƿ��ڷ�������
** ����:       ��
** ����:       ��
********************************************************************/
bool_t udp_is_sending(void)
{
    return s_ucb.sending;
}

/*******************************************************************
** ������:     udp_reset
** ��������:   ��λsocket, ���Ѵ򿪵�socket�رյ� 
** ����:       ��
** ����:       ��
********************************************************************/
void udp_reset(void)
{
    if (s_ucb.sock_id != -1) {                                       /* �ر����� */
        close(s_ucb.sock_id);
        s_ucb.sock_id = -1;
    }

    s_ucb.sending   = false;
    s_ucb.send_len  = 0;
    s_ucb.recv_step = RECV_HEAD;
    s_ucb.data_len  = 0;

}

/*******************************************************************
** ������:     udp_stopsend
** ��������:   ֹͣ��������
** ����:       ��
** ����:       ��
********************************************************************/
void udp_stopsend(void)
{
    s_ucb.sending   = false;
    s_ucb.send_len  = 0;
    s_ucb.recv_step = RECV_HEAD;
}

/*******************************************************************
** ������:     udp_big_send
** ��������:   ���������ݰ�
** ����:       [in]  pkt:              �������ݰ�
**             [in]  sock_informer:    �ص�֪ͨ����
** ����:       true--�ɹ�;  false--ʧ��;
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
** ������:     init_udp
** ��������:   ģ���ʼ������
** ����:       ��
** ����:       ��
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
** ������:     uninit_udp
** ��������:   ģ��ȥʼ������
** ����:       ��
** ����:       ��
********************************************************************/
void uninit_udp(void)
{
    stop_timer(s_scantmrid);
}

