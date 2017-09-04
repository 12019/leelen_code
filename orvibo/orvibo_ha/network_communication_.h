/**
 * Copyrights (C) 2016
 * All right reserved
 *
 * File : network_communication_
 *
 * 网络通信接口
 *
 * Change Logs:
 * Date         Author      Notes
 * 2016-12-08   wangmaopeng     Create
 */

#ifndef __NETWORK_COMMUNICATION_
#define __NETWORK_COMMUNICATION_

/**************************************************************************
*                                 INCLUDES
**************************************************************************/
#include"tool_function_.h"

#include<errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
*                                 DECLARES
**************************************************************************/

/**************************************************************************
*                             TYPE DEFINES
**************************************************************************/

/**************************************************************************
*                               FUNCTIONS
**************************************************************************/

int  client_broadcast_socekt_(unsigned char* local_ip,int local_port);

int  client_broadcast_send_(int socket_fd,int server_port,char* data,int len);

int  client_broadcast_recv_(int socket_fd,int sec,int usec,struct sockaddr_in* server_ip,char* data,int len);

//int  server_broadcast_socekt_();

int  client_tcp_socekt_(unsigned char* local_ip,int local_port,unsigned char* server_ip,int server_port);

int  client_tcp_send_(int socket_fd,char* data,int len);

int  client_tcp_recv_(int socket_fd,int sec,int usec,char* data,int len);

//int  server_tcp_socekt_();

#ifdef __cplusplus
}
#endif

#endif //__NETWORK_COMMUNICATION_
