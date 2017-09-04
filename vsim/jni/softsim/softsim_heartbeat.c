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
 * Filename: softsim_heartbeat.c
 *
 * Description:
 *
 * Author: tongtong.sun
 * 
 * Date: 2016-08-03
 ******************************************************************************/

/*****************************************************************************
* Include																	   
*****************************************************************************/
#include "softsim_include.h"
#include "softsim_heartbeat.h"
#include <dlfcn.h> 
#include <string.h>  
#include <sys/types.h>  
#include <unistd.h>
#include <fcntl.h>
#include "stdio.h"
#include <pthread.h>
/*****************************************************************************
* Define																	   
*****************************************************************************/
#undef 	SOFTSIM_MD_ID
#undef 	SOFTSIM_TRACE_HEADER
#define SOFTSIM_MD_ID					SOFTSIM_MD_HEARTBEAT
#define SOFTSIM_TRACE_HEADER			"[HEARTBEAT]" 

#define SOFTSIM_HEARTBEAT_CHECK_TIMES_MAX	(10)
#define SOFTSIM_HEARTBEAT_CHECK_TIMEOUT	(1*1000)
#define SOFTSIM_HEARTBEAT_EXCEPTION_TIMES_MAX	(3)
/*****************************************************************************
* Typedef																	   
*****************************************************************************/

/*****************************************************************************
* Static Local Variable															  
*****************************************************************************/
static SoftsimFuncPtr heartbeat_callback = NULL;
static s32 heartbeat_check_timer_id = -1;
static u8 heartbeat_check_times = 0;
static u8 heartbeat_exception_times = 0;
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

static void softsim_heartbeat_status_check_timeout(void)
{
	SoftsimSessionContext *ss = softsim_get_auth_session();

	if(ss->heartbeat)
	{
		softsim_heartbeat_status_check_stop();
	}
	else
	{
		if(heartbeat_check_times > SOFTSIM_HEARTBEAT_CHECK_TIMES_MAX)
		{
			SOFTSIM_DEBUG("Heartbeat timeout!");
			softsim_heartbeat_status_check_stop();
			heartbeat_exception_times++;

			if(heartbeat_exception_times > SOFTSIM_HEARTBEAT_EXCEPTION_TIMES_MAX)
			{
				softsim_channel_exception_report(ss->channel);
			}
		}
		else
		{
			heartbeat_check_times++;
		}
	}
}

static void softsim_heartbeat_status_check_start(void)
{
	SOFTSIM_TIMER_STOP(heartbeat_check_timer_id);
	heartbeat_check_times = 0;
	heartbeat_check_timer_id = softsim_timer_create(softsim_heartbeat_status_check_timeout, SOFTSIM_HEARTBEAT_CHECK_TIMEOUT);
}

static s32 softsim_heartbeat_send(void)
{
	s32 ret = 0;
	u8 times = 0;
	SoftsimSessionContext *ss = softsim_get_auth_session();
	SoftsimInformation *info = softsim_get_infomation();
		
	if((SOFTSIM_INVALID_SESSION != ss->session_id) && (SOFTSIM_SESSION_CONNECTED == ss->state))
	{
		if(ss->heartbeat)
		{
			SOFTSIM_TRACE_HEX("Send HB-REQ:", info->ssid, softsim_strlen(info->ssid));

			while(times < 5)
			{
				softsim_channel_info(ss->channel);
				ret = softsim_session_send(ss->session_id, info->ssid, softsim_strlen(info->ssid));

				if(0 != ret)
				{
					SOFTSIM_DEBUG("softsim heartbeat send fail! ret = %d", ret);
					times++;
					usleep(1000*1000);
				}
				else
				{
					ss->heartbeat = false;
					softsim_heartbeat_status_check_start();
					break;
				}
			}
		}
		else
		{
			softsim_socket_heart_exception();
			ss->heartbeat = true;
		}
	}
	
	return 0;
}

s32 softsim_send_signal(s32 signal)
{
#if defined(__SOFTSIM_MONITOR__)
	if(signal == 1)
	{
		if(NULL != heartbeat_callback)
		{
			heartbeat_callback();
		}
		else
		{
			return 1;
		}
		
	}
	else
	{
		return 1;	
	}
#endif
	
	return 0;
}

void softsim_heartbeat_status_check_stop(void)
{
	SOFTSIM_TIMER_STOP(heartbeat_check_timer_id);
	heartbeat_check_times = 0;
}

void softsim_heartbeat_exception_reset(void)
{
	heartbeat_exception_times = 0;
}

void softsim_heartbeat_init(void)
{
	heartbeat_callback = NULL;
	heartbeat_exception_times = 0;
}

void softsim_heartbeat_status_update(bool status)
{
	if(status)
	{
		heartbeat_callback = softsim_heartbeat_send;
	}
	else
	{
		heartbeat_callback = NULL;
	}
}

void softsim_heartbeat_exit(void)
{
	heartbeat_callback = NULL;
}

