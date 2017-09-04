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
 * Filename: softsim_uicc_parameter.h
 *
 * Description:
 *
 * Author:chuan.mei
 * 
 * Date: 2016-07-08
 ******************************************************************************/

#ifndef __SOFTSIM_UICC_PARAMETER_H__
#define __SOFTSIM_UICC_PARAMETER_H__

#define SOFTSIM_FILE_ID_LEN	(2)
#define SOFTSIM_ICCID_LEN	(10)	
#define SOFTSIM_IMSI_LEN	(9)
#define SOFTSIM_KEYS_LEN	(33)
#define SOFTSIM_KEYSPS_LEN	(33)
#define SOFTSIM_PLMN_LEN		(6)
#define SOFTSIM_FPLMN_LEN	(12)	/*size 3n (n>=4)*/
#define SOFTSIM_PLMN_SEL_LEN	(24)	/*size 3n (n>=8)*/
#define SOFTSIM_OPLMNWACT_LEN	(250)	/*size 5n (n>=8)*/
#define SOFTSIM_HPLMNWACT_LEN	(5)	/*size 5n (n>=1)*/
#define SOFTSIM_HPPLMN_LEN	(1)
#define SOFTSIM_SPN_LEN	(17)
#define SOFTSIM_PUCT_LEN	(5)
#define SOFTSIM_LOCI_LEN	(11)
#define SOFTSIM_PSLOCI_LEN	(14)
#define SOFTSIM_SMS_LEN	(176)
#define SOFTSIM_SMSP_NONALPHA_LEN	(28)  /* Excludes the alphabetic part. */
#define SOFTSIM_SMS_STORAGE_LEN	(40)
#define SOFTSIM_MSISDN_LEN	(14)
#define SOFTSIM_ACC_LEN	(2)
#define SOFTSIM_AD_LEN	(4)
#define SOFTSIM_PIN_LEN	(4)
#define SOFTSIM_PUK_LEN	(4)
#define SOFTSIM_ADM_LEN	(4)
#define SOFTSIM_UST_LEN	(13)
#define SOFTSIM_EST_LEN	(13)
#define SOFTSIM_ACL_LEN	(100)

/*3GPP TS 31.102 Annex C 6.3.7 Length of authentication parameters*/
#define SOFTSIM_K_LEN	(16)	/*The authentication key(K) shall have a length of 128 bits*/
#define SOFTSIM_RANDOM_LEN	(16)	/*The random chllenge(RAND) shall have a length of 128 bits*/
#define SOFTSIM_SQN_LEN	(6)	/*Sequence numbers(SQN) shall have a length of 48 bits*/
#define SOFTSIM_AK_LEN	(6)	/*The anonymity key(AK) shall have a length of 48 bits*/
#define SOFTSIM_AMF_LEN	(2)	/*The authentication management filed(AMF) shall have a length of 16 bits*/
#define SOFTSIM_MAC_LEN	(8)	/*The message authentication codes MAC in AUTN and MAC-S in AUTS shall have a length of 64 bits*/
#define SOFTSIM_CK_LEN	(16)	/*The cipher key(CK) shall have a length of 128 bits*/
#define SOFTSIM_IK_LEN	(16)	/*The integrity key(IK) shall have a length of 128 bits*/
#define SOFTSIM_RES_LEN	(16)	/*The authentication response(RES) shall have a variable length of 4-16 octets*/
#define SOFTSIM_XRES_LEN (16)
#define SOFTSIM_KC_LEN	(8)
#define SOFTSIM_KC_FILE_LEN	(9)
#define SOFTSIM_KCGPRS_LEN	(8)
#define SOFTSIM_KCGPRS_FILE_LEN	(9)
#define SOFTSIM_OP_LEN	(16)
#define SOFTSIM_OPC_LEN	(16)
#define SOFTSIM_AUTN_LEN	(16)	
#define SOFTSIM_AUTS_LEN	(16)	
#define SOFTSIM_SQN_LIST_LEN	(32)
#define SOFTSIM_APDU_HEADER_LEN	(4)
#define SOFTSIM_TPDU_HEADER_LEN	(5)
#define SOFTSIM_APDU_MAX_LEN	(261)

#define SOFTSIM_RES_SPC_LEN	(8)
#define SOFTSIM_SRES_SPC_LEN	(4)
#define SOFTSIM_XRES_SPC_LEN	(12)

#define SOFTSIM_SQN_DIFFERENCE_MAX	(0x0FFFFFFFFF)
#define SOFTSIM_SQN_DIFFERENCE_MIN	(32)

#define SOFTSIM_ICCID_STR_LEN	(20)
#define SOFTSIM_IMSI_STR_LEN	(15)
#define SOFTSIM_K_STR_LEN	(32)
#define SOFTSIM_OPC_STR_LEN	(32)
#define SOFTSIM_SMSADDR_STR_LEN	(30)
#define SOFTSIM_MSISDN_STR_LEN	(28)
#define SOFTSIM_PLMNLIST_STR_LEN	(80)
#define SOFTSIM_FPLMNLIST_STR_LEN	(80)
#define SOFTSIM_AD_STR_LEN	(80)
#define SOFTSIM_APN_STR_LEN	(100)
#define SOFTSIM_SPAN_STR_LEN	(100)

#define SOFTSIM_PLMN_MAX_LEN	(6)
#define SOFTSIM_MCC_LEN	(3)
#define SOFTSIM_MNC_MAX_LEN	(3)

#define SOFTSIM_USIM_FILE_PATH_LEN	(32)
#endif /*__SOFTSIM_UICC_PARAMETER_H__*/

