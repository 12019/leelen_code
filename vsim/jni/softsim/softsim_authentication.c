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
 * Filename: softsim_authentication.c
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
#include "softsim_include.h"
#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<errno.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<netinet/in.h>  
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

#define SOFTSIM_AUTHENTICATION_CHECK_TIMEOUT	(500)

#if defined(__SOFTSIM_T3__)
#define AUTHENTICATION_TIMEOUT_TIMES_MAX	(4)
#elif defined(__SOFTSIM_T2C__)
#define AUTHENTICATION_TIMEOUT_TIMES_MAX	(8)
#else
#define AUTHENTICATION_TIMEOUT_TIMES_MAX	(8)
#endif
#define AUTHENTICATION_TIMEOUT_COUNT_MAX	(6)
#define SESSION_CONNECT_TIMES_MAX	(5)
#define AUTHENTICATION_PROTOCOL_HEAD_LEN	(8)
/*****************************************************************************
* Typedef																	   
*****************************************************************************/

/*****************************************************************************
* Static Local Variable															  
*****************************************************************************/
static u8 remotesim_auth_response[SOFTSIM_APDU_MAX_LEN] = {0};
static u16 remotesim_auth_response_len = 0;
static u8 authentication_response[] = {0xDB, 0x08, 0x8F, 0xDE, 0xBC, 0xAE, 0x90, 0x6A, 0xDF, 0x46, 
0x10, 0x12, 0xA4, 0x90, 0x9A, 0xFD, 0x8C, 0xC7, 0xF1, 0x22, 0xD2, 0xD0, 0x8A, 0x5B, 0xB5, 0x63, 0x2B, 
0x10, 0xA8, 0x2E, 0x06, 0xFE, 0xCB, 0x2C, 0x60, 0x7A, 0xAD, 0x19, 0x54, 0xC9, 0x52, 0xE5, 0xAF, 0xCD, 
0x08, 0x35, 0x41, 0x12, 0x27, 0x3F, 0xF0, 0x6B, 0x6D, 0x90, 0x00};

static SoftsimSessionContext session;
static volatile bool auth_rsp_flag = false;
static u8 auth_session_connect_times = 0;
static u16 auth_apdu_sequence_number = 0;
static u16 auth_timeout_times = 0;
static SoftsimFuncPtr softsim_auth_session_creat_cb = NULL;
static s32 auth_response_timer_id = -1;
static SoftsimFuncPtr authcallback = NULL;
static u8 timeout_times = 0;
static u8 timeout_count = 0;
static s32 auth_rsp_timeout_exception_timer_id = -1;
static bool auth_status_reset = false;
/*****************************************************************************
* Global Variable														 
*****************************************************************************/

/*****************************************************************************
* Extern Global Variable															
*****************************************************************************/
extern bool Authentication_flag;
/*****************************************************************************
* Static Local Function															  
*****************************************************************************/
static void softsim_auth_session_reset(void);
static void softsim_remote_sim_auth_timeout(void);
static void softsim_auth_session_connected(SoftsimSessionConnected *info);
static void softsim_auth_session_disconnected(SoftsimSessionDisconnected *info);
static void softsim_auth_session_recieve(SoftsimSessionReceive *info);
static void softsim_auth_connect_creat(void);
/*****************************************************************************
* Global Function														
*****************************************************************************/
	 
/*****************************************************************************
* Extern Global Function													  
*****************************************************************************/

/****************************************************************************
* Function
****************************************************************************/
static void softsim_auth_session_reset(void)
{
	auth_rsp_flag = false;
	softsim_memset(remotesim_auth_response, 0x00, sizeof(remotesim_auth_response));
	remotesim_auth_response_len = 0;
	usleep(100*1000);
	Authentication_flag = false;
	auth_apdu_sequence_number = 0;
	auth_session_connect_times = 0;
}

static void softsim_remote_sim_auth_timeout(void)
{	
	SoftsimFuncPtr auth_cb = NULL;
	if(NULL != authcallback)
	{
		auth_cb = authcallback;
		auth_cb();
	}
}

static void softsim_auth_session_connected(SoftsimSessionConnected *info)
{
	SoftsimSessionContext* ss = softsim_get_auth_session();
	
	if(ss->session_id == info->session_id)
	{
		SOFTSIM_DEBUG("Auth session connected channel_id = %d, session_id = %d", info->net_info.channel, info->session_id);
		ss->state = SOFTSIM_SESSION_CONNECTED;
		auth_session_connect_times = 0;
		softsim_socket_status_update(SOFTSIM_SOCKET_STATUS_CONNECTED);
	}
	else
	{
		SOFTSIM_DEBUG("Auth session session_id = %d, connected session_id = %d!", ss->session_id, info->session_id);
	}
}

static void softsim_auth_session_disconnected(SoftsimSessionDisconnected *info)
{
	SoftsimSessionContext* ss = softsim_get_auth_session();
	
	if(ss->session_id == info->session_id)
	{
		SOFTSIM_DEBUG("Auth session disconnected channel_id = %d, session_id = %d, error_code = %d", info->net_info.channel, info->session_id, info->error_code);
		ss->session_id = SOFTSIM_INVALID_SESSION;
		ss->state = SOFTSIM_SESSION_IDLE;
		softsim_socket_status_update(SOFTSIM_SOCKET_STATUS_DISCONNECTED);
	}
	else
	{
		SOFTSIM_DEBUG("Auth session session_id = %d, connected session_id = %d, error_code = %d", ss->session_id, info->session_id, info->error_code);
	}

	if(SOFTSIM_SESSION_SUCCESS != info->error_code)
	{
		if(SOFTSIM_SESSION_ERROR_CONNECT_TIMEOUT == info->error_code 
			|| SOFTSIM_SESSION_ERROR_CONNECT == info->error_code)
		{	
			if(auth_session_connect_times < SESSION_CONNECT_TIMES_MAX)
			{
				softsim_auth_session_recreat();
			}
			else
			{
				//softsim_channel_exception_report(softsim_get_current_channel());
			}
		}
		else if((SOFTSIM_SESSION_ERROR_SERVER_BREAK == info->error_code) && (SOFTSIM_SESSION_IDLE == ss->state))
		{
			#if defined(__SOFTSIM_HEARTBEAT__)
				softsim_auth_session_recreat();
			#endif
		}
	}
}

static void softsim_auth_session_recieve(SoftsimSessionReceive *info)
{
	u16 apdu_length = 0;
	u16 index = 0;
	u16 seq_num = 0;
	u16 slot_id = 0;
	char string_buffer[200] = {0};
	bool pseudo_auth = false;
	SoftsimSessionContext* ss = softsim_get_auth_session();
	
	if(NULL == info)
	{
		return;
	}

	if(0 >= info->data_len)
	{
		return;
	}

	if((0xAA == info->data[0]) || (0xA5 == info->data[0]))
	{
		SOFTSIM_TRACE_HEX("Recieve AUTH-RSP:", info->data, info->data_len);
		auth_timeout_times = 0;		/*timeout times reset*/
		/*Authentication response*/
		if(AUTHENTICATION_PROTOCOL_HEAD_LEN < info->data_len)
		{
			index += 2;
			byte_8_16(seq_num, info->data[index++], info->data[index++]);
			byte_8_16(slot_id, info->data[index++], info->data[index++]);
			byte_8_16(apdu_length, info->data[index++], info->data[index++]);
		}
		else if((AUTHENTICATION_PROTOCOL_HEAD_LEN >= info->data_len) && (4 <= info->data_len))
		{
			index += 2;
			byte_8_16(seq_num, info->data[index++], info->data[index++]);
		}
		else
		{
			;
		}
		
		softsim_rwlock_wrlock();
		//softsim_AuthEvent_notify(AUTH_RECEIVE_RESPONSE, "SS=>VSIM:AUTHENTICATION_RESPONSE", seq_num);
		softsim_memset(string_buffer, 0x00, sizeof(string_buffer));
		softsim_strncpy(string_buffer, "SS=>VSIM:AUTHENTICATION_RESPONSE", softsim_strlen("SS=>VSIM:AUTHENTICATION_RESPONSE"));
		softsim_memset(remotesim_auth_response, 0x00, sizeof(remotesim_auth_response));

		if(apdu_length > 2)
		{
			remotesim_auth_response_len = apdu_length;

			if(remotesim_auth_response_len <= sizeof(remotesim_auth_response))
			{
				softsim_memcpy(remotesim_auth_response, info->data+index, remotesim_auth_response_len);
				if(0xDB == info->data[index])
				{
					softsim_strcat(string_buffer, "  DB:Authentication successful!");
				}
				else if(0xDC == info->data[index])
				{
					softsim_strcat(string_buffer, "  DC:Synchronisation failure!");
				}
				else
				{

				}
			}
			else
			{
				SOFTSIM_DEBUG("Authentication response data length overflow!");
				softsim_strcat(string_buffer, "  Authentication response data length overflow!");
				softsim_AuthEvent_notify(AUTH_EXCEPTION, "SS:Data length overflow,Invalid authentication response!", seq_num);
				softsim_exception_report(SOFTSIM_EXCEPTION_INVALID_AUTHENTICATION_RESPONSE);
				softsim_memset(remotesim_auth_response, 0x00, sizeof(remotesim_auth_response));
				remotesim_auth_response_len = sizeof(authentication_response);
				softsim_memcpy(remotesim_auth_response, authentication_response, remotesim_auth_response_len);
				pseudo_auth = true;
			}
		}
		else if(2 == apdu_length)
		{
			if((0xFF == info->data[index]) && (0x01 == info->data[index+1]))
			{
				SOFTSIM_DEBUG("Auth invalid slot!");
				softsim_strcat(string_buffer, "  Auth invalid slot!");
				softsim_exception_report(SOFTSIM_EXCEPTION_INVALID_SLOT);
				softsim_AuthEvent_notify(AUTH_EXCEPTION,"SS:Invalid slot!",seq_num);
			}
			else if((0xFF == info->data[index]) && (0xFF == info->data[index+1]))
			{
				SOFTSIM_DEBUG("Sim Server Authentication timeout!");
				softsim_strcat(string_buffer, "  Sim Server Authentication timeout!");
				softsim_exception_report(SOFTSIM_EXCEPTION_SIMSERVER_AUTHENTICATION_TIMEOUT);
				softsim_AuthEvent_notify(AUTH_EXCEPTION,"SS:Process authentication timeout!",seq_num);
			}
			else if((0xFF == info->data[index]) && (0xFD == info->data[index+1]))
			{
				SOFTSIM_DEBUG("Sim Server Authentication exception!");
				softsim_strcat(string_buffer, "  Sim Server Authentication exception!");
				softsim_exception_report(SOFTSIM_EXCEPTION_SIMSERVER_NOTIFY_EXCEPTION);
				softsim_AuthEvent_notify(AUTH_EXCEPTION,"SS:Process authentication exception!",seq_num);
			}
			else
			{
				SOFTSIM_DEBUG("Sim Server sim status: 0x%02X", info->data[index+1]);
				softsim_strcat(string_buffer, "  Sim Server sim status!");
				softsim_AuthEvent_notify(AUTH_EXCEPTION,"SS:Sim Server sim status",info->data[index+1]);
			}

			remotesim_auth_response_len = sizeof(authentication_response);
			softsim_memcpy(remotesim_auth_response, authentication_response, remotesim_auth_response_len);
			pseudo_auth = true;
		}
		else
		{
			SOFTSIM_DEBUG("Authentication response invalid!");
			softsim_strcat(string_buffer, "  Authentication response invalid!");
			softsim_AuthEvent_notify(AUTH_EXCEPTION, "SS:Invalid authentication response!", seq_num);
			softsim_exception_report(SOFTSIM_EXCEPTION_INVALID_AUTHENTICATION_RESPONSE);
			softsim_memset(remotesim_auth_response, 0x00, sizeof(remotesim_auth_response));
			remotesim_auth_response_len = sizeof(authentication_response);
			softsim_memcpy(remotesim_auth_response, authentication_response, remotesim_auth_response_len);
			pseudo_auth = true;
		}

		if(seq_num != auth_apdu_sequence_number)
		{
			SOFTSIM_DEBUG("Auth sequence number exception!");
			softsim_AuthEvent_notify(AUTH_EXCEPTION,"Exception!! SQN_REQ(VSIM) != SQN_RSP(SS)", seq_num);
			softsim_memset(remotesim_auth_response, 0x00, sizeof(remotesim_auth_response));
			remotesim_auth_response_len = 0;
		}
		else
		{
			softsim_auth_procedure_processing_stop();
		}
		
		softsim_AuthEvent_notify(AUTH_RECEIVE_RESPONSE, string_buffer, seq_num);
		if(pseudo_auth)
		{
			softsim_AuthEvent_notify(AUTH_RECEIVE_RESPONSE, "Pseudo authentication code!", seq_num);
		}
		
		softsim_rwlock_unlock();
	}
	else
	{
		SOFTSIM_TRACE_HEX("Recieve HB-RSP:", info->data, info->data_len);
		softsim_heartbeat_status_check_stop();
		ss->heartbeat = true;
		softsim_heartbeat_exception_reset();
	}
}

static void softsim_auth_connect_creat(void)
{
	SoftsimSessionConnectRequest connectreq;
	SoftsimSessionCallback callback;
	SoftsimInformation *info = softsim_get_infomation();
	SoftsimSessionContext *ss = softsim_get_auth_session();
	
	softsim_socket_status_update(SOFTSIM_SOCKET_STATUS_UNDEFINED);
	softsim_memset(&connectreq, 0, sizeof(connectreq));
	softsim_memset(&callback, 0, sizeof(callback));
	softsim_strcpy(connectreq.net_info.domain, info->address.ip);
	
	connectreq.net_info.channel = softsim_get_current_channel();
	connectreq.net_info.port = info->address.port;
	
	callback.connected_cb = softsim_auth_session_connected;
	callback.disconnected_cb = softsim_auth_session_disconnected;
	callback.receive_cb = softsim_auth_session_recieve;

	if((0 == connectreq.net_info.port) || (0xffff == connectreq.net_info.port)
		|| (0 == softsim_strlen(connectreq.net_info.domain)))
	{
		SOFTSIM_DEBUG("Invalid IP and Port!");
		return;
	}

	ss->heartbeat = true;
	ss->session_id = softsim_session_connect(&connectreq, &callback);
	ss->channel = softsim_get_current_channel();
	auth_session_connect_times++;

	if(auth_session_connect_times > SESSION_CONNECT_TIMES_MAX)
	{
		SOFTSIM_DEBUG("Session creat times up to MAX!");
		if(softsim_is_running())
		{
			softsim_exception_report(SOFTSIM_EXCEPTION_SESSION_CREAT_FAIL);
		}
		return;
	}
	
	if(ss->session_id >= SOFTSIM_SESSION_SUCCESS)
	{
		ss->state = SOFTSIM_SESSION_CONNECTING;
		softsim_socket_status_update(SOFTSIM_SOCKET_STATUS_CONNECTING);
	}
	else if(ss->session_id == SOFTSIM_SESSION_ERROR_CONNECT)
	{
		softsim_socket_status_update(SOFTSIM_SOCKET_STATUS_ERROR);
		SOFTSIM_DEBUG("Auth session error!! %d", ss->session_id);

		if(softsim_get_current_channel() < SOFTSIM_CHANNEL_MAX)
		{
			softsim_auth_session_recreat();
		}
	}
	else
	{
		softsim_socket_status_update(SOFTSIM_SOCKET_STATUS_ERROR);
		SOFTSIM_DEBUG("Auth session error %d!!", ss->session_id);
	}
}

SoftsimSessionContext *softsim_get_auth_session(void)
{
	return &session;
}

void softsim_auth_session_close(void)
{
	if(SOFTSIM_INVALID_SESSION != session.session_id)
	{
		SOFTSIM_DEBUG("softsim_auth_session_close session.session_id = %d", session.session_id);
		softsim_socket_status_update(SOFTSIM_SOCKET_STATUS_DISCONNECTING);
		softsim_session_close(session.session_id);
		//usleep(500*1000);
		session.session_id = SOFTSIM_INVALID_SESSION;
		session.state = SOFTSIM_SESSION_IDLE;
	}
}

void softsim_auth_timeout_exception_handle(void)
{
	SOFTSIM_DEBUG_MARK;
	SOFTSIM_TIMER_STOP(auth_rsp_timeout_exception_timer_id);
	auth_timeout_times++;
	if(auth_timeout_times > 2)
	{
		if(NULL != softsim_auth_session_creat_cb)
		{
			auth_session_connect_times = 0;
			softsim_auth_session_creat_cb();
			auth_timeout_times = 0;
		}
	}
}

void softsim_auth_exception_handle(void)
{
	SOFTSIM_DEBUG_MARK;
	softsim_rwlock_wrlock();
	softsim_memset(remotesim_auth_response, 0x00, sizeof(remotesim_auth_response));
	remotesim_auth_response_len = sizeof(authentication_response);
	softsim_memcpy(remotesim_auth_response, authentication_response, remotesim_auth_response_len);
	softsim_rwlock_unlock();
	softsim_AuthEvent_notify(AUTH_RECEIVE_RESPONSE, "Pseudo authentication code!", auth_apdu_sequence_number);
}

void softsim_auth_procedure_processing(void)
{
	u8 index = 0;
	u8 buffer[3] = {0x00};

	if(SOFTSIM_SESSION_CONNECTED == session.state)
	{
		if(timeout_times >= AUTHENTICATION_TIMEOUT_TIMES_MAX*2)
		{
			if(auth_rsp_flag)
			{
				SOFTSIM_DEBUG_MARK;
				SOFTSIM_TIMER_STOP(auth_response_timer_id);
				softsim_rwlock_wrlock();
				timeout_times = 0;
				softsim_rwlock_unlock();
				return;
			}
			
			SOFTSIM_DEBUG("Authentication timeout!");
			softsim_AuthEvent_notify(AUTH_EXCEPTION,"VSIM:Authentication timeout!",auth_apdu_sequence_number);
			SOFTSIM_TIMER_STOP(auth_response_timer_id);
			softsim_auth_exception_handle();
			softsim_rwlock_wrlock();
			timeout_times = 0;
			auth_rsp_flag = true;
			softsim_rwlock_unlock();
			softsim_channel_exception_report(softsim_get_current_channel());
			SOFTSIM_TIMER_STOP(auth_rsp_timeout_exception_timer_id);
			auth_rsp_timeout_exception_timer_id = softsim_timer_create(softsim_auth_timeout_exception_handle, 1000);
		}
		else
		{
			if(auth_rsp_flag)
			{
				SOFTSIM_DEBUG_MARK;
				SOFTSIM_TIMER_STOP(auth_response_timer_id);
				timeout_times = 0;
			}
			else
			{
				timeout_times++;
				index = 0;
				buffer[index++] = 0x60;
				softsim_null_byte_send(buffer, index);
			}
		}
	}
	else
	{
		if(timeout_count >= AUTHENTICATION_TIMEOUT_COUNT_MAX*2)
		{
			SOFTSIM_DEBUG_MARK;
			SOFTSIM_TIMER_STOP(auth_response_timer_id);
			softsim_auth_exception_handle();
			softsim_rwlock_wrlock();
			timeout_count = 0;
			auth_rsp_flag = true;
			softsim_rwlock_unlock();
			softsim_AuthEvent_notify(AUTH_EXCEPTION,"VSIM:Session not created successfully!",auth_apdu_sequence_number);
		}
		else
		{
			timeout_count++;
			index = 0;
			buffer[index++] = 0x60;
			softsim_null_byte_send(buffer, index);
		}
	}
}

void softsim_remote_sim_authenticaton(u8 *apdu, u32 apdu_len, u8 *apdu_rsp, u32 *apdu_rsp_len)
{
	u8 send_buff[512] = {0};
	bool send_status = false;
	u16 length = 0;
	u16 auth_apdu_length = 0;
	u16 index = 0;
	u8 times = 0;
	u8 buffer[3] = {0};
	char plmn[SOFTSIM_PLMN_MAX_LEN+1] = {0};
	SoftsimInformation *softsim_info = softsim_get_infomation();
	SoftsimSessionContext *ss = softsim_get_auth_session();
		
	if(NULL == apdu || apdu_len < SOFTSIM_APDU_HEADER_LEN || NULL == apdu_rsp || NULL == apdu_rsp_len)
	{
		SOFTSIM_DEBUG("softsim authentication data error!!!");
		return;
	}

	if(SOFTSIM_VIRTUAL_SIM_CHANNEL == ss->channel)
	{
		softsim_memset(plmn, 0x00, sizeof(plmn));
		softsim_strncpy(plmn, softsim_info->imsi, softsim_atoi(softsim_info->mnc_len));

		//AT&T Cingular Wireless
		if((0 == softsim_strncmp("310410", plmn, softsim_atoi(softsim_info->mnc_len)))
			&& (0 == softsim_strncmp("310", softsim_info->mcc, SOFTSIM_MCC_LEN)))
		{
			SOFTSIM_DEBUG("AT&T network, switch channel!");
			softsim_channel_exception_report(SOFTSIM_EXCEPTION_M2_CHNANEL_UNAVAILABLE);
		}
	}

	auth_apdu_sequence_number++;
	if(0 == auth_apdu_sequence_number)
	{
		SOFTSIM_DEBUG("Auth apdu sequence number overflow!");
		auth_apdu_sequence_number++;
	}
	
	softsim_AuthEvent_notify(AUTH_RECEIVE_REQUEST,"MODEM=>VSIM:AUTHENTICATION_REQUEST",auth_apdu_sequence_number);

	softsim_null_len_reset();

	if(SOFTSIM_SESSION_CONNECTED != session.state)
	{
		SOFTSIM_DEBUG("Session not created successfully!");
		softsim_AuthEvent_notify(AUTH_EXCEPTION,"VSIM:Session not created successfully!",auth_apdu_sequence_number);
		auth_session_connect_times = 0;

		if(NULL != softsim_auth_session_creat_cb)
		{
			softsim_auth_session_creat_cb();
		}
	}

	auth_rsp_flag = false;
	timeout_times = 0;
	timeout_count = 0;
	softsim_rwlock_wrlock();
	softsim_memset(remotesim_auth_response, 0x00, sizeof(remotesim_auth_response));
	softsim_rwlock_unlock();
	remotesim_auth_response_len = 0;
	
	index = 0;
	send_buff[index++] = 0xAA;	/*Header*/
	send_buff[index++] = 0x66;	/*Header*/
	byte_16_8(send_buff[index++], send_buff[index++], auth_apdu_sequence_number);
	byte_16_8(send_buff[index++], send_buff[index++], (u16)softsim_get_sim_id());
	send_buff[index++] = apdu_len;	/*APDU Length*/
	softsim_memcpy(send_buff+index, apdu, apdu_len);
	index += apdu_len;
	authcallback = softsim_auth_procedure_processing;
	SOFTSIM_TIMER_STOP(auth_response_timer_id);
	auth_response_timer_id = softsim_timer_create(softsim_remote_sim_auth_timeout, SOFTSIM_AUTHENTICATION_CHECK_TIMEOUT);

	while(!auth_rsp_flag)
	{
		if(softsim_is_stopping())
		{
			auth_rsp_flag = true;
			break;
		}
		
		if(SOFTSIM_SESSION_CONNECTED != session.state)
		{
			usleep(500*1000);
		}
		else
		{
			if(!send_status)
			{
				SOFTSIM_PRINT_HEX("Send AUTH-REQ:", send_buff, index);
				softsim_channel_info(session.channel);
				if(softsim_session_send(session.session_id, send_buff, index) < 0)
				{
					SOFTSIM_DEBUG("Failed to send authentication data!");
					softsim_AuthEvent_notify(AUTH_EXCEPTION,"VSIM=>SS:Failed to send authentication data!", 0);
				}
				else
				{
					softsim_AuthEvent_notify(AUTH_RECEIVE_REQUEST,"VSIM=>SS:Authentication data is sent successfully!",auth_apdu_sequence_number);
					auth_status_reset = false;
				}
				
				send_status = true;
			}
			else
			{
				usleep(500*1000);
			}
		}
	}
	
	index = 0;

	if(remotesim_auth_response_len > 2)
	{
		apdu_rsp[index++] = 0x61;
		apdu_rsp[index++] = remotesim_auth_response_len - 2;		
		Authentication_flag = true;
	}
	else if(2 == remotesim_auth_response_len)
	{
		SOFTSIM_DEBUG("Authentication error!");
		apdu_rsp[index++] = remotesim_auth_response[0];
		apdu_rsp[index++] = remotesim_auth_response[1];
		softsim_AuthEvent_notify(AUTH_EXCEPTION, "Authentication error!", auth_apdu_sequence_number);
	}
	else
	{
		SOFTSIM_DEBUG("Invalid respose!");
		apdu_rsp[index++] = 0x90;
		apdu_rsp[index++] = 0x00;
	}

	if(auth_status_reset)
	{
		auth_status_reset = false;
		index = 0;
	}
	
	*apdu_rsp_len = index;
}

bool softsim_get_remote_authentication_response(u8 slot, u8 *data, u32 *data_len)
{
	SOFTSIM_DEBUG_MARK;
	softsim_null_len_reset();
	if(slot >= SOFTSIM_SLOT_MAX)
	{
		SOFTSIM_DEBUG("Invalid slot!");
		return false;
	}

	if(NULL == data || NULL == data_len)
	{
		SOFTSIM_DEBUG("Invalid argument!");
		return false;
	}

	while(!auth_rsp_flag)
	{
		if(softsim_is_stopping())
		{
			auth_rsp_flag = true;
			break;
		}
		
		usleep(500*1000);
	}
	
	softsim_rwlock_wrlock();
	
	if(2 > remotesim_auth_response_len)
	{
		SOFTSIM_DEBUG("Authentication response invalid!");
		softsim_exception_report(SOFTSIM_EXCEPTION_INVALID_AUTHENTICATION_RESPONSE);
		softsim_AuthEvent_notify(AUTH_EXCEPTION,"SS:Invalid authentication response!",0);
		softsim_memset(remotesim_auth_response, 0x00, sizeof(remotesim_auth_response));
		remotesim_auth_response_len = sizeof(authentication_response);
		softsim_memcpy(remotesim_auth_response, authentication_response, remotesim_auth_response_len);
	}
	else if(2 == remotesim_auth_response_len)
	{
		if((0xFF == remotesim_auth_response[0]) && (0x01 == remotesim_auth_response[1]))
		{
			SOFTSIM_DEBUG("Auth invalid slot!");
		}
		else if((0xFF == remotesim_auth_response[0]) && (0xFF == remotesim_auth_response[1]))
		{
			SOFTSIM_DEBUG("Sim Server Authentication timeout!");
		}
		else if((0xFF == remotesim_auth_response[0]) && (0xFD == remotesim_auth_response[1]))
		{
			SOFTSIM_DEBUG("Sim Server Authentication exception!");
		}
		else
		{
			SOFTSIM_DEBUG("Sim Server sim status: 0x%02X", remotesim_auth_response[1]);
		}
		
		softsim_memset(remotesim_auth_response, 0x00, sizeof(remotesim_auth_response));
		remotesim_auth_response_len = sizeof(authentication_response);
		softsim_memcpy(remotesim_auth_response, authentication_response, remotesim_auth_response_len);
	}

	data[0] = 0xC0;
	softsim_memcpy(data+1, remotesim_auth_response, remotesim_auth_response_len);
	*data_len = remotesim_auth_response_len+1;

	softsim_memset(remotesim_auth_response, 0x00, sizeof(remotesim_auth_response));
	remotesim_auth_response_len = 0;
	
	softsim_rwlock_unlock();

	if(auth_status_reset)
	{
		auth_status_reset = false;
		*data_len = 0;
	}
	
	return true;
}

void softsim_auth_procedure_processing_stop(void)
{
	SOFTSIM_DEBUG_MARK;
	softsim_rwlock_wrlock();
	if((-1) != auth_response_timer_id)
	{
		SOFTSIM_TIMER_STOP(auth_response_timer_id);
	}

	timeout_times = 0;
	timeout_count = 0;
	auth_rsp_flag = true;
	softsim_rwlock_unlock();
}

void softsim_auth_reset_handle(void)
{
	softsim_auth_procedure_processing_stop();
	auth_apdu_sequence_number = 0;
	auth_timeout_times = 0;
}

void softsim_auth_status_reset(bool status)
{
	auth_status_reset = status;
}

void softsim_auth_session_recreat(void)
{
	SOFTSIM_DEBUG_MARK;
	SoftsimSessionContext *ss = softsim_get_auth_session();
	if(SOFTSIM_SESSION_CONNECTING == ss->state)
	{
		SOFTSIM_DEBUG("Session created in progress!");
		return;
	}
	softsim_auth_session_close();
	session.session_id = SOFTSIM_INVALID_SESSION;
	softsim_auth_connect_creat();
}

void softsim_auth_session_init(void)
{
	auth_timeout_times = 0;
	softsim_auth_session_creat_cb = softsim_auth_session_recreat;
	softsim_auth_session_close();
	session.session_id = SOFTSIM_INVALID_SESSION;
	softsim_auth_connect_creat();
}

void softsim_auth_session_exit(void)
{
	softsim_auth_session_close();
	softsim_auth_session_reset();
}
