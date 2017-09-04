/********************************************************************************
**
** �ļ���:     leelen_interface.c
** ��Ȩ����:   (c) 2015 �������ֿƼ����޹�˾
** �ļ�����:   ʵ��ƽ̨��ؽӿڹ���
**
*********************************************************************************
**             �޸���ʷ��¼
**===============================================================================
**| ����       | ����   |  �޸ļ�¼
**===============================================================================
**| 2015/09/28 | zzh    |  �������ļ�
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
* ����ģ��ֲ�����
********************************************************************************
*/



/*******************************************************************
** ������:     write_security_record_0x0001
** ��������:   ���沼������¼
** ����:       [in]type��    ��������
**             [in]index��   ����
**             [in]rec��     ��������¼
** ����:       ��
** ע��:
********************************************************************/
void write_security_record_0x0001(uint8_t type, uint8_t index, security_record_t *rec)
{

}

/*******************************************************************
** ������:     write_security_record_0x0002
** ��������:   Զ�̲�����
** ����:       [in]attrib��  ����
**             [in]state��   ״̬
** ����:       ��
** ע��:
********************************************************************/
void write_security_record_0x0002(uint8_t attrib, uint8_t state)
{   
    if(g_udp_callback)
    {
        g_udp_callback(UDP_CALLBACK_REMOTE_SET_SECURITY,(unsigned long)state);
    }
}

/*******************************************************************
** ������:     write_security_record_0x0003
** ��������:   ����������Ϣ�ϴ�
** ����:       [in]area��     ����������¼
**             [in]index��    ����
**             [in]pic_name�� ͼƬ��Ϣ
** ����:       ��
** ע��:
********************************************************************/
void write_security_record_0x0003(area_record_t *area, uint8_t index, name_t *pic_name)
{

}

/*******************************************************************
** ������:     write_security_record_0x0004
** ��������:   ����������Ϣ
** ����:       [in]rem_user:   ���յ��ĶԷ����룻
**             [in]rem_ip��    ���յ��ĶԷ�IP
** ����:       ��
** ע��:
********************************************************************/
void write_security_record_0x0004(uint8_t *rem_user, uint8_t *rem_ip)
{
}

/*******************************************************************
** ������:     write_card_record_0x0101
** ��������:   ���浥�ſ���¼
** ����:       [in]rec��    ����Ϣ
** ����:       ��
** ע��:
********************************************************************/
void write_card_record_0x0101(card_info_t *rec)
{

}

/*******************************************************************
** ������:     delete_card_record_0x0102
** ��������:   ɾ����
** ����:       [in]rec��    ����Ϣ
** ����:       ��
** ע��:
********************************************************************/
void delete_card_record_0x0102(card_info_t *rec)
{

}

/*******************************************************************
** ������:     delete_card_record_0x0103
** ��������:   ɾ��ĳ����
** ����:       [in]rec��    ����Ϣ
** ����:       ��
** ע��:
********************************************************************/
void delete_card_record_0x0103(card_info_t *rec)
{

}

/*******************************************************************
** ������:     delete_card_record_0x0104
** ��������:   ɾ��ȫ��������
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
void delete_card_record_0x0104(void)
{

}

/*******************************************************************
** ������:     delete_card_record_0x0105
** ��������:   ɾ���������п�
** ����:       ��
** ����:       ��
** ע��:
********************************************************************/
void delete_card_record_0x0105(void)
{

}

/*******************************************************************
** ������:     write_card_record_0x0106
** ��������:   ˢ����¼�ϴ�
** ����:       [in]rec��      ˢ����¼
**             [in]index��    ����
**             [in]pic_name�� ͼƬ��Ϣ
** ����:       ��
** ע��:
********************************************************************/
void write_card_record_0x0106(card_record_t *rec, uint8_t index, name_t *pic_name)
{

}

/*******************************************************************
** ������:     write_card_record_0x0101
** ��������:   ���浥�ſ���¼
** ����:       [in]rec��    ����Ϣ
**             [in]index��    ����
**             [in]pic_name�� ͼƬ��Ϣ
** ����:       ��
** ע��:
********************************************************************/
void write_card_record_0x0107(card_record_t *rec,uint8_t index, name_t *pic_name)
{

}

/*******************************************************************
** ������:     write_card_record_0x0108
** ��������:   ��״̬��Ϣ�ϴ�
** ����:       [in]rec��     ��״̬��Ϣ
** ����:       ��
** ע��:
********************************************************************/
void write_card_record_0x0108(card_record_t *rec)
{

}

/*******************************************************************
** ������:     write_card_record_0x0109
** ��������:   Զ�̿���
** ����:       [in]rec��     ����Ϣ��¼
** ����:       �����ɹ���ʧ�ܽ��
** ע��:
********************************************************************/
int write_card_record_0x0109(card_record_t *rec)
{
    return RESULT_SUCCESS;
}

/*******************************************************************
** ������:     write_card_record_0x010a
** ��������:   Ѳ��ˢ����Ϣ�ϴ�
** ����:       [in]rec��      ˢ����¼
**             [in]index��    ����
**             [in]pic_name�� ͼƬ��Ϣ
** ����:       ��
** ע��:
********************************************************************/
void write_card_record_0x010a(card_patrol_t *rec, uint8_t index, name_t *pic_name)
{

}

/*******************************************************************
** ������:     call_lift_0x0201
** ��������:   ����
** ����:       [in]dir:            ����
**             [in]dest_floor��    ����¥����Ϣ
** ����:       ��
** ע��:
********************************************************************/
void call_lift_0x0201(uint8_t dir, uint8_t* dest_floor)
{

}

/*******************************************************************
** ������:     call_lift_0x0202
** ��������:   ���������ݿغ���
** ����:       [in]src_floor:      ��ʼ¥����Ϣ
**             [in]dest_floor��    ����¥����Ϣ
** ����:       ��
** ע��:
********************************************************************/
void call_lift_0x0202(uint8_t* src_floor, uint8_t* dest_floor)
{

}

/*******************************************************************
** ������:     call_lift_0x0203
** ��������:   ˢ���ݿ�
** ����:       [in]src_floor:      ��ʼ¥����Ϣ
**             [in]dest_floor��    ����¥����Ϣ
** ����:       ��
** ע��:
********************************************************************/
void call_lift_0x0203(uint8_t* src_floor, uint8_t* dest_floor)
{

}

/*******************************************************************
** ������:     get_lift_info_0x0204
** ��������:   ��ȡ��ǰ������Ϣ״̬
** ����:       [out]lift:       ������Ϣ
**             [in/out]num��    ��������
** ����:       ��
** ע��:
********************************************************************/
void get_lift_info_0x0204(lift_info_t *lift, uint8_t *num)
{

}

/*******************************************************************
** ������:     set_lift_info_0x0204
** ��������:   ���õ�ǰ������Ϣ״̬
** ����:       [in]lift:       ������Ϣ
**             [in]num��    ��������
** ����:       ��
** ע��:
********************************************************************/
void set_lift_info_0x0204(lift_info_t *lift, uint8_t num)
{

}

/*******************************************************************
** ������:     write_talk_record_0x0301
** ��������:   ������м�¼
** ����:       [in]rec��    ͨ����¼
**             [in]num��    ͼƬ����
**             [in]pic��     ͼƬ
** ����:       ��
** ע��:
********************************************************************/
void write_talk_record_0x0301(talk_record_t *rec, uint8_t num, name_t* pic)
{

}

/*******************************************************************
** ������:     file_delete_0x0401
** ��������:   ɾ���ļ�
** ����:       [in]fullpath��ȫ·��
** ����:       �ɹ�true,ʧ��false
********************************************************************/
bool_t file_delete_0x0401(uint8_t *fullpath)
{
   return true;
}

/*******************************************************************
** ������:     file_scan_0x0402
** ��������:   �ļ�ɨ���ȡҪ��·���������ļ�
** ����:       [in]fullpath��ȫ·��
**             [in]offset��  ��ʼƫ��λ��
**             [in]max_files:����ļ���
**             [out]file:     ��ȡ�������ļ���Ϣ
** ����:       ����ʵ���ļ�����
********************************************************************/
uint8_t file_scan_0x0402(uint8_t *fullpath, uint32_t offset, uint8_t max_files, file_info_t *file)
{
   return 0;
}

/*******************************************************************
** ������:     file_mkdir_0x0403
** ��������:   ����Ŀ¼
** ����:       [in]fullpath��ȫ·��
** ����:       �ɹ�true,ʧ��false
********************************************************************/
bool_t file_mkdir_0x0403(uint8_t *fullpath)
{
   return true;
}

/*******************************************************************
** ������:     file_rename_0x0404
** ��������:   ������
** ����:       [in]oldfullpath�����ļ�ȫ·��
**             [in]newfullpath�����ļ�ȫ·��
** ����:       �ɹ�true,ʧ��false
********************************************************************/
bool_t file_rename_0x0404(uint8_t *oldfullpath, uint8_t *newfullpath)
{
   return true;
}

/*******************************************************************
** ������:     file_checksize_0x0405
** ��������:   ���ϵͳ�洢�ռ���ڴ��С�Ƿ�����Ҫ��
** ����:       [in]fullpath�� �ļ�ȫ·��
**             [in]length��   �ļ���С
** ����:       �ɹ�true,ʧ��false
********************************************************************/
bool_t file_checksize_0x0405(uint8_t *fullpath, uint32_t length)
{
   return true;
}

/*******************************************************************
** ������:     file_checkfile_0x0406
** ��������:   ����ļ��Ƿ����
** ����:       [in]fullpath���ļ�ȫ·��
** ����:       �ɹ�true,ʧ��false
********************************************************************/
bool_t file_checkfile_0x0406(uint8_t *fullpath)
{
   return true;
}

/*******************************************************************
** ������:     get_version_info_0x0501
** ��������:   ��ȡЭ��汾����Ϣ
** ����:       [out]data:         ���յ������ݣ�
**             [in/out]data_len��    ���յ����ݵĳ���
** ����:       ��
** ע��:
********************************************************************/
void get_version_info_0x0501(uint8_t *data, uint8_t *data_len)
{

}


/*******************************************************************
** ������:     set_version_info_0x0501
** ��������:   ����Э��汾����Ϣ
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
** ����:       ��
** ע��:
********************************************************************/
void set_version_info_0x0501(uint8_t *data, uint8_t data_len)
{

}

/*******************************************************************
** ������:     get_datetime_0x0502
** ��������:   ��ȡϵͳʱ��
** ����:       [out]data:         ���յ������ݣ�
**             [in/out]data_len��    ���յ����ݵĳ���
** ����:       ��
** ע��:
********************************************************************/
void get_datetime_0x0502(uint8_t *data, uint8_t *data_len)
{

}


/*******************************************************************
** ������:     set_datetime_0x0502
** ��������:   ����ϵͳʱ��
** ����:       [in]data:         ���յ������ݣ�
**             [in]data_len��    ���յ����ݵĳ���
** ����:       ��
** ע��:
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
** ������:     write_device_record_0x0503
** ��������:   �豸�쳣������¼
** ����:       [in]rec��     �쳣��Ϣ
** ����:       ��
** ע��:
********************************************************************/
void write_device_record_0x0503(device_record_t *rec)
{

}

/*******************************************************************
** ������:     write_info_0x0504
** ��������:   �յ���Ϣ
** ����:       [in]info:         ���⼰��Ϣ����
**             [in]attach_num��  ��������
**             [in]attach��      ����
**             [in]from_name    ��Ϣ���ͷ�����
** ����:       ��
** ע��:
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
** ������:     read_big_data_0x0601
** ��������:   ��ȡ������
** ����:       [in]fullpath:       �ļ�ȫ·��
**             [in]offset��        ƫ��λ��
**             [out]data��          ����
**             [in]data_len��      ���ݳ���
** ����:       ���ݳɹ���ȡ����
** ע��:
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
** ������:     write_big_data_0x0601
** ��������:   ��ȡ������
** ����:       [in]type:           ��������
**             [in]fullpath:       �ļ�ȫ·��
**             [in]offset��        ƫ��λ��Ϊ0,Ҫ�ж��ļ��Ƿ����,��������ɾ��֮
**             [in]data��          ����
**             [in]data_len��      ���ݳ���
** ����:       ��
** ע��:
********************************************************************/
uint32_t write_big_data_0x0601(uint8_t type, uint8_t *fullpath, uint32_t offset, uint8_t *data, uint32_t data_len)
{
#if 0
    uint32_t len = 0;
    FILE *fp = NULL;   
    if (offset == 0){ 
        fp = fopen(fullpath, "w+");  /* ��ɾ�� */
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
        fp = fopen(file_path, "w+");  /* ��ɾ�� */
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



