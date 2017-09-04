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
 * Filename: softsim_remote_authentication.h
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-07-14
 ******************************************************************************/
#ifndef __SOFTSIM_CONSOLE_H__
#define __SOFTSIM_CONSOLE_H__

#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************
* Include																	   
*****************************************************************************/
#include "softsim_type.h"

/*****************************************************************************
* Define																	   
*****************************************************************************/
#define TGT_SOFTSIM_SLOT_ID (1)	 
/*****************************************************************************
* Typedef																	   
*****************************************************************************/
typedef enum
{
	SOFTSIM_PROJECT_T2C,
	SOFTSIM_PROJECT_T3,
	SOFTSIM_PROJECT_MAX,
	SOFTSIM_PROJECT_END = 0xff
}SoftsimProjectEnum;

typedef struct
{
	u16 sim_id;
	char ip[SOFTSIM_IP_STRING_LEN];
	u16 port;
}SoftsimAddress;

typedef struct
{
	char ssid[SOFTSIM_SSID_STRING_LEN+1];
	char iccid[SOFTSIM_ICCID_STR_LEN+1];
	char imsi[SOFTSIM_IMSI_STR_LEN+1];
	char plmn_list[SOFTSIM_PLMNLIST_STR_LEN];
	char fplmn_list[SOFTSIM_FPLMNLIST_STR_LEN];
	char mnc_len[SOFTSIM_AD_STR_LEN];
	char mcc[SOFTSIM_MCC_LEN+1];
	bool roaming;
	SoftsimAddress address;
}SoftsimInformation;
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
extern void softsim_init(void);
extern void softsim_exit(void);
extern u8 softsim_get_project_type(void);
extern bool softsim_is_running(void);
extern bool softsim_is_stopping(void);
extern void softsim_status_update(bool status);
extern s32 softsim_channel_status_update(s32 ps_rt, s32 vs_rt);
extern u16 softsim_get_sim_id(void);
extern s32 softsim_data_load(char* ssid, char *iccid, char *imsi, char *plmn_list, char *fplmn_list, char *mnc_len, s32 sim_id, char *ip, s32 port);
extern u8 softsim_get_current_channel(void);
extern s32 softsim_start(char *plmn);
extern s32 softsim_stop(s32 cause_value);
extern SoftsimInformation *softsim_get_infomation(void);
extern void softsim_authentication_init(void);
extern void softsim_authentication_exit(void);
/****************************************************************************
* Function
****************************************************************************/
#ifdef __cplusplus
			}
#endif
	
#endif /*__SOFTSIM_CONSOLE_H__*/
