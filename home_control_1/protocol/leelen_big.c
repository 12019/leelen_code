/********************************************************************************
**
** 文件名:     leelen_big.c
** 版权所有:   (c) 2015 厦门立林科技有限公司
** 文件描述:   实现大数据通讯协议功能
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
#include "leelen_stream.h"
#include "leelen_protocol.h"
#include "leelen_big.h"


/*
********************************************************************************
* 定义模块局部变量
********************************************************************************
*/
static protoco_send_t  s_psb;


/*******************************************************************
** 函数名:     big_informer
** 函数描述:   数据发送通知函数
** 参数:       [in]  result:        发送结果
** 返回:       无
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
** 函数名:     start_big_0x0601
** 函数描述:   大数据记录上传
** 参数:       [in]addr:     目标地址、源地址信息
**             [in]type：    大数据类型
**             [in]file：    大数据路径信息
**             [in]len：     大数据大小
**             [in]informer：回调通知函数
** 返回:       处理成功true，失败false
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
** 函数名:     stop_big_0x0601
** 函数描述:   停止大数据记录上传
** 参数:       无
** 返回:       无
** 注意:
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
** 函数名:     hdl_big_data_0x0601
** 函数描述:   请求协议的处理
** 参数:       [in]data:         接收到的数据；
**             [in]data_len：    接收到数据的长度
**             [in/out]ptk:      数据包参数  
** 返回:       1成功;2错误;3不支持;4内存不足;5终止传输
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
            LEE_MEMCPY(fullpath, pkt->dir_name, pkt->dir_namelen);  /* 路径名称 */
            fullpath[pkt->dir_namelen] = '/';
            dirpath_len++;
        }
        LEE_MEMCPY(&fullpath[dirpath_len], pkt->file_name, pkt->file_namelen);
        fullpath[dirpath_len + pkt->file_namelen] = '\0';

        pkt->file_length= stream_readint(&rstrm);
        pkt->total_pkt = stream_readint(&rstrm);                   /* 包数量 */
        pkt->cur_pktid = stream_readint(&rstrm);
        pkt->cur_size  = stream_readint(&rstrm); 
        
        offset = (pkt->cur_pktid - 1) * PKT_DATA_SIZE;             /* 包序号从1开始 */
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
** 函数名:     init_big
** 函数描述:   模块初始化函数
** 参数:       无
** 返回:       无
********************************************************************/
void init_big(void)
{
    s_psb.status   = 0;
    s_psb.informer =  NULL;
}






