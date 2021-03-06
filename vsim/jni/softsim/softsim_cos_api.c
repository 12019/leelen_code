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
 * Filename: softsim_cos_api.c
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
#include "softsim_trace.h"
/*****************************************************************************
* Define                                                                      
*****************************************************************************/
#undef 	SOFTSIM_MD_ID
#undef 	SOFTSIM_TRACE_HEADER
#define SOFTSIM_MD_ID					SOFTSIM_MD_API
#define SOFTSIM_TRACE_HEADER			"[API]"
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
s32 softsim_cos_init(void)
{
	softsim_init();
	return 0;
}

s32 softsim_cos_start(char *plmn)
{
	return softsim_start(plmn);
}

s32 softsim_cos_stop(s32 cause_value)
{
	return softsim_stop(cause_value);
}

s32 softsim_cos_channel_status_update(s32 ps_rt, s32 vs_rt)
{
	return softsim_channel_status_update(ps_rt, vs_rt);
}

s32 softsim_cos_command(u8 slot, u8 *rx_data, u32 rx_size, u8 *tx_data, u32 *tx_size)
{
	return softsim_tpdu_command(slot, rx_data, rx_size, tx_data, tx_size);
}

s32 softsim_cos_data_load(char *ssid, char *iccid, char *imsi, char *plmn_list, char *fplmn_list, char *mnc_len, s32 sim_id, char *ip, s32 port)
{
	return softsim_data_load(ssid,iccid,imsi,plmn_list,fplmn_list,mnc_len,sim_id,ip,port);
}
