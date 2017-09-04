#include "sendinfothread.h"
#include "maindialog.h"

static RESULT_E g_check_remote_udp_state ;//协议探测结果
extern MainDialog *g_pMainDlg;
extern char g_strNativeName[50];

void info_check_result_callback(RESULT_E result) //协议探测回调函数
{
    printf("=============[%s -- %d] result = %d\n", __func__, __LINE__, result);
    g_check_remote_udp_state = result;
}

SendInfoThread::SendInfoThread(QObject *parent) :
    QThread(parent)
{
    m_bRun = false;
}

SendInfoThread::~SendInfoThread()
{
}

void SendInfoThread::run()
{
    m_bRun = true;
    g_check_remote_udp_state = RESULT_NULL;

    CheckUdpState();

    if (g_check_remote_udp_state == RESULT_SUCCESS)
    {
        ((InfoWriteDialog *)parent())->emitSigUdpSendInfo();
    }
    else
    {
        ((InfoWriteDialog *)parent())->emitSigTcpSendInfo();
    }

    m_bRun = false;
}

void SendInfoThread::CheckUdpState()
{
    addr_info_t    addr;
    QString strPeerName = m_sendToName;
    char loc_ip[20] = {'\0'} ;
    char loc_usr[20] = {'\0'} ;
    char rem_ip[20] = {'\0'};
    char rem_usr[20] = {'\0'};
    char strIP[50] = {'\0'};

    GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;
    strcpy(loc_ip, pGetPeerIPThread->m_nativeIP.toLatin1().data());
    strcpy(loc_usr, pGetPeerIPThread->m_nativeName.toLatin1().data());

    int len = strPeerName.length();
    QString strTemp;
    if ((strPeerName.at(len - 2) == '-') && (strPeerName.at(len - 1).isDigit()))
    {
        strTemp = strPeerName.left(len - 2);
    }
    else
    {
        strTemp = strPeerName;
    }

    if (strTemp.indexOf("-") == -1)
    {
        if (g_strNativeName[0] != '\0')
        {
            int pos;

            strTemp = g_strNativeName;
            pos = strTemp.indexOf("?");
            if (pos >= 0) strTemp = strTemp.left(pos);

            len = strTemp.length();
            if ((strTemp.at(len - 2) == '-') && (strTemp.at(len - 1).isDigit()))
            {
                strTemp = strTemp.left(len - 2);
            }

            pos = strTemp.indexOf("-");
            if (pos != -1)
            {
                strTemp = strTemp.left(pos);
                strPeerName = strTemp + "-" + strPeerName;
            }
        }
    }

    if (pGetPeerIPThread->GetPeerIP(strPeerName.toLocal8Bit().data(), strIP) != 0)
    {
        return;
    }

    char *ptr;
    ptr = strstr(strIP, "?");
    if (ptr)
    {
        *ptr = '\0';
    }


    strcpy(rem_ip,strIP);
    strcpy(rem_usr,strPeerName.toLocal8Bit().data());

    addr.loc_ip = (uint8_t *)loc_ip;
    addr.loc_usr = (uint8_t *)loc_usr;
    addr.rem_ip = (uint8_t *)rem_ip;
    addr.rem_usr = (uint8_t *)rem_usr;

    bool ret = start_system_0x0501(&addr, info_check_result_callback);
    printf("start_system_0x0501 ret = %d\n",ret);
    if(ret)
    {
        int time_count = 0;
        while (1)
        {
            usleep(100 * 1000);

            if (g_check_remote_udp_state != RESULT_NULL || time_count++ >= 60)
            {
                break;
            }
        }
        stop_system_0x0501();
    }

}


