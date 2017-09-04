#include "uploaddeviceinfothread.h"
#include "getpeeripthread.h"
#include "core/leelen_comdef.h"
#include "leelen_protocol.h"
#include "leelen_system.h"
#include "leelen_security.h"
#include "maindialog.h"

extern RESULT_E g_check_computercenter_udp_state ;//协议探测结果

UploadDeviceInfoThread::UploadDeviceInfoThread(QObject *parent) :
    QThread(parent)
{
    m_bRun = false;
    m_infotype = -1;
}
UploadDeviceInfoThread::~UploadDeviceInfoThread()
{

}

void UploadDeviceInfoThread::run()
{
    m_bRun = true;
    g_check_computercenter_udp_state = RESULT_NULL;

    ((SecurityMainDialog *)parent())->CheckUdpState();

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

    if (g_check_computercenter_udp_state == RESULT_SUCCESS)
    {
        ((SecurityMainDialog *)parent())->SendDeviceInfoByUdp(m_infotype);
    }
    else
    {
        ((SecurityMainDialog *)parent())->SendDeviceInfoByTcp(m_infotype);
    }

    m_infotype = -1;
    m_bRun = false;
}

