/**
 * Copyrights (C) 2016
 * All right reserved
 *
 * File : orvibo_ha_communication_
 *
 * 对接欧瑞博通信
 *
 * Change Logs:
 * Date         Author      Notes
 * 2016-12-08   wangmaopeng     Create
 */

#ifndef __ORVIBO_HA_COMMUNICATION_H__
#define __ORVIBO_HA_COMMUNICATION_H__

/**************************************************************************
*                                 INCLUDES
**************************************************************************/
#include"network_communication_.h"
#include"tool_function_.h"
#include"vihome2Aes.h"
#include"crc32.h"
#include"cJSON.h"
#include"orvibo_ha_interface.h"

#include<pthread.h>

#ifdef  __cplusplus
extern "C" {
#endif

/**************************************************************************
*                             TYPE DEFINES
**************************************************************************/
typedef struct LocalCommunicationParameter{

    char account_number[33];
    unsigned int account_number_len;
    char account_number_passwd[100];
    unsigned char local_ip[20];
    unsigned int local_port;
    int socket_flags;
}LocalCommunicationParameter;   //通信本地相关数据结构体

typedef struct ServerCommunicationParameter{

    unsigned int server_broadcast_port;
    unsigned int server_port;
    unsigned char sever_ip[20];
    char sever_sessionId[33];
    char sever_dynamic_key[20];
    char server_uid[20];
}ServerCommunicationParameter;   //通信网关相关数据结构体

#define PUBLIC_KEY "khggd54865SNJHGF"
#define PACKAGE_HEAD 0x6864
#define BROADCAST_CMD 86
#define GET_KEY_CMD 0
#define ENTER_GATEWAY_CMD 2
#define BREATHE_CMD 32
#define GET_SCENE_CMD 4
#define SCENE_CONTROL_CMD 15

#define D_PROTOCOL_TYPE 0
#define P_PROTOCOL_TYPE 1
#define SEND_DATA_LEN 1024
#define RECV_DATA_LEN 1024
#define BEST_RECV_DATA_LEN 10240

/**************************************************************************
*                                 DECLARES
**************************************************************************/
extern LocalCommunicationParameter local_communication_parameter;
extern ServerCommunicationParameter server_communication_parameter;
extern pthread_t communication_thread;
extern int temp_socketfd;
extern char temp_account_number[33];
extern char temp_account_number_passwd[100];
extern pthread_mutex_t ovribo_socket_flags_lock;

/**************************************************************************
*                               FUNCTIONS
**************************************************************************/
int  pack_ovribo_data(int protocol_type,char* secret_key,char* sessionId,char* payload,int payload_len,char* return_data);

int  analysis_ovribo_data(char *recv_data,int recv_len,cJSON **json,char *secret_key);

int  broadcast_gateway(ServerCommunicationParameter* server_parameter,LocalCommunicationParameter* local_parameter);

int  tcp_link_gateway(ServerCommunicationParameter* server_parameter,LocalCommunicationParameter* local_parameter);

int  enter_gateway(int socketfd,ServerCommunicationParameter* server_parameter,LocalCommunicationParameter* local_parameter);

void  *ovribo_communication_thread(void *arg);

int  ovribo_breathe(int socketfd,ServerCommunicationParameter* server_parameter,LocalCommunicationParameter* local_parameter);

void  set_socket_flags(int sock);

void  deal_ovribo_data();

#ifdef  __cplusplus
}
#endif

#endif  //__ORVIBO_HA_COMMUNICATION_H__
