#include "datetimesetthread.h"
#include "core/leelen_comdef.h"
#include "datetimesetdialog.h"


extern RESULT_E g_check_managemachine_udp_state ;
extern RESULT_E g_check_computercenter_udp_state ;

DateTimeSetThread::DateTimeSetThread(QObject *parent) :
    QThread(parent)
{
    m_bRun = false;
    m_TcpSetCmmputerTimeResult = TIME_RESULT_INIT;
    m_autoUpdateTimeFlag = false;
}

DateTimeSetThread::~DateTimeSetThread()
{

}
void DateTimeSetThread::run()
{
    m_bRun = true;
    m_TcpSetCmmputerTimeResult = TIME_RESULT_INIT;
    g_check_managemachine_udp_state = RESULT_NULL;
    g_check_computercenter_udp_state = RESULT_NULL;

    ((DatetimeSetDialog *)parent())->CheckUdpState();

    if (g_check_computercenter_udp_state == RESULT_SUCCESS)
    {
        int res = ((DatetimeSetDialog *)parent())->SetTimeByUdp(TO_COMPUTER_CENTER);       
        if(res == 0)
        {
             m_bRun = false;
            return ;
        }
    }
    else
    {
        ((DatetimeSetDialog *)parent())->emitSigTcpSetTime(TO_COMPUTER_CENTER);
        while(1)
        {
            if(m_TcpSetCmmputerTimeResult == TIME_RESULT_INIT)
            {
                usleep(50*1000);
            }
            else if(m_TcpSetCmmputerTimeResult == TIME_RESULT_FALSE)
            {
                break ;
            }
            else
            {
                m_bRun = false;
                return ;
            }
        }
    }

    if (g_check_managemachine_udp_state == RESULT_SUCCESS)
    {
        ((DatetimeSetDialog *)parent())->SetTimeByUdp(TO_MANAGER_MACHINE);

    }
    else
    {
        ((DatetimeSetDialog *)parent())->emitSigTcpSetTime(TO_MANAGER_MACHINE);    
    }

     m_bRun = false;

}

