/********************************************************************************
**
** 文件名:     leelen_security.c
** 版权所有:   (c) 2015 厦门立林科技有限公司
** 文件描述:   实现安防相关通讯协议功能
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
** 函数名:     security_informer
** 函数描述:   数据发送通知函数
** 参数:       [in]  result:        发送结果
** 返回:       无
********************************************************************/
static void security_informer(RESULT_E result)
{
    
    if (s_psb.informer != NULL){
        s_psb.informer(result);
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;
}

/*******************************************************************
** 函数名:     start_security_0x0001
** 函数描述:   布撤防记录上传
** 参数:       [in]addr:     目标地址、源地址信息
**             [in]type：    操作类型
**             [in]arrays：  防区组数
**             [in]rec：     布撤防记录
**             [in]informer：回调通知函数
** 返回:       处理成功true，失败false
********************************************************************/
bool_t start_security_0x0001(addr_info_t *addr, uint8_t type, uint8_t arrays, security_record_t *rec, void (*informer)(RESULT_E))
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

    len = arrays*sizeof(rec) + sizeof(type) + sizeof(arrays);
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writebyte(&wstream, type);               /* 操作类型  */
    stream_writebyte(&wstream, arrays);             /* 防区组数  */
    for (i = 0; i < arrays; i++){
        stream_writebyte(&wstream, rec[i].attrib); 
        stream_writebyte(&wstream, rec[i].seq); 
        stream_writebyte(&wstream, rec[i].state); 
    }
    
    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0001, pkt, 0, 0, security_informer);
}

/*******************************************************************
** 函数名:     stop_security_0x0001
** 函数描述:   布撤防记录上传
** 参数:       无
** 返回:       无
** 注意:
********************************************************************/
bool_t stop_security_0x0001(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;
    cancel_send_list(0x0001);
    return true;
}


/*******************************************************************
** 函数名:     start_security_0x0002
** 函数描述:   远程布撤防
** 参数:       [in]addr:     目标地址、源地址信息
**             [in]attrib：  属性
**             [in]state：   状态
**             [in]informer：回调通知函数
** 返回:       处理成功true，失败false
********************************************************************/
bool_t start_security_0x0002(addr_info_t *addr, uint8_t attrib, uint8_t state, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;

    len = sizeof(attrib) + sizeof(state);
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writebyte(&wstream, attrib);               /* 属性  */
    stream_writebyte(&wstream, state);                /* 状态  */
    
    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0002, pkt, 0, 0, security_informer);
}

/*******************************************************************
** 函数名:     stop_security_0x0002
** 函数描述:   停止远程布撤防
** 参数:       无
** 返回:       无
** 注意:
********************************************************************/
bool_t stop_security_0x0002(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;
    cancel_send_list(0x0002);
    return true;
}

/*******************************************************************
** 函数名:     start_security_0x0003
** 函数描述:   防区报警上传
** 参数:       [in]addr:     目标地址、源地址信息
**             [in]area：    防区报警信息
**             [in]pic_num： 图片数量
**             [in]pic：     图片信息
**             [in]informer：回调通知函数
** 返回:       处理成功true，失败false
********************************************************************/
bool_t start_security_0x0003(addr_info_t *addr, area_record_t *area, uint8_t pic_num, name_t *pic, void (*informer)(RESULT_E))
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
    
    len  = sizeof(area->area_type) + sizeof(area->area_num) + sizeof(area->area_name);
    len += sizeof(area->cam_channel) + sizeof(area->area_attrib) + sizeof(area->area_len);
    len += area->area_len + sizeof(area->alarm_time) + sizeof(pic_num);
    for(i = 0; i < pic_num; i++){
        len += sizeof(pic[i].name_len);
        len += pic[i].name_len;
    }
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writebyte(&wstream, area->area_type);              
    stream_writebyte(&wstream, area->area_num);
    stream_writebyte(&wstream, area->area_name);
    stream_writebyte(&wstream, area->cam_channel);
    stream_writebyte(&wstream, area->area_attrib);
    stream_writebyte(&wstream, area->area_len);
    stream_writedata(&wstream, area->area_addr, area->area_len);
    stream_writedata(&wstream, area->alarm_time, sizeof(area->alarm_time));

    stream_writebyte(&wstream, pic_num);              
    for (i = 0; i < pic_num; i++){
        stream_writebyte(&wstream, pic[i].name_len); 
        stream_writedata(&wstream, pic[i].name, pic[i].name_len);
    }

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);

    return apply_send_list(0x0003, pkt, 0, 0, security_informer);
}


/*******************************************************************
** 函数名:     stop_security_0x0003
** 函数描述:   停止防区报警上传
** 参数:       无
** 返回:       无
** 注意:
********************************************************************/
bool_t stop_security_0x0003(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0003);
    return true;
}


/*******************************************************************
** 函数名:     start_security_0x0004
** 函数描述:   紧急求助
** 参数:       [in]addr:     目标地址、源地址信息
**             [in]informer：回调通知函数
** 返回:       处理成功true，失败false
********************************************************************/
bool_t start_security_0x0004(addr_info_t *addr, void (*informer)(RESULT_E))
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
    return apply_send_list(0x0004, pkt, 0, 0, security_informer);
}


/*******************************************************************
** 函数名:     stop_security_0x0004
** 函数描述:   停止紧急求助
** 参数:       无
** 返回:       无
** 注意:
********************************************************************/
bool_t stop_security_0x0004(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0004);
    return true;
}


/*******************************************************************
** 函数名:     hdl_data_0x0001
** 函数描述:   请求协议的处理
** 参数:       [in]data:         接收到的数据；
**             [in]data_len：    接收到数据的长度
**             [in/out]ptk:      数据包参数  
** 返回:       1成功;2错误;3不支持;4内存不足;5终止传输
********************************************************************/
static uint8_t hdl_data_0x0001(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  result, operate, arrays, i;
    security_record_t rec;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0001");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_security_0x0001();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        
        operate = stream_readbyte(&rstrm);
        arrays  = stream_readbyte(&rstrm);
        for (i = 0; i < arrays; i++){
            rec.attrib = stream_readbyte(&rstrm);
            rec.seq    = stream_readbyte(&rstrm);
            rec.state  = stream_readbyte(&rstrm);
            write_security_record_0x0001(operate, i, &rec);
        }
        
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** 函数名:     hdl_data_0x0102
** 函数描述:   请求协议的处理
** 参数:       [in]data:         接收到的数据；
**             [in]data_len：    接收到数据的长度
**             [in/out]ptk:      数据包参数  
** 返回:       1成功;2错误;3不支持;4内存不足;5终止传输
********************************************************************/
static uint8_t hdl_data_0x0002(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  result, attrib, state;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0002");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_security_0x0002();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        
        attrib = stream_readbyte(&rstrm);
        state  = stream_readbyte(&rstrm);
        write_security_record_0x0002(attrib, state);
        LEE_LOG("attrib = %d state = %d", attrib, state);
        return RESULT_SUCCESS;
    }
    return 0;
}



/*******************************************************************
** 函数名:     hdl_data_0x0003
** 函数描述:   请求协议的处理
** 参数:       [in]data:         接收到的数据；
**             [in]data_len：    接收到数据的长度
**             [in/out]ptk:      数据包参数  
** 返回:       1成功;2错误;3不支持;4内存不足;5终止传输
********************************************************************/
static uint8_t hdl_data_0x0003(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  result, pic_num, i;
    uint8_t  addr[60], name[60];
    area_record_t area;
    name_t pic;
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0003");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_security_0x0003();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        LEE_MEMSET(area.area_user, 0, sizeof(area.area_user));
        bytetoaddr(pkt->src, area.area_user);
        LEE_MEMSET(area.area_ip, 0, sizeof(area.area_ip));
        iptostr(area.area_ip, pkt->dest_ip);

        area.area_type  = stream_readbyte(&rstrm);
        area.area_num   = stream_readbyte(&rstrm);
        area.area_name   = stream_readbyte(&rstrm);
        area.cam_channel= stream_readbyte(&rstrm);
        area.area_attrib= stream_readbyte(&rstrm);
        area.area_len   = stream_readbyte(&rstrm);
        area.area_addr  = addr;
        stream_readdata(&rstrm, area.area_addr, area.area_len);
        LEE_LOG("addr %s", area.area_addr);
        stream_readdata(&rstrm, area.alarm_time, sizeof(area.alarm_time));
        pic_num = stream_readbyte(&rstrm);
        if (pic_num == 0){
            write_security_record_0x0003(&area, 0, NULL);
        }
        for (i = 0; i < pic_num; i++){
            pic.name_len = stream_readbyte(&rstrm);
            pic.name = name;
            stream_readdata(&rstrm, pic.name, pic.name_len);
            LEE_LOG("picname %s", pic.name);
            write_security_record_0x0003(&area, i, &pic);
        }
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** 函数名:     hdl_data_0x0004
** 函数描述:   请求协议的处理
** 参数:       [in]data:         接收到的数据；
**             [in]data_len：    接收到数据的长度
**             [in/out]ptk:      数据包参数  
** 返回:       1成功;2错误;3不支持;4内存不足;5终止传输
********************************************************************/
static uint8_t hdl_data_0x0004(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    uint8_t  src_str[12], dest_str[12];
    uint8_t  result, dest_ip[20];
        
    LEE_ASSERT(data != NULL || data_len != 0);
    
    LEE_LOG("hdl_data_0x0004");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }

        stop_security_0x0004();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        
        LEE_MEMSET(src_str, 0, sizeof(src_str));
        bytetoaddr(pkt->src, src_str);

        LEE_MEMSET(dest_str, 0, sizeof(dest_str));
        bytetoaddr(pkt->dest, dest_str);
        
        LEE_LOG("src = %s dest = %s", src_str, dest_str);

        LEE_MEMSET(dest_ip, 0, sizeof(dest_ip));
        iptostr(dest_ip, pkt->dest_ip);
        write_security_record_0x0004(src_str, dest_ip);
        return RESULT_SUCCESS;
    }
    return 0;
}


static handle_table_t  hdl_tab[] = { 0x0001, false, hdl_data_0x0001,
                                     0x0002, false, hdl_data_0x0002,
                                     0x0003, false, hdl_data_0x0003,
                                     0x0004, false, hdl_data_0x0004,
                                   };

/*******************************************************************
** 函数名:     init_security
** 函数描述:   模块初始化函数
** 参数:       无
** 返回:       无
********************************************************************/
void init_security(void)
{
    int i;
    
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    for (i = 0; i < sizeof(hdl_tab)/sizeof(hdl_tab[0]); i++) {
        regist_pkt_handler(hdl_tab[i].cmd_type, hdl_tab[i].param,  hdl_tab[i].handler);
    }

}






