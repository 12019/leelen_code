/********************************************************************************
**
** �ļ���:     leelen_big.c
** ��Ȩ����:   (c) 2015 �������ֿƼ����޹�˾
** �ļ�����:   ʵ�ִ�����ͨѶЭ�鹦��
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
#include "leelen_big_packet.h"
#include "leelen_stream.h"
#include "leelen_protocol.h"
#include "leelen_big.h"


/*
********************************************************************************
* ����ģ��ֲ�����
********************************************************************************
*/
static protoco_send_t  s_psb;


/*******************************************************************
** ������:     big_informer
** ��������:   ���ݷ���֪ͨ����
** ����:       [in]  result:        ���ͽ��
** ����:       ��
********************************************************************/
static void big_informer(RESULT_E result)
{
    LEE_LOG("big_informer %d", result);
    if (s_psb.informer != NULL){
        s_psb.informer(result);
    }
    s_psb.status   = 0;
   // s_psb.informer =  NULL;
}

/*******************************************************************
** ������:     start_big_0x0601
** ��������:   �����ݼ�¼�ϴ�
** ����:       [in]addr:     Ŀ���ַ��Դ��ַ��Ϣ
**             [in]type��    ����������
**             [in]file��    ������·����Ϣ
**             [in]len��     �����ݴ�С
**             [in]informer���ص�֪ͨ����
** ����:       ����ɹ�true��ʧ��false
********************************************************************/
bool_t start_big_0x0601(addr_info_t *addr, uint8_t type, big_file_t *file, uint32_t len, void (*informer)(RESULT_E))
{
    big_packet_t  *bigpkt;

    if (s_psb.status != 0) {
        //return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;
    
    bigpkt = big_packet_alloc();
    if (bigpkt == NULL){
        return false;
    }

    bigpkt->req_ack     = LEE_REQ;
    bigpkt->data_type   = type;
    bigpkt->file_length = len;
    bigpkt->dir_namelen = LEE_MIN(sizeof(bigpkt->dir_name)-1, file->dir_namelen);
    LEE_MEMCPY(bigpkt->dir_name, file->dir_name, bigpkt->dir_namelen);
    bigpkt->dir_name[bigpkt->dir_namelen] = '\0';
    bigpkt->file_namelen = LEE_MIN(sizeof(bigpkt->file_name)-1, file->file_namelen);
    LEE_MEMCPY(bigpkt->file_name, file->file_name, bigpkt->file_namelen);
    bigpkt->file_name[bigpkt->file_namelen] = '\0';
    addrtobyte(addr->loc_usr, bigpkt->src);
    addrtobyte(addr->rem_usr, bigpkt->dest);
    strtoip(&bigpkt->dest_ip, addr->rem_ip);
    return apply_big_list(0x0601, bigpkt, 0, 0, big_informer);
}


/*******************************************************************
** ������:     stop_big_0x0601
** ��������:   ֹͣ�����ݼ�¼�ϴ�
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
bool_t stop_big_0x0601(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;
    cancel_big_list(0x0601);
    return true;
}


/*******************************************************************
** ������:     hdl_big_data_0x0601
** ��������:   ����Э��Ĵ���
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
**             [in/out]ptk:      ���ݰ�����  
** ����:       1�ɹ�;2����;3��֧��;4�ڴ治��;5��ֹ����
********************************************************************/
uint8_t hdl_big_data_0x0601(uint8_t *data, int data_len, big_packet_t *pkt)
{
    uint8_t  result;
    stream_t rstrm;
    uint32_t offset;
    uint8_t  fullpath[120], dirpath_len;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0601");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        pkt->cur_pktid = stream_readint(&rstrm); 

        LEE_LOG("restult = %d %d", result, pkt->cur_pktid);
    } else if (pkt->req_ack == LEE_REQ){
        pkt->data_type   = stream_readbyte(&rstrm);
        pkt->dir_namelen = stream_readbyte(&rstrm);
        stream_readdata(&rstrm, pkt->dir_name, pkt->dir_namelen);
        pkt->file_namelen = stream_readbyte(&rstrm);
        stream_readdata(&rstrm, pkt->file_name, pkt->file_namelen);

        if ((pkt->dir_namelen + pkt->file_namelen + 2) > sizeof(fullpath)){
            return RESULT_FAIL;
        }

        dirpath_len = pkt->dir_namelen;
        if (dirpath_len > 0){
            LEE_MEMCPY(fullpath, pkt->dir_name, pkt->dir_namelen);  /* ·������ */
            fullpath[pkt->dir_namelen] = '/';
            dirpath_len++;
        }
        LEE_MEMCPY(&fullpath[dirpath_len], pkt->file_name, pkt->file_namelen);
        fullpath[dirpath_len + pkt->file_namelen] = '\0';

        pkt->file_length= stream_readint(&rstrm);
        pkt->total_pkt = stream_readint(&rstrm);                   /* ������ */
        pkt->cur_pktid = stream_readint(&rstrm);
        pkt->cur_size  = stream_readint(&rstrm); 
        
        offset = (pkt->cur_pktid - 1) * PKT_DATA_SIZE;             /* ����Ŵ�1��ʼ */
        LEE_LOG("bigfile %s %d; %d %d %d", fullpath, pkt->file_length, pkt->total_pkt, pkt->cur_pktid, pkt->cur_size);
        write_big_data_0x0601(pkt->data_type, fullpath, offset, stream_getptr(&rstrm), pkt->cur_size);
        if (((pkt->cur_pktid - 1)*PKT_DATA_SIZE + pkt->cur_size) >= pkt->file_length){
            LEE_LOG("finish recv");
        }
        return RESULT_SUCCESS;
    }
    return 0;
}


/*******************************************************************
** ������:     init_big
** ��������:   ģ���ʼ������
** ����:       ��
** ����:       ��
********************************************************************/
void init_big(void)
{
    s_psb.status   = 0;
    s_psb.informer =  NULL;
}






