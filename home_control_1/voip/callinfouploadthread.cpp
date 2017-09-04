#include "callinfouploadthread.h"
#include <Qt/qtcpsocket.h>
#include "voipmaindialog.h"
#include "core/leelen_comdef.h"
#include "leelen_protocol.h"
#include "leelen_system.h"
#include "leelen_talk.h"
#include <QFile>
#include "mediastreamer2/mediastream.h"
#include "lpcore.h"
#include "lpconfig.h"
#include "maindialog.h"
#include <QSettings>
extern LinphoneCore *lphone;
extern unsigned char *g_jpg_image_buf;
extern int g_jpg_image_ready;
extern char g_strLphoneName[3][50];
extern char g_strNativeName[50];
extern MainDialog *g_pMainDlg;

extern char g_strLphoneName[3][50];
extern char g_strNativeName[50];


extern RESULT_E g_check_computercenter_udp_state;
void result_callback(RESULT_E result)
{
    printf("=============[%s -- %d] result = %d\n", __func__, __LINE__, result);
    g_check_computercenter_udp_state = result;
}

static void yuv420_to_rgb565(unsigned char *src, unsigned short *dst, int width, int height)
{
    int line, col, linewidth;
    int y, u, v, yy, vr, ug, vg, ub;
    int r, g, b;
    const unsigned char *py, *pu, *pv;

    linewidth = width >> 1;
    py = src;
    pu = py + (width * height);
    pv = pu + (width * height) / 4;

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
                pu++;
                pv++;

                u = *pu - 128;
                ug =   88 * u;
                ub = 454 * u;
                v = *pv - 128;
                vg = 183 * v;
                vr = 359 * v;
            }
        } /* ..for col */
        if ((line & 1) == 0) { // even line: rewind
            pu -= linewidth;
            pv -= linewidth;
        }
    } /* ..for line */
}

CallInfoUploadThread::CallInfoUploadThread(QObject *parent) :
    QThread(parent)
{
    m_bRun = false;
    m_InfoUploadSocket = NULL;
    m_IsUpImage = 0;
    m_upInfoType = 0;
    m_TalkTime = 0;
    m_TalkTimer = new QTimer(this);
    connect(m_TalkTimer,SIGNAL(timeout()),this,SLOT(slotPhoneTime()));

}

CallInfoUploadThread::~CallInfoUploadThread()
{

}

int CallInfoUploadThread::connectToCenter()
{
    if(m_InfoUploadSocket)  delete m_InfoUploadSocket;

    m_InfoUploadSocket = new QTcpSocket;

    char strIP[80];

    GetPeerIPThread *pGetPeerIPThread = ((VoipMainDialog *)parent())->m_pGetPeerIPThread;

    if(pGetPeerIPThread->GetPeerIP(g_strLphoneName[2],strIP) != 0)
    {
        printf("can not conenct to manage center software\n");
        return -1;
    }

    char *ptr;
    int iModalType = 0;

    ptr = strstr(strIP,"?");

    if(ptr)
    {
        iModalType = atoi(ptr + strlen("?"));
        *ptr = '\0';
    }

    if(iModalType != MANCENTER_MODAL_TYPE)  return -1;

    m_InfoUploadSocket->connectToHost(strIP,17721);

    if(!m_InfoUploadSocket->waitForConnected(500))
    {
        delete m_InfoUploadSocket;
        m_InfoUploadSocket = NULL;
    }

   return 0;
}
/************************************************************
描述：记录通话时长，m_taletime一秒加一
参数：无
返回：无
************************************************************/
void CallInfoUploadThread::slotPhoneTime()
{
    m_TalkTime++;
}

void CallInfoUploadThread::getCallCode(QString str)
{
    strPeerName = str;
}

void CallInfoUploadThread::run()
{
    m_bRun = true;

    int image_width = 352,image_height = 288;

    if(lphone->video_resolution == 0)
    {
        image_width = 352;
        image_height = 288;
    }
    else if(lphone->video_resolution == 1)
    {
        image_width = USED_VGA_VIDEO_W;
        image_height = USED_VGA_VIDEO_H;
    }
    else if(lphone->video_resolution == 2)
    {
        image_width = 720;
        image_height = 288;
    }

    int ret;
    ret = connectToCenter();
    if(ret != 0)
    {
        printf("can not connect to center\n");
        m_bRun = false;
        return;
    }

    if(!m_InfoUploadSocket)
    {
        m_bRun = false;
        return;
    }

    printf("strpeername:%s\n",qPrintable(strPeerName));
    if(m_upInfoType == 1)
    {
        g_check_computercenter_udp_state = RESULT_NULL;
        printf("=============[%s -- %d] \n", __func__, __LINE__);
        CheckUdpState();

        int time_count = 0;
        while (1)
        {
            usleep(100 * 1000);

            if (g_check_computercenter_udp_state != RESULT_NULL || time_count++ >= 60)
            {
                break;
            }
        }

        stop_system_0x0501();

        if(g_check_computercenter_udp_state == RESULT_SUCCESS)
        {
            m_upInfoType = 0;
            m_bRun = false;
            return;
        }

        printf("m_upInfoType 1\n");
        unsigned char tag = 0x80;
        char callCodeLen = 9;

        char callCode[9];

        memcpy(callCode,qPrintable(strPeerName),9);

        if(m_InfoUploadSocket)
        {
            m_InfoUploadSocket->flush();
            m_InfoUploadSocket->write((char *)&tag,sizeof(unsigned char));
            m_InfoUploadSocket->write((char *)&callCodeLen,sizeof(char));
            m_InfoUploadSocket->write(callCode,9);

            m_InfoUploadSocket->flush();

         }

        if(m_IsUpImage == 0)
        {
            int len = 0;

            m_InfoUploadSocket->write((char *)&len,4);

            m_InfoUploadSocket->flush();
        }
        else
        {
            m_IsUpImage = 0;

            char *buf = (char *)malloc(image_width * image_height * 2);

            QString strFileName;

            while (m_bRun && (g_jpg_image_ready != 2))
            {
                usleep(200000);
            }

            g_jpg_image_ready = 0;

            m_IsUpImage = 0;

            int cnt = 0;

            while(m_bRun)
            {
                cnt++;

                if(cnt > 60 )
                {
                    cnt = 1;
                    break;
                }

                usleep(100000);

                if(g_jpg_image_ready == 1)
                {
                   yuv420_to_rgb565((unsigned char*)g_jpg_image_buf,(unsigned short*)buf,image_width,image_height);

                    strFileName = "/mnt/disk/door.jpg";

                    QImage image1((unsigned char*)buf,image_width,image_height,QImage::Format_RGB16);

                    if(lphone->video_resolution == 2)
                    {
                        image1 = image1.scaled(720,576);
                        image_height = 576;
                    }
                    QImage image;

                    if(lphone->video_resolution == 0)
                    {
                        image = image1.copy(6,0,340,288);
                        image_width = 340;
                        image_height = 288;
                    }
                    else if(lphone->video_resolution == 2)
                    {
                        image = image1.copy(6,2,690,576);

                        image_width = 690;
                        image_height = 576;
                    }

                    ret = image.save(strFileName,"jpg");

                    g_jpg_image_ready = 0;

                    FILE *fp = NULL;

                    int len;

                    fp = fopen(strFileName.toLocal8Bit().data(),"rb");

                    if(fp == NULL)
                    {
                        printf("no image file\n");
                        m_bRun = false;
                        return;
                    }

                    len = fread(buf,1,image_width * image_height * 2,fp);

                    fclose(fp);

                    QFile::remove(strFileName);

                    if(m_InfoUploadSocket)
                    {
                        if(m_InfoUploadSocket->state() == QTcpSocket::ConnectedState)
                        {
                            m_InfoUploadSocket->write((char *)&len,4);

                            m_InfoUploadSocket->flush();

                            int cnt  = len / 1000;

                            int i = 0;

                            while(i <= cnt)
                            {
                                if(i == cnt)
                                {
                                    m_InfoUploadSocket->write(buf + 1000 * i,len%1000);
                                    m_InfoUploadSocket->flush();
                                }
                                else
                                {
                                    m_InfoUploadSocket->write(buf + 1000 * i,1000);
                                    m_InfoUploadSocket->flush();
                                }
                                i++;
                            }

                            break;

                        }
                        else
                        {
                            delete m_InfoUploadSocket;
                            m_InfoUploadSocket = NULL;
                        }
                }
            }

        }

            if(cnt == 1)
            {
                int len = 0;

                m_InfoUploadSocket->write((char *)&len,4);

                m_InfoUploadSocket->flush();
            }

            g_jpg_image_ready = 3;

        free(buf);
        }

      }
      if(m_upInfoType == 2)
      {
          printf("m_upInfoType 2\n");
          unsigned char tag = 0x81;
          char callCodeLen = 9;

          char callCode[9];

          memcpy(callCode,qPrintable(strPeerName),9);


          if(m_InfoUploadSocket)
          {
              m_InfoUploadSocket->flush();
              m_InfoUploadSocket->write((char *)&tag,sizeof(unsigned char));
              m_InfoUploadSocket->write((char *)&callCodeLen,sizeof(char));
              m_InfoUploadSocket->write(callCode,9);

              m_InfoUploadSocket->flush();
            }
      }

      if(m_upInfoType == 3)
      {
          printf("m_upInfoType 3\n");
          unsigned char tag = 0x82;
          char callCodeLen = 9;

          char callCode[9];

          memcpy(callCode,qPrintable(strPeerName),9);

          if(m_InfoUploadSocket)
          {
              m_InfoUploadSocket->flush();
              m_InfoUploadSocket->write((char *)&tag,sizeof(unsigned char));
              m_InfoUploadSocket->write((char *)&callCodeLen,sizeof(char));
              m_InfoUploadSocket->write(callCode,9);

              m_InfoUploadSocket->flush();
          }

      }

    m_InfoUploadSocket->close();
    delete m_InfoUploadSocket;
    m_InfoUploadSocket = NULL;

    m_upInfoType = 0;

    m_bRun = false;

    m_IsUpImage = 0;

}
/************************************************************
描述：协议探测，物业中心是否支持udp上传通话记录
参数：无
返回：无
************************************************************/
void CallInfoUploadThread::CheckUdpState()
{
    addr_info_t    addr_computer;
    char loc_ip[20] = {'\0'} ;
    char loc_usr[20] = {'\0'} ;
    char remcomputer_ip[20] = {'\0'};
    char remcomputer_usr[20] = {'\0'};
    char strIP[50] = {'\0'};

    GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;
    strcpy(loc_ip, pGetPeerIPThread->m_nativeIP.toLatin1().data());
    strcpy(loc_usr, pGetPeerIPThread->m_nativeName.toLatin1().data());

    if (pGetPeerIPThread->GetPeerIP(g_strLphoneName[2], strIP) != 0)
    {
        return;
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
        return ;
    }

    strcpy(remcomputer_ip,strIP);
    strcpy(remcomputer_usr,g_strLphoneName[2]);

    addr_computer.loc_ip = (uint8_t *)loc_ip;
    addr_computer.loc_usr = (uint8_t *)loc_usr;
    addr_computer.rem_ip = (uint8_t *)remcomputer_ip;
    addr_computer.rem_usr = (uint8_t *)remcomputer_usr;

    int ret = start_system_0x0501(&addr_computer, result_callback);
    printf("start_system_0x0501 ret = %d\n",ret);

}
/************************************************************
描述：使用udp协议向物业中心上传通话记录
参数：无
返回：无
************************************************************/
void CallInfoUploadThread::SendRecordByUdp()
{
    addr_info_t    addr_center, addr_from_to;
    talk_record_t  rec;
    char strIP[50] = {'\0'};
    char loc_usr_ip[20] = {'\0'};
    char loc_usr_name[20] = {'\0'};
    char centersoft_ip[20] = {'\0'};
    char centersoft_name[20] = {'\0'};

    char talk_from_ip[20] = {'\0'};
    char talk_from_name[20] = {'\0'};
    char talk_to_ip[20] = {'\0'};
    char talk_to_name[20] = {'\0'};
    char str_name[20] = {'\0'};

    GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;
    strcpy(loc_usr_ip, pGetPeerIPThread->m_nativeIP.toLatin1().data());
    strcpy(str_name, pGetPeerIPThread->m_nativeName.toLatin1().data());

    strcpy(loc_usr_name,str_name);
    if (pGetPeerIPThread->GetPeerIP(g_strLphoneName[2], strIP) != 0)
    {
        return ;
    }
    char *ptr;
    ptr = strstr(strIP, "?");
    if (ptr)
    {
       *ptr = '\0';
    }

    strcpy(centersoft_ip,strIP);
    strcpy(centersoft_name,g_strLphoneName[2]);

    strcpy(talk_from_ip,loc_usr_ip);
    strcpy(talk_from_name,loc_usr_name);

    addr_center.loc_ip = (unsigned char*)loc_usr_ip;
    addr_center.loc_usr = (unsigned char*)loc_usr_name;
    addr_center.rem_ip = (unsigned char*)centersoft_ip;
    addr_center.rem_usr = (unsigned char*)centersoft_name;


    addr_from_to.loc_ip = (unsigned char*)talk_from_ip;
    addr_from_to.loc_usr = (unsigned char*)talk_from_name;
    strcpy(talk_to_name, strPeerName.toLatin1().data());
    addr_from_to.rem_usr = (unsigned char*)talk_to_name;
    memset(strIP,'\0',20);

    if (pGetPeerIPThread->GetPeerIP(strPeerName.toLocal8Bit().data(), strIP) != 0)
    {
        return ;
    }
    char *ptr2;
    ptr2 = strstr(strIP, "?");
    if (ptr2)
    {
       *ptr2 = '\0';
    }

    strcpy(talk_to_ip,strIP);
    addr_from_to.rem_ip = (unsigned char*)talk_to_ip;
    memset(&rec, 0x00, sizeof(talk_record_t));
    printf("m_upInfoType = %02x, m_TalkTime = %04x.\n", m_upInfoType, m_TalkTime);

    rec.answer = m_upInfoType & RICT_ANSER;
    rec.length = m_TalkTime;
    rec.message = (m_upInfoType & RICT_RECORD) >> 2;
    rec.unlock = (m_upInfoType & RICT_OPEND) >> 1;

    int res = start_talk_0x0301(&addr_center, &addr_from_to, &rec, 0, NULL, NULL);
    printf("start_talk_0x0301 res = %d\n",res);

    m_upInfoType = 0;
    m_TalkTime = 0;

}
