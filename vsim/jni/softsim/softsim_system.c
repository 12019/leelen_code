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
 * Filename: softsim_system.c
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
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>

#include "softsim_type.h"
#include "softsim_system.h"
#include "softsim_trace.h"
/*****************************************************************************
* Define																	   
*****************************************************************************/
#undef 	SOFTSIM_MD_ID
#undef 	SOFTSIM_TRACE_HEADER
#define SOFTSIM_MD_ID					SOFTSIM_MD_SYSTEM
#define SOFTSIM_TRACE_HEADER			"[SYSTEM]" 

/*****************************************************************************
* Typedef																	   
*****************************************************************************/
typedef struct
{
	SoftsimDateTime time;
	u32 run_msec;
}SoftsimRunTime;

/*****************************************************************************
* Static Local Variable															  
*****************************************************************************/
static SoftsimRunTime gmt_time;
static long start_msec;
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
void softsim_get_local_time(SoftsimDateTime *date_time)
{
#if 0
	time_t now; 		
	struct tm w;
		
	time(&now);
	localtime_r(&now, &w);
	date_time->year = w.tm_year+1900;
	date_time->month = w.tm_mon+1;
	date_time->day = w.tm_mday;
	date_time->hour = w.tm_hour;
	date_time->minute = w.tm_min;
	date_time->second = w.tm_sec;
	date_time->week = 0;
#else
	struct timeval tv;
	struct tm w;
	gettimeofday(&tv, NULL);
	localtime_r(&tv.tv_sec, &w);
	
	date_time->year = w.tm_year+1900;
	date_time->month = w.tm_mon+1;
	date_time->day = w.tm_mday;
	date_time->hour = w.tm_hour;
	date_time->minute = w.tm_min;
	date_time->second = w.tm_sec;
	date_time->usecond = tv.tv_usec;
	date_time->week = 0;
#endif
}

void softsim_get_system_time(SoftsimDateTime *date_time)
{
	if(gmt_time.run_msec == 0)
	{
		time_t now; 		
		struct tm w;
			
		time(&now);
		//localtime_r(&now, &w);
		now += SOFTSIM_SYSTEM_TIME_ZONE*60*60;
		gmtime_r(&now, &w);
		date_time->year = w.tm_year+1900;
		date_time->month = w.tm_mon+1;
		date_time->day = w.tm_mday;
		date_time->hour = w.tm_hour;
		date_time->minute = w.tm_min;
		date_time->second = w.tm_sec;
		date_time->week = 0;
	}
	else
	{
		u32 current_run_msec = 0;
		u32 current_date_sec = 0;
		current_run_msec = softsim_get_system_msec();
		current_date_sec =	softsim_utc_date_to_sec(&gmt_time.time);
		current_date_sec += (current_run_msec - gmt_time.run_msec)/1000;
		current_date_sec += SOFTSIM_SYSTEM_TIME_ZONE*60*60;	
		softsim_utc_sec_to_date(current_date_sec, date_time);
	}
}

void softsim_set_system_time(SoftsimDateTime *date_time)
{
	softsim_memcpy(&gmt_time.time, date_time, sizeof(SoftsimDateTime));
	gmt_time.run_msec = softsim_get_system_msec();
}

u32 softsim_get_system_msec(void)
{
	struct timespec now;
	long msec = 0;
	long current = 0;
	
	clock_gettime(CLOCK_BOOTTIME, &now);
	current = (now.tv_sec*1000 + (now.tv_nsec/1000)/1000);
	msec = current - start_msec;
	return (u32)msec;
}

bool softsim_utc_sec_to_date(u32 utc_sec, SoftsimDateTime *date_time)
{
	struct tm w;
	time_t cur = (time_t)utc_sec; 
	
	localtime_r(&cur, &w);
	date_time->year = w.tm_year+1900;
	date_time->month = w.tm_mon+1;
	date_time->day = w.tm_mday;
	date_time->hour = w.tm_hour;
	date_time->minute = w.tm_min;
	date_time->second = w.tm_sec;
	date_time->week = 0;	
	return false;
}

u32 softsim_utc_date_to_sec(SoftsimDateTime *date_time)
{
	struct tm current;
	time_t ret_sec; 

	softsim_memset(&current, 0, sizeof(current));
	current.tm_year = date_time->year-1900;
	current.tm_mon = date_time->month-1;
	current.tm_mday = date_time->day;
	current.tm_hour = date_time->hour;
	current.tm_min = date_time->minute;
	current.tm_sec = date_time->second;
	
	ret_sec = mktime(&current);
	
	return (u32)ret_sec;
}

static void * softsim_system_cmd_callback(void *arg)
{
	char* cmd_buffer = arg;
	FILE *file;
	char read_buffer[SOFTSIM_SYSTEM_CMD_RESULT_LEN];

	if(NULL == cmd_buffer)
	{
		return NULL;
	}
	
	file = popen(cmd_buffer, "r");
	if(NULL != file)
	{
	    while(fgets(read_buffer, sizeof(read_buffer), file) != NULL)
	    {
	    	SOFTSIM_DEBUG("%s", read_buffer);
			softsim_memset(read_buffer, 0, sizeof(read_buffer));
	    }
	    pclose(file);
	}
	free(cmd_buffer);
	return NULL;
}

void softsim_system_cmd(const char* cmd_buffer)
{
	s32 err;
	pthread_t  thread_id;
	char* temp_buffer = NULL;

	temp_buffer = malloc(softsim_strlen(cmd_buffer)+1);
	if(NULL == temp_buffer)
	{
		return;	
	}
	softsim_strcpy(temp_buffer, cmd_buffer);
	temp_buffer[softsim_strlen(cmd_buffer)] = 0;
	SOFTSIM_DEBUG("Cmd:%s", temp_buffer);
	err = pthread_create(&thread_id, NULL, softsim_system_cmd_callback, temp_buffer);
	if(err == 0)  
	{
		pthread_detach(thread_id);
	}  
}
