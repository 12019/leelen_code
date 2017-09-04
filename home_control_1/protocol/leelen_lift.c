/********************************************************************************
**
** �ļ���:     leelen_lift.c
** ��Ȩ����:   (c) 2015 �������ֿƼ����޹�˾
** �ļ�����:   ʵ���ݿ�ͨѶЭ�鹦��
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
#include "leelen_packet.h"
#include "leelen_hdlpkt.h"
#include "leelen_stream.h"
#include "leelen_packet.h"
#include "leelen_udp.h"
#include "leelen_protocol.h"
#include "leelen_security.h"
#include "leelen_talk.h"
#include "leelen_card.h"
#include "leelen_lift.h"
#include "leelen_system.h"
#include "leelen_file.h"
#include "leelen_interface.h"

/*
********************************************************************************
* ����ģ��ֲ�����
********************************************************************************
*/
static protoco_send_t  s_psb;

/*******************************************************************
** ������:     lift_informer
** ��������:   ���ݷ���֪ͨ����
** ����:       [in]  result:        ���ͽ��
** ����:       ��
********************************************************************/
static void lift_informer(RESULT_E result)
{
    
    if (s_psb.informer != NULL){
        s_psb.informer(result);
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;
}



/*******************************************************************
** ������:     start_lift_0x0201
** ��������:   ��ʼ����
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]dir:      ������Ϣ
**             [in]floor:    ��ʼ¥����Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_lift_0x0201(addr_info_t *addr, uint8_t dir, floor_info_t *floor, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;

    len = sizeof(dir) + sizeof(floor->src_floor);
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writebyte(&wstream, dir);               /* ���ݷ���  */
    stream_writedata(&wstream, floor->src_floor, sizeof(floor->src_floor)); 

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    
    return apply_send_list(0x0201, pkt, 0, 0, lift_informer);
}


/*******************************************************************
** ������:     stop_lift_0x0201
** ��������:   ֹͣ����
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_lift_0x0201(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0201);
    return true;
}


/*******************************************************************
** ������:     start_lift_0x0202
** ��������:   ���������ݿ�
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]floor:    ¥����Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_lift_0x0202(addr_info_t *addr, floor_info_t *floor, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;

    len = sizeof(floor->src_floor) + sizeof(floor->dest_floor);
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writedata(&wstream, floor->src_floor, sizeof(floor->src_floor)); 
    stream_writedata(&wstream, floor->dest_floor, sizeof(floor->dest_floor));

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0202, pkt, 0, 0, lift_informer);
}


/*******************************************************************
** ������:     stop_lift_0x0202
** ��������:   ֹͣ���������ݿ�
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_lift_0x0202(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0202);
    return true;
}


/*******************************************************************
** ������:     start_lift_0x0203
** ��������:   ˢ���ݿ�
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]floor:    ¥����Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_lift_0x0203(addr_info_t *addr, floor_info_t *floor, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;

    len = sizeof(floor->src_floor) + sizeof(floor->dest_floor);
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writedata(&wstream, floor->src_floor, sizeof(floor->src_floor)); 
    stream_writedata(&wstream, floor->dest_floor, sizeof(floor->dest_floor));

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0203, pkt, 0, 0, lift_informer);
}


/*******************************************************************
** ������:     stop_lift_0x0203
** ��������:   ֹͣˢ���ݿ�
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_lift_0x0203(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0203);
    return true;
}

/*******************************************************************
** ������:     start_lift_0x0204
** ��������:   ����״̬��ѯ
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_lift_0x0204(addr_info_t *addr, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;

    len = 0;
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0204, pkt, 0, 0, lift_informer);
}

/*******************************************************************
** ������:     stop_lift_0x0204
** ��������:   ֹͣ����״̬��ѯ
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_lift_0x0204(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0204);
    return true;
}

/*******************************************************************
** ������:     hdl_data_0x0201
** ��������:   ����Э��Ĵ���,����
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0201(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  result, dir, floor[5], floorstr[12];
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0201");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_lift_0x0201();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        
        dir = stream_readbyte(&rstrm);
        stream_readdata(&rstrm, floor, sizeof(floor));
        LEE_MEMSET(floorstr, 0, sizeof(floorstr));
        bytetoaddr(floor, floorstr);
        LEE_LOG("dir = %d s = %s", dir, floorstr);
        
        call_lift_0x0201(dir, floor);
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** ������:     hdl_data_0x0102
** ��������:   ����Э��Ĵ���,�ݿ�
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0202(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  src_floor[5], src_floorstr[12];
    uint8_t  dest_floor[5], dest_floorstr[12];
    uint8_t  result;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0202");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_lift_0x0202();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        
        stream_readdata(&rstrm, src_floor, sizeof(src_floor));
        stream_readdata(&rstrm, dest_floor, sizeof(dest_floor));

        LEE_MEMSET(src_floorstr, 0, sizeof(src_floorstr));
        bytetoaddr(src_floor, src_floorstr);

        LEE_MEMSET(dest_floorstr, 0, sizeof(dest_floorstr));
        bytetoaddr(dest_floor, dest_floorstr);

        call_lift_0x0202(src_floor, dest_floor);
        LEE_LOG("sfloor = %s dfloor = %s", src_floorstr, dest_floorstr);
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** ������:     hdl_data_0x0203
** ��������:   ����Э��Ĵ���,ˢ���ݿ�
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0203(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  src_floor[5], src_floorstr[12];
    uint8_t  dest_floor[5], dest_floorstr[12];
    uint8_t  result;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0203");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_lift_0x0203();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        
        stream_readdata(&rstrm, src_floor, sizeof(src_floor));
        stream_readdata(&rstrm, dest_floor, sizeof(dest_floor));
        
        LEE_MEMSET(src_floorstr, 0, sizeof(src_floorstr));
        bytetoaddr(src_floor, src_floorstr);

        LEE_MEMSET(dest_floorstr, 0, sizeof(dest_floorstr));
        bytetoaddr(dest_floor, dest_floorstr);
        call_lift_0x0203(src_floor, dest_floor);
        LEE_LOG("sfloor = %s dfloor = %s", src_floorstr, dest_floorstr);
        return RESULT_SUCCESS;
    }
    return 0;
}


/*******************************************************************
** ������:     hdl_data_0x0204
** ��������:   ����Э��Ĵ���,����״̬��ѯ
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0204(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm, wstrm;
    uint8_t  result, num, len, i;
    lift_info_t lift[5];
        
    LEE_ASSERT(data != NULL);
    
    LEE_LOG("hdl_data_0x0204");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        num    = stream_readbyte(&rstrm);
        num    = LEE_MIN(num, sizeof(lift)/sizeof(lift[0]));
        for (i = 0; i < num; i++){
            lift[i].state = stream_readbyte(&rstrm);
            lift[i].dir   = stream_readbyte(&rstrm);
            lift[i].floor = stream_readbyte(&rstrm);
            LEE_LOG("lift: %d %d %d", lift[i].state, lift[i].dir, lift[i].floor);
        }
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_lift_0x0204();
        set_lift_info_0x0204(lift, num);
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        num = sizeof(lift)/sizeof(lift[0]);
        
        get_lift_info_0x0204(lift, &num);
        len = sizeof(uint8_t);
        for (i = 0; i < num; i++){
            len += sizeof(lift->state);
            len += sizeof(lift->dir);
            len += sizeof(lift->floor);
        }
    
        pkt->body_len = len;
        pkt->prot_body = (uint8_t *)LEE_MALLOC(pkt->body_len);  /* ���ͺ��Զ��ͷ� */
        if (pkt->prot_body == NULL){     
            pkt->body_len  = 0;
            pkt->prot_body = NULL;
            return RESULT_NOMEM;
        }
        stream_init(&wstrm, pkt->prot_body, pkt->body_len);
        stream_writebyte(&wstrm, num);  
        for (i = 0; i < num; i++){
            stream_writebyte(&wstrm, lift[i].state); 
            stream_writebyte(&wstrm, lift[i].dir); 
            stream_writebyte(&wstrm, lift[i].floor); 
        }
        if (num == 0){
            return RESULT_NOSUPPORT;
        }
        return RESULT_SUCCESS;
    }
    return 0;
}

static handle_table_t  hdl_tab[] = { 0x0201, false, hdl_data_0x0201,
                                     0x0202, false, hdl_data_0x0202,
                                     0x0203, false, hdl_data_0x0203,
                                     0x0204, true,  hdl_data_0x0204
                                   };

/*******************************************************************
** ������:     init_lift
** ��������:   ģ���ʼ������
** ����:       ��
** ����:       ��
********************************************************************/
void init_lift(void)
{
    int i;

    s_psb.status   = 0;
    s_psb.informer =  NULL;

    for (i = 0; i < sizeof(hdl_tab)/sizeof(hdl_tab[0]); i++) {
        regist_pkt_handler(hdl_tab[i].cmd_type, hdl_tab[i].param, hdl_tab[i].handler);
    }

}






