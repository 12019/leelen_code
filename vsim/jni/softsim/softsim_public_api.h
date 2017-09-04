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
 * Filename: softsim_public_api.h
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-07-08
 ******************************************************************************/

 #ifndef __SOFTSIM_PUBLIC_API_H__
 #define __SOFTSIM_PUBLIC_API_H__

#ifdef __cplusplus
 extern "C" { 
#endif

/*****************************************************************************
* Include																		
*****************************************************************************/
#include "softsim_type.h"

/*****************************************************************************
* Define																		
*****************************************************************************/
#define ENCODING_LENGTH	(2)

#define ascii_to_bin(c) ((c)>='a'?(c-59):(c)>='A'?((c)-53):(c)-'.')
#define bin_to_ascii(c) ((c)>=38?((c)-38+'a'):(c)>=12?((c)-12+'A'):(c)+'.')	  

#define is_valid(x, start, end) ((x) >= (start) && (x) <= (end))
#define is_num(x)       is_valid((x), '0', '9')
#define is_hex_upper(x) is_valid((x), 'A', 'F')
#define is_hex_lower(x) is_valid((x), 'a', 'f')

#define ascii_to_hex(c)	(is_num(c)?(c-'0'):(is_hex_upper(c)?(c-55):(c-87)))

/* Macro to check whether a character is an ASCII alpha.*/
#define is_alpha(c) ( ((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') )
#define is_lower(c) ('a' <= (c) && (c) <= 'z')
#define is_upper(c) ('A' <= (c) && (c) <= 'Z')
#define to_upper(c) (is_lower(c) ? 'A' + ((c) - 'a') : (c))
#define to_lower(c) (is_upper(c) ? 'a' + ((c) - 'A') : (c))
#define xor(e,f) (((e) && !(f)) || (!(e) && (f)))
#define is_ascii(c) (((c) & ~0x7f) == 0)	/*If C is a 7 bit value.*/
#define is_digit(x) (((x) >= '0') && ((x) <= '9'))
#define is_xdigit(ch) (is_digit(ch) || ((ch)>= 'A' && (ch) <= 'F' )||((ch)>='a' && (ch)<='f'))
#define is_space(c) (((c)==' ') || ((c)=='\n') || ((c)=='\t') || ((c)=='\r') || ((c)=='\a'))

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define min(a, b)		((a)<(b)?(a):(b))
#define max(a,b)		((a)>(b)?(a):(b))
#define square(a)		((a)*(a))
#define roundup(x, y)	((((x) + ((y) - 1)) / (y)) * (y))

#define byte_8_64(d, a0, a1, a2, a3, a4, a5, a6, a7) 	{u64 t; t+=((((u64)(a0))<<56)&0xff00000000000000); t+=((((u64)(a1))<<48)&0xff000000000000); t+=((((u64)(a2))<<40)&0xff0000000000); t+=((((u64)(a3))<<32)&0xff00000000);\
t+=((((u64)(a4))<<24)&0xff000000); t+=((((u64)(a5))<<16)&0xff0000); t+=((((u64)(a6))<<8)&0xff00); t+=(((u64)(a7))&0xff); (d)=t;}
#define byte_64_8(a0, a1, a2, a3, a4, a5, a6, a7, d) 	{u64 t; t=d; (a0)=(u8)((t)>>56); (a1)=(u8)((t)>>48); (a2)=(u8)((t)>>40); (a3)=(u8)((t)>>32);(a4)=(u8)((t)>>24); (a5)=(u8)((t)>>16); (a6)=(u8)((t)>>8); (a7)=(u8)(t);}

#define byte_8_32(d, a0, a1, a2, a3) 	{u32 t; t=((((u32)(a0))<<24)&0xff000000); t+=((((u32)(a1))<<16)&0xff0000); t+=((((u32)(a2))<<8)&0xff00); t+=(((u32)(a3))&0xff); (d)=t;}
#define byte_32_8(a0, a1, a2, a3, d) 	{u32 t; t=d; (a0)=(u8)((t)>>24); (a1)=(u8)((t)>>16); (a2)=(u8)((t)>>8); (a3)=(u8)(t);}
#define byte_8_16(d, a0, a1) 			{u16 t; t=((((u16)(a0))<<8)&0xff00); t+=(((u16)(a1))&0xff); (d)=t;}
#define byte_16_8(a0, a1, d) 				{u16 t; t=d; (a0)=(u8)((t)>>8); (a1)=(u8)(t);}
/*****************************************************************************
* Typedef																		
*****************************************************************************/
 
/*****************************************************************************
* Static Local Variable														   
*****************************************************************************/
	  
/*****************************************************************************
* Global Variable														  
*****************************************************************************/

/*****************************************************************************
* Extern Global Variable															 
*****************************************************************************/

/*****************************************************************************
* Static Local Function														   
*****************************************************************************/
	  
/*****************************************************************************
* Global Function														 
*****************************************************************************/

/*****************************************************************************
* Extern Global Function													   
*****************************************************************************/
 
/****************************************************************************
* Function
****************************************************************************/
#ifdef __cplusplus
	}
#endif
#endif /*__SOFTSIM_PUBLIC_API_H__*/
