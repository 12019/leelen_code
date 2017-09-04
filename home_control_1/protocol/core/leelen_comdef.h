/********************************************************************************
**
** 文件名:     leelen_comdef.h
** 版权所有:   (c) 2015 厦门立林科技有限公司
** 文件描述:   定义程序通用数据
**
*********************************************************************************
**             修改历史记录
**===============================================================================
**| 日期       | 作者   |  修改记录
**===============================================================================
**| 2015/09/28 | zzh    |  创建该文件
**
*********************************************************************************/

#ifndef LEELEN_COMDEF_H
#define LEELEN_COMDEF_H

//#include <android/log.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
********************************************************************************
* 常用变量定义 
********************************************************************************
*/
#ifndef NULL
#define NULL        0
#endif

#ifndef TRUE
#define TRUE        1
#endif

#ifndef FALSE
#define FALSE       0
#endif

#ifndef true
#define true        1
#endif

#ifndef false
#define false       0
#endif

#define LEE_ALIGN4(SIZE)        ((((SIZE)+3)/4)*4)

#define LEE_MIN(d1, d2)         (((d1) < (d2)) ? (d1) : (d2))
#define LEE_MAX(d1, d2)         (((d1) > (d2)) ? (d1) : (d2))
#define LEE_ABS(d1, d2)         ((d1) > (d2)) ? ((d1) - (d2)) : ((d2) - (d1)))

#define LEE_ASSERT(EXP)         assert(EXP)                          


#define LEE_TICK                20       /* 1个tick的定时时间, 单位: ms */
#define LEE_SECOND              1000     /* 1秒的定时时间, 单位: ms */



#define LEE_MEMSET              memset
#define LEE_MEMCPY              memcpy

#define LEE_SPRINTF             sprintf
#define LEE_STRLEN              strlen
#define LEE_STRCPY              strcpy
#define LEE_STRCAT              strcat
#define LEE_STRCMP              strcmp

#define LEE_MALLOC              malloc
#define LEE_FREE                free
#define LEE_LOG                 printf

//#define LEE_LOG(...)            __android_log_print(ANDROID_LOG_INFO, "protocol", __VA_ARGS__)


/*
********************************************************************************
* 数据类型定义
********************************************************************************
*/
typedef unsigned char  bool_t;
typedef unsigned char  uint8_t;                        /* Unsigned  8 bit quantity */
typedef signed   char  int8_t;                         /* Signed    8 bit quantity */
typedef unsigned short uint16_t;                       /* Unsigned 16 bit quantity */
typedef signed   short int16_t;                        /* Signed   16 bit quantity */
typedef unsigned int   uint32_t;                       /* Unsigned 32 bit quantity */
typedef signed   int   int32_t;                        /* Signed   32 bit quantity */


/*
********************************************************************************
* 定义枚举应答
********************************************************************************
*/
typedef enum {
    RESULT_NULL,
    RESULT_SUCCESS,                         /* 成功 */
    RESULT_FAIL,                            /* 失败 */
    RESULT_NOSUPPORT,                       /* 不支持 */
    RESULT_NOMEM,                           /* 内存不足 */
    RESULT_NOTRANS,                         /* 终止传输 */
    RESULT_OVER                             /* 发送超时 */
} RESULT_E;

/*
********************************************************************************
* 定义socket结果
********************************************************************************
*/
typedef enum {
    SOCK_RESULT_ACK,                        /* 接收到正确的socket数据包应答 */
    SOCK_RESULT_NAK,                        /* 接收到错误的socket数据包应答 */
    SOCK_RESULT_ERROR_MEM,                  /* 内存不足出错 */
    SOCK_RESULT_ERROR_SEQ,                  /* 流水号不正确出错 */
    SOCK_RESULT_ERROR_SOCK,                 /* sock错误 */
    SOCK_RESULT_ERROR_OTHER                 /* 其它错误 */
} SOCK_RESULT_E;

/*
********************************************************************************
* 请求应答
********************************************************************************
*/
typedef enum {
    LEE_REQ,                                /* 请求 */
    LEE_ACK                                 /* 应答 */
} ACTION_E;

/*
********************************************************************************
* 大数据类别
********************************************************************************
*/
typedef enum {
    FILE_DATA,                 /* 文件传输 */
    CARD_RECORD,               /* 刷卡记录 */
    PASSWORD_RECORD,           /* 密码开锁记录 */
    PATROL_RECORD,             /* 巡逻卡刷卡记录*/
    TALK_RECORD,               /* 呼叫记录 */
    SECURITY_RECORD,           /* 防区报警记录 */
    INFO_RECORD                /* 信息发送 */
} BIG_DATA_E;

/*
********************************************************************************
* 定义同步头标识
********************************************************************************
*/

#define SYNC_HEAD0  0xd1
#define SYNC_HEAD1  0xd2
#define SYNC_HEAD2  0xd5


/*
********************************************************************************
* 数据包大小
********************************************************************************
*/
#define PKT_DATA_SIZE                       1024
#define PKT_MAX_SIZE                        1200


/*
********************************************************************************
* 协议版本号
********************************************************************************
*/

#define PROTOCOL_VER    0x0301


#ifdef __cplusplus
}
#endif


#endif          /* end of LEELEN_COMDEF_H */


