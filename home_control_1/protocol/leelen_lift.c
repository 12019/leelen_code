/********************************************************************************
**
** 文件名:     leelen_lift.c
** 版权所有:   (c) 2015 厦门立林科技有限公司
** 文件描述:   实现梯控通讯协议功能
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
* 定义模块局部变量
********************************************************************************
*/
static protoco_send_t  s_psb;

/*******************************************************************
** 函数名:     lift_informer
** 函数描述:   数据发送通知函数
** 参数:       [in]  result:        发送结果
** 返回:       无
********************************************************************/
static void lift_informer(RESULT_E result)
{
    
    if (s_psb.informer != NULL){
        s_psb.informer(result);
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;
}



/*******************************************************************
** 函数名:     start_lift_0x0201
** 函数描述:   开始呼梯
** 参数:       [in]addr:     目标地址、源地址信息
**             [in]dir:      方向信息
**             [in]floor:    起始楼层信息
**             [in]informer：回调通知函数
** 返回:       处理成功true，失败false
********************************************************************/
bool_t start_lift_0x0201(addr_info_t *addr, uint8_t dir, floor_info_t *floor, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;

    len = sizeof(dir) + sizeof(floor->src_floor);
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writebyte(&wstream, dir);               /* 电梯方向  */
    stream_writedata(&wstream, floor->src_floor, sizeof(floor->src_floor)); 

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    
    return apply_send_list(0x0201, pkt, 0, 0, lift_informer);
}


/*******************************************************************
** 函数名:     stop_lift_0x0201
** 函数描述:   停止呼梯
** 参数:       无
** 返回:       无
** 注意:
********************************************************************/
bool_t stop_lift_0x0201(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0201);
    return true;
}


/*******************************************************************
** 函数名:     start_lift_0x0202
** 函数描述:   户户互访梯控
** 参数:       [in]addr:     目标地址、源地址信息
**             [in]floor:    楼层信息
**             [in]informer：回调通知函数
** 返回:       处理成功true，失败false
********************************************************************/
bool_t start_lift_0x0202(addr_info_t *addr, floor_info_t *floor, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;

    len = sizeof(floor->src_floor) + sizeof(floor->dest_floor);
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writedata(&wstream, floor->src_floor, sizeof(floor->src_floor)); 
    stream_writedata(&wstream, floor->dest_floor, sizeof(floor->dest_floor));

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0202, pkt, 0, 0, lift_informer);
}


/*******************************************************************
** 函数名:     stop_lift_0x0202
** 函数描述:   停止户户互访梯控
** 参数:       无
** 返回:       无
** 注意:
********************************************************************/
bool_t stop_lift_0x0202(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0202);
    return true;
}


/*******************************************************************
** 函数名:     start_lift_0x0203
** 函数描述:   刷卡梯控
** 参数:       [in]addr:     目标地址、源地址信息
**             [in]floor:    楼层信息
**             [in]informer：回调通知函数
** 返回:       处理成功true，失败false
********************************************************************/
bool_t start_lift_0x0203(addr_info_t *addr, floor_info_t *floor, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;

    len = sizeof(floor->src_floor) + sizeof(floor->dest_floor);
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writedata(&wstream, floor->src_floor, sizeof(floor->src_floor)); 
    stream_writedata(&wstream, floor->dest_floor, sizeof(floor->dest_floor));

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0203, pkt, 0, 0, lift_informer);
}


/*******************************************************************
** 函数名:     stop_lift_0x0203
** 函数描述:   停止刷卡梯控
** 参数:       无
** 返回:       无
** 注意:
********************************************************************/
bool_t stop_lift_0x0203(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0203);
    return true;
}

/*******************************************************************
** 函数名:     start_lift_0x0204
** 函数描述:   电梯状态查询
** 参数:       [in]addr:     目标地址、源地址信息
**             [in]informer：回调通知函数
** 返回:       处理成功true，失败false
********************************************************************/
bool_t start_lift_0x0204(addr_info_t *addr, void (*informer)(RESULT_E))
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
    return apply_send_list(0x0204, pkt, 0, 0, lift_informer);
}

/*******************************************************************
** 函数名:     stop_lift_0x0204
** 函数描述:   停止电梯状态查询
** 参数:       无
** 返回:       无
** 注意:
********************************************************************/
bool_t stop_lift_0x0204(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0204);
    return true;
}

/*******************************************************************
** 函数名:     hdl_data_0x0201
** 函数描述:   请求协议的处理,呼梯
** 参数:       [in]data:         接收到的数据；
**             [in]data_len：    接收到数据的长度
**             [in/out]ptk:      数据包参数  
** 返回:       1成功;2错误;3不支持;4内存不足;5终止传输
********************************************************************/
static uint8_t hdl_data_0x0201(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  result, dir, floor[5], floorstr[12];
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0201");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_lift_0x0201();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        
        dir = stream_readbyte(&rstrm);
        stream_readdata(&rstrm, floor, sizeof(floor));
        LEE_MEMSET(floorstr, 0, sizeof(floorstr));
        bytetoaddr(floor, floorstr);
        LEE_LOG("dir = %d s = %s", dir, floorstr);
        
        call_lift_0x0201(dir, floor);
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** 函数名:     hdl_data_0x0102
** 函数描述:   请求协议的处理,梯控
** 参数:       [in]data:         接收到的数据；
**             [in]data_len：    接收到数据的长度
**             [in/out]ptk:      数据包参数  
** 返回:       1成功;2错误;3不支持;4内存不足;5终止传输
********************************************************************/
static uint8_t hdl_data_0x0202(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  src_floor[5], src_floorstr[12];
    uint8_t  dest_floor[5], dest_floorstr[12];
    uint8_t  result;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0202");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_lift_0x0202();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        
        stream_readdata(&rstrm, src_floor, sizeof(src_floor));
        stream_readdata(&rstrm, dest_floor, sizeof(dest_floor));

        LEE_MEMSET(src_floorstr, 0, sizeof(src_floorstr));
        bytetoaddr(src_floor, src_floorstr);

        LEE_MEMSET(dest_floorstr, 0, sizeof(dest_floorstr));
        bytetoaddr(dest_floor, dest_floorstr);

        call_lift_0x0202(src_floor, dest_floor);
        LEE_LOG("sfloor = %s dfloor = %s", src_floorstr, dest_floorstr);
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** 函数名:     hdl_data_0x0203
** 函数描述:   请求协议的处理,刷卡梯控
** 参数:       [in]data:         接收到的数据；
**             [in]data_len：    接收到数据的长度
**             [in/out]ptk:      数据包参数  
** 返回:       1成功;2错误;3不支持;4内存不足;5终止传输
********************************************************************/
static uint8_t hdl_data_0x0203(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  src_floor[5], src_floorstr[12];
    uint8_t  dest_floor[5], dest_floorstr[12];
    uint8_t  result;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0203");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_lift_0x0203();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        
        stream_readdata(&rstrm, src_floor, sizeof(src_floor));
        stream_readdata(&rstrm, dest_floor, sizeof(dest_floor));
        
        LEE_MEMSET(src_floorstr, 0, sizeof(src_floorstr));
        bytetoaddr(src_floor, src_floorstr);

        LEE_MEMSET(dest_floorstr, 0, sizeof(dest_floorstr));
        bytetoaddr(dest_floor, dest_floorstr);
        call_lift_0x0203(src_floor, dest_floor);
        LEE_LOG("sfloor = %s dfloor = %s", src_floorstr, dest_floorstr);
        return RESULT_SUCCESS;
    }
    return 0;
}


/*******************************************************************
** 函数名:     hdl_data_0x0204
** 函数描述:   请求协议的处理,电梯状态查询
** 参数:       [in]data:         接收到的数据；
**             [in]data_len：    接收到数据的长度
**             [in/out]ptk:      数据包参数  
** 返回:       1成功;2错误;3不支持;4内存不足;5终止传输
********************************************************************/
static uint8_t hdl_data_0x0204(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm, wstrm;
    uint8_t  result, num, len, i;
    lift_info_t lift[5];
        
    LEE_ASSERT(data != NULL);
    
    LEE_LOG("hdl_data_0x0204");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        num    = stream_readbyte(&rstrm);
        num    = LEE_MIN(num, sizeof(lift)/sizeof(lift[0]));
        for (i = 0; i < num; i++){
            lift[i].state = stream_readbyte(&rstrm);
            lift[i].dir   = stream_readbyte(&rstrm);
            lift[i].floor = stream_readbyte(&rstrm);
            LEE_LOG("lift: %d %d %d", lift[i].state, lift[i].dir, lift[i].floor);
        }
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_lift_0x0204();
        set_lift_info_0x0204(lift, num);
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        num = sizeof(lift)/sizeof(lift[0]);
        
        get_lift_info_0x0204(lift, &num);
        len = sizeof(uint8_t);
        for (i = 0; i < num; i++){
            len += sizeof(lift->state);
            len += sizeof(lift->dir);
            len += sizeof(lift->floor);
        }
    
        pkt->body_len = len;
        pkt->prot_body = (uint8_t *)LEE_MALLOC(pkt->body_len);  /* 发送后自动释放 */
        if (pkt->prot_body == NULL){     
            pkt->body_len  = 0;
            pkt->prot_body = NULL;
            return RESULT_NOMEM;
        }
        stream_init(&wstrm, pkt->prot_body, pkt->body_len);
        stream_writebyte(&wstrm, num);  
        for (i = 0; i < num; i++){
            stream_writebyte(&wstrm, lift[i].state); 
            stream_writebyte(&wstrm, lift[i].dir); 
            stream_writebyte(&wstrm, lift[i].floor); 
        }
        if (num == 0){
            return RESULT_NOSUPPORT;
        }
        return RESULT_SUCCESS;
    }
    return 0;
}

static handle_table_t  hdl_tab[] = { 0x0201, false, hdl_data_0x0201,
                                     0x0202, false, hdl_data_0x0202,
                                     0x0203, false, hdl_data_0x0203,
                                     0x0204, true,  hdl_data_0x0204
                                   };

/*******************************************************************
** 函数名:     init_lift
** 函数描述:   模块初始化函数
** 参数:       无
** 返回:       无
********************************************************************/
void init_lift(void)
{
    int i;

    s_psb.status   = 0;
    s_psb.informer =  NULL;

    for (i = 0; i < sizeof(hdl_tab)/sizeof(hdl_tab[0]); i++) {
        regist_pkt_handler(hdl_tab[i].cmd_type, hdl_tab[i].param, hdl_tab[i].handler);
    }

}






