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
 * Filename: softsim_console.h
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-07-14
 ******************************************************************************/

/*****************************************************************************
* Include																	   
*****************************************************************************/
#include "softsim_include.h"
#include <dlfcn.h>
#include <pthread.h>  
#include <string.h>  
#include <sys/types.h>  
#include <unistd.h>
#include <fcntl.h>

/*****************************************************************************
* Define																	   
*****************************************************************************/
#undef 	SOFTSIM_MD_ID
#undef 	SOFTSIM_TRACE_HEADER
#define SOFTSIM_MD_ID					SOFTSIM_MD_CONSOLE
#define SOFTSIM_TRACE_HEADER			"[CONSOLE]" 
	
	
#define TGT_IMSI	"460019723702105"
#define TGT_IMSI2	"460019723702105"
#define	TGT_ICCID	"89860115851038343371"
#define	TGT_MSISDN "85255697360"
#define	TGT_PIN1 	"0000"
#define	TGT_PUK1 	"16417942"
#define	TGT_PIN2 	"0000"
#define	TGT_PUK2 	"77497605"
#define	TGT_K 		"00000000000000000000000000000000" 
#define	TGT_ADM1 	"44001591"
#define	TGT_ACC1 	"0001"
#define	TGT_ACC2 	"0001"
#define	TGT_OPC 	"00000000000000000000000000000000"
#define TGT_APN		"3g.net"
#define TGT_SPN		"TGT"
#define TEST_OPLMNLIST	"460-01"
#define TEST_FPLMNLIST	"460-00,460-03"	 

#define SOFTSIM_CHECK_TIMEOUT	(1*1000)

#define SOFTSIM_CHECK_MAX_COUNT	(14)

#if defined(__SOFTSIM_TEST__)
#define SOFTSIM_HPLMN_TEST	"46011"
#endif
/*****************************************************************************
* Typedef																	   
*****************************************************************************/

/*****************************************************************************
* Static Local Variable															  
*****************************************************************************/
static s32 psim_channel_status = 0;
static s32 softsim_channel_status = 0;
static s32 channel_status_check_timer_id = -1;
static s32 softsim_stop_timer_id = -1;
static s32 softsim_atr_check_timer_id = -1;
static s32 softsim_session_status_check_timer_id = -1;
static volatile u8 softsim_session_check_count = 0;
static volatile u8 softsim_check_count = 0;
static SoftsimFuncPtr softsim_begin = NULL;
static SoftsimFuncPtr softsim_auth_session_destory = NULL;
static SoftsimFuncPtr softsim_auth_channel_switch = NULL;
static volatile bool softsim_running = false;
static volatile bool softsim_stopping = false;
static u8 current_channel = 0xff;
static u8 last_channel = 0xff;
static SoftsimInformation softsim_information;
static bool vsim_data_load = false;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_rwlock_t rwlock;

static SoftsimProjectEnum project_type = 0xff;
/*****************************************************************************
* Global Variable														 
*****************************************************************************/

/*****************************************************************************
* Extern Global Variable															
*****************************************************************************/

/*****************************************************************************
* Static Local Function															  
*****************************************************************************/
static bool softsim_plug_in(void);
static void softsim_plug_out(void);
static void softsim_channel_status_check_callback(void);
static void softsim_channel_status_check(void);
static void softsim_atr_status_check(void);
static s32 softsim_information_update(char*ssid, char *iccid, char *imsi, char *plmn_list, char *fplmn_list, char *mnc_len, s32 sim_id, char *ip, s32 port);
static s32 softsim_data_custom(char*iccid, char*imsi);
static void softsim_chmod_update(void);
static void softsim_current_channel_update(void);
/*****************************************************************************
* Global Function														
*****************************************************************************/
	 
/*****************************************************************************
* Extern Global Function													  
*****************************************************************************/
extern bool softsim_demo_scan(void);
/****************************************************************************
* Function
****************************************************************************/
static bool softsim_plug_in(void)
{
	SOFTSIM_DEBUG("softsim_plug_in");
	usleep(50*1000);
	softsim_status_update(true);
	softsim_atr_status_check();
	return softsim_scan();
}

static void softsim_plug_out(void)
{
	softsim_status_update(false);
}

static void softsim_channel_status_check_callback(void)
{
	SOFTSIM_DEBUG("psim_channel_status = %d", psim_channel_status);
	if(1 == psim_channel_status)
	{
		SOFTSIM_TIMER_STOP(channel_status_check_timer_id);

		if(NULL != softsim_begin)
		{
			softsim_begin();
		}
	}
}

static void softsim_channel_status_check(void)
{
	SOFTSIM_TIMER_STOP(channel_status_check_timer_id);
	channel_status_check_timer_id = softsim_timer_create(softsim_channel_status_check_callback, SOFTSIM_CHECK_TIMEOUT);
}

static void softsim_atr_status_check_callback(void)
{
	if(!softsim_is_running())
	{
		SOFTSIM_DEBUG("softsim is not running!");
		return;
	}
	
	if(SOFTSIM_ATR_SUCCESS == softsim_get_atr_status())
	{
		SOFTSIM_TIMER_STOP(softsim_atr_check_timer_id);
		softsim_check_count = 0;
	}
	else
	{
		if(softsim_check_count >= SOFTSIM_CHECK_MAX_COUNT)
		{
			SOFTSIM_DEBUG("SoftSIM ATR fail, then restart!");
			SOFTSIM_TIMER_STOP(softsim_atr_check_timer_id);
			softsim_check_count = 0;
			softsim_atr_status_update(SOFTSIM_ATR_TIMEOUT);
			softsim_plug_out();
			usleep(500*1000);
			if(NULL != softsim_begin)
			{
				softsim_begin();
			}
		}
		else
		{
			softsim_check_count++;
		}
	}
}

static void softsim_atr_status_check(void)
{
	SOFTSIM_DEBUG_MARK;
	if(!softsim_is_running())
	{
		return;
	}
	
	SOFTSIM_TIMER_STOP(softsim_atr_check_timer_id);
	softsim_check_count = 0;
	softsim_atr_check_timer_id = softsim_timer_create(softsim_atr_status_check_callback, SOFTSIM_CHECK_TIMEOUT);
}

static s32 softsim_information_update(char*ssid, char *iccid, char *imsi, char *plmn_list, char *fplmn_list, char *mnc_len, s32 sim_id, char *ip, s32 port)
{
	if(!softsim_param_valid(ssid) || !softsim_param_valid(iccid) || !softsim_param_valid(imsi) || !softsim_param_valid(mnc_len) || !softsim_param_valid(ip))
	{
		return SOFTSIM_EXCEPTION_INVALID_ARGUMENT;
	}

	if(SOFTSIM_ICCID_STR_LEN != softsim_strlen(iccid))
	{
		SOFTSIM_DEBUG("Invalid ICCID!");
		return SOFTSIM_EXCEPTION_INVALID_ICCID;
	}

	if(SOFTSIM_IMSI_STR_LEN != softsim_strlen(imsi))
	{
		SOFTSIM_DEBUG("Invalid IMSI!");
		return SOFTSIM_EXCEPTION_INVALID_IMSI;
	}

	if((SOFTSIM_MNC_MAX_LEN != softsim_atoi(mnc_len)) && (2 != softsim_atoi(mnc_len)))
	{
		SOFTSIM_DEBUG("Invalid MNC Length!");
		return SOFTSIM_EXCEPTION_INVALID_MNC_LENGTH;
	}

	softsim_memset(&softsim_information, 0x00, sizeof(softsim_information));
	softsim_strncpy(softsim_information.ssid, ssid, softsim_strlen(ssid));
	softsim_strncpy(softsim_information.imsi, imsi, SOFTSIM_IMSI_STR_LEN);
	softsim_strncpy(softsim_information.iccid, iccid, SOFTSIM_ICCID_STR_LEN);
	softsim_strncpy(softsim_information.mnc_len, mnc_len, softsim_strlen(mnc_len));
	softsim_information.address.sim_id = (u16)sim_id;
	softsim_strncpy(softsim_information.address.ip, ip, softsim_strlen(ip));
	softsim_information.address.port = (u16)port;
	vsim_data_load = true;
	
	return SOFTSIM_SUCCESS;
}

static s32 softsim_data_custom(char*iccid, char*imsi)
{
	u8 buffer[20] = {0x00};

	if(!softsim_param_valid(imsi) || SOFTSIM_IMSI_STR_LEN != softsim_strlen(imsi))
	{
		return SOFTSIM_EXCEPTION_INVALID_IMSI;
	}

	if(!softsim_param_valid(iccid) || SOFTSIM_ICCID_STR_LEN != softsim_strlen(iccid))
	{
		return SOFTSIM_EXCEPTION_INVALID_ICCID;
	}

	#if !defined(__SOFTSIM_TEST__)
	softsim_memset(buffer, 0x00, sizeof(buffer));
	softsim_asc_to_bcd(buffer, iccid);
	softsim_write_binary(0x2FE2, buffer, SOFTSIM_ICCID_LEN);

	softsim_memset(buffer, 0x00, sizeof(buffer));
	softsim_imsi_asc_to_bcd(buffer, imsi);
	softsim_write_binary(0x6F07, buffer, SOFTSIM_IMSI_LEN);
	#endif

	#if 0	//for test
	if(0 == softsim_strncmp("22210", imsi, 3))
	{
		SOFTSIM_DEBUG("update fplmn!");
		softsim_memset(buffer, 0x00, sizeof(buffer));
		softsim_string_to_hex(buffer, SOFTSIM_FPLMN_LEN, "62F27062F23062F210", softsim_strlen("62F27062F23062F210"));
		softsim_write_binary(0x6F7B, buffer, SOFTSIM_IMSI_LEN);
	}
	#endif

	return SOFTSIM_SUCCESS;
}

static void softsim_chmod_update(void)
{
	SOFTSIM_DEBUG("softsim_chmod_update");
	//softsim_system_cmd("mount -o remount rw /system");
	softsim_system_cmd("chmod -R 777 /dev");
	softsim_system_cmd("chmod 777  /dev/ttyMSM1");
}

static void softsim_current_channel_update(void)
{
	if(1 != softsim_channel_status)
	{
		if(1 == psim_channel_status)
		{
			current_channel = SOFTSIM_PHYSICAL_SIM_CHANNEL;
		}
		else
		{
			current_channel = SOFTSIM_CHANNEL_END;
		}
	}
	else if(1 != psim_channel_status)
	{
		if(1 == softsim_channel_status)
		{
			current_channel = SOFTSIM_VIRTUAL_SIM_CHANNEL;
		}
		else
		{
			current_channel = SOFTSIM_CHANNEL_END;
		}
	}
	else if((1 == psim_channel_status) && (1 == softsim_channel_status))
	{
		current_channel = SOFTSIM_VIRTUAL_SIM_CHANNEL;
	}

	SOFTSIM_DEBUG("last_channel = %d, current_channel = %d", last_channel, current_channel);
}

static void softsim_auth_session_update(void)
{
	#if defined(__SOFTSIM_T2C__)
		if(softsim_is_running())
		{
			if(current_channel < SOFTSIM_CHANNEL_MAX)
			{
				if(SOFTSIM_CHANNEL_END == last_channel)
				{
					SOFTSIM_DEBUG("Start to connect Sim Server!");
					softsim_channel_info(current_channel);
					softsim_authentication_init();
				}
				else if((last_channel != current_channel) && (last_channel < SOFTSIM_CHANNEL_MAX))
				{
					SOFTSIM_DEBUG("Session channel switch!");
					softsim_channel_info(current_channel);
					softsim_auth_session_close();
					softsim_auth_session_recreat();
				}
				else if((last_channel == current_channel) && (current_channel < SOFTSIM_CHANNEL_MAX))
				{
					/*SOFTSIM_DEBUG("Session recreat!");
					softsim_channel_info(current_channel);
					softsim_auth_session_close();
					softsim_auth_session_recreat();*/
				}
			}
		}
	#endif
}

static void softsim_session_status_check_timeout(void)
{
	if(softsim_running)
	{
		SOFTSIM_TIMER_STOP(softsim_session_status_check_timer_id);
		softsim_session_check_count = 0;
	}
	else
	{
		if(softsim_session_check_count >= 60)
		{
			SOFTSIM_TIMER_STOP(softsim_session_status_check_timer_id);
			softsim_session_check_count = 0;
			softsim_auth_session_close();
		}
		else
		{
			softsim_session_check_count++;
		}
	}
}

static softsim_session_status_check_start(void)
{
	softsim_session_check_count = 0;
	SOFTSIM_TIMER_STOP(softsim_session_status_check_timer_id);
	softsim_session_status_check_timer_id = softsim_timer_create(softsim_session_status_check_timeout, SOFTSIM_CHECK_TIMEOUT);
}


static void softsim_project_init(void)
{
	
}

u8 softsim_get_project_type(void)
{
	return project_type;
}

void softsim_channel_info(u8 channel)
{
	char channel_info[10] = {0};
	switch(channel)
	{
		case SOFTSIM_PHYSICAL_SIM_CHANNEL:
			softsim_strncpy(channel_info, "M1", softsim_strlen("M1"));
			break;

		case SOFTSIM_VIRTUAL_SIM_CHANNEL:
			softsim_strncpy(channel_info, "M2", softsim_strlen("M2"));
			break;

		default:
			softsim_strncpy(channel_info, "UNKNOW", softsim_strlen("UNKNOW"));
			break;
	}

	SOFTSIM_DEBUG("Network transmission channel is %s", channel_info);
}

void softsim_init(void)
{
	s32 ret;
	softsim_trace_init();
	SOFTSIM_DEBUG("***********************************TGT COS Start***********************************");
	//softsim_chmod_update();
	softsim_version_main();
	softsim_timer_init();
	softsim_socket_init();
	softsim_session_init();
	softsim_heartbeat_init();
	softsim_project_init();
	pthread_rwlock_destroy(&rwlock);
	ret = pthread_rwlock_init(&rwlock, NULL);
	if(0 != ret)
	{
		SOFTSIM_DEBUG("pthread_rwlock_init error!");
	}
}
	
void softsim_exit(void)
{
	softsim_trace_exit();
	softsim_heartbeat_exit();
}

bool softsim_is_running(void)
{
	return softsim_running;
}

bool softsim_is_stopping(void)
{
	return softsim_stopping;
}

void softsim_rwlock_wrlock(void)
{
	s32 ret;
	ret = pthread_rwlock_wrlock(&rwlock);
	if(0 != ret)
	{
		SOFTSIM_DEBUG("pthread_rwlock_wrlock:%s", strerror(ret));
	}
}

void softsim_rwlock_unlock(void)
{
	s32 ret;
	ret = pthread_rwlock_unlock(&rwlock);
	if(0 != ret)
	{
		SOFTSIM_DEBUG("pthread_rwlock_wrlock:%s", strerror(ret));
	}
}


void softsim_status_update(bool status)
{
	softsim_rwlock_wrlock();
	softsim_running = status;
	softsim_rwlock_unlock();
	SOFTSIM_DEBUG("softsim_running = %d", softsim_running);
}

void softsim_stop_status_update(bool status)
{
	softsim_rwlock_wrlock();
	softsim_stopping = status;
	softsim_rwlock_unlock();
	SOFTSIM_DEBUG("softsim_stopping = %d", softsim_stopping);
}


s32 softsim_channel_status_update(s32 ps_rt, s32 vs_rt)
{
	SOFTSIM_DEBUG("ps_rt = %d, vs_rt = %d", ps_rt, vs_rt);
	last_channel = current_channel;
	psim_channel_status = ps_rt;
	softsim_channel_status = vs_rt;
	softsim_current_channel_update();
	#if defined(__SOFTSIM_AUTH_DEMO__)
	softsim_auth_demo_session_update(ps_rt, vs_rt);
	#else
	softsim_auth_session_update();
	#endif
	SOFTSIM_DEBUG("psim_channel_status = %d, softsim_channel_status = %d", psim_channel_status, softsim_channel_status);
	return SOFTSIM_SUCCESS;
}

u16 softsim_get_sim_id(void)
{
	return softsim_information.address.sim_id;
}

s32 softsim_data_load(char*ssid, char *iccid, char *imsi, char *plmn_list, char *fplmn_list, char *mnc_len, s32 sim_id, char *ip, s32 port)
{
	SOFTSIM_DEBUG("SSID:%s, ICCID:%s, IMSI:%s", ssid, iccid, imsi);
	SOFTSIM_DEBUG("IP:[%s], PORT:[%d], sim_id:%d", ip, port, sim_id);

	if(softsim_is_running())
	{
		SOFTSIM_DEBUG("SoftSIM is running!");
		return SOFTSIM_EXCEPTION_IS_RUNNING;
	}
	
	if(!softsim_param_valid(ssid) || !softsim_param_valid(iccid) || !softsim_param_valid(imsi) || !softsim_param_valid(mnc_len) || !softsim_param_valid(ip))
	{
		return SOFTSIM_EXCEPTION_INVALID_ARGUMENT;
	}

	return softsim_information_update(ssid, iccid, imsi, plmn_list, fplmn_list, mnc_len, sim_id, ip, port);
}

s32 softsim_start(char *plmn)
{
	s32 ret = 0;
	char hplmn[SOFTSIM_PLMN_MAX_LEN+1] = {0};
	SOFTSIM_DEBUG("softsim_start plmn: %s", plmn);
	SOFTSIM_DEBUG("psim_channel_status = %d", psim_channel_status);

	if(softsim_is_running())
	{
		SOFTSIM_DEBUG("SoftSIM is running!");
		return SOFTSIM_EXCEPTION_IS_RUNNING;
	}

	if(!vsim_data_load)
	{
		SOFTSIM_DEBUG("VSIM data not load!");
		return SOFTSIM_EXCEPTION_VSIM_DATA_NOT_LOAD;
	}

	if(softsim_strlen(plmn) < 5 || softsim_strlen(plmn) > 6
		|| (0 == softsim_strncmp("000", plmn, 3)))
	{
		SOFTSIM_DEBUG("Invalid USIM template name!");
		return SOFTSIM_EXCEPTION_INVALID_PLMN;
	}

	SOFTSIM_TIMER_STOP(softsim_session_status_check_timer_id);
	SOFTSIM_TIMER_STOP(softsim_atr_check_timer_id);
	#if defined(__SOFTSIM_ANDROID__)
		softsim_thread_status_check();
	#endif
	softsim_plug_out();
	usleep(500*1000);

	if(SOFTSIM_IMSI_STR_LEN != softsim_strlen(softsim_information.imsi))
	{
		SOFTSIM_DEBUG("Invalid IMSI!");
		return SOFTSIM_EXCEPTION_INVALID_IMSI;
	}
	
	if((SOFTSIM_MNC_MAX_LEN != softsim_atoi(softsim_information.mnc_len)) && (2 != softsim_atoi(softsim_information.mnc_len)))
	{
		SOFTSIM_DEBUG("Invalid MNC Length!");
		return SOFTSIM_EXCEPTION_INVALID_MNC_LENGTH;
	}
	
	softsim_strncpy(hplmn, softsim_information.imsi, softsim_atoi(softsim_information.mnc_len)+SOFTSIM_MCC_LEN);
	//softsim_strncpy(softsim_information.mcc, plmn, SOFTSIM_MCC_LEN);

	#if defined(__SOFTSIM_TEST__)
		softsim_memset(hplmn, 0x00, sizeof(hplmn));
		softsim_strncpy(hplmn, SOFTSIM_HPLMN_TEST, softsim_strlen(SOFTSIM_HPLMN_TEST));
	#endif
	
	/*if(0 != softsim_strncmp(plmn, hplmn, SOFTSIM_MCC_LEN))
	{
		SOFTSIM_DEBUG("Softsim in roaming!");
		softsim_information.roaming = true;
	}*/

	SOFTSIM_DEBUG("HPLMN: %s", hplmn);

	
	#if defined(__SOFTSIM_TEST__)
		ret = softsim_uicc_init(hplmn);
	#else
		ret = softsim_uicc_init(plmn);
	#endif
	
	if(0 != ret)
	{
		SOFTSIM_DEBUG("SoftSIM uicc init fail! ret = %d", ret);
		return ret;
	}

	ret = softsim_data_custom(softsim_information.iccid, softsim_information.imsi);

	if(0 != ret)
	{
		SOFTSIM_DEBUG("update custom data fail!");
		return ret;
	}
	
	softsim_stopping = false;
	softsim_begin = softsim_plug_in;
	softsim_plug_in();
	softsim_auth_session_init();
	SOFTSIM_DEBUG("SoftSIM start success!");
	return SOFTSIM_SUCCESS;
}

s32 softsim_stop(s32 cause_value)
{
	SOFTSIM_DEBUG("softsim_stop cause_value = %d", cause_value);
	SOFTSIM_TIMER_STOP(softsim_atr_check_timer_id);
	SOFTSIM_TIMER_STOP(softsim_stop_timer_id);
	softsim_stopping = true;
	softsim_auth_reset_handle();
	softsim_stop_handle(cause_value);
	softsim_begin = NULL;
	softsim_plug_out();
	//softsim_session_status_check_start();
	softsim_auth_session_exit();
	softsim_memset(&softsim_information, 0x00, sizeof(softsim_information));
	vsim_data_load = false;
	SOFTSIM_DEBUG("softsim_stop success!!");
	return SOFTSIM_SUCCESS;
}

u8 softsim_get_current_channel(void)
{
	return current_channel;
}

SoftsimInformation *softsim_get_infomation(void)
{
	return &softsim_information;
}

void softsim_authentication_channel_switch_register(SoftsimFuncPtr fub_cb)
{
	softsim_auth_channel_switch = fub_cb;
}

void softsim_authentication_init(void)
{
	softsim_auth_session_init();

	//Heartbeat
	#if defined(__SOFTSIM_HEARTBEAT__)
		softsim_heartbeat_status_update(true);
	#endif
}

void softsim_authentication_exit(void)
{
	softsim_heartbeat_status_update(false);
	softsim_auth_session_exit();
}
