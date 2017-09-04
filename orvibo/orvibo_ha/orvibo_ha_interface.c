#include"orvibo_ha_interface.h"

OrviboData ovribo_data[100];
int ovribo_data_head;
int ovribo_data_len;
pthread_mutex_t ovribo_data_lock;

pthread_t ovribo_data_thread;
ReturnOvriboData ovribo_data_pf=NULL;

//初始化欧瑞博通信模块
void  ovribo_module_init(ReturnOvriboData pf,unsigned char* account_number,int account_number_len,unsigned char* passwd,int passwd_len)
{
    server_communication_parameter.server_broadcast_port = 10000;
    server_communication_parameter.server_port = -1;
    memset(server_communication_parameter.server_uid,0,20*sizeof(char));
    memset(server_communication_parameter.sever_dynamic_key,0,20*sizeof(char));
    memset(server_communication_parameter.sever_ip,0,20*sizeof(char));
    memset(server_communication_parameter.sever_sessionId,0,33*sizeof(char));

    local_communication_parameter.socket_flags = -1;
    local_communication_parameter.local_port = 10000;
    local_communication_parameter.account_number_len = -1;
    memset(local_communication_parameter.account_number,0,33*sizeof(char));
    memset(local_communication_parameter.account_number_passwd,0,100*sizeof(char));
    memset(local_communication_parameter.local_ip,0,20*sizeof(char));

    memset(temp_account_number,0,33*sizeof(char));
    memset(temp_account_number_passwd,0,100*sizeof(char));
    temp_socketfd = -1;

    ovribo_data_head=0;
    ovribo_data_len=0;
    pthread_mutex_init(&ovribo_data_lock,NULL);

    ovribo_data_pf=pf;
    set_account_number(account_number,account_number_len,passwd,passwd_len);

    pthread_create(&ovribo_data_thread,NULL,deal_ovribo_data_thread,NULL);
    pthread_mutex_init(&ovribo_socket_flags_lock,NULL);
    pthread_create(&communication_thread,NULL,ovribo_communication_thread,NULL);
}

//配置账号与密码
int  set_account_number(unsigned char* account_number,int account_number_len,unsigned char *passwd,int passwd_len)
{
    unsigned char digest[16]={0};
    if(NULL==account_number||NULL==passwd) {

        return -1;
    }

    memset(temp_account_number,0,33*sizeof(char));
    memcpy(temp_account_number,account_number,account_number_len*sizeof(char));

    memset(temp_account_number_passwd,0,100*sizeof(char));
    MD5(passwd,passwd_len,digest);
    if(-1==hex_to_str_(digest,temp_account_number_passwd,16)) {

        return -1;
    }

        set_socket_flags(-2);
        return 0;
}

//获取场景信息
int  get_ovribo_scene_data(int flags)
{
    char send_data[SEND_DATA_LEN];
    int return_len=0;
    char *json_text=NULL;

    cJSON *root=NULL;
    root=cJSON_CreateObject();
    cJSON_AddNumberToObject(root,"cmd",GET_SCENE_CMD);
    cJSON_AddNumberToObject(root,"serial",flags);
    cJSON_AddStringToObject(root,"userName",local_communication_parameter.account_number);
    cJSON_AddStringToObject(root,"uid",server_communication_parameter.server_uid);
    cJSON_AddNumberToObject(root,"lastUpdateTime",0);
    cJSON_AddStringToObject(root,"tableName","scene");
    cJSON_AddNumberToObject(root,"pageIndex",0);
    cJSON_AddStringToObject(root,"dataType","all");

    json_text = cJSON_PrintUnformatted(root);
    return_len = pack_ovribo_data(D_PROTOCOL_TYPE,server_communication_parameter.sever_dynamic_key,server_communication_parameter.sever_sessionId,json_text,strlen(json_text),send_data);

    if(-1==(client_tcp_send_(temp_socketfd,send_data,return_len))) {

        printf("client_tcp_send_ error\n");
        free(json_text);
        cJSON_Delete(root);
        return -1;
    }

    free(json_text);
    cJSON_Delete(root);
    return 0;
}

//触发场景
int  trigger_ovribo_scene(int scene_id)
{
    char send_data[SEND_DATA_LEN];
    int return_len=0;
    char *json_text=NULL;

    cJSON *root=NULL;
    root=cJSON_CreateObject();
    cJSON_AddNumberToObject(root,"cmd",SCENE_CONTROL_CMD);
    cJSON_AddNumberToObject(root,"serial",scene_id);
    cJSON_AddStringToObject(root,"userName",local_communication_parameter.account_number);
    cJSON_AddStringToObject(root,"uid",server_communication_parameter.server_uid);
    cJSON_AddStringToObject(root,"deviceId","");
    cJSON_AddStringToObject(root,"order","scene control");
    cJSON_AddNumberToObject(root,"value1",scene_id);
    cJSON_AddNumberToObject(root,"value2",0);
    cJSON_AddNumberToObject(root,"value3",0);
    cJSON_AddNumberToObject(root,"value4",0);
    cJSON_AddNumberToObject(root,"delayTime",0);
    cJSON_AddNumberToObject(root,"defaultResponse",1);

    json_text = cJSON_PrintUnformatted(root);
    return_len = pack_ovribo_data(D_PROTOCOL_TYPE,server_communication_parameter.sever_dynamic_key,server_communication_parameter.sever_sessionId,json_text,strlen(json_text),send_data);

    if(-1==(client_tcp_send_(temp_socketfd,send_data,return_len))) {

        printf("client_tcp_send_ error\n");
        free(json_text);
        cJSON_Delete(root);
        return -1;
    }

    free(json_text);
    cJSON_Delete(root);
    return 0;
}

//读写接收数据缓冲区
void  read_and_wirte_ovribo_data(OrviboData *data,int action)
{
    pthread_mutex_lock(&ovribo_data_lock);

    if(OVRIBO_WRITE==action)
    {
        show_log_int_("wirte_ovribo_data",ovribo_data_len);

        if(ovribo_data_len<100)
        {
            ovribo_data[(ovribo_data_head+ovribo_data_len)%100].data=data->data;
            ovribo_data[(ovribo_data_head+ovribo_data_len)%100].data_len=data->data_len;
            ovribo_data_len++;
            data->data_len=0;
        }
    }
    else  if(OVRIBO_READ==action)
    {
        if(ovribo_data_len>0)
        {
            data->data=ovribo_data[ovribo_data_head].data;
            data->data_len=ovribo_data[ovribo_data_head].data_len;
            ovribo_data_len--;
            ovribo_data_head=(ovribo_data_head+1)%100;
        }
    }

    pthread_mutex_unlock(&ovribo_data_lock);
}

//解析接收到的数据线程
void  *deal_ovribo_data_thread(void *arg)
{
    OrviboData data;

    while(1) {

        data.data=NULL;
        data.data_len=0;
        read_and_wirte_ovribo_data(&data,OVRIBO_READ);

        if(data.data_len!=0&&data.data!=NULL) {

            decompose_ovribo_data(&data);
            free(data.data);
        }

        usleep(20000);
    }

    return NULL;
}

//解析网关返回信息
void  decompose_ovribo_data(OrviboData* data)
{
    cJSON *json=NULL;
    cJSON *json_cmd=NULL;
    cJSON *json_status=NULL;
    cJSON *json_serial=NULL;
    OrviboValue value;

    json=NULL;
    if(-1==(analysis_ovribo_data(data->data,data->data_len,&json,server_communication_parameter.sever_dynamic_key))) {

        printf("analysis_ovribo_data error\n");
        return;
    }

    if(NULL==json) {

        printf("NULL==json error\n");
        return;
    }

    json_cmd = cJSON_GetObjectItem(json, "cmd");
    if(json_cmd==NULL||json_cmd->type!=cJSON_Number) {

        printf("json_cmd error\n");
        cJSON_Delete(json);
        return;
    }

    if(BREATHE_CMD==json_cmd->valueint) {


    }
    else  if(GET_SCENE_CMD==json_cmd->valueint) {

        json_status = cJSON_GetObjectItem(json, "status");
        if(json_status==NULL||json_status->type!=cJSON_Number||json_status->valueint!=0) {

            return;
        }

        decompose_ovribo_scene_data(json);
    }
    else  if(SCENE_CONTROL_CMD==json_cmd->valueint) {

        json_status = cJSON_GetObjectItem(json, "status");
        json_serial = cJSON_GetObjectItem(json, "serial");
        if(json_status==NULL||json_status->type!=cJSON_Number||json_serial==NULL) {

            value.value=1;
        }

        if(NULL!=ovribo_data_pf) {

            value.value = json_status->valueint;
            value.scene_id = json_serial->valueint;
            ovribo_data_pf(TRIGGER_SCENE_RESLUT,(void*)&value,1);
        }
    }

    cJSON_Delete(json);
}

//解析场景信息
void  decompose_ovribo_scene_data(cJSON* json)
{
    int size=0,i=0;
    OrviboScene *scene=NULL;
    cJSON *json_scene_list=NULL,*array_item=NULL;
    cJSON *scene_id=NULL,*scene_name=NULL,*scene_pic=NULL;
    cJSON *json_serial=NULL;
    json_scene_list = cJSON_GetObjectItem(json, "allList");
    json_serial = cJSON_GetObjectItem(json,"serial");

    if(NULL!=json_scene_list) {

        size = cJSON_GetArraySize(json_scene_list);
        scene = (OrviboScene*)malloc(size*sizeof(OrviboScene));

        for(i=0;i<size;i++) {

            array_item = cJSON_GetArrayItem(json_scene_list,i);
            scene[i].scene_id=-1;
            scene[i].scene_pic=-1;
            scene[i].scene_flags=-1;
            memset(scene[i].scene_name,0,20*sizeof(char));

            if(NULL!=array_item) {

                scene_id = cJSON_GetObjectItem(array_item,"sceneId");
                scene_name = cJSON_GetObjectItem(array_item,"sceneName");
                scene_pic = cJSON_GetObjectItem(array_item,"pic");

                if(NULL==scene_id||NULL==scene_name||NULL==scene_pic||NULL==json_serial) {

                    continue;
                }

                scene[i].scene_id=scene_id->valueint;
                scene[i].scene_pic=scene_pic->valueint;
                scene[i].scene_flags=json_serial->valueint;
                strcpy(scene[i].scene_name,scene_name->valuestring);
                scene[i].scene_name[strlen(scene_name->valuestring)]='\0';
                printf("scene message: %d %d %s\n", scene[i].scene_id,scene[i].scene_pic,scene[i].scene_name);
            }
        }
    }

    if(NULL!=ovribo_data_pf) {

        ovribo_data_pf(OVRIBO_SCENE_MESSAGE,(void*)scene,size);
    }

    free(scene);
}
