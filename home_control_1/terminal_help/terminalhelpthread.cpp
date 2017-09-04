#include "terminalhelpthread.h"
#include "terminalhelpmaindialog.h"
#include "maindialog.h"
#include <QMessageBox>
#include "core/leelen_comdef.h"
/************************************************************
  该文件为终端求助发送线程主程序
************************************************************/

extern char g_strNativeName[50];
extern char g_strLphoneName[3][50];
extern RESULT_E g_check_managemachine_udp_state ;
extern RESULT_E g_check_computercenter_udp_state ;
extern MainDialog *g_pMainDlg;
QString g_strInfo;
void terminal_help_manager_result_callback(RESULT_E result)
{

    if(result == RESULT_SUCCESS)
    {
        g_pMainDlg->m_pTermHelpThread->m_managerHelpResultFlag = SEND_RES_SUCCESS;
    }
    else
    {
        g_pMainDlg->m_pTermHelpThread->m_managerHelpResultFlag = SEND_RES_FALSE;
    }

}

void terminal_help_computer_result_callback(RESULT_E result)
{
    if(result == RESULT_SUCCESS)
    {
        g_pMainDlg->m_pTermHelpThread->m_computerHelpResultFlag = SEND_RES_SUCCESS;
    }
    else
    {
        g_pMainDlg->m_pTermHelpThread->m_computerHelpResultFlag = SEND_RES_FALSE;
    }

}
/************************************************************
描述：终端求助发送线程构造函数
参数：parent - 该线程的父对象指针
返回：无
************************************************************/
TerminalHelpThread::TerminalHelpThread(QObject *parent) :
    QThread(parent)
{
    m_managerHelpResultFlag = SEND_RES_NULL;
    m_computerHelpResultFlag = SEND_RES_NULL;
}

/************************************************************
描述：终端求助发送线程主函数，该主函数分别向管理中心机和管理中心计算机
	 发送终端求助信息，接收管理机的返回信号并生成显示信息
参数：无
返回：无
************************************************************/
void TerminalHelpThread::run()
{
    printf("TerminalHelpThread run\n");
//    MainDialog *pMainDlg = (MainDialog *)parent();

//    int i;
//    //const char *str[2] = {QT_TRANSLATE_NOOP("TerminalHelpThread", "管理中心计算机"), QT_TRANSLATE_NOOP("TerminalHelpThread", "管理中心机")};
//    GetPeerIPThread *pGetPeerIPThread = pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;
//    QString strInfo = tr("终端求助发送失败");
//    for (i = 0; i < 2; i++)
//    {
//        char strIP[200] = "192.168.1.107?16";
//        printf("g_strLphoneName %d:%s\n",i,g_strLphoneName[i + 1]);
//        if (g_strLphoneName[i + 1][0] == '\0') continue;

//        if (pGetPeerIPThread->GetPeerIP(g_strLphoneName[i + 1], strIP) != 0)
//        {
//            if (i == 0)
//            {
//                strInfo = tr("获取管理中心机IP失败");
//            }
//            else
//            {
//                strInfo += tr("\n获取管理中心计算机IP失败");
//            }
//            //model->setData((i == 0) ? index0 : index1, tr("获取") + tr(str[i]) + tr("IP失败"));
//            continue;
//        }

//        char *ptr;
//        int iModalType = BASIC_MODAL_TYPE;

//        ptr = strstr(strIP, "?");
//        if (ptr)
//        {
//            iModalType = atoi(ptr + strlen("?"));
//            *ptr = '\0';
//        }

//        if (iModalType != MANCENTER_MODAL_TYPE)
//        {
//            if (i == 0)
//            {
//                strInfo = tr("管理中心机设备类型错误");
//            }
//            else
//            {
//                strInfo += tr("\n管理中心计算机设备类型错误");
//            }

//            continue;
//        }

//        ptr = strstr(strIP, "->");
//        if (ptr)
//        {
//            ptr[0] = '\0';
//        }

//        QTcpSocket *pTcpSock = new QTcpSocket;
//        pTcpSock->connectToHost(strIP, 17731);
//        //int ret = 0;

//        if (pTcpSock->waitForConnected())
//        {
//            short len;

//            len = strlen(g_strNativeName);
//            pTcpSock->write((char *)&len, sizeof(short));
//            pTcpSock->write((char *)g_strNativeName, len);
//            pTcpSock->flush();

//            char buf[2];
//            int cnt = 0;
//            int loop = 0;
//            while (pTcpSock->waitForReadyRead(800))
//            {
//                cnt += pTcpSock->read(buf + cnt, 2 - cnt);
//                if (cnt >= 2) break;
//                if (++loop >= 2) break;
//            }

//            if (i == 0)
//            {
//                if (cnt == 2)
//                {
//                    if ((buf[0] == 1) && (buf[1] == 1))
//                    {
//                        strInfo = tr("管理中心机空闲，等待管理中心机回呼");
//                        //ret = 1;
//                    }
//                    else if ((buf[0] == 1) && (buf[1] == 0))
//                    {
//                        strInfo = tr("管理中心机忙");
//                    }
//                    else
//                    {
//                        strInfo = tr("管理中心机返回数据错误");
//                    }
//                }
//                else
//                {
//                    strInfo = tr("管理中心机返回数据错误");
//                }
//            }
//            else
//            {
//                if (cnt == 2)
//                {
//                    strInfo += tr("\n向管理中心计算机发送求助成功");
//                }
//                else
//                {
//                    strInfo += tr("\n管理中心计算机返回数据错误");
//                }
//            }

//            pTcpSock->close();
//        }
//        else
//        {
//            if (i == 0)
//            {
//                strInfo = tr("管理中心机连接失败");
//            }
//            else
//            {
//                strInfo += tr("\n管理中心计算机连接失败");
//            }
//        }

//        delete pTcpSock;

//        //if (ret == 1) break;
//    }

//    emit sigShowTermHelpResult(strInfo);
    g_check_managemachine_udp_state = RESULT_NULL;
    g_check_computercenter_udp_state = RESULT_NULL;
    m_managerHelpResultFlag = SEND_RES_NULL;
    m_computerHelpResultFlag = SEND_RES_NULL;
    g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->CheckUdpState();

    if(g_check_managemachine_udp_state == RESULT_SUCCESS)
    {
        UdpTerminalHelp(TO_MANAGER_MACHINE);

        int time_count = 0;
        while (1)
        {
            if(m_managerHelpResultFlag == SEND_RES_FALSE)
            {
                break;
            }
            usleep(100 * 1000);
            if (m_managerHelpResultFlag != SEND_RES_NULL || time_count++ >= 40)
            {
                if(m_managerHelpResultFlag == SEND_RES_SUCCESS)
                {
                    g_strInfo = tr("向管理中心机发送求助成功");
                }
                else
                {
                    g_strInfo = tr("向管理中心机发送求助失败");
                }
                break;
            }        
        }
    }
    else
    {
        TcpTerminalHelp(TO_MANAGER_MACHINE);
    }



    if(g_check_computercenter_udp_state == RESULT_SUCCESS)
    {
        UdpTerminalHelp(TO_COMPUTER_CENTER);

        int time_count = 0;
        while (1)
        {
            if(m_computerHelpResultFlag == SEND_RES_FALSE)
            {
                break;
            }
            usleep(100 * 1000);
            if (m_computerHelpResultFlag != SEND_RES_NULL || time_count++ >= 40)
            {
                if(m_computerHelpResultFlag == SEND_RES_SUCCESS)
                {
                    g_strInfo += tr("\n向管理中心计算机发送求助成功");
                }
                else
                {
                    g_strInfo += tr("\n向管理中心计算机发送求助失败");
                }
                break;
            }
        }
    }
    else
    {
        TcpTerminalHelp(TO_COMPUTER_CENTER);
    }

    emit sigShowTermHelpResult(g_strInfo);
}


void TerminalHelpThread::TcpTerminalHelp(int destination)
{
    //const char *str[2] = {QT_TRANSLATE_NOOP("TerminalHelpThread", "管理中心计算机"), QT_TRANSLATE_NOOP("TerminalHelpThread", "管理中心机")};
    GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;
    //QString strInfo = tr("终端求助发送失败");

    char strIP[200] = "192.168.1.107?16";
    printf("g_strLphoneName %d:%s\n",destination,g_strLphoneName[destination + 1]);
    if (g_strLphoneName[destination + 1][0] == '\0')
    {
        return;
    }

    if (pGetPeerIPThread->GetPeerIP(g_strLphoneName[destination + 1], strIP) != 0)
    {
        if (destination == 0)
        {
            g_strInfo = tr("获取管理中心机IP失败");
        }
        else
        {
            g_strInfo += tr("\n获取管理中心计算机IP失败");
        }
        //model->setData((i == 0) ? index0 : index1, tr("获取") + tr(str[i]) + tr("IP失败"));
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
        if (destination == 0)
        {
            g_strInfo = tr("管理中心机设备类型错误");
        }
        else
        {
            g_strInfo += tr("\n管理中心计算机设备类型错误");
        }

        return;
    }

    ptr = strstr(strIP, "->");
    if (ptr)
    {
        ptr[0] = '\0';
    }

    QTcpSocket *pTcpSock = new QTcpSocket;
    pTcpSock->connectToHost(strIP, 17731);
    //int ret = 0;

    if (pTcpSock->waitForConnected())
    {
        short len;

        len = strlen(g_strNativeName);
        pTcpSock->write((char *)&len, sizeof(short));
        pTcpSock->write((char *)g_strNativeName, len);
        pTcpSock->flush();

        char buf[2];
        int cnt = 0;
        int loop = 0;
        while (pTcpSock->waitForReadyRead(800))
        {
            cnt += pTcpSock->read(buf + cnt, 2 - cnt);
            if (cnt >= 2) break;
            if (++loop >= 2) break;
        }

        if (destination == 0)
        {
            if (cnt == 2)
            {
                if ((buf[0] == 1) && (buf[1] == 1))
                {
                    g_strInfo = tr("管理中心机空闲，等待管理中心机回呼");
                    //ret = 1;
                }
                else if ((buf[0] == 1) && (buf[1] == 0))
                {
                    g_strInfo = tr("管理中心机忙");
                }
                else
                {
                    g_strInfo = tr("管理中心机返回数据错误");
                }
            }
            else
            {
                g_strInfo = tr("管理中心机返回数据错误");
            }
        }
        else
        {
            if (cnt == 2)
            {
                g_strInfo += tr("\n向管理中心计算机发送求助成功");
            }
            else
            {
                g_strInfo += tr("\n管理中心计算机返回数据错误");
            }
        }

        pTcpSock->close();
    }
    else
    {
        if (destination == 0)
        {
            g_strInfo = tr("管理中心机连接失败");
        }
        else
        {
            g_strInfo += tr("\n管理中心计算机连接失败");
        }
    }

    delete pTcpSock;

        //if (ret == 1) break;    
}
void TerminalHelpThread::UdpTerminalHelp(int destination)
{
    addr_info_t    addr;
    char loc_ip[20] = {'\0'} ;
    char loc_usr[20] = {'\0'} ;
    char rem_ip[20] = {'\0'};
    char rem_usr[20] = {'\0'};
    char strIP[50] = {'\0'};

    GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;
    strcpy(loc_ip, pGetPeerIPThread->m_nativeIP.toLatin1().data());
    strcpy(loc_usr, pGetPeerIPThread->m_nativeName.toLatin1().data());

    if (pGetPeerIPThread->GetPeerIP(g_strLphoneName[destination+1], strIP) != 0)
    {
        if (destination == 0)
        {
            g_strInfo = tr("获取管理中心机IP失败");
            m_managerHelpResultFlag = SEND_RES_FALSE;
        }
        else
        {
            g_strInfo += tr("\n获取管理中心计算机IP失败");
            m_computerHelpResultFlag = SEND_RES_FALSE;
        }

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
        if (destination == 0)
        {
            g_strInfo = tr("管理中心机设备类型错误");
            m_managerHelpResultFlag = SEND_RES_FALSE;
        }
        else
        {
            g_strInfo += tr("\n管理中心计算机设备类型错误");
            m_computerHelpResultFlag = SEND_RES_FALSE;
        }

        return;
    }

    strcpy(rem_ip,strIP);
    strcpy(rem_usr,g_strLphoneName[2]);

    addr.loc_ip = (uint8_t *)loc_ip;
    addr.loc_usr = (uint8_t *)loc_usr;
    addr.rem_ip = (uint8_t *)rem_ip;
    addr.rem_usr = (uint8_t *)rem_usr;

    if(destination == TO_MANAGER_MACHINE)
    {
        int ret = start_security_0x0004(&addr, terminal_help_manager_result_callback);
        printf("start_security_0x0004 ret = %d\n",ret);
    }
    else
    {
        int ret = start_security_0x0004(&addr, terminal_help_computer_result_callback);
        printf("start_security_0x0004 ret = %d\n",ret);
    }

}

