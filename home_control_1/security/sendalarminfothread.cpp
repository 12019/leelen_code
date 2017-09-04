#include "sendalarminfothread.h"
#include "core/leelen_comdef.h"
#include "maindialog.h"

extern RESULT_E g_check_managemachine_udp_state ;
extern RESULT_E g_check_computercenter_udp_state ;

SendAlarmInfoThread::SendAlarmInfoThread(QObject *parent) :
    QThread(parent)
{
    m_bRun = false;
    m_StarSendJpegThreadFlag = false;
    m_num = -1;
}

SendAlarmInfoThread::~SendAlarmInfoThread()
{

}

void SendAlarmInfoThread::run()
{
    m_bRun = true;

    g_check_managemachine_udp_state = RESULT_NULL;
    g_check_computercenter_udp_state = RESULT_NULL;
    ((Serial1Thread*)parent())->CheckUdpState();

    if (((Serial1Thread*)parent())->m_bIsRecord)
    {
        printf("stop Jpeg Sent Thread\n");
        while (((Serial1Thread*)parent())->m_pJpegSendThread->m_bBusy)
        {
            usleep(10000);
        }

        if (((Serial1Thread*)parent())->m_pJpegSendThread->m_bRun)
        {
            ((Serial1Thread*)parent())->m_pJpegSendThread->m_bRun = false;
            ((Serial1Thread*)parent())->m_pJpegSendThread->quit();
        }

        ((Serial1Thread*)parent())->m_pJpegSendThread->wait();

    }
    printf("g_check_managemachine_udp_state===%d\n",g_check_managemachine_udp_state);
    printf("g_check_computercenter_udp_state===%d\n",g_check_computercenter_udp_state);
    ((Serial1Thread*)parent())->m_AlarmDateTime = QDateTime::currentDateTime();
    if (g_check_managemachine_udp_state == RESULT_SUCCESS)
    {

        ((Serial1Thread*)parent())->SendByUdp(m_num,TO_MANAGER_MACHINE); //使用udp发送报警信息
    }
    else
    {

        ((Serial1Thread*)parent())->SendByTcp(m_num,TO_MANAGER_MACHINE); //使用tcp发送报警信息
    }

    if (g_check_computercenter_udp_state == RESULT_SUCCESS)
    {
        if( g_check_managemachine_udp_state == RESULT_SUCCESS)
        {
            usleep(400*1000);
        }

        ((Serial1Thread*)parent())->SendByUdp(m_num,TO_COMPUTER_CENTER);
    }
    else
    {

        ((Serial1Thread*)parent())->SendByTcp(m_num,TO_COMPUTER_CENTER);
    }

    while(m_bRun)
    {
        if(m_StarSendJpegThreadFlag == false)
        {
            usleep(100*1000);
        }
        else
        {
            if (((Serial1Thread*)parent())->m_pJpegSendThread )
            {

                printf("m_iSendPicToCenter:%d\n",((SecurityMainDialog*)parent()->parent())->m_iSendPicToCenter);
                if (((SecurityMainDialog*)parent()->parent())->m_iSendPicToCenter)
                {
                    //g_jpg_tvd_image_ready = 3;//bug
                    ((Serial1Thread*)parent())->m_pJpegSendThread->m_bRun = true;
                    ((Serial1Thread*)parent())->m_pJpegSendThread-> m_alarm_num = m_num;
                    ((Serial1Thread*)parent())->m_pJpegSendThread->start();
                    break;
                }
                else
                {
                    int i;
                    for (i = 0; i < 2; i++)
                    {
                        if (((Serial1Thread*)parent())->m_pJpegSendThread->m_pTcpSock[i])
                        {
                            ((Serial1Thread*)parent())->m_pJpegSendThread->m_pTcpSock[i]->close();
                            delete ((Serial1Thread*)parent())->m_pJpegSendThread->m_pTcpSock[i];
                            ((Serial1Thread*)parent())->m_pJpegSendThread->m_pTcpSock[i] = NULL;
                        }
                    }
                    break;
                }
            }
        }
    }

    m_num = -1;
    m_StarSendJpegThreadFlag = false;
    m_bRun = false;
}
