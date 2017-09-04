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
 * Filename: softsim_trace.h
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-07-08
 ******************************************************************************/
#ifndef __SOFTSIM_TRACE_H__
#define __SOFTSIM_TRACE_H__

#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************
* Include                                                                     
*****************************************************************************/
#include "softsim_type.h"
#include "softsim_macro.h"
#include "softsim_release.h"
#include "softsim_trace.h"

#if defined(__SOFTSIM_ANDROID_TRACE__)
#include <android/log.h>
#endif
/*****************************************************************************
* Define                                                                      
*****************************************************************************/
#define SOFTSIM_TRACE_DIR						"softsim_trace"
#define SOFTSIM_TRACE_CRASH_FILE				"softsim_crash.txt"
#define SOFTSIM_TRACE_ERROR_FILE				"softsim_error.txt"
#define SOFTSIM_TRACE_LOG_FILE					"softsim_log.txt"
	
#define SOFTSIM_DEBUG_HEAD_LEN					(10)
#define SOFTSIM_DEBUG_FILE_NUM					(10)
	
#define SOFTSIM_TRACE_BUFFER_SIZE				(1024)
	
#define SOFTSIM_TRACE_FILE_SIZE_MAX				(20*1024*1024)


#define SOFTSIM_TRACE_BUFFER_SIZE	(1024)
#define SOFTSIM_TRACE_LEVEL                     (SOFTSIM_TRACE_ALL)

#define SOFTSIM_MD_DEFAULT			0x00000001L
#define SOFTSIM_MD_MAIN				0x00000002L
#define SOFTSIM_MD_TEST				0x00000004L
#define SOFTSIM_MD_VERSION			0x00000008L
#define SOFTSIM_MD_FILESYSTEM		0x00000010L
#define SOFTSIM_MD_AUTHENTICATION	0x00000020L
#define SOFTSIM_MD_API				0x00000040L
#define SOFTSIM_MD_UICC				0x00000080L
#define SOFTSIM_MD_ISO_7816			0x00000100L
#define SOFTSIM_MD_CONSOLE			0x00000200L
#define SOFTSIM_MD_SOCKET			0x00000400L
#define SOFTSIM_MD_APDU				0x00000800L
#define SOFTSIM_MD_HEARTBEAT		0x00001000L
#define SOFTSIM_MD_MESSAGE			0x00002000L
#define SOFTSIM_MD_SESSION			0x00004000L
#define SOFTSIM_MD_MONITOR			0x00008000L
#define SOFTSIM_MD_EXCEPTION		0x00010000L
#define SOFTSIM_MD_TIMER			0x00020000L
#define SOFTSIM_MD_SYSTEM			0x00040000L
#define SOFTSIM_MD_DRIVER			0x00080000L


#define SOFTSIM_MD_ALL		0xFFFFFFFFL

//#define SOFTSIM_TRACE_MODULE	(SOFTSIM_MD_ALL)
#define SOFTSIM_TRACE_MODULE	(SOFTSIM_MD_HEARTBEAT|SOFTSIM_MD_VERSION | SOFTSIM_MD_ISO_7816 | SOFTSIM_MD_UICC\
								| SOFTSIM_MD_CONSOLE | SOFTSIM_MD_AUTHENTICATION | SOFTSIM_MD_API\
								| SOFTSIM_MD_EXCEPTION | SOFTSIM_MD_SOCKET | SOFTSIM_MD_SESSION | SOFTSIM_MD_TEST\
								| SOFTSIM_MD_DRIVER | SOFTSIM_MD_SYSTEM)

#define SOFTSIM_TRACE_HEADER	"[softsim]"

#define __SOFTSIM_FILE__							((softsim_strrchr(__FILE__, '/') == NULL)? __FILE__ : softsim_strrchr(__FILE__, '/')+1)
#define __SOFTSIM_FUNC__				   			__FUNCTION__ 
#define __SOFTSIM_LINE__				    			__LINE__ 
#define __SOFTSIM_MODE__

/*****************************************************************************
* Typedef                                                                     
*****************************************************************************/
typedef enum
{
	SOFTSIM_TRACE_NONE,
	SOFTSIM_TRACE_CRASH,
	SOFTSIM_TRACE_ERROR,
	SOFTSIM_TRACE_LOG,
	SOFTSIM_TRACE_DEBUG,
	SOFTSIM_TRACE_ALL,
	SOFTSIM_TRACE_END = 0xff,
}SoftsimTraceLevel;

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
extern void softsim_trace_printf(SoftsimTraceLevel level, char *file, char *function, u32 line, const char *fmt, ...);
extern void softsim_trace_printf_ex(char *module, SoftsimTraceLevel level, char *file, char *function, u32 line, const char *fmt, ...);
extern void softsim_trace_printf_hex(SoftsimTraceLevel level, char *file, char *function, u32 line, char *header, u8 *data, u32 data_len);
extern void softsim_trace_printf_bin(SoftsimTraceLevel level, char *file, u8 *data, u32 data_len);
extern void softsim_printf_hex(char*header, u8 *data, u32 data_len);
/****************************************************************************
 * Function
 ****************************************************************************/
#if defined(__SOFTSIM_TRACE__)
#define SOFTSIM_TRACE_PRINT(level, ...)     			softsim_trace_printf(level, (char *)__SOFTSIM_FILE__, (char *)__SOFTSIM_FUNC__, (u32)__SOFTSIM_LINE__, __VA_ARGS__)
#define SOFTSIM_TRACE_PRINT_EX(mod, level, ...)     	softsim_trace_printf_ex(mod, level, (char *)__SOFTSIM_FILE__, (char *)__SOFTSIM_FUNC__, (u32)__SOFTSIM_LINE__, __VA_ARGS__)
#define SOFTSIM_TRACE_HEX(header, data, data_len)	softsim_trace_printf_hex(SOFTSIM_TRACE_DEBUG, (char *)__SOFTSIM_FILE__, (char *)__SOFTSIM_FUNC__, (u32)__SOFTSIM_LINE__, header, data, data_len)
#define SOFTSIM_TRACE_BIN(file, data, data_len)		softsim_trace_printf_bin(SOFTSIM_TRACE_DEBUG, file, data, data_len)

#define SOFTSIM_CRASG_EX(mod_id, header, ...)		SOFTSIM_TRACE_PRINT_EX(header, SOFTSIM_TRACE_CRASH, __VA_ARGS__)
#define SOFTSIM_ERROR_EX(mod_id, header, ...)		SOFTSIM_TRACE_PRINT_EX(header, SOFTSIM_TRACE_ERROR, __VA_ARGS__)
#define SOFTSIM_LOG_EX(mod_id, header, ...)			SOFTSIM_TRACE_PRINT_EX(header, SOFTSIM_TRACE_LOG, __VA_ARGS__)
#define SOFTSIM_DEBUG_EX(mod_id, header, ...)		do{if(mod_id&SOFTSIM_TRACE_MODULE){SOFTSIM_TRACE_PRINT_EX(header, SOFTSIM_TRACE_DEBUG, __VA_ARGS__);}}while(0)

#define SOFTSIM_CRASH(...)		    					do{SOFTSIM_CRASG_EX(SOFTSIM_MD_ID, SOFTSIM_TRACE_HEADER, __VA_ARGS__);SOFTSIM_TRACE_PRINT_EX(SOFTSIM_TRACE_HEADER, SOFTSIM_TRACE_DEBUG, __VA_ARGS__);}while(0)
#define SOFTSIM_ERROR(...)						do{SOFTSIM_ERROR_EX(SOFTSIM_MD_ID, SOFTSIM_TRACE_HEADER, __VA_ARGS__); SOFTSIM_TRACE_PRINT_EX(SOFTSIM_TRACE_HEADER, SOFTSIM_TRACE_DEBUG, __VA_ARGS__);}while(0)
#define SOFTSIM_LOG(...)							do{SOFTSIM_LOG_EX(SOFTSIM_MD_ID, SOFTSIM_TRACE_HEADER, __VA_ARGS__);SOFTSIM_TRACE_PRINT_EX(SOFTSIM_TRACE_HEADER, SOFTSIM_TRACE_DEBUG, __VA_ARGS__);}while(0)
#define SOFTSIM_DEBUG(...)						SOFTSIM_DEBUG_EX(SOFTSIM_MD_ID, SOFTSIM_TRACE_HEADER, __VA_ARGS__)
#define SOFTSIM_CRASH_MARK						SOFTSIM_CRASH("")
#define SOFTSIM_ERROR_MARK						SOFTSIM_ERROR("")
#define SOFTSIM_LOG_MARK						SOFTSIM_LOG("")
#define SOFTSIM_DEBUG_MARK						SOFTSIM_DEBUG("")	
#define SOFTSIM_PRINT_HEX 	SOFTSIM_TRACE_HEX
#define SOFTSIMTRACE	printf
#elif defined(__SOFTSIM_ANDROID_TRACE__)
#define LOG	"SoftSIMLogic"
#define SOFTSIMTRACE(...)	 __android_log_print(ANDROID_LOG_DEBUG,LOG,__VA_ARGS__) 
#define SOFTSIM_PRINT_HEX 	softsim_printf_hex
#define SOFTSIM_DEBUG_MARK						SOFTSIMTRACE("")
#define SOFTSIM_DEBUG	SOFTSIMTRACE
#else
#define SOFTSIM_TRACE_PRINT(...)
#define SOFTSIM_TRACE_HEX(header, data, data_len)
#define SOFTSIM_PRINT_HEX 	softsim_printf_hex
#define SOFTSIM_TRACE_BIN(file, data, data_len)
#define SOFTSIMTRACE(...)
#define SOFTSIM_CRASH(...)
#define SOFTSIM_ERROR(...)
#define SOFTSIM_LOG(...)
#define SOFTSIM_DEBUG(...)
#define SOFTSIM_DEBUG_MARK	
#endif

#ifdef __cplusplus
}
#endif

#endif /*__SOFTSIM_TRACE_H__*/

