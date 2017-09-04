/********************************************************************************
**
** �ļ���:     leelen_system.c
** ��Ȩ����:   (c) 2015 �������ֿƼ����޹�˾
** �ļ�����:   ʵ��ϵͳ�����ͨѶЭ�鹦��
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
** ������:     system_informer
** ��������:   ���ݷ���֪ͨ����
** ����:       [in]  result:        ���ͽ��
** ����:       ��
********************************************************************/
static void system_informer(RESULT_E result)
{
    
    if (s_psb.informer != NULL){
        s_psb.informer(result);
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;
}


/*******************************************************************
** ������:     start_system_0x0501
** ��������:   Э��̽��
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_system_0x0501(addr_info_t *addr, void (*informer)(RESULT_E))
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
    return apply_send_list(0x0501, pkt, 0, 0, system_informer);
}

/*******************************************************************
** ������:     stop_system_0x0501
** ��������:   ֹͣЭ��̽��
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_system_0x0501(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0501);
    return true;
}

/*******************************************************************
** ������:     start_system_0x0502
** ��������:   ʱ��ͬ��
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_system_0x0502(addr_info_t *addr, void (*informer)(RESULT_E))
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
    return apply_send_list(0x0502, pkt, 0, 0, system_informer);
}

/*******************************************************************
** ������:     stop_system_0x0502
** ��������:   ֹͣʱ��ͬ��
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_system_0x0502(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0502);
    return true;
}

/*******************************************************************
** ������:     start_system_0x0503
** ��������:   �豸�쳣�����ϴ�
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]device��  �쳣�豸��Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_system_0x0503(addr_info_t *addr, device_record_t *device, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;
    
    len = sizeof(device->device_type) + sizeof(device->device_num) + sizeof(device->device_alarm);
    len += sizeof(device->device_len) + device->device_len;
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writebyte(&wstream, device->device_type);
    stream_writedata(&wstream, device->device_num, sizeof(device->device_num));
    stream_writebyte(&wstream, device->device_alarm);
    stream_writebyte(&wstream, device->device_len);
    stream_writedata(&wstream, device->device_name, device->device_len);

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0503, pkt, 0, 0, system_informer);
}


/*******************************************************************
** ������:     stop_system_0x0503
** ��������:   ֹͣ�豸�쳣�����ϴ�
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_system_0x0503(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0503);
    return true;
}

/*******************************************************************
** ������:     start_system_0x0504
** ��������:   ��Ϣ����
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]info��    ��Ϣ��������
**             [in]num��     ��������
**             [in]attach:   ������Ϣ
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_system_0x0504(addr_info_t *addr, info_t *info, uint8_t num, attachment_t *attach, void (*informer)(RESULT_E))
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
    
    len  = sizeof(info->title_len) + info->title_len + sizeof(info->content_len) + info->content_len;
    len += sizeof(num);
    for(i = 0; i < num; i++){
        len += sizeof(attach[i].type);
        len += sizeof(attach[i].file_len);
        len += attach[i].file_len;
        len += sizeof(attach[i].length);
    }
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writebyte(&wstream, info->title_len);              
    stream_writedata(&wstream, info->title, info->title_len);
    stream_writeint(&wstream, info->content_len);              
    stream_writedata(&wstream, info->content, info->content_len);

    stream_writebyte(&wstream, num);              
    for (i = 0; i < num; i++){
        stream_writebyte(&wstream, attach[i].type); 
        stream_writebyte(&wstream, attach[i].file_len);
        stream_writedata(&wstream, attach[i].file, attach[i].file_len);
        stream_writeint(&wstream, attach[i].length); 
    }

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);

    return apply_send_list(0x0504, pkt, 0, 0, system_informer);
}


/*******************************************************************
** ������:     stop_system_0x0504
** ��������:   ֹͣ��Ϣ����
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_system_0x0504(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0504);
    return true;
}


/*******************************************************************
** ������:     hdl_data_0x0501
** ��������:   ����Э��Ĵ���
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0501(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm, wstrm;
    uint8_t  *version, version_buf[100];
    uint8_t  result, version_len;
    uint16_t max_len;
    
    LEE_ASSERT(data != NULL);
    
    LEE_LOG("hdl_data_0x0501");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        max_len = stream_readshort(&rstrm);
        version_len = stream_readbyte(&rstrm);
        version_len = LEE_MIN(version_len, sizeof(version_buf)-1);
        stream_readdata(&rstrm, version_buf, version_len);
        version_buf[version_len] = '\0';
        
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }
        
        stop_system_0x0501();
        set_version_info_0x0501(version_buf, version_len);
        LEE_LOG("restult = %d %s", result, version_buf);
    } else if (pkt->req_ack == LEE_REQ){
        version     = get_version();
        version_len = LEE_STRLEN(version);
        
        pkt->body_len = version_len + sizeof(uint8_t) + sizeof(uint16_t); 
        pkt->prot_body = (uint8_t *)LEE_MALLOC(pkt->body_len);  /* ���ͺ��Զ��ͷ� */
        if (pkt->prot_body == NULL){     
            pkt->body_len  = 0;
            pkt->prot_body = NULL;
            return RESULT_NOMEM;
        }

        stream_init(&wstrm, pkt->prot_body, pkt->body_len);
        max_len = PKT_MAX_SIZE;
        stream_writeshort(&wstrm, max_len);  
        stream_writebyte(&wstrm,  version_len); 
        stream_writedata(&wstrm,  version, version_len);  
      
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** ������:     hdl_data_0x0502
** ��������:   ����Э��Ĵ���
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0502(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm, wstrm;
    uint8_t  date_time[7];
    uint8_t  result, datetime_len;
        
    LEE_ASSERT(data != NULL);
    
    LEE_LOG("hdl_data_0x0502 %d", pkt->req_ack);
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        stream_readdata(&rstrm, date_time, sizeof(date_time));

        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_system_0x0502();
        set_datetime_0x0502(date_time, sizeof(date_time));
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        datetime_len = sizeof(date_time);
        get_datetime_0x0502(date_time, &datetime_len);
        
        pkt->body_len = datetime_len;
        pkt->prot_body = (uint8_t *)LEE_MALLOC(pkt->body_len);  /* ���ͺ��Զ��ͷ� */
        if (pkt->prot_body == NULL){     
            pkt->body_len  = 0;
            pkt->prot_body = NULL;
            return RESULT_NOMEM;
        }

        stream_init(&wstrm, pkt->prot_body, pkt->body_len);
        stream_writedata(&wstrm, date_time, pkt->body_len);  

        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** ������:     hdl_data_0x0503
** ��������:   ����Э��Ĵ���,�豸�쳣�����ϴ�
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0503(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  result;
    uint8_t  name_buf[80];
    device_record_t rec;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0503");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_system_0x0503();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        rec.device_type = stream_readbyte(&rstrm);
        stream_readdata(&rstrm, rec.device_num, sizeof(rec.device_num));
        rec.device_alarm = stream_readbyte(&rstrm);
        rec.device_len = stream_readbyte(&rstrm);
        rec.device_len = LEE_MIN(rec.device_len, sizeof(name_buf)-1);
        rec.device_name = name_buf;
        stream_readdata(&rstrm, rec.device_name, rec.device_len);

        write_device_record_0x0503(&rec);
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** ������:     hdl_data_0x0504
** ��������:   ����Э��Ĵ���,��Ϣ����
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
static uint8_t hdl_data_0x0504(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  result, num, i;
    info_t   info;
    attachment_t *attach;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0504");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_system_0x0504();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        info.title_len = stream_readbyte(&rstrm);
        info.title     = (uint8_t *)LEE_MALLOC(info.title_len+1);
        if (info.title == NULL){
            return 0;
        }
        stream_readdata(&rstrm, info.title, info.title_len);
        info.title[info.title_len] = '\0';
        LEE_LOG("title %s", info.title);
        info.content_len = stream_readint(&rstrm);
        info.content = (uint8_t *)LEE_MALLOC(info.content_len+1);
        if (info.content == NULL){
            return 0;
        }
        stream_readdata(&rstrm, info.content, info.content_len);
        info.content[info.content_len] = '\0';
        LEE_LOG("content %s", info.content);
        num = stream_readbyte(&rstrm);
        attach = (attachment_t *)LEE_MALLOC(sizeof(attachment_t) * num);
        if (attach == NULL){
            return 0;
        }
        for (i = 0; i < num; i++){
            attach[i].type = stream_readbyte(&rstrm);
            attach[i].file_len = stream_readbyte(&rstrm);
            attach[i].file = (uint8_t *)LEE_MALLOC(attach[i].file_len + 1);
            if (attach[i].file == NULL){
                return 0;
            }
            stream_readdata(&rstrm, attach[i].file, attach[i].file_len);
            attach[i].file[attach[i].file_len] = '\0';
            attach[i].length = stream_readint(&rstrm);
            LEE_LOG("attach_file %s %d", attach[i].file, attach[i].length);
        }
        write_info_0x0504(&info, num, attach, pkt->src);
        if (info.title != NULL){
            LEE_FREE(info.title);
        }
        if (info.content != NULL){
            LEE_FREE(info.content);
        }

        if (attach != NULL){
            for (i = 0; i < num; i++){
                if (attach[i].file != NULL){
                    LEE_FREE(attach[i].file);
                }
            }
            LEE_FREE(attach);
        }
        // to save
        return RESULT_SUCCESS;
    }
    return 0;
}


static handle_table_t  hdl_tab[] = { 0x0501, true,  hdl_data_0x0501,
                                     0x0502, true,  hdl_data_0x0502,
                                     0x0503, false, hdl_data_0x0503,
                                     0x0504, false, hdl_data_0x0504,
                                   };

/*******************************************************************
** ������:     init_system
** ��������:   ģ���ʼ������
** ����:       ��
** ����:       ��
********************************************************************/
void init_system(void)
{
    int i;

    s_psb.status   = 0;
    s_psb.informer =  NULL;

    for (i = 0; i < sizeof(hdl_tab)/sizeof(hdl_tab[0]); i++) {
        regist_pkt_handler(hdl_tab[i].cmd_type, hdl_tab[i].param, hdl_tab[i].handler);
    }

}


