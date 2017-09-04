/********************************************************************************
**
** �ļ���:     leelen_card.c
** ��Ȩ����:   (c) 2015 �������ֿƼ����޹�˾
** �ļ�����:   ʵ���Ž�ˢ�����ͨѶЭ�鹦��
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
** ������:     card_informer
** ��������:   ���ݷ���֪ͨ����
** ����:       [in]  result:        ���ͽ��
** ����:       ��
********************************************************************/
static void card_informer(RESULT_E result)
{
    
    if (s_psb.informer != NULL){
        s_psb.informer(result);
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;
}

/*******************************************************************
** ������:     start_card_0x0101
** ��������:   ��ʼ���ص��ſ�
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]card��    ����Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_card_0x0101(addr_info_t *addr, card_info_t *card, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;

    len = sizeof(card->card_id) + sizeof(card->card_usr) + sizeof(card->card_type) + sizeof(card->card_start) + sizeof(card->card_end);
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writedata(&wstream, card->card_id, sizeof(card->card_id)); 
    stream_writedata(&wstream, card->card_usr, sizeof(card->card_usr)); 
    stream_writebyte(&wstream, card->card_type); 
    stream_writedata(&wstream, card->card_start, sizeof(card->card_start));
    stream_writedata(&wstream, card->card_end, sizeof(card->card_end)); 

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0101, pkt, 0, 0, card_informer);
}


/*******************************************************************
** ������:     stop_card_0x0101
** ��������:   ֹͣ���ص��ſ�
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_card_0x0101(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0101);
    return true;
}


/*******************************************************************
** ������:     start_card_0x0102
** ��������:   ɾ�����ſ�
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]card��    ����Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_card_0x0102(addr_info_t *addr, card_info_t *card, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;

    len = sizeof(card->card_id);
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writedata(&wstream, card->card_id, sizeof(card->card_id)); 

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0102, pkt, 0, 0, card_informer);
}


/*******************************************************************
** ������:     stop_card_0x0102
** ��������:   ֹͣɾ�����ſ�
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_card_0x0102(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0102);
    return true;
}


/*******************************************************************
** ������:     start_card_0x0103
** ��������:   ɾ��ĳ��ȫ����
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]card��    ����Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_card_0x0103(addr_info_t *addr, card_info_t *card, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;

    len = sizeof(card->card_usr);
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writedata(&wstream, card->card_usr, sizeof(card->card_usr)); 

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0103, pkt, 0, 0, card_informer);
}


/*******************************************************************
** ������:     stop_card_0x0103
** ��������:   ֹͣɾ��ĳ��ȫ����
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_card_0x0103(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0103);
    return true;
}


/*******************************************************************
** ������:     start_card_0x0104
** ��������:   ɾ��ĳ��ȫ��������
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_card_0x0104(addr_info_t *addr, void (*informer)(RESULT_E))
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
    return apply_send_list(0x0104, pkt, 0, 0, card_informer);
}


/*******************************************************************
** ������:     stop_card_0x0104
** ��������:   ֹͣɾ��ȫ��������
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_card_0x0104(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0104);
    return true;
}

/*******************************************************************
** ������:     start_card_0x0105
** ��������:   ɾ���������п�
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_card_0x0105(addr_info_t *addr, void (*informer)(RESULT_E))
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
    return apply_send_list(0x0105, pkt, 0, 0, card_informer);
}


/*******************************************************************
** ������:     stop_card_0x0105
** ��������:   ɾ���������п�
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_card_0x0105(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0105);
    return true;
}

/*******************************************************************
** ������:     start_card_0x0106
** ��������:   ˢ����¼�ϴ�
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]rec��     ˢ����¼
**             [in]pic_num�� ͼƬ����
**             [in]pic��     ͼƬ��Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_card_0x0106(addr_info_t *addr, card_record_t *rec, uint8_t pic_num, name_t *pic, void (*informer)(RESULT_E))
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

    len = sizeof(rec->card_pos) + sizeof(rec->card_id) + sizeof(rec->lock_state) + sizeof(pic_num);
    for(i = 0; i < pic_num; i++){
        len += sizeof(pic[i].name_len);
        len += pic[i].name_len;
    }

    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writebyte(&wstream, rec->card_pos); 
    stream_writedata(&wstream, rec->card_id, sizeof(rec->card_id)); 
    stream_writebyte(&wstream, rec->lock_state);
    stream_writebyte(&wstream, pic_num);
    
    for (i = 0; i < pic_num; i++){
        stream_writebyte(&wstream, pic[i].name_len); 
        stream_writedata(&wstream, pic[i].name, pic[i].name_len);
    }

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0106, pkt, 0, 0, card_informer);
}


/*******************************************************************
** ������:     stop_card_0x0106
** ��������:   ֹͣˢ����¼�ϴ�
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_card_0x0106(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0106);
    return true;
}


/*******************************************************************
** ������:     start_card_0x0107
** ��������:   ���뿪����¼�ϴ�
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]rec��     ˢ����¼
**             [in]pic_num�� ͼƬ����
**             [in]pic��     ͼƬ��Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_card_0x0107(addr_info_t *addr, card_record_t *rec, uint8_t pic_num, name_t *pic, void (*informer)(RESULT_E))
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

    len = sizeof(rec->card_usr) + sizeof(rec->lock_state) + sizeof(pic_num);
    for(i = 0; i < pic_num; i++){
        len += sizeof(pic[i].name_len);
        len += pic[i].name_len;
    }

    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writedata(&wstream, rec->card_usr, sizeof(rec->card_usr)); 
    stream_writebyte(&wstream, rec->lock_state); 
    stream_writebyte(&wstream, pic_num);
    
    for (i = 0; i < pic_num; i++){
        stream_writebyte(&wstream, pic[i].name_len); 
        stream_writedata(&wstream, pic[i].name, pic[i].name_len);
    }

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0107, pkt, 0, 0, card_informer);
}


/*******************************************************************
** ������:     stop_card_0x0107
** ��������:   ֹͣ���뿪����¼�ϴ�
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_card_0x0107(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0107);
    return true;
}


/*******************************************************************
** ������:     start_card_0x0108
** ��������:   ��״̬��Ϣ��¼�ϴ�
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]rec��     ˢ����¼
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_card_0x0108(addr_info_t *addr, card_record_t *rec, void (*informer)(RESULT_E))
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

    len = sizeof(rec->open_reason) + sizeof(rec->lock_state) + sizeof(rec->card_pos);
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writebyte(&wstream, rec->open_reason); 
    stream_writebyte(&wstream, rec->lock_state); 
    stream_writebyte(&wstream, rec->card_pos);
    
    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0108, pkt, 0, 0, card_informer);
}


/*******************************************************************
** ������:     stop_card_0x0108
** ��������:   ֹͣ��״̬��Ϣ��¼�ϴ�
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_card_0x0108(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0108);
    return true;
}


/*******************************************************************
** ������:     start_card_0x0109
** ��������:   Զ�̿���
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]rec��     ˢ����¼
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_card_0x0109(addr_info_t *addr, card_record_t *rec, void (*informer)(RESULT_E))
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

    len = sizeof(rec->card_pos);
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writebyte(&wstream, rec->card_pos); 

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0109, pkt, 0, 0, card_informer);
}


/*******************************************************************
** ������:     stop_card_0x0109
** ��������:   ֹͣ��״̬��Ϣ��¼�ϴ�
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_card_0x0109(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0109);
    return true;
}


/*******************************************************************
** ������:     start_card_0x010a
** ��������:   Ѳ����ˢ����¼�ϴ�
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]rec��     Ѳ��ˢ����¼
**             [in]pic_num�� ͼƬ����
**             [in]pic��     ͼƬ��Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_card_0x010a(addr_info_t *addr, card_patrol_t *rec, uint8_t pic_num, name_t *pic, void (*informer)(RESULT_E))
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

    len = sizeof(rec->card_id) + sizeof(rec->time) + sizeof(rec->device) + sizeof(rec->card_pos) + sizeof(pic_num);
    for(i = 0; i < pic_num; i++){
        len += sizeof(pic[i].name_len);
        len += pic[i].name_len;
    }

    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writedata(&wstream, rec->time,   sizeof(rec->time)); 
    stream_writedata(&wstream, rec->device, sizeof(rec->device));
    stream_writebyte(&wstream, rec->card_pos); 
    stream_writedata(&wstream, rec->card_id, sizeof(rec->card_id)); 
    stream_writebyte(&wstream, pic_num);
    
    for (i = 0; i < pic_num; i++){
        stream_writebyte(&wstream, pic[i].name_len); 
        stream_writedata(&wstream, pic[i].name, pic[i].name_len);
    }
    
    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x010a, pkt, 0, 0, card_informer);
}


/*******************************************************************
** ������:     stop_card_0x010a
** ��������:   ֹͣѲ����ˢ����¼�ϴ�
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_card_0x010a(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x010a);
    return true;
}

/*******************************************************************
** ������:     hdl_data_0x0101
** ��������:   ����Э��Ĵ���,�·����ſ�
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0101(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  result;
    card_info_t card;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0101");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_card_0x0101();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){

        stream_readdata(&rstrm, card.card_id,  sizeof(card.card_id));
        stream_readdata(&rstrm, card.card_usr, sizeof(card.card_usr));
        card.card_type = stream_readbyte(&rstrm);
        stream_readdata(&rstrm, card.card_start, sizeof(card.card_start));
        stream_readdata(&rstrm, card.card_end,   sizeof(card.card_end));

        write_card_record_0x0101(&card);
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** ������:     hdl_data_0x0102
** ��������:   ����Э��Ĵ���,ɾ����
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0102(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  result;
    card_info_t card;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0102");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_card_0x0102();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){

        stream_readdata(&rstrm, card.card_id,  sizeof(card.card_id));

        delete_card_record_0x0102(&card);
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** ������:     hdl_data_0x0103
** ��������:   ����Э��Ĵ���,ɾ��ĳ�����п�
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0103(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  result;
    card_info_t card;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0103");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_card_0x0103();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){

        stream_readdata(&rstrm, card.card_usr,  sizeof(card.card_usr));

        delete_card_record_0x0103(&card);
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** ������:     hdl_data_0x0104
** ��������:   ����Э��Ĵ���,ɾ��ȫ��������
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0104(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  result;
        
    LEE_ASSERT(data != NULL);
    
    LEE_LOG("hdl_data_0x0104");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_card_0x0104();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){

        delete_card_record_0x0104();
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** ������:     hdl_data_0x0105
** ��������:   ����Э��Ĵ���,ɾ���������п�
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0105(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  result;
        
    LEE_ASSERT(data != NULL);
    
    LEE_LOG("hdl_data_0x0105");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_card_0x0105();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){

        delete_card_record_0x0105();
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** ������:     hdl_data_0x0106
** ��������:   ����Э��Ĵ���,ˢ����¼�ϴ�
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0106(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  name[100];
    uint8_t  result, nums, i;
    card_record_t rec;
    name_t pic;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0106");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_card_0x0106();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        
        rec.card_pos = stream_readbyte(&rstrm);
        stream_readdata(&rstrm, rec.card_id, sizeof(rec.card_id));
        rec.lock_state= stream_readbyte(&rstrm);

        nums  = stream_readbyte(&rstrm);
        if (nums == 0){
            write_card_record_0x0106(&rec, 0, NULL);
        }
        for (i = 0; i < nums; i++){
            pic.name_len = stream_readbyte(&rstrm);
            pic.name = name;
            stream_readdata(&rstrm, pic.name, pic.name_len);
            write_card_record_0x0106(&rec, i, &pic);
        }
        
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** ������:     hdl_data_0x0107
** ��������:   ����Э��Ĵ���,���뿪����¼�ϴ�
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0107(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  name[100];
    uint8_t  result, nums, i;
    card_record_t rec;
    name_t pic;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0107");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_card_0x0107();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        
        stream_readdata(&rstrm, rec.card_usr, sizeof(rec.card_usr));
        rec.lock_state= stream_readbyte(&rstrm);

        nums  = stream_readbyte(&rstrm);
        if (nums == 0){
            write_card_record_0x0107(&rec, 0, NULL);
        }
        for (i = 0; i < nums; i++){
            pic.name_len = stream_readbyte(&rstrm);
            pic.name = name;
            stream_readdata(&rstrm, pic.name, pic.name_len);
            write_card_record_0x0107(&rec, i, &pic);
        }
        
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** ������:     hdl_data_0x0108
** ��������:   ����Э��Ĵ���,��״̬��Ϣ�ϴ�
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0108(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  result;
    card_record_t rec;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0108");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_card_0x0108();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){

        rec.open_reason = stream_readbyte(&rstrm);
        rec.lock_state  = stream_readbyte(&rstrm);
        rec.card_pos    = stream_readbyte(&rstrm);
        
        write_card_record_0x0108(&rec);
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** ������:     hdl_data_0x0109
** ��������:   ����Э��Ĵ���,Զ�̿���
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0109(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  result;
    card_record_t rec;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0109");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_card_0x0109();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){

        rec.card_pos    = stream_readbyte(&rstrm);
        return write_card_record_0x0109(&rec);
    }
    return 0;
}

/*******************************************************************
** ������:     hdl_data_0x010a
** ��������:   ����Э��Ĵ���,Ѳ��ˢ����Ϣ�ϴ�
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x010a(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  name[100];
    uint8_t  result, nums, i;
    card_patrol_t rec;
    name_t pic;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x010a");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_card_0x010a();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        stream_readdata(&rstrm, rec.time, sizeof(rec.time));
        stream_readdata(&rstrm, rec.device, sizeof(rec.device));
        rec.card_pos = stream_readbyte(&rstrm);
        stream_readdata(&rstrm, rec.card_id, sizeof(rec.card_id));

        nums  = stream_readbyte(&rstrm);
        if (nums == 0){
            write_card_record_0x010a(&rec, 0, NULL);
        }
        for (i = 0; i < nums; i++){
            pic.name_len = stream_readbyte(&rstrm);
            pic.name = name;
            stream_readdata(&rstrm, pic.name, pic.name_len);
            write_card_record_0x010a(&rec, i, &pic);
        }
        return RESULT_SUCCESS;
    }
    return 0;
}

static handle_table_t  hdl_tab[] = { 0x0101, false, hdl_data_0x0101,
                                     0x0102, false, hdl_data_0x0102,
                                     0x0103, false, hdl_data_0x0103,
                                     0x0104, false, hdl_data_0x0104,
                                     0x0105, false, hdl_data_0x0105,
                                     0x0106, false, hdl_data_0x0106,
                                     0x0107, false, hdl_data_0x0107,
                                     0x0108, false, hdl_data_0x0108,
                                     0x0109, false, hdl_data_0x0109,
                                     0x010a, false, hdl_data_0x010a
                                   };

/*******************************************************************
** ������:     init_card
** ��������:   ģ���ʼ������
** ����:       ��
** ����:       ��
********************************************************************/
void init_card(void)
{
    int i;

    s_psb.status   = 0;
    s_psb.informer =  NULL;

    for (i = 0; i < sizeof(hdl_tab)/sizeof(hdl_tab[0]); i++) {
        regist_pkt_handler(hdl_tab[i].cmd_type, hdl_tab[i].param, hdl_tab[i].handler);
    }

}






