/********************************************************************************
**
** �ļ���:     leelen_stream.c
** ��Ȩ����:   (c) 2015 �������ֿƼ����޹�˾
** �ļ�����:   ʵ��������������
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
#include "leelen_stream.h"



/*******************************************************************
** ������:     stream_init
** ��������:   ��ʼ��������
** ����:       [in]  sp:              ������
**             [in]  bp:              �������������ڴ��ַ
**             [in]  maxlen:          �������������ڴ��ֽ���
** ����:       �ɹ���ʧ��
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
** ������:     stream_getleftlen
** ��������:   ��ȡ��������ʣ��Ŀ����ֽ���
** ����:       [in]  sp:              ������
** ����:       ������ʣ��Ŀ����ֽ���
********************************************************************/
uint32_t stream_getleftlen(stream_t *sp){
    if (sp->maxlen >= sp->len) {
        return (sp->maxlen - sp->len);
    } else {
        return 0;
    }
}

/*******************************************************************
** ������:     stream_getlen
** ��������:   ��ȡ�������������ֽ���
** ����:       [in]  sp:              ������
** ����:       �����������ֽ���
********************************************************************/
uint32_t stream_getlen(stream_t *sp){
    return (sp->len);
}

/*******************************************************************
** ������:     stream_getptr
** ��������:   ��ȡ��������ǰ��/дָ��
** ����:       [in]  sp:              ������
** ����:       ��ǰ��/дָ��
********************************************************************/
void *stream_getptr(stream_t *sp){
    return (sp->curptr);
}

/*******************************************************************
** ������:     stream_getstartptr
** ��������:   ��ȡ�������������ڴ�ĵ�ַ
** ����:       [in]  sp:              ������
** ����:       �������ڴ��ַ
********************************************************************/
void *stream_getstartptr(stream_t *sp){
    return (sp->startptr);
}

/*******************************************************************
** ������:     stream_movptr
** ��������:   �ƶ��������ж�/дָ��
** ����:       [in]  sp:              ������
**             [in]  len:             �ƶ��ֽ���
** ����:       �ɹ���ʧ��
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
** ������:     stream_writebyte
** ��������:   ����������д��һ���ֽ�����
** ����:       [in]  sp:              ������
**             [in]  writebyte:       д�������
** ����:       �ɹ���ʧ��
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
** ������:     stream_writeshort
** ��������:   ����������д��һ��������(С��ģʽ)
** ����:       [in]  sp:              ������
**             [in]  writeword:       д�������
** ����:       �ɹ���ʧ��
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
** ������:     stream_writeint
** ��������:   ����������д��һ��˫������(С��ģʽ)
** ����:       [in]  sp:              ������
**             [in]  writeword:       д�������
** ����:       �ɹ���ʧ��
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
** ������:     stream_writelf
** ��������:   ����������д���ַ���
** ����:       [in]  sp:              ������
**             [in]  ptr:             д����ַ���ָ��
** ����:       �ɹ���ʧ��
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
** ������:     stream_writedata
** ��������:   ����������д��һ���ڴ�����
** ����:       [in]  sp:              ������
**             [in]  ptr:             д������ݿ��ַ
**             [in]  len:             д������ݿ��ֽ���
** ����:       �ɹ���ʧ��
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
** ������:     stream_readbyte
** ��������:   ���������ж�ȡһ���ֽ�
** ����:       [in]  sp:              ������
** ����:       ��ȡ�����ֽ�
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
** ������:     stream_readshort
** ��������:   ���������ж�ȡһ����(С��ģʽ)
** ����:       [in]  sp:              ������
** ����:       ��ȡ������
********************************************************************/
uint16_t stream_readshort(stream_t *sp){
    uint16_t tempw;
    
    tempw   = stream_readbyte(sp);
    tempw  |= stream_readbyte(sp) << 8;
    return tempw;
}

/*******************************************************************
** ������:     stream_readint
** ��������:   ���������ж�ȡһ��int������(С��ģʽ)
** ����:       [in]  sp:              ������
** ����:       ��ȡ��������
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
** ������:     stream_readdata
** ��������:   ���������ж�ȡָ�����ȵ���������
** ����:       [in]  sp:              ������
**             [out] ptr:             ��ȡ�������ݴ�ŵ��ڴ��ַ
**             [in]  len:             ��ȡ�����ݳ���
** ����:       ��
********************************************************************/
void stream_readdata(stream_t *sp, uint8_t *ptr, uint32_t len){
    while (len--){
        *ptr++ = stream_readbyte(sp);
    }
}

