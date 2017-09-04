/********************************************************************************
**
** 文件名:     leelen_big_packet.c
** 版权所有:   (c) 2015 厦门立林科技有限公司
** 文件描述:   实现通讯协议大数据包分配管理模块
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
#include "leelen_big_packet.h"


/*******************************************************************
** 函数名:     big_packet_head_length
** 函数描述:   获取包头长度(包括累加和预留字节 3个字节)
** 参数:       [in]  pkt:       数据包
** 返回:       包头数据长度
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
** 函数名:     big_packet_assemble
** 函数描述:   根据协议进行组帧
** 参数:       [in]  pkt:       数据包
**             [in]  cmd:       协议命令字
**             [in]  sid:       会话id
**             [in]  len:       包长度
** 返回:       无
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
    pkt->cur_pktid    =  1;                  /* 包序号从1开始 */
    pkt->total_pkt    =  (pkt->file_length + len -1)/len;
    pkt->cur_size     =  pkt->file_length <= len ? pkt->file_length : len;
    pkt->reserved     =  0xffff;
}

/*******************************************************************
** 函数名:     big_packet_alloc
** 函数描述:   请求分配一个数据包
** 参数:       [in]  len:       请求分配数据包的大小
** 返回:       分配到的数据包; 如分配失败, 则返回0
********************************************************************/
big_packet_t *big_packet_alloc(void)
{
    big_packet_t     *pkt;

    pkt = (big_packet_t *)LEE_MALLOC(sizeof(big_packet_t));
    return pkt;
}

/*******************************************************************
** 函数名:     big_packet_free
** 函数描述:   释放分配到的数据包
** 参数:       [in]  pkt:       待释放的数据包
** 返回:       释放成功或失败
********************************************************************/
void big_packet_free(big_packet_t *pkt)
{
    LEE_ASSERT(pkt != 0);
    LEE_FREE(pkt);
}


