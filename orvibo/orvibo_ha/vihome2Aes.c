/**
 * Copyrights (C) 2015 - 2020, Orvibo
 * All rights reserved
 *
 * File : vihome2Aes.c
 *
 * ViHome加密解密函数实现
 *
 * Change Logs:
 * Date         Author      Notes
 * 2015-09-10   Sid Lee     整理
 */

#include <string.h>
#include "aes.h"

#define PROTOCOL_KEY    "khggd54865SNJHGF"

/**
 * 将buff中的明文加密到out地址缓存区
 */
int ViHome2AesEncryptTo(short type, const char *buf, int len, const char *dkey, char *out)
{
    const char *key = dkey;

//    if (type==PROTOCOL_TYPE_PK)
//        key = PROTOCOL_KEY;

    return AesEncrypt(buf, len, key, out);
}

/**
 * 将buff中的密文解密到out地址缓冲区
 */
int ViHome2AesDecryptTo(short type, const char *buf, int len, const char *dkey, char *out)
{
    const char *key = dkey;

//    if (type==PROTOCOL_TYPE_PK)
//        key = PROTOCOL_KEY;

    return AesDecrypt(buf, len, key, out);
}

/**
 * 将明文件加密到buf本身
 */
int ViHome2AesEncrypt(short type, char *buf, int len, const char *dkey)
{
//    char out[PAYLOAD_SIZE];

//    int outlen = ViHome2AesEncryptTo(type, buf, len, dkey, out);

//    memset(buf, 0, len);
//    memcpy(buf, out, outlen);
//    return outlen;
}

/**
 * 将密文件解密到buf本身
 */
int ViHome2AesDecrypt(short type, char *buf, int len, const char *dkey)
{
//    char out[PAYLOAD_SIZE] = {0};

//    int outlen = ViHome2AesDecryptTo(type, buf, len, dkey, out);

//    memset(buf,0,len);
//    memcpy(buf,out,outlen);
//    return outlen;
}
