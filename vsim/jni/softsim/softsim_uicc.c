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
 * Filename: softsim_uicc.c
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
#include <dlfcn.h>
#include<pthread.h>  
#include<string.h>  
#include<sys/types.h>  
#include<unistd.h>
#include <fcntl.h>
#include "softsim_include.h"
#include "VirtualSim.h"
/*****************************************************************************
* Define																	   
*****************************************************************************/
#undef 	SOFTSIM_MD_ID
#undef 	SOFTSIM_TRACE_HEADER
#define SOFTSIM_MD_ID					SOFTSIM_MD_UICC
#define SOFTSIM_TRACE_HEADER			"[UICC]"

#if defined(__UICC_DATA_TRACE__)
#define SOFTSIM_UICC_TRACE_HEX	SOFTSIM_PRINT_HEX
#define SOFTSIM_UICC_TRACE	SOFTSIM_DEBUG
#define SOFTSIM_AUTH_TRACE_HEX(header, data, data_len)
#define SOFTSIM_AUTH_TRACE(...)
#elif defined(__AUTH_DATA_TRACE__)
#define SOFTSIM_UICC_TRACE_HEX(header, data, data_len)
#define SOFTSIM_UICC_TRACE(...)
#define SOFTSIM_AUTH_TRACE_HEX	SOFTSIM_PRINT_HEX
#define SOFTSIM_AUTH_TRACE	SOFTSIM_DEBUG
#else
#define SOFTSIM_UICC_TRACE_HEX(header, data, data_len)
#define SOFTSIM_UICC_TRACE(...)
#define SOFTSIM_AUTH_TRACE_HEX(header, data, data_len)
#define SOFTSIM_AUTH_TRACE(...)
#endif


#define SOFTSIM_RETRY_COUNT	(5)
/*****************************************************************************
* Typedef																	   
*****************************************************************************/

/*****************************************************************************
* Static Local Variable															  
*****************************************************************************/
bool is_command_processing = false;
u8 tpdu_buffer[SOFTSIM_APDU_MAX_LEN] = {0};
u8 tpdu_len = 0;
bool Authentication_flag = false;
static volatile bool softsim_ready = false;
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
#if defined(__SOFTSIM_ANDROID__) || defined(__SOFTSIM_LINUX__)
s32 softsim_uicc_init(char *plmn)
{
	s32 ret = -1;

	if(softsim_strlen(plmn) < 5)
	{
		SOFTSIM_DEBUG("PLMN is invalid!");
		return SOFTSIM_EXCEPTION_INVALID_PLMN;
	}

	return VirtualSim_init(plmn);
}

s32 softsim_apdu_command(u8 slot, u8 *rx_data, u32 rx_size, u8 *tx_data, u32 *tx_size)
{
	s32 result = 1;
	u32 len = 0;
	
	if(NULL == rx_data || rx_size < 1 || NULL == tx_data || NULL == tx_size)
	{
		return result;
	}
	
	SOFTSIM_UICC_TRACE_HEX("C-APDU:", rx_data, rx_size);

	if(5 == softsim_get_apdu_struct(rx_data, rx_size))
	{
		SOFTSIM_DEBUG("Invalid APDU!");
		softsim_exception_report(SOFTSIM_EXCEPTION_INVALID_APDU);
		return result;
	}

	if((0x00 == rx_data[0]) && ((0x88 == rx_data[1]) || (0x89 == rx_data[1]))
		|| (0xA0 == rx_data[0]) && ((0x88 == rx_data[1]) || (0x89 == rx_data[1])))
	{
		SOFTSIM_AUTH_TRACE_HEX("C-APDU:", rx_data, rx_size);
		softsim_remote_sim_authenticaton(rx_data, rx_size, tx_data, tx_size);
		SOFTSIM_AUTH_TRACE_HEX("R-APDU:", tx_data, *tx_size);
	}
	else if(Authentication_flag && ((0x00 == rx_data[0]) || (0xA0 == rx_data[0])) && (0xC0 == rx_data[1]))
	{
		SOFTSIM_AUTH_TRACE_HEX("C-APDU:", rx_data, rx_size);
		softsim_get_remote_authentication_response(slot, tx_data, tx_size);
		Authentication_flag = false;
		SOFTSIM_AUTH_TRACE_HEX("R-APDU:", tx_data, *tx_size);
	}
	else
	{
		if(NULL != TransmitApdu)
		{
			len = (u32)TransmitApdu((char*)rx_data, (char*)tx_data);
			*tx_size = len;
		}
	}
	
	SOFTSIM_UICC_TRACE_HEX("R-APDU:", tx_data, *tx_size);
	return 0;
}

s32 softsim_tpdu_command(u8 slot, u8 *rx_data, u32 rx_size, u8 *tx_data, u32 *tx_size)
{
	s32 result = 1;
	SoftsimApduClass class = 0xff;
	SoftsimApduInstruction instruction = 0xff;
	u8 sw1 = SOFTSIM_APDU_SW1_NORMAL_END;
	u8 sw2 = SOFTSIM_APDU_SW2_NORMAL_END;
	u32 index = 0;
	
	if(NULL == rx_data || rx_size < 1 || NULL == tx_data || NULL == tx_size)
	{
		return result;
	}
	
	if(is_command_processing)
	{
		softsim_memcpy(tpdu_buffer+tpdu_len, rx_data, rx_size);
		tpdu_len += rx_size;
		if(tpdu_len >= (SOFTSIM_APDU_HEADER_LEN + tpdu_buffer[SOFTSIM_APDU_HEADER_LEN]))
		{
			softsim_apdu_command(slot, tpdu_buffer, tpdu_len, tx_data, tx_size);
			is_command_processing = false;
		}
		
		return result;
	}

	class = (SoftsimApduClass)softsim_get_apdu_class(rx_data[0], rx_data[1]);

	if(SOFTSIM_APDU_CLASS_UICC != class)
	{
		sw1 = SOFTSIM_APDU_SW1_CLA_BAD;
		tx_data[index++] = (u8)sw1;
		tx_data[index++] = (u8)sw2;
		*tx_size = index;
		
		return result;
	}

	instruction = (SoftsimApduInstruction)softsim_get_apdu_instruction(rx_data[0], rx_data[1]);

	if(instruction >= SOFTSIM_APDU_INSTRUCTION_NOT_SUPPORT)
	{
		sw1 = SOFTSIM_APDU_SW1_INS_INVALID;
		tx_data[index++] = (u8)sw1;
		tx_data[index++] = (u8)sw2;
		*tx_size = index;
		
		return result;
	}

	if(softsim_command_is_tpdu(instruction, rx_data, rx_size))
	{
		is_command_processing = true;
		softsim_memset(tpdu_buffer, 0x00, sizeof(tpdu_buffer));
		softsim_memcpy(tpdu_buffer, rx_data, rx_size);
		tpdu_len = rx_size;
		*tx_data = tpdu_buffer[1];
		*tx_size = 1;
	}
	else
	{
		if(SOFTSIM_APDU_VERIFY == instruction || SOFTSIM_APDU_UNBLOCK_PIN == instruction)
		{
			softsim_apdu_command(slot, rx_data, rx_size, tx_data, tx_size);
		}
		else
		{
			softsim_apdu_command(slot, rx_data, rx_size, tx_data, tx_size);
		}
	}
	
	return result;
}

void softsim_write_binary(u16 file_id, u8 *data, u8 data_len)
{
	VirtualSimWriteBinaryCustom(file_id, data, data_len);
}
#else
s32 softsim_uicc_init(char *plmn)
{
	return 0;
}

s32 softsim_tpdu_command(u8 slot, u8 *rx_data, u32 rx_size, u8 *tx_data, u32 *tx_size)
{
	return 0;
}

void softsim_write_binary(u16 file_id, u8 *data, u8 data_len)
{

}

#endif

bool softsim_command_is_tpdu_processing(void)
{
	return is_command_processing;
}

void softsim_tpdu_process_state_reset(void)
{
	is_command_processing = false;
	Authentication_flag = false;
}

void softsim_uicc_exit(void)
{

}
