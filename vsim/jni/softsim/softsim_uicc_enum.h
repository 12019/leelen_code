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
 * Filename: softsim_uicc_enum.h
 *
 * Description:
 *
 * Author:chuan.mei
 * 
 * Date: 2016-07-08
 ******************************************************************************/
#ifndef __SOFTSIM_UICC_ENUM_H__
#define __SOFTSIM_UICC_ENUM_H__
	 
#ifdef __cplusplus
	 extern "C"{
#endif

typedef enum
{
	SOFTSIM_MF_ID = 0x3F00,
	SOFTSIM_DF_GSM_ID = 0x7F20,
	SOFTSIM_DF_TELECOM_ID = 0x7F10,
	SOFTSIM_ADF_USIM_ID = 0x7FFF,
	
	/* EFs at the MF level */
  	SOFTSIM_DIR_ID = 0x2F00,                    /* EF DIR for the USIM */
  	SOFTSIM_PL_ID = 0x2F05,                    /* Extended Language Preference */
  	SOFTSIM_ARR_ID = 0x2F06,                    /* Access rule reference */
	SOFTSIM_ICCID_ID = 0x2FE2,					/* ICCID */

	SOFTSIM_MF_FILE_ID_MAX,
	SOFTSIM_MF_FILE_ID_END = 0xFF00
}SoftsimMFFileEnum;

typedef enum
{
	/* EFs in DF GSM */
	SOFTSIM_GSM_LP_ID = 0x6F05, 			 /* Language Preference */
	SOFTSIM_GSM_IMSI_ID = 0x6F07,			 /* IMSI */
	SOFTSIM_GSM_KC_ID = 0x6F20, 			 /* Ciphering Key Kc */
	SOFTSIM_GSM_PLMNSEL_ID = 0x6F30,		 /* PLMN selector */
	SOFTSIM_GSM_HPPLMN_ID = 0x6F31, 		 /* HPLMN search period */
	SOFTSIM_GSM_ACMMAX_ID = 0x6F37, 		/* ACM Maximum value */
	SOFTSIM_GSM_SST_ID = 0x6F38,			 /* SIM Service table */
	SOFTSIM_GSM_ACM_ID = 0x6F39,			 /* Accumulated call meter */
	SOFTSIM_GSM_GID1_ID = 0x6F3E,			 /* Group Identifier Level 1 */
	SOFTSIM_GSM_GID2_ID = 0x6F3F,			 /* Group Identifier Level 2 */
	SOFTSIM_GSM_PUCT_ID = 0x6F41,			 /* Price Per Unit and currency table */
	SOFTSIM_GSM_CBMI_ID = 0x6F45,			 /* Cell broadcast message identifier selection */
	SOFTSIM_GSM_SPN_ID = 0x6F46,			 /* Service Provider Name */
	SOFTSIM_GSM_CBMID_ID = 0x6F48,			 /* Cell Broadcast Message id for data dload */
	SOFTSIM_GSM_BCCH_ID = 0x6F74,			 /* Broadcast control channels */
	SOFTSIM_GSM_ACC_ID = 0x6F78,			 /* Access control class */
	SOFTSIM_GSM_FPLMN_ID = 0x6F7B,			 /* Forbidden PLMNs */
	SOFTSIM_GSM_LOCI_ID = 0x6F7E,			 /* Location information */
	SOFTSIM_GSM_AD_ID = 0x6FAD, 			 /* Administrative data */
	SOFTSIM_GSM_PHASE_ID = 0x6FAE,			 /* Phase identification */
	SOFTSIM_GSM_VGCS_ID = 0x6FB1,			 /* Voice Group Call service */
	SOFTSIM_GSM_VGCSS_ID = 0x6FB2,			 /* Voice Group Call service status */
	SOFTSIM_GSM_VBS_ID = 0x6FB3,			 /* Voice Broadcast service  */
	SOFTSIM_GSM_VBSS_ID = 0x6FB4,			 /* Voice Broadcast service status */
	SOFTSIM_GSM_EMLPP_ID = 0x6FB5,			 /* Enhanced multi level pre-emption and priority */
	SOFTSIM_GSM_AAEM_ID = 0x6FB6,			 /* Automatic Answer for eMLPP service */
	SOFTSIM_GSM_ECC_ID = 0x6FB7,			 /* Emergency Call Codes */
	SOFTSIM_GSM_CBMIR_ID = 0x6F50,			 /* Cell Broadcast Message id range selection */
	SOFTSIM_GSM_NIA_ID = 0x6F51,			 /* Network's indication of alerting */
	SOFTSIM_GSM_KCGPRS_ID = 0x6F52, 		 /* GPRS ciphering key */
	SOFTSIM_GSM_LOCIGPRS_ID = 0x6F53,		 /* GPRS location information */
	SOFTSIM_GSM_SUME_ID = 0x6F54,			 /* Setup Menu elements */
	SOFTSIM_GSM_PLMNWACT_ID = 0x6F60,		 /* PLMN Selector with Access technology */
	SOFTSIM_GSM_OPLMNWACT_ID = 0x6F61,		 /* Operator controlled PLMNWACT */
	SOFTSIM_GSM_HPLMNWACT_ID = 0x6F62,		 /* HPLNMN Access technology */
	SOFTSIM_GSM_CPBCCH_ID = 0x6F63, 		 /* CPBCCH information */
	SOFTSIM_GSM_INVSCAN_ID = 0x6F64,		 /* Investigation PLMN Scan */
	/* EFs at SoLSA */
	SOFTSIM_GSM_SAI_ID = 0x4F30,			 /* SoLSA access indicator */
	SOFTSIM_GSM_SLL_ID = 0x4F31,			 /* SoLSA LSA list */
	
	/* EFs at MExE Level */
	SOFTSIM_GSM_MEXE_ST_ID = 0x4F40,		 /* MExE Service table */
	SOFTSIM_GSM_ORPK_ID = 0x4F41,			 /* Operator Root Public Key */
	SOFTSIM_GSM_ARPK_ID = 0x4F42,			 /* Administrator Root Public Key */
	SOFTSIM_GSM_TPRPK_ID = 0x4F43,			 /* Third party Root public key */

	SOFTSIM_GSM_FILE_ID_MAX,
	SOFTSIM_GSM_FILE_ID_END = 0xFF00
}SoftsimDFGSMFileEnum;

typedef enum
{
	/* EFs at the telecom DF in GSM and CDMA */
  	SOFTSIM_TELECOM_ARR_ID = 0x6F06,         /* Access Rule reference */
	SOFTSIM_TELECOM_ADN_ID = 0x6F3A,		/* Abbrev Dialing Numbers */
  	SOFTSIM_TELECOM_FDN_ID = 0x6F3B,         /* Fixed dialling numbers */
  	SOFTSIM_TELECOM_SMS_ID = 0x6F3C,         /* Short messages */
  	SOFTSIM_TELECOM_MSISDN_ID = 0x6F40,      /* MSISDN */
  	SOFTSIM_TELECOM_SMSP_ID = 0x6F42,        /* SMS parameters */
  	SOFTSIM_TELECOM_SMSS_ID = 0x6F43,        /* SMS Status */
  	SOFTSIM_TELECOM_LND_ID = 0x6F44,         /* Last number dialled */
  	SOFTSIM_TELECOM_SMSR_ID = 0x6F47,        /* SMS reports */
  	SOFTSIM_TELECOM_SDN_ID = 0x6F49,         /* Service Dialling numbers */
  	SOFTSIM_TELECOM_EXT1_ID = 0x6F4A,        /* Extension 1 */
  	SOFTSIM_TELECOM_EXT2_ID = 0x6F4B,        /* Extension 2 */
  	SOFTSIM_TELECOM_EXT3_ID = 0x6F4C,        /* Extension 3 */
  	SOFTSIM_TELECOM_BDN_ID = 0x6F4D,         /* Barred Dialing Numbers */
  	SOFTSIM_TELECOM_EXT4_ID = 0x6F4E,        /* Extension 4 */
  	SOFTSIM_TELECOM_ECCP_ID = 0x6F4F,        /* Extended CCP */
  	SOFTSIM_TELECOM_CCP_ID = 0x6F3A,         /* Capability Configuration Parameters */
	SOFTSIM_TELECOM_RMA_ID = 0x6F53,		 
	SOFTSIM_TELECOM_SUME_ID = 0x6F54,		 /* Setup Menu elements */
	SOFTSIM_TELECOM_ICE_DN_ID = 0x6FE0,		 
	SOFTSIM_TELECOM_ICE_FF_ID = 0x6FE1,		 
	SOFTSIM_TELECOM_PSISMSC_ID = 0x6FE5,		 

	
	SOFTSIM_TELECOM_GRAPHICS_ID = 0x5F50,		 
  	SOFTSIM_IMAGE_ID = 0x4F20,    

	SOFTSIM_TELECOM_PHONEBOOK_ID = 0x5F3A,

  	/* EFs at the DF PHONEBOOK under  DF Telecom */
  	SOFTSIM_TELECOM_PSC_ID = 0x4F22,         /* Phone book synchronization center */
  	SOFTSIM_TELECOM_CC_ID = 0x4F23,          /* Change counter */
  	SOFTSIM_TELECOM_PUID_ID = 0x4F24,        /* Previous Unique Identifier */
  	SOFTSIM_TELECOM_PBR_ID = 0x4F30,         /* Phone book reference file */
  	
  	SOFTSIM_TELECOM_XX1_ID = 0x4F09,
	SOFTSIM_TELECOM_XX2_ID = 0x4F11,
	SOFTSIM_TELECOM_XX3_ID = 0x4F19,
	SOFTSIM_TELECOM_XX4_ID = 0x4F25,
	SOFTSIM_TELECOM_XX5_ID = 0x4F26,
	SOFTSIM_TELECOM_XX6_ID = 0x4F3A,
  	SOFTSIM_TELECOM_XX7_ID = 0x4F3D,
	SOFTSIM_TELECOM_XX8_ID = 0x4F4A,
	SOFTSIM_TELECOM_XX9_ID = 0x4F4B,
	SOFTSIM_TELECOM_XXA_ID = 0x4F4C,
	SOFTSIM_TELECOM_XXB_ID = 0x4F50,
	  	
	SOFTSIM_TELECOM_FILE_ID_MAX,
	SOFTSIM_TELECOM_FILE_ID_END = 0xFF00

}SoftsimDFTELECOMFileEnum;

typedef enum
{	
	/* EFs of USIM ADF */															   
	SOFTSIM_USIM_LI_ID = 0x6F05,			  /* Language Indication */
  	SOFTSIM_USIM_ARR_ID = 0x6F06,			/* Access Rule reference */
	SOFTSIM_USIM_IMSI_ID = 0x6F07, 		  /* IMSI */											   
	SOFTSIM_USIM_KEYS_ID = 0x6F08, 		  /* Ciphering and Integrity keys */					   
	SOFTSIM_USIM_KEYSPS_ID = 0x6F09,		  /* C and I keys for packet switched domain */ 		   
  	SOFTSIM_USIM_DCK_ID = 0x6F2C,            /* De-personalization Control Keys */
	SOFTSIM_USIM_HPPLMN_ID = 0x6F31,		  /* Higher Priority PLMN search period */ 							   
  	SOFTSIM_USIM_CNL_ID = 0x6F32,            /* Co-operative network list */
	SOFTSIM_USIM_ACMMAX_ID = 0x6F37,		  /* ACM maximum value */								   
	SOFTSIM_USIM_UST_ID = 0x6F38,			  /* USIM Service table */	
  	SOFTSIM_USIM_ACM_ID = 0x6F39,            /* Accumulated Call meter */
  	SOFTSIM_USIM_FDN_ID = 0x6F3B,            /* Fixed dialling numbers */
  	SOFTSIM_USIM_SMS_ID = 0x6F3C,            /* Short messages */
  	SOFTSIM_USIM_GID1_ID = 0x6F3E,           /* Group Identifier Level  */
  	SOFTSIM_USIM_GID2_ID = 0x6F3F,           /* Group Identifier Level 2 */
	SOFTSIM_USIM_MSISDN_ID = 0x6F40,         /* MSISDN */
  	SOFTSIM_USIM_PUCT_ID = 0x6F41,           /* Price Per Unit and Currency table */
  	SOFTSIM_USIM_SMSP_ID = 0x6F42,           /* SMS parameters */
  	SOFTSIM_USIM_SMSS_ID = 0x6F43,           /* SMS Status */
  	SOFTSIM_USIM_CBMI_ID = 0x6F45,           /* Cell Broadcast Message identifier selection */
  	SOFTSIM_USIM_SPN_ID = 0x6F46,            /* Service Provider Name */
  	SOFTSIM_USIM_SMSR_ID = 0x6F47,			/* Short Message status reports */
  	SOFTSIM_USIM_CBMID_ID = 0x6F48,          /* Cell Broadcast msg identifier for data download */
  	SOFTSIM_USIM_SDN_ID = 0x6F49,            /* Service dialling numbers */
  	SOFTSIM_USIM_EXT2_ID = 0x6F4B,			/* Extension 2 */
  	SOFTSIM_USIM_EXT3_ID = 0x6F4C,			/* Extension 3 */
  	SOFTSIM_USIM_BDN_ID = 0x6F4D,            /* Barred dialling numbers */
	SOFTSIM_USIM_EXT5_ID = 0x6F4E,			/* Extension 5 */
  	SOFTSIM_USIM_CCP2_ID = 0x6F4F,			/* Capability Configuration Parameters 2 */
  	SOFTSIM_USIM_CBMIR_ID = 0x6F50,          /* Cell broadcast msg identifier range selection */
  	SOFTSIM_USIM_EXT4_ID = 0x6F55,           /* Extension 4 */
	SOFTSIM_USIM_EST_ID = 0x6F56,            /* Enabled services table */
  	SOFTSIM_USIM_ACL_ID = 0x6F57,            /* Access Point Name Control List */
  	SOFTSIM_USIM_CMI_ID = 0x6F58,            /* Comparison Method information */
  	SOFTSIM_USIM_START_HFN_ID = 0x6F5B,		/* Init values for Hyper-frame number */
  	SOFTSIM_USIM_THRESHOLD_ID = 0x6F5C,		/* Max value of START */
	SOFTSIM_USIM_PLMNWACT_ID = 0x6F60,	  /* User controlled PLMN selector with access tech */
	SOFTSIM_USIM_OPLMNWACT_ID = 0x6F61,		/* Operator controlled PLMN sel with access tech */
  	SOFTSIM_USIM_HPLMNWACT_ID = 0x6F62,		/* HPLMN selector with access tech */
  	SOFTSIM_USIM_PSLOCI_ID = 0x6F73,         /* Packet switched location information */
  	SOFTSIM_USIM_ACC_ID = 0x6F78,            /* Access control class */
  	SOFTSIM_USIM_FPLMN_ID = 0x6F7B,          /* Forbidden PLMNs */
  	SOFTSIM_USIM_LOCI_ID = 0x6F7E,           /* Location information */
  	SOFTSIM_USIM_ICI_ID = 0x6F80,			/* Incoming call information */
  	SOFTSIM_USIM_OCI_ID = 0x6F81,			/* Outgoing call information */
  	SOFTSIM_USIM_ICT_ID = 0x6F82,			/* Incoming call timer */
  	SOFTSIM_USIM_OCT_ID = 0x6F83,			/* Outgoing call timer */
  	SOFTSIM_USIM_AD_ID = 0x6FAD,             /* Administrative data */
  	SOFTSIM_USIM_VGCS_ID = 0x6FB1,		/*Voice Group Call Service*/
	SOFTSIM_USIM_VGCSS_ID = 0x6FB2,		/*Voice Group Call Service Status*/
	SOFTSIM_USIM_VBS_ID = 0x6FB3,		/*Voice Broadcast Service*/
	SOFTSIM_USIM_VBSS_ID = 0x6FB4,		/*Voice Broadcast Service Status*/
  	SOFTSIM_USIM_EMLPP_ID = 0x6FB5,		/* Enhanced Multi Level Precedence and pre-emption */
  	SOFTSIM_USIM_AAEM_ID = 0x6FB6,           /* Automatic answer for eMLPP service */
  	SOFTSIM_USIM_ECC_ID = 0x6FB7,            /* Emergency call codes */
  	SOFTSIM_USIM_GMSI_ID = 0x6FC3,           /* Group identity */
  	SOFTSIM_USIM_HIDDENKEY_ID = 0x6FC3,      /* key for hidden phonebook entries */  	
  	SOFTSIM_USIM_NETPAR_ID = 0x6FC4, 		/* Network parameters */
  	SOFTSIM_USIM_PNN_ID = 0x6FC5,            /* PLMN Netowrk Operator Name */
  	SOFTSIM_USIM_OPL_ID = 0x6FC6,            /* Operator PLMN List */
  	SOFTSIM_USIM_MBDN_ID = 0x6FC7,           /* Mailbox Dialing Number*/
  	SOFTSIM_USIM_EXT6_ID = 0x6FC8,           /* Extension 6 */
  	SOFTSIM_USIM_MBI_ID = 0x6FC9,            /* Mailbox Identifier*/
  	SOFTSIM_USIM_MWIS_ID = 0x6FCA,           /* Message Waiting Indication Status*/
  	SOFTSIM_USIM_CFIS_ID = 0x6FCB,           /* USIM ADF: Call Forwarding indicator status */
	SOFTSIM_USIM_EXT7_ID = 0x6FCC, 		  /* Extension 7 */
	SOFTSIM_USIM_SPDI_ID = 0x6FCD, 		  /* Service Provider Display Information*/
	SOFTSIM_USIM_MMSN_ID = 0x6FCE, 	/*MMS Notification*/
	SOFTSIM_USIM_EXT8_ID = 0x6FCF, 		  /* Extension 8 */
	SOFTSIM_USIM_MMSICP_ID = 0x6FD0,	/*MMS Issuer Connectivity Parameters*/
	SOFTSIM_USIM_MMSUP_ID = 0x6FD1,	/*MMS User Preferences*/
	SOFTSIM_USIM_MMSUCP_ID = 0x6FD2,	/*MMS User Connectivity Parameters*/
	SOFTSIM_USIM_NIA_ID = 0x6FD3,		/*Network's Indication of Alerting*/
	SOFTSIM_USIM_VGCSCA_ID = 0x6FD4,	/*Voice Group Call Service Ciphering Algorithm*/
	SOFTSIM_USIM_VBSCA_ID = 0x6FD5,		/*Voice Broadcast Service Ciphering Algorithm*/
	SOFTSIM_USIM_GBABP_ID = 0x6FD6,		/*GBA Bootstrapping parameters*/
	SOFTSIM_USIM_MSK_ID = 0x6FD7,		/*MBMS Service Keys List*/
	SOFTSIM_USIM_HPPLMUK_ID = 0x6FD6,	/*MBMS User Key*/
	SOFTSIM_USIM_GBANL_ID = 0x6FDA,		/*GBA NAF List*/
	SOFTSIM_USIM_EHPLMN_ID = 0x6FD9,	/* Equivalent HPLMN */
	SOFTSIM_USIM_EHPLMNPI_ID = 0x6FDB,	/* Equivalent HPLMN Presentation Indication*/
	SOFTSIM_USIM_LRPLMNSI_ID = 0x6FDC,	/* Last RPLMN Selection Indication */
	SOFTSIM_USIM_NAFKCA_ID = 0x6FDD,	/* NAF Key Center Address */
	SOFTSIM_USIM_SPNI_ID = 0x6FDE,		/*Service Provider Name Icon*/
	SOFTSIM_USIM_PNNI_ID = 0x6FDF,		/*PLMN Network Name Icon*/
	SOFTSIM_USIM_NCPIP_ID = 0x6FE2,		/*Network Connectivity Parameters for USIM IP connections*/
	SOFTSIM_USIM_EPSLOCI_ID = 0x6FE3,		/*EPS location information*/
	SOFTSIM_USIM_EPSNSC_ID = 0x6FE4,	/*EPS NAS Security Context*/
	SOFTSIM_USIM_UFC_ID = 0x6FE6,		/*USAT Facility Control*/
	SOFTSIM_USIM_UICCIARI_ID = 0x6FE7,	/*UICC IARI*/
	SOFTSIM_USIM_NASCONFIG_ID = 0x6FE8,		/*Non Access Stratum Configuration*/
	SOFTSIM_USIM_PWS_ID = 0x6FEC,		/*Public Warning System*/
	SOFTSIM_USIM_FDNURI_ID = 0x6FED,	/*Fixed Dialling Number URI*/
	SOFTSIM_USIM_BDNURI_ID = 0x6FEE,	/*Barred Dialling Numbers URI*/
	SOFTSIM_USIM_SDNURI_ID = 0x6FEF,	/*Service Dialling Numbers URI*/
	SOFTSIM_USIM_IWL_ID = 0x6FF0,		/*IMEI(SV) White Lists*/
	SOFTSIM_USIM_IPS_ID = 0x6FF1,		/*IMEI(SV) Pairing Status*/
	SOFTSIM_USIM_IPD_ID = 0x6FF2,		/*IMEI(SV) Pairing Device*/

	
	SOFTSIM_USIM_PHONEBOOK_ID = 0x5F3A,
	SOFTSIM_USIM_GSM_ACCESS_ID = 0x5F3B,
	SOFTSIM_USIM_MEXE_ID = 0x5F3C,
	SOFTSIM_USIM_SOLSA = 0x5F70,
	SOFTSIM_USIM_WLAN_ID = 0x5F40,
	SOFTSIM_USIM_HNB_ID = 0x5F50,
	SOFTSIM_USIM_PROSE_ID = 0x5F90,
	/*EFs at DF GSM under USIM ADF */
  	SOFTSIM_USIM_KC_ID = 0x4F20,             /* GSM ciphering key Kc */
  	SOFTSIM_USIM_KCGPRS_ID = 0x4F52,         /* GPRS ciphering key KcGPRS*/
  	SOFTSIM_USIM_CPBCCH_ID = 0x4F63,         /* CPBCCH information */
  	SOFTSIM_USIM_INVSCAN_ID = 0x4F64,        /* Investigation scan */


	SOFTSIM_USIM_FILE_ID_MAX,
	SOFTSIM_USIM_FILE_ID_END = 0xFF00
}SoftsimADFUSIMFileId;




/* Softsim file types. */
typedef enum {
    	SOFTSIM_MASTER_FILE     = 0x00000001,
    	SOFTSIM_DEDICATED_FILE  = 0x00000002,
    	SOFTSIM_ELEMENTARY_FILE = 0x00000004
}SoftsimFileType;

/*Softsim file structure type*/
typedef enum
{
  	SOFTSIM_LINEAR_FIXED,
  	SOFTSIM_TRANSPARENT,
  	SOFTSIM_CYCLIC
}SoftsimFileStructureType;

/* Context parameter for a 3G Authentication*/
typedef enum {
    	SOFTSIM_GSM_CONTEXT  = 0,
    	SOFTSIM_3G_CONTEXT   = 1
}SoftsimAuthenticationContext;

typedef enum{
	SOFTSIM_APDU_CLASS_NONE,
	SOFTSIM_APDU_CLASS_ICC,
	SOFTSIM_APDU_CLASS_UICC,
	SOFTSIM_APDU_CLASS_MAX,
	SOFTSIM_APDU_CLASS_NOT_SUPPORT,
	SOFTSIM_APDU_CLASS_END = 0xff
}SoftsimApduClass;

typedef enum {
		SOFTSIM_APDU_NONE,
    	SOFTSIM_APDU_SELECT_FILE,
		SOFTSIM_APDU_STATUS,
		SOFTSIM_APDU_READ_BINARY,
		SOFTSIM_APDU_UPDATE_BINARY,
		SOFTSIM_APDU_READ_RECORD,
		SOFTSIM_APDU_UPDATE_RECORD,
		SOFTSIM_APDU_SEARCH_CORD,
		SOFTSIM_APDU_INCREASE,
		SOFTSIM_APDU_RETRIEVE_DATA,
		SOFTSIM_APDU_SET_DATA,
		SOFTSIM_APDU_VERIFY,
		SOFTSIM_APDU_CHANGE_PIN,
		SOFTSIM_APDU_DISABLE_PIN,
		SOFTSIM_APDU_ENABLE_PIN,
		SOFTSIM_APDU_UNBLOCK_PIN,
		SOFTSIM_APDU_DEACTIVATE_FILE,
		SOFTSIM_APDU_ACTIVE_FILE,
		SOFTSIM_APDU_AUTHENTICATE,
		SOFTSIM_APDU_GET_CHALLENGE,
		SOFTSIM_APDU_TERMINAL_CAPABILITY,
		SOFTSIM_APDU_TERMINAL_PROFILE,
		SOFTSIM_APDU_ENVELOPE,
		SOFTSIM_APDU_FETCH,
		SOFTSIM_APDU_TERMINAL_RESPONSE,
		SOFTSIM_APDU_MANAGE_CHANNEL,
		SOFTSIM_APDU_MANAGE_SECURE_CHANNEL,
		SOFTSIM_APDU_TRANSACT_DATA,
		SOFTSIM_APDU_GET_RESPONSE,
		SOFTSIM_APDU_INSTRUCTION_NOT_SUPPORT,
		SOFTSIM_APDU_INSTRUCTION_TYPE_MAX,
		SOFTSIM_APDU_INSTRUCTION_TYPE_END = 0xff
}SoftsimApduInstruction;

/* This enum defines the first status word                                   */
typedef enum {
  SOFTSIM_APDU_SW1_NORMAL_END = 0x90,  /* Normal ending of the command*/
  SOFTSIM_APDU_SW1_END_FETCH  = 0x91,  /* Normal ending of the command with extra information */
  SOFTSIM_APDU_SW1_END_RETRY  = 0x92,  /* Normal ending of the command with extra information concerning an ongoing data transfer session*/
  SOFTSIM_APDU_SW1_BUSY 	  = 0x93,  /* SIM App toolkit is busy*/
  SOFTSIM_APDU_SW1_REFERENCE  = 0x94,  /* A reference management problem				  */

  SOFTSIM_APDU_SW1_DLOAD_ERR  = 0x9E,  /* SIM data download error */
  SOFTSIM_APDU_SW1_END_RESP   = 0x9F,  /* Normal ending of the command with a response*/
  SOFTSIM_APDU_SW1_SECURITY   = 0x98,  /* A security problem                              */
  SOFTSIM_APDU_SW1_NULL       = 0x60,  /* NULL procedure byte                             */
  SOFTSIM_APDU_SW1_WRONG_LENGTH  = 0x67,  /* Wrong length                          */
  SOFTSIM_APDU_SW1_P1_P2_BAD  = 0x6B,  /* Wrong parameter P1 or P2                    */
  SOFTSIM_APDU_SW1_INS_INVALID    = 0x6D,  /* Instruction code not support or invalid */
  SOFTSIM_APDU_SW1_CLA_BAD    = 0x6E,  /* Wrong instruction class given in command        */
  SOFTSIM_APDU_SW1_PROBLEM    = 0x6F,  /* Technical problem; no diagnostic given          */
  SOFTSIM_APDU_SW1_SIG_ERROR1 = 0xFF,  /* This is an indication of an error signal from the transmitted byte*/
  SOFTSIM_APDU_SW1_SIG_ERROR2 = 0xFE,  /* This is an indication of an error signal from the transmitted byte*/
  SOFTSIM_APDU_SW1_NO_STATUS  = 0x00,   /* This status word is used when a status word cannot
                             be obtained from the card.  It offers to status
                             information */
  SOFTSIM_APDU_SW1_WARNINGS1 = 0x62,     /* Warnings                                      */
  SOFTSIM_APDU_SW1_WARNINGS2 = 0x63,     /* Warnings                                      */
  SOFTSIM_APDU_SW1_EXECUTION_ERR2    = 0x65, /* No info given, state of NV memory changed */
  SOFTSIM_APDU_SW1_CMD_NOT_ALLOWED   = 0x69, /* Command not allowed                       */
  SOFTSIM_APDU_SW1_WRONG_PARAMS      = 0x6A, /* Wrong parameters                          */
  SOFTSIM_APDU_SW1_USIM_RESEND_APDU  = 0x6C, /* Re-send the APDU header                   */
  SOFTSIM_APDU_SW1_EXECUTION_ERR1    = 0x64, /* No info given, NV memory unchanged        */
  SOFTSIM_APDU_SW1_CLA_NOT_SUPPORTED = 0x68, /* functions in CLA not supported            */
  SOFTSIM_APDU_SW1_USIM_END_RESP     = 0x61, /* Normal ending of a command                */
  SOFTSIM_APDU_SW1_UTIL_END_RESP     = 0x61, /* Normal ending of a command                */
  SOFTSIM_APDU_SW1_LENGTH_ERROR      = 0x6C, /* Wrong length Le                           */
  SOFTSIM_APDU_SW1_SE_FAIL           = 0x66 /* Security Environment error/not set        */
}SoftsimApduSw1type;

/* This enum defines the second status word                                  */
typedef enum {
  SOFTSIM_APDU_SW2_NORMAL_END    = 0x00,  /* Normal ending of the command                 */
  SOFTSIM_APDU_SW2_MEM_PROBLEM   = 0x40,  /* Memory problem                               */
  SOFTSIM_APDU_SW2_NO_EF         = 0x00,  /* No EF selected                               */
  SOFTSIM_APDU_SW2_OUT_OF_RANGE  = 0x02,  /* Out of range (invalid address)               */
  SOFTSIM_APDU_SW2_NOT_FOUND     = 0x04,  /* File ID/pattern not found                    */
  SOFTSIM_APDU_SW2_INCONSISTENT  = 0x08,  /* File inconsistent with command               */
  SOFTSIM_APDU_SW2_NO_CHV        = 0x02,  /* No CHV initialized                           */
  SOFTSIM_APDU_SW2_NOT_FULFILLED = 0x04,  /* Access condition not fulfilled               */
  SOFTSIM_APDU_SW2_CONTRADICTION = 0x08,  /* In contradiction with CHV status             */
  SOFTSIM_APDU_SW2_INVALIDATION  = 0x10,  /* In contradiction with invalidation status    */
  SOFTSIM_APDU_SW2_SEQ_PROBLEM   = 0x34,  /* Problem in the sequence of the command       */
  SOFTSIM_APDU_SW2_BLOCKED       = 0x40,  /* Access blocked                               */
  SOFTSIM_APDU_SW2_MAX_REACHED   = 0x50,  /* Increase cannot be performed; Max value
                                reached                                      */
  SOFTSIM_APDU_SW2_ALGORITHM_NOT_SUPPORTED   = 0x84,
  SOFTSIM_APDU_SW2_INVALID_KEY_CHECK_VALUE   = 0x85,
  SOFTSIM_APDU_SW2_PIN_BLOCKED               = 0x83,
  SOFTSIM_APDU_SW2_NO_EF_SELECTED            = 0x86,
  SOFTSIM_APDU_SW2_FILE_NOT_FOUND            = 0x82,
  SOFTSIM_APDU_SW2_REF_DATA_NOT_FOUND        = 0x88,
  SOFTSIM_APDU_SW2_NV_STATE_UNCHANGED        = 0x00,
  SOFTSIM_APDU_SW2_CMD_SUCCESS_INT_X_RETRIES = 0xC0,
  SOFTSIM_APDU_SW2_FUNCTION_NOT_SUPPORT      = 0x81,	/*Function not supported*/
  SOFTSIM_APDU_SW2_SECURITY_NOT_SATISFIED    = 0x82,
  SOFTSIM_APDU_SW2_NO_INFO_GIVEN             = 0x00,
  SOFTSIM_APDU_SW2_LOGICAL_CHAN_NOT_SUPPORTED= 0x81,
  SOFTSIM_APDU_SW2_SECURE_MSG_NOT_SUPPORTED  = 0x82,
  SOFTSIM_APDU_SW2_FILE_STRUCT_INCOMPATIBLE  = 0x81,
  SOFTSIM_APDU_SW2_REF_DATA_INVALIDATED      = 0x84,
  SOFTSIM_APDU_SW2_CONDITIONS_NOT_SATISFIED  = 0x85,
  SOFTSIM_APDU_SW2_BAD_PARAMS_IN_DATA_FIELD  = 0x80,
  SOFTSIM_APDU_SW2_FUNCTION_NOT_SUPPORTED    = 0x81,
  SOFTSIM_APDU_SW2_RECORD_NOT_FOUND          = 0x83,
  SOFTSIM_APDU_SW2_INCONSISTENT_LC           = 0x85,
  SOFTSIM_APDU_SW2_BAD_PARAMS_P1_P2          = 0x86,
  SOFTSIM_APDU_SW2_LC_INCONSISTENT_WITH_P1_P2= 0x87,
  SOFTSIM_APDU_SW2_NOT_ENOUGH_MEMORY_SPACE   = 0x84,
  SOFTSIM_APDU_SW2_NV_STATE_CHANGED          = 0x00,
  SOFTSIM_APDU_SW2_MORE_DATA_AVAILABLE       = 0x10,

  SOFTSIM_APDU_SW2_RET_DATA_MAY_BE_CORRUPTED = 0x81,
  SOFTSIM_APDU_SW2_EOF_REACHED               = 0x82,
  SOFTSIM_APDU_SW2_SEL_EF_INVALIDATED        = 0x83,
  SOFTSIM_APDU_SW2_INCORRECT_FCI_FORMAT      = 0x84,

  SOFTSIM_APDU_SW2_FILE_FILLED_BY_LAST_WRITE = 0x81,

  SOFTSIM_APDU_SW2_AUTH_INCORRECT_MAC   = 0x62,	/* Authentication error, incorrect MAC */
  SOFTSIM_APDU_SW2_AUTH_SECURITY_CONTEXT_NOT_SUPPORT	= 0x64, /* Authentication error, security context not supported */
  SOFTSIM_APDU_SW2_KEY_FRESHNESS_FAIL	= 0x65, /* Key freshness failure */
  SOFTSIM_APDU_SW2_AUTH_NO_MEMORY_AVAILABLE	= 0x66, /* Authentication error, no memory space available */
  SOFTSIM_APDU_SW2_AUTH_NO_MEMORY_AVAILABLE_IN_EFMUK = 0x67, /* Authentication error, no memory space available in EFmuk */

  SOFTSIM_APDU_SW2_PIN_STATUS_ERR     = 0x85, /* In contradiction with PIN status */
  SOFTSIM_APDU_SW2_PREMSTR_NOT_RDY    = 0x85, /* Pre-master secret not ready */
  SOFTSIM_APDU_SW2_INTRNL_NOT_RDY     = 0x85, /* Internal data not ready */
  SOFTSIM_APDU_SW2_INCORRECT_TAG      = 0x80, /* Incorrect data or tag */
  SOFTSIM_APDU_SW2_UNDEF_99           = 0x99,
  SOFTSIM_APDU_SW2_MORE_DATA          = 0xF1,
  SOFTSIM_APDU_SW2_AUTH_RSP_DATA      = 0xF3, /* Authentication Response Data Available*/

  SOFTSIM_APDU_SW2_UCAST_AUTH_KEY_NOT_INIT                = 0x01,   /* sw1 = 0x69 */
  SOFTSIM_APDU_SW2_UCAST_DATA_BLOCK_NOT_INIT              = 0x02,   /* sw1 = 0x69 */
  SOFTSIM_APDU_SW2_VERIFY_UCAST_FAILED                    = 0x04,   /* sw1 = 0x69 */
  SOFTSIM_APDU_SW2_LOCKED_INIT_PARAM                      = 0x05,   /* sw1 = 0x69 */
  SOFTSIM_APDU_SW2_APPLET_NOT_INIT                        = 0x06,   /* sw1 = 0x69 */
  SOFTSIM_APDU_SW2_PUBLIC_KEY_OR_CERT_NOT_INIT            = 0x07    /* sw1 = 0x69 */

}SoftsimApduSw2type;

typedef enum
{
	SOFTSIM_SERVICE_N1,		/*Local phone book*/	
	SOFTSIM_SERVICE_N2,
	SOFTSIM_SERVICE_N3,
	SOFTSIM_SERVICE_N4,
	SOFTSIM_SERVICE_N5,
	SOFTSIM_SERVICE_N6,
	SOFTSIM_SERVICE_N7,
	SOFTSIM_SERVICE_N8,
	SOFTSIM_SERVICE_N9,
	SOFTSIM_SERVICE_N10,
	SOFTSIM_SERVICE_N11,
	SOFTSIM_SERVICE_N12,
	SOFTSIM_SERVICE_N13,
	SOFTSIM_SERVICE_N14,
	SOFTSIM_SERVICE_N15,
	SOFTSIM_SERVICE_N16,
	SOFTSIM_SERVICE_N17,
	SOFTSIM_SERVICE_N18,
	SOFTSIM_SERVICE_N19,
	SOFTSIM_SERVICE_N20,
	SOFTSIM_SERVICE_N21,
	SOFTSIM_SERVICE_N22,
	SOFTSIM_SERVICE_N23,
	SOFTSIM_SERVICE_N24,
	SOFTSIM_SERVICE_N25,
	SOFTSIM_SERVICE_N26,
	SOFTSIM_SERVICE_N27,	/*GSM Access*/
	SOFTSIM_SERVICE_N28,
	SOFTSIM_SERVICE_N29,
	SOFTSIM_SERVICE_N30,
	SOFTSIM_SERVICE_N31,
	SOFTSIM_SERVICE_N32,
	SOFTSIM_SERVICE_N33,
	SOFTSIM_SERVICE_N34,
	SOFTSIM_SERVICE_N35,
	SOFTSIM_SERVICE_N36,
	SOFTSIM_SERVICE_N37,
	SOFTSIM_SERVICE_N38,	/*GSM security context*/	
	SOFTSIM_SERVICE_N39,
	SOFTSIM_SERVICE_N40,
	SOFTSIM_SERVICE_N41,
	SOFTSIM_SERVICE_N42,
	SOFTSIM_SERVICE_N43,
	SOFTSIM_SERVICE_N44,
	SOFTSIM_SERVICE_N45,
	SOFTSIM_SERVICE_N46,
	SOFTSIM_SERVICE_N47,
	SOFTSIM_SERVICE_N48,
	SOFTSIM_SERVICE_N49,
	SOFTSIM_SERVICE_N50,
	SOFTSIM_SERVICE_N51,
	SOFTSIM_SERVICE_N52,
	SOFTSIM_SERVICE_N53,
	SOFTSIM_SERVICE_N54,
	SOFTSIM_SERVICE_N55,
	SOFTSIM_SERVICE_N56,
	SOFTSIM_SERVICE_N57,
	SOFTSIM_SERVICE_N58,
	SOFTSIM_SERVICE_N59,
	SOFTSIM_SERVICE_N60,
	SOFTSIM_SERVICE_N61,
	SOFTSIM_SERVICE_N62,
	SOFTSIM_SERVICE_N63,
	SOFTSIM_SERVICE_N64,
	SOFTSIM_SERVICE_N65,
	SOFTSIM_SERVICE_N66,
	SOFTSIM_SERVICE_N67,
	SOFTSIM_SERVICE_N68,
	SOFTSIM_SERVICE_N69,
	SOFTSIM_SERVICE_N70,
	SOFTSIM_SERVICE_N71,
	SOFTSIM_SERVICE_N72,
	SOFTSIM_SERVICE_N73,
	SOFTSIM_SERVICE_N74,
	SOFTSIM_SERVICE_N75,
	SOFTSIM_SERVICE_N76,
	SOFTSIM_SERVICE_N77,
	SOFTSIM_SERVICE_N78,
	SOFTSIM_SERVICE_N79,
	SOFTSIM_SERVICE_N80,
	SOFTSIM_SERVICE_N81,
	SOFTSIM_SERVICE_N82,
	SOFTSIM_SERVICE_N83,
	SOFTSIM_SERVICE_N84,
	SOFTSIM_SERVICE_N85,
	SOFTSIM_SERVICE_N86,
	SOFTSIM_SERVICE_N87,
	SOFTSIM_SERVICE_N88,
	SOFTSIM_SERVICE_N89,
	SOFTSIM_SERVICE_N90,
	SOFTSIM_SERVICE_N91,
	SOFTSIM_SERVICE_N92,
	SOFTSIM_SERVICE_N93,
	SOFTSIM_SERVICE_N94,
	SOFTSIM_SERVICE_N95,
	SOFTSIM_SERVICE_N96,
	SOFTSIM_SERVICE_N97,
	SOFTSIM_SERVICE_N98,
	SOFTSIM_SERVICE_N99,
	SOFTSIM_SERVICE_N100,
	SOFTSIM_SERVICE_N101,
	SOFTSIM_SERVICE_N102,
	SOFTSIM_SERVICE_MAX,
	SOFTSIM_SERVICE_END = 0XFF
}SoftsimServiceType;

typedef enum
{
	SOFTSIM_RECORD_NONE,
	SOFTSIM_RECORD_NEXT,
	SOFTSIM_RECORD_PREVIOUS,
	SOFTSIM_RECORD_ABSOLUTE,
	SOFTSIM_RECORD_END
}SoftsimRecordOffset;

typedef enum
{
	SOFTSIM_GSM_SECURITY_CONTEXT = 0x0000,
	SOFTSIM_3G_SECURITY_CONTEXT = 0x0001,
	SOFTSIM_VGCS_VBS_SECURITY_CONTEXT = 0x0010,
	SOFTSIM_GBA_SECURITY_CONTEXT = 0x0100,
	SOFTSIM_SECURITY_CONTEXT_END = 0XFFFF
}SoftsimAuthencationSecurityContext;

typedef enum
{
	SOFTSIM_ACTION_NONE,
	SOFTSIM_SELECT_FILE,
	SOFTSIM_AUTHENTICATION,
	SOFTSIM_ACTION_MAX,
	SOFTSIM_ACTION_EDN = 0xFF
}SoftsimActionType;

typedef enum
{
	SOFTSIM_APDU_CASE_NONE,
	SOFTSIM_APDU_CASE_1,
	SOFTSIM_APDU_CASE_2,
	SOFTSIM_APDU_CASE_3,
	SOFTSIM_APDU_CASE_4,
	SOFTSIM_APDU_NOT_SUPPORT,
	SOFTSIM_APDU_CASE_END = 0xff,
}SoftsimApduContentStructure;

typedef enum
{
	SOFTSIM_GSM = 0x0001,
	SOFTSIM_GSM_COMPACT = 0x0002,
	SOFTSIM_CDMA2000_HRPD = 0x0004,
	SOFTSIM_CDMA2000_1xRTT = 0x008,
	SOFTSIM_UTRAN = 0x0010,
	SOFTSIM_EUTRAN = 0x0020,
}SoftsimAccessTechnology;


typedef enum
{
	SOFTSIM_APDU_STATUS_NONE,
	SOFTSIM_APDU_IDLE,
	SOFTSIM_APDU_BUSY,
	SOFTSIM_APDU_STATUS_END=0xff,
}SoftsimApduStatus;

typedef enum
{
	SOFTSIM_CS_DOMAIN,	/*circuit switch domain*/
	SOFTSIM_PS_DOMAIN,	/*packet switch domain*/
	SOFTSIM_IMS_DOMAIN,	/*imsi application*/
	SOFTSIM_BSF_DOMAIN,	/*multimedia*/
	SOFTSIM_AAA_DOMAIN,	/*AAA*/
	SOFTSIM_DOMIAN_MAX,
	SOFTSIM_DOMAIN_END = 0xff
}SoftsimAuthDomain;

typedef enum
{
	SOFTSIM_MF_LEVEL,
	SOFTSIM_DF_TELECOM_LEVEL,
	SOFTSIM_DF_GSM_LEVEL,
	SOFTSIM_ADF_USIM_LEVEL,
	SOFTSIM_FILE_LEVEL_MAX,
	SOFTSIM_FILE_LEVEL_END = 0xff
}SoftsimFileLevel;

#ifdef __cplusplus
		}
#endif

#endif /*__SOFTSIM_UICC_ENUM_H__*/
