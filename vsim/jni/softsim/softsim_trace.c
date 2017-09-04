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
 * Filename: softsim_trace.c
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
#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
/*****************************************************************************
* Define																	   
*****************************************************************************/

/*****************************************************************************
* Typedef																	   
*****************************************************************************/
	 
/*****************************************************************************
* Static Local Variable															  
*****************************************************************************/
static SoftsimTraceLevel softsim_current_trace_level = SOFTSIM_TRACE_LEVEL; 
static volatile  u8   softsim_debug_file_index = 0;
static SoftsimTraceLevel softsim_last_trace_level = SOFTSIM_TRACE_LEVEL;
static bool softsim_trace_is_init = false;

static const char *softsim_debug_file[] = 
{
	"softsim_debug1.txt",
	"softsim_debug2.txt",
	"softsim_debug3.txt",
	"softsim_debug4.txt",
	"softsim_debug5.txt",
	"softsim_debug6.txt",
	"softsim_debug7.txt",
	"softsim_debug8.txt",
	"softsim_debug9.txt",
	"softsim_debug10.txt",
	"softsim_debug11.txt",
};

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
#if defined(__SOFTSIM_TRACE__)
static void softsim_create_trace_dir(void)
{
	char trace_dir[SOFTSIM_FILE_PATH_LEN];
	char normal_drv[SOFTSIM_FILE_NAME_LEN];

	softsim_memset(trace_dir, 0, sizeof(trace_dir));
	softsim_memset(normal_drv, 0, sizeof(normal_drv));
	softsim_fs_get_normal_disk(normal_drv);
	softsim_sprintf(trace_dir, "%s/%s/", normal_drv, SOFTSIM_TRACE_DIR);
	softsim_fs_create_dir_rf(trace_dir);
}

static void softsim_trace_file_init(void)
{
	s32 handle;
	u32 index, file_len;
	char trace_file[SOFTSIM_FILE_PATH_LEN];
	char normal_drv[SOFTSIM_FILE_NAME_LEN];

	softsim_last_trace_level = SOFTSIM_TRACE_LEVEL;
	softsim_current_trace_level = SOFTSIM_TRACE_LEVEL;
	softsim_create_trace_dir();
	
	softsim_memset(normal_drv, 0, sizeof(normal_drv));
	softsim_fs_get_normal_disk(normal_drv);
	for(index = 0; index < SOFTSIM_DEBUG_FILE_NUM; index++)
	{
		softsim_memset(trace_file, 0, sizeof(trace_file));
		softsim_sprintf(trace_file, "%s/%s/%s", normal_drv, SOFTSIM_TRACE_DIR, softsim_debug_file[index]);
		handle = softsim_fs_open(trace_file,  SOFTSIM_FS_READ_ONLY);
		if (handle >= SOFTSIM_FS_NO_ERROR)
		{
			softsim_fs_length(handle, &file_len);
			softsim_fs_close(handle);
			if(file_len < SOFTSIM_TRACE_FILE_SIZE_MAX)
			{
				softsim_debug_file_index = index;
				break;
			}
		}
		else
		{
			softsim_debug_file_index = index;
			break;
		}
	}
	if(index >= SOFTSIM_DEBUG_FILE_NUM)
	{
		softsim_debug_file_index = 0;
		softsim_memset(trace_file, 0, sizeof(trace_file));
		softsim_sprintf(trace_file, "%s/%s/%s", normal_drv, SOFTSIM_TRACE_DIR, softsim_debug_file[0]);
		softsim_fs_delete(trace_file);
	}
	softsim_trace_is_init = true;	
}

static void softsim_write_trace_to_file(SoftsimTraceLevel level, char* buffer)
{
	s32 handle;
	u32 write_len, file_len;
	char trace_file[SOFTSIM_FILE_PATH_LEN];
	char normal_drv[SOFTSIM_FILE_NAME_LEN];

	if(!softsim_trace_is_init)
	{
		softsim_trace_file_init();
	}
	
	softsim_memset(trace_file, 0, sizeof(trace_file));
	softsim_memset(normal_drv, 0, sizeof(normal_drv));
	softsim_fs_get_normal_disk(normal_drv);
	softsim_sprintf(trace_file, "%s/%s/", normal_drv, SOFTSIM_TRACE_DIR);
	softsim_create_trace_dir();
	switch(level)
	{
		case SOFTSIM_TRACE_CRASH:
			softsim_strcat(trace_file, SOFTSIM_TRACE_CRASH_FILE);
			break;
		case SOFTSIM_TRACE_ERROR:
			softsim_strcat(trace_file, SOFTSIM_TRACE_ERROR_FILE);
			break;
		case SOFTSIM_TRACE_LOG:
			softsim_strcat(trace_file, SOFTSIM_TRACE_LOG_FILE);
			break;
		case SOFTSIM_TRACE_DEBUG:
			softsim_strcat(trace_file, softsim_debug_file[softsim_debug_file_index]);
			break;		
		default:
			return;
	}
	
	handle = softsim_fs_open(trace_file,  SOFTSIM_FS_CREATE | SOFTSIM_FS_READ_WRITE);
	if (handle >= SOFTSIM_FS_NO_ERROR)
	{
		softsim_fs_length(handle, &file_len);
		if(file_len >= SOFTSIM_TRACE_FILE_SIZE_MAX)
		{
			if(level == SOFTSIM_TRACE_DEBUG)
			{
				if(softsim_debug_file_index < (SOFTSIM_DEBUG_FILE_NUM-1))
				{
					softsim_debug_file_index++;
				}
				else
				{
					softsim_debug_file_index = 0;
				}
				softsim_memset(trace_file, 0, sizeof(trace_file));
				softsim_sprintf(trace_file, "%s/%s/%s", normal_drv, SOFTSIM_TRACE_DIR, softsim_debug_file[softsim_debug_file_index]);
			}
			softsim_fs_close(handle);
			handle = softsim_fs_open(trace_file,  SOFTSIM_FS_CREATE_ALWAYS | SOFTSIM_FS_READ_WRITE);
		}
		
		if(handle >= SOFTSIM_FS_NO_ERROR)
		{
			softsim_fs_seek(handle, 0, SOFTSIM_FS_SET_END);
			softsim_fs_write(handle, buffer, softsim_strlen(buffer), &write_len);
			softsim_fs_close(handle);
		}
	}
}

static void softsim_write_trace_to_uart(char* buffer)
{
	//val_uart_trace_print(buffer);
	SOFTSIMTRACE("%s", buffer);
}
#else
static void softsim_create_trace_dir(void)
{

}

static void softsim_trace_file_init(void)
{

}

static void softsim_write_trace_to_file(SoftsimTraceLevel level, char* buffer)
{

}

static void softsim_write_trace_to_uart(char* buffer)
{
	SOFTSIMTRACE("%s", buffer);
}
#endif


void softsim_trace_printf(SoftsimTraceLevel level, char *file, char *function, u32 line, const char *fmt, ...)
{
	va_list arglist;
	char trace_buffer[SOFTSIM_TRACE_BUFFER_SIZE];
	u32 msec, buf_len;
	SoftsimDateTime time;
		
	if(level > softsim_current_trace_level)
	{
		return;
	}

	msec = softsim_get_system_msec();
	//softsim_get_system_time(&time);
	softsim_get_local_time(&time);
	
	softsim_memset(trace_buffer, 0, sizeof(trace_buffer));
	softsim_sprintf(trace_buffer, "[default]\t%04d-%02d-%02d %02d:%02d:%02d.%06d\t%d\t%s\t%s\tline:%d\t", 
		time.year, time.month, time.day, time.hour, time.minute, time.second, time.usecond, msec, file, function,line);
	buf_len = (sizeof(trace_buffer)-softsim_strlen(trace_buffer)-softsim_strlen("\r\n")-1);
	va_start(arglist, fmt);
	vsnprintf(trace_buffer + softsim_strlen(trace_buffer), buf_len, fmt, arglist);
	va_end(arglist);
	softsim_strcat(trace_buffer,"\r\n");	

	softsim_write_trace_to_uart(trace_buffer);
	softsim_write_trace_to_file(level, trace_buffer);
}

void softsim_trace_printf_ex(char *module, SoftsimTraceLevel level, char *file, char *function, u32 line, const char *fmt, ...)
{
	va_list arglist;
	char trace_buffer[SOFTSIM_TRACE_BUFFER_SIZE];
	u32 msec, buf_len;
	SoftsimDateTime time;
		
	if(level > softsim_current_trace_level)
	{
		return;
	}
	
	msec = softsim_get_system_msec();
	//softsim_get_system_time(&time);
	softsim_get_local_time(&time);
	
	softsim_memset(trace_buffer, 0, sizeof(trace_buffer));
	softsim_sprintf(trace_buffer, "%s\t%04d-%02d-%02d %02d:%02d:%02d.%06d\t%d\t%s\t%s\tline:%d\t", 
		module, time.year, time.month, time.day, time.hour, time.minute, time.second, time.usecond, msec, file, function,line);
	buf_len = (sizeof(trace_buffer)-softsim_strlen(trace_buffer)-softsim_strlen("\r\n")-1);
	va_start(arglist, fmt);
	vsnprintf(trace_buffer+softsim_strlen(trace_buffer), buf_len,fmt, arglist);
	va_end(arglist);
	softsim_strcat(trace_buffer,"\r\n");

	softsim_write_trace_to_uart(trace_buffer);
	softsim_write_trace_to_file(level, trace_buffer);
}

void softsim_trace_printf_hex(SoftsimTraceLevel level, char *file, char *function, u32 line, char *header, u8 *data, u32 data_len)
{
	//char trace_buffer[VAL_TRACE_BUFFER_SIZE];
	char *trace_buffer;
	u32 msec, buf_len;
	SoftsimDateTime time;
	u32 index, trace_buffer_len = 0;
	
	if(level > softsim_current_trace_level)
	{
		return;
	}

	trace_buffer_len = (SOFTSIM_TRACE_BUFFER_SIZE + 6*data_len);
	trace_buffer = malloc(trace_buffer_len);
	if(trace_buffer == NULL)
		return;
	
	msec = softsim_get_system_msec();
	//softsim_get_system_time(&time);
	softsim_get_local_time(&time);
	
	softsim_memset(trace_buffer, 0, trace_buffer_len);
	softsim_sprintf(trace_buffer, "[HEX]\t%04d-%02d-%02d %02d:%02d:%02d.%06d\t%d\t%s\t%s\tline:%d\t", 
		time.year, time.month, time.day, time.hour, time.minute, time.second, time.usecond, msec, file, function,line);
	buf_len = (trace_buffer_len-softsim_strlen(trace_buffer)-softsim_strlen("[]\r\n")-1);
	softsim_strncpy(trace_buffer+softsim_strlen(trace_buffer), header, buf_len);
	softsim_sprintf(trace_buffer+softsim_strlen(trace_buffer), " %d", data_len);
	softsim_strcat(trace_buffer,"[");
	for(index = 0; index < data_len; index ++)
	{
		char format[8];

		buf_len = (trace_buffer_len - softsim_strlen("0xff, ]\r\n") -1);
		if(softsim_strlen(trace_buffer) >= buf_len)
		{
			break;
		}
		softsim_memset(format, 0, sizeof(format));
		softsim_sprintf(format, "0x%02X, ", data[index]);
		softsim_strcat(trace_buffer, format);
		
	}
	softsim_strcat(trace_buffer,"]\r\n");

	softsim_write_trace_to_uart(trace_buffer);
	softsim_write_trace_to_file(level, trace_buffer);
	free(trace_buffer);
}

void softsim_trace_printf_bin(SoftsimTraceLevel level, char *file, u8 *data, u32 data_len)
{
	
}

void softsim_printf_hex(char*msg, u8 *hex, u32 hex_len)
{
	static char buf[1024];
	s32 buf_index, hex_index;

	buf_index = 0;
	hex_index = 0;
	
	buf_index +=softsim_sprintf(&buf[buf_index], "%s %d[", msg, hex_len);
	while(hex_index < hex_len)
	{
		if((buf_index + 64) >= 1024)
		{
			break;
		}

		buf_index += softsim_sprintf(&buf[buf_index], "0x%02X, ", hex[hex_index]);
		hex_index ++;
	}
	buf_index += softsim_sprintf(&buf[buf_index], "]\r\n");
	buf[buf_index] = '\0';
	
	SOFTSIMTRACE("%s", buf);
}


void softsim_trace_init(void)
{
#if defined(__SOFTSIM_TRACE__)
	softsim_trace_file_init();
	softsim_fs_system_init();
#endif
}

void softsim_trace_exit(void)
{

}
