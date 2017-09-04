/********************************************************************************
**
** �ļ���:     leelen_comdef.h
** ��Ȩ����:   (c) 2015 �������ֿƼ����޹�˾
** �ļ�����:   �������ͨ������
**
*********************************************************************************
**             �޸���ʷ��¼
**===============================================================================
**| ����       | ����   |  �޸ļ�¼
**===============================================================================
**| 2015/09/28 | zzh    |  �������ļ�
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
* ���ñ������� 
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


#define LEE_TICK                20       /* 1��tick�Ķ�ʱʱ��, ��λ: ms */
#define LEE_SECOND              1000     /* 1��Ķ�ʱʱ��, ��λ: ms */



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
* �������Ͷ���
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
* ����ö��Ӧ��
********************************************************************************
*/
typedef enum {
    RESULT_NULL,
    RESULT_SUCCESS,                         /* �ɹ� */
    RESULT_FAIL,                            /* ʧ�� */
    RESULT_NOSUPPORT,                       /* ��֧�� */
    RESULT_NOMEM,                           /* �ڴ治�� */
    RESULT_NOTRANS,                         /* ��ֹ���� */
    RESULT_OVER                             /* ���ͳ�ʱ */
} RESULT_E;

/*
********************************************************************************
* ����socket���
********************************************************************************
*/
typedef enum {
    SOCK_RESULT_ACK,                        /* ���յ���ȷ��socket���ݰ�Ӧ�� */
    SOCK_RESULT_NAK,                        /* ���յ������socket���ݰ�Ӧ�� */
    SOCK_RESULT_ERROR_MEM,                  /* �ڴ治����� */
    SOCK_RESULT_ERROR_SEQ,                  /* ��ˮ�Ų���ȷ���� */
    SOCK_RESULT_ERROR_SOCK,                 /* sock���� */
    SOCK_RESULT_ERROR_OTHER                 /* �������� */
} SOCK_RESULT_E;

/*
********************************************************************************
* ����Ӧ��
********************************************************************************
*/
typedef enum {
    LEE_REQ,                                /* ���� */
    LEE_ACK                                 /* Ӧ�� */
} ACTION_E;

/*
********************************************************************************
* ���������
********************************************************************************
*/
typedef enum {
    FILE_DATA,                 /* �ļ����� */
    CARD_RECORD,               /* ˢ����¼ */
    PASSWORD_RECORD,           /* ���뿪����¼ */
    PATROL_RECORD,             /* Ѳ�߿�ˢ����¼*/
    TALK_RECORD,               /* ���м�¼ */
    SECURITY_RECORD,           /* ����������¼ */
    INFO_RECORD                /* ��Ϣ���� */
} BIG_DATA_E;

/*
********************************************************************************
* ����ͬ��ͷ��ʶ
********************************************************************************
*/

#define SYNC_HEAD0  0xd1
#define SYNC_HEAD1  0xd2
#define SYNC_HEAD2  0xd5


/*
********************************************************************************
* ���ݰ���С
********************************************************************************
*/
#define PKT_DATA_SIZE                       1024
#define PKT_MAX_SIZE                        1200


/*
********************************************************************************
* Э��汾��
********************************************************************************
*/

#define PROTOCOL_VER    0x0301


#ifdef __cplusplus
}
#endif


#endif          /* end of LEELEN_COMDEF_H */


