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
 * Filename: softsim_session.h
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-08-10
 ******************************************************************************/
#ifndef __SOFTSIM_SESSION_H__
#define __SOFTSIM_SESSION_H__

#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************
* Include																	   
*****************************************************************************/
#include "softsim_include.h"
/*****************************************************************************
* Define																	   
*****************************************************************************/
#define SOFTSIM_INVALID_SESSION 			(-1)
/*****************************************************************************
* Typedef																	   
*****************************************************************************/
typedef enum
{
	SOFTSIM_SESSION_SUCCESS = 0,
	SOFTSIM_SESSION_ERROR_PARAM = -1,
	SOFTSIM_SESSION_ERROR_LIMITED_RESOURCE = -2,
	SOFTSIM_SESSION_ERROR_MEMORY = -3,
	SOFTSIM_SESSION_ERROR_SOCKET_CREATE = -4,
	SOFTSIM_SESSION_ERROR_CONNECT = -5,	
	SOFTSIM_SESSION_ERROR_CONNECT_TIMEOUT = -6,
	SOFTSIM_SESSION_ERROR_WRITE = -7,
	SOFTSIM_SESSION_ERROR_SERVER_BREAK = -8,
	SOFTSIM_SESSION_ERROR_INVALID_HANDLE = -9,
	SOFTSIM_SESSION_ERROR_NOT_CONNECTED = -10,
	SOFTSIM_SESSION_ERROR_GETHOSTBYNAME = -11
}SoftsimSessionErrorCode;

typedef enum
{
	SOFTSIM_SESSION_IDLE,
	SOFTSIM_SESSION_CONNECTING,
	SOFTSIM_SESSION_CONNECTED
}SoftsimSessionStateEnum;

typedef struct
{
	SoftsimChannelType channel; 
	char domain[SOFTSIM_DOMAIN_LEN];
	u16 port;
	SoftsimAddressType	address_type;
	s32 keepalive;	/*0: keepalive eable; 1: keepalive disbale*/
	s32 keepidle;
	s32 keepinterval;
	s32 keepcount;
}SoftsimSessionNetInfo;

typedef struct
{
	SoftsimSessionNetInfo net_info;
	u32 timeout_secends;
}SoftsimSessionConnectRequest;

typedef struct
{
	s32 session_id;
	SoftsimSessionNetInfo net_info;
}SoftsimSessionConnected;

typedef struct
{
	s32 session_id;
	s32 error_code;
	SoftsimSessionNetInfo net_info;
}SoftsimSessionDisconnected;

typedef struct
{
	s32 session_id;
	SoftsimSessionNetInfo net_info;
	u16 data_len;
	u8 *data;
}SoftsimSessionReceive;

typedef void (* SoftsimSessionConnectedFun)(SoftsimSessionConnected *info);
typedef void (* SoftsimSessionDisconnectedFun)(SoftsimSessionDisconnected *info);
typedef void (* SoftsimSessionReceiveFun)(SoftsimSessionReceive *info);

typedef struct
{
	SoftsimSessionConnectedFun connected_cb;
	SoftsimSessionDisconnectedFun disconnected_cb;
	SoftsimSessionReceiveFun receive_cb;
}SoftsimSessionCallback;

typedef struct
{
	s32 session_id;
	u8 channel;
	u32 state;
	bool heartbeat;
}SoftsimSessionContext;

typedef struct
{
	s32 socket_fd;
	SoftsimSessionStateEnum state;
	SoftsimSessionNetInfo net_info;
	SoftsimSessionCallback ss_cb;
	u32 timeout_secends;
	s32 timer_id;
	u16 data_len;
	u8 *data;
}SoftsimSessionStruct;
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
extern s32 softsim_session_connect(SoftsimSessionConnectRequest *info, SoftsimSessionCallback *ss_cb);
extern s32 softsim_session_send(s32 session_id, u8 *data, u16 data_len);
extern void softsim_session_close(s32 session_id);
extern void softsim_session_init(void);
/****************************************************************************
* Function
****************************************************************************/
#ifdef __cplusplus
		}
#endif

#endif /*__SOFTSIM_SESSION_H__*/
