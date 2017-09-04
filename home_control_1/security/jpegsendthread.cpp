#include "jpegsendthread.h"
#include <QImage>
#include <stdio.h>
#include <QFile>
#include "maindialog.h"
#include "core/leelen_comdef.h"
#include "leelen_protocol.h"
#include "leelen_big.h"
#include "core/leelen_big_packet.h"
#include "leelen_protocol.h"
#include "mediastreamer2/msvideo.h"
#include "lpcore.h"

/************************************************************
  该文件为安防报警录像向管理机发送JPEG图像线程的主程序
************************************************************/

extern unsigned char *g_jpg_image_buf;
extern int g_jpg_image_ready;
extern char g_strLphoneName[3][50];
extern char g_strNativeName[50];

extern int g_jpg_tvd_image_ready;
extern char *g_jpg_tvd_image_buf;
extern MainDialog *g_pMainDlg;
extern RESULT_E g_check_managemachine_udp_state;
extern RESULT_E g_check_computercenter_udp_state;
extern LinphoneCore *lphone;
/************************************************************
描述：若发送图片给管理机收到管理机返回的发送结果后判定是否使用udp发送图片给物业
     否则删除图片
参数：result :图片发送结果
返回：无
注意:因使用udp发送图片给管理机，使用tcp发送图片给物业，
    tcp发送给物业先于回调完成，故发送给管理机回调回来后删除图片
************************************************************/
void sendpic_to_manager_result_callback(RESULT_E result) //回调函数
{
    printf("\n=============[%s -- %d] result = %d\n", __func__, __LINE__, result);

    JpegSendThread * pJpegSendThread = g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_pJpegSendThread;
    if((g_check_computercenter_udp_state == RESULT_SUCCESS) && (!pJpegSendThread->m_FileNameList.isEmpty()))
    {
        big_file_t big_file;
        char dir_name[] = "/mnt/disk/alert_pic";
        char file_name[64] = {'\0'};
        QString get_file_name = pJpegSendThread->m_FileNameList.takeFirst();

        memcpy(file_name,get_file_name.toLocal8Bit().data(),get_file_name.length());
        big_file.dir_name = (uint8_t*)dir_name;
        big_file.dir_namelen = (uint8_t)strlen(dir_name);
        big_file.file_name = (uint8_t*)file_name;
        big_file.file_namelen = (uint8_t)strlen(file_name);

        int res = pJpegSendThread->SendPicByUdp(&big_file,TO_COMPUTER_CENTER); //若使用udp发送图片给物业过程中物业断网也需删除图片
        if(res != 0)
        {
            if(!pJpegSendThread->m_FileDirList.isEmpty())
            {
                QString file_dir_name = pJpegSendThread->m_FileDirList.takeFirst();
                QFile::remove(file_dir_name);
            }
        }

    }
    else
    {
        if(!pJpegSendThread->m_FileNameList.isEmpty()) //清除队列中本张图片的名字数据
        {
           pJpegSendThread->m_FileNameList.takeFirst();
        }

        if(!pJpegSendThread->m_FileDirList.isEmpty()) //  若使用udp发送图片给管理机 tcp发送图片给物业中心也需删除图片
        {
            QString file_dir_name = pJpegSendThread->m_FileDirList.takeFirst();
            QFile::remove(file_dir_name);
        }
    }
}
/************************************************************
描述：实现删除本次发送的图片功能
参数：result :图片发送结果
返回：无
************************************************************/
void sendpic_to_computer_result_callback(RESULT_E result) //回调函数
{
     JpegSendThread * pJpegSendThread = g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_pJpegSendThread;
     if(!pJpegSendThread->m_FileDirList.isEmpty())
     {
         QString file_dir_name = pJpegSendThread->m_FileDirList.takeFirst();
         QFile::remove(file_dir_name);
     }
}


/************************************************************
描述：YUV420图像向RGB565图像格式的转换
参数：src - YUV420图像缓存区，Y、U、V数据连续存储
     dst - RGB565图像缓存区
     width - 图像宽
     height - 图像高
返回：无
************************************************************/
static void tvd_yuv420_to_rgb565(unsigned char *src, unsigned short *dst, int width, int height)
{
    int line, col, linewidth;
    int y, u, v, yy, vr, ug, vg, ub;
    int r, g, b;
    const unsigned char *py, *pu, *pv;

    linewidth = width >> 1;
    py = src;
    pu = py + (width * height);
    pv = pu + 1; //(width * height) / 4;

    y = *py++;
    yy = y << 8;
    u = *pu - 128;
    ug =   88 * u;
    ub = 454 * u;
    v = *pv - 128;
    vg = 183 * v;
    vr = 359 * v;

    for (line = 0; line < height; line++) {
        for (col = 0; col < width; col++) {
            r = (yy +      vr) >> 8;
            g = (yy - ug - vg) >> 8;
            b = (yy + ub     ) >> 8;

            if (r < 0)   r = 0;
            if (r > 255) r = 255;
            if (g < 0)   g = 0;
            if (g > 255) g = 255;
            if (b < 0)   b = 0;
            if (b > 255) b = 255;
            *dst++ = (((unsigned short)r>>3)<<11) | (((unsigned short)g>>2)<<5) | (((unsigned short)b>>3)<<0);

            y = *py++;
            yy = y << 8;
            if (col & 1) {
                //pu++;
                //pv++;
                pu += 2;
                pv += 2;

                u = *pu - 128;
                ug =   88 * u;
                ub = 454 * u;
                v = *pv - 128;
                vg = 183 * v;
                vr = 359 * v;
            }
        } /* ..for col */
        if ((line & 1) == 0) { // even line: rewind
            //pu -= linewidth;
            //pv -= linewidth;
            pu -= linewidth * 2;
            pv -= linewidth * 2;
        }
    } /* ..for line */
}

/************************************************************
描述：JPEG发送线程构造函数
参数：parent - 该线程的父对象指针
返回：无
************************************************************/
JpegSendThread::JpegSendThread(QObject *parent) :
    QThread(parent)
{
    m_pTcpSock[0] = NULL;
    m_pTcpSock[1] = NULL;
    m_bRun = false;
    m_bBusy = false;
    m_alarm_num = -1;
}

/************************************************************
描述：JPEG发送线程析构函数
参数：无
返回：无
************************************************************/
JpegSendThread::~JpegSendThread()
{
    //if (m_pTcpSock) delete m_pTcpSock;

    m_bRun = false;
}

/************************************************************
描述：连接管理机或者物业软件
参数：num : 报警防区号
     i : 为0表示向管理机发送，为1表示向物业软件发送
返回：0 - 成功
     -1 - 失败
************************************************************/
int JpegSendThread::ConnectToManCenter(int i)
{
    m_bBusy = true;

    char strIP[200] = "";//192.168.1.139";
    GetPeerIPThread *pGetPeerIPThread = ((MainDialog *)(parent()->parent()/*->parent()*/))->m_pVoipMainDlg->m_pGetPeerIPThread;

    if (m_pTcpSock[i])
    {
        m_pTcpSock[i]->close();
        delete m_pTcpSock[i];
    }
    m_pTcpSock[i] = new QTcpSocket;

    printf("JpegSendThread 0\n");

    if (pGetPeerIPThread->GetPeerIP(g_strLphoneName[i + 1], strIP) != 0)
    {
        delete m_pTcpSock[i];
        m_pTcpSock[i] = NULL;
        m_bBusy = false;
        return -1;
    }

    char *ptr;
    int iModalType = BASIC_MODAL_TYPE;

    ptr = strstr(strIP, "?");
    if (ptr)
    {
        iModalType = atoi(ptr + strlen("?"));
        *ptr = '\0';
    }

    if (iModalType != MANCENTER_MODAL_TYPE)
    {
        delete m_pTcpSock[i];
        m_pTcpSock[i] = NULL;
        m_bBusy = false;
        return -1;
    }

    ptr = strstr(strIP, "->");
    if (ptr)
    {
        ptr[0] = '\0';
    }

    if(m_pTcpSock[i] == NULL)
    {
        m_pTcpSock[i]  = new QTcpSocket;

        if(m_pTcpSock[i] == NULL)
        {
            delete m_pTcpSock[i];
            m_pTcpSock[i] = NULL;
            m_bBusy = false;
            return -1;
        }
    }

    m_pTcpSock[i]->connectToHost(strIP, 17701);

    if (!m_pTcpSock[i]->waitForConnected(500))
    {
        delete m_pTcpSock[i];
        m_pTcpSock[i] = NULL;
        m_bBusy = false;
        return -1;
    }

    m_bBusy = false;
    return 0;
}

/************************************************************
描述：向管理机或者物业软件发送报警防区信息
参数：type - 防区类型(有线/无线防区)
     number - 防区号
     name - 防区名称
     channel - 防区摄像头通道号(0-3)
     property - 防区属性
     strLocation - 防区安装位置
     strDateTime - 报警时间
     destination - 为0表示向管理机发送，为1表示向物业发送
返回：0 - 成功
     -1 - 失败
************************************************************/
int JpegSendThread::SendAlertInfo(int type, int number, int name, int channel, int property,
                                  QString strLocation, QString strDateTime, int destination)
{
    m_bBusy = true;

    unsigned char tag = 0x80;
    short len;
    char strTemp[256];

    if (!m_pTcpSock[destination])
    {
        m_bBusy = false;
        return -1;
    }
    if (m_pTcpSock[destination]->state() != QTcpSocket::ConnectedState)
    {
        m_bBusy = false;
        return -1;
    }

    len = strlen(g_strNativeName);
    m_pTcpSock[destination]->write((char *)&tag, sizeof(unsigned char));
    m_pTcpSock[destination]->write((char *)&len, sizeof(short));
    m_pTcpSock[destination]->write((char *)g_strNativeName, len);
    m_pTcpSock[destination]->write((char *)&type, sizeof(int));
    m_pTcpSock[destination]->write((char *)&number, sizeof(int));
    m_pTcpSock[destination]->write((char *)&name, sizeof(int));
    m_pTcpSock[destination]->write((char *)&channel, sizeof(int));
    m_pTcpSock[destination]->write((char *)&property, sizeof(int));

    strcpy(strTemp, strLocation.toLocal8Bit().data());
    len = strlen(strTemp);
    m_pTcpSock[destination]->write((char *)&len, sizeof(short));
    m_pTcpSock[destination]->write(strTemp, len);

    strcpy(strTemp, strDateTime.toLocal8Bit().data());
    len = strlen(strTemp);
    m_pTcpSock[destination]->write((char *)&len, sizeof(short));
    m_pTcpSock[destination]->write(strTemp, len);

    m_pTcpSock[destination]->flush();

    printf("\tJpegSendThread::SendAlertInfo complete %d %p\n", destination,m_pTcpSock[destination]);

    m_bBusy = false;

    return 0;
}

/************************************************************
描述：使用udp协议发送图片
参数：big_file : 包含文件目录文件名结构
     num : 为0表示向管理机发送，为1表示向物业软件发送
返回：0成功  -1失败
************************************************************/
int JpegSendThread::SendPicByUdp(big_file_t *big_file , int num)
{
    m_bBusy = true;

    addr_info_t    addr;
    char loc_ip[20] = {'\0'} ;
    char loc_usr[20] = {'\0'} ;
    char rem_ip[20] = {'\0'};
    char rem_usr[20] = {'\0'};
    char strIP[50] = {'\0'};

    GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;
    strcpy(loc_ip, pGetPeerIPThread->m_nativeIP.toLatin1().data());
    strcpy(loc_usr, pGetPeerIPThread->m_nativeName.toLatin1().data());

    if (pGetPeerIPThread->GetPeerIP(g_strLphoneName[num + 1], strIP) != 0)
    {
        m_bBusy = false;
        return-1 ;
    }
    char *ptr;
    ptr = strstr(strIP, "?");
    if (ptr)
    {
        *ptr = '\0';
    }

    strcpy(rem_ip,strIP);
    strcpy(rem_usr,g_strLphoneName[num + 1]);

    addr.loc_ip = (uint8_t *)loc_ip;
    addr.loc_usr = (uint8_t *)loc_usr;
    addr.rem_ip = (uint8_t *)rem_ip;
    addr.rem_usr = (uint8_t *)rem_usr;

    unsigned int  file_len = 0;
    FILE 	      *fp = NULL;
    char	      file_path[64] = {0};
    sprintf(file_path, "%s/%s", big_file->dir_name, big_file->file_name);
    if ((fp = fopen(file_path, "r")) == NULL)
    {
        m_bBusy = false;
        return -1;
    }

    fseek(fp, 0L, SEEK_END);
    file_len = ftell(fp);
    fclose(fp);

    if(num == TO_MANAGER_MACHINE)
    {
        int res = start_big_0x0601(&addr, 0x05, big_file, file_len, sendpic_to_manager_result_callback);
        printf("start_big_0x0601 res1 = %d\n",res);
    }
    else
    {
        int res = start_big_0x0601(&addr, 0x05, big_file, file_len, sendpic_to_computer_result_callback);
        printf("start_big_0x0601 res2 = %d\n",res);
    }
    m_bBusy = false;
    return 0;
}

/************************************************************
描述：使用tcp协议发送图片
参数：buf : 待发送图片数据缓冲区
     buf_len : 待发送图片大小
     jpg_num : 待发送图片编号
     num : 为0表示向管理机发送，为1表示向物业软件发送
返回：无
************************************************************/
void JpegSendThread::SendPicByTcp(char *buf, int buf_len , int jpg_num , int num)
{
    m_bBusy = true;

    m_pTcpSock[num]->write((char *)&jpg_num, 4);
    m_pTcpSock[num]->write((char *)&buf_len, 4);
    m_pTcpSock[num]->write(buf, buf_len);
    m_pTcpSock[num]->waitForBytesWritten(2000);
    m_pTcpSock[num]->flush();

    m_bBusy = false;

}

/************************************************************
描述：JPEG发送线程主函数，向管理机和物业软件每隔3秒钟发送一幅JPEG格式的报警图像，
     最多发送6张图像
参数：无
返回：无
************************************************************/
void JpegSendThread::run()
{
    int image_width = 720 - TVD_LEFT_SPAN - TVD_RIGHT_SPAN, image_height = 576 - TVD_TOP_SPAN - TVD_BOTTOM_SPAN;
    char *buf = (char *)malloc(image_width * image_height  * 2);
    int jpgCnt = 0;
    QString strFileName;
    bool ret;
    big_file_t big_file;
    char file_name[64] = {'\0'};
    char path_name[] = "/mnt/disk/alert_pic";
    QDateTime datetime =  g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_AlarmDateTime;
    QString loc_name = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread->m_nativeName;
    int i;

    if(g_check_managemachine_udp_state != RESULT_SUCCESS && g_check_computercenter_udp_state != RESULT_SUCCESS)//防止发送物业与管理机的时候都使用udp发送，两个socket都为NULL导致函数返回。
    {
        for (i = 0; i < 2; i++)
        {
            if (m_pTcpSock[i] && (m_pTcpSock[i]->state() != QTcpSocket::ConnectedState))
            {
                printf("lg 0:%d\n", i);
                delete m_pTcpSock[i];
                m_pTcpSock[i] = NULL;
            }
        }

        if ((!m_pTcpSock[0]) && (!m_pTcpSock[1]))
        {
            m_bRun = false;
            free(buf);
            return;
        }
    }


    printf("JpegSendThread::run 1\n");

    unsigned char tag = 0x90;

    if (g_check_managemachine_udp_state != RESULT_SUCCESS && m_pTcpSock[TO_MANAGER_MACHINE])//防止管理机与物业有一个断网时候
    {
        m_pTcpSock[TO_MANAGER_MACHINE]->write((char *)&tag, sizeof(unsigned char));
    }
    if (g_check_computercenter_udp_state != RESULT_SUCCESS &&  m_pTcpSock[TO_COMPUTER_CENTER])
    {
        m_pTcpSock[TO_COMPUTER_CENTER]->write((char *)&tag, sizeof(unsigned char));

    }

    while (m_bRun && (g_jpg_tvd_image_ready != 2))
    {
        printf("lg 1 wait\n");
        usleep(200000);
    }

    g_jpg_tvd_image_ready = 0;

    while (m_bRun)
    {
        /*while (m_bRun && (g_jpg_image_ready == 0))
        {
            usleep(15000);
        }

        if (!m_bRun) break;
        */
        printf("JpegSendThread::run 2 g_jpg_tvd_image_ready:%d\n",g_jpg_tvd_image_ready);

        if (g_jpg_tvd_image_ready == 1)
        {
            FILE *fp = NULL;
            int len = 0;

            tvd_yuv420_to_rgb565((unsigned char *)g_jpg_tvd_image_buf, (unsigned short *)buf, image_width, image_height);
            printf("JpegSendThread::run 3-1\n");

            strFileName = "/mnt/disk/alert_pic/" + loc_name + "_"+ datetime.toString("yyyyMMddhhmmss") + "-" +
                          QString::number(m_alarm_num) + "-" + QString::number(jpgCnt) + ".jpg";
            QImage image((unsigned char *)buf, image_width, image_height, QImage::Format_RGB16);

            ret = image.save(strFileName, "jpg");

            if(g_check_managemachine_udp_state == RESULT_SUCCESS && g_check_computercenter_udp_state == RESULT_SUCCESS)
            {
                sprintf(file_name,"%s_%s-%d-%d.jpg",loc_name.toLocal8Bit().data(),datetime.toString("yyyyMMddhhmmss").toLocal8Bit().data(),m_alarm_num,jpgCnt);
                big_file.dir_name = (uint8_t*)path_name;
                big_file.dir_namelen = (uint8_t)strlen(path_name);
                big_file.file_name = (uint8_t*)file_name;
                big_file.file_namelen = (uint8_t)strlen(file_name);
                m_FileNameList.append(QString(file_name));
                m_FileDirList.append(strFileName);

            }
            else if(g_check_managemachine_udp_state != RESULT_SUCCESS && g_check_computercenter_udp_state != RESULT_SUCCESS)
            {
                fp = fopen(strFileName.toLocal8Bit().data(), "rb");
                len = fread(buf, 1, image_width * image_height * 2, fp);
                fclose(fp);
                printf("send jpg:%d len:%d\n", jpgCnt, len);
            }
            else
            {
                sprintf(file_name,"%s_%s-%d-%d.jpg",loc_name.toLocal8Bit().data(),datetime.toString("yyyyMMddhhmmss").toLocal8Bit().data(),m_alarm_num,jpgCnt);
                big_file.dir_name = (uint8_t*)path_name;
                big_file.dir_namelen = (uint8_t)strlen(path_name);
                big_file.file_name = (uint8_t*)file_name;
                big_file.file_namelen = (uint8_t)strlen(file_name);
                m_FileNameList.append(QString(file_name));
                m_FileDirList.append(strFileName);

                fp = fopen(strFileName.toLocal8Bit().data(), "rb");
                len = fread(buf, 1, image_width * image_height * 2, fp);
                fclose(fp);
                printf("send jpg:%d len:%d\n", jpgCnt, len);
            }

            if(g_check_managemachine_udp_state == RESULT_SUCCESS)
            {
                int res = SendPicByUdp(&big_file,TO_MANAGER_MACHINE);
                if(res != 0) //若使用udp发送图片给管理机失败，则需判断是否使用udp发送图片给物业中心
                {
                    if(g_check_computercenter_udp_state == RESULT_SUCCESS)
                    {
                        int res2 = SendPicByUdp(&big_file,TO_COMPUTER_CENTER);
                        if(res2 != 0) //用udp发送图片给物业软件也失败
                        {
                            if(!m_FileNameList.isEmpty())
                            {
                                m_FileNameList.takeFirst();
                            }
                            if(!m_FileDirList.isEmpty())
                            {
                                m_FileDirList.takeFirst();
                            }

                            QFile::remove(strFileName);
                        }
                        else
                        {
                            if(!m_FileNameList.isEmpty())
                            {
                                m_FileNameList.takeFirst();
                            }
                        }
                    }
                }

                if(g_check_computercenter_udp_state != RESULT_SUCCESS)
                {                                    
                    if( m_pTcpSock[TO_COMPUTER_CENTER] && ( m_pTcpSock[TO_COMPUTER_CENTER]->state() == QTcpSocket::ConnectedState ) )
                    {
                        SendPicByTcp(buf,len,jpgCnt,TO_COMPUTER_CENTER);
                    }
                    else
                    {
                        delete m_pTcpSock[TO_COMPUTER_CENTER];
                        m_pTcpSock[TO_COMPUTER_CENTER] = NULL;
                    }

                    if(res != 0) //若使用udp发送图片给管理机失败，使用tcp发送图片给物业后也需删除图片
                    {
                        if(!m_FileNameList.isEmpty())
                        {
                            m_FileNameList.takeFirst();
                        }
                        if(!m_FileDirList.isEmpty())
                        {
                            m_FileDirList.takeFirst();
                        }
                        QFile::remove(strFileName);
                    }
                }
            }
            else
            {               
                if( m_pTcpSock[TO_MANAGER_MACHINE] && ( m_pTcpSock[TO_MANAGER_MACHINE]->state() == QTcpSocket::ConnectedState ))
                {
                    SendPicByTcp(buf,len,jpgCnt,TO_MANAGER_MACHINE);
                }
                else
                {
                    delete m_pTcpSock[TO_MANAGER_MACHINE];
                    m_pTcpSock[TO_MANAGER_MACHINE] = NULL;
                }

                if(g_check_computercenter_udp_state == RESULT_SUCCESS)
                {                  
                    int res = SendPicByUdp(&big_file,TO_COMPUTER_CENTER);
                    if(res != 0) //若使用tcp发送图片给管理机 使用udp发送图片给物业且发送失败
                    {
                        if(!m_FileNameList.isEmpty())
                        {
                            m_FileNameList.takeFirst();
                        }
                        if(!m_FileDirList.isEmpty())
                        {
                            m_FileDirList.takeFirst();
                        }
                        QFile::remove(strFileName);
                    }
                    else
                    {
                        if(!m_FileNameList.isEmpty())
                        {
                            m_FileNameList.takeFirst();
                        }
                    }
                }
                else
                {                                      
                    if( m_pTcpSock[TO_COMPUTER_CENTER] && ( m_pTcpSock[TO_COMPUTER_CENTER]->state() == QTcpSocket::ConnectedState ) )
                    {
                        SendPicByTcp(buf,len,jpgCnt,TO_COMPUTER_CENTER);
                    }
                    else
                    {
                        delete m_pTcpSock[TO_COMPUTER_CENTER];
                        m_pTcpSock[TO_COMPUTER_CENTER] = NULL;
                    }
                }
            }

            if( g_check_managemachine_udp_state != RESULT_SUCCESS && g_check_computercenter_udp_state != RESULT_SUCCESS  )
            {
                QFile::remove(strFileName);
                if ((m_pTcpSock[TO_MANAGER_MACHINE] == NULL) && (m_pTcpSock[TO_COMPUTER_CENTER] == NULL))
                {
                    break;
                }
            }
            g_jpg_tvd_image_ready = 0;
            //printf("JpegSendThread::run 6:%d\n", jpgCnt);

            if (++jpgCnt >= 6)
            {
                break;
            }
        }

        /*loop = 0;
        while (m_bRun)
        {
            //printf("lg 2\n");

            if (loop++ >= 3000000/200000)
            {
                break;
            }
            usleep(200000);
        }*/
        usleep(200000);
        //QFile::remove(strFileName);
    }

    free(buf);

    g_jpg_tvd_image_ready = 3;//bug
    //printf("JpegSendThread::run 7\n");
////jump:
    for (i = 0; i < 2; i++)
    {
        if (m_pTcpSock[i])
        {
            m_pTcpSock[i]->close();
            delete m_pTcpSock[i];
            m_pTcpSock[i] = NULL;
        }
    }

    m_alarm_num = -1;
    m_bRun = false;

}
