/********************************************************************************
**
** �ļ���:     leelen_security.c
** ��Ȩ����:   (c) 2015 �������ֿƼ����޹�˾
** �ļ�����:   ʵ�ְ������ͨѶЭ�鹦��
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
** ������:     security_informer
** ��������:   ���ݷ���֪ͨ����
** ����:       [in]  result:        ���ͽ��
** ����:       ��
********************************************************************/
static void security_informer(RESULT_E result)
{
    
    if (s_psb.informer != NULL){
        s_psb.informer(result);
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;
}

/*******************************************************************
** ������:     start_security_0x0001
** ��������:   ��������¼�ϴ�
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]type��    ��������
**             [in]arrays��  ��������
**             [in]rec��     ��������¼
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_security_0x0001(addr_info_t *addr, uint8_t type, uint8_t arrays, security_record_t *rec, void (*informer)(RESULT_E))
{
    uint8_t   i;
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;

    len = arrays*sizeof(rec) + sizeof(type) + sizeof(arrays);
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writebyte(&wstream, type);               /* ��������  */
    stream_writebyte(&wstream, arrays);             /* ��������  */
    for (i = 0; i < arrays; i++){
        stream_writebyte(&wstream, rec[i].attrib); 
        stream_writebyte(&wstream, rec[i].seq); 
        stream_writebyte(&wstream, rec[i].state); 
    }
    
    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0001, pkt, 0, 0, security_informer);
}

/*******************************************************************
** ������:     stop_security_0x0001
** ��������:   ��������¼�ϴ�
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_security_0x0001(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;
    cancel_send_list(0x0001);
    return true;
}


/*******************************************************************
** ������:     start_security_0x0002
** ��������:   Զ�̲�����
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]attrib��  ����
**             [in]state��   ״̬
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_security_0x0002(addr_info_t *addr, uint8_t attrib, uint8_t state, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;

    len = sizeof(attrib) + sizeof(state);
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writebyte(&wstream, attrib);               /* ����  */
    stream_writebyte(&wstream, state);                /* ״̬  */
    
    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0002, pkt, 0, 0, security_informer);
}

/*******************************************************************
** ������:     stop_security_0x0002
** ��������:   ֹͣԶ�̲�����
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_security_0x0002(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;
    cancel_send_list(0x0002);
    return true;
}

/*******************************************************************
** ������:     start_security_0x0003
** ��������:   ���������ϴ�
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]area��    ����������Ϣ
**             [in]pic_num�� ͼƬ����
**             [in]pic��     ͼƬ��Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_security_0x0003(addr_info_t *addr, area_record_t *area, uint8_t pic_num, name_t *pic, void (*informer)(RESULT_E))
{
    uint8_t   i;
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;
    
    len  = sizeof(area->area_type) + sizeof(area->area_num) + sizeof(area->area_name);
    len += sizeof(area->cam_channel) + sizeof(area->area_attrib) + sizeof(area->area_len);
    len += area->area_len + sizeof(area->alarm_time) + sizeof(pic_num);
    for(i = 0; i < pic_num; i++){
        len += sizeof(pic[i].name_len);
        len += pic[i].name_len;
    }
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writebyte(&wstream, area->area_type);              
    stream_writebyte(&wstream, area->area_num);
    stream_writebyte(&wstream, area->area_name);
    stream_writebyte(&wstream, area->cam_channel);
    stream_writebyte(&wstream, area->area_attrib);
    stream_writebyte(&wstream, area->area_len);
    stream_writedata(&wstream, area->area_addr, area->area_len);
    stream_writedata(&wstream, area->alarm_time, sizeof(area->alarm_time));

    stream_writebyte(&wstream, pic_num);              
    for (i = 0; i < pic_num; i++){
        stream_writebyte(&wstream, pic[i].name_len); 
        stream_writedata(&wstream, pic[i].name, pic[i].name_len);
    }

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);

    return apply_send_list(0x0003, pkt, 0, 0, security_informer);
}


/*******************************************************************
** ������:     stop_security_0x0003
** ��������:   ֹͣ���������ϴ�
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_security_0x0003(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0003);
    return true;
}


/*******************************************************************
** ������:     start_security_0x0004
** ��������:   ��������
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_security_0x0004(addr_info_t *addr, void (*informer)(RESULT_E))
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
    return apply_send_list(0x0004, pkt, 0, 0, security_informer);
}


/*******************************************************************
** ������:     stop_security_0x0004
** ��������:   ֹͣ��������
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_security_0x0004(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0004);
    return true;
}


/*******************************************************************
** ������:     hdl_data_0x0001
** ��������:   ����Э��Ĵ���
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0001(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  result, operate, arrays, i;
    security_record_t rec;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0001");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_security_0x0001();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        
        operate = stream_readbyte(&rstrm);
        arrays  = stream_readbyte(&rstrm);
        for (i = 0; i < arrays; i++){
            rec.attrib = stream_readbyte(&rstrm);
            rec.seq    = stream_readbyte(&rstrm);
            rec.state  = stream_readbyte(&rstrm);
            write_security_record_0x0001(operate, i, &rec);
        }
        
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** ������:     hdl_data_0x0102
** ��������:   ����Э��Ĵ���
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0002(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  result, attrib, state;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0002");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_security_0x0002();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        
        attrib = stream_readbyte(&rstrm);
        state  = stream_readbyte(&rstrm);
        write_security_record_0x0002(attrib, state);
        LEE_LOG("attrib = %d state = %d", attrib, state);
        return RESULT_SUCCESS;
    }
    return 0;
}



/*******************************************************************
** ������:     hdl_data_0x0003
** ��������:   ����Э��Ĵ���
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0003(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  result, pic_num, i;
    uint8_t  addr[60], name[60];
    area_record_t area;
    name_t pic;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0003");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_security_0x0003();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        LEE_MEMSET(area.area_user, 0, sizeof(area.area_user));
        bytetoaddr(pkt->src, area.area_user);
        LEE_MEMSET(area.area_ip, 0, sizeof(area.area_ip));
        iptostr(area.area_ip, pkt->dest_ip);

        area.area_type  = stream_readbyte(&rstrm);
        area.area_num   = stream_readbyte(&rstrm);
        area.area_name   = stream_readbyte(&rstrm);
        area.cam_channel= stream_readbyte(&rstrm);
        area.area_attrib= stream_readbyte(&rstrm);
        area.area_len   = stream_readbyte(&rstrm);
        area.area_addr  = addr;
        stream_readdata(&rstrm, area.area_addr, area.area_len);
        LEE_LOG("addr %s", area.area_addr);
        stream_readdata(&rstrm, area.alarm_time, sizeof(area.alarm_time));
        pic_num = stream_readbyte(&rstrm);
        if (pic_num == 0){
            write_security_record_0x0003(&area, 0, NULL);
        }
        for (i = 0; i < pic_num; i++){
            pic.name_len = stream_readbyte(&rstrm);
            pic.name = name;
            stream_readdata(&rstrm, pic.name, pic.name_len);
            LEE_LOG("picname %s", pic.name);
            write_security_record_0x0003(&area, i, &pic);
        }
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** ������:     hdl_data_0x0004
** ��������:   ����Э��Ĵ���
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0004(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  src_str[12], dest_str[12];
    uint8_t  result, dest_ip[20];
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0004");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_security_0x0004();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        
        LEE_MEMSET(src_str, 0, sizeof(src_str));
        bytetoaddr(pkt->src, src_str);

        LEE_MEMSET(dest_str, 0, sizeof(dest_str));
        bytetoaddr(pkt->dest, dest_str);
        
        LEE_LOG("src = %s dest = %s", src_str, dest_str);

        LEE_MEMSET(dest_ip, 0, sizeof(dest_ip));
        iptostr(dest_ip, pkt->dest_ip);
        write_security_record_0x0004(src_str, dest_ip);
        return RESULT_SUCCESS;
    }
    return 0;
}


static handle_table_t  hdl_tab[] = { 0x0001, false, hdl_data_0x0001,
                                     0x0002, false, hdl_data_0x0002,
                                     0x0003, false, hdl_data_0x0003,
                                     0x0004, false, hdl_data_0x0004,
                                   };

/*******************************************************************
** ������:     init_security
** ��������:   ģ���ʼ������
** ����:       ��
** ����:       ��
********************************************************************/
void init_security(void)
{
    int i;
    
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    for (i = 0; i < sizeof(hdl_tab)/sizeof(hdl_tab[0]); i++) {
        regist_pkt_handler(hdl_tab[i].cmd_type, hdl_tab[i].param,  hdl_tab[i].handler);
    }

}






