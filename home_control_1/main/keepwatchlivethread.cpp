#include "keepwatchlivethread.h"
#include <linux/watchdog.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>

extern int g_vFlagmessage;

extern int g_mainMonitorFlag;

KeepWatchLiveThread::KeepWatchLiveThread(QObject *parent) :
    QThread(parent)
{
    m_watchdog_fd = -1;
}

KeepWatchLiveThread::~KeepWatchLiveThread()
{

}

void KeepWatchLiveThread::run()
{
//    int timeout = 30;
    int timeout = 15;

    m_watchdog_fd = ::open("/dev/watchdog", O_WRONLY);

    if (m_watchdog_fd >= 0)
    {
        printf("Watchdog device enabled\n");
        ioctl(m_watchdog_fd, WDIOC_SETTIMEOUT, &timeout);
    }

//    int cnt=0;
    while(1)
    {
	sleep(7);
//        if(cnt<10){
            if(g_vFlagmessage&&g_mainMonitorFlag){

                if (m_watchdog_fd >= 0){
                    ioctl(m_watchdog_fd, WDIOC_KEEPALIVE, 0);
                }

                g_vFlagmessage = 0;

                g_mainMonitorFlag = 0;
            }
//        }
//	else if(cnt == 13){
//
//            int val = WDIOS_DISABLECARD;
//
//            ioctl(m_watchdog_fd,WDIOC_SETOPTIONS,&val);
//
//        }else if(cnt == 30){
//
//            int val = WDIOS_ENABLECARD;
//
//            ioctl(m_watchdog_fd, WDIOC_SETOPTIONS, &val);
//
//        }
//	else if(cnt==32){
//
//            cnt=0;

//        }

//        cnt++;
//        usleep(1000000);
    }

    ::close(m_watchdog_fd);
}
