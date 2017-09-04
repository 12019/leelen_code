/********************************************************************************
**
** �ļ���:     leelen_talk.c
** ��Ȩ����:   (c) 2015 �������ֿƼ����޹�˾
** �ļ�����:   ʵ�ֶԽ����ͨѶЭ�鹦��
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
** ������:     talk_informer
** ��������:   ���ݷ���֪ͨ����
** ����:       [in]  result:        ���ͽ��
** ����:       ��
********************************************************************/
static void talk_informer(RESULT_E result)
{
    
    if (s_psb.informer != NULL){
        s_psb.informer(result);
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;
}



/*******************************************************************
** ������:     start_talk_0x0301
** ��������:   ���м�¼�ϴ�
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]fromto��  ���С����е�ַ
**             [in]rec��     �Խ���¼
**             [in]pic_num�� ͼƬ����
**             [in]pic��     ͼƬ��Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_talk_0x0301(addr_info_t *addr, addr_info_t *fromto, talk_record_t *rec, uint8_t pic_num, name_t *pic, void (*informer)(RESULT_E))
{
    uint8_t   from[5], to[5], i;
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;
    
    len = sizeof(talk_record_t) + sizeof(from) + sizeof(to) +sizeof(pic_num);
    for(i = 0; i < pic_num; i++){
        len += sizeof(pic[i].name_len);
        len += pic[i].name_len;
    }
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }
    addrtobyte(fromto->loc_usr, from);
    addrtobyte(fromto->rem_usr, to);

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writedata(&wstream, from, sizeof(from));
    stream_writedata(&wstream, to,   sizeof(to));
    stream_writeshort(&wstream, rec->length);              
    stream_writebyte(&wstream, rec->answer);
    stream_writebyte(&wstream, rec->unlock);
    stream_writebyte(&wstream, rec->message);
    stream_writebyte(&wstream, pic_num);
    
    for (i = 0; i < pic_num; i++){
        stream_writebyte(&wstream, pic[i].name_len); 
        stream_writedata(&wstream, pic[i].name, pic[i].name_len);
    }

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0301, pkt, 0, 0, talk_informer);
}


/*******************************************************************
** ������:     stop_talk_0x0301
** ��������:   ֹͣ���м�¼�ϴ�
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_talk_0x0301(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0301);
    return true;
}


/*******************************************************************
** ������:     hdl_data_0x0301
** ��������:   ����Э��Ĵ���
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0301(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  name[100], from[5], to[5];
    uint8_t  result, nums, i;
    talk_record_t rec;
    name_t pic;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0301");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_talk_0x0301();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        stream_readdata(&rstrm, from, sizeof(from));
        stream_readdata(&rstrm, to,  sizeof(to));

        rec.length = stream_readshort(&rstrm);
        rec.answer = stream_readbyte(&rstrm);
        rec.unlock = stream_readbyte(&rstrm);
        rec.message= stream_readbyte(&rstrm);
        nums  = stream_readbyte(&rstrm);
        if (nums == 0){
            write_talk_record_0x0301(&rec, 0, NULL);
        }
        for (i = 0; i < nums; i++){
            pic.name_len = stream_readbyte(&rstrm);
            pic.name_len = LEE_MIN(sizeof(name)-1, pic.name_len);
            pic.name = name;
            stream_readdata(&rstrm, pic.name, pic.name_len);
            pic.name[pic.name_len] = '\0';
            write_talk_record_0x0301(&rec, i, &pic);
        }
        return RESULT_SUCCESS;
    }
    return 0;
}


/*******************************************************************
** ������:     init_talk
** ��������:   ģ���ʼ������
** ����:       ��
** ����:       ��
********************************************************************/
void init_talk(void)
{
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    regist_pkt_handler(0x0301, false, hdl_data_0x0301);
}






