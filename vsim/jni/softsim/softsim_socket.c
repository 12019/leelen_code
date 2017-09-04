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
 * Filename: softsim_socket.c
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-07-16
 ******************************************************************************/

/*****************************************************************************
* Include																	   
*****************************************************************************/
#include "softsim_include.h"
/*****************************************************************************
* Define																	   
*****************************************************************************/
#undef 	SOFTSIM_MD_ID
#undef 	SOFTSIM_TRACE_HEADER
#define SOFTSIM_MD_ID					SOFTSIM_MD_SOCKET
#define SOFTSIM_TRACE_HEADER			"[SOCKET]" 
/*****************************************************************************
* Typedef																	   
*****************************************************************************/
typedef enum
{
	SOFTSIM_SOCKET_THREAD_CONNECT,
	SOFTSIM_SOCKET_THREAD_READ,
	SOFTSIM_SOCKET_THREAD_WRITE,
	SOFTSIM_SOCKET_THREAD_LISTEN,
	SOFTSIM_SOCKET_THREAD_MAX,
}SoftsimSocketThreadEnum;
	
//socket contex, socket_fd, domain, type, fun_cb
typedef struct
{
	s32 socket_fd;
	SoftsimSocketDomain domain;
	SoftsimSocketType type;
	SoftsimSocketNotifyFunctionCallBack notify_cb;
	SoftsimGetHostNameFunctionCallBack parse_domain_cb;
	SoftsimSocketAddress address;
	pthread_t thread_id[SOFTSIM_SOCKET_THREAD_MAX];
}SoftsimSocketStruct;
	
typedef struct
{
	char	domain[SOFTSIM_DOMAIN_LEN];
	u8		address[SOFTSIM_MAX_ADDR_LEN];
}SoftsimSocketDnsCache;
/*****************************************************************************
* Static Local Variable															  
*****************************************************************************/
static SoftsimSocketStruct socket_array[SOFTSIM_MAX_SOCKET_NUM];
static SoftsimSocketDnsCache socket_dns_cache[SOFTSIM_MAX_DNS_CACHE];
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
static void softsim_socket_dns_cache_init(void)
{
	softsim_memset(&socket_dns_cache, 0, sizeof(socket_dns_cache));	
}
	
static void softsim_dns_add_new_item(char* domain, u8* ip)
{
	u32 index, idle;
	
	if((NULL == domain)||(NULL == ip))
	{
		return;
	}
		
	idle = SOFTSIM_MAX_DNS_CACHE;
	for(index = 0; index < SOFTSIM_MAX_DNS_CACHE; index ++)
	{
		SoftsimSocketDnsCache* item = &socket_dns_cache[index];
			
		if((softsim_strlen(item->domain) != 0)
			&&(softsim_strcasecmp(item->domain, domain) == 0))
		{
			softsim_memcpy(item->address, ip, SOFTSIM_IP_V4_ADDR_LEN);
			return;
		}
		else if((idle == SOFTSIM_MAX_DNS_CACHE)&&(softsim_strlen(item->domain) == 0))
		{
			idle = index;
		}
	}
		
	if(idle != SOFTSIM_MAX_DNS_CACHE)
	{
		SoftsimSocketDnsCache* item = &socket_dns_cache[idle];
	
		softsim_strncpy(item->domain, domain, sizeof(item->domain)-1);
		softsim_memcpy(item->address, ip, SOFTSIM_IP_V4_ADDR_LEN);
	}
}
	
static bool softsim_dns_find_cache(const char* domain, u8* address)
{
	u32 index;
	
	if((NULL == domain)||(NULL == address))
	{
		return;
	}
		
	for(index = 0; index < SOFTSIM_MAX_DNS_CACHE; index ++)
	{
		SoftsimSocketDnsCache* item = &socket_dns_cache[index];
			
		if((softsim_strlen(item->domain) != 0)
			&&(softsim_strcasecmp((const char*)item->domain, (const char*)domain) == 0))
		{
			softsim_memcpy(address, item->address, SOFTSIM_IP_V4_ADDR_LEN);
			SOFTSIM_DEBUG("find dns cache domain = %s, ip = [%d.%d.%d.%d]", 
					domain, address[0], address[1], address[2], address[3]);
			return true;
		}
	}
		
	return false;
}
	
	
static s32 softsim_insert_socket_domain_type(s32 socket_fd, SoftsimSocketDomain domain, SoftsimSocketType type)
{
	u32 i;
	
	for(i = 0;i < SOFTSIM_MAX_SOCKET_NUM;i++)
	{
		if(socket_array[i].socket_fd == SOFTSIM_INVALID_SOCKET_FD)
		{
			break;
		}
	}
	
	if(i >= SOFTSIM_MAX_SOCKET_NUM)
	{
			SOFTSIM_ERROR("Not Enough Resource!!");
			return SOFTSIM_SOCKET_LIMIT_RESOURCE;
	}
	
	socket_array[i].socket_fd = socket_fd;
	socket_array[i].domain = domain;
	socket_array[i].type = type;
	
	return SOFTSIM_SOCKET_SUCCESS;
}

static void softsim_set_socket_function_callback(s32 socket_fd, SoftsimSocketNotifyFunctionCallBack fun_cb)
{
	u32 i;

	for(i = 0;i < SOFTSIM_MAX_SOCKET_NUM;i++)
	{
		if(socket_array[i].socket_fd == socket_fd)
		{
			break;
		}
	}

	if(i >= SOFTSIM_MAX_SOCKET_NUM)
	{
		SOFTSIM_ERROR("Can not find socket_fd = %d", socket_fd);
		return;
	}

	socket_array[i].notify_cb = fun_cb;
}

static void softsim_set_socket_parse_domain_function_callback(s32 socket_fd, SoftsimGetHostNameFunctionCallBack fun_cb)
{
	u32 i;

	for(i = 0;i < SOFTSIM_MAX_SOCKET_NUM;i++)
	{
		if(socket_array[i].socket_fd == socket_fd)
		{
			break;
		}
	}

	if(i >= SOFTSIM_MAX_SOCKET_NUM)
	{
		SOFTSIM_ERROR("Can not find socket_fd = %d", socket_fd);
		return;
	}

	socket_array[i].parse_domain_cb = fun_cb;
}

static void softsim_set_socket_connect_address(s32 socket_fd, SoftsimSocketAddress *address)
{
	u32 i;

	for(i = 0;i < SOFTSIM_MAX_SOCKET_NUM;i++)
	{
		if(socket_array[i].socket_fd == socket_fd)
		{
			break;
		}
	}

	if(i >= SOFTSIM_MAX_SOCKET_NUM)
	{
		SOFTSIM_ERROR("Can not find socket_fd = %d", socket_fd);
		return;
	}

	softsim_memcpy(&(socket_array[i].address), address, sizeof(SoftsimSocketAddress));
}

static void softsim_set_socket_thread_id(s32 socket_fd, SoftsimSocketThreadEnum type, pthread_t *thread)
{
	u32 i;

	for(i = 0;i < SOFTSIM_MAX_SOCKET_NUM;i++)
	{
		if(socket_array[i].socket_fd == socket_fd)
		{
			break;
		}
	}

	if(i >= SOFTSIM_MAX_SOCKET_NUM)
	{
		SOFTSIM_ERROR("Can not find socket_fd = %d", socket_fd);
		return;
	}

	softsim_memcpy(&(socket_array[i].thread_id[type]), thread, sizeof(pthread_t));
}

static bool softsim_get_socket_connect_address(s32 socket_fd, SoftsimSocketAddress *address)
{
	u32 i;

	for(i = 0;i < SOFTSIM_MAX_SOCKET_NUM;i++)
	{
		if(socket_array[i].socket_fd == socket_fd)
		{
			break;
		}
	}

	if(i >= SOFTSIM_MAX_SOCKET_NUM)
	{
		SOFTSIM_ERROR("Can not find socket_fd = %d", socket_fd);
		return false;
	}

	softsim_memcpy(address, &(socket_array[i].address), sizeof(SoftsimSocketAddress));
	
	return true;
}

static SoftsimSocketDomain softsim_get_socket_domain(s32 socket_fd)
{
	u32 i;

	for(i = 0;i < SOFTSIM_MAX_SOCKET_NUM;i++)
	{
		if(socket_array[i].socket_fd == socket_fd)
		{
			break;
		}
	}

	if(i >= SOFTSIM_MAX_SOCKET_NUM)
	{
		SOFTSIM_DEBUG("Can not find socket_fd = %d", socket_fd);
		return SOFTSIM_SOCKET_DOMAIN_MAX;
	}

	return socket_array[i].domain;
}

static SoftsimSocketType softsim_get_socket_type(s32 socket_fd)
{
	u32 i;

	for(i = 0;i < SOFTSIM_MAX_SOCKET_NUM;i++)
	{
		if(socket_array[i].socket_fd == socket_fd)
		{
			break;
		}
	}

	if(i >= SOFTSIM_MAX_SOCKET_NUM)
	{
		SOFTSIM_ERROR("Can not find socket_fd = %d", socket_fd);
		return SOFTSIM_SOCKET_TYPE_MAX;
	}

	return socket_array[i].type;
}

static SoftsimSocketNotifyFunctionCallBack softsim_get_socket_function_callback(s32 socket_fd)
{
	u32 i;

	for(i = 0;i < SOFTSIM_MAX_SOCKET_NUM;i++)
	{
		if(socket_array[i].socket_fd == socket_fd)
		{
			break;
		}
	}

	if(i >= SOFTSIM_MAX_SOCKET_NUM)
	{
		SOFTSIM_ERROR("Can not find socket_fd = %d", socket_fd);
		return NULL;
	}

	return socket_array[i].notify_cb;
}

static SoftsimGetHostNameFunctionCallBack softsim_get_socket_parse_domain_fun_cb(s32 socket_fd)
{
	u32 i;

	for(i = 0;i < SOFTSIM_MAX_SOCKET_NUM;i++)
	{
		if(socket_array[i].socket_fd == socket_fd)
		{
			break;
		}
	}

	if(i >= SOFTSIM_MAX_SOCKET_NUM)
	{
		SOFTSIM_ERROR("Can not find socket_fd = %d", socket_fd);
		return NULL;
	}

	return socket_array[i].parse_domain_cb;
}

static void softsim_get_socket_thread_id(s32 socket_fd, SoftsimSocketThreadEnum type, pthread_t *thread)
{
	u32 i;

	for(i = 0;i < SOFTSIM_MAX_SOCKET_NUM;i++)
	{
		if(socket_array[i].socket_fd == socket_fd)
		{
			break;
		}
	}

	if(i >= SOFTSIM_MAX_SOCKET_NUM)
	{
		SOFTSIM_ERROR("Can not find socket_fd = %d", socket_fd);
		return;
	}

	softsim_memcpy(thread, &(socket_array[i].thread_id[type]), sizeof(pthread_t));
}

static void softsim_clear_socket_thread_id(s32 socket_fd, SoftsimSocketThreadEnum type)
{
	u32 i;

	for(i = 0;i < SOFTSIM_MAX_SOCKET_NUM;i++)
	{
		if(socket_array[i].socket_fd == socket_fd)
		{
			break;
		}
	}

	if(i >= SOFTSIM_MAX_SOCKET_NUM)
	{
		return;
	}

	softsim_memset(&(socket_array[i].thread_id[type]), 0, sizeof(pthread_t));
}


static void softsim_delete_socket(s32 socket_fd)
{
	u32 i;

	for(i = 0;i < SOFTSIM_MAX_SOCKET_NUM;i++)
	{
		if(socket_array[i].socket_fd == socket_fd)
		{
			break;
		}
	}

	if(i >= SOFTSIM_MAX_SOCKET_NUM)
	{
		SOFTSIM_DEBUG("Can not find socket_fd = %d", socket_fd);
		return;
	}

	softsim_memset(&(socket_array[i]), 0, sizeof(SoftsimSocketStruct));
	socket_array[i].socket_fd = SOFTSIM_INVALID_SOCKET_FD;
}

static void softsim_socket_notify_handle_msg(void *msg)
{
	SoftsimSocketNotify *notify = (SoftsimSocketNotify *)msg;
	SoftsimSocketNotifyFunctionCallBack fun_cb;

	fun_cb = softsim_get_socket_function_callback(notify->socket_fd);
	
	//need to close, when notify close or false result
	if(notify->result == false || notify->event_type == SOFTSIM_SOCKET_CLOSE)
	{
		//close by upper layer
		//val_socket_close(notify->socket_fd);
	}
	
	if(fun_cb)
	{
		fun_cb(notify);
	}
	else
	{
		SOFTSIM_ERROR("result = %d, event = %d", notify->result, notify->event_type);
	}
}

static void softsim_socket_notify_handle(SoftsimSocketNotify *notify)
{
	softsim_socket_notify_handle_msg(notify);
}

static void softsim_get_host_by_name_notify_handle_msg(void *msg)
{
	SoftsimSocketGetHostNameNotify *notify = (SoftsimSocketGetHostNameNotify *)msg;
	SoftsimGetHostNameFunctionCallBack fun_cb;

	fun_cb = softsim_get_socket_parse_domain_fun_cb(notify->socket_fd);
	if(fun_cb)
	{
		fun_cb(notify);
	}
}

static s32 softsim_socket_get_map_error_code(void)
{
	s32 error_code = SOFTSIM_SOCKET_ERROR;
	s32 err_no = (s32)errno;
	
	SOFTSIM_DEBUG("err_no = %d", err_no);

	switch(err_no)
	{
		case EAGAIN:
		case EINPROGRESS:
			error_code = SOFTSIM_SOCKET_WOULDBLOCK;
			break;
		case EPIPE:
			error_code = SOFTSIM_SOCKET_PIPE;
			break;
		//to be add more map
		default:
			error_code = SOFTSIM_SOCKET_ERROR;
			break;
	}

	return error_code;
}

static void softsim_socket_get_if_name(SoftsimSocketDomain domain, char *ifname, u32 size)
{
	char *default_ifname = SOFTSIM_IFNAME_NET_WLAN;

	if(domain == SOFTSIM_SOCKET_PF_INET_PHYSICALSIM)
	{
		default_ifname = SOFTSIM_IFNAME_NET_PHYSICALSIM;
	}
	else if(domain == SOFTSIM_SOCKET_PF_INET_SOFTSIM)
	{
		default_ifname = SOFTSIM_IFNAME_NET_SOFTSIM;
	}
	else if(domain == SOFTSIM_SOCKET_PF_LOCAL)
	{
		default_ifname = SOFTSIM_IFNAME_LOCAL_DEVICE;
	}

	softsim_strncpy(ifname, default_ifname, size - 1);
}

static bool softsim_socket_exist(int socket_fd)
{
	u32 i;

	for(i = 0;i < SOFTSIM_MAX_SOCKET_NUM;i++)
	{
		if(socket_array[i].socket_fd == socket_fd)
		{
			break;
		}
	}

	if(i >= SOFTSIM_MAX_SOCKET_NUM)
	{
		return false;
	}

	return true;
}

static bool softsim_socket_thread_exist(int socket_fd, SoftsimSocketThreadEnum type)
{
	u32 i;
	pthread_t thread_id;

	for(i = 0;i < SOFTSIM_MAX_SOCKET_NUM;i++)
	{
		if(socket_array[i].socket_fd == socket_fd)
		{
			break;
		}
	}

	if(i >= SOFTSIM_MAX_SOCKET_NUM)
	{
		return false;
	}

	thread_id = pthread_self();
	//SOFTSIM_TRACE_HEX("self thread_id = ", &thread_id, sizeof(thread_id));
	//SOFTSIM_TRACE_HEX("thread_id = ", &(socket_array[i].thread_id[type]), sizeof(thread_id));
	if(thread_id != socket_array[i].thread_id[type])
	{
		return false;
	}

	return true;
}

static void softsim_socket_notify_event(s32 socket_fd, SoftsimSocketEvent event)
{
	SoftsimSocketNotify notify;

	notify.result = true;
	notify.socket_fd = socket_fd;
	notify.event_type = event;
	notify.error_code = 0;
	notify.detail_error = 0;

	softsim_socket_notify_handle(&notify);
}

static void softsim_socket_notify_exception(s32 socket_fd, SoftsimSocketEvent event, SoftsimSocketErrorCode err, s32 detail_err)
{
	SoftsimSocketNotify notify;

	notify.result = false;
	notify.socket_fd = socket_fd;
	notify.event_type = event;
	notify.error_code = err;
	notify.detail_error = detail_err;

	softsim_socket_notify_handle(&notify);
}

static void * softsim_socket_read_thread (void *arg)
{
	s32 socket_fd = (s32)arg, ret;
	fd_set read_fd, write_fd, except_fd;
	struct timeval soc_timeout;

	//SOFTSIM_DEBUG("socket_fd = %d", socket_fd);
    while(softsim_socket_exist(socket_fd))
    {
		//SOFTSIM_DEBUG_MARK;
		soc_timeout.tv_usec = SOFTSIM_SOCKET_SELECT_TIMEOUT*10*1000;
    	soc_timeout.tv_sec = 0;//SOFTSIM_SOCKET_SELECT_TIMEOUT;	
	
		FD_ZERO(&read_fd);
		FD_ZERO(&except_fd);		

		FD_SET(socket_fd, &read_fd);
        FD_SET(socket_fd, &except_fd);

        ret = select(socket_fd + 1, &read_fd, NULL, &except_fd, &soc_timeout);
		if(false == softsim_socket_thread_exist(socket_fd, SOFTSIM_SOCKET_THREAD_READ))
		{
			SOFTSIM_DEBUG_MARK;
			break;
		}
		
		if(ret >= 0)
		{
			if(FD_ISSET(socket_fd, &read_fd))
	        {
				SOFTSIM_DEBUG_MARK;	        	
				{
					struct tcp_info info; 
					int len = sizeof(info); 
					
					getsockopt(socket_fd, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len); 
					//SOFTSIM_DEBUG("tcpi_state = %d, TCP_ESTABLISHED = %d", info.tcpi_state, TCP_ESTABLISHED);
					if(info.tcpi_state != TCP_ESTABLISHED)
					{
						//soc closed by server
						SOFTSIM_DEBUG_MARK;						
						softsim_socket_notify_event(socket_fd, SOFTSIM_SOCKET_CLOSE);	
						break;
					}
	        	}
	            //read ready
	            softsim_socket_notify_event(socket_fd, SOFTSIM_SOCKET_READ);	
				break;
				
	        }
			
	        if (FD_ISSET(socket_fd, &except_fd))
	        {
				//error happen
				SOFTSIM_DEBUG("err = %d", errno);
				if(errno == EINTR)
					continue;
				softsim_socket_notify_exception(socket_fd, SOFTSIM_SOCKET_CLOSE, softsim_socket_get_map_error_code(), (s32)errno);	
				break;
	        }
		}
		else
		{
			SOFTSIM_DEBUG("ret = %d, error = %s", ret, strerror(errno));
			if(errno == EBADF)
				break;
		}
    }

	return NULL;
}

static void * softsim_socket_delay_read_thread(void *arg)
{
	sleep(1);
	return softsim_socket_read_thread(arg);
}

static void * softsim_socket_write_thread (void *arg)
{
	s32 socket_fd = (s32)arg, ret;
	fd_set read_fd, write_fd, except_fd;
	struct timeval soc_timeout;

	SOFTSIM_DEBUG_MARK;	
    while(softsim_socket_exist(socket_fd))
    {
    	SOFTSIM_DEBUG("");
		soc_timeout.tv_usec = 0;
    	soc_timeout.tv_sec = SOFTSIM_SOCKET_SELECT_TIMEOUT;	
	
		FD_ZERO(&write_fd);
		FD_ZERO(&except_fd);
		
		FD_SET(socket_fd, &write_fd);
        FD_SET(socket_fd, &except_fd);

        ret = select(socket_fd + 1, NULL, &write_fd, &except_fd, &soc_timeout);
		if(false == softsim_socket_thread_exist(socket_fd, SOFTSIM_SOCKET_THREAD_WRITE))
		{
			SOFTSIM_DEBUG_MARK;
			break;
		}
		if(ret >= 0)
		{
	        if (FD_ISSET(socket_fd, &write_fd))
	        {
	        	SOFTSIM_DEBUG("");		
	            //write ready
	            softsim_socket_notify_event(socket_fd, SOFTSIM_SOCKET_WRITE);
				break;
	        }
	        if (FD_ISSET(socket_fd, &except_fd))
	        {
				//error happen
				SOFTSIM_DEBUG("err = %d", errno);
				if(errno == EINTR)
					continue;
				softsim_socket_notify_exception(socket_fd, SOFTSIM_SOCKET_CLOSE, softsim_socket_get_map_error_code(), (s32)errno);	
				break;
	        }
		}
		else
		{
			SOFTSIM_DEBUG("ret = %d, error = %s", ret, strerror(errno));
			if(errno == EBADF)
				break;
		}
    }

	return NULL;
}

static void * softsim_socket_connect_thread(void *arg)
{
	s32 socket_fd = (int)arg, ret;
	fd_set write_fd, except_fd;
	struct timeval soc_timeout;

    while(softsim_socket_exist(socket_fd))
    {
		SOFTSIM_DEBUG("");
		soc_timeout.tv_usec = 0;
   	 	soc_timeout.tv_sec = SOFTSIM_SOCKET_SELECT_TIMEOUT;
		
		FD_ZERO(&write_fd);
    	FD_ZERO(&except_fd);
		
        FD_SET(socket_fd, &write_fd);
        FD_SET(socket_fd, &except_fd);

        ret = select(socket_fd + 1, NULL, &write_fd, &except_fd, &soc_timeout);
		if(!softsim_socket_thread_exist(socket_fd, SOFTSIM_SOCKET_THREAD_CONNECT))
		{
			SOFTSIM_DEBUG_MARK;
			break;
		}
		if(ret >= 0)
		{
	        if (FD_ISSET(socket_fd, &write_fd))
	        {
	            //connected
				SOFTSIM_DEBUG("");
				{
					s32  err;
					pthread_t thread_id;
					
					err = pthread_create(&thread_id, NULL, softsim_socket_read_thread, (void *)socket_fd);
					if(err == 0)  
					{
						pthread_detach(thread_id);
						softsim_set_socket_thread_id(socket_fd, SOFTSIM_SOCKET_THREAD_READ, &thread_id);
					}  
				}
	            softsim_socket_notify_event(socket_fd, SOFTSIM_SOCKET_CONNECT);				
	            break;
	        }
	        if (FD_ISSET(socket_fd, &except_fd))
	        {
				//error happen
				// SOFTSIM_DEBUG("err = %d", errno);
				if(errno == EINTR)
					continue;
				softsim_socket_notify_exception(socket_fd, SOFTSIM_SOCKET_CLOSE, softsim_socket_get_map_error_code(), (s32)errno);	
				break;
	        }
			SOFTSIM_DEBUG("");
		}
		else
		{
			//SOFTSIM_DEBUG("ret = %d, error = %s", ret, strerror(errno));
			if(errno == EBADF)
				break;
		}
    }

	return NULL;
}

static void * softsim_socket_listen_thread(void *arg)
{
	s32 socket_fd = (int)arg, ret;
	fd_set read_fd, write_fd, except_fd;
	struct timeval soc_timeout;

    while(softsim_socket_exist(socket_fd))
    {
		SOFTSIM_DEBUG("");
		soc_timeout.tv_usec = 0;
   	 	soc_timeout.tv_sec = SOFTSIM_SOCKET_SELECT_TIMEOUT;

		FD_ZERO(&read_fd);
		FD_ZERO(&except_fd);
		
        FD_SET(socket_fd, &read_fd);
		FD_SET(socket_fd, &write_fd);
        FD_SET(socket_fd, &except_fd);

        ret = select(socket_fd + 1, &read_fd, NULL, &except_fd, &soc_timeout);
		if(false == softsim_socket_thread_exist(socket_fd, SOFTSIM_SOCKET_THREAD_LISTEN))
		{
			SOFTSIM_DEBUG_MARK;
			break;
		}
		if(ret > 0)
		{
			if (FD_ISSET(socket_fd, &read_fd))
	        {
	        	SOFTSIM_DEBUG("");
				
	            //read ready, to be accept
	            softsim_socket_notify_event(socket_fd, SOFTSIM_SOCKET_ACCEPT);				
	        }
	        if (FD_ISSET(socket_fd, &except_fd))
	        {
	            //error happen
	            SOFTSIM_DEBUG("err = %d", errno);
				if(errno == EINTR)
					continue;
				softsim_socket_notify_exception(socket_fd, SOFTSIM_SOCKET_CLOSE, softsim_socket_get_map_error_code(), (s32)errno);	
				break;
	        }
			SOFTSIM_DEBUG("");
		}
		SOFTSIM_DEBUG("ret = %d", ret);
		sleep(1);
    }

	return NULL;
}


static void softsim_connect_get_host_by_name_cb(SoftsimSocketGetHostNameNotify *notify)
{
	s32 socket_fd;
	SoftsimSocketAddress address;

	if(notify->result)	//parse host name success
	{
		socket_fd = notify->socket_fd;
		if(softsim_get_socket_connect_address(socket_fd, &address))
		{
			//change domain mode to ip mode, then recall connect
			address.address_type = SOFTSIM_ADDRESS_TYPE_IP;
			softsim_memcpy(address.address, notify->address, notify->address_len);
			address.address_len = notify->address_len;

			softsim_socket_connect(socket_fd, &address);
		}
	}
	else
	{
		softsim_socket_notify_exception(notify->socket_fd, SOFTSIM_SOCKET_CONNECT, SOFTSIM_SOCKET_GETHOSTBYNAME_FAILED, 0);
	}
}


/*======================================================================================================*/

void softsim_socket_init(void)
{
	u32 i;

	softsim_memset(&socket_array, 0, sizeof(socket_array));
	for(i = 0;i < SOFTSIM_MAX_SOCKET_NUM;i++)
	{
		socket_array[i].socket_fd = SOFTSIM_INVALID_SOCKET_FD;
	}
	softsim_socket_dns_cache_init();
	//softsim_socket_dns_init(_get_host_by_name_notify_handle);
}

s32 softsim_socket_create(SoftsimSocketDomain domain, SoftsimSocketType type)
{
	s32 socket = SOFTSIM_INVALID_SOCKET_FD;
	s32 ret = SOFTSIM_SOCKET_INVAL;
	char ifname[SOFTSIM_IF_NAME_LEN];

	SOFTSIM_DEBUG("domain = %d, type = %d", domain, type);
	
	socket = socket_create(domain, type);
	
	SOFTSIM_DEBUG("socket = %d", socket);
	if(socket >= 0)
	{
		ret = softsim_insert_socket_domain_type(socket, domain, type);
		if(ret != SOFTSIM_SOCKET_SUCCESS)
		{
			softsim_socket_close(socket);
			return ret;
		}

		softsim_memset(ifname, 0, sizeof(ifname));
		softsim_socket_get_if_name(domain, ifname, sizeof(ifname));
		
		if((0 == softsim_strcmp(ifname, SOFTSIM_IFNAME_NET_SOFTSIM)))
		{
			return socket;
		}
		
		#if defined(__SOFTSIM_SOCKET_INTERFACE_NAME__)
		ret = socket_bind_device(socket, ifname);
		if(ret != SOFTSIM_SOCKET_SUCCESS)
		{
			softsim_socket_close(socket);
			return ret;
		}
		#endif
	}
	else
	{
		SOFTSIM_ERROR("error = %s", strerror(errno));
	}

	return socket;
}

s32 softsim_socket_connect(s32 socket_fd, SoftsimSocketAddress *address)
{
	s32 ret = SOFTSIM_SOCKET_INVAL;
	char ip_str[SOFTSIM_IP_STRING_LEN];

	if(softsim_get_socket_type(socket_fd) != address->type)
	{
		SOFTSIM_ERROR("Socket type not match with create");
	}

	if(address->address_type == SOFTSIM_ADDRESS_TYPE_DOMAIN)
	{
		ret = softsim_socket_get_host_by_name(socket_fd, address->domain, address->address, &(address->address_len));
		if(ret < SOFTSIM_SOCKET_SUCCESS)
		{
			if(ret == SOFTSIM_SOCKET_WOULDBLOCK)
			{
				softsim_socket_set_get_host_by_name_notify(socket_fd, softsim_connect_get_host_by_name_cb);
				softsim_set_socket_connect_address(socket_fd, address);
			}
			return ret;
		}
	}

	//softism_ip_to_ipstring(address->address, address->address_len, ip_str, sizeof(ip_str));
	ret = socket_connect(socket_fd, address->domain, address->port);

	if(ret == SOFTSIM_SOCKET_WOULDBLOCK)
	{
		s32 err;
		pthread_t thread_id;
		
		err = pthread_create(&thread_id, NULL, softsim_socket_connect_thread,(void *)socket_fd);
		if(err == 0)  
		{
			pthread_detach(thread_id);
			softsim_set_socket_thread_id(socket_fd, SOFTSIM_SOCKET_THREAD_CONNECT, &thread_id);
		}  
	}
	return ret;
}

s32 softsim_socket_bind(s32 socket_fd, SoftsimSocketAddress *address)
{
	s32 ret = SOFTSIM_SOCKET_INVAL;
	char ip_str[SOFTSIM_IP_STRING_LEN];

	if(softsim_get_socket_type(socket_fd) != address->type)
	{
		SOFTSIM_ERROR("Socket type not match with create");
	}

	if(address->address_type == SOFTSIM_ADDRESS_TYPE_DOMAIN)
	{
		ret = softsim_socket_get_host_by_name(socket_fd, address->domain, address->address, &(address->address_len));
		if(ret == SOFTSIM_SOCKET_SUCCESS)
		{
			//parse success, then change flag
			address->address_type = SOFTSIM_ADDRESS_TYPE_IP;
		}
		else
		{
			return SOFTSIM_SOCKET_INVAL;
		}
	}

	softism_ip_to_ipstring(address->address, address->address_len, ip_str, sizeof(ip_str));
	ret = socket_bind(socket_fd, ip_str, address->port);

	return ret;
}

s32 softsim_socket_listen(s32 socket_fd, u32 backlog)
{
	s32 ret = SOFTSIM_SOCKET_INVAL;

	ret = socket_listen(socket_fd, backlog);

	if(ret == SOFTSIM_SOCKET_SUCCESS)
	{
		s32 err;
		pthread_t thread_id;
		
		err = pthread_create(&thread_id, NULL, softsim_socket_listen_thread,(void *)socket_fd);
		if(err == 0)  
		{
			pthread_detach(thread_id);
			softsim_set_socket_thread_id(socket_fd, SOFTSIM_SOCKET_THREAD_LISTEN, &thread_id);
		}  
	}

	return ret;
}

s32 softsim_socket_accept(s32 socket_fd, SoftsimSocketAddress *address)
{
	s32 socket = SOFTSIM_INVALID_SOCKET_FD;
	s32 ret = SOFTSIM_SOCKET_INVAL;
	char ip_str[SOFTSIM_IP_STRING_LEN];
	SoftsimSocketDomain domain;
	SoftsimSocketType type;

	domain = softsim_get_socket_domain(socket_fd);
	type = softsim_get_socket_type(socket_fd);

	softsim_memset(ip_str, 0, sizeof(ip_str));
	socket = socket_accept(socket_fd, ip_str, &(address->port));

	SOFTSIM_DEBUG("socket = %d", socket);

	if(socket >= 0)
	{
		softsim_ipstring_to_ip(ip_str, softsim_strlen(ip_str), address->address, sizeof(address->address));
		address->address_len = 4;

		SOFTSIM_DEBUG("ip[%s], port[%d]", ip_str, address->port);

		ret = softsim_insert_socket_domain_type(socket, domain, type);
		if(ret != SOFTSIM_SOCKET_SUCCESS)
		{
			softsim_socket_close(socket);
			return ret;
		}

		{
			s32 err;
			pthread_t thread_id;
			
			err =pthread_create(&thread_id, NULL, softsim_socket_delay_read_thread, (void *)socket);
			if(err == 0)  
			{
				pthread_detach(thread_id);
				softsim_set_socket_thread_id(socket, SOFTSIM_SOCKET_THREAD_READ, &thread_id);
			}  
		}
	}

	return socket;
}

s32 softsim_socket_recv(s32 socket_fd, u8 *buffer, u32 buffer_len)
{
	s32 ret = SOFTSIM_SOCKET_INVAL;

	ret = socket_recv(socket_fd, buffer, buffer_len);
	if(ret == 0)
	{
		//server closed
		SOFTSIM_DEBUG("rev server closed");
		softsim_socket_close(socket_fd);
		softsim_socket_notify_event(socket_fd, SOFTSIM_SOCKET_CLOSE);	
	}
	else if(ret != buffer_len)
	{
		s32 err;
		pthread_t thread_id;
		
		err = pthread_create(&thread_id, NULL, softsim_socket_read_thread, (void *)socket_fd);
		if(err == 0)  
		{
			pthread_detach(thread_id);
			//SOFTSIM_TRACE_HEX("thread_id: ", &thread_id, sizeof(thread_id));
			softsim_set_socket_thread_id(socket_fd, SOFTSIM_SOCKET_THREAD_READ, &thread_id);
		}  
	}

	return ret;
}

s32 softsim_socket_recv_from(s32 socket_fd, u8 *buffer, u32 buffer_len, SoftsimSocketAddress *address)
{
	s32 ret = SOFTSIM_SOCKET_INVAL;
	char ip_str[SOFTSIM_IP_STRING_LEN];

	softsim_memset(ip_str, 0, sizeof(ip_str));
	ret = socket_recv_from(socket_fd, buffer, buffer_len, ip_str, &(address->port));

	if(ret >= 0)
	{
		softsim_ipstring_to_ip(ip_str, softsim_strlen(ip_str), address->address, sizeof(address->address));
		address->address_len = 4;
	}

	return ret;
}

s32 softsim_socket_send(s32 socket_fd, u8 *data, u32 data_len)
{
	s32 ret = SOFTSIM_SOCKET_INVAL;

	ret = socket_send(socket_fd, data, data_len);
	if(ret == SOFTSIM_SOCKET_WOULDBLOCK)
	{
		s32 err;
		pthread_t thread_id;
		
		err = pthread_create(&thread_id, NULL, softsim_socket_write_thread, (void *)socket_fd);
		if(err == 0)  
		{
			pthread_detach(thread_id);
			softsim_set_socket_thread_id(socket_fd, SOFTSIM_SOCKET_THREAD_WRITE, &thread_id);
		}  
	}

	return ret;
}

s32 softsim_socket_send_to(s32 socket_fd, u8 *data, u32 data_len, SoftsimSocketAddress *address)
{
	s32 ret = SOFTSIM_SOCKET_INVAL;
	char ip_str[SOFTSIM_IP_STRING_LEN];

	softism_ip_to_ipstring(address->address, address->address_len, ip_str, sizeof(ip_str));
	ret = socket_send_to(socket_fd, data, data_len, ip_str, address->port);

	return ret;
}

s32 softsim_socket_close(s32 socket_fd)
{
	s32 ret = SOFTSIM_SOCKET_INVAL;

	if(socket_fd < 0)
	{
		return ret;
	}

	ret = socket_close(socket_fd);

	if(ret < 0)
	{
		SOFTSIM_DEBUG("socket close error!");
		return ret;
	}
	
	SOFTSIM_DEBUG("socket = %d", socket_fd);
	softsim_delete_socket(socket_fd);

	return ret;
}

s32 softsim_socket_setsockopt(s32 socket_fd, s32 level, s32 optname, const void *optval, s32 optlen)
{
	return socket_setsockopt(socket_fd, level, optname, optval, optlen);
}

s32 softsim_socket_get_host_by_name(s32 socket_fd, const char *domain_name, u8 *address, u8 *address_len)
{
	s32 ret = SOFTSIM_SOCKET_INVAL;
	char ifname[SOFTSIM_IF_NAME_LEN];

	if(softsim_ip_check(domain_name, address))
	{
		SOFTSIM_DEBUG_MARK;
		*address_len = 4;
		return SOFTSIM_SOCKET_SUCCESS;
	}

	if(softsim_dns_find_cache(domain_name, address))
	{
		SOFTSIM_DEBUG_MARK;
		*address_len = 4;
		return SOFTSIM_SOCKET_SUCCESS;
	}
	
	SOFTSIM_DEBUG_MARK;
	softsim_memset(ifname, 0, sizeof(ifname));
	softsim_socket_get_if_name(softsim_get_socket_domain(socket_fd), ifname, sizeof(ifname));
	ret = softsim_get_host_by_name(socket_fd, ifname, domain_name);
	
	return ret;
}

void softsim_socket_set_event_notify(s32 socket_fd, SoftsimSocketNotifyFunctionCallBack function_callback)
{
	softsim_set_socket_function_callback(socket_fd, function_callback);
}

void softsim_socket_set_get_host_by_name_notify(s32 socket_fd, SoftsimGetHostNameFunctionCallBack function_callback)
{
	softsim_set_socket_parse_domain_function_callback(socket_fd, function_callback);
}
