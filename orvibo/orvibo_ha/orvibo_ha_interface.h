/**
 * Copyrights (C) 2016
 * All right reserved
 *
 * File : orvibo_ha_interface_
 *
 * 欧瑞博模块对外接口
 *
 * Change Logs:
 * Date         Author      Notes
 * 2016-12-08   wangmaopeng     Create
 */

#ifndef __ORVIBO_HA_INTERFACE_H__
#define __ORVIBO_HA_INTERFACE_H__

/**************************************************************************
*                                 INCLUDES
**************************************************************************/
#include"orvibo_ha_communication.h"
#include<openssl/md5.h>

#ifdef  __cplusplus
extern "C" {
#endif

/**************************************************************************
*                             TYPE DEFINES
**************************************************************************/
typedef struct OrviboData{

    char *data;
    int data_len;
}OrviboData;   //欧瑞博返回数据

typedef struct OrviboScene{

    char scene_name[20];
    int scene_pic;
    int scene_id;
    int scene_flags;
}OrviboScene;   //欧瑞博场景数据

typedef struct OrviboValue{

    //如果类型是连接状态，0：处理成功,-1表示失败,-2表示没有配置账号,
    // 6：主机未绑定用户信息或者绑定信息已经被重置
    //12：用户名或者密码错误
    //8：主机不在线
    //9：该用户名未绑定到主机。
    //25：管理员登录失败，同一时间只允许一个管理员登录和配置
    //如果数据类型是操作结果，0表示成功，1表示失败
    int value;
    int scene_id;
}OrviboValue;   //欧瑞博相应值

#define OVRIBO_READ 100
#define OVRIBO_WRITE 101

typedef int (*ReturnOvriboData)(int cmd_type,void*str,int num);

typedef enum OvriboDataType{

    LINK_OVRIBO_GATEWAY = 0 , //连接网关状态
    OVRIBO_SCENE_MESSAGE,  //网关场景信息
    TRIGGER_SCENE_RESLUT  //触发场景结果
}EnumOvriboDataType;

/**************************************************************************
*                                 DECLARES
**************************************************************************/
extern pthread_t ovribo_data_thread;

/**************************************************************************
*                               FUNCTIONS
**************************************************************************/
/*******************************************************************
** 函数名:     ovribo_module_init
** 函数描述:   初始化欧瑞博通信模块
** 参数:       [ReturnOvriboData]     回调函数指针
** 参数:       [char*]           账号
** 参数:       [int]          账号长度
** 参数:       [char*]       密码
** 参数:       [int]  s         密码长度
** 返回:       void
** 注意:       对外接口
********************************************************************/
void  ovribo_module_init(ReturnOvriboData pf,unsigned char* account_number,int account_number_len,unsigned char *passwd,int passwd_len);

/*******************************************************************
** 函数名:     set_account_number
** 函数描述:   配置账号与密码
** 参数:       [ReturnOvriboData]     回调函数指针
** 参数:       [char*]           账号
** 参数:       [int]          账号长度
** 参数:       [char*]       密码
** 参数:       [int]  s         密码长度
** 返回:       0表示成功，-1表示失败
** 注意:       对外接口
********************************************************************/
int  set_account_number(unsigned char* account_number,int account_number_len,unsigned char *passwd,int passwd_len);

/*******************************************************************
** 函数名:     trigger_ovribo_scene
** 函数描述:  触发场景
** 参数:       [int]          场景id
** 返回:       0表示发送成功，-1表示发送失败
** 注意:       对外接口
********************************************************************/
int  trigger_ovribo_scene(int scene_id);

/*******************************************************************
** 函数名:     get_ovribo_scene_data
** 函数描述:   获取场景信息
** 返回:       0表示发送成功，-1表示发送失败
** 注意:       对外接口
********************************************************************/
int  get_ovribo_scene_data(int flags);

/*******************************************************************
** 函数名:     read_and_wirte_ovribo_data
** 函数描述:   读写接收数据缓冲区
** 参数:       OrviboData *data  数据
** 参数:     int action  读写
** 返回:       转换后的字符
********************************************************************/
void  read_and_wirte_ovribo_data(OrviboData *data,int action);

/*******************************************************************
** 函数名:     deal_ovribo_data_thread
** 函数描述:  解析接收到的数据线程
** 参数:
** 返回:       void
********************************************************************/
void  *deal_ovribo_data_thread(void *arg);

/*******************************************************************
** 函数名:     decompose_ovribo_data
** 函数描述:   解析网关返回信息
** 参数:       [OrviboData*]
** 返回:       void
********************************************************************/
void  decompose_ovribo_data(OrviboData* data);

/*******************************************************************
** 函数名:     decompose_ovribo_scene_data
** 函数描述:  解析场景信息
** 参数:       [cJSON*]
** 返回:       void
********************************************************************/
void  decompose_ovribo_scene_data(cJSON*);

#ifdef  __cplusplus
}
#endif

#endif  //__ORVIBO_HA_INTERFACE_H__
