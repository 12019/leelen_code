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
 * Filename: softsim_version.h
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
#include <stdio.h>
#include "softsim_include.h"
/*****************************************************************************
* Define																	   
*****************************************************************************/
#undef 	SOFTSIM_MD_ID
#undef 	SOFTSIM_TRACE_HEADER
#define SOFTSIM_MD_ID					SOFTSIM_MD_VERSION
#define SOFTSIM_TRACE_HEADER			"[VERSION]" 

/*****************************************************************************
* Typedef																	   
*****************************************************************************/

/*****************************************************************************
* Static Local Variable															  
*****************************************************************************/
static u8 softsim_version[SOFTSIM_VERSION_STRING_LEN] = {0};
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

static char *softsim_get_main_version_info(void)
{
	return SOFTSIM_MAIN_VERSION;
}

static char *softsim_get_sub_version_info(void)
{
	return SOFTSIM_SUB_VERSION;
}

static char *softsim_get_version_info(void)
{
	softsim_memset(softsim_version, 0x00, sizeof(softsim_version));
	softsim_memcpy(softsim_version, softsim_get_main_version_info(), softsim_strlen(softsim_get_main_version_info()));
	softsim_strcat(softsim_version, softsim_get_sub_version_info());

	return softsim_version;
}

static void softsim_get_month(s8 *buf, s8 *month)
{
	if(softsim_strcmp(month, "Jan") == 0)
	{
		softsim_memcpy(buf, "01", 2);
	}
	else if(softsim_strcmp(month, "Feb") == 0)
	{
		softsim_memcpy(buf, "02", 2);
	}
	else if(softsim_strcmp(month, "Mar") == 0)
	{
		softsim_memcpy(buf, "03", 2);
	}
	else if(softsim_strcmp(month, "Apr") == 0)
	{
		softsim_memcpy(buf, "04", 2);
	}
	else if(softsim_strcmp(month, "May") == 0)
	{
		softsim_memcpy(buf, "05", 2);
	}
	else if(softsim_strcmp(month, "Jun") == 0)
	{
		softsim_memcpy(buf, "06", 2);
	}
	else if(softsim_strcmp(month, "Jul") == 0)
	{
		softsim_memcpy(buf, "07", 2);
	}
	else if(softsim_strcmp(month, "Aug") == 0)
	{
		softsim_memcpy(buf, "08", 2);
	}
	else if(softsim_strcmp(month, "Sep") == 0)
	{
		softsim_memcpy(buf, "09", 2);
	}
	else if(softsim_strcmp(month, "Oct") == 0)
	{
		softsim_memcpy(buf, "10", 2);
	}
	else if(softsim_strcmp(month, "Nov") == 0)
	{
		softsim_memcpy(buf, "11", 2);
	}
	else if(softsim_strcmp(month, "Dec") == 0)
	{
		softsim_memcpy(buf, "12", 2);
	}
	else
	{
		softsim_memcpy(buf, "99", 2);
	}
}

static u32 softsim_parse_date_string(s8 *buff, s8 *date_string)
{
	u32 len = 0;

	u32 index = 0;
	u8 day[10] = {0};
	u8 month[10] = {0};
	u8 year[10] = {0};
	u8 month_d[10] = {0};
	char *date_p = date_string;
	
	if(NULL == buff || NULL == date_string || 0 == softsim_strlen(date_string))
	{
		return len;
	}

 	/*parse month string*/
 	while((*date_p != '\0') && (*date_p != ' '))
 	{
		month[index++] = *date_p;
		date_p++;
	}

	while(*date_p == ' ')
	{
		date_p++;	
	}

	index = 0;

	while((*date_p != '\0') && (*date_p != ' '))
	{
		day[index++] = *date_p;
		date_p++;
	}

	while(*date_p == ' ')
	{
		date_p++;	
	}

	index = 0;

	while((*date_p != '\0') && (*date_p != ' '))
	{
		year[index++] = *date_p;
		date_p++;
	}

	softsim_strcpy(buff, year);
	buff[softsim_strlen(buff)] = '-';
	softsim_get_month(month_d, month);
	softsim_strcat(buff, month_d);
	buff[softsim_strlen(buff)] = '-';
	if(1 == softsim_strlen(day))
	{
		buff[softsim_strlen(buff)] = '0';
	}
	
	softsim_strcat(buff, day);
	date_p = NULL;
	
	len = softsim_strlen(buff);

	return len;
}

static u32 softsim_get_build_date_time(s8 *buf, u32 buf_len)
{
	s8 date[20] = {0};
	s8 time[20] = {0};
	u32 index = 0;

	if(NULL == buf)
	{
		return 0;
	}

	softsim_memcpy(date, __DATE__, softsim_strlen(__DATE__));
	softsim_memcpy(time, __TIME__, softsim_strlen(__TIME__));
	softsim_memset(buf, 0x00, buf_len);
	softsim_parse_date_string(buf, date);
	softsim_strcat(buf, " ");
	softsim_strcat(buf, time);
	index += softsim_strlen(buf);

	return index;
}

static bool softsim_get_main_version_byte(u8 *byte, u8 num)
{
	char *p1 = NULL;
	char *p2 = NULL;
	char buffer[3] = {0x00};
	
	if((NULL == byte) || (num < 3))
	{
		return false;
	}

	p1 = softsim_get_main_version_info();

	if(0 == softsim_strlen(p1))
	{
		return false;
	}

	/*skip space*/
	while(' ' == *p1)
	{
		p1++;
	}

	p2 = softsim_strstr(p1, ".");

	if((NULL == p2) || (p2-p1 > 2))
	{
		return false;
	}
	
	softsim_memset(buffer, 0x00, sizeof(buffer));
	softsim_strncpy(buffer, p1, p2-p1);
	*byte = softsim_atoi(buffer);
	p2++;	/*skip dot*/

	p1 = softsim_strstr(p2, ".");
	
	if((NULL == p1) || (p1-p2 > 2))
	{
		return false;
	}
		
	softsim_memset(buffer, 0x00, sizeof(buffer));
	softsim_strncpy(buffer, p2, p1-p2);
	*(byte+1) = softsim_atoi(buffer);
	p1++;

	p2 = p1;

	while((0 != *p1) && (' ' != *p1))
	{
		p1++;
	}

	if((p1-p2 > 2) || (p1 == p2))
	{
		return false;
	}
	
	softsim_memset(buffer, 0x00, sizeof(buffer));
	softsim_strncpy(buffer, p2, p1-p2);
	*(byte+2) = softsim_atoi(buffer);

	return true;
}

static bool softsim_get_build_date_byte(u8 *byte, u8 num)
{
	u8 *p1 = NULL;
	u8 *p2 = NULL;
	u8 buffer[5] = {0x00};
	u8 build_time[SOFTSIM_VERSION_STRING_LEN] = {0};
	u16 year_base = 2016;
	u16 year_max = 2031;
	s32 time = 0;
	
	if((NULL == byte) || (num < 4))
	{
		return false;
	}

	softsim_get_build_date_time(build_time, sizeof(build_time));
	p1 = build_time;

	if(0 == softsim_strlen(p1))
	{
		return false;
	}

	/*skip space*/
	while(' ' == *p1)
	{
		p1++;
	}

	p2 = softsim_strstr(p1, "-");

	if((NULL == p2) || (p2-p1 > 4))
	{
		return false;
	}

	/*parse year*/
	softsim_memset(buffer, 0x00, sizeof(buffer));
	softsim_strncpy(buffer, p1, p2-p1);
	time = softsim_atoi(buffer);
	if((time >= year_base) && (time <= year_max))
	{
		*byte = (time - year_base);
	}
	else
	{
		return false;
	}

	p2++;	/*skip '-'*/

	p1 = softsim_strstr(p2, "-");
	
	if((NULL == p1) || (p1-p2 > 2))
	{
		return false;
	}

	/*parse month*/
	softsim_memset(buffer, 0x00, sizeof(buffer));
	softsim_strncpy(buffer, p2, p1-p2);
	time = softsim_atoi(buffer);
	if((time >= 1) && (time <= 12))
	{
		*(byte+1) = time;
	}
	else
	{
		return false;
	}
	
	p1++;	/*skip '-'*/

	p2 = p1;

	while((0 != *p1) && (' ' != *p1))
	{
		p1++;
	}

	if((p1 == p2) || (p1-p2 > 2))
	{
		return false;
	}
	
	softsim_memset(buffer, 0x00, sizeof(buffer));
	softsim_strncpy(buffer, p2, p1-p2);

	time = softsim_atoi(buffer);
	if((time >= 1) && (time <= 15))
	{
		*(byte+2) = 0;
		*(byte+3) = time;
		
	}
	else if((time >= 16) && (time <= 31))
	{
		*(byte+2) = 1;
		*(byte+3) = (time-16);
	}
	else
	{
		return false;
	}

	return true;
}


u32 softsim_get_version_number(void)
{
	u32 version = 0;
	u8 version_byte[SOFTSIM_VERSION_BYTE+1] = {0x00};
	u8 buffer[SOFTSIM_VERSION_STRING_LEN] = {0x00};
	bool ret = true;
	u8 index = 0;

	softsim_memcpy(buffer, SOFTSIM_VERSION_TYPE, softsim_strlen(SOFTSIM_VERSION_TYPE));
	
	switch(buffer[0])
	{
		case 'A':
			version_byte[0] = 0xA;
			break;

		case 'B':
			version_byte[0] = 0xB;
			break;

		case 'R':
			version_byte[0] = 0xC;
			break;

		default:
			ret = false;
			break;
	}

	if(!ret)
	{
		SOFTSIM_DEBUG("Version type unsupport!");
		return 0xffffffff;
	}

	if(!softsim_get_main_version_byte(version_byte+1, 3))
	{
		SOFTSIM_DEBUG("Main Version error!");
		return 0xffffffff;
	}

	if(!softsim_get_build_date_byte(version_byte+4, 4))
	{
		SOFTSIM_DEBUG("Build date error!");
		return 0xffffffff;
	}

	for(index=0; index<SOFTSIM_VERSION_BYTE; index++)
	{
		version += version_byte[index] << ((SOFTSIM_VERSION_BYTE - index -1)*4);
	}

	SOFTSIM_DEBUG("version = 0x%X", version);

	return version;
}

void softsim_version_main(void)
{	
	u8 build_time[SOFTSIM_VERSION_STRING_LEN] = {0};
	u8 version_info[SOFTSIM_VERSION_STRING_LEN] = {0};
	 
	softsim_memcpy(version_info, SOFTSIM_VERSION_TYPE, softsim_strlen(SOFTSIM_VERSION_TYPE));
	softsim_strcat(version_info, " ");
	softsim_strcat(version_info, softsim_get_version_info());
	softsim_strcat(version_info, " ");
	softsim_get_build_date_time(build_time, SOFTSIM_VERSION_STRING_LEN);
	softsim_strcat(version_info, build_time);
	SOFTSIM_DEBUG("	TGT COS Version: %s", version_info);
}
