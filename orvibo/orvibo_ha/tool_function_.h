/**
 * Copyrights (C) 2016
 * All right reserved
 *
 * File : tool_function_
 *
 * 辅助功能函数
 *
 * Change Logs:
 * Date         Author      Notes
 * 2016-12-08   wangmaopeng     Create
 */

#ifndef __TOOL_FUNCTION__
#define __TOOL_FUNCTION__

/**************************************************************************
*                                 INCLUDES
**************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<netdb.h>
#include<string.h>
#include<assert.h>
#include<net/if.h>

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
*                                 DECLARES
**************************************************************************/
extern int enable_showlog;

/**************************************************************************
*                             TYPE DEFINES
**************************************************************************/
typedef unsigned char  bool_t;
typedef unsigned char  uint8_t;                        /* Unsigned  8 bit quantity */
typedef signed char  int8_t;                         /* Signed    8 bit quantity */
typedef unsigned short  uint16_t;                       /* Unsigned 16 bit quantity */
typedef signed short  int16_t;                        /* Signed   16 bit quantity */
typedef unsigned int  uint32_t;                       /* Unsigned 32 bit quantity */
typedef signed int  int32_t;                        /* Signed   32 bit quantity */

/**************************************************************************
*                               FUNCTIONS
**************************************************************************/

/*******************************************************************
** 函数名:     open_log
** 函数描述:   开启打印信息
** 参数:
** 返回:
********************************************************************/
int  open_log_();

/*******************************************************************
** 函数名:     close_log
** 函数描述:   关闭打印信息
** 参数:
** 返回:
********************************************************************/
int  close_log_();

/*******************************************************************
** 函数名:     hex_to_char
** 函数描述:   将hex数据转换成字符, 例如: 将0x0a转换成字符'A'
** 参数:       [unsigned char]  sbyte:           待转换的hex数据
** 返回:       转换后的字符
** 注意:       sbyte的取值范围为: 0x00~0x0F
********************************************************************/
char hex_to_char_(unsigned char sbyte);

/*******************************************************************
** 函数名:     show_log_hex_
** 函数描述:   将缓冲区中的数据按hex格式输出
** 参数:       [in]  str:             在待打印的缓冲区数据前附加打印的字符串
**             [in]  ptr:             待打印的数据缓冲区
**             [in]  len:             待打印的数据缓冲区长度
** 返回:       无
********************************************************************/
void  show_log_hex_(char *str, char *ptr, int len);

/*******************************************************************
** 函数名:     show_log_char_
** 函数描述:   将缓冲区中的数据按char格式输出
** 参数:       [in]  str:             在待打印的缓冲区数据前附加打印的字符串
**             [in]  ptr:             待打印的数据缓冲区
** 返回:       无
********************************************************************/
void  show_log_char_(char *str, char *ptr);

/*******************************************************************
** 函数名:     show_log_int_
** 函数描述:   将缓冲区中的数据按int格式输出
** 参数:       [in]  str:             在待打印的缓冲区数据前附加打印的字符串
**             [in]  ptr:             待打印的数据缓冲区
** 返回:       无
********************************************************************/
void  show_log_int_(char *str, int ptr);

/*******************************************************************
** 函数名:     get_wlan0_ip_addr
** 函数描述:   获取无线网卡wlan0 ip地址
** 参数:       [out]  addr:获取到的ip地址
** 返回:       成功0, 失败-1
********************************************************************/
int  get_wlan0_ip_addr_(unsigned char *addr);

/*******************************************************************
** 函数名:     hex_to_str_
** 函数描述:   将指定个数的数值型16进制的数据转换成字符表示的数据
** 参数:       [in] str :  如:0x77 0x67 0x57 0x47 0x37
**            [out] ptr  :  对应字符串 "7767574737"
**            [in] len   : 转换的字节数
** 返回:     成功true, 失败false
** 注意:
********************************************************************/
int  hex_to_str_(char *src,char *dst,int len);

#ifdef __cplusplus
}
#endif

#endif //__TOOL_FUNCTION__
