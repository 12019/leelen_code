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
 * Filename: softsim_socket.h
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-07-16
 ******************************************************************************/
#ifndef __SOFTSIM_SOCKET_H__
#define __SOFTSIM_SOCKET_H__

#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************
* Include																	   
*****************************************************************************/


/*****************************************************************************
* Define																	   
*****************************************************************************/
#define SOFTSIM_MAX_SOCKET_NUM		30
#define SOFTSIM_MAX_ADDR_LEN 		16
#define SOFTSIM_MAX_DNS_CACHE		16	
#define SOFTSIM_INVALID_SOCKET_FD	(-1)	 
/*****************************************************************************
* Typedef																	   
*****************************************************************************/
typedef enum
{
	SOFTSIM_SOCKET_READ    = 0x01,	/* Notify for read */
	SOFTSIM_SOCKET_WRITE   = 0x02,	/* Notify for write */
	SOFTSIM_SOCKET_ACCEPT  = 0x04,	/* Notify for accept */
	SOFTSIM_SOCKET_CONNECT = 0x08,	/* Notify for connect */
	SOFTSIM_SOCKET_CLOSE   = 0x10	/* Notify for close */
}SoftsimSocketEvent;

typedef enum
{
	SOFTSIM_SOCKET_SUCCESS 		  		= 0,
	SOFTSIM_SOCKET_ERROR			  	= -1,
	SOFTSIM_SOCKET_WOULDBLOCK		  	= -2,
	SOFTSIM_SOCKET_LIMIT_RESOURCE	  	= -3,    /* limited resource */
	SOFTSIM_SOCKET_INVALID_SOCKET	  	= -4,    /* invalid socket */
	SOFTSIM_SOCKET_INVALID_ACCOUNT   	= -5,    /* invalid account id */
	SOFTSIM_SOCKET_NAMETOOLONG 	  		= -6,    /* address too long */
	SOFTSIM_SOCKET_ALREADY 		  		= -7,    /* operation already in progress */
	SOFTSIM_SOCKET_OPNOTSUPP		  	= -8,    /* operation not support */
	SOFTSIM_SOCKET_CONNABORTED 	  		= -9,    /* Software caused connection abort */
	SOFTSIM_SOCKET_INVAL			  	= -10,   /* invalid argument */
	SOFTSIM_SOCKET_PIPE			  		= -11,   /* broken pipe */
	SOFTSIM_SOCKET_NOTCONN 		  		= -12,   /* socket is not connected */
	SOFTSIM_SOCKET_MSGSIZE 		  		= -13,   /* msg is too long */
	SOFTSIM_SOCKET_BEARER_FAIL 	  		= -14,   /* bearer is broken */
	SOFTSIM_SOCKET_CONNRESET		  	= -15,   /* TCP half-write close, i.e., FINED */
	SOFTSIM_SOCKET_DHCP_ERROR		  	= -16,
	SOFTSIM_SOCKET_IP_CHANGED		  	= -17,
	SOFTSIM_SOCKET_ADDRINUSE		  	= -18,
	SOFTSIM_SOCKET_CANCEL_ACT_BEARER 	= -19,	/* cancel the activation of bearer */
	SOFTSIM_SOCKET_GETHOSTBYNAME_FAILED = -20	  /* gethostbyname failed */
}SoftsimSocketErrorCode;

typedef enum 
{
	SOFTSIM_SOCKET_PF_INET_PHYSICALSIM = 0,
	SOFTSIM_SOCKET_PF_INET_SOFTSIM,
	SOFTSIM_SOCKET_PF_LOCAL,
	SOFTSIM_SOCKET_PF_WIFI,
	SOFTSIM_SOCKET_PF_UART,
	SOFTSIM_SOCKET_PF_RAW,
	SOFTSIM_SOCKET_DOMAIN_MAX = 0x1fffffff
}SoftsimSocketDomain;
	
typedef enum 
{
	SOFTSIM_SOCKET_STREAM = 0,
	SOFTSIM_SOCKET_DGRAM,
	SOFTSIM_SOCKET_RAW,
	SOFTSIM_SOCKET_TYPE_MAX = 0x1fffffff
}SoftsimSocketType;

typedef enum 
{
	SOFTSIM_ADDRESS_TYPE_IP = 0,
	SOFTSIM_ADDRESS_TYPE_DOMAIN,
	SOFTSIM_ADDRESS_TYPE_MAX = 0x1fffffff
}SoftsimAddressType;


typedef struct
{
	bool   					result; 		
	s32         			socket_fd; 		
	SoftsimSocketEvent  	event_type;   	
	SoftsimSocketErrorCode  error_code;  	/* used only when EVENT is close/connect */
	s32       				detail_error; 	/*  */
}SoftsimSocketNotify;

typedef struct
{
    bool   					result;
	s32         			socket_fd;
    u8						address_len;
    u8						address[SOFTSIM_MAX_ADDR_LEN];   /* for backward compatibility */
    /* New for DNS query reply for reporting bearer fail */
    SoftsimSocketErrorCode  error_code;   						/* bearer fail */
    s32   					detail_error;  	
}SoftsimSocketGetHostNameNotify;

typedef struct 
{
	SoftsimSocketType	type;
	SoftsimAddressType	address_type;
	char 		domain[SOFTSIM_DOMAIN_LEN];
	u8			address_len;
    u8			address[SOFTSIM_MAX_ADDR_LEN];
    u16			port;
}SoftsimSocketAddress;

typedef void (*SoftsimSocketNotifyFunctionCallBack)(SoftsimSocketNotify *notify);
typedef void (*SoftsimGetHostNameFunctionCallBack)(SoftsimSocketGetHostNameNotify *notify);
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
extern void softsim_socket_init(void);
extern s32 softsim_socket_create(SoftsimSocketDomain domain, SoftsimSocketType type);
extern s32 softsim_socket_connect(s32 socket_fd, SoftsimSocketAddress *address);
extern s32 softsim_socket_bind(s32 socket_fd, SoftsimSocketAddress *address);
extern s32 softsim_socket_listen(s32 socket_fd, u32 backlog);
extern s32 softsim_socket_accept(s32 socket_fd, SoftsimSocketAddress *address);
extern s32 softsim_socket_recv(s32 socket_fd, u8 *buffer, u32 buffer_len);
extern s32 softsim_socket_recv_from(s32 socket_fd, u8 *buffer, u32 buffer_len, SoftsimSocketAddress *address);
extern s32 softsim_socket_send(s32 socket_fd, u8 *data, u32 data_len);
extern s32 softsim_socket_send_to(s32 socket_fd, u8 *data, u32 data_len, SoftsimSocketAddress *address);
extern s32 softsim_socket_close(s32 socket_fd);
extern s32 softsim_socket_setsockopt(s32 socket_fd, s32 level, s32 optname, const void *optval, s32 optlen);
extern s32 softsim_socket_get_host_by_name(s32 socket_fd, const char *domain_name, u8 *address, u8 *address_len);
extern void softsim_socket_set_event_notify(s32 socket_fd, SoftsimSocketNotifyFunctionCallBack function_callback);
extern void softsim_socket_set_get_host_by_name_notify(s32 socket_fd, SoftsimGetHostNameFunctionCallBack function_callback);
/****************************************************************************
* Function
****************************************************************************/
#ifdef __cplusplus
		}
#endif

#endif /*__SOFTSIM_SOCKET_H__*/


