#include"orvibo_ha_communication.h"

LocalCommunicationParameter local_communication_parameter;
ServerCommunicationParameter server_communication_parameter;
pthread_t communication_thread;

int temp_socketfd;
char temp_account_number[33];
char temp_account_number_passwd[100];
pthread_mutex_t ovribo_socket_flags_lock;

char msg_buffer[BEST_RECV_DATA_LEN];
int msg_buffer_head;
int msg_buffer_len;

extern ReturnOvriboData ovribo_data_pf;

int  pack_ovribo_data(int protocol_type,char* secret_key,char* sessionId,char* payload,int payload_len,char* return_data)
{
    int i=0;
    int return_data_len=0;
    int secret_data_len=0;
    uint32_t crc_key=0;

    show_log_char_("pack_ovribo_data",payload);
    char secret_payload[SEND_DATA_LEN];  //加密数据并CRC校验
    memset(secret_payload,0,SEND_DATA_LEN*sizeof(char));
    secret_data_len=ViHome2AesEncryptTo(0,payload,payload_len,secret_key,secret_payload);
    crc_key = crc_check(secret_payload, secret_data_len);
    return_data_len = secret_data_len+42;

    return_data[0]=(PACKAGE_HEAD&0xFF00)>>8;  //设置包头
    return_data[1]=PACKAGE_HEAD&0xFF;

    return_data[2]=(return_data_len&0xFF00)>>8;   //设置长度
    return_data[3]=return_data_len&0xFF;

    if(D_PROTOCOL_TYPE==protocol_type) {      //设置解密类型

        return_data[4]='d';
        return_data[5]='k';
    }
    else  if(P_PROTOCOL_TYPE==protocol_type) {

        return_data[4]='p';
        return_data[5]='k';
    }

    return_data[6]=(crc_key>>24)&0xFF;  //CRC校验
    return_data[7]=(crc_key>>16)&0xFF;
    return_data[8]=(crc_key>>8)&0xFF;
    return_data[9]=crc_key&0xFF;

    for(i=0;i<32;i++) {  //设置会话ID

        return_data[i+10]=sessionId[i];
    }

    for(i=0;i<secret_data_len;i++) {  //设置有效数据

        return_data[i+42]=secret_payload[i];
    }

    show_log_hex_("pack_ovribo_data:",return_data,return_data_len);
    return return_data_len;
}

int  analysis_ovribo_data(char *recv_data,int recv_len,cJSON **json,char *secret_key)
{
    int i=0;
    int len=0;
    uint32_t crc_key1=0;
    uint32_t crc_key2=0;
    char *payload;
    char *decode_payload;
    int decode_payload_len=0;

    show_log_hex_("analysis_ovribo_data:",recv_data,recv_len);
    len = ((recv_data[2]<<8)&0xFF00)+(recv_data[3]&0xFF);
    if(len!=recv_len) {

        printf("analysis_ovribo_data recv_len error\n");
        return -1;
    }

    payload=(char*)malloc((recv_len+1)*sizeof(char));
    decode_payload=(char*)malloc((recv_len+1)*sizeof(char));
    memset(payload,0,(recv_len+1)*sizeof(char));
    memset(decode_payload,0,(recv_len+1)*sizeof(char));

    for(i=0;i<recv_len-42;i++) { payload[i]=recv_data[42+i]; }

    crc_key1 = ((recv_data[6]<<24)&0xFF000000)+((recv_data[7]<<16)&0xFF0000)+((recv_data[8]<<8)&0xFF00)+(recv_data[9]&0xFF);
    crc_key2 = crc_check(payload,recv_len-42);
    if(crc_key1!=crc_key2) {

        printf("analysis_ovribo_data crc_key error\n");
        return -1;
    }

    decode_payload_len = ViHome2AesDecryptTo(0,payload,recv_len-42,secret_key,decode_payload);
    show_log_char_("analysis_ovribo_data:",decode_payload);

    *json = cJSON_Parse(decode_payload);
    if(NULL == *json) {

        printf("analysis_ovribo_data json error\n");
        return -1;
    }

    free(payload);
    free(decode_payload);
    return decode_payload_len;
}

int  broadcast_gateway(ServerCommunicationParameter* server_parameter,LocalCommunicationParameter* local_parameter)
{
    int i=0;
    int socketfd=-1;
    int recv_len;
    unsigned char local_ip[20];
    struct sockaddr_in server_addr;
    char payload[50]={"{\"cmd\":86,\"serial\":0}"};
    char send_data[SEND_DATA_LEN];
    int return_len=0;
    char key[20]={"khggd54865SNJHGF"};
    char sessionId[33];
    char recv_data[RECV_DATA_LEN+1];
    cJSON *json=NULL;
    cJSON *json_cmd=NULL;
    cJSON *json_status=NULL;
    cJSON *json_uid=NULL;
    cJSON *json_servicePort=NULL;

    memset(local_ip,0,20*sizeof(char));
    if(-1 == get_wlan0_ip_addr_(local_ip)==-1) {

        printf("get_wlan0_ip_addr_ error\n");
        return -1;
    }

    socketfd = client_broadcast_socekt_(local_ip,local_parameter->local_port);
    if(-1 == socketfd) {

        printf("client_broadcast_socekt_ error\n");
        return -1;
    }

    show_log_int_("client_broadcast_socekt_",socketfd);

    for(i=0;i<32;i++) {   sessionId[i]=0x30; }
    memset(send_data,0,SEND_DATA_LEN*sizeof(char));
    return_len = pack_ovribo_data(P_PROTOCOL_TYPE,key,sessionId,payload,strlen(payload),send_data);

    for(i=0;i<4;i++) {

        if(-1==(client_broadcast_send_(socketfd,server_parameter->server_broadcast_port,send_data,return_len))) {

            printf("client_broadcast_send_ error\n");
            continue;
        }

        recv_len=-1;
        memset(recv_data,0,(RECV_DATA_LEN+1)*sizeof(char));
        recv_len = client_broadcast_recv_(socketfd,0,250000,&server_addr,recv_data,RECV_DATA_LEN);

        if(-1==recv_len) {

            printf("client_broadcast_recv_ error\n");
            continue;
        }

        json=NULL;
        if(-1==(analysis_ovribo_data(recv_data,recv_len,&json,key))) {

            printf("analysis_ovribo_data error\n");
            continue;
        }

        if(NULL==json) {

            printf("NULL==json error\n");
            continue;
        }

        json_cmd = cJSON_GetObjectItem(json, "cmd");
        json_status = cJSON_GetObjectItem(json, "status");
        json_uid = cJSON_GetObjectItem(json, "uid");
        json_servicePort = cJSON_GetObjectItem(json, "servicePort");

        if(json_cmd==NULL||json_cmd->type!=cJSON_Number||json_cmd->valueint!=BROADCAST_CMD) {

            printf("json_cmd error\n");
            cJSON_Delete(json);
            continue;
        }

        if(json_status==NULL||json_status->type!=cJSON_Number||json_status->valueint!=0) {

            printf("json_status error\n");
            cJSON_Delete(json);
            continue;
        }

        if(json_servicePort==NULL||json_servicePort->type!=cJSON_Number) {

            printf("json_servicePort error\n");
            cJSON_Delete(json);
            continue;
        }
        server_parameter->server_port=json_servicePort->valueint;

        if(json_uid==NULL||json_uid->type!=cJSON_String) {

            printf("json_uid error\n");
            cJSON_Delete(json);
            continue;
        }
        memset(server_parameter->server_uid,0,20*sizeof(char));
        strcpy(server_parameter->server_uid,json_uid->valuestring);

        memset(server_parameter->sever_ip,0,20*sizeof(char));
        strcpy(server_parameter->sever_ip,inet_ntoa(server_addr.sin_addr));
        memset(local_parameter->local_ip,0,20*sizeof(char));
        strcpy(local_parameter->local_ip,local_ip);

        cJSON_Delete(json);
        close(socketfd);
        return 0;
    }

    close(socketfd);
    return -1;
}

int  tcp_link_gateway(ServerCommunicationParameter* server_parameter,LocalCommunicationParameter* local_parameter)
{
    int socketfd=-1;
    int i=0;
    char payload[150]={"{\"cmd\":0,\"serial\":1,\"source\":\"wiwo\",\"softwareVersion\":\"v2.0.1\",\"hardwareVersion\":\"iPhone6\",\"language\":\"chinese\"}"};
    char send_data[SEND_DATA_LEN];
    int return_len=0;
    char key[20]={"khggd54865SNJHGF"};
    char sessionId[33];
    char recv_data[RECV_DATA_LEN+1];
    cJSON *json=NULL;
    cJSON *json_cmd=NULL;
    cJSON *json_status=NULL;
    cJSON *json_key=NULL;
    int recv_len=-1;

    socketfd = client_tcp_socekt_(local_parameter->local_ip,local_parameter->local_port,server_parameter->sever_ip,server_parameter->server_port);
    if(-1==socketfd) {

        printf("tcp_link_gateway error\n");
        return -1;
    }

    show_log_int_("client_tcp_socekt_",socketfd);

    for(i=0;i<32;i++) {   sessionId[i]=0x30; }
    memset(send_data,0,SEND_DATA_LEN*sizeof(char));
    return_len = pack_ovribo_data(P_PROTOCOL_TYPE,key,sessionId,payload,strlen(payload),send_data);

    if(-1==(client_tcp_send_(socketfd,send_data,return_len))) {

        close(socketfd);
        printf("client_tcp_send_ error\n");
        return -1;
    }

    memset(recv_data,0,(RECV_DATA_LEN+1)*sizeof(char));
    recv_len = client_tcp_recv_(socketfd,1,0,recv_data,RECV_DATA_LEN);
    if(-1==recv_len) {

        close(socketfd);
        printf("client_tcp_recv_ error\n");
        return -1;
    }

    json=NULL;
    if(-1==(analysis_ovribo_data(recv_data,recv_len,&json,key))) {

        close(socketfd);
        printf("analysis_ovribo_data error\n");
        return -1;
    }

    if(NULL==json) {

        close(socketfd);
        printf("NULL==json error\n");
        return -1;
    }

    json_cmd = cJSON_GetObjectItem(json, "cmd");
    json_status = cJSON_GetObjectItem(json, "status");
    json_key = cJSON_GetObjectItem(json, "key");

    if(json_cmd==NULL||json_cmd->type!=cJSON_Number||json_cmd->valueint!=0) {

        printf("json_cmd error\n");
        close(socketfd);
        cJSON_Delete(json);
        return -1;
    }

    if(json_status==NULL||json_status->type!=cJSON_Number||json_status->valueint!=0) {

        printf("json_status error\n");
        cJSON_Delete(json);
        close(socketfd);
        return -1;
    }

    if(json_key==NULL||json_key->type!=cJSON_String) {

        printf("json_key error\n");
        cJSON_Delete(json);
        close(socketfd);
        return -1;
    }

    memset(server_parameter->sever_dynamic_key,0,20*sizeof(char));
    strcpy(server_parameter->sever_dynamic_key,json_key->valuestring);
    memset(server_parameter->sever_sessionId,0,33*sizeof(char));
    for(i=0;i<32;i++) {server_parameter->sever_sessionId[i]=recv_data[10+i]; }

    cJSON_Delete(json);
    return socketfd;
}

int  enter_gateway(int socketfd,ServerCommunicationParameter* server_parameter,LocalCommunicationParameter* local_parameter)
{
    int enter_result=-2;
    char send_data[SEND_DATA_LEN];
    int return_len=0;
    char recv_data[RECV_DATA_LEN+1];
    int recv_len=-1;
    cJSON *json=NULL;
    cJSON *json_cmd=NULL;
    cJSON *json_status=NULL;
    char *json_text=NULL;

    cJSON *root=NULL;
    root=cJSON_CreateObject();
    cJSON_AddNumberToObject(root,"cmd",ENTER_GATEWAY_CMD);
    cJSON_AddNumberToObject(root,"serial",2);
    cJSON_AddStringToObject(root,"userName",local_parameter->account_number);
    cJSON_AddStringToObject(root,"uid",server_parameter->server_uid);
    cJSON_AddStringToObject(root,"password",local_parameter->account_number_passwd);
    cJSON_AddNumberToObject(root,"type",0);

    json_text = cJSON_PrintUnformatted(root);
    return_len = pack_ovribo_data(D_PROTOCOL_TYPE,server_parameter->sever_dynamic_key,server_parameter->sever_sessionId,json_text,strlen(json_text),send_data);

    if(-1==(client_tcp_send_(socketfd,send_data,return_len))) {

        printf("client_tcp_send_ error\n");
        free(json_text);
        cJSON_Delete(root);
        return -1;
    }

    free(json_text);
    cJSON_Delete(root);

    memset(recv_data,0,(RECV_DATA_LEN+1)*sizeof(char));
    recv_len = client_tcp_recv_(socketfd,1,0,recv_data,RECV_DATA_LEN);
    if(-1==recv_len) {

        printf("client_tcp_recv_ error\n");
        return -1;
    }

    json=NULL;
    if(-1==(analysis_ovribo_data(recv_data,recv_len,&json,server_parameter->sever_dynamic_key))) {

        printf("analysis_ovribo_data error\n");
        return -1;
    }

    json_cmd = cJSON_GetObjectItem(json, "cmd");
    json_status = cJSON_GetObjectItem(json, "status");

    if(json_cmd==NULL||json_cmd->type!=cJSON_Number||json_cmd->valueint!=2) {

        printf("json_cmd error\n");
        cJSON_Delete(json);
        return -1;
    }

    if(json_status==NULL||json_status->type!=cJSON_Number) {

        printf("json_status error\n");
        cJSON_Delete(json);
        return -1;
    }

    enter_result = json_status->valueint;
    cJSON_Delete(json);
    return enter_result;
}

void  *ovribo_communication_thread(void *arg)
{
    int i=0;
    int irtn=-1;
    struct timeval timeout;
    int time=0;
    char msg[RECV_DATA_LEN+1];
    OrviboValue value;
    value.scene_id=0;

    for(;;) {

        fcntl(temp_socketfd, F_SETFL, 0);
        timeout.tv_sec = 1;
        timeout.tv_usec = 0; //设置超时时间
        setsockopt(temp_socketfd,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));
        time=1;

        //socket可用
        while(0 <= local_communication_parameter.socket_flags) {

            memset(msg,0,(RECV_DATA_LEN+1)*sizeof(char));
            irtn = recv(temp_socketfd,msg,RECV_DATA_LEN,0);

            if(irtn<=0) {

                if(++time > 150) {

                    if(time > 300) {

                        set_socket_flags(-3);
                    }
                }
                else  if(time == 150) {

                    ovribo_breathe(temp_socketfd,&server_communication_parameter,&local_communication_parameter);
                }
            }
            else  {

                time = 0;
                for(i = 0;i<irtn;i++) {

                    msg_buffer[(msg_buffer_head+msg_buffer_len)%BEST_RECV_DATA_LEN]=msg[i];
                    msg_buffer_len++;
                }

                deal_ovribo_data();
            }
        }

        if(temp_socketfd>=0) {

            value.value=-1;
            close(temp_socketfd);
        }

        if(NULL!=ovribo_data_pf) {

            ovribo_data_pf(LINK_OVRIBO_GATEWAY,(void*)&value,1);
        }

        //没有设置账户与密码
        if(-1==local_communication_parameter.socket_flags) {

            value.value=-2;
            printf("socket_flags error\n");
            sleep(2);
            continue;
        }
        //重置设置账户与密码
        else  if(-2==local_communication_parameter.socket_flags)  {

            memset(local_communication_parameter.account_number,0,33*sizeof(char));
            memset(local_communication_parameter.account_number_passwd,0,100*sizeof(char));
            memcpy(local_communication_parameter.account_number,temp_account_number,33*sizeof(char));
            memcpy(local_communication_parameter.account_number_passwd,temp_account_number_passwd,100*sizeof(char));
            set_socket_flags(-10);
        }
        else  if(-3==local_communication_parameter.socket_flags)  //获取数据出错或超时
        {

        }

        if(-1==(broadcast_gateway(&server_communication_parameter,&local_communication_parameter))) {

            value.value=-1;
            printf("broadcast_gateway error\n");
            sleep(2);
            continue;
        }

        irtn = tcp_link_gateway(&server_communication_parameter,&local_communication_parameter);
        if(-1 == irtn) {

            value.value=-1;
            printf("tcp_link_gateway error\n");
            sleep(2);
            continue;
        }

        value.value = enter_gateway(irtn,&server_communication_parameter,&local_communication_parameter);
        if(0 != value.value) {

            printf("enter_gateway error\n");
            close(irtn);
            sleep(2);
            continue;
        }

        temp_socketfd=irtn;
        set_socket_flags(irtn);
        memset(msg_buffer,0,BEST_RECV_DATA_LEN*sizeof(char));
        msg_buffer_head=0;
        msg_buffer_len=0;

        if(NULL!=ovribo_data_pf) {

            ovribo_data_pf(LINK_OVRIBO_GATEWAY,(void*)&value,1);
        }

        //get_ovribo_scene_data();
    }

    return NULL;
}

void  set_socket_flags(int sock)
{
    pthread_mutex_lock(&ovribo_socket_flags_lock);

    if(-2!=local_communication_parameter.socket_flags) {

         local_communication_parameter.socket_flags=sock;
    }
    else  if(-10==sock) {

        local_communication_parameter.socket_flags=sock;
    }

    pthread_mutex_unlock(&ovribo_socket_flags_lock);
}

int  ovribo_breathe(int socketfd,ServerCommunicationParameter* server_parameter,LocalCommunicationParameter* local_parameter)
{
    char send_data[SEND_DATA_LEN];
    int return_len=0;
    char *json_text=NULL;

    cJSON *root=NULL;
    root=cJSON_CreateObject();
    cJSON_AddNumberToObject(root,"cmd",BREATHE_CMD);
    cJSON_AddNumberToObject(root,"serial",3);
    cJSON_AddStringToObject(root,"userName",local_parameter->account_number);
    cJSON_AddStringToObject(root,"uid",server_parameter->server_uid);

    json_text = cJSON_PrintUnformatted(root);
    return_len = pack_ovribo_data(D_PROTOCOL_TYPE,server_parameter->sever_dynamic_key,server_parameter->sever_sessionId,json_text,strlen(json_text),send_data);

    if(-1==(client_tcp_send_(socketfd,send_data,return_len))) {

        printf("client_tcp_send_ error\n");
        free(json_text);
        cJSON_Delete(root);
        return -1;
    }

    free(json_text);
    cJSON_Delete(root);
    return 0;
}

void  deal_ovribo_data()
{
    int i=0;
    int temp_len=0;
    OrviboData data;

    while(msg_buffer_len>42) {

        temp_len=0;
        data.data=NULL;
        data.data_len=0;

        if(((PACKAGE_HEAD&0xFF00)>>8==msg_buffer[msg_buffer_head])
                &&((PACKAGE_HEAD&0xFF)==msg_buffer[(msg_buffer_head+1)%BEST_RECV_DATA_LEN])) {

            temp_len+=msg_buffer[(msg_buffer_head+3)%BEST_RECV_DATA_LEN];
            temp_len+=(msg_buffer[(msg_buffer_head+2)%BEST_RECV_DATA_LEN]<<8)&0xFF00;

            if(temp_len<=msg_buffer_len) {

                data.data=(char*)malloc((temp_len+1)*sizeof(char));
                (data.data,0,(temp_len+1)*sizeof(char));
                //memcpy(data.data,&msg_buffer[msg_buffer_head],temp_len*sizeof(char));
                for(i = 0;i < temp_len;i++) { data.data[i]=msg_buffer[(msg_buffer_head+i)%BEST_RECV_DATA_LEN]; }
                data.data_len=temp_len;
                msg_buffer_len-=temp_len;
                msg_buffer_head=(msg_buffer_head+temp_len)%BEST_RECV_DATA_LEN;

                read_and_wirte_ovribo_data(&data,OVRIBO_WRITE);
            }
            else {

                return;
            }
        }
        else {

            msg_buffer_head=(msg_buffer_head+1)%BEST_RECV_DATA_LEN;
            msg_buffer_len-=1;
        }
    }
}
