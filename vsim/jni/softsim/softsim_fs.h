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
 * Filename: softsim_fs.h
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-08-13
 ******************************************************************************/
#ifndef __SOFTSIM_FS_H__
#define __SOFTSIM_FS_H__

#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************
* Include																	   
*****************************************************************************/
#include "softsim_macro.h"
#include "softsim_type.h"
#if defined(__SOFTSIM_TRACE__)
/*****************************************************************************
* Define																	   
*****************************************************************************/
#define SOFTSIM_FS_READ_WRITE					0x00000000L
#define SOFTSIM_FS_READ_ONLY					0x00000100L
#define SOFTSIM_FS_OPEN_SHARED          		0x00000200L
#define SOFTSIM_FS_OPEN_NO_DIR          		0x00000400L
#define SOFTSIM_FS_OPEN_DIR						0x00000800L
#define SOFTSIM_FS_CREATE						0x00010000L
#define SOFTSIM_FS_CREATE_ALWAYS				0x00020000L
#define SOFTSIM_FS_NONBLOCK_MODE        		0x10000000L

#define SOFTSIM_FS_ATTR_UNKNOW					0x00
#define SOFTSIM_FS_ATTR_READ_ONLY				0x01
#define SOFTSIM_FS_ATTR_HIDDEN					0x02
#define SOFTSIM_FS_ATTR_SYSTEM					0x04
#define SOFTSIM_FS_ATTR_VOLUME					0x08
#define SOFTSIM_FS_ATTR_DIR						0x10
#define SOFTSIM_FS_ATTR_ARCHIVE					0x20 
/*****************************************************************************
* Typedef																	   
*****************************************************************************/
typedef enum
{
	SOFTSIM_FS_NO_ERROR 					=	 0,    /* No error */
	SOFTSIM_FS_ERROR_RESERVED				=	-1,    /* Reserved error, not used currently */
	SOFTSIM_FS_PARAM_ERROR					=	-2,    /* User */
	SOFTSIM_FS_INVALID_FILENAME 			=	-3,    /* User */
	SOFTSIM_FS_DRIVE_NOT_FOUND				=	-4,    /* User or Fatal */
	SOFTSIM_FS_TOO_MANY_FILES				=	-5,    /* User or Normal: use over max file handle number or more than 256 files in sort */
	SOFTSIM_FS_NO_MORE_FILES				=	-6,    /* Normal */
	SOFTSIM_FS_WRONG_MEDIA					=	-7,    /* Fatal */
	SOFTSIM_FS_INVALID_FILE_SYSTEM			=	-8,    /* Fatal */
	SOFTSIM_FS_FILE_NOT_FOUND				=	-9,    /* User or Normal */
	SOFTSIM_FS_INVALID_FILE_HANDLE			=  -10,    /* User or Normal */
	SOFTSIM_FS_UNSUPPORTED_DEVICE			=  -11,    /* User */
	SOFTSIM_FS_UNSUPPORTED_DRIVER_FUNCTION	=  -12,    /* User or Fatal */
	SOFTSIM_FS_CORRUPTED_PARTITION_TABLE	=  -13,    /* fatal */
	SOFTSIM_FS_TOO_MANY_DRIVES				=  -14,    /* not use so far */
	SOFTSIM_FS_INVALID_FILE_POS 			=  -15,    /* User */
	SOFTSIM_FS_ACCESS_DENIED				=  -16,    /* User or Normal */
	SOFTSIM_FS_STRING_BUFFER_TOO_SMALL		=  -17,    /* User */
	SOFTSIM_FS_GENERAL_FAILURE				=  -18,    /* Normal */
	SOFTSIM_FS_PATH_NOT_FOUND				=  -19,    /* User */
	SOFTSIM_FS_FAT_ALLOC_ERROR				=  -20,    /* Fatal: disk crash */
	SOFTSIM_FS_ROOT_DIR_FULL				=  -21,    /* Normal */
	SOFTSIM_FS_DISK_FULL					=  -22,    /* Normal */
	SOFTSIM_FS_TIMEOUT						=  -23,    /* Normal: FS_CloseMSDC with nonblock */
	SOFTSIM_FS_BAD_SECTOR					=  -24,    /* Normal: NAND flash bad block */
	SOFTSIM_FS_DATA_ERROR					=  -25,    /* Normal: NAND flash bad block */
	SOFTSIM_FS_MEDIA_CHANGED				=  -26,    /* Normal */
	SOFTSIM_FS_SECTOR_NOT_FOUND 			=  -27,    /* Fatal */
	SOFTSIM_FS_ADDRESS_MARK_NOT_FOUND		=  -28,    /* not use so far */
	SOFTSIM_FS_DRIVE_NOT_READY				=  -29,    /* Normal */
	SOFTSIM_FS_WRITE_PROTECTION 			=  -30,    /* Normal: only for MSDC */
	SOFTSIM_FS_DMA_OVERRUN					=  -31,    /* not use so far */
	SOFTSIM_FS_CRC_ERROR					=  -32,    /* not use so far */
	SOFTSIM_FS_DEVICE_RESOURCE_ERROR		=  -33,    /* Fatal: Device crash */
	SOFTSIM_FS_INVALID_SECTOR_SIZE			=  -34,    /* Fatal */
	SOFTSIM_FS_OUT_OF_BUFFERS				=  -35,    /* Fatal */
	SOFTSIM_FS_FILE_EXISTS					=  -36,    /* User or Normal */
	SOFTSIM_FS_LONG_FILE_POS				=  -37,    /* User : FS_Seek new pos over sizeof int */
	SOFTSIM_FS_FILE_TOO_LARGE				=  -38,    /* User: filesize + pos over sizeof int */
	SOFTSIM_FS_BAD_DIR_ENTRY				=  -39,    /* Fatal */
	SOFTSIM_FS_ATTR_CONFLICT				=  -40,    /* User: Can't specify FS_PROTECTION_MODE and FS_NONBLOCK_MOD */
	SOFTSIM_FS_CHECKDISK_RETRY				=  -41,    /* System: don't care */
	SOFTSIM_FS_LACK_OF_PROTECTION_SPACE 	=  -42,    /* Fatal: Device crash */
	SOFTSIM_FS_SYSTEM_CRASH 				=  -43,    /* Normal */
	SOFTSIM_FS_FAIL_GET_MEM 				=  -44,    /* Normal */
	SOFTSIM_FS_READ_ONLY_ERROR				=  -45,    /* User or Normal */
	SOFTSIM_FS_DEVICE_BUSY					=  -46,    /* Normal */
	SOFTSIM_FS_ABORTED_ERROR				=  -47,    /* Normal */
	SOFTSIM_FS_QUOTA_OVER_DISK_SPACE		=  -48,    /* Normal: Configuration Mistake */
	SOFTSIM_FS_PATH_OVER_LEN_ERROR			=  -49,    /* Normal */
	SOFTSIM_FS_APP_QUOTA_FULL				=  -50,    /* Normal */
	SOFTSIM_FS_VF_MAP_ERROR 				=  -51,    /* User or Normal */
	SOFTSIM_FS_DEVICE_EXPORTED_ERROR		=  -52,    /* User or Normal */
	SOFTSIM_FS_DISK_FRAGMENT				=  -53,    /* Normal */
	SOFTSIM_FS_DIRCACHE_EXPIRED 			=  -54,    /* Normal */
	SOFTSIM_FS_QUOTA_USAGE_WARNING			=  -55,    /* Normal or Fatal: System Drive Free Space Not Enought */
	SOFTSIM_FS_ERR_DIRDATA_LOCKED			=  -56,    /* Normal */
	SOFTSIM_FS_INVALID_OPERATION			=  -57,    /* Normal */
	SOFTSIM_FS_ERR_VF_PARENT_CLOSED 		=  -58,    /* Virtual file's parent is closed */
	SOFTSIM_FS_ERR_UNSUPPORTED_SERVICE		=  -59,    /* The specified FS service is closed in this project. */
	
	SOFTSIM_FS_ERR_INVALID_JOB_ID			=  -81,
	
	SOFTSIM_FS_MSDC_MOUNT_ERROR 			= -100,    /* Normal */
	SOFTSIM_FS_MSDC_READ_SECTOR_ERROR		= -101,    /* Normal */
	SOFTSIM_FS_MSDC_WRITE_SECTOR_ERROR		= -102,    /* Normal */
	SOFTSIM_FS_MSDC_DISCARD_SECTOR_ERROR	= -103,    /* Normal */
	SOFTSIM_FS_MSDC_PRESNET_NOT_READY		= -104,    /* System */
	SOFTSIM_FS_MSDC_NOT_PRESENT 			= -105,    /* Normal */
	
	SOFTSIM_FS_EXTERNAL_DEVICE_NOT_PRESENT	= -106,    /* Normal */
	SOFTSIM_FS_HIGH_LEVEL_FORMAT_ERROR		= -107,    /* Normal */
	
	SOFTSIM_FS_CARD_BATCHCOUNT_NOT_PRESENT	= -110,    /* Normal */
	
	SOFTSIM_FS_FLASH_MOUNT_ERROR			= -120,    /* Normal */
	SOFTSIM_FS_FLASH_ERASE_BUSY 			= -121,    /* Normal: only for nonblock mode */
	SOFTSIM_FS_NAND_DEVICE_NOT_SUPPORTED	= -122,    /* Normal: Configuration Mistake */
	SOFTSIM_FS_FLASH_OTP_UNKNOWERR			= -123,    /* User or Normal */
	SOFTSIM_FS_FLASH_OTP_OVERSCOPE			= -124,    /* User or Normal */
	SOFTSIM_FS_FLASH_OTP_WRITEFAIL			= -125,    /* User or Normal */
	SOFTSIM_FS_FDM_VERSION_MISMATCH 		= -126,    /* System */
	SOFTSIM_FS_FLASH_OTP_LOCK_ALREADY		= -127,    /* User or Normal */
	SOFTSIM_FS_FDM_FORMAT_ERROR 			= -128,    /* The format of the disk content is not correct */
	
	SOFTSIM_FS_FDM_USER_DRIVE_BROKEN		= -129,    /*User drive unrecoverable broken*/
	SOFTSIM_FS_FDM_SYS_DRIVE_BROKEN 		= -130,    /*System drive unrecoverable broken*/
	SOFTSIM_FS_FDM_MULTIPLE_BROKEN			= -131,    /*multiple unrecoverable broken*/
	
	SOFTSIM_FS_LOCK_MUTEX_FAIL				= -141,    /* System: don't care */
	SOFTSIM_FS_NO_NONBLOCKMODE				= -142,    /* User: try to call nonblock mode other than NOR flash */
	SOFTSIM_FS_NO_PROTECTIONMODE			= -143,    /* User: try to call protection mode other than NOR flash */
	/*
	* If disk size exceeds FS_MAX_DISK_SIZE (unit is KB, defined in makefile),
	* FS_TestMSDC(), FS_GetDevStatus(FS_MOUNT_STATE_ENUM) and all access behaviors will
	* get this error code.
	*/
	SOFTSIM_FS_DISK_SIZE_TOO_LARGE			= (SOFTSIM_FS_MSDC_MOUNT_ERROR),	 /*Normal*/
	
	SOFTSIM_FS_MINIMUM_ERROR_CODE			= -65536 /* 0xFFFF0000 */
}SoftsimFileErrorEnum;
	
typedef enum
{
	SOFTSIM_FS_SET_BEGIN,
	SOFTSIM_FS_SET_CURRENT,
	SOFTSIM_FS_SET_END
}SoftsimFileSeekWhence;
	
typedef struct
{
	char attr;
	u32 file_size;
	u64 create_time;	//TODO:
	u64 access_time;	//TODO:
	u64 modify_time;	//TODO:
	char file_name[SOFTSIM_FILE_NAME_LEN]; //file_name
}SoftsimFileAttrInfo;

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
extern s32 softsim_fs_open(char* utf8_name, u32 flags); 	/* notic:File path just like "d:/dir/file" */
extern s32 softsim_fs_delete(char* utf8_name);
extern s32 softsim_fs_close(s32 handle);
extern s32 softsim_fs_read(s32 handle, void * data, u32 data_len, u32* read_len);
extern s32 softsim_fs_write(s32 handle, void * data, u32 data_len, u32* write_len);
extern s32 softsim_fs_seek(s32 handle, s32 offset, s32 whence);
extern s32 softsim_fs_length(s32 handle, u32* out_len);
	
extern s32 softsim_fs_rename(char* old_name, char* new_name);
	
extern s32 softsim_fs_create_dir(char* utf8_name);
extern s32 softsim_fs_delete_dir(char* utf8_name);
	
extern s32 softsim_fs_create_dir_rf(char* utf8_name);	/*This function provides a facility to create a folder recursively*/
extern s32 softsim_fs_delete_dir_rf(char* utf8_name);	/*This function provides a facility to delete a folder recursively, must end by '/'*/
	
extern bool softsim_fs_file_is_exist(char* utf8_name);
extern bool softsim_fs_dir_is_exist(char* utf8_name);
	
extern s32 softsim_fs_get_normal_disk(char* utf8_name);
extern s32 softsim_fs_get_system_disk(char* utf8_name);
extern s32 softsim_fs_get_position(s32 handle, u32* position);
	
/*
extern s32 softsim_fs_get_attribute(char* utf8_name, SoftsimFileAttrInfo* info);
extern s32 softsim_fs_findfirst(char* pattern, SoftsimFileAttrInfo* info);
extern s32 softsim_fs_findnext(s32 handle, SoftsimFileAttrInfo* info);
extern s32 softsim_fs_findclose(s32 handle);
*/
	
extern void softsim_fs_system_init(void);
extern void softsim_fs_system_exit(void);

/****************************************************************************
* Function
****************************************************************************/
#endif/*__SOFTSIM_TRACE__*/

#ifdef __cplusplus
		}
#endif

#endif /*__SOFTSIM_FS_H__*/
