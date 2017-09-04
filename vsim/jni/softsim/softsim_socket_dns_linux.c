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
 * Filename: softsim_socket_dns_linux.h
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-07-23
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
#define SOFTSIM_TRACE_HEADER			"[DNS]" 
/*****************************************************************************
* Typedef																	   
*****************************************************************************/
typedef struct
{
	s32 socket_fd;
	char ifname[SOFTSIM_IF_NAME_LEN];
	char domain_name[SOFTSIM_DOMAIN_LEN];
}SoftsimSocketDomainParam;
/*****************************************************************************
* Static Local Variable															  
*****************************************************************************/
static SoftsimGetHostNameFunctionCallBack parse_domain_fun = NULL;
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
static void val_property_get(char* key, char* value)
{
	if((key == NULL)||(value == NULL))
	{
		return;
	}

	#if defined(__SOFTSIM_ANDROID__)
		__system_property_get(key, value);
	#endif
}


static s32 softsim_socket_dns_get_map_error_code(void)
{
	s32 err_code = SOFTSIM_SOCKET_ERROR;
	s32 err_no = (s32)errno;
	
	SOFTSIM_DEBUG("err_no = %d", err_no);

	switch(err_no)
	{
		case EAGAIN:
		case EINPROGRESS:
			err_code = SOFTSIM_SOCKET_WOULDBLOCK;
			break;
		case EPIPE:
			err_code = SOFTSIM_SOCKET_PIPE;
			break;
		//to be add more map
		default:
			err_code = SOFTSIM_SOCKET_ERROR;
			break;
	}

	return err_code;
}

static void softsim_socket_parse_domain_err_cb(s32 socket_fd, SoftsimSocketErrorCode error_code)
{
	SoftsimSocketGetHostNameNotify notify;

	SOFTSIM_DEBUG("");
	
	if(NULL == parse_domain_fun)
	{
		return;
	}

	softsim_memset(&notify, 0, sizeof(notify));
	notify.result = false;
	notify.socket_fd = socket_fd;
	notify.error_code = error_code;
	notify.detail_error = (s32)errno;
	
	parse_domain_fun(&notify);
}

static void softsim_socket_parse_domain_success(s32 socket_fd, char *ip_string)
{
	SoftsimSocketGetHostNameNotify notify;
	
	if(NULL == parse_domain_fun)
	{
		return;
	}

	softsim_memset(&notify, 0, sizeof(notify));
	notify.result = true;
	notify.socket_fd = socket_fd;
	softsim_ipstring_to_ip(ip_string, softsim_strlen(ip_string), notify.address, sizeof(notify.address));
	notify.address_len = 4;
	notify.error_code = 0;
	notify.detail_error = (s32)0;
	
	parse_domain_fun(&notify);
}
 
/**
 * Check whether the current byte is 
 * a dns pointer or a length
 */
static int softsim_dns_is_pointer(int in)
{
	return ((in & 0xc0) == 0xc0);
}
 
/**
 * Parse data chunk into dns name
 * @param chunk The complete response chunk
 * @param ptr The pointer points to data
 * @param out This will be filled with dns name
 * @param len This will be filled with the length of dns name
 */
static void softsim_socket_parse_dns_name(unsigned char *chunk, unsigned char *ptr , char *out , int *len)
{
	int n, flag;
	char *pos = out + (*len);
 
	for(;;){
		flag = (int)ptr[0];
		if(flag == 0)
			break;
		if(softsim_dns_is_pointer(flag)){
			n = (int)ptr[1];
			ptr = chunk + n;
			softsim_socket_parse_dns_name(chunk , ptr , out , len);
			break;
		}else{
			ptr ++;
			softsim_memcpy(pos , ptr , flag);	
			pos += flag;
			ptr += flag;
			*len += flag;
			if((int)ptr[0] != 0){
				softsim_memcpy(pos , "." , 1);
				pos += 1;
				(*len) += 1;
			}
		}
	}
}
 
static void softsim_socket_parse_dns_response(s32 socket_fd, int socketfd)
{
	unsigned char buf[512];
	unsigned char *ptr = buf;
	struct sockaddr_in addr;
	//char *src_ip;
	int n , i , flag , querys , answers;
	int type , ttl , datalen , len;
	char cname[128] , aname[128] , ip[20];
	unsigned char netip[4];
	socklen_t addr_len = sizeof(struct sockaddr_in);
	bool first_ip = true; //we only get the first ip, ignore the others

	SOFTSIM_DEBUG("socketfd = %d, socket_fd = %d", socketfd, socket_fd);
	n = recvfrom(socketfd , buf , sizeof(buf) , 0, (struct sockaddr *)&addr , &addr_len);
	if(n < 0)
	{
		SOFTSIM_DEBUG("dns error (%d) reason = %s", n, strerror(errno));
		softsim_socket_parse_domain_err_cb(socket_fd, (SoftsimSocketErrorCode)softsim_socket_dns_get_map_error_code());
		return;
	}
	SOFTSIM_DEBUG("");
	ptr += 4; /* move ptr to Questions */
	querys = ntohs(*((unsigned short*)ptr));
	ptr += 2; /* move ptr to Answer RRs */
	answers = ntohs(*((unsigned short*)ptr));
	ptr += 6; /* move ptr to Querys */
	/* move over Querys */
	for(i= 0 ; i < querys ; i ++){
		for(;;){
			flag = (int)ptr[0];
			ptr += (flag + 1);
			if(flag == 0)
				break;
		}
		ptr += 4;
	}
	/* now ptr points to Answers */
	for(i = 0 ; i < answers ; i ++){
		bzero(aname , sizeof(aname));
		len = 0;
		softsim_socket_parse_dns_name(buf , ptr , aname , &len);
		ptr += 2; /* move ptr to Type*/
		type = htons(*((unsigned short*)ptr));
		ptr += 4; /* move ptr to Time to live */
		ttl = htonl(*((unsigned int*)ptr));
		ptr += 4; /* move ptr to Data lenth */
		datalen = ntohs(*((unsigned short*)ptr));
		ptr += 2; /* move ptr to Data*/
		if(type == SOFTSIM_SOCKET_DNS_CNAME){
			bzero(cname , sizeof(cname));
			len = 0;
			softsim_socket_parse_dns_name(buf , ptr , cname , &len);
			SOFTSIM_DEBUG("%s is an alias for %s" , aname , cname);
			ptr += datalen;
		}
		if(type == SOFTSIM_SOCKET_DNS_HOST){
			bzero(ip , sizeof(ip));
			if(datalen == 4){
				softsim_memcpy(netip , ptr , datalen);
				inet_ntop(AF_INET , netip , ip , sizeof(struct sockaddr));
				SOFTSIM_DEBUG("%s has address %s" , aname , ip);
				if(first_ip)
				{
					first_ip = false;
					softsim_socket_parse_domain_success(socket_fd, ip);
				}
				
				SOFTSIM_DEBUG("Time to live: %d minutes , %d seconds"
						, ttl / 60 , ttl % 60);
					
			}
			ptr += datalen;
		}
 
	}
	ptr += 2;
}

/**
 * Generate DNS question chunk
 */
static void softsim_socket_dns_generate_question(const char *dns_name , unsigned char *buffer , int *len)
{
	char *pos;
	unsigned char *ptr;
	int n;
 
	*len = 0;
	ptr = buffer;	
	pos = (char*)dns_name; 
	for(;;){
		n = strlen(pos) - (strstr(pos , ".") ? strlen(strstr(pos , ".")) : 0);
		*ptr ++ = (unsigned char)n;
		softsim_memcpy(ptr , pos , n);
		*len += n + 1;
		ptr += n;
		if(!strstr(pos , ".")){
			*ptr = (unsigned char)0;
			ptr ++;
			*len += 1;
			break;
		}
		pos += n + 1;
	}
	*((unsigned short*)ptr) = htons(1);
	*len += 2;
	ptr += 2;
	*((unsigned short*)ptr) = htons(1);
	*len += 2;
}
 
static void softsim_socket_send_dns_request(int socket_fd, struct sockaddr_in *dest, const char *dns_name)
{
	unsigned char request[512];
	unsigned char *ptr = request;
	unsigned char question[256];
	int question_len, ret;
 
 	softsim_memset(request, 0, sizeof(request));
 	softsim_memset(question, 0, sizeof(question));
 	SOFTSIM_DEBUG("domain = %s, dns ip = %s", dns_name, inet_ntoa(dest->sin_addr));
	softsim_socket_dns_generate_question(dns_name , question , &question_len);
 	SOFTSIM_DEBUG("question_len = %d", question_len);
	*((unsigned short*)ptr) = htons(0xff00);
	ptr += 2;
	*((unsigned short*)ptr) = htons(0x0100);
	ptr += 2;
	*((unsigned short*)ptr) = htons(1);
	ptr += 2;
	*((unsigned short*)ptr) = 0;
	ptr += 2;
	*((unsigned short*)ptr) = 0;
	ptr += 2;
	*((unsigned short*)ptr) = 0;
	ptr += 2;
	softsim_memcpy(ptr , question , question_len);
	ptr += question_len;
	ret = sendto(socket_fd , request , question_len + 12 , 0
	   , (struct sockaddr *)dest , sizeof(struct sockaddr));
	SOFTSIM_DEBUG("socket_fd = %d, ret = %d, error = %s", socket_fd, ret, strerror(errno));
}

static void * softsim_socket_dns_get_host_name_thread(void *arg)
{
	SoftsimSocketDomainParam arg_info;
	SoftsimSocketDomainParam *param = (SoftsimSocketDomainParam *)arg;
	int socketfd = -1, ret;
	struct sockaddr_in dest, dest_bak;
	char bind_ifname[SOFTSIM_IF_NAME_LEN];
	struct timeval tv_out;
	

	softsim_memset(&arg_info, 0, sizeof(arg_info));
	softsim_memcpy(&arg_info, param, sizeof(SoftsimSocketDomainParam));
	param = &arg_info;

	socketfd = socket(AF_INET , SOCK_DGRAM , 0);
	if(socketfd < 0)
	{
		SOFTSIM_DEBUG("create socket failed");
		softsim_socket_parse_domain_err_cb(param->socket_fd, socketfd);
		return NULL;
	}
	
	tv_out.tv_sec = SOFTSIM_SOCKET_DNS_TIMEOUT;
	tv_out.tv_usec = 0;
	setsockopt(socketfd, SOL_SOCKET, SO_SNDTIMEO, &tv_out, sizeof(tv_out));
	setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));

	softsim_memset(bind_ifname, 0, sizeof(bind_ifname));
	softsim_strcpy(bind_ifname, param->ifname);
	if( 0 == softsim_strcmp(bind_ifname, SOFTSIM_IFNAME_NET_SOFTSIM))
		ret = 0;
	else
		ret = socket_bind_device(socketfd, param->ifname);
	do
	{
		if(ret < 0)
		{
			SOFTSIM_DEBUG("dns bind failed");
			softsim_socket_parse_domain_err_cb(param->socket_fd, ret);
			return NULL;
		}
	} 
	while(0);

	bzero(&dest , sizeof(dest));
	bzero(&dest_bak, sizeof(dest_bak));
	
	dest.sin_family = AF_INET;
	dest.sin_port = htons(53);
	
	dest_bak.sin_family = AF_INET;
	dest_bak.sin_port = htons(53);
	
	//dest.sin_addr.s_addr = inet_addr(SOC_DNS_BACKUP);
	{
		char prop_value[PROP_VALUE_MAX_LEN];

		if(0 == softsim_strcmp(bind_ifname, SOFTSIM_IFNAME_NET_PHYSICALSIM))
			val_property_get(PROP_RSIM_DNS, prop_value);
		else
			val_property_get(PROP_DNS, prop_value);
		SOFTSIM_DEBUG("PROP_DNS = %s", prop_value);
		if(softsim_strlen(prop_value) > 0)
		{
			dest.sin_addr.s_addr = inet_addr(prop_value);
		}
		else
		{
			dest.sin_addr.s_addr = inet_addr(SOFTSIM_SOCKET_DNS_BACKUP1);
		}
	}

	dest_bak.sin_addr.s_addr = inet_addr(SOFTSIM_SOCKET_DNS_BACKUP2);
	softsim_socket_send_dns_request(socketfd, &dest, param->domain_name);
	softsim_socket_send_dns_request(socketfd, &dest_bak, param->domain_name);
	softsim_socket_parse_dns_response(param->socket_fd, socketfd);
	close(socketfd);
	return NULL;
}

// =====================================================================

s32 softsim_get_host_by_name(s32 socket_fd, const char * ifname, const char *domain_name)
{
	static SoftsimSocketDomainParam para;

	softsim_memset(&para, 0, sizeof(para));
	para.socket_fd = socket_fd;
	softsim_strcpy(para.ifname, ifname);
	softsim_strcpy(para.domain_name, domain_name);

	{
		s32 err;
		pthread_t thread_id;
		
		err = pthread_create(&thread_id, NULL, softsim_socket_dns_get_host_name_thread,(void *)&para);
		if(err == 0)  
		{
			pthread_detach(thread_id);
		}  
	}
	
	return SOFTSIM_SOCKET_WOULDBLOCK;
}

void softsim_socket_dns_init(SoftsimGetHostNameFunctionCallBack parse_domain_notify)
{
	parse_domain_fun = parse_domain_notify;
}


