/********************************************************************************
**
** 文件名:     leelen_protocol.c
** 版权所有:   (c) 2015 厦门立林科技有限公司
** 文件描述:   实现通讯协议功能
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
#include "leelen_timer.h"
#include "leelen_udp.h"
#include "leelen_big_packet.h"
#include "leelen_big_udp.h"
#include "leelen_send_list.h"
#include "leelen_big_list.h"
#include "leelen_protocol.h"
#include "leelen_security.h"
#include "leelen_card.h"
#include "leelen_lift.h"
#include "leelen_talk.h"
#include "leelen_file.h"
#include "leelen_system.h"
#include "leelen_big.h"

udp_callback g_udp_callback;

/*
********************************************************************************
* 定义模块局部变量
********************************************************************************
*/
static uint8_t    s_version_buf[50];


/*******************************************************************
** 函数名:     get_version
** 函数描述:   版本号,用于协议探测
** 参数:       无
** 返回:       无
********************************************************************/
uint8_t* get_version(void)
{
    return s_version_buf;
}

/*******************************************************************
** 函数名:     get_version
** 函数描述:   版本号,用于协议探测
** 参数:       [in] ver:  系统版本号,用于协议探测
** 返回:       无
********************************************************************/
void set_version(uint8_t *ver)
{
    uint8_t ver_len;

    ver_len = LEE_MIN(sizeof(s_version_buf) - 1, LEE_STRLEN(ver));
    LEE_MEMCPY(s_version_buf, ver, ver_len);
    s_version_buf[ver_len] = '\0';
}
    
/*******************************************************************
** 函数名:     init_protocol
** 函数描述:   模块初始化函数
** 参数:       [in] ver:  系统版本号,用于协议探测
**            [in]callback：回调通知函数
** 返回:       无
********************************************************************/
void init_protocol(char *version,udp_callback callback)
{
    g_udp_callback = callback;

    set_version(version);
    init_timer();
    init_hdlpkt();
    init_udp();
    init_big_udp();
    init_send_list();
    init_big_list();
        
    init_security();
    init_card();
    init_lift();
    init_talk();
    init_file();
    init_system();

    init_big();
 }


/*******************************************************************
** 函数名:     uninit_protocol
** 函数描述:   模块去始化函数
** 参数:       无
** 返回:       无
********************************************************************/
void uninit_protocol(void)
{
    uninit_udp();
    uninit_big_udp();
}




