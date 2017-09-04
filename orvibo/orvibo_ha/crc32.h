/**
 * Copyrights (C) 2015 - 2020, Orvibo
 * All rights reserved.
 *
 * File : crc32.h
 *
 * 生成 CRC32 校验码
 *
 * Change Logs:
 * Date         Author          Note
 * 2015-08-19   Sid Lee         Create this file
 */
#ifndef __CRC32_H__
#define __CRC32_H__

#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

int crc_check(const uint8_t *buf, int size );

#ifdef  __cplusplus
}
#endif

#endif  //__CRC32_H__
