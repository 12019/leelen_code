/********************************************************************************
**
** 文件名:     leelen_packet.c
** 版权所有:   (c) 2015 厦门立林科技有限公司
** 文件描述:   实现通讯协议数据包分配管理模块
**
*********************************************************************************
**             修改历史记录
**===============================================================================
**| 日期       | 作者   |  修改记录
**===============================================================================
**| 2015/09/28 | zzh    |  创建该文件
**
*********************************************************************************/

#include "leelen_comdef.h"
#include "leelen_packet.h"


/*******************************************************************
** 函数名:     packet_head_length
** 函数描述:   获取包头长度(包括累加和预留字节 3个字节)
** 参数:       [in]  pkt:       数据包
** 返回:       包头数据长度
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
** 函数名:     packet_assemble
** 函数描述:   根据协议进行组帧
** 参数:       [in]  pkt:       数据包
**             [in]  cmd:       协议命令字
**             [in]  sid:       会话id
** 返回:       无
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
** 函数名:     packet_alloc
** 函数描述:   请求分配一个数据包
** 参数:       [in]  len:       请求分配数据包的大小
** 返回:       分配到的数据包; 如分配失败, 则返回0
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
** 函数名:     packet_free
** 函数描述:   释放分配到的数据包
** 参数:       [in]  pkt:       待释放的数据包
** 返回:       释放成功或失败
********************************************************************/
void packet_free(packet_t *pkt)
{
    LEE_ASSERT(pkt != NULL);
    LEE_FREE(pkt);
}


