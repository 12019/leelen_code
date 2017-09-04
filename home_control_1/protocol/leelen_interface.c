/********************************************************************************
**
** 文件名:     leelen_interface.c
** 版权所有:   (c) 2015 厦门立林科技有限公司
** 文件描述:   实现平台相关接口功能
**
*********************************************************************************
**             修改历史记录
**===============================================================================
**| 日期       | 作者   |  修改记录
**===============================================================================
**| 2015/09/28 | zzh    |  创建该文件
**
*********************************************************************************/

#include "leelen_comdef.h"
#include "leelen_protocol.h"
#include "leelen_system.h"
#include "leelen_talk.h"
#include "leelen_security.h"
#include "leelen_card.h"
#include "leelen_lift.h"
#include "leelen_file.h"

#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>


extern udp_callback g_udp_callback;
extern char remote_ip[30];
#define SEC_FILE_PATH       "/mnt/disk/info/"
static char info_datatime [128];
/*
********************************************************************************
* 定义模块局部变量
********************************************************************************
*/



/*******************************************************************
** 函数名:     write_security_record_0x0001
** 函数描述:   保存布撤防记录
** 参数:       [in]type：    操作类型
**             [in]index：   索引
**             [in]rec：     布撤防记录
** 返回:       无
** 注意:
********************************************************************/
void write_security_record_0x0001(uint8_t type, uint8_t index, security_record_t *rec)
{

}

/*******************************************************************
** 函数名:     write_security_record_0x0002
** 函数描述:   远程布撤防
** 参数:       [in]attrib：  属性
**             [in]state：   状态
** 返回:       无
** 注意:
********************************************************************/
void write_security_record_0x0002(uint8_t attrib, uint8_t state)
{   
    if(g_udp_callback)
    {
        g_udp_callback(UDP_CALLBACK_REMOTE_SET_SECURITY,(unsigned long)state);
    }
}

/*******************************************************************
** 函数名:     write_security_record_0x0003
** 函数描述:   防区报警信息上传
** 参数:       [in]area：     防区报警记录
**             [in]index：    索引
**             [in]pic_name： 图片信息
** 返回:       无
** 注意:
********************************************************************/
void write_security_record_0x0003(area_record_t *area, uint8_t index, name_t *pic_name)
{

}

/*******************************************************************
** 函数名:     write_security_record_0x0004
** 函数描述:   保存求助信息
** 参数:       [in]rem_user:   接收到的对方号码；
**             [in]rem_ip：    接收到的对方IP
** 返回:       无
** 注意:
********************************************************************/
void write_security_record_0x0004(uint8_t *rem_user, uint8_t *rem_ip)
{
}

/*******************************************************************
** 函数名:     write_card_record_0x0101
** 函数描述:   保存单张卡记录
** 参数:       [in]rec：    卡信息
** 返回:       无
** 注意:
********************************************************************/
void write_card_record_0x0101(card_info_t *rec)
{

}

/*******************************************************************
** 函数名:     delete_card_record_0x0102
** 函数描述:   删除卡
** 参数:       [in]rec：    卡信息
** 返回:       无
** 注意:
********************************************************************/
void delete_card_record_0x0102(card_info_t *rec)
{

}

/*******************************************************************
** 函数名:     delete_card_record_0x0103
** 函数描述:   删除某户卡
** 参数:       [in]rec：    卡信息
** 返回:       无
** 注意:
********************************************************************/
void delete_card_record_0x0103(card_info_t *rec)
{

}

/*******************************************************************
** 函数名:     delete_card_record_0x0104
** 函数描述:   删除全部保安卡
** 参数:       无
** 返回:       无
** 注意:
********************************************************************/
void delete_card_record_0x0104(void)
{

}

/*******************************************************************
** 函数名:     delete_card_record_0x0105
** 函数描述:   删除主机所有卡
** 参数:       无
** 返回:       无
** 注意:
********************************************************************/
void delete_card_record_0x0105(void)
{

}

/*******************************************************************
** 函数名:     write_card_record_0x0106
** 函数描述:   刷卡记录上传
** 参数:       [in]rec：      刷卡记录
**             [in]index：    索引
**             [in]pic_name： 图片信息
** 返回:       无
** 注意:
********************************************************************/
void write_card_record_0x0106(card_record_t *rec, uint8_t index, name_t *pic_name)
{

}

/*******************************************************************
** 函数名:     write_card_record_0x0101
** 函数描述:   保存单张卡记录
** 参数:       [in]rec：    卡信息
**             [in]index：    索引
**             [in]pic_name： 图片信息
** 返回:       无
** 注意:
********************************************************************/
void write_card_record_0x0107(card_record_t *rec,uint8_t index, name_t *pic_name)
{

}

/*******************************************************************
** 函数名:     write_card_record_0x0108
** 函数描述:   门状态信息上传
** 参数:       [in]rec：     门状态信息
** 返回:       无
** 注意:
********************************************************************/
void write_card_record_0x0108(card_record_t *rec)
{

}

/*******************************************************************
** 函数名:     write_card_record_0x0109
** 函数描述:   远程开锁
** 参数:       [in]rec：     卡信息记录
** 返回:       开锁成功或失败结果
** 注意:
********************************************************************/
int write_card_record_0x0109(card_record_t *rec)
{
    return RESULT_SUCCESS;
}

/*******************************************************************
** 函数名:     write_card_record_0x010a
** 函数描述:   巡更刷卡信息上传
** 参数:       [in]rec：      刷卡记录
**             [in]index：    索引
**             [in]pic_name： 图片信息
** 返回:       无
** 注意:
********************************************************************/
void write_card_record_0x010a(card_patrol_t *rec, uint8_t index, name_t *pic_name)
{

}

/*******************************************************************
** 函数名:     call_lift_0x0201
** 函数描述:   呼梯
** 参数:       [in]dir:            方向
**             [in]dest_floor：    到达楼层信息
** 返回:       无
** 注意:
********************************************************************/
void call_lift_0x0201(uint8_t dir, uint8_t* dest_floor)
{

}

/*******************************************************************
** 函数名:     call_lift_0x0202
** 函数描述:   户户互访梯控呼梯
** 参数:       [in]src_floor:      起始楼层信息
**             [in]dest_floor：    到达楼层信息
** 返回:       无
** 注意:
********************************************************************/
void call_lift_0x0202(uint8_t* src_floor, uint8_t* dest_floor)
{

}

/*******************************************************************
** 函数名:     call_lift_0x0203
** 函数描述:   刷卡梯控
** 参数:       [in]src_floor:      起始楼层信息
**             [in]dest_floor：    到达楼层信息
** 返回:       无
** 注意:
********************************************************************/
void call_lift_0x0203(uint8_t* src_floor, uint8_t* dest_floor)
{

}

/*******************************************************************
** 函数名:     get_lift_info_0x0204
** 函数描述:   获取当前电梯信息状态
** 参数:       [out]lift:       电梯信息
**             [in/out]num：    电梯数量
** 返回:       无
** 注意:
********************************************************************/
void get_lift_info_0x0204(lift_info_t *lift, uint8_t *num)
{

}

/*******************************************************************
** 函数名:     set_lift_info_0x0204
** 函数描述:   设置当前电梯信息状态
** 参数:       [in]lift:       电梯信息
**             [in]num：    电梯数量
** 返回:       无
** 注意:
********************************************************************/
void set_lift_info_0x0204(lift_info_t *lift, uint8_t num)
{

}

/*******************************************************************
** 函数名:     write_talk_record_0x0301
** 函数描述:   保存呼叫记录
** 参数:       [in]rec：    通话记录
**             [in]num：    图片数量
**             [in]pic：     图片
** 返回:       无
** 注意:
********************************************************************/
void write_talk_record_0x0301(talk_record_t *rec, uint8_t num, name_t* pic)
{

}

/*******************************************************************
** 函数名:     file_delete_0x0401
** 函数描述:   删除文件
** 参数:       [in]fullpath：全路径
** 返回:       成功true,失败false
********************************************************************/
bool_t file_delete_0x0401(uint8_t *fullpath)
{
   return true;
}

/*******************************************************************
** 函数名:     file_scan_0x0402
** 函数描述:   文件扫描获取要求路径下所有文件
** 参数:       [in]fullpath：全路径
**             [in]offset：  起始偏移位置
**             [in]max_files:最大文件数
**             [out]file:     获取到所有文件信息
** 返回:       返回实际文件个数
********************************************************************/
uint8_t file_scan_0x0402(uint8_t *fullpath, uint32_t offset, uint8_t max_files, file_info_t *file)
{
   return 0;
}

/*******************************************************************
** 函数名:     file_mkdir_0x0403
** 函数描述:   创建目录
** 参数:       [in]fullpath：全路径
** 返回:       成功true,失败false
********************************************************************/
bool_t file_mkdir_0x0403(uint8_t *fullpath)
{
   return true;
}

/*******************************************************************
** 函数名:     file_rename_0x0404
** 函数描述:   重命名
** 参数:       [in]oldfullpath：旧文件全路径
**             [in]newfullpath：新文件全路径
** 返回:       成功true,失败false
********************************************************************/
bool_t file_rename_0x0404(uint8_t *oldfullpath, uint8_t *newfullpath)
{
   return true;
}

/*******************************************************************
** 函数名:     file_checksize_0x0405
** 函数描述:   检查系统存储空间和内存大小是否满足要求
** 参数:       [in]fullpath： 文件全路径
**             [in]length：   文件大小
** 返回:       成功true,失败false
********************************************************************/
bool_t file_checksize_0x0405(uint8_t *fullpath, uint32_t length)
{
   return true;
}

/*******************************************************************
** 函数名:     file_checkfile_0x0406
** 函数描述:   检查文件是否存在
** 参数:       [in]fullpath：文件全路径
** 返回:       成功true,失败false
********************************************************************/
bool_t file_checkfile_0x0406(uint8_t *fullpath)
{
   return true;
}

/*******************************************************************
** 函数名:     get_version_info_0x0501
** 函数描述:   获取协议版本等信息
** 参数:       [out]data:         接收到的数据；
**             [in/out]data_len：    接收到数据的长度
** 返回:       无
** 注意:
********************************************************************/
void get_version_info_0x0501(uint8_t *data, uint8_t *data_len)
{

}


/*******************************************************************
** 函数名:     set_version_info_0x0501
** 函数描述:   设置协议版本等信息
** 参数:       [in]data:         接收到的数据；
**             [in]data_len：    接收到数据的长度
** 返回:       无
** 注意:
********************************************************************/
void set_version_info_0x0501(uint8_t *data, uint8_t data_len)
{

}

/*******************************************************************
** 函数名:     get_datetime_0x0502
** 函数描述:   获取系统时间
** 参数:       [out]data:         接收到的数据；
**             [in/out]data_len：    接收到数据的长度
** 返回:       无
** 注意:
********************************************************************/
void get_datetime_0x0502(uint8_t *data, uint8_t *data_len)
{

}


/*******************************************************************
** 函数名:     set_datetime_0x0502
** 函数描述:   设置系统时间
** 参数:       [in]data:         接收到的数据；
**             [in]data_len：    接收到数据的长度
** 返回:       无
** 注意:
********************************************************************/
void set_datetime_0x0502(uint8_t *data, uint8_t data_len)
{
    union sigval  mysigval;
    mysigval.sival_ptr = (void*)data;
    LEE_LOG("sending SIGUSR2 signal to %d......\n", getpid());
    if (sigqueue(getpid(), SIGUSR2, mysigval) == -1)
    {
        LEE_LOG("sigqueue SIGUSR2 error\n");
    }
}

/*******************************************************************
** 函数名:     write_device_record_0x0503
** 函数描述:   设备异常报警记录
** 参数:       [in]rec：     异常信息
** 返回:       无
** 注意:
********************************************************************/
void write_device_record_0x0503(device_record_t *rec)
{

}

/*******************************************************************
** 函数名:     write_info_0x0504
** 函数描述:   收到信息
** 参数:       [in]info:         标题及信息内容
**             [in]attach_num：  附件数量
**             [in]attach：      附件
**             [in]from_name    信息发送方号码
** 返回:       无
** 注意:
********************************************************************/
void write_info_0x0504(info_t  *info, uint8_t attach_num, attachment_t *attach, uint8_t *from_name)
{
    time_t now_time;
    struct tm *p_time;
    char *str_info_filename;
    char str_pathname[128] = {'\0'};
    FILE *fp;
    int rand_num = rand()%1000;
    int i,j;
    char remote_name[12];
    time(&now_time);
    p_time = localtime(&now_time);

    str_info_filename = malloc(sizeof(char)*128);
    if(!str_info_filename)
    {
        return ;
    }

    memset(str_info_filename,'\0',sizeof(char)*128);
    memset(info_datatime,'\0',sizeof(info_datatime));
    sprintf(info_datatime, "%04d%02d%02d-%02d%02d%02d-%03d", p_time->tm_year + 1900, \
            p_time->tm_mon + 1, p_time->tm_mday, p_time->tm_hour, p_time->tm_min, p_time->tm_sec, rand_num);
    sprintf(str_info_filename,"%s.txt",info_datatime);
    sprintf(str_pathname, "./info/%s", str_info_filename);
    if ((fp = fopen(str_pathname, "wt")) == NULL)
    {
        return;
    }
    if(from_name[0] == 0xff)
    {
        sprintf(remote_name,"%02u%02u-%02u%02u",from_name[4],from_name[3],from_name[2],from_name[1]);
        printf("%u%u-%u%u",from_name[4],from_name[3],from_name[2],from_name[1]);
        remote_name[9] = '\0';
    }
    else
    {
        sprintf(remote_name,"%02u%02u-%02u%02u-%u",from_name[4],from_name[3],from_name[2],from_name[1],from_name[0]);
        remote_name[11] = '\0';
    }

    fprintf(fp,"FROM:%s\n",remote_name);
    fprintf(fp,"FROM_IP:%s\n",remote_ip);
    fprintf(fp,"TITLE:%s\n",info->title);
    fprintf(fp,"UNREAD:1\n");
    fprintf(fp,"ATTACHMENT:\n");
    if(attach_num != 0)
    {
        for(i=0;i<attach_num;i++)
        {
            fprintf(fp,"\t%s\n",attach->file);
        }
    }
    fprintf(fp,"BODY:%s\n",info->content);
    fclose(fp);

    if(g_udp_callback)
    {
        g_udp_callback(UDP_CALLBACK_INFO,(unsigned long)str_info_filename);
    }

    if (str_info_filename)
    {
        free(str_info_filename);
        str_info_filename = NULL;
    }
}


/*******************************************************************
** 函数名:     read_big_data_0x0601
** 函数描述:   读取大数据
** 参数:       [in]fullpath:       文件全路径
**             [in]offset：        偏移位置
**             [out]data：          数据
**             [in]data_len：      数据长度
** 返回:       数据成功读取长度
** 注意:
********************************************************************/
uint32_t read_big_data_0x0601(uint8_t *fullpath, uint32_t offset, uint8_t *data, uint32_t data_len)
{    
#if 1
    uint32_t len = 0;
    FILE *fp = NULL;
    
    fp = fopen(fullpath, "r"); 
    if (fp != NULL){
        fseek(fp, offset, SEEK_SET);
        len = fread(data, 1, data_len, fp);
        fclose(fp);
    }
    return len;
#endif    
}

/*******************************************************************
** 函数名:     write_big_data_0x0601
** 函数描述:   读取大数据
** 参数:       [in]type:           数据属性
**             [in]fullpath:       文件全路径
**             [in]offset：        偏移位置为0,要判断文件是否存在,若存在先删除之
**             [in]data：          数据
**             [in]data_len：      数据长度
** 返回:       无
** 注意:
********************************************************************/
uint32_t write_big_data_0x0601(uint8_t type, uint8_t *fullpath, uint32_t offset, uint8_t *data, uint32_t data_len)
{
#if 0
    uint32_t len = 0;
    FILE *fp = NULL;   
    if (offset == 0){ 
        fp = fopen(fullpath, "w+");  /* 先删除 */
        fclose(fp);
    }
    
    fp = fopen(fullpath, "a+");
    if (fp != NULL){
        fseek(fp, offset, SEEK_SET);
        len = fwrite(data, 1, data_len, fp);
        fflush(fp);
        fclose(fp);
    }
    return len;
#else
    if (type != 0x06)
    {
        return;
    }

    LEE_LOG("\n write_big_data_0x0601 fullpath = %s\n", fullpath);

    FILE	  *fp = NULL;
    uint32_t  file_len = 0;
    uint32_t  len = 0;
    char	  file_path[128] = {0};

    sprintf(file_path, "%s%s-%s", SEC_FILE_PATH,info_datatime,fullpath);
    if (strlen(file_path) < strlen(SEC_FILE_PATH) + 4)
    {
        return;
    }

    LEE_LOG("\n\n  file_path = %s.\n", file_path);
    if (offset == 0)
    {
        fp = fopen(file_path, "w+");  /* 先删除 */
        fclose(fp);
    }

    fp = fopen(file_path, "a+");
    if (fp != NULL)
    {
        fseek(fp, offset, SEEK_SET);
        len = fwrite(data, 1, data_len, fp);
        fflush(fp);

        fseek(fp, 0L, SEEK_END);
        file_len = ftell(fp);

        fclose(fp);
    }

#endif
        return len;
}



