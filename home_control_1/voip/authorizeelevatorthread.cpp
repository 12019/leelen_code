#include "authorizeelevatorthread.h"
#include "core/leelen_comdef.h"
#include "leelen_protocol.h"
#include "leelen_system.h"
#include "leelen_lift.h"
#include "core/leelen_utils.h"
#include "maindialog.h"


RESULT_E g_check_doorway_udp_state  = RESULT_NULL;//协议探测结果
extern MainDialog *g_pMainDlg;
extern char g_strDialRemoteName[50];
extern char g_strLphoneName[3][50];

void doorway_result_callback(RESULT_E result) //回调函数
{
    printf("\n=============[%s -- %d] result = %d\n", __func__, __LINE__, result);
    g_check_doorway_udp_state = result;
}
void call_back(RESULT_E result)
{
    printf("res = %d\n",result);
}

AuthorizeElevatorThread::AuthorizeElevatorThread(QObject *parent) :
    QThread(parent)
{
    m_bRun = false;
}
AuthorizeElevatorThread::~AuthorizeElevatorThread()
{

}

void AuthorizeElevatorThread::run()
{
    m_bRun = true;
    g_check_doorway_udp_state = RESULT_NULL;

    stop_lift_0x0202();
    bool ret = CheckUdpState();
    if(!ret)
    {
        return;
    }

    int time_count = 0;
    while (1)
    {
        usleep(100 * 1000);

        if (g_check_doorway_udp_state != RESULT_NULL || time_count++ >= 60)
        {
            break;
        }
    }

    stop_system_0x0501();

    if(g_check_doorway_udp_state == RESULT_SUCCESS)
    {
        UdpAuthorizeElevator();
    }
    else
    {
        TcpAuthorizeElevator();
    }

    m_bRun = false;
}

void AuthorizeElevatorThread::TcpAuthorizeElevator()
{
    QString strDialRemoteName(m_talkRemoteName);
    ((VoipMainDialog *)parent()->parent())->authorizeElevator(strDialRemoteName);
}

void AuthorizeElevatorThread::UdpAuthorizeElevator()
{
    addr_info_t    addr;
    floor_info_t floor;
    char loc_ip[20] = {'\0'};
    char loc_usr[20] = {'\0'};
    char rem_ip[20] = {'\0'};
    char rem_usr[20] = {'\0'};
    char strIP[50] = {'\0'};

    GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;
    strcpy(loc_ip, pGetPeerIPThread->m_nativeIP.toLatin1().data());
    strcpy(loc_usr, pGetPeerIPThread->m_nativeName.toLatin1().data());

    if (pGetPeerIPThread->GetPeerIP(g_strLphoneName[0], strIP) != 0)
    {
        return ;
    }
    char *ptr;
    ptr = strstr(strIP, "?");
    if (ptr)
    {
       *ptr = '\0';
    }

    strcpy(rem_ip,strIP);
    strcpy(rem_usr,g_strLphoneName[0]);

    addr.loc_ip = (uint8_t *)loc_ip;
    addr.loc_usr = (uint8_t *)loc_usr;
    addr.rem_ip = (uint8_t *)rem_ip;
    addr.rem_usr = (uint8_t *)rem_usr;

    printf(" addr.loc_ip........................%s\n", addr.loc_ip);
    printf(" addr.loc_usr........................%s\n", addr.loc_usr);
    printf(" addr.rem_ip........................%s\n", addr.rem_ip);
    printf(" addr.rem_usr........................%s\n", addr.rem_usr);

    addrtobyte((uint8_t *)m_talkRemoteName.toLocal8Bit().data(),floor.src_floor);
    addrtobyte(addr.loc_usr,floor.dest_floor);
    start_lift_0x0202(&addr,&floor,call_back);

}

bool AuthorizeElevatorThread::CheckUdpState()
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

    if (pGetPeerIPThread->GetPeerIP(g_strLphoneName[0], strIP) != 0)
    {
        return false;
    }

    char *ptr;
    int iModalType = BASIC_MODAL_TYPE;
    ptr = strstr(strIP, "?");
    if (ptr)
    {   iModalType = atoi(ptr + strlen("?"));
        *ptr = '\0';
    }
    if (!(iModalType & DOORWAY_MODAL_TYPE))
    {
        return false;
    }

    strcpy(rem_ip,strIP);
    strcpy(rem_usr,g_strLphoneName[0]);

    addr.loc_ip = (uint8_t *)loc_ip;
    addr.loc_usr = (uint8_t *)loc_usr;
    addr.rem_ip = (uint8_t *)rem_ip;
    addr.rem_usr = (uint8_t *)rem_usr;

    bool ret = start_system_0x0501(&addr, doorway_result_callback);
    printf("start_system_0x0501 ret = %d\n",ret);
    return ret;

}
