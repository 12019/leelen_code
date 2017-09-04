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
 * Filename: softsim_socket_linux.h
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-07-16
 ******************************************************************************/
#ifndef __SOFTSIM_SOCKET_LINUX_H__
#define __SOFTSIM_SOCKET_LINUX_H__

#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************
* Include																	   
*****************************************************************************/
#include "softsim_include.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h> 
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <pthread.h>

/*****************************************************************************
* Define																	   
*****************************************************************************/
#ifndef IFNAMSIZ
#define IFNAMSIZ    (16)
#endif

#define SOFTSIM_IFNAME_NET_WLAN       	"wlan0"
#define SOFTSIM_IFNAME_NET_PHYSICALSIM  "ppp0"
#if defined(__SOFTSIM_T3__)
#define SOFTSIM_IFNAME_NET_SOFTSIM      "rmnet0"
#else
#define SOFTSIM_IFNAME_NET_SOFTSIM      "ccmni0"
#endif
#define SOFTSIM_IFNAME_LOCAL_DEVICE   	"lo"

#define SOFTSIM_SOCKET_DNS_BACKUP1 				"8.8.8.8"
#define SOFTSIM_SOCKET_DNS_BACKUP2 				"223.5.5.5"
#define SOFTSIM_SOCKET_DNS_BACKUP3 				"223.6.6.6"

#define SOFTSIM_SOCKET_DNS_HOST  				0x01
#define SOFTSIM_SOCKET_DNS_CNAME 				0x05
#define SOFTSIM_SOCKET_DNS_TIMEOUT				(10)

#define SOFTSIM_SOCKET_SELECT_TIMEOUT				(5)

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
extern s32 socket_map_ret_error_code(s32 ret);
extern s32 socket_bind_device(s32 socket_fd, char *if_name);
extern s32 socket_create(SoftsimSocketDomain domain, SoftsimSocketType type);
extern s32 socket_connect(s32 socket_fd, const char *ip, u16 port);
extern s32 socket_bind(s32 socket_fd, const char *ip, u16 port);
extern s32 socket_listen(s32 socket_fd, u32 backlog);
extern s32 socket_accept(s32 socket_fd, char *ip, u16 *port);
extern s32 socket_recv(s32 socket_fd, u8 *buffer, u32 buffer_len);
extern s32 socket_recv_from(s32 socket_fd, u8 *buffer, u32 buffer_len, char *ip, u16 *port);
extern s32 socket_send(s32 socket_fd, u8 *data, u32 data_len);
extern s32 socket_send_to(s32 socket_fd, u8 *data, u32 data_len, const char *ip, u16 port);
extern s32 socket_close(s32 socket_fd);
extern s32 socket_setsockopt(s32 socket_fd, s32 level, s32 optname, const void *optval, s32 optlen);
/****************************************************************************
* Function
****************************************************************************/

#ifdef __cplusplus
		}
#endif

#endif /*__SOFTSIM_SOCKET_LINUX_H__*/


