/********************************************************************************
**
** �ļ���:     leelen_big_packet.c
** ��Ȩ����:   (c) 2015 �������ֿƼ����޹�˾
** �ļ�����:   ʵ��ͨѶЭ������ݰ��������ģ��
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


/*******************************************************************
** ������:     big_packet_head_length
** ��������:   ��ȡ��ͷ����(�����ۼӺ�Ԥ���ֽ� 3���ֽ�)
** ����:       [in]  pkt:       ���ݰ�
** ����:       ��ͷ���ݳ���
********************************************************************/
uint8_t big_packet_head_length(big_packet_t *pkt)
{
    uint8_t length;
    length = sizeof(pkt->sync_head) + sizeof(pkt->prot_ver) + sizeof(pkt->cmd_type) +
        sizeof(pkt->session_id) + sizeof(pkt->req_ack) + sizeof(pkt->encrypt) +
        sizeof(pkt->pkt_length) + sizeof(pkt->src) + sizeof(pkt->dest) +
        sizeof(pkt->data_type)  + sizeof(pkt->dir_namelen) + pkt->dir_namelen +
        sizeof(pkt->file_namelen) + pkt->file_namelen + sizeof(pkt->file_length) +
        sizeof(pkt->total_pkt)   + sizeof(pkt->cur_pktid) + sizeof(pkt->cur_size) +
        sizeof(pkt->reserved) + sizeof(pkt->check_sum);
    return length;
}

/*******************************************************************
** ������:     big_packet_assemble
** ��������:   ����Э�������֡
** ����:       [in]  pkt:       ���ݰ�
**             [in]  cmd:       Э��������
**             [in]  sid:       �Ựid
**             [in]  len:       ������
** ����:       ��
********************************************************************/
void big_packet_assemble(big_packet_t *pkt, uint16_t cmd, uint32_t sid, uint32_t len)
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
    pkt->cur_pktid    =  1;                  /* ����Ŵ�1��ʼ */
    pkt->total_pkt    =  (pkt->file_length + len -1)/len;
    pkt->cur_size     =  pkt->file_length <= len ? pkt->file_length : len;
    pkt->reserved     =  0xffff;
}

/*******************************************************************
** ������:     big_packet_alloc
** ��������:   �������һ�����ݰ�
** ����:       [in]  len:       ����������ݰ��Ĵ�С
** ����:       ���䵽�����ݰ�; �����ʧ��, �򷵻�0
********************************************************************/
big_packet_t *big_packet_alloc(void)
{
    big_packet_t     *pkt;

    pkt = (big_packet_t *)LEE_MALLOC(sizeof(big_packet_t));
    return pkt;
}

/*******************************************************************
** ������:     big_packet_free
** ��������:   �ͷŷ��䵽�����ݰ�
** ����:       [in]  pkt:       ���ͷŵ����ݰ�
** ����:       �ͷųɹ���ʧ��
********************************************************************/
void big_packet_free(big_packet_t *pkt)
{
    LEE_ASSERT(pkt != 0);
    LEE_FREE(pkt);
}


