/********************************************************************************
**
** �ļ���:     leelen_protocol.c
** ��Ȩ����:   (c) 2015 �������ֿƼ����޹�˾
** �ļ�����:   ʵ��ͨѶЭ�鹦��
**
*********************************************************************************
**             �޸���ʷ��¼
**===============================================================================
**| ����       | ����   |  �޸ļ�¼
**===============================================================================
**| 2015/09/28 | zzh    |  �������ļ�
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
* ����ģ��ֲ�����
********************************************************************************
*/
static uint8_t    s_version_buf[50];


/*******************************************************************
** ������:     get_version
** ��������:   �汾��,����Э��̽��
** ����:       ��
** ����:       ��
********************************************************************/
uint8_t* get_version(void)
{
    return s_version_buf;
}

/*******************************************************************
** ������:     get_version
** ��������:   �汾��,����Э��̽��
** ����:       [in] ver:  ϵͳ�汾��,����Э��̽��
** ����:       ��
********************************************************************/
void set_version(uint8_t *ver)
{
    uint8_t ver_len;

    ver_len = LEE_MIN(sizeof(s_version_buf) - 1, LEE_STRLEN(ver));
    LEE_MEMCPY(s_version_buf, ver, ver_len);
    s_version_buf[ver_len] = '\0';
}
    
/*******************************************************************
** ������:     init_protocol
** ��������:   ģ���ʼ������
** ����:       [in] ver:  ϵͳ�汾��,����Э��̽��
**            [in]callback���ص�֪ͨ����
** ����:       ��
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
** ������:     uninit_protocol
** ��������:   ģ��ȥʼ������
** ����:       ��
** ����:       ��
********************************************************************/
void uninit_protocol(void)
{
    uninit_udp();
    uninit_big_udp();
}




