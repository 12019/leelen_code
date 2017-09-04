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
 * Filename: softsim_fs_linux.c
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
#if defined(__SOFTSIM_TRACE__)
#include "softsim_fs_linux.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

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
static s32 softsim_fs_errno_to_errcode(s32 result)
{
	s32 err = SOFTSIM_FS_ERROR_RESERVED;
	
	switch(result)
	{
		case ENOENT:
			err = SOFTSIM_FS_FILE_NOT_FOUND;
			break;
		case EBADF:
			err = SOFTSIM_FS_INVALID_FILE_HANDLE;
			break;
		case EEXIST:
			err = SOFTSIM_FS_FILE_EXISTS;
			break;
		case EMFILE:
			err = SOFTSIM_FS_TOO_MANY_FILES;
			break;
		case EFBIG:
			err = SOFTSIM_FS_FILE_TOO_LARGE;
			break;
		case EROFS:
			err = SOFTSIM_FS_READ_ONLY_ERROR;
			break;
		case ENOMEM:
			err = SOFTSIM_FS_FAIL_GET_MEM;
			break;
		case ENAMETOOLONG:
			err = SOFTSIM_FS_INVALID_FILENAME;
			break;			
		default:
			printf("file other error reason: %s\n", strerror(result));
			break;
	}

	return err;
}

s32 softsim_fs_open(char* utf8_name, u32 flags)
{	
	s32 read_flags = 0;
	s32 mode = 0;
	s32 ret;

	if(flags & SOFTSIM_FS_READ_WRITE)
	{
		read_flags |= O_RDWR;
	}
	
	if(flags & SOFTSIM_FS_READ_ONLY)
	{
		read_flags |= O_RDONLY;
	}
	
	if(flags & SOFTSIM_FS_OPEN_DIR)
	{
		read_flags |= O_DIRECTORY;
	}
	
	if(flags & SOFTSIM_FS_CREATE)
	{
		read_flags |= O_CREAT|O_RDWR;
		mode = S_IRUSR | S_IWUSR;
	}
	
	if(flags & SOFTSIM_FS_CREATE_ALWAYS)
	{
		read_flags |= O_CREAT|O_RDWR|O_TRUNC;
		mode = S_IRUSR | S_IWUSR;
	}
	
	if(flags & SOFTSIM_FS_ATTR_HIDDEN)
	{
		//TODO
	}

	ret = open(utf8_name, read_flags, mode);
	if(ret == EOF)
	{
		ret = softsim_fs_errno_to_errcode(errno);
	}
	
	return ret;
}

s32 softsim_fs_delete(char* utf8_name)
{
	s32 ret;
	
	ret = remove(utf8_name);
	if(ret == EOF)
	{
		ret = softsim_fs_errno_to_errcode(errno);
	}
	else
	{
		ret = SOFTSIM_FS_NO_ERROR;
	}
	
	return ret;
}

s32 softsim_fs_close(s32 handle)
{
	s32 ret;
	
	ret = close(handle);
	if(ret == EOF)
	{
		ret = softsim_fs_errno_to_errcode(errno);
	}
	else
	{
		ret = SOFTSIM_FS_NO_ERROR;
	}
	
	return ret;
}

s32 softsim_fs_read(s32 handle, void * data, u32 data_len, u32* read_len)
{
	s32 ret;
	
	ret = read(handle, data, data_len);
	if(ret == EOF)
	{
		*read_len = 0;
		ret = softsim_fs_errno_to_errcode(errno);
	}
	else
	{
		*read_len = ret;
		ret = SOFTSIM_FS_NO_ERROR;
	}
	
	return ret;
}

s32 softsim_fs_write(s32 handle, void * data, u32 data_len, u32* write_len)
{
	s32 ret;
	
	ret = write(handle, data, data_len);
	if(ret == EOF)
	{
		*write_len = 0;
		ret = softsim_fs_errno_to_errcode(errno);
	}
	else
	{
		*write_len = ret;
		ret = SOFTSIM_FS_NO_ERROR;
	}
	
	return ret;
}

s32 softsim_fs_seek(s32 handle, s32 offset, s32 whence)
{
	s32 ret;
	s32 fromwhere;

	switch(whence)
	{
		case SOFTSIM_FS_SET_BEGIN:
			fromwhere = SEEK_SET;
			break;
		case SOFTSIM_FS_SET_CURRENT:
			fromwhere = SEEK_CUR;
			break;
		case SOFTSIM_FS_SET_END:
			fromwhere = SEEK_END;
			break;
		default:
			return SOFTSIM_FS_PARAM_ERROR;
	}
	ret = lseek(handle, offset, fromwhere);
	if(ret == EOF)
	{
		ret = softsim_fs_errno_to_errcode(errno);
	}

	return ret;
}

s32 softsim_fs_length(s32 handle, u32* out_len)
{
	u32 start, end;
	s32 ret;
	
	start = lseek(handle, 0, SEEK_SET);
	end = lseek(handle, 0, SEEK_END);
	ret = lseek(handle, 0, SEEK_SET);
	if(ret == EOF)
	{
		ret = softsim_fs_errno_to_errcode(errno);
	}
	else
	{
		*out_len = end-start;
		ret = SOFTSIM_FS_NO_ERROR;
	}

	return ret;
}


s32 softsim_fs_get_position(s32 handle, u32* position)
{
	s32 ret;
	
	ret = lseek(handle, 0, SEEK_SET);
	if(ret == EOF)
	{
		ret = softsim_fs_errno_to_errcode(errno);
	}
	else
	{
		*position = (u32)ret;
		ret = SOFTSIM_FS_NO_ERROR;
	}

	return ret;
}


s32 softsim_fs_rename(char* old_name, char* new_name)
{
	s32 ret;
	
	ret = rename(old_name, new_name);
	if(ret == EOF)
	{
		ret = softsim_fs_errno_to_errcode(errno);
	}
	else
	{
		ret = SOFTSIM_FS_NO_ERROR;
	}
	
	return ret;
}

s32 softsim_fs_create_dir(char* utf8_name)
{
	s32 ret;

	ret = mkdir(utf8_name, S_IRWXU);
	if(ret == EOF)
	{
		ret = softsim_fs_errno_to_errcode(errno);
	}
	else
	{
		ret = SOFTSIM_FS_NO_ERROR;
	}
	return ret;
}

s32 softsim_fs_delete_dir(char* utf8_name)
{
	return softsim_fs_delete(utf8_name);
}

s32 softsim_fs_create_dir_rf(char* utf8_name)
{
	s32 ret = SOFTSIM_FS_NO_ERROR;
	u32 i;
	char name_clone[SOFTSIM_FILE_PATH_LEN];

	if(utf8_name == NULL)
	{
		return SOFTSIM_FS_PARAM_ERROR;
	}
	
	softsim_memset(name_clone, 0, sizeof(name_clone));
	softsim_strncpy(name_clone, utf8_name, SOFTSIM_FILE_PATH_LEN);
	for(i = 1; name_clone[i] != 0; i++)
	{
		if(name_clone[i] == '/')
		{
			char tmp;
			
			tmp = name_clone[i+1];
			name_clone[i+1] = 0;
			ret = softsim_fs_create_dir(name_clone);
			name_clone[i+1] = tmp;
		}
	}
	return ret;
}

s32 softsim_fs_delete_dir_rf(char* utf8_name)
{
	s32 ret = SOFTSIM_FS_NO_ERROR;
	DIR *pDir = NULL;
	struct dirent *p_dirent = NULL;
	char file_path[SOFTSIM_FILE_PATH_LEN];
	
	if(utf8_name == NULL)
	{
		return SOFTSIM_FS_PARAM_ERROR;
	}
	if ((pDir = opendir(utf8_name)) != NULL)
	{
		while((p_dirent = readdir(pDir)) != NULL)
		{
			if ((softsim_strcmp(p_dirent->d_name, ".") != 0)&&(softsim_strcmp(p_dirent->d_name, "..") != 0))
			{
				if((softsim_strlen(p_dirent->d_name)+softsim_strlen(utf8_name)) >= SOFTSIM_FILE_PATH_LEN)
				{
					return SOFTSIM_FS_INVALID_FILENAME;
				}
				softsim_memset(file_path, 0, sizeof(file_path));
				if(p_dirent->d_type == DT_DIR)
				{
					softsim_sprintf(file_path, "%s/%s/", utf8_name, p_dirent->d_name);
					softsim_fs_delete_dir_rf(file_path);
				}
				else
				{
					softsim_sprintf(file_path, "%s/%s", utf8_name, p_dirent->d_name);
				}
				ret = softsim_fs_delete(file_path);
			}
		}
		ret = remove(utf8_name);
		closedir(pDir);
	}
	
	return ret;
}

bool softsim_fs_file_is_exist(char* utf8_name)
{
	s32 ret;
	bool is_exist = false;
	struct stat stat_info;

	softsim_memset(&stat_info, 0, sizeof(stat_info));
	ret = stat(utf8_name, &stat_info);
	if((ret != EOF)&&(!(stat_info.st_mode&O_DIRECTORY)))
	{
		is_exist = true;
	}

	return is_exist;
}

bool softsim_fs_dir_is_exist(char* utf8_name)
{
	s32 ret;
	bool is_exist = false;
	struct stat stat_info;

	softsim_memset(&stat_info, 0, sizeof(stat_info));
	ret = stat(utf8_name, &stat_info);
	if((ret != EOF)&&(stat_info.st_mode&O_DIRECTORY))
	{
		is_exist = true;
	}

	return is_exist;
}

//System disk: mtk like "c:", linux like "/sdcard"
s32 softsim_fs_get_normal_disk(char* utf8_name)
{
	if(utf8_name == NULL)
	{
		return SOFTSIM_FS_PARAM_ERROR;
	}
	softsim_strcpy(utf8_name,  SOFTSIM_LINUX_NORMAL_DISK);
	return SOFTSIM_FS_NO_ERROR;
}

s32 softsim_fs_get_system_disk(char* utf8_name)
{
	if(utf8_name == NULL)
	{
		return SOFTSIM_FS_PARAM_ERROR;
	}
	softsim_strcpy(utf8_name,  SOFTSIM_LINUX_SYSTEM_DISK);
	return SOFTSIM_FS_NO_ERROR;
}

s32 softsim_fs_get_attribute(char* utf8_name, SoftsimFileAttrInfo* info)
{
	//TODO
	return SOFTSIM_FS_ERROR_RESERVED;
}

s32 softsim_fs_findfirst(char* pattern, SoftsimFileAttrInfo* info)
{
#if 0
	char *path = NULL;
	char *temp = NULL;
	u32 len;
	s32 ret;
	DIR* p_dir = NULL;
	struct dirent *p_dirent = NULL;
	struct stat stat_info;

	if((pattern == NULL)||(info == NULL))
	{
		return SOFTSIM_FS_PARAM_ERROR;
	}
	
	len = val_strlen(pattern)+1;
	path =val_malloc(len);
	
	if(path == NULL)
	{
		return SOFTSIM_FS_FAIL_GET_MEM;
	}
	
	val_strcpy(path, pattern);
	temp = val_strchr(path, '/');
	if(temp != NULL)
	{
		temp[1] = 0x00;
	}
	else
	{
		return SOFTSIM_FS_PARAM_ERROR;
	}
	
	p_dir = opendir(pattern);
	if(p_dir == NULL)
	{
		val_free(path);
		return _val_fs_errno_to_errcode(errno);
	}
	p_dirent = readdir(p_dir));
	if(p_dirent == NULL)
	{
		val_free(path);
		return  _val_fs_errno_to_errcode(errno);
	}

	ret = (s32)p_dir;
	if((val_strlen(p_dirent->d_name)+) >= STP_FILE_PATH_LEN)
	{
		val_free(path);
		return VAL_FS_INVALID_FILENAME;
	}
	val_memset(info, 0, sizeof(FileAttrInfo));
	val_memset(&stat_info, 0, sizeof(stat_info));
	val_strncpy(info->file_path, path, sizeof(info->file_path)-1);
	val_strcat(info->file_path, p_dirent->d_name);
	stat(info->file_path, &stat_info);
	info->file_size = stat_info.st_size;
	if(stat_info.st_mode&S_IFDIR)
	{
		info->attr |= VAL_FS_ATTR_DIR;
	}
#endif	
	return SOFTSIM_FS_ERROR_RESERVED;
}

s32 softsim_fs_findnext(s32 handle, SoftsimFileAttrInfo* info)
{
	/*
	char *path = NULL;
	char *temp = NULL;
	u32 len;
	s32 ret;
	DIR* p_dir = (DIR*)handle;
	struct dirent *p_dirent = NULL;
	struct stat stat_info;

	//TODO;
	return ret;
	*/
	return SOFTSIM_FS_ERROR_RESERVED;
}

s32 softsim_fs_findclose(s32 handle)
{
	s32 ret = 0;
 #if 0
	DIR *p_dir = (DIR*)handle;
	
	ret = closedir(p_dir);
	if(ret == EOF)
	{
		ret = softsim_fs_errno_to_errcode(errno);
	}
	else
	{
		ret = SOFTSIM_FS_NO_ERROR;
	}
#endif
	return ret;
}

void softsim_fs_system_init(void)
{
	char system_disk[SOFTSIM_FILE_PATH_LEN];
	char normal_disk[SOFTSIM_FILE_PATH_LEN];

	softsim_memset(system_disk, 0, sizeof(system_disk));
	softsim_memset(normal_disk, 0, sizeof(normal_disk));
	softsim_fs_get_system_disk(system_disk);
	softsim_fs_create_dir_rf(system_disk);
	softsim_fs_get_normal_disk(system_disk);
	softsim_fs_create_dir_rf(system_disk);
}

void softsim_fs_system_exit(void)
{

}
#endif /*__SOFTSIM_TRACE__*/
