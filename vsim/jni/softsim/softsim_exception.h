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
 * Filename: softsim_exception.h
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-08-13
 ******************************************************************************/
#ifndef __SOFTSIM_EXCEPTION_H__
#define __SOFTSIM_EXCEPTION_H__

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
	 
/*****************************************************************************
* Typedef																	   
*****************************************************************************/
typedef enum
{
	SOFTSIM_CAUSE_BSS_NOTIFY_RELEASE,
	SOFTSIM_CAUSE_CARD_ABSENT,
	SOFTSIM_CAUSE_CARD_NOT_DETECTED,
	SOFTSIM_CAUSE_CS_REGISTER_TIMEOUT,
	SOFTSIM_CAUSE_PS_REGISTER_TIMEOUT,
	SOFTSIM_CAUSE_PS_DETACH,
	SOFTSIM_CAUSE_TIMEOUT,
	SOFTSIM_CAUSE_MAX,
	SOFTSIM_CAUSE_END = 0x1fffffff
}SoftsimStopCauseValue;

typedef enum
{
	SOFTSIM_SUCCESS = 0,
	SOFTSIM_EXCEPTION_NONE = 500,
	SOFTSIM_EXCEPTION_LOAD_USIM_TEMPLATE_FILE_FAIL = 501,
	SOFTSIM_EXCEPTION_INVALID_USIM_TEMPLATE_FILE = 502,
	SOFTSIM_EXCEPTION_INVALID_ARGUMENT = 503,
	SOFTSIM_EXCEPTION_VSIM_DATA_NOT_LOAD = 504,
	SOFTSIM_EXCEPTION_IS_RUNNING = 505,
	SOFTSIM_EXCEPTION_INVALID_PLMN = 506,
	SOFTSIM_EXCEPTION_INVALID_MNC_LENGTH = 507,
	SOFTSIM_EXCEPTION_INVALID_IMSI = 508,
	SOFTSIM_EXCEPTION_INVALID_ICCID = 509,
	SOFTSIM_EXCEPTION_INVALID_IP = 510,
	SOFTSIM_EXCEPTION_INVALID_PORT = 511,
	SOFTSIM_EXCEPTION_INVALID_APDU = 512,
	SOFTSIM_EXCEPTION_CONNECT_SIMSERVER_REFUSED = 513,
	SOFTSIM_EXCEPTION_SIMSERVER_AUTHENTICATION_TIMEOUT = 514,
	SOFTSIM_EXCEPTION_INVALID_AUTHENTICATION_RESPONSE = 515,
	SOFTSIM_EXCEPTION_SIMSERVER_NOTIFY_EXCEPTION = 516,
	SOFTSIM_EXCEPTION_INVALID_SLOT = 517,
	SOFTSIM_EXCEPTION_M1_CHNANEL_UNAVAILABLE = 518,
	SOFTSIM_EXCEPTION_M2_CHNANEL_UNAVAILABLE = 519,
	SOFTSIM_EXCEPTION_IS_STOPPING = 520,
	SOFTSIM_EXCEPTION_SESSION_CREAT_FAIL = 521,
	SOFTSIM_EXCEPTION_SIM_SERVER_INVALID_DATA_FORMAT = 522,
	SOFTSIM_EXCEPTION_VSIM_INVALID_DATA_FORMAT = 523,
	SOFTSIM_EXCEPTION_SIM_AUTHENTICATION_ERROR = 524,
	SOFTSIM_EXCEPTION_SIM_SERVER_NOTIFY_NO_USIM = 525,
 	/*Customer add exception id above this*/
	SOFTSIM_EXCEPTION_ID_MAX,
	SOFTSIM_EXCEPTION_END = 0xffff
}SoftsimExceptionCode;
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
extern void softsim_stop_handle(s32 cause_value);
/****************************************************************************
* Function
****************************************************************************/

#ifdef __cplusplus
		}
#endif

#endif /*__SOFTSIM_EXCEPTION_H__*/
