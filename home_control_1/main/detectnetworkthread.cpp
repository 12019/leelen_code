#include "detectnetworkthread.h"

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <QStandardItemModel>
#include "maindialog.h"
#include "ui_maindialog.h"

extern MainDialog *g_pMainDlg;

bool g_network_mask;			// 该标志设置为true时候 不进行网络状态改变

DetectNetworkThread::DetectNetworkThread(QObject *parent) :
    QThread(parent)
{

}

int detectnet(char* net_name)
{
    int skfd = 0;
    struct ifreq ifr;
    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(skfd < 0)
    {
        printf("socket create failed!\n");
        return -1;
    }

    strcpy(ifr.ifr_name, net_name);
    if(ioctl(skfd, SIOCGIFFLAGS, &ifr) <0 )
    {
        printf("SIOCGIFFLAGS failed: %s/n", strerror(errno));
        close(skfd);
        return -1;
    }

    if(ifr.ifr_ifru.ifru_flags & IFF_RUNNING)
    {
        //printf("%s is running :)\n", ifr.ifr_name);
        close(skfd);
        return 0;
    }
    else
    {
        //printf("%s is not running :(\n", ifr.ifr_name);
        close(skfd);
        return -1;
    }
}

void DetectNetworkThread::run()
{
    int nownetwork = 0;
    m_bRun = true;

    oldnetwork = detectnet("eth0");

    g_pMainDlg->emitSigSetNetWork(oldnetwork);

    g_network_mask = false;

    while (m_bRun)
    {
        usleep(1000000);

	if ( g_network_mask == false )
	{
		nownetwork = detectnet("eth0");

		if (oldnetwork != nownetwork)
		{
			g_pMainDlg->emitSigSetNetWork(nownetwork);
			oldnetwork = nownetwork;
		}
	}
    }
}
