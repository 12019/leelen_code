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
 * Filename: softsim_message.h
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-08-04
 ******************************************************************************/
#ifndef __SOFTSIM_MESSAGE_H__
#define __SOFTSIM_MESSAGE_H__

#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************
* Include																	   
*****************************************************************************/
#include "softsim_include.h"
#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/time.h>
#include <pthread.h>  
#include <sys/types.h>
#include <sys/ipc.h>
//#include <sys/msg.h>
/*****************************************************************************
* Define																	   
*****************************************************************************/
#define SOFTSIM_MESSAGE_BUFFER_LEN	(512)
/*****************************************************************************
* Typedef																	   
*****************************************************************************/
typedef enum
{
	SOFTSIM_THREAD_NONE,
	SOFTSIM_THREAD_MAIN,
	SOFTSIM_THREAD_TEST,
	SOFTSIM_THREAD_ISO7816,
	SOFTSIM_THREAD_TIMER,
	SOFTSIM_THREAD_MAX,
	SOFTSIM_THREAD_END = 0xff
}SoftsimThreadType;

typedef struct
{
	s32 msg_type;
	char msg_buffer[SOFTSIM_MESSAGE_BUFFER_LEN];
}SoftsimMsg;

typedef struct
{
	s32 tid;
	u32 thread_type;
}SoftsimThreadInfo;

typedef struct _SoftsimThreadQNode
{
	SoftsimThreadInfo info;
	struct _SoftsimThreadQNode *next;
}SoftsimThreadQNode;

typedef struct
{
	s32	mutex;
	SoftsimThreadQNode *front;
	SoftsimThreadQNode *rear;
}SoftsimThreadQueue;

typedef struct
{
	s32 msg_id;
	u32 thread_type;
}SoftsimMessageInfo;

typedef struct _SoftsimMessageQNode
{
	SoftsimMessageInfo info;
	struct _SoftsimMessageQNode *next;
}SoftsimMessageQNode;

typedef struct
{
	s32	mutex;
	SoftsimMessageQNode *front;
	SoftsimMessageQNode *rear;
}SoftsimMessageQueue;

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

#endif /*__SOFTSIM_MESSAGE_H__*/


