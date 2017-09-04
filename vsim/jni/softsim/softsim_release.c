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
 * Filename: softsim_release.c
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-07-08
 ******************************************************************************/

/*****************************************************************************
* Include																	   
*****************************************************************************/
//#include <stdio.h>
#include <stdarg.h>
#include <string.h>
//#include <time.h>
//#include <stdlib.h>
//#include <ctype.h>
//#include <math.h>

#include "softsim_type.h"
/*****************************************************************************
* Define																	   
*****************************************************************************/
	 
/*****************************************************************************
* Typedef																	   
*****************************************************************************/
	 
/*****************************************************************************
* Static Local Variable															  
*****************************************************************************/
static u8 charmap[] = {
	(u8)'\000',(u8)'\001',(u8)'\002',(u8)'\003',(u8)'\004',(u8)'\005',(u8)'\006',(u8)'\007',
	(u8)'\010',(u8)'\011',(u8)'\012',(u8)'\013',(u8)'\014',(u8)'\015',(u8)'\016',(u8)'\017',
	(u8)'\020',(u8)'\021',(u8)'\022',(u8)'\023',(u8)'\024',(u8)'\025',(u8)'\026',(u8)'\027',
	(u8)'\030',(u8)'\031',(u8)'\032',(u8)'\033',(u8)'\034',(u8)'\035',(u8)'\036',(u8)'\037',
	(u8)'\040',(u8)'\041',(u8)'\042',(u8)'\043',(u8)'\044',(u8)'\045',(u8)'\046',(u8)'\047',
	(u8)'\050',(u8)'\051',(u8)'\052',(u8)'\053',(u8)'\054',(u8)'\055',(u8)'\056',(u8)'\057',
	(u8)'\060',(u8)'\061',(u8)'\062',(u8)'\063',(u8)'\064',(u8)'\065',(u8)'\066',(u8)'\067',
	(u8)'\070',(u8)'\071',(u8)'\072',(u8)'\073',(u8)'\074',(u8)'\075',(u8)'\076',(u8)'\077',
	(u8)'\100',(u8)'\141',(u8)'\142',(u8)'\143',(u8)'\144',(u8)'\145',(u8)'\146',(u8)'\147',
	(u8)'\150',(u8)'\151',(u8)'\152',(u8)'\153',(u8)'\154',(u8)'\155',(u8)'\156',(u8)'\157',
	(u8)'\160',(u8)'\161',(u8)'\162',(u8)'\163',(u8)'\164',(u8)'\165',(u8)'\166',(u8)'\167',
	(u8)'\170',(u8)'\171',(u8)'\172',(u8)'\133',(u8)'\134',(u8)'\135',(u8)'\136',(u8)'\137',
	(u8)'\140',(u8)'\141',(u8)'\142',(u8)'\143',(u8)'\144',(u8)'\145',(u8)'\146',(u8)'\147',
	(u8)'\150',(u8)'\151',(u8)'\152',(u8)'\153',(u8)'\154',(u8)'\155',(u8)'\156',(u8)'\157',
	(u8)'\160',(u8)'\161',(u8)'\162',(u8)'\163',(u8)'\164',(u8)'\165',(u8)'\166',(u8)'\167',
	(u8)'\170',(u8)'\171',(u8)'\172',(u8)'\173',(u8)'\174',(u8)'\175',(u8)'\176',(u8)'\177',
	(u8)'\200',(u8)'\201',(u8)'\202',(u8)'\203',(u8)'\204',(u8)'\205',(u8)'\206',(u8)'\207',
	(u8)'\210',(u8)'\211',(u8)'\212',(u8)'\213',(u8)'\214',(u8)'\215',(u8)'\216',(u8)'\217',
	(u8)'\220',(u8)'\221',(u8)'\222',(u8)'\223',(u8)'\224',(u8)'\225',(u8)'\226',(u8)'\227',
	(u8)'\230',(u8)'\231',(u8)'\232',(u8)'\233',(u8)'\234',(u8)'\235',(u8)'\236',(u8)'\237',
	(u8)'\240',(u8)'\241',(u8)'\242',(u8)'\243',(u8)'\244',(u8)'\245',(u8)'\246',(u8)'\247',
	(u8)'\250',(u8)'\251',(u8)'\252',(u8)'\253',(u8)'\254',(u8)'\255',(u8)'\256',(u8)'\257',
	(u8)'\260',(u8)'\261',(u8)'\262',(u8)'\263',(u8)'\264',(u8)'\265',(u8)'\266',(u8)'\267',
	(u8)'\270',(u8)'\271',(u8)'\272',(u8)'\273',(u8)'\274',(u8)'\275',(u8)'\276',(u8)'\277',
	(u8)'\300',(u8)'\341',(u8)'\342',(u8)'\343',(u8)'\344',(u8)'\345',(u8)'\346',(u8)'\347',
	(u8)'\350',(u8)'\351',(u8)'\352',(u8)'\353',(u8)'\354',(u8)'\355',(u8)'\356',(u8)'\357',
	(u8)'\360',(u8)'\361',(u8)'\362',(u8)'\363',(u8)'\364',(u8)'\365',(u8)'\366',(u8)'\367',
	(u8)'\370',(u8)'\371',(u8)'\372',(u8)'\333',(u8)'\334',(u8)'\335',(u8)'\336',(u8)'\337',
	(u8)'\340',(u8)'\341',(u8)'\342',(u8)'\343',(u8)'\344',(u8)'\345',(u8)'\346',(u8)'\347',
	(u8)'\350',(u8)'\351',(u8)'\352',(u8)'\353',(u8)'\354',(u8)'\355',(u8)'\356',(u8)'\357',
	(u8)'\360',(u8)'\361',(u8)'\362',(u8)'\363',(u8)'\364',(u8)'\365',(u8)'\366',(u8)'\367',
	(u8)'\370',(u8)'\371',(u8)'\372',(u8)'\373',(u8)'\374',(u8)'\375',(u8)'\376',(u8)'\377',	
};

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

void * softsim_memcpy(void* dst, const void* src, u32 len)
{
	return memcpy(dst, src, len);
}

void *softsim_memmove(void *dst, const void *src, u32 len)
{
	return memmove(dst, src, len);	
}

void *softsim_memset(void *mem, s32 data, u32 len)
{
	return memset(mem, data, len);
}

void * softsim_memchr(const void *s, s32 c, u32 n)
{	
	return memchr(s, c, n);
}

s32 softsim_memcmp(const void *src1, const void  *src2, u32 size)
{
	return memcmp((void *)src1, (void *)src2, size);
}

u32 softsim_strlen(const char *str)
{
	if(NULL != str)
	{
		return strlen(str);
	}
	else
	{
		return 0;
	}
}

char *softsim_strcat(char *dst, const char *src)
{
	return strcat(dst, src);
}

s32  softsim_strcmp(const char *str1, const char *str2)
{
	return strcmp(str1 , str2);
}

s32  softsim_strncmp(const char *str1, const char *str2, u32 len)
{
	return strncmp(str1, str2, len);
}

s32 softsim_strcasecmp(const char *string1, const char *string2)
{
	register unsigned char u1, u2;
	
	for (;;) 
	{
		u1 = (unsigned char) *string1++;
		u2 = (unsigned char) *string2++;
		if (charmap[u1] != charmap[u2]) 
		{
			return charmap[u1] - charmap[u2];
		}
		
		if (u1 == '\0') 
		{
			return 0;
		}
	}
}

s32 softsim_strncasecmp(const char *string1, const char *string2, s32 n)
{
	register u8 *cm = charmap,
	*us1 = (u8 *)string1,
	*us2 = (u8 *)string2;
	
	while (--n >= 0 && cm[*us1] == cm[*us2++])
	if (*us1++ == ' ')
	return(0);
	return(n < 0 ? 0 : cm[*us1] - cm[*--us2]);
}

char * softsim_strchr(const char *str, s32 ch)
{
	return strchr(str, ch);
}

char * softsim_strrchr(const char *str, s32 ch)
{
	return strrchr(str, ch);
}

char * softsim_strstr(const char *buf, const char *sub)
{
	return strstr(buf, sub);
}

char * softsim_strcpy(char *dest, const char *src)
{
	return strcpy(dest, src);
}

char *softsim_strncpy(char *dest, const char *src, u32 count)
{
	return strncpy(dest, src, count); 
}

u32 softsim_strcspn(const char *s, const char *reject)
{
	return strcspn(s, reject);
}

char *softsim_strpbrk(const char *cs, const char *ct)
{
	return strpbrk(cs, ct);
}

char *softsim_strtok(char *s, const char *delim)
{
	return strtok(s, delim);
}

s32 softsim_sprintf(char * buf, const char * fmt, ...)
{
	s32 rvalue = 0;
	va_list l;
	
	va_start(l, fmt);
	rvalue = vsprintf((char*) buf, fmt, l);
	va_end(l);
	return (rvalue);
}

void softsim_a3a8(u8 key[16], u8 rand[16], u8 output[12])
{
	
}

void softsim_f1(u8 opc[16], u8 key[16], u8 rand[16], u8 sqn[6], u8 amf[2], u8 xmac[8])
{
	//f1(opc, key, rand, sqn, amf, xmac);
}

void softsim_f2345(u8 opc[16], u8 k[16], u8 rand[16], u8 res[8], u8 ck[16], u8 ik[16], u8 ak[6])  
{
	//f2345(opc, k, rand, res, ck, ik, ak);
}

void softsim_f1star(u8 opc[16], u8 k[16], u8 rand[16], u8 sqn[6], u8 amf[2], u8 macs[8])
{
	//f1star(opc, k, rand, sqn, amf, macs);
}

void softsim_f5star(u8 opc[16], u8 k[16], u8 rand[16], u8 ak[6])
{
	//f5star(opc, k, rand, ak);
}


