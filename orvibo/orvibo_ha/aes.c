/**
 * Copyrights (C) 2015 - 2020, Orvibo
 * All rights reserved
 *
 * File : aes_wrap.c
 *
 * Aes加密函数封装实现
 *
 * Change Logs:
 * Date         Author      Notes
 * 2015-09-10   Sid Lee     创建，并实现功能
 */

#include <openssl/aes.h>
#include <openssl/evp.h>

/*=================================================================
 *  Variables
 *=================================================================*/

static const EVP_CIPHER *_aes_cipher = NULL;
static EVP_CIPHER_CTX    _aes_ctx;

/*=================================================================
 *  Functions
 *=================================================================*/

void AesInit(void) {
    if (_aes_cipher == NULL) {
        EVP_CIPHER_CTX_init(&_aes_ctx);
        _aes_cipher  = EVP_aes_128_ecb();
    }
}

void AesDestroy(void) {
    if (_aes_cipher != NULL) {
        EVP_CIPHER_CTX_cleanup(&_aes_ctx);
        _aes_cipher = NULL;
    }
}

int AesEncrypt(const unsigned char *in, int len, const char *key, unsigned char *out) {
    int outlen = 0;
    unsigned char iv[8]= {0};
    int block = 0;

    AesInit();

    EVP_EncryptInit_ex(&_aes_ctx, _aes_cipher, NULL, (const unsigned char*)key, iv);
    EVP_EncryptUpdate(&_aes_ctx, out, &block, in, len);

    outlen = block;

    EVP_EncryptFinal_ex(&_aes_ctx, out+block, &block);
    outlen += block;
    return outlen;
}

/**
 * AES 解密函数
 *
 * @param   in      密文缓冲地址
 * @param   len     密文长度
 * @param   dkey    密钥
 * @param   out     明文缓冲地址
 * @return  解密之后的数据长度
 */
int AesDecrypt(const unsigned char *in, int len, const char *key, unsigned char *out) {
    int block = 0;
    unsigned char iv[8]= {0};
    int outlen = 0;
    AesInit();

    EVP_DecryptInit_ex(&_aes_ctx, _aes_cipher, NULL, (const unsigned char*)key, iv);
    EVP_DecryptUpdate(&_aes_ctx, out, &block, in, len);
    outlen += block;

    EVP_DecryptFinal_ex(&_aes_ctx, out+block, &outlen);
    outlen += block;
    return outlen;
}
