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
 * Filename: softsim_monitor.h
 *
 * Description:
 *
 * Author: tongtong.sun
 * 
 * Date: 2016-08-03
 ******************************************************************************/

#ifndef __SOFTSIM_MONITOR_H__
#define __SOFTSIM_MONITOR_H__

#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************
* Include																	   
*****************************************************************************/
#include "softsim_macro.h"
#if defined(__SOFTSIM_ANDROID__)
#include <jni.h>
#include <semaphore.h>
/*****************************************************************************
* Define																	   
*****************************************************************************/
	 
/*****************************************************************************
* Typedef																	   
*****************************************************************************/

/*****************************************************************************
* Static Local Variable															  
*****************************************************************************/
	 
/*****************************************************************************
* Global Variable														 
*****************************************************************************/
#define AUTH_INFORMATION_MAX_LEN  64

typedef enum
{
	SOFTSIM_CALLBACK_SCOKET_STATUS,
	SOFTSIM_CALLBACK_EXCEPTION,
	SOFTSIM_CALLBACK_AUTH_EVENT
}SoftsimCallbackType;

typedef enum
{
	SOFTSIM_SOCKET_STATUS_UNDEFINED,
	SOFTSIM_SOCKET_STATUS_DISCONNECTED,
	SOFTSIM_SOCKET_STATUS_CONNECTING,
	SOFTSIM_SOCKET_STATUS_CONNECTED,
	SOFTSIM_SOCKET_STATUS_DISCONNECTING,
	SOFTSIM_SOCKET_STATUS_ERROR = 0xFFFFFFFF	
}SoftsimSocketStatus;

typedef enum
{
	SOFTSIM_EXCEPTION_HEART,
}SoftsimException;

typedef enum
{
	AUTH_RECEIVE_REQUEST, 
	AUTH_RECEIVE_RESPONSE,
	AUTH_EXCEPTION = 0xFFFFFFFF
}SoftsimAuthStatus;


typedef struct 
{

	int timestamp;
	SoftsimAuthStatus auth_status;
	char auth_information[AUTH_INFORMATION_MAX_LEN];
	int auth_sequence_num;
}SoftsimAuthEvent;

typedef struct node
{
	SoftsimCallbackType callbacktype;
	SoftsimSocketStatus socket_status;
	SoftsimException exception;
	SoftsimAuthEvent auth_event;
	struct node *next;
}SoftsimCallbackNode;

typedef struct queue
{
	SoftsimCallbackNode *head;
	SoftsimCallbackNode *tail;
}SoftsimCallbackQueue;


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
extern int socket_status_monitor_Start(JNIEnv *env,jobject object);
extern void softsim_socket_heart_exception(void);
extern void softsim_socket_status_update(SoftsimSocketStatus socket_status);
extern void softsim_exception_notify(u16 exception_code);
extern void softsim_AuthEvent_notify(SoftsimAuthStatus auth_status,const char *auth_information, int auth_sequence_num);

/****************************************************************************
* Function
****************************************************************************/
#endif	/*__SOFTSIM_ANDROID__*/

#ifdef __cplusplus
		}
#endif

#endif /*__SOFTSIM_MONITOR_H__*/
