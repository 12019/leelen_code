/********************************************************************************
**
** 文件名:     leelen_utils.c
** 版权所有:   (c) 2015 厦门立林科技有限公司
** 文件描述:   实现工具类函数
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
#include "leelen_utils.h"

static char   s_debugmem[1024];

/*******************************************************************
** 函数名:     loghex
** 函数描述:   将缓冲区中的数据按hex格式输出
** 参数:       [in]  str:             在待打印的缓冲区数据前附加打印的字符串
**             [in]  ptr:             待打印的数据缓冲区
**             [in]  len:             待打印的数据缓冲区长度
** 返回:       无
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
** 函数名:     inttobyte(小端模式)
** 函数描述:   将int类型的数据src按高字节到低字节的顺序存放在dest所指的缓冲区中
** 参数:       [out] dest:            缓冲区指针
**             [in]  src:             int类型数据
** 返回:       无
********************************************************************/
void inttobyte(uint8_t *dest, uint32_t src)
{
    dest[0] = (uint8_t)(src);
    dest[1] = (uint8_t)(src >> 8);
    dest[2] = (uint8_t)(src >> 16);
    dest[3] = (uint8_t)(src >> 24);
}

/*******************************************************************
** 函数名:     bytetoint(小端模式)
** 函数描述:   将src缓冲区中的数据内容按低地址对应高字节的规则转换成int类型的数据
** 参数:       [in]  src:             缓冲区指针
** 返回:       int类型的数据
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
** 函数名:     shorttobyte(小端模式)
** 函数描述:   将short类型的数据src按高字节到低字节的顺序存放在dest所指的缓冲区中
** 参数:       [out] dest:            缓冲区指针
**             [in]  src:             short类型数据
** 返回:       无
********************************************************************/
void shorttobyte(uint8_t *dest, uint16_t src)
{
    dest[0] = (uint8_t)(src);
    dest[1] = (uint8_t)(src >> 8);
}

/*******************************************************************
** 函数名:     bytetoshort(小端模式)
** 函数描述:   将src缓冲区中的数据内容按低地址对应高字节的规则转换成short类型的数据
** 参数:       [in]  src:             缓冲区指针
** 返回:       short类型的数据
********************************************************************/
uint16_t bytetoshort(uint8_t *src)
{
    uint16_t dest;
    
    dest  = (uint16_t)(src[0]);
    dest |= (uint16_t)(src[1]) << 8;
    return dest;
}

/*******************************************************************
** 函数名:     lowerchar
** 函数描述:   将字符ch转换成小写字符
** 参数:       [in]  ch:    待转换字符
** 返回:       转换成小写的字符
********************************************************************/
static uint8_t lowerchar(uint8_t ch){
    if (ch >= 'A' && ch <= 'Z'){
        ch = (uint8_t)('a' + (ch - 'A'));
    }
    return ch;
}

/*******************************************************************
** 函数名:     gps_hextochar
** 函数描述:   将hex数据转换成字符, 例如: 将0x0a转换成字符'A'
** 参数:       [in]  sbyte:           待转换的hex数据
** 返回:       转换后的字符
** 注意:       sbyte的取值范围为: 0x00~0x0F
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
** 函数名:     chartohex
** 函数描述:   将字符转换成hex数据, 例如: 将字符'a'转换成0x0a
** 参数:       [in]  schar:           待转换字符
** 返回:       转换后的hex数据
** 注意:       schar的取值范围为: '0'~'9'、'a'~'f'、'A'~'F'
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
** 函数名:     gps_decstrtodigit
** 函数描述:   将十进制表示的字符串转换成数据
** 参数:       [in]  sptr:      字符串缓冲区
**             [in]  len:       字符串长度
** 返回:       转换后的hex数据
** 例子:       将"78997899", 转换成78997899(即0x4B5698B)
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
** 函数名:     decstrtoint
** 函数描述:   将十进制表示的字符串转换成数据
** 参数:       [in]  sptr:      字符串缓冲区
**             [in]  len:       字符串长度
** 返回:       转换后的hex数据
** 例子:       将"78997899", 转换成78997899(即0x4B5698B)
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
** 函数名:     inttodecstr
** 函数描述:   将四个字节的hex数据转换成十进制表示的字符串
** 参数:       [out] dptr:      转换后的字符串缓冲区
**             [in]  hex_data:  待转换的hex数据
**             [in]  reflen:    转换结果参考; 
**                              如reflen==0, 则转换结果不受reflen控制
**                              如reflen!=0, 则当转换结果的字符串长度小于reflen时, 在字符串前补(reflen-实际长度)个0
** 返回:       转换的字符串长度(包括前置0)
** 例子:       将0x1EDB(即7899), 转换成"7899"
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
** 函数名:     decstrtohex
** 函数描述:   将十进制表示的字符串转换成四个字节的hex数据
** 参数:       [in]  sptr:      字符串缓冲区
**             [in]  len:       字符串长度
** 返回:       转换后的hex数据
** 例子:       将"7899", 转换成0x1EDB(即7899)
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
** 函数名:     hextodecstr
** 函数描述:   将两个字节的hex数据转换成十进制表示的字符串
** 参数:       [out] dptr:      转换后的字符串缓冲区
**             [in]  hex_data:  待转换的hex数据
**             [in]  reflen:    转换结果参考; 
**                              如reflen==0, 则转换结果不受reflen控制
**                              如reflen!=0, 则当转换结果的字符串长度小于reflen时, 在字符串前补(reflen-实际长度)个0
** 返回:       转换的字符串长度(包括前置0)
** 例子:       将0x1EDB(即7899), 转换成"7899"
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
** 函数名:     iptostr
** 函数描述:   将4个字节的ip地址转换成字符串
** 参数:       [out] str:        字符串地址
**             [in]  ip:         待转换的ip地址
** 返回:       无
** 注意:       转换后的字符串以'\0'结束
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
** 函数名:     strtoip
** 函数描述:   将字符串转换成4个字节的ip地址
** 参数:       [out] ip:         转换后的ip地址
**             [in]  str:        字符串地址
** 返回:       成功或失败
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
** 函数名:     getchksum
** 函数描述:   计算累加和
** 参数:       [in]  dptr:     待计算累加和数据缓冲区
**             [in]  len:      数据缓冲区长度
** 返回:       一个字节的累加和
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
** 函数名:     getchksum_n
** 函数描述:   计算使累加和为0的校验码
** 参数:       [in]  dptr:     待计算累加和数据缓冲区
**             [in]  len:      数据缓冲区长度
** 返回:       校验码
********************************************************************/
uint8_t getchksum_n(uint8_t *dptr, int len)
{
    return (uint8_t)(0 - getchksum(dptr, len));
}

/*******************************************************************
** 函数名:     addrtobyte
** 函数描述:   设备号转换成5字节格式
** 参数:       [in]  sptr: 输入地址信息字符串,如"1122-3344-1"   
**             [out] dptr: 1、3344%100、3344/100、1122%100、1122/100
** 返回:       成功true, 失败false
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
** 函数名:     addrtobyte
** 函数描述:   5字节格式转换成设备号字符串
** 参数:       [in]  sptr: 输入地址信息字符串1   
**             [out] dptr: 输出地址信息字符串,如"1122-3344-1"   
** 返回:       成功true, 失败false
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


