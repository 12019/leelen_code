/********************************************************************************
**
** �ļ���:     leelen_packet.c
** ��Ȩ����:   (c) 2015 �������ֿƼ����޹�˾
** �ļ�����:   ʵ��ͨѶЭ�����ݰ��������ģ��
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


/*******************************************************************
** ������:     packet_head_length
** ��������:   ��ȡ��ͷ����(�����ۼӺ�Ԥ���ֽ� 3���ֽ�)
** ����:       [in]  pkt:       ���ݰ�
** ����:       ��ͷ���ݳ���
********************************************************************/
uint8_t packet_head_length(packet_t *pkt)
{
    uint8_t length;
    
    length = sizeof(pkt->sync_head) + sizeof(pkt->prot_ver) + sizeof(pkt->cmd_type) +
        sizeof(pkt->session_id) + sizeof(pkt->req_ack) + sizeof(pkt->encrypt) +
        sizeof(pkt->pkt_length) + sizeof(pkt->src) + sizeof(pkt->dest) +
        sizeof(pkt->reserved) + sizeof(pkt->check_sum);
    return length;
}

/*******************************************************************
** ������:     packet_assemble
** ��������:   ����Э�������֡
** ����:       [in]  pkt:       ���ݰ�
**             [in]  cmd:       Э��������
**             [in]  sid:       �Ựid
** ����:       ��
********************************************************************/
void packet_assemble(packet_t *pkt, uint16_t cmd, uint32_t sid)
{
    pkt->sync_head[0] =  SYNC_HEAD0;
    pkt->sync_head[1] =  SYNC_HEAD1;
    pkt->sync_head[2] =  SYNC_HEAD2;
    pkt->prot_ver     =  PROTOCOL_VER;
    pkt->cmd_type     =  cmd;
    if (pkt->req_ack == LEE_REQ){
        pkt->session_id   =  sid;
    }
    pkt->encrypt      =  0;
    pkt->reserved     =  0xffff;
    pkt->pkt_length   =  packet_head_length(pkt) + pkt->body_len;
}

/*******************************************************************
** ������:     packet_alloc
** ��������:   �������һ�����ݰ�
** ����:       [in]  len:       ����������ݰ��Ĵ�С
** ����:       ���䵽�����ݰ�; �����ʧ��, �򷵻�0
********************************************************************/
packet_t *packet_alloc(int len)
{
    packet_t     *pkt;

    pkt = (packet_t *)LEE_MALLOC(LEE_ALIGN4(sizeof(packet_t)) + len);
    if (pkt == NULL) {
        return NULL;
    }

    pkt->prot_body = (uint8_t *)pkt + sizeof(packet_t);
    pkt->body_len = len;
    return pkt;
}

/*******************************************************************
** ������:     packet_free
** ��������:   �ͷŷ��䵽�����ݰ�
** ����:       [in]  pkt:       ���ͷŵ����ݰ�
** ����:       �ͷųɹ���ʧ��
********************************************************************/
void packet_free(packet_t *pkt)
{
    LEE_ASSERT(pkt != NULL);
    LEE_FREE(pkt);
}


