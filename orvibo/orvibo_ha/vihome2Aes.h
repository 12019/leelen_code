/**
 * Copyrights (C) 2015 - 2020, Orvibo
 * All rights reserved
 *
 * File : vihome2Aes.h
 *
 * ViHome加密解决函数
 *
 * Change Logs:
 * Date         Author      Notes
 * 2015-09-10   Sid Lee     创建
 */

#ifndef __VIHOME2_ENCRYPT_H__
#define __VIHOME2_ENCRYPT_H__

#ifdef  __cplusplus
extern "C" {
#endif

/*=================================================================
 *  Functions
 *=================================================================*/

int ViHome2AesEncryptTo(short type, const char *buf, int len, const char *getkey, char *out);
int ViHome2AesDecryptTo(short type, const char *buf, int len, const char *getkey, char *out);

int ViHome2AesEncrypt(short type, char *buf, int len, const char *getkey);
int ViHome2AesDecrypt(short type, char *buf, int len, const char *getkey);

#ifdef  __cplusplus
}
#endif

#endif /* !__VIHOME2_ENCRYPT_H__ */
