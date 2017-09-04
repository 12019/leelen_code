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
 * Filename: softsim_timer.h
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-08-13
 ******************************************************************************/
#ifndef __SOFTSIM_TIMER_H__
#define __SOFTSIM_TIMER_H__

#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************
* Include																	   
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "softsim_type.h"
/*****************************************************************************
* Define																	   
*****************************************************************************/
#define SOFTSIM_TIMER_ID_MAX				100
#define SOFTSIM_INVAILD_TIMERID				(-1)

/*****************************************************************************
* Typedef																	   
*****************************************************************************/
typedef void (*softsim_timer_call_back)(void);
typedef void (*softsim_timer_ex_call_back)(s32 timer_id, void* arg);

typedef union
{
	softsim_timer_call_back callback;
	softsim_timer_ex_call_back ex_callback;
}SoftsimTimerCallBack;

	
typedef struct
{
	bool used;
	s32 timer_id;
	s32 delay;
	void *arg;
	SoftsimTimerCallBack handle;
}SOFTSIM_TIMER_STRUCT;
	
typedef struct
{
	u16 year;
	u8 month;
	u8 day;
	u8 hour;
	u8 minute;
	u8 second;
	u32 usecond;
}softsim_system_time;

#define SOFTSIM_TIMER_STOP(timer_id)			do{\
											if(timer_id != -1){\
											softsim_timer_die(timer_id);\
											timer_id = -1;\
											}}while(0)


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
extern void softsim_timer_init(void);				
extern s32 softsim_timer_die(s32 timerid);
extern s32 softsim_timer_create(softsim_timer_call_back callback, s32 delay);
extern s32 softsim_timer_ex_create(softsim_timer_ex_call_back callback, s32 delay, void *arg, u16 arg_size);
/****************************************************************************
* Function
****************************************************************************/

#ifdef __cplusplus
		}
#endif

#endif /*__SOFTSIM_TIMER_H__*/
