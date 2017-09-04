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
 * Filename: softsim_socket_linux.c
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

static s32 socket_get_map_domain(SoftsimSocketDomain domain)
{
	s32 ret;
	
	switch(domain)
	{
		case SOFTSIM_SOCKET_PF_INET_PHYSICALSIM:
		case SOFTSIM_SOCKET_PF_INET_SOFTSIM:
		case SOFTSIM_SOCKET_PF_LOCAL:
		case SOFTSIM_SOCKET_PF_WIFI:
			ret = PF_INET;
			break;
			
		default:
			ret = PF_UNIX;
			break;
	}

	return ret;
}

static s32 socket_get_map_type(SoftsimSocketType type)
{
	s32 ret;
	
	switch(type)
	{
		case SOFTSIM_SOCKET_STREAM:
			ret = SOCK_STREAM;
			break;
			
		case SOFTSIM_SOCKET_DGRAM:
			ret = SOCK_DGRAM;
			break;
			
		case SOFTSIM_SOCKET_RAW:
			ret = SOCK_RAW;
			break;
			
		default:
			ret = SOCK_STREAM;
			break;
	}	
	
	return ret;
}

s32 socket_map_ret_error_code(s32 ret)
{
	s32 error_code = SOFTSIM_SOCKET_ERROR;
	s32 error_no = (s32)errno;
	
	SOFTSIM_DEBUG("ret = %d, error_no = %d, error: %s", ret, error_no, strerror(error_no));
	
	if(ret >= 0)
	{
		return ret;
	}

	//SOFTSIM_DEBUG("EAGAIN = %d, EINPROGRESS = %d, EPIPE = %d", EAGAIN, EINPROGRESS, EPIPE);

	switch(error_no)
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
			//SOFTSIM_DEBUG("error = %s", strerror(error_no));
			break;
	}

	//SOFTSIM_DEBUG("err_code = %d", error_code);

	return error_code;
}

s32 socket_bind_device(s32 socket_fd, char *if_name)
{
	s32 ret;
	struct ifreq if_remnet0;

	softsim_strncpy(if_remnet0.ifr_name, if_name, IFNAMSIZ);
	SOFTSIM_DEBUG("interface_name : %s", if_remnet0.ifr_name);
	ret = setsockopt(socket_fd, SOL_SOCKET, SO_BINDTODEVICE, (char *)&if_remnet0, sizeof(if_remnet0));

	return socket_map_ret_error_code(ret);
}

s32 socket_create(SoftsimSocketDomain domain, SoftsimSocketType type)
{
	s32	socket_fd = -1; 
    s32 flags;

	socket_fd = socket(socket_get_map_domain(domain), socket_get_map_type(type), 0);

	if(socket_fd < 0)
	{
		return socket_fd;
	}
	
	flags = fcntl(socket_fd, F_GETFL, 0);
	fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);

	return socket_fd;
}

s32 socket_connect(s32 socket_fd, const char *ip, u16 port)
{
	s32 ret;
    struct sockaddr_in addr;

	SOFTSIM_DEBUG("socket_connect socket_fd:%d, IP[%s], PORT[%d]", socket_fd, ip, port);

    softsim_memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
	bzero(&(addr.sin_zero), 8);
    ret = connect(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
	
	return socket_map_ret_error_code(ret);
}

s32 socket_bind(s32 socket_fd, const char *ip, u16 port)
{
	s32 ret, opt = 1;
	struct sockaddr_in addr;

	SOFTSIM_DEBUG("socket_fd: %d, ip[%s], port[%d]", socket_fd, ip, port);

	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	ret = bind(socket_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr));

	return socket_map_ret_error_code(ret);
}

s32 socket_listen(s32 socket_fd, u32 backlog)
{
	s32 ret;
	ret = listen(socket_fd, backlog);

	return socket_map_ret_error_code(ret);
}

s32 socket_accept(s32 socket_fd, char *ip, u16 *port)
{
	s32 ret;
	struct sockaddr_in address;
	socklen_t addrlen = (socklen_t)sizeof(address);
	s32 child_socket;

	child_socket = accept(socket_fd, (struct sockaddr *)&address, &addrlen);
	if(child_socket < 0)
	{
		ret = child_socket;
		return socket_map_ret_error_code(ret);
	}

	softsim_strcpy(ip, inet_ntoa(address.sin_addr));
	*port = ntohs(address.sin_port);

	return child_socket;
}

s32 socket_recv(s32 socket_fd, u8 *buffer, u32 buffer_len)
{
	s32 ret;

	do
	{
		ret = recv(socket_fd, buffer, buffer_len, 0);
	} 
	while ((ret == -1) && (errno == EINTR));

	return socket_map_ret_error_code(ret);
}

s32 socket_recv_from(s32 socket_fd, u8 *buffer, u32 buffer_len, char *ip, u16 *port)
{
	s32 ret;
	struct sockaddr_in remote_addr;
	socklen_t remote_addrlen;
	
	ret = recvfrom(socket_fd, buffer, buffer_len, 0, (struct sockaddr *)(&remote_addr), &remote_addrlen);
	if(ret > 0)
	{
		softsim_strcpy(ip, inet_ntoa(remote_addr.sin_addr));
		*port = ntohs(remote_addr.sin_port);
	}

	return socket_map_ret_error_code(ret);
}

s32 socket_send(s32 socket_fd, u8 *data, u32 data_len)
{
	s32 ret;

	SOFTSIM_DEBUG("socket_fd = %d", socket_fd);
	
	do
	{
		ret = send(socket_fd, data, data_len, 0);
	} 
	while ((ret == -1) && (errno == EINTR));

	return socket_map_ret_error_code(ret);
}

s32 socket_send_to(s32 socket_fd, u8 *data, u32 data_len, const char *ip, u16 port)
{
	s32 ret;
	
	struct sockaddr_in addr;

	bzero(&addr , sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	ret = sendto(socket_fd, data, data_len, 0, (struct sockaddr *)(&addr), sizeof(struct sockaddr_in));

	return socket_map_ret_error_code(ret);
}

s32 socket_close(s32 socket_fd)
{
	s32 ret;
	
	ret = close(socket_fd);

	return socket_map_ret_error_code(ret);
}

s32 socket_setsockopt(s32 socket_fd, s32 level, s32 optname, const void *optval, s32 optlen)
{
	s32 ret;
	
	ret = setsockopt(socket_fd, level, optname, optval, optlen);

	return socket_map_ret_error_code(ret);
}
