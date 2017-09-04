/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of TGT Limited. (C) 2016
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("TGT SOFTWARE")
*  RECEIVED FROM TGT AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. TGT EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES TGT PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE TGT SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. TGT SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY TGT SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND TGT'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE TGT SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT TGT'S OPTION, TO REVISE OR REPLACE THE TGT SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  TGT FOR SUCH TGT SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename: softsim_release.h
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-07-08
 ******************************************************************************/
#ifndef __SOFTSIM_TYPE_H__
#define __SOFTSIM_TYPE_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "softsim_type.h"


extern void *softsim_memcpy(void* dst, const void* src, u32 len);
extern void *softsim_memmove(void *dst, const void *src, u32 len);
extern void *softsim_memset(void *mem, s32 data, u32 len);
extern void *softsim_memchr(const void *s, s32 c, u32 n);
extern s32 softsim_memcmp(const void *src1, const void  *src2, u32 size);
extern u32 softsim_strlen(const char *str);
extern char *softsim_strcat(char *dst, const char *src);
extern s32  softsim_strcmp(const char *str1, const char *str2);
extern s32  softsim_strncmp(const char *str1, const char *str2, u32 len);
extern s32 softsim_strcasecmp(const char *s1, const char *s2);
extern s32 softsim_strncasecmp(const char *string1, const char *string2, s32 n);
extern char *softsim_strchr(const char *str, s32 ch);
extern char *softsim_strrchr(const char *str, s32 ch);
extern char *softsim_strstr(const char *buf, const char *sub);
extern char *softsim_strcpy(char *dest, const char *src);
extern char *softsim_strncpy(char *dest, const char *src, u32 count);
extern u32 softsim_strcspn(const char *s, const char *reject);
extern char *softsim_strpbrk(const char *cs, const char *ct);
extern char *softsim_strtok(char *s, const char *delim);
extern s32 softsim_sprintf(char * buf, const char * fmt, ...);

extern void softsim_a3a8(u8 key[16], u8 rand[16], u8 output[12]);
extern void softsim_f1(u8 opc[16], u8 key[16], u8 rand[16], u8 sqn[6], u8 amf[2], u8 xmac[8]);
extern void softsim_f2345(u8 opc[16], u8 k[16], u8 rand[16], u8 res[8], u8 ck[16], u8 ik[16], u8 ak[6]);  
extern void softsim_f1star(u8 opc[16], u8 k[16], u8 rand[16], u8 sqn[6], u8 amf[2], u8 macs[8]);
extern void softsim_f5star(u8 opc[16], u8 k[16], u8 rand[16], u8 ak[6]);

#ifdef __cplusplus
	}
#endif

#endif/*__SOFTSIM_TYPE_H__*/

