#include "getpeeripthread.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "maindialog.h"
#include "ui_maindialog.h"
#include "ui_calendar.h"
#include "exosip_main.h"
#include "exosip_message.h"
#include "command.h"
#include "application.h"
#include "voip/addresslist.h"


/************************************************************
  该文件为根据网络名查询IP地址线程主程序
************************************************************/

extern LinphoneCore *lphone;
extern eXosip_element_t *eXosipElement;
extern eXosip_t eXosip;

extern MainDialog *g_pMainDlg;

extern int g_iRcvACKBUSY;

extern UserAddrInfo g_userAddr[USERADDRMAX];
extern char g_strNativeIP[50];
extern int g_iValidAddrCnt;
extern QString SoftVersion;
extern QString HardVersion;

/************************************************************
描述： 根据硬件版本信息填写设备类型字段
参数： 硬件版本信息，设备类型字段
返回：
************************************************************/
static void Fill_DevInfo_From_HardVersion(const QString& hv,QString& dev)
{
	if 	( hv == "1201" || hv == "1202" || hv == "1203" )
		dev = "FJ75";
	else if ( hv == "1301" || hv == "1302" )
		dev = "FJ70";
	else
		dev = "FJ70";
}
/************************************************************
描述：根据网络名查询IP地址线程构造函数
参数：parent - 该线程的父对象指针
返回：无
************************************************************/
GetPeerIPThread::GetPeerIPThread(QObject *parent) :
    QThread(parent)
{
    m_iModalType = BASIC_MODAL_TYPE;
}

GetPeerIPThread::~GetPeerIPThread()
{

}

/************************************************************
描述：终端求助发送线程主函数，该主函数在224.0.0.1端口监听局域网中的组
	 播信息，如果接收到的其它终端传来的网络名信息和本机网络名一致，则
	 向那个终端回发本机IP地址和设备型号
参数：无
返回：无
************************************************************/
void GetPeerIPThread::run()
{
    struct sockaddr_in peeraddr;
    struct in_addr ia;
    int sockfd;
    char recmsg[200];
    int n;
    socklen_t socklen;
    struct hostent *group;
    struct ip_mreq mreq;
    char group_addr[16] = "224.0.0.1";
    int port = 6789;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
    {
        printf("socket creating err in udptalk\n");
        return;
    }

    bzero(&mreq, sizeof(struct ip_mreq));

    if ((group = gethostbyname(group_addr)) == (struct hostent *) 0)
    {
        perror("gethostbyname");
        return;
    }

    bcopy((void *) group->h_addr, (void *) &ia, group->h_length);
    bcopy(&ia, &mreq.imr_multiaddr.s_addr, sizeof(struct in_addr));

    //mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (inet_pton(AF_INET, g_strNativeIP, &mreq.imr_interface) <= 0)
    {
        printf("Wrong local IP address!\n");
        return;
    }

    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(struct ip_mreq)) == -1)
    {
        perror("setsockopt 2");
        return;
    }

    socklen = sizeof(struct sockaddr_in);
    memset(&peeraddr, 0, socklen);
    peeraddr.sin_family = AF_INET;

    peeraddr.sin_port = htons(port);
    if (inet_pton(AF_INET, group_addr, &peeraddr.sin_addr) <= 0)
    {
        printf("Wrong dest IP address!\n");
        return;
    }

    if (bind(sockfd, (struct sockaddr *) &peeraddr, sizeof(struct sockaddr_in)) == -1)
    {
        printf("Bind error\n");
        return;
    }

    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    fd_set rd;
    struct timeval tv;

    m_bRun = true;
    while (m_bRun)
    {
        tv.tv_sec = 0;
        tv.tv_usec = 200000;

        FD_ZERO(&rd);
        FD_SET(sockfd, &rd);
        if (select(sockfd + 1, &rd, NULL, NULL, &tv) <= 0)
        {
            continue;
        }

        n = recvfrom(sockfd, recmsg, sizeof(recmsg), 0, (struct sockaddr *) &peeraddr, &socklen);

        if (n > 0)
        {

            recmsg[n] = 0;

            if (recmsg[0] == '#')
            {
                QTextCodec *gk_codec = QTextCodec::codecForName("GBK");
                g_pMainDlg->ui->widgetCalendar->ui->labelJieriJieqi->setText(gk_codec->toUnicode(recmsg + 1));
            }
            else if (m_nativeName != "" /*&& eXosipElement->state != ICT_CALLING*/)
                //david 2014.4.12 why add the condition
            {
                if ((m_nativeName == recmsg) ||
                        ((m_nativeName.at(9) == '-')&& (m_nativeName.left(9) == recmsg)))
                {
                    printf("eXosip state:%d\n",eXosipElement->state);

		    QString dev;
		    Fill_DevInfo_From_HardVersion(HardVersion,dev);     // 获得设备型号名称
                    sprintf(recmsg, "%s?%d*A20L-%s-%s-%s", m_nativeIP.toLatin1().data(),g_pMainDlg->m_iCurModalType,dev.toLatin1().data(),HardVersion.toLatin1().data(),SoftVersion.toLatin1().data());
                    printf("local machine:%s\n", recmsg);
                    sendto(sockfd, recmsg, strlen(recmsg), 0, (struct sockaddr *)&peeraddr, socklen);
                }
            }
        }

        //usleep(100000);
    }

    close(sockfd);
}

/************************************************************
描述：在224.0.0.1地址上发送网络名组播信息，然后等待接收其它终端回复的
	 IP信息，
参数：strPeerName - 需查询IP地址的网络名
	 strPeerIP - 返回的IP地址和设备类型
返回：0 - 成功
	 -1 - 失败
************************************************************/
int GetPeerIPThread::GetPeerIP(char* strPeerName, char *strPeerIP)
{
    int ret = -1;

    if(g_iValidAddrCnt > 1)
    {
        ret = getIPFromAddrList(strPeerName,strPeerIP);

        return ret;
    }

	struct sockaddr_in peeraddr, myaddr;

	int sockfd;

	char recmsg[200];

	char strPeerIp1[200];

	char strPeerName2[80];

	char strPeerIp2[200];

	unsigned int socklen;

	int port = 6789;

	char group_addr[16] = "224.0.0.1";

	int i;

	if (strPeerName[0] == '\0') return -1;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
	{
		printf("socket creating error\n");
		return -1;
	}
	socklen = sizeof(struct sockaddr_in);

	memset(&myaddr, 0, socklen);

    myaddr.sin_family = AF_INET;

    //myaddr.sin_addr.s_addr = INADDR_ANY;
    if (inet_pton(AF_INET, g_strNativeIP, &myaddr.sin_addr) <= 0)
    {
        printf("Wrong loacl IP address!\n");
        return -1;
    }

	if (bind(sockfd, (struct sockaddr *) &myaddr, sizeof(struct sockaddr_in)) == -1)
	{
		printf("Bind error\n");
		close(sockfd);
		return -1;
	}

	strcpy(recmsg, strPeerName);

	int iModalType = BASIC_MODAL_TYPE;

	for (i = 0; i < 2; i++)
	{
		memset(&peeraddr, 0, socklen);

		peeraddr.sin_family = AF_INET;

		peeraddr.sin_port = htons(port);

		if (inet_pton(AF_INET, group_addr, &peeraddr.sin_addr) <= 0)
		{
			printf("wrong group address!\n");
			close(sockfd);
			return -1;
		}

		if (sendto(sockfd, recmsg, strlen(recmsg), 0,
			(struct sockaddr *) &peeraddr,
			sizeof(struct sockaddr_in)) < 0)
		{
			printf("sendto error!\n");
			close(sockfd);
			return -1;
		}

//		fcntl(sockfd, F_SETFL, O_NONBLOCK);

		fd_set rd;
		struct timeval tv;

//		tv.tv_sec = 0;
//		tv.tv_usec = 1000000;

		tv.tv_sec = 2;
		tv.tv_usec = 0;

		FD_ZERO(&rd);
		FD_SET(sockfd, &rd);
		recmsg[0] = '\0';
		if (select(sockfd + 1, &rd, NULL, NULL, &tv) > 0)
		{
			int n;

            n = recvfrom(sockfd, recmsg, sizeof(recmsg), 0, (struct sockaddr *) &peeraddr, &socklen);

            if (n > 0)
			{
				recmsg[n] = 0;

				printf("%s\n", recmsg);

				ret = 0;
			}
		}

		char *ptr;

		ptr = strstr(recmsg, "?");
		if (ptr)
		{
			iModalType = atoi(ptr + strlen("?"));
			*ptr = '\0';
		}

		if (i == 0)
		{
			ptr = strstr(recmsg, "->");
			if (!ptr)
			{
				strcpy(strPeerIp1, recmsg);
				break;
			}

			*ptr = '\0';
			ptr += strlen("->");

			strcpy(strPeerIp1, recmsg);
			strcpy(strPeerName2, ptr);
			strcpy(recmsg, strPeerName2);
		}
		else
		{
			ptr = strstr(recmsg, "->");
			if (ptr) *ptr = 0;
			strcpy(strPeerIp2, recmsg);
		}
	}

	close(sockfd);

	if (i == 0)
	{
		sprintf(strPeerIP, "%s?%d", strPeerIp1, iModalType);
	}
	else
	{
		sprintf(strPeerIP, "%s->%s:%s?%d", strPeerIp1, strPeerName2, strPeerIp2, iModalType);
	}

	printf("strPeerIP:%s\n", strPeerIP);

	return ret;
}

int GetPeerIPThread::CallForPeerName(QString strPeerName)
{
    if(g_iValidAddrCnt > 1)
    {
        int ret;

        ret = CallByAddrList(strPeerName);

        return ret;
    }

    struct sockaddr_in peeraddr, myaddr;

    int sockfd;

    char recmsg[100];

    strcpy(recmsg, strPeerName.toLocal8Bit().data());

    if (m_nativeName == strPeerName) return -1;

    unsigned int socklen;

    int port = 6789;

    int getDevice = 0;

    char group_addr[16] = "224.0.0.1";

    if (strPeerName == "") return -1;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
    {
        printf("socket creating error\n");
        return -1;
    }
    socklen = sizeof(struct sockaddr_in);

    memset(&myaddr, 0, socklen);

    myaddr.sin_family = AF_INET;

    //myaddr.sin_addr.s_addr = INADDR_ANY;
    if (inet_pton(AF_INET, g_strNativeIP, &myaddr.sin_addr) <= 0)
    {
        printf("Wrong loacl IP address!\n");
        return -1;
    }

    if (bind(sockfd, (struct sockaddr *) &myaddr, sizeof(struct sockaddr_in)) == -1)
    {
        printf("Bind error\n");
        close(sockfd);
        return -1;
    }

    m_iModalType = BASIC_MODAL_TYPE;

    memset(&peeraddr, 0, socklen);

    peeraddr.sin_family = AF_INET;

    peeraddr.sin_port = htons(port);

    if (inet_pton(AF_INET, group_addr, &peeraddr.sin_addr) <= 0)
    {
        printf("wrong group address!\n");
        close(sockfd);
        return -1;
    }

    if (sendto(sockfd, recmsg, strlen(recmsg), 0,
               (struct sockaddr *) &peeraddr,
               sizeof(struct sockaddr_in)) < 0)
    {
        printf("sendto error!\n");
        close(sockfd);
        return -1;
    }

    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    int cnt = 0;

    while (1)
    {
        fd_set rd;

        struct timeval tv;

        tv.tv_sec = 0;

        tv.tv_usec = 200000;

        FD_ZERO(&rd);

        FD_SET(sockfd, &rd);

        recmsg[0] = '\0';

        cnt++;

        if (cnt > 9)
        {
            close(sockfd);

            if(eXosipElement->state == IDLE &&  !getDevice)
            {
                return -1;
            }
            else
            {
                return 0;
            }
        }

        qApp->processEvents();

        if (select(sockfd + 1, &rd, NULL, NULL, &tv) > 0)
        {

            int n;
            n = recvfrom(sockfd, recmsg, sizeof(recmsg), 0, (struct sockaddr *) &peeraddr, &socklen);

            cnt = 0;

            if(n > 0)
            {
                recmsg[n] = 0;
            }

            char *ptr;

            if(strstr(recmsg,","))
            {
                char *ptr1;

                int bFound = 0;

                ptr1 = recmsg;

                while(1)
                {
                    ptr = strstr(ptr1,",");

                    if(ptr)
                    {
                        *ptr = '\0';

                        lpc_cmd_call(lphone,ptr1);

                        usleep(300000);

                        setGM7113Channel(0);

                        ptr1 = ptr + 1;

                        bFound = 1;

                        getDevice = 1;
                    }
                    else
                    {
                        if(!bFound) break;
                        else
                        {
                            ptr = strstr(ptr1,"?");

                            *ptr = '\0';

                            lpc_cmd_call(lphone,ptr1);

                            usleep(300000);

                            setGM7113Channel(0);

                            close(sockfd);

                            getDevice = 1;

                            return 0;
                        }
                    }
                }
            }

            ptr = strstr(recmsg, "?");
            if (ptr)
            {
                m_iModalType = atoi(ptr + strlen("?"));
                *ptr = '\0';
            }

            ptr = strstr(recmsg, "->");

            if (!ptr)
            {

                if(m_nativeIP != QString(recmsg))
                {
                    lpc_cmd_call(lphone, recmsg);

                    usleep(300000);

                    setGM7113Channel(0);

                    getDevice = 1;

                    continue;
                }

            }

            usleep(100000);
        }

    }

    close(sockfd);

    return 0;
}

int GetPeerIPThread::CallByAddrList(QString strPeerName)
{
    if(g_iValidAddrCnt < 2) return -1;

    int ret;

    char IPBUF[16];

    if (m_nativeName == strPeerName) return -1;

    ret = recurbinary((char *)qPrintable(strPeerName),0,USERADDRMAX);

    if(ret == -1)
    {
        return -1;
    }
    else
    {
        memcpy(IPBUF,g_userAddr[ret].userIP,15);

        IPBUF[15] = '\0';

        lpc_cmd_call(lphone,IPBUF);

        usleep(200000);

        qApp->processEvents();

        setGM7113Channel(0);

        if((g_userAddr[ret].type) != BASIC_MODAL_TYPE)
        {
            return 0;
        }

        for(int i = 1;i < 9;i++)
        {
            if((i >= g_iValidAddrCnt) || (i < 0) ) continue;

            if(!strncmp(qPrintable(strPeerName),g_userAddr[ret + i].userName,
                       strPeerName.length()))
            {
                memcpy(IPBUF,g_userAddr[ret + i].userIP,15);

                IPBUF[15] = '\0';

                lpc_cmd_call(lphone,IPBUF);

                usleep(200000);

                setGM7113Channel(0);
            }

            if(!strncmp(qPrintable(strPeerName),g_userAddr[ret - i].userName,
                       strPeerName.length()))
            {

                memcpy(IPBUF,g_userAddr[ret - i].userIP,15);

                IPBUF[15] = '\0';

                lpc_cmd_call(lphone,IPBUF);

                usleep(200000);

                setGM7113Channel(0);

            }
        }
    }
    return 0;
}


int GetPeerIPThread::getIPFromAddrList(QString strPeerName,char *strPeerIP)
{
    int ret;

    ret = recurbinary((char *)qPrintable(strPeerName),0,USERADDRMAX);

    if(ret == -1)
    {
        return -1;
    }
    else
    {
        char IPBUF[16];

        memcpy(IPBUF,g_userAddr[ret].userIP,15);

        IPBUF[15] = '\0';

        sprintf(strPeerIP, "%s?%d", IPBUF,g_userAddr[ret].type);

        return 0;
    }
}
