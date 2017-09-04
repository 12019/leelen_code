/********************************************************************************
**
** 文件名:     leelen_file.c
** 版权所有:   (c) 2015 厦门立林科技有限公司
** 文件描述:   实现文件传输相关通讯协议功能
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
** 函数名:     file_informer
** 函数描述:   数据发送通知函数
** 参数:       [in]  result:        发送结果
** 返回:       无
********************************************************************/
static void file_informer(RESULT_E result)
{
    
    if (s_psb.informer != NULL){
        s_psb.informer(result);
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;
}


/*******************************************************************
** 函数名:     start_file_0x0401
** 函数描述:   文件删除
** 参数:       [in]addr:     目标地址、源地址信息
**             [in]path：    文件路径信息
**             [in]informer：回调通知函数
** 返回:       处理成功true，失败false
********************************************************************/
bool_t start_file_0x0401(addr_info_t *addr, file_path_t *path, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;
    
    len = sizeof(path->dir_namelen) + path->dir_namelen + sizeof(path->file_namelen) + path->file_namelen;
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writebyte(&wstream, path->dir_namelen);
    stream_writedata(&wstream, path->dir_name, path->dir_namelen);
    stream_writebyte(&wstream, path->file_namelen);
    stream_writedata(&wstream, path->file_name, path->file_namelen);

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0401, pkt, 0, 0, file_informer);
}


/*******************************************************************
** 函数名:     stop_file_0x0401
** 函数描述:   停止文件删除
** 参数:       无
** 返回:       无
** 注意:
********************************************************************/
bool_t stop_file_0x0401(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0401);
    return true;
}

/*******************************************************************
** 函数名:     start_file_0x0402
** 函数描述:   获取某路径下的文件列表
** 参数:       [in]addr:     目标地址、源地址信息
**             [in]offset：  文件起始值
**             [in]path：    文件路径信息
**             [in]informer：回调通知函数
** 返回:       处理成功true，失败false
********************************************************************/
bool_t start_file_0x0402(addr_info_t *addr, int offset, file_path_t *path, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;
    
    len = sizeof(offset) + sizeof(path->dir_namelen) + path->dir_namelen;
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writeint(&wstream, offset);
    stream_writebyte(&wstream, path->dir_namelen);
    stream_writedata(&wstream, path->dir_name, path->dir_namelen);

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0402, pkt, 0, 0, file_informer);
}


/*******************************************************************
** 函数名:     stop_file_0x0402
** 函数描述:   停止获取某路径下的文件列表
** 参数:       无
** 返回:       无
** 注意:
********************************************************************/
bool_t stop_file_0x0402(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0402);
    return true;
}


/*******************************************************************
** 函数名:     start_file_0x0403
** 函数描述:   创建目录
** 参数:       [in]addr:     目标地址、源地址信息
**             [in]path：    文件路径信息
**             [in]informer：回调通知函数
** 返回:       处理成功true，失败false
********************************************************************/
bool_t start_file_0x0403(addr_info_t *addr, file_path_t *path, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;
    
    len = sizeof(path->dir_namelen) + path->dir_namelen + sizeof(path->file_namelen) + path->file_namelen;
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writebyte(&wstream, path->dir_namelen);
    stream_writedata(&wstream, path->dir_name, path->dir_namelen);
    stream_writebyte(&wstream, path->file_namelen);                     /* 目录名称长度 */
    stream_writedata(&wstream, path->file_name, path->file_namelen);    /* 目录名称  */

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0403, pkt, 0, 0, file_informer);
}


/*******************************************************************
** 函数名:     stop_file_0x0403
** 函数描述:   停止创建目录
** 参数:       无
** 返回:       无
** 注意:
********************************************************************/
bool_t stop_file_0x0403(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0403);
    return true;
}


/*******************************************************************
** 函数名:     start_file_0x0404
** 函数描述:   重命名文件夹/文件
** 参数:       [in]addr:     目标地址、源地址信息
**             [in]attrib：  文件目录夹或文件
**             [in]old：     旧文件路径信息
**             [in]new：     新文件名
**             [in]informer：回调通知函数
** 返回:       处理成功true，失败false
********************************************************************/
bool_t start_file_0x0404(addr_info_t *addr, uint8_t attrib, file_path_t *old, name_t *new_name, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;
    
    len = sizeof(attrib) + sizeof(old->dir_namelen) + old->dir_namelen + sizeof(old->file_namelen) + old->file_namelen;
    len += sizeof(new_name->name_len) + new_name->name_len; 
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writebyte(&wstream, attrib);
    stream_writebyte(&wstream, old->dir_namelen);
    stream_writedata(&wstream, old->dir_name, old->dir_namelen);
    stream_writebyte(&wstream, old->file_namelen);                     /* 旧文件夹/文件长度 */
    stream_writedata(&wstream, old->file_name, old->file_namelen);     /* 旧文件夹/文件名称  */
    stream_writebyte(&wstream, new_name->name_len);                    /* 新文件夹/文件名称长度 */
    stream_writedata(&wstream, new_name->name, new_name->name_len);    /* 新文件夹/文件名称  */

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0404, pkt, 0, 0, file_informer);
}


/*******************************************************************
** 函数名:     stop_file_0x0404
** 函数描述:   停止重命名文件/目录
** 参数:       无
** 返回:       无
** 注意:
********************************************************************/
bool_t stop_file_0x0404(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0404);
    return true;
}

/*******************************************************************
** 函数名:     start_file_0x0405
** 函数描述:   文件上传(发送文件给对方)
** 参数:       [in]addr:     目标地址、源地址信息
**             [in]path：    文件路径信息
**             [in]length：  文件大小
**             [in]informer：回调通知函数
** 返回:       处理成功true，失败false
********************************************************************/
bool_t start_file_0x0405(addr_info_t *addr, file_path_t *path, int length, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;
    
    len = sizeof(path->dir_namelen) + path->dir_namelen + sizeof(path->file_namelen) + path->file_namelen + sizeof(length);
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writebyte(&wstream, path->dir_namelen);
    stream_writedata(&wstream, path->dir_name, path->dir_namelen);
    stream_writebyte(&wstream, path->file_namelen);                     /* 文件名称长度 */
    stream_writedata(&wstream, path->file_name, path->file_namelen);    /* 文件名称  */
    stream_writeint(&wstream,  length);

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0405, pkt, 0, 0, file_informer);
}


/*******************************************************************
** 函数名:     stop_file_0x0405
** 函数描述:   停止文件上传
** 参数:       无
** 返回:       无
** 注意:
********************************************************************/
bool_t stop_file_0x0405(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0405);
    return true;
}

/*******************************************************************
** 函数名:     start_file_0x0406
** 函数描述:   文件下载(从对方下载文件到本机)
** 参数:       [in]addr:     目标地址、源地址信息
**             [in]path：    文件路径信息
**             [in]informer：回调通知函数
** 返回:       处理成功true，失败false
********************************************************************/
bool_t start_file_0x0406(addr_info_t *addr, file_path_t *path, void (*informer)(RESULT_E))
{
    packet_t  *pkt;
    stream_t  wstream;
    int       len;

    if (s_psb.status != 0) {
        return false;
    }
    
    s_psb.status   = STS_RUNNING;
    s_psb.informer = informer;
    
    len = sizeof(path->dir_namelen) + path->dir_namelen + sizeof(path->file_namelen) + path->file_namelen;
    pkt = packet_alloc(len);
    if (pkt == NULL){
        return false;
    }

    stream_init(&wstream, pkt->prot_body, pkt->body_len);
    stream_writebyte(&wstream, path->dir_namelen);
    stream_writedata(&wstream, path->dir_name, path->dir_namelen);
    stream_writebyte(&wstream, path->file_namelen);                     /* 文件名称长度 */
    stream_writedata(&wstream, path->file_name, path->file_namelen);    /* 文件名称  */

    pkt->req_ack = LEE_REQ;
    addrtobyte(addr->loc_usr, pkt->src);
    addrtobyte(addr->rem_usr, pkt->dest);
    strtoip(&pkt->dest_ip, addr->rem_ip);
    return apply_send_list(0x0406, pkt, 0, 0, file_informer);
}


/*******************************************************************
** 函数名:     stop_file_0x0406
** 函数描述:   停止文件下载
** 参数:       无
** 返回:       无
** 注意:
********************************************************************/
bool_t stop_file_0x0406(void)
{
    if ((s_psb.status & STS_RUNNING) == 0) {
        return false;
    }
    s_psb.status   = 0;
    s_psb.informer =  NULL;

    cancel_send_list(0x0406);
    return true;
}

/*******************************************************************
** 函数名:     hdl_data_0x0401
** 函数描述:   请求协议的处理,文件删除
** 参数:       [in]data:         接收到的数据；
**             [in]data_len：    接收到数据的长度
**             [in/out]ptk:      数据包参数  
** 返回:       1成功;2错误;3不支持;4内存不足;5终止传输
********************************************************************/
static uint8_t hdl_data_0x0401(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    file_path_t path;
    uint8_t  fullpath[100];
    uint8_t  result, len;
    bool_t   ret;
    
    LEE_ASSERT(data != NULL);
    
    LEE_LOG("hdl_data_0x0401");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }
     
        stop_file_0x0401();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        path.dir_namelen   = stream_readbyte(&rstrm);
        path.dir_name  = fullpath;
        if ((path.dir_namelen + 1) > sizeof(fullpath)){
            return 0;
        }
        if (path.dir_namelen > 0){
            stream_readdata(&rstrm, path.dir_name, path.dir_namelen);
            fullpath[path.dir_namelen] = '/';
            path.dir_namelen++;
        }
        path.file_namelen = stream_readbyte(&rstrm);
        if ((path.dir_namelen + path.file_namelen + 1) > sizeof(fullpath)){
            return 0;
        }
        path.file_name = &fullpath[path.dir_namelen];
        stream_readdata(&rstrm, path.file_name, path.file_namelen);
        fullpath[path.dir_namelen + path.file_namelen] = '\0';

        LEE_LOG("fullpath: %s", fullpath);
        ret = file_delete_0x0401(fullpath);
        if (ret){
            ret = RESULT_SUCCESS;
        } else {
            ret = RESULT_FAIL;
        }
        
        return ret;
    }
    return 0;
}

/*******************************************************************
** 函数名:     hdl_data_0x0402
** 函数描述:   请求协议的处理,获取某路径下的文件列表
** 参数:       [in]data:         接收到的数据；
**             [in]data_len：    接收到数据的长度
**             [in/out]ptk:      数据包参数  
** 返回:       1成功;2错误;3不支持;4内存不足;5终止传输
********************************************************************/
static uint8_t hdl_data_0x0402(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm, wstrm;
    file_info_t *file;
    file_path_t path;
    uint8_t  fullpath[100], attrib,i;
    uint8_t  file_num, result, len;
    uint32_t file_length, offset;
    
    LEE_ASSERT(data != NULL);
    
    LEE_LOG("hdl_data_0x0402");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }
        file_num = stream_readbyte(&rstrm);
        for(i = 0; i < file_num; i++){
            path.file_namelen = stream_readbyte(&rstrm);
            path.file_name    = fullpath;
            if ((path.file_namelen + 1) > sizeof(fullpath)){
                return 0;
            }
            stream_readdata(&rstrm, path.file_name, path.file_namelen);
            fullpath[path.file_namelen] = '\0';
            attrib = stream_readbyte(&rstrm);
            file_length = stream_readint(&rstrm);
            LEE_LOG("path: %s %d %d", fullpath, attrib, file_length);
        }
        
        stop_file_0x0402();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        offset = stream_readint(&rstrm);
        path.dir_namelen   = stream_readbyte(&rstrm);
        path.dir_name  = fullpath;
        if ((path.dir_namelen + 1)> sizeof(fullpath)){
            return 0;
        }
        stream_readdata(&rstrm, path.dir_name, path.dir_namelen);
        fullpath[path.dir_namelen] = '\0';

        LEE_LOG("dir: %s %d", fullpath, offset);
        file = (file_info_t *)LEE_MALLOC(100*sizeof(file_info_t));
        if (file == NULL){
            pkt->body_len  = 0;
            return RESULT_NOMEM;
        }
        file_num = file_scan_0x0402(fullpath, offset, 100, file);

        len = sizeof(file_num);
        for(i = 0; i < file_num; i++){
            len += sizeof(file[i].file_namelen);
            len += file[i].file_namelen;
            len += sizeof(file[i].file_attrib);
            len += sizeof(file[i].file_length);
        }
        pkt->body_len = len;
        pkt->prot_body = (uint8_t *)LEE_MALLOC(pkt->body_len);  /* 发送后自动释放 */
        if (pkt->prot_body == NULL){     
            pkt->body_len  = 0;
            pkt->prot_body = NULL;
            return RESULT_NOMEM;
        }

        stream_init(&wstrm, pkt->prot_body, pkt->body_len);
        stream_writebyte(&wstrm, file_num);
        for (i = 0; i < file_num; i++){
            stream_writebyte(&wstrm, file[i].file_namelen);
            stream_writedata(&wstrm, file[i].file_name, file[i].file_namelen);  
            stream_writebyte(&wstrm, file[i].file_attrib);
            stream_writeint(&wstrm, file[i].file_length);
        }

        if (file != NULL){
            LEE_FREE(file);
        }
        return RESULT_SUCCESS;
    }
    return 0;
}

/*******************************************************************
** 函数名:     hdl_data_0x0403
** 函数描述:   请求协议的处理,创建目录
** 参数:       [in]data:         接收到的数据；
**             [in]data_len：    接收到数据的长度
**             [in/out]ptk:      数据包参数  
** 返回:       1成功;2错误;3不支持;4内存不足;5终止传输
********************************************************************/
static uint8_t hdl_data_0x0403(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm, wstrm;
    file_path_t path;
    uint8_t  fullpath[100];
    uint8_t  result, len;
    bool_t   ret;
    
    LEE_ASSERT(data != NULL);
    
    LEE_LOG("hdl_data_0x0403");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }
     
        stop_file_0x0403();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        path.dir_namelen   = stream_readbyte(&rstrm);
        path.dir_name  = fullpath;
        if ((path.dir_namelen + 1) > sizeof(fullpath)){
            return 0;
        }
        if (path.dir_namelen > 0){
            stream_readdata(&rstrm, path.dir_name, path.dir_namelen);
            fullpath[path.dir_namelen] = '/';
            path.dir_namelen++;
        }
        path.file_namelen = stream_readbyte(&rstrm);
        if ((path.dir_namelen + path.file_namelen + 1) > sizeof(fullpath)){
            return 0;
        }
        path.file_name = &fullpath[path.dir_namelen];
        stream_readdata(&rstrm, path.file_name, path.file_namelen);
        fullpath[path.dir_namelen + path.file_namelen] = '\0';

        LEE_LOG("fullpath: %s", fullpath);
        ret = file_mkdir_0x0403(fullpath);
        if (ret){
            ret = RESULT_SUCCESS;
        } else {
            ret = RESULT_FAIL;
        }
        
        return ret;
    }
    return 0;
}

/*******************************************************************
** 函数名:     hdl_data_0x0404
** 函数描述:   请求协议的处理,重命名文件夹/文件
** 参数:       [in]data:         接收到的数据；
**             [in]data_len：    接收到数据的长度
**             [in/out]ptk:      数据包参数  
** 返回:       1成功;2错误;3不支持;4内存不足;5终止传输
********************************************************************/
static uint8_t hdl_data_0x0404(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm, wstrm;
    file_path_t path;
    uint8_t  oldfullpath[100], newfullpath[100];
    uint8_t  result, len;
    bool_t   ret;
    
    LEE_ASSERT(data != NULL);
    
    LEE_LOG("hdl_data_0x0404");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }
     
        stop_file_0x0404();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        path.dir_namelen   = stream_readbyte(&rstrm);
        path.dir_name  = oldfullpath;
        if ((path.dir_namelen + 1) > sizeof(oldfullpath)){
            return 0;
        }
        if (path.dir_namelen > 0){
            stream_readdata(&rstrm, path.dir_name, path.dir_namelen);
            oldfullpath[path.dir_namelen] = '/';
            path.dir_namelen++;
            LEE_MEMCPY(newfullpath, oldfullpath, path.dir_namelen);
        }
        path.file_namelen = stream_readbyte(&rstrm);
        if ((path.dir_namelen + path.file_namelen + 1) > sizeof(oldfullpath)){
            return 0;
        }
        path.file_name = &oldfullpath[path.dir_namelen];
        stream_readdata(&rstrm, path.file_name, path.file_namelen);
        oldfullpath[path.dir_namelen + path.file_namelen] = '\0';

        path.file_namelen = stream_readbyte(&rstrm);
        if ((path.dir_namelen + path.file_namelen + 1) > sizeof(newfullpath)){
            return 0;
        }
        path.file_name = &newfullpath[path.dir_namelen];
        stream_readdata(&rstrm, path.file_name, path.file_namelen);
        newfullpath[path.dir_namelen + path.file_namelen] = '\0';


        LEE_LOG("oldfullpath: %s", oldfullpath);
        LEE_LOG("newfullpath: %s", newfullpath);
        ret = file_rename_0x0404(oldfullpath, newfullpath);
        if (ret){
            ret = RESULT_SUCCESS;
        } else {
            ret = RESULT_FAIL;
        }
        
        return ret;
    }
    return 0;
}

/*******************************************************************
** 函数名:     hdl_data_0x0405
** 函数描述:   请求协议的处理,文件上传(向对方发文件)
** 参数:       [in]data:         接收到的数据；
**             [in]data_len：    接收到数据的长度
**             [in/out]ptk:      数据包参数  
** 返回:       1成功;2错误;3不支持;4内存不足;5终止传输
********************************************************************/
static uint8_t hdl_data_0x0405(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    file_path_t path;
    uint8_t  fullpath[100];
    uint8_t  result, len;
    uint32_t file_namelength;
    bool_t   ret;
    
    LEE_ASSERT(data != NULL);
    
    LEE_LOG("hdl_data_0x0405");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }
       
        stop_file_0x0405();
        if(result == RESULT_SUCCESS){ /* 开始发送文件 */
            
        }
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        path.dir_namelen   = stream_readbyte(&rstrm);
        path.dir_name  = fullpath;
        if ((path.dir_namelen + 1) > sizeof(fullpath)){
            return 0;
        }
        if (path.dir_namelen > 0){
            stream_readdata(&rstrm, path.dir_name, path.dir_namelen);
            fullpath[path.dir_namelen] = '/';
            path.dir_namelen++;
        }
        path.file_namelen = stream_readbyte(&rstrm);
        if ((path.dir_namelen + path.file_namelen + 1) > sizeof(fullpath)){
            return 0;
        }
        path.file_name = &fullpath[path.dir_namelen];
        stream_readdata(&rstrm, path.file_name, path.file_namelen);
        fullpath[path.dir_namelen + path.file_namelen] = '\0';

        
        file_namelength = stream_readint(&rstrm);
        
        LEE_LOG("fullpath: %s", fullpath);
        ret = file_checksize_0x0405(fullpath, file_namelength);
        
        if (ret){
            ret = RESULT_SUCCESS;
        } else {
            ret = RESULT_FAIL;
        }
        return ret;
    }
    return 0;
}

/*******************************************************************
** 函数名:     hdl_data_0x0406
** 函数描述:   请求协议的处理,文件下(向对方要文件)
** 参数:       [in]data:         接收到的数据；
**             [in]data_len：    接收到数据的长度
**             [in/out]ptk:      数据包参数  
** 返回:       1成功;2错误;3不支持;4内存不足;5终止传输
********************************************************************/
static uint8_t hdl_data_0x0406(uint8_t *data, int data_len, packet_t *pkt)
{
    stream_t rstrm;
    file_path_t path;
    uint8_t  fullpath[100];
    uint8_t  result, len;
    bool_t   ret;
    
    LEE_ASSERT(data != NULL);
    
    LEE_LOG("hdl_data_0x0406");
    stream_init(&rstrm, data, data_len);
    if (pkt->req_ack == LEE_ACK){
        if ((s_psb.status & STS_RUNNING) == 0) {
            return 0;
        }
        result = stream_readbyte(&rstrm);
        if (s_psb.informer != NULL){
            s_psb.informer(result);
        }
     
        stop_file_0x0406();
        LEE_LOG("restult = %d", result);
    } else if (pkt->req_ack == LEE_REQ){
        path.dir_namelen   = stream_readbyte(&rstrm);
        path.dir_name  = fullpath;
        if ((path.dir_namelen + 1) > sizeof(fullpath)){
            return 0;
        }
        if (path.dir_namelen > 0){
            stream_readdata(&rstrm, path.dir_name, path.dir_namelen);
            fullpath[path.dir_namelen] = '/';
            path.dir_namelen++;
        }
        path.file_namelen = stream_readbyte(&rstrm);
        if ((path.dir_namelen + path.file_namelen + 1) > sizeof(fullpath)){
            return 0;
        }
        path.file_name = &fullpath[path.dir_namelen];
        stream_readdata(&rstrm, path.file_name, path.file_namelen);
        fullpath[path.dir_namelen + path.file_namelen] = '\0';

        LEE_LOG("fullpath: %s", fullpath);
        ret = file_checkfile_0x0406(fullpath);
        
        if (ret){
            ret = RESULT_SUCCESS;
        } else {
            ret = RESULT_FAIL;
        }
        return ret;
    }
    return 0;
}

static handle_table_t  hdl_tab[] = { 0x0401, false, hdl_data_0x0401,
                                     0x0402, true,  hdl_data_0x0402,
                                     0x0403, false, hdl_data_0x0403,
                                     0x0404, false, hdl_data_0x0404,
                                     0x0405, false, hdl_data_0x0405,
                                     0x0406, false, hdl_data_0x0406
                                   };

/*******************************************************************
** 函数名:     init_file
** 函数描述:   模块初始化函数
** 参数:       无
** 返回:       无
********************************************************************/
void init_file(void)
{
    int i;

    s_psb.status   = 0;
    s_psb.informer =  NULL;

    for (i = 0; i < sizeof(hdl_tab)/sizeof(hdl_tab[0]); i++) {
        regist_pkt_handler(hdl_tab[i].cmd_type, hdl_tab[i].param, hdl_tab[i].handler);
    }

}





