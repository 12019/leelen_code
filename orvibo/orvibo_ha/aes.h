/**
 * Copyrights (C) 2015 - 2020, Orvibo
 * All rights reserved
 *
 * File : aes.h
 *
 * Aes加密封装函数
 *
 * Change Logs:
 * Date         Author      Notes
 * 2015-09-10   Sid Lee     创建
 */
#ifndef __AES_WRAP_H__
#define __AES_WRAP_H__

#ifdef __cplusplus
extern "C" {
#endif

/*=================================================================
 *  Functions
 *=================================================================*/

void AesInit(void);
void AesDestroy(void);

/** 加密 */
int AesEncrypt(const unsigned char *in, int len, const char *dkey, unsigned char *out);
/** 解密 */
int AesDecrypt(const unsigned char *in, int len, const char *dkey, unsigned char *out);

#ifdef __cplusplus
}
#endif

#endif
