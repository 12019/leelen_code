/********************************************************************************
**
** �ļ���:     leelen_utils.c
** ��Ȩ����:   (c) 2015 �������ֿƼ����޹�˾
** �ļ�����:   ʵ�ֹ����ຯ��
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
#include "leelen_utils.h"

static char   s_debugmem[1024];

/*******************************************************************
** ������:     loghex
** ��������:   ���������е����ݰ�hex��ʽ���
** ����:       [in]  str:             �ڴ���ӡ�Ļ���������ǰ���Ӵ�ӡ���ַ���
**             [in]  ptr:             ����ӡ�����ݻ�����
**             [in]  len:             ����ӡ�����ݻ���������
** ����:       ��
********************************************************************/
void loghex(char *str, char *ptr, int len){
    int i;
    char  ch;

    i = LEE_STRLEN(str);
    if (i > sizeof(s_debugmem)) {
        return;
    }
    LEE_MEMCPY(&s_debugmem[0], str, i);

    for (; len > 0; len--) {
        ch = *ptr++;
        if ((i + 3) > sizeof(s_debugmem)) {
            break;
        }
        s_debugmem[i++] = hextochar((char)(ch >> 4));
        s_debugmem[i++] = hextochar(ch);
        s_debugmem[i++] = ' ';
    }
    s_debugmem[i] = '\0';
    LEE_LOG("%s\n", s_debugmem);
}

/*******************************************************************
** ������:     inttobyte(С��ģʽ)
** ��������:   ��int���͵�����src�����ֽڵ����ֽڵ�˳������dest��ָ�Ļ�������
** ����:       [out] dest:            ������ָ��
**             [in]  src:             int��������
** ����:       ��
********************************************************************/
void inttobyte(uint8_t *dest, uint32_t src)
{
    dest[0] = (uint8_t)(src);
    dest[1] = (uint8_t)(src >> 8);
    dest[2] = (uint8_t)(src >> 16);
    dest[3] = (uint8_t)(src >> 24);
}

/*******************************************************************
** ������:     bytetoint(С��ģʽ)
** ��������:   ��src�������е��������ݰ��͵�ַ��Ӧ���ֽڵĹ���ת����int���͵�����
** ����:       [in]  src:             ������ָ��
** ����:       int���͵�����
********************************************************************/
uint32_t bytetoint(uint8_t *src)
{
    uint32_t dest;
    
    dest  = (uint32_t)(src[0]);
    dest |= (uint32_t)(src[1]) << 8;
    dest |= (uint32_t)(src[2]) << 16;
    dest |= (uint32_t)(src[3]) << 24;
    return dest;
}

/*******************************************************************
** ������:     shorttobyte(С��ģʽ)
** ��������:   ��short���͵�����src�����ֽڵ����ֽڵ�˳������dest��ָ�Ļ�������
** ����:       [out] dest:            ������ָ��
**             [in]  src:             short��������
** ����:       ��
********************************************************************/
void shorttobyte(uint8_t *dest, uint16_t src)
{
    dest[0] = (uint8_t)(src);
    dest[1] = (uint8_t)(src >> 8);
}

/*******************************************************************
** ������:     bytetoshort(С��ģʽ)
** ��������:   ��src�������е��������ݰ��͵�ַ��Ӧ���ֽڵĹ���ת����short���͵�����
** ����:       [in]  src:             ������ָ��
** ����:       short���͵�����
********************************************************************/
uint16_t bytetoshort(uint8_t *src)
{
    uint16_t dest;
    
    dest  = (uint16_t)(src[0]);
    dest |= (uint16_t)(src[1]) << 8;
    return dest;
}

/*******************************************************************
** ������:     lowerchar
** ��������:   ���ַ�chת����Сд�ַ�
** ����:       [in]  ch:    ��ת���ַ�
** ����:       ת����Сд���ַ�
********************************************************************/
static uint8_t lowerchar(uint8_t ch){
    if (ch >= 'A' && ch <= 'Z'){
        ch = (uint8_t)('a' + (ch - 'A'));
    }
    return ch;
}

/*******************************************************************
** ������:     gps_hextochar
** ��������:   ��hex����ת�����ַ�, ����: ��0x0aת�����ַ�'A'
** ����:       [in]  sbyte:           ��ת����hex����
** ����:       ת������ַ�
** ע��:       sbyte��ȡֵ��ΧΪ: 0x00~0x0F
********************************************************************/
char hextochar(uint8_t sbyte){
    sbyte &= 0x0F;
    if (sbyte < 0x0A) {
        return (char)(sbyte + '0');
    } else {
        return (char)(sbyte - 0x0A + 'A');
    }
}

/*******************************************************************
** ������:     chartohex
** ��������:   ���ַ�ת����hex����, ����: ���ַ�'a'ת����0x0a
** ����:       [in]  schar:           ��ת���ַ�
** ����:       ת�����hex����
** ע��:       schar��ȡֵ��ΧΪ: '0'~'9'��'a'~'f'��'A'~'F'
********************************************************************/
uint8_t chartohex(uint8_t schar){
    if (schar >= '0' && schar <= '9') {
        return (uint8_t)(schar - '0');
    } else if (schar >= 'A' && schar <= 'F') {
        return (uint8_t)(schar - 'A' + 0x0A);
    } else if (schar >= 'a' && schar <= 'f') {
        return (uint8_t)(schar - 'a' + 0x0A);
    } else {
        return 0;
    }
}

/*******************************************************************
** ������:     gps_decstrtodigit
** ��������:   ��ʮ���Ʊ�ʾ���ַ���ת��������
** ����:       [in]  sptr:      �ַ���������
**             [in]  len:       �ַ�������
** ����:       ת�����hex����
** ����:       ��"78997899", ת����78997899(��0x4B5698B)
********************************************************************/
int decstrtodigit(uint8_t *sptr, uint8_t len){
    int result;
    
    result = 0;
    for (; len > 0; len--) {
        result = result * 10 + chartohex(*sptr++);
    }
    return result;
}

/*******************************************************************
** ������:     decstrtoint
** ��������:   ��ʮ���Ʊ�ʾ���ַ���ת��������
** ����:       [in]  sptr:      �ַ���������
**             [in]  len:       �ַ�������
** ����:       ת�����hex����
** ����:       ��"78997899", ת����78997899(��0x4B5698B)
********************************************************************/
int decstrtoint(uint8_t *sptr, uint8_t len){
    int result;
    
    result = 0;
    for (; len > 0; len--) {
        result = result * 10 + chartohex(*sptr++);
    }
    return result;
}

/*******************************************************************
** ������:     inttodecstr
** ��������:   ���ĸ��ֽڵ�hex����ת����ʮ���Ʊ�ʾ���ַ���
** ����:       [out] dptr:      ת������ַ���������
**             [in]  hex_data:  ��ת����hex����
**             [in]  reflen:    ת������ο�; 
**                              ��reflen==0, ��ת���������reflen����
**                              ��reflen!=0, ��ת��������ַ�������С��reflenʱ, ���ַ���ǰ��(reflen-ʵ�ʳ���)��0
** ����:       ת�����ַ�������(����ǰ��0)
** ����:       ��0x1EDB(��7899), ת����"7899"
********************************************************************/
int inttodecstr(uint8_t *dptr, uint32_t hex_data, uint8_t reflen){
    uint8_t i, len, temp;
    uint8_t *tempptr;
    
    len     = 0;
    tempptr = dptr;
    for (;;) {
        *dptr++ = hextochar((uint8_t)(hex_data % 10));
        len++;
        if (hex_data < 10) break;
        else hex_data /= 10;
    }
    if (len != 0) {
        dptr = tempptr;
        for (i = 0; i < (len/2); i++) {
            temp = *(dptr + i);
            *(dptr + i) = *(dptr + (len - 1 - i));
            *(dptr + (len - 1 - i)) = temp;
        }
        if (reflen > len) {
            dptr = tempptr + (len - 1);
            for (i = 1; i <= reflen; i++) {
                if (i <= len) *(dptr + (reflen - len)) = *dptr;
                else *(dptr + (reflen - len)) = '0';
                dptr--;
            }
            len = reflen;
        }
    }
    return len;
}

/*******************************************************************
** ������:     decstrtohex
** ��������:   ��ʮ���Ʊ�ʾ���ַ���ת�����ĸ��ֽڵ�hex����
** ����:       [in]  sptr:      �ַ���������
**             [in]  len:       �ַ�������
** ����:       ת�����hex����
** ����:       ��"7899", ת����0x1EDB(��7899)
********************************************************************/
uint32_t decstrtohex(uint8_t *sptr, uint8_t len){
    uint32_t result;
    
    result = 0;
    for (; len > 0; len--) {
        result = (uint32_t)(result * 10 + chartohex(*sptr++));
    }
    return result;
}

/*******************************************************************
** ������:     hextodecstr
** ��������:   �������ֽڵ�hex����ת����ʮ���Ʊ�ʾ���ַ���
** ����:       [out] dptr:      ת������ַ���������
**             [in]  hex_data:  ��ת����hex����
**             [in]  reflen:    ת������ο�; 
**                              ��reflen==0, ��ת���������reflen����
**                              ��reflen!=0, ��ת��������ַ�������С��reflenʱ, ���ַ���ǰ��(reflen-ʵ�ʳ���)��0
** ����:       ת�����ַ�������(����ǰ��0)
** ����:       ��0x1EDB(��7899), ת����"7899"
********************************************************************/
uint8_t hextodecstr(uint8_t *dptr, int hex_data, uint8_t reflen)
{
    uint8_t i, len, temp;
    uint8_t *tempptr;
    
    len     = 0;
    tempptr = dptr;
    for (;;) {
        *dptr++ = hextochar((uint8_t)(hex_data % 10));
        len++;
        if (hex_data < 10) break;
        else hex_data /= 10;
    }
    if (len != 0) {
        dptr = tempptr;
        for (i = 0; i < (len/2); i++) {
            temp = *(dptr + i);
            *(dptr + i) = *(dptr + (len - 1 - i));
            *(dptr + (len - 1 - i)) = temp;
        }
        if (reflen > len) {
            dptr = tempptr + (len - 1);
            for (i = 1; i <= reflen; i++) {
                if (i <= len) *(dptr + (reflen - len)) = *dptr;
                else *(dptr + (reflen - len)) = '0';
                dptr--;
            }
            len = reflen;
        }
    }
    return len;
}

/*******************************************************************
** ������:     iptostr
** ��������:   ��4���ֽڵ�ip��ַת�����ַ���
** ����:       [out] str:        �ַ�����ַ
**             [in]  ip:         ��ת����ip��ַ
** ����:       ��
** ע��:       ת������ַ�����'\0'����
********************************************************************/
void iptostr(char *str, uint32_t ip)
{
    str   += hextodecstr((uint8_t *)str, (ip >> 24) & 0xff, 0);
    *str++ = '.';
    str   += hextodecstr((uint8_t *)str, (ip >> 16) & 0xff, 0);
    *str++ = '.';
    str   += hextodecstr((uint8_t *)str, (ip >> 8) & 0xff, 0);
    *str++ = '.';
    str   += hextodecstr((uint8_t *)str, ip & 0xff, 0);
    *str   = '\0';
}

/*******************************************************************
** ������:     strtoip
** ��������:   ���ַ���ת����4���ֽڵ�ip��ַ
** ����:       [out] ip:         ת�����ip��ַ
**             [in]  str:        �ַ�����ַ
** ����:       �ɹ���ʧ��
********************************************************************/
bool_t strtoip(uint32_t *ip, char *str)
{
    uint8_t  ch, i, pos, seg[3], tmp[4];
    int res;
    
    for (i = 0; i <= 3; i++) {
        pos = 0;
        for (;;) {
            ch = *str++;
            if (!(ch >= '0' && ch <= '9') && ch != '.' && ch != '\0') {
                return false;
            }
            if (ch == '\0' || ch == '.') {
                if (pos == 0) return false;
                if ((res = decstrtohex(seg, pos)) > 255) return false;
                tmp[i] = (uint8_t)res;
                if (ch == '\0') {
                    if (i == 3) {
                        *ip = ((uint32_t)tmp[0] << 24) + ((uint32_t)tmp[1] << 16) + ((uint32_t)tmp[2] << 8) + tmp[3];
                        return true;
                    } else {
                        return false;
                    }
                }
                break;
            } else {
                if (pos >= sizeof(seg)) return false;
                seg[pos++] = ch;
            }
        }
    }
    return false;
}

/*******************************************************************
** ������:     getchksum
** ��������:   �����ۼӺ�
** ����:       [in]  dptr:     �������ۼӺ����ݻ�����
**             [in]  len:      ���ݻ���������
** ����:       һ���ֽڵ��ۼӺ�
********************************************************************/
uint8_t getchksum(uint8_t *dptr, int len)
{
    int result;

    result = 0;
    for (; len > 0; len--) {
        result = (int)(result + *dptr++);
    }
    return (uint8_t)result;
}

/*******************************************************************
** ������:     getchksum_n
** ��������:   ����ʹ�ۼӺ�Ϊ0��У����
** ����:       [in]  dptr:     �������ۼӺ����ݻ�����
**             [in]  len:      ���ݻ���������
** ����:       У����
********************************************************************/
uint8_t getchksum_n(uint8_t *dptr, int len)
{
    return (uint8_t)(0 - getchksum(dptr, len));
}

/*******************************************************************
** ������:     addrtobyte
** ��������:   �豸��ת����5�ֽڸ�ʽ
** ����:       [in]  sptr: �����ַ��Ϣ�ַ���,��"1122-3344-1"   
**             [out] dptr: 1��3344%100��3344/100��1122%100��1122/100
** ����:       �ɹ�true, ʧ��false
********************************************************************/
bool_t addrtobyte(uint8_t *sptr, uint8_t *dptr)
{
    uint8_t len;

    len = LEE_STRLEN(sptr);
    if (len != 11 && len != 9){
        return false;
    }

    if (len == 9){
        dptr[0] = 0xff;
    } else {
        dptr[0] = decstrtoint(&sptr[10], 1);
    }
    dptr[1] = decstrtoint(&sptr[5], 4)%100;
    dptr[2] = decstrtoint(&sptr[5], 4)/100;
    dptr[3] = decstrtoint(&sptr[0], 4)%100;
    dptr[4] = decstrtoint(&sptr[0], 4)/100;
    return true;
}

/*******************************************************************
** ������:     addrtobyte
** ��������:   5�ֽڸ�ʽת�����豸���ַ���
** ����:       [in]  sptr: �����ַ��Ϣ�ַ���1   
**             [out] dptr: �����ַ��Ϣ�ַ���,��"1122-3344-1"   
** ����:       �ɹ�true, ʧ��false
********************************************************************/
bool_t bytetoaddr(uint8_t *sptr, uint8_t *dptr)
{
    int data;

    data = sptr[4]*100 + sptr[3];
    inttodecstr(&dptr[0], data, 4);
    dptr[4] = '-';
    data = sptr[2]*100 + sptr[1];
    inttodecstr(&dptr[5], data, 4);
    dptr[9] = '-';
    data = sptr[0];
    inttodecstr(&dptr[10], data, 1);

    return true;
}


