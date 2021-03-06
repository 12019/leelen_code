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
 * Filename: softsim_cos_api.h
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-07-08
 ******************************************************************************/

#ifndef __SOFTSIM_COS_API_H__
#define __SOFTSIM_COS_API_H__

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
* Static Local Function                                                              
*****************************************************************************/

/*****************************************************************************
* Global Function                                                      
*****************************************************************************/

/*****************************************************************************
* Extern Global Function                                                     
*****************************************************************************/
/*softsim cos init*/
extern s32 softsim_cos_init(void);

/*softsim cos start*/
extern s32 softsim_cos_start(char *plmn);

/*softsim cos stop*/
extern s32 softsim_cos_stop(s32 cause_value);

/*
softsim apdu data interface
slot: slot id
rx_data: apdu request data
rx_size: adpu request data length
tx_data: apdu response data
tx_size: apdu response data length
*/
extern s32 softsim_cos_command(u8 slot, u8 *rx_data, u32 rx_size, u8 *tx_data, u32 *tx_size);
extern s32 softsim_cos_channel_status_update(s32 ps_rt, s32 vs_rt);
extern s32 softsim_cos_data_load(char *ssid, char *iccid, char *imsi, char *plmn_list, char *fplmn_list, char *mnc_len, s32 sim_id, char *ip, s32 port);
/****************************************************************************
 * Function
 ****************************************************************************/
#ifdef __cplusplus
		}
#endif

#endif /*__SOFTSIM_COS_API_H__*/


