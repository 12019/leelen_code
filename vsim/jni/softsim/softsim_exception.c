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
 * Filename: softsim_exception.c
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-08-13
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
#define SOFTSIM_MD_ID					SOFTSIM_MD_EXCEPTION
#define SOFTSIM_TRACE_HEADER			"[EXCEPTION]" 	 
/*****************************************************************************
* Typedef																	   
*****************************************************************************/

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
void softsim_stop_handle(s32 cause_value)
{
	char buffer[100] = {0x00};

	switch(cause_value)
	{
		case SOFTSIM_CAUSE_BSS_NOTIFY_RELEASE:
			softsim_strcpy(buffer, "BSS notify release card, then restart flow!");
			break;

		case SOFTSIM_CAUSE_CARD_ABSENT:
			softsim_strcpy(buffer, "Softsim absent, then restart flow!");
			break;

		case SOFTSIM_CAUSE_CARD_NOT_DETECTED:
			softsim_strcpy(buffer, "Softsim could not be detected!");
			break;

		case SOFTSIM_CAUSE_CS_REGISTER_TIMEOUT:
			softsim_strcpy(buffer, "Softsim cs domain register timeout!");
			break;

		case SOFTSIM_CAUSE_PS_REGISTER_TIMEOUT:
			softsim_strcpy(buffer, "Softsim ps domain register timeout!");
			break;

		case SOFTSIM_CAUSE_PS_DETACH:
			softsim_strcpy(buffer, "PS detached, then restart flow!");
			break;

		case SOFTSIM_CAUSE_TIMEOUT:
			softsim_strcpy(buffer, "Flow timeout, then restart flow!");
			break;

		default:
			softsim_strcpy(buffer, "Unknow cause value!");
			break;
	}

	SOFTSIM_DEBUG("%s", buffer);
}

void softsim_exception_report(u16 exception_code)
{
	SOFTSIM_DEBUG("Exception : %d", exception_code);
	softsim_exception_notify(exception_code);
}

void softsim_channel_exception_report(u8 channel)
{
	u16 exception_code = 0xffff;
	char msg[50] = {0};
	
	switch(channel)
	{
		case SOFTSIM_PHYSICAL_SIM_CHANNEL:
			exception_code = SOFTSIM_EXCEPTION_M1_CHNANEL_UNAVAILABLE;
			softsim_strncpy(msg, "M1 ", softsim_strlen("M1 "));
			break;

		case SOFTSIM_VIRTUAL_SIM_CHANNEL:
			exception_code = SOFTSIM_EXCEPTION_M2_CHNANEL_UNAVAILABLE;
			softsim_strncpy(msg, "M2 ", softsim_strlen("M2 "));
			break;

		default:
			break;
	}

	if(channel >= SOFTSIM_CHANNEL_MAX)
	{
		SOFTSIM_DEBUG("Unknow channel type!");
		return;
	}

	softsim_strcat(msg, "channel unavailable!");
	SOFTSIM_DEBUG("%s", msg);
	softsim_exception_report(exception_code);
}

