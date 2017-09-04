/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of TGT Limited. (C) 2017
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
 * Filename: softsim_auth_demo.c
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2017-01-16
 ******************************************************************************/

/*****************************************************************************
* Include																	   
*****************************************************************************/
#include "softsim_include.h"
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/select.h> 
#include <sys/time.h> 
#include <pthread.h>  
/*****************************************************************************
* Define																	   
*****************************************************************************/
#undef 	SOFTSIM_MD_ID
#undef 	SOFTSIM_TRACE_HEADER
#define SOFTSIM_MD_ID					SOFTSIM_MD_AUTHENTICATION
#define SOFTSIM_TRACE_HEADER			"[AUTH]" 

#define DEMO_SESSION_CONNECT_TIMES_MAX	(4)
/*****************************************************************************
* Typedef																	   
*****************************************************************************/

/*****************************************************************************
* Static Local Variable															  
*****************************************************************************/
static SoftsimSessionContext demo_session;
static u8 auth_demo_session_connect_times = 0;
static u16 auth_apdu_demo_sequence_number = 0;
static SoftsimFuncPtr softsim_auth_demo_session_creat_cb = NULL;
static s16 auth_demo_timer_id = -1;
static s16 auth_demo_rsp_timer_id = -1;
static bool auth_demo_rsp = false;
static bool auth_rsp_timeout_times = 0;
/*****************************************************************************
* Global Variable														 
*****************************************************************************/

/*****************************************************************************
* Extern Global Variable															
*****************************************************************************/

/*****************************************************************************
* Static Local Function															  
*****************************************************************************/
static void softsim_auth_demo_session_reset(void);
static void softsim_auth_demo_session_connected(SoftsimSessionConnected *info);
static void softsim_auth_demo_session_disconnected(SoftsimSessionDisconnected *info);
static void softsim_auth_demo_session_recieve(SoftsimSessionReceive *info);
static void softsim_auth_demo_connect_creat(void);
/*****************************************************************************
* Global Function														
*****************************************************************************/
	 
/*****************************************************************************
* Extern Global Function													  
*****************************************************************************/

/****************************************************************************
* Function
****************************************************************************/
static void softsim_auth_demo_session_connected(SoftsimSessionConnected *info)
{
	SoftsimSessionContext* ss = &demo_session;
	
	if(ss->session_id == info->session_id)
	{
		SOFTSIM_DEBUG("Auth demo session connected channel_id = %d, session_id = %d", info->net_info.channel, info->session_id);
		ss->state = SOFTSIM_SESSION_CONNECTED;
		auth_demo_session_connect_times = 0;
	}
	else
	{
		SOFTSIM_DEBUG("Auth demo session session_id = %d, connected session_id = %d!", ss->session_id, info->session_id);
	}
}

static void softsim_auth_demo_session_disconnected(SoftsimSessionDisconnected *info)
{
	SoftsimSessionContext* ss = &demo_session;
	
	if(ss->session_id == info->session_id)
	{
		SOFTSIM_DEBUG("Auth demo session disconnected channel_id = %d, session_id = %d, error_code = %d", info->net_info.channel, info->session_id, info->error_code);
		ss->session_id = SOFTSIM_INVALID_SESSION;
		ss->state = SOFTSIM_SESSION_IDLE;
	}
	else
	{
		SOFTSIM_DEBUG("Auth demo session session_id = %d, connected session_id = %d, error_code = %d", ss->session_id, info->session_id, info->error_code);
	}

	if(SOFTSIM_SESSION_SUCCESS != info->error_code)
	{
		if(SOFTSIM_SESSION_ERROR_CONNECT_TIMEOUT == info->error_code 
			|| SOFTSIM_SESSION_ERROR_CONNECT == info->error_code)
		{
			if(auth_demo_session_connect_times < DEMO_SESSION_CONNECT_TIMES_MAX)
			{
				softsim_auth_demo_session_recreat();
			}
		}
		else if((SOFTSIM_SESSION_ERROR_SERVER_BREAK == info->error_code) && (SOFTSIM_SESSION_IDLE == ss->state))
		{
			softsim_auth_demo_session_recreat();
			auth_demo_session_connect_times = 0;
		}
	}
}

static void softsim_auth_demo_session_recieve(SoftsimSessionReceive *info)
{
	u16 apdu_length = 0;
	u16 index = 0;
	u16 seq_num = 0;
	u16 slot_id = 0;
	SoftsimSessionContext* ss = &demo_session;
	
	if(NULL == info)
	{
		return;
	}

	if((0xAA == info->data[0]) || (0xA5 == info->data[0]))
	{
		SOFTSIM_PRINT_HEX("Recieve AUTH-RSP:", info->data, info->data_len);
		auth_demo_rsp = true;
		SOFTSIM_TIMER_STOP(auth_demo_rsp_timer_id);
		auth_rsp_timeout_times = 0;

		index += 2;
		byte_8_16(seq_num, info->data[index++], info->data[index++]);
		byte_8_16(slot_id, info->data[index++], info->data[index++]);
		byte_8_16(apdu_length, info->data[index++], info->data[index++]);

		if(seq_num != auth_apdu_demo_sequence_number)
		{
			SOFTSIM_DEBUG("Auth apdu demo sequence number exception!");
		}
	}
}

static void softsim_auth_demo_connect_creat(void)
{
	SoftsimSessionConnectRequest connectreq;
	SoftsimSessionCallback callback;
	SoftsimInformation *info = softsim_get_infomation();
	SoftsimSessionContext *ss = &demo_session;
	
	softsim_memset(&connectreq, 0, sizeof(connectreq));
	softsim_memset(&callback, 0, sizeof(callback));
	softsim_strcpy(connectreq.net_info.domain, info->address.ip);
	
	connectreq.net_info.channel = softsim_get_current_channel();
	connectreq.net_info.port = info->address.port;

	/*socket option*/
	connectreq.net_info.keepalive = 1;
	connectreq.net_info.keepidle = 5;
	connectreq.net_info.keepinterval = 5;
	connectreq.net_info.keepcount = 3;
	
	callback.connected_cb = softsim_auth_demo_session_connected;
	callback.disconnected_cb = softsim_auth_demo_session_disconnected;
	callback.receive_cb = softsim_auth_demo_session_recieve;

	if((0 == connectreq.net_info.port) || (0xffff == connectreq.net_info.port)
		|| (0 == softsim_strlen(connectreq.net_info.domain)))
	{
		SOFTSIM_DEBUG("Invalid IP and Port!");
		return;
	}

	ss->heartbeat = true;
	ss->session_id = softsim_session_connect(&connectreq, &callback);
	ss->channel = softsim_get_current_channel();
	auth_demo_session_connect_times++;

	if(auth_demo_session_connect_times > DEMO_SESSION_CONNECT_TIMES_MAX)
	{
		SOFTSIM_DEBUG("Demo session creat times up to MAX!");
		return;
	}
	
	if(ss->session_id >= SOFTSIM_SESSION_SUCCESS)
	{
		ss->state = SOFTSIM_SESSION_CONNECTING;
	}
	else if(ss->session_id == SOFTSIM_SESSION_ERROR_CONNECT)
	{
		SOFTSIM_DEBUG("Auth demo session error!! %d", ss->session_id);

		if(softsim_get_current_channel() < SOFTSIM_CHANNEL_MAX)
		{
			softsim_auth_demo_session_recreat();
		}
	}
	else
	{
		SOFTSIM_DEBUG("Auth demo session error %d!!", ss->session_id);
	}
}

void softsim_auth_demo_session_close(void)
{
	if(SOFTSIM_INVALID_SESSION != demo_session.session_id)
	{
		softsim_session_close(demo_session.session_id);
		usleep(500*1000);
		demo_session.session_id = SOFTSIM_INVALID_SESSION;
		demo_session.state = SOFTSIM_SESSION_IDLE;
	}
}


void softsim_auth_demo_session_recreat(void)
{
	SOFTSIM_DEBUG_MARK;
	SoftsimSessionContext *ss = &demo_session;
	if(SOFTSIM_SESSION_CONNECTING == ss->state)
	{
		SOFTSIM_DEBUG("Session in progress!");
		return;
	}
	softsim_auth_demo_session_close();
	demo_session.session_id = SOFTSIM_INVALID_SESSION;
	softsim_auth_demo_connect_creat();
}

void softsim_auth_demo_session_init(void)
{
	softsim_auth_demo_session_creat_cb = softsim_auth_demo_session_recreat;
	softsim_auth_demo_session_close();
	demo_session.session_id = SOFTSIM_INVALID_SESSION;
	softsim_auth_demo_connect_creat();
}

void softsim_auth_demo_session_reset(void)
{
	
}

void softsim_auth_demo_session_exit(void)
{
	softsim_auth_demo_session_close();
	softsim_auth_demo_session_reset();
}

void softsim_auth_demo_session_update(s32 ps_rt, s32 vs_rt)
{
	if(1 == ps_rt || 1 == vs_rt)
	{
		if(SOFTSIM_SESSION_IDLE == demo_session.state)
		{
			softsim_auth_demo_session_init();
		}
	}
}

void softsim_auth_demo_init(void)
{
	softsim_auth_demo_session_init();
}

void softsim_auth_demo_response_timeout(void)
{
	if(auth_rsp_timeout_times > 15)
	{
		SOFTSIM_DEBUG("Auth demo rsp timeout!");
		SOFTSIM_TIMER_STOP(auth_demo_rsp_timer_id);
		auth_demo_rsp = true;
		auth_rsp_timeout_times = 0;
	}
	else
	{
		if(auth_demo_rsp)
		{
			SOFTSIM_TIMER_STOP(auth_demo_rsp_timer_id);
			auth_rsp_timeout_times = 0;
		}
		else
		{
			auth_rsp_timeout_times++;
		}
	}
}

void softsim_auth_demo_send_request(void)
{
	u8 send_buff[512] = {0};
	bool send_status = false;
	u16 length = 0;
	u16 auth_apdu_length = 0;
	u16 index = 0;
	char auth_req[] = {0x00, 0x88, 0x00, 0x81, 0x22, 
		0x10, 0x50, 0x7E, 0x34, 0xA1, 0xFA, 0xD2, 0xD7, 0x5E, 0x29, 0x73, 0x89, 0x94, 0x5C, 0xCA, 0x4D, 0xD8, 
		0x10, 0xEC, 0x71, 0xCF, 0x42, 0x44, 0x74, 0x00, 0x00, 0x75, 0x8B, 0x3B, 0xF7, 0x8B, 0x05, 0x71, 0x78};

	index = 0;
	send_buff[index++] = 0xAA;	/*Header*/
	send_buff[index++] = 0x66;	/*Header*/
	byte_16_8(send_buff[index++], send_buff[index++], auth_apdu_demo_sequence_number);
	byte_16_8(send_buff[index++], send_buff[index++], (u16)softsim_get_sim_id());
	send_buff[index++] = sizeof(auth_req);	/*APDU Length*/
	softsim_memcpy(send_buff+index, auth_req, sizeof(auth_req));
	index += sizeof(auth_req);

	SOFTSIM_PRINT_HEX("Send AUTH-REQ:", send_buff, index);
	if(softsim_session_send(demo_session.session_id, send_buff, index) < 0)
	{
		SOFTSIM_DEBUG("Failed to send authentication data!");
		softsim_system_cmd("ping -c 3 www.baidu.com");
	}
	else
	{
		auth_demo_rsp = false;
		SOFTSIM_TIMER_STOP(auth_demo_rsp_timer_id);
		auth_demo_rsp_timer_id = softsim_timer_create(softsim_auth_demo_response_timeout,1000);
	}

}

void softsim_auth_demo_request_timeout(void)
{
	s32 ret = -1;
	
	if(SOFTSIM_SESSION_CONNECTED == demo_session.state)
	{
		auth_apdu_demo_sequence_number++;
		if(0 == auth_apdu_demo_sequence_number)
		{
			SOFTSIM_DEBUG("Auth apdu sequence number overflow!");
			auth_apdu_demo_sequence_number++;
		}

		softsim_auth_demo_send_request();
	}
	else if(SOFTSIM_SESSION_IDLE == demo_session.state)
	{
		if(NULL != softsim_auth_demo_session_creat_cb)
		{
			softsim_auth_demo_session_creat_cb();
		}
	}
}

void softsim_auth_demo_main(void)
{
	auth_demo_rsp = false;
	SOFTSIM_TIMER_STOP(auth_demo_timer_id);
	auth_demo_timer_id = softsim_timer_create(softsim_auth_demo_request_timeout, 30*1000);
}
