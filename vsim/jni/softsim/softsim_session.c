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
 * Filename: softsim_session.c
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-08-10
 ******************************************************************************/

/*****************************************************************************
* Include																	   
*****************************************************************************/
#include "softsim_include.h"
/*****************************************************************************
* Define																	   
*****************************************************************************/
#undef 	SOFTSIM_MD_ID
#undef 	SOFTSIM_TRACE_HEADER
#define SOFTSIM_MD_ID					SOFTSIM_MD_SESSION
#define SOFTSIM_TRACE_HEADER			"[SESSION]"

#define SOFTSIM_SESSION_MAX	(10)
#define SOFTSIM_SESSION_START 		(1)
#define SOFTSIM_SESSION_MAX_DATA_LEN		(2048)

#define SOFTSIM_SESSION_CONNECT_TIMEOUT	(6*1000)
/*****************************************************************************
* Typedef																	   
*****************************************************************************/

/*****************************************************************************
* Static Local Variable															  
*****************************************************************************/
static SoftsimSessionStruct softsim_session_array[SOFTSIM_SESSION_MAX];
/*****************************************************************************
* Global Variable														 
*****************************************************************************/

/*****************************************************************************
* Extern Global Variable															
*****************************************************************************/

/*****************************************************************************
* Static Local Function															  
*****************************************************************************/
static s32 softsim_session_create(s32 socket_fd, SoftsimSessionConnectRequest *info, SoftsimSessionCallback *ss_cb);
static void softsim_session_delete(s32 session_id);
static s32 softsim_get_session_id_by_socket_fd(s32 socket_fd);
static s32 softsim_session_check_write_handle(s32 session_id);
static void softsim_session_error_handle(s32 session_id, s32 error_code);
static void softsim_session_connected(s32 session_id);
static void softsim_session_receive(s32 session_id, u8 *data, u16 data_len);
static void softsim_session_connect_timeout(s32 timer_id, void *arg);
static void softsim_session_check_connect_timeout_start(s32 session_id);
static void softsim_session_check_connect_timeout_stop(s32 session_id);
static void softsim_session_notify(SoftsimSocketNotify *notify);
/*****************************************************************************
* Global Function														
*****************************************************************************/
	 
/*****************************************************************************
* Extern Global Function													  
*****************************************************************************/

/****************************************************************************
* Function
****************************************************************************/
//find a ununsed session, session start from 1
static s32 softsim_session_create(s32 socket_fd, SoftsimSessionConnectRequest *info, SoftsimSessionCallback *ss_cb)
{
	static u32 unuse_index = 0;
	u32 i;

	for(i = 0;i < SOFTSIM_SESSION_MAX;i++)
	{
		unuse_index++;
		unuse_index = unuse_index % SOFTSIM_SESSION_MAX;
		if(unuse_index == 0)
		{
			continue;
		}
		
		if((softsim_session_array[unuse_index].socket_fd == SOFTSIM_INVALID_SOCKET_FD) &&
		   (softsim_session_array[unuse_index].state == SOFTSIM_SESSION_IDLE))
		{
			break;
		}
	}

	if(i >= SOFTSIM_SESSION_MAX)
	{
		SOFTSIM_ERROR("Not Enough Resource!!");
		return SOFTSIM_SESSION_ERROR_LIMITED_RESOURCE;
	}

	softsim_session_array[unuse_index].socket_fd = socket_fd;
	softsim_session_array[unuse_index].state = SOFTSIM_SESSION_CONNECTING;
	softsim_memcpy(&(softsim_session_array[unuse_index].net_info), &(info->net_info), sizeof(SoftsimSessionNetInfo));
	softsim_memcpy(&(softsim_session_array[unuse_index].ss_cb), ss_cb, sizeof(SoftsimSessionCallback));
	softsim_session_array[unuse_index].timeout_secends = info->timeout_secends;

	return unuse_index;
}

static void softsim_session_delete(s32 session_id)
{
	if((session_id < SOFTSIM_SESSION_START) ||
	   (session_id >= SOFTSIM_SESSION_MAX))
	{
		return;
	}

	if((softsim_session_array[session_id].socket_fd == SOFTSIM_INVALID_SOCKET_FD) &&
		   (softsim_session_array[session_id].state == SOFTSIM_SESSION_IDLE))
	{
		return;
	}

	softsim_session_check_connect_timeout_stop(session_id);
	softsim_socket_close(softsim_session_array[session_id].socket_fd);
	
	if(NULL != softsim_session_array[session_id].data)
	{
		free(softsim_session_array[session_id].data);
	}
	softsim_memset(&(softsim_session_array[session_id]), 0, sizeof(SoftsimSessionStruct));
	softsim_session_array[session_id].socket_fd = SOFTSIM_INVALID_SOCKET_FD;
	softsim_session_array[session_id].timer_id = SOFTSIM_INVAILD_TIMERID;
}

static s32 softsim_get_session_id_by_socket_fd(s32 socket_fd)
{
	u32 i;

	for(i = 0;i < SOFTSIM_SESSION_MAX;i++)
	{
		if(softsim_session_array[i].socket_fd == socket_fd)
		{
			break;
		}
	}

	if(i >= SOFTSIM_SESSION_MAX)
	{
		SOFTSIM_ERROR("Can not find socket_fd = %d", socket_fd);
		return SOFTSIM_INVALID_SESSION;
	}

	return i;
}

static s32 softsim_session_check_write_handle(s32 session_id)
{
	if((session_id < SOFTSIM_SESSION_START) ||
	   (session_id >= SOFTSIM_SESSION_MAX))
	{
		return SOFTSIM_SESSION_ERROR_PARAM;
	}

	if(softsim_session_array[session_id].socket_fd == SOFTSIM_INVALID_SOCKET_FD)
	{
		return SOFTSIM_SESSION_ERROR_INVALID_HANDLE;
	}

	if(softsim_session_array[session_id].state != SOFTSIM_SESSION_CONNECTED)
	{
		return SOFTSIM_SESSION_ERROR_NOT_CONNECTED;
	}

	return SOFTSIM_SESSION_SUCCESS;
}

static void softsim_session_error_handle(s32 session_id, s32 error_code)
{
	SoftsimSessionDisconnected info;
	SoftsimSessionCallback ss_cb;
		
	if((session_id < SOFTSIM_SESSION_START) ||
	   (session_id >= SOFTSIM_SESSION_MAX))
	{
		SOFTSIM_DEBUG_MARK;
		return;
	}

	if(error_code == SOFTSIM_SESSION_SUCCESS)
	{
		SOFTSIM_DEBUG_MARK;
		return;
	}

	info.session_id = session_id;
	info.error_code = error_code;
	softsim_memcpy(&(info.net_info), &(softsim_session_array[session_id].net_info), sizeof(SoftsimSessionNetInfo));
	softsim_memcpy(&ss_cb, &(softsim_session_array[session_id].ss_cb), sizeof(SoftsimSessionCallback));
	softsim_session_delete(session_id);
	
	if(NULL != ss_cb.disconnected_cb)
	{
		ss_cb.disconnected_cb(&info);
	}
}

static void softsim_session_connected(s32 session_id)
{
	SoftsimSessionConnected info;
	SoftsimSessionCallback ss_cb;
		
	if((session_id < SOFTSIM_SESSION_START) ||
	   (session_id >= SOFTSIM_SESSION_MAX))
	{
		SOFTSIM_DEBUG_MARK;
		return;
	}

	softsim_session_array[session_id].state = SOFTSIM_SESSION_CONNECTED;

	info.session_id = session_id;
	softsim_memcpy(&(info.net_info), &(softsim_session_array[session_id].net_info), sizeof(SoftsimSessionNetInfo));
	softsim_memcpy(&ss_cb, &(softsim_session_array[session_id].ss_cb), sizeof(SoftsimSessionCallback));
	
	if(NULL != ss_cb.connected_cb)
	{
		ss_cb.connected_cb(&info);
	}
}

static void softsim_session_receive(s32 session_id, u8 *data, u16 data_len)
{
	SoftsimSessionReceive info;
	SoftsimSessionCallback ss_cb;

	softsim_memset(&info, 0x00, sizeof(info));
	softsim_memset(&ss_cb, 0x00, sizeof(ss_cb));
	info.session_id = session_id;
	softsim_memcpy(&(info.net_info), &(softsim_session_array[session_id].net_info), sizeof(SoftsimSessionNetInfo));
	info.data_len = data_len;
	info.data = data;
	
	softsim_memcpy(&ss_cb, &(softsim_session_array[session_id].ss_cb), sizeof(SoftsimSessionCallback));
	
	if(NULL != ss_cb.receive_cb)
	{
		ss_cb.receive_cb(&info);
	}
}

static void softsim_session_connect_timeout(s32 timer_id, void *arg)
{
	s32 session_id = 0;
	u8 *p = (u8*)arg;

	if(NULL == arg)
	{
		SOFTSIM_DEBUG_MARK;
		return;
	}

	byte_8_32(session_id, *p, *(p+1), *(p+2), *(p+3));
	
	SOFTSIM_DEBUG("softsim_session_connect_timeout session_id = %d", session_id);
	softsim_session_error_handle(session_id, SOFTSIM_SESSION_ERROR_CONNECT_TIMEOUT);
}

static void softsim_session_check_connect_timeout_start(s32 session_id)
{
	u8 argument[4] = {0x00};
	if((session_id < SOFTSIM_SESSION_START) ||
	   (session_id >= SOFTSIM_SESSION_MAX))
	{
		SOFTSIM_DEBUG_MARK;
		return;
	}

	byte_32_8(argument[0], argument[1], argument[2], argument[3], session_id);
	softsim_session_array[session_id].timer_id = softsim_timer_ex_create(softsim_session_connect_timeout, SOFTSIM_SESSION_CONNECT_TIMEOUT, (void*)argument, sizeof(argument));
	SOFTSIM_DEBUG("softsim_session_check_connect_timeout_start session_id = %d, timer_id = %lld", session_id, softsim_session_array[session_id].timer_id);
}

static void softsim_session_check_connect_timeout_stop(s32 session_id)
{
	if((session_id < SOFTSIM_SESSION_START) ||
	   (session_id >= SOFTSIM_SESSION_MAX))
	{
		SOFTSIM_DEBUG_MARK;
		return;
	}
	
	SOFTSIM_DEBUG("softsim_session_check_connect_timeout_stop session_id = %d", session_id);
	SOFTSIM_TIMER_STOP(softsim_session_array[session_id].timer_id);
}


static void softsim_session_notify(SoftsimSocketNotify *notify)
{
	s32 session_id = softsim_get_session_id_by_socket_fd(notify->socket_fd);
	/*SOFTSIM_DEBUG("result = %d", notify->result);
	SOFTSIM_DEBUG("socket_fd = %d", notify->socket_fd);
	SOFTSIM_DEBUG("event = %d", notify->event_type);
	SOFTSIM_DEBUG("error_code = %d", notify->error_code);
	SOFTSIM_DEBUG("detail_error = %d", notify->detail_error);*/

	if(session_id < 0)
	{
		return;
	}

	if((!notify->result) || (notify->event_type == SOFTSIM_SOCKET_CLOSE))
	{
		if(softsim_session_array[session_id].state == SOFTSIM_SESSION_CONNECTED)
		{
			softsim_session_error_handle(session_id, SOFTSIM_SESSION_ERROR_SERVER_BREAK);
		}
		else
		{
			if(notify->error_code == SOFTSIM_SOCKET_GETHOSTBYNAME_FAILED)
			{
				softsim_session_error_handle(session_id, SOFTSIM_SESSION_ERROR_GETHOSTBYNAME);
			}
			else
			{
				softsim_session_error_handle(session_id, SOFTSIM_SESSION_ERROR_CONNECT);
			}
		}
		return;
	}

	if(notify->result)
	{
		if(notify->event_type == SOFTSIM_SOCKET_READ)
		{
			s32 ret = 0;
			u8 *recv_buffer = NULL;

			recv_buffer = malloc(SOFTSIM_SESSION_MAX_DATA_LEN);
			if(NULL == recv_buffer)
			{
				SOFTSIM_ERROR("Error!! Malloc size = %d", SOFTSIM_SESSION_MAX_DATA_LEN);
				return;
			}
			
			do
			{
				softsim_memset(recv_buffer, 0, SOFTSIM_SESSION_MAX_DATA_LEN);
				ret = softsim_socket_recv(notify->socket_fd, recv_buffer, SOFTSIM_SESSION_MAX_DATA_LEN);
				if(ret > 0)
				{
					softsim_session_receive(session_id, recv_buffer, ret);
				}
				
			} while(ret == SOFTSIM_SESSION_MAX_DATA_LEN);

			free(recv_buffer);
		}		
		else if(notify->event_type == SOFTSIM_SOCKET_WRITE)
		{
		}
		else if(notify->event_type == SOFTSIM_SOCKET_CONNECT)
		{
			softsim_session_check_connect_timeout_stop(session_id);
			softsim_session_connected(session_id);
		}
	}
}

s32 softsim_session_connect(SoftsimSessionConnectRequest *info, SoftsimSessionCallback *ss_cb)
{
	s32 session_id;
	s32 socket_fd;
	s32 ret;
	SoftsimSocketDomain domain;
	SoftsimSocketAddress address;
	
	if(info->net_info.channel == SOFTSIM_PHYSICAL_SIM_CHANNEL)
	{
		domain = SOFTSIM_SOCKET_PF_INET_PHYSICALSIM;
	}
	else if(info->net_info.channel == SOFTSIM_VIRTUAL_SIM_CHANNEL)
	{
		domain = SOFTSIM_SOCKET_PF_INET_SOFTSIM;
	}
	else
	{
		SOFTSIM_ERROR("Invalid channel param");
		return SOFTSIM_SESSION_ERROR_PARAM;
	}
	
	socket_fd = softsim_socket_create(domain, SOFTSIM_SOCKET_STREAM);
	if(socket_fd < 0)
	{
		SOFTSIM_ERROR("Socket create fail, socket_fd = %d", socket_fd);
		return SOFTSIM_SESSION_ERROR_SOCKET_CREATE;
	}

	if(1 == info->net_info.keepalive)
	{
		SOFTSIM_DEBUG("keepalive = %d, keepidle = %d, keepinterval = %d, keepcount = %d", info->net_info.keepalive, info->net_info.keepidle, info->net_info.keepinterval, info->net_info.keepcount);
		softsim_socket_setsockopt(socket_fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&(info->net_info.keepalive), sizeof(info->net_info.keepalive));
		softsim_socket_setsockopt(socket_fd, SOL_TCP, TCP_KEEPIDLE, (void*)&(info->net_info.keepidle) , sizeof((info->net_info.keepidle)));
		softsim_socket_setsockopt(socket_fd, SOL_TCP, TCP_KEEPINTVL, (void *)&(info->net_info.keepinterval) , sizeof((info->net_info.keepinterval)));
		softsim_socket_setsockopt(socket_fd, SOL_TCP, TCP_KEEPCNT, (void *)&(info->net_info.keepcount) , sizeof((info->net_info.keepcount)));
	}

	session_id = softsim_session_create(socket_fd, info, ss_cb);
	if(session_id < 0)
	{
		softsim_socket_close(socket_fd);
		return session_id;
	}

	softsim_memset(&address, 0, sizeof(address));
	address.type = SOFTSIM_SOCKET_STREAM;
	address.port = info->net_info.port;
	address.address_type = SOFTSIM_ADDRESS_TYPE_IP;
	softsim_strncpy(address.domain, info->net_info.domain, sizeof(address.domain) - 1);
	
	SOFTSIM_DEBUG("socket_fd = %d, session_id = %d", socket_fd, session_id);
	
	ret = softsim_socket_connect(socket_fd, &address);
	if(ret == SOFTSIM_SOCKET_WOULDBLOCK)
	{
		softsim_socket_set_event_notify(socket_fd, softsim_session_notify);
	}
	else
	{
		SOFTSIM_ERROR("Connect Error (%d) !!", ret);
		softsim_session_delete(session_id);
		return SOFTSIM_SESSION_ERROR_CONNECT;
	}

	softsim_session_check_connect_timeout_start(session_id);

	return session_id;
}

s32 softsim_session_send(s32 session_id, u8 *data, u16 data_len)
{
	s32 send_ret, ret;
	s32 send_index = 0;
	s32 socket_fd;

	ret = softsim_session_check_write_handle(session_id);

	if(ret != SOFTSIM_SESSION_SUCCESS)
	{
		SOFTSIM_ERROR("session write error!! check ret = %d", ret);
		return ret;
	}

	socket_fd = softsim_session_array[session_id].socket_fd;

	if(SOFTSIM_INVALID_SOCKET_FD == socket_fd)
	{
		SOFTSIM_ERROR("session socket invalid!");
		return ret;
	}

	send_ret = softsim_socket_send(socket_fd, data, data_len);

	SOFTSIM_DEBUG("session_id = %d, socket_fd = %d", session_id, socket_fd);

	if(send_ret != data_len)
	{
		SOFTSIM_ERROR("session write error!! data_len = %d, send_ret = %d", data_len, send_ret);
		return SOFTSIM_SESSION_ERROR_WRITE;
	}

	return SOFTSIM_SESSION_SUCCESS;
}

void softsim_session_close(s32 session_id)
{
	softsim_session_delete(session_id);
}

void softsim_session_init(void)
{
	u32 i;

	softsim_memset(&softsim_session_array, 0, sizeof(softsim_session_array));
	for(i = 0;i < SOFTSIM_SESSION_MAX;i++)
	{
		softsim_session_array[i].socket_fd = SOFTSIM_INVALID_SOCKET_FD;
		softsim_session_array[i].timer_id = SOFTSIM_INVAILD_TIMERID;
	}	
}
