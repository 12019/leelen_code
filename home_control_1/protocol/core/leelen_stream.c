/********************************************************************************
**
** 文件名:     leelen_stream.c
** 版权所有:   (c) 2015 厦门立林科技有限公司
** 文件描述:   实现数据流管理功能
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
#include "leelen_stream.h"



/*******************************************************************
** 函数名:     stream_init
** 函数描述:   初始化数据流
** 参数:       [in]  sp:              数据流
**             [in]  bp:              数据流所管理内存地址
**             [in]  maxlen:          数据流所管理内存字节数
** 返回:       成功或失败
********************************************************************/
bool_t stream_init(stream_t *sp, void *bp, uint32_t maxlen){
    if (sp == 0) {
        return false;
    }
    sp->len      = 0;
    sp->maxlen   = maxlen;
    sp->curptr   = bp;
    sp->startptr = bp;

    return true;
}

/*******************************************************************
** 函数名:     stream_getleftlen
** 函数描述:   获取数据流中剩余的可用字节数
** 参数:       [in]  sp:              数据流
** 返回:       数据流剩余的可用字节数
********************************************************************/
uint32_t stream_getleftlen(stream_t *sp){
    if (sp->maxlen >= sp->len) {
        return (sp->maxlen - sp->len);
    } else {
        return 0;
    }
}

/*******************************************************************
** 函数名:     stream_getlen
** 函数描述:   获取数据流中已用字节数
** 参数:       [in]  sp:              数据流
** 返回:       数据流已用字节数
********************************************************************/
uint32_t stream_getlen(stream_t *sp){
    return (sp->len);
}

/*******************************************************************
** 函数名:     stream_getptr
** 函数描述:   获取数据流当前读/写指针
** 参数:       [in]  sp:              数据流
** 返回:       当前读/写指针
********************************************************************/
void *stream_getptr(stream_t *sp){
    return (sp->curptr);
}

/*******************************************************************
** 函数名:     stream_getstartptr
** 函数描述:   获取数据流所管理内存的地址
** 参数:       [in]  sp:              数据流
** 返回:       所管理内存地址
********************************************************************/
void *stream_getstartptr(stream_t *sp){
    return (sp->startptr);
}

/*******************************************************************
** 函数名:     stream_movptr
** 函数描述:   移动数据流中读/写指针
** 参数:       [in]  sp:              数据流
**             [in]  len:             移动字节数
** 返回:       成功或失败
********************************************************************/
bool_t stream_movptr(stream_t *sp, uint32_t len){
    if (sp != 0) {
        if ((sp->len + len) <= sp->maxlen) {
            sp->len    += len;
            sp->curptr += len;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

/*******************************************************************
** 函数名:     stream_writebyte
** 函数描述:   往数据流中写入一个字节数据
** 参数:       [in]  sp:              数据流
**             [in]  writebyte:       写入的数据
** 返回:       成功或失败
********************************************************************/
bool_t stream_writebyte(stream_t *sp, uint8_t writebyte){
    if (sp != 0){
        if (sp->len < sp->maxlen) {
            *sp->curptr = writebyte;
            sp->curptr += 1;
            sp->len++;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

/*******************************************************************
** 函数名:     stream_writeshort
** 函数描述:   往数据流中写入一个字数据(小端模式)
** 参数:       [in]  sp:              数据流
**             [in]  writeword:       写入的数据
** 返回:       成功或失败
********************************************************************/
bool_t stream_writeshort(stream_t *sp, uint16_t writeword){
    if (sp != 0) {
        if ((sp->len + 2) <= sp->maxlen) {
            stream_writebyte(sp, (uint8_t)(writeword & 0xff));
            stream_writebyte(sp, (uint8_t)(writeword >> 8));
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

/*******************************************************************
** 函数名:     stream_writeint
** 函数描述:   往数据流中写入一个双字数据(小端模式)
** 参数:       [in]  sp:              数据流
**             [in]  writeword:       写入的数据
** 返回:       成功或失败
********************************************************************/
bool_t stream_writeint(stream_t *sp, uint32_t writedword){
    if (sp != 0) {
        if ((sp->len + 4) <= sp->maxlen) {
            stream_writebyte(sp, (uint8_t)(writedword & 0xff));
            stream_writebyte(sp, (uint8_t)(writedword >> 8));
            stream_writebyte(sp, (uint8_t)(writedword >> 16));
            stream_writebyte(sp, (uint8_t)(writedword >> 24));
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

/*******************************************************************
** 函数名:     stream_writelf
** 函数描述:   往数据流中写入字符串
** 参数:       [in]  sp:              数据流
**             [in]  ptr:             写入的字符串指针
** 返回:       成功或失败
********************************************************************/
bool_t stream_writestr(stream_t *sp, char *ptr){
    uint16_t ct;

    ct = 0;
    while (*ptr){
        if (!stream_writebyte(sp, *ptr++)) {
            return false;
        }
        if (++ct > (5*1024)) {
            return false;
        }
    }
    return true;
}

/*******************************************************************
** 函数名:     stream_writedata
** 函数描述:   往数据流中写入一块内存数据
** 参数:       [in]  sp:              数据流
**             [in]  ptr:             写入的数据块地址
**             [in]  len:             写入的数据块字节数
** 返回:       成功或失败
********************************************************************/
bool_t stream_writedata(stream_t *sp, uint8_t *ptr, uint32_t len){
    while (len--){
        if (!stream_writebyte(sp, *ptr++)) {
            return false;
        }
    }
    return true;
}

/*******************************************************************
** 函数名:     stream_readbyte
** 函数描述:   从数据流中读取一个字节
** 参数:       [in]  sp:              数据流
** 返回:       读取到的字节
********************************************************************/
uint8_t stream_readbyte(stream_t *sp){
    uint8_t ret;

    if (sp->len >= sp->maxlen) {
        return 0;
    }
    
    sp->len++;
    ret = *sp->curptr;
    sp->curptr += 1;
    return ret;
}

/*******************************************************************
** 函数名:     stream_readshort
** 函数描述:   从数据流中读取一个字(小端模式)
** 参数:       [in]  sp:              数据流
** 返回:       读取到的字
********************************************************************/
uint16_t stream_readshort(stream_t *sp){
    uint16_t tempw;
    
    tempw   = stream_readbyte(sp);
    tempw  |= stream_readbyte(sp) << 8;
    return tempw;
}

/*******************************************************************
** 函数名:     stream_readint
** 函数描述:   从数据流中读取一个int型数据(小端模式)
** 参数:       [in]  sp:              数据流
** 返回:       读取到的数据
********************************************************************/
uint32_t stream_readint(stream_t *sp){
    uint32_t tempw;
    
    tempw   = stream_readbyte(sp);
    tempw  |= stream_readbyte(sp) << 8;
    tempw  |= stream_readbyte(sp) << 16;
    tempw  |= stream_readbyte(sp) << 24;
    return tempw;
}

/*******************************************************************
** 函数名:     stream_readdata
** 函数描述:   从数据流中读取指定长度的数据内容
** 参数:       [in]  sp:              数据流
**             [out] ptr:             读取到的数据存放的内存地址
**             [in]  len:             读取的数据长度
** 返回:       无
********************************************************************/
void stream_readdata(stream_t *sp, uint8_t *ptr, uint32_t len){
    while (len--){
        *ptr++ = stream_readbyte(sp);
    }
}

