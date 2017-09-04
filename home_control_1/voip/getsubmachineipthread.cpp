#include "getsubmachineipthread.h"
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

GetSubMachineIPThread::GetSubMachineIPThread(QObject *parent) :
    QThread(parent)
{
	GetPeerIPThread *m_pGetPeerIPThread = (GetPeerIPThread *)parent;

	m_bRun = false;
	m_bAvoid = false;
	m_bBusy = false;

	memset(m_pGetPeerIPThread->m_strSubMachineIPs, 0, sizeof(m_pGetPeerIPThread->m_strSubMachineIPs));
}

void GetSubMachineIPThread::run()
{
	struct sockaddr_in peeraddr, myaddr;
	int sockfd;
	char recmsg[200];
	char strPeerName[80];
	unsigned int socklen;
	int port = 6789;
	char group_addr[16] = "224.0.0.1";
	int i;
	int ret = -1;
	QString str;
	int len;
	int local_idx;
	char strSubMachineIPs[200];

	GetPeerIPThread *m_pGetPeerIPThread = (GetPeerIPThread *)parent();

	m_bRun = true;

	while (m_bRun && ((m_pGetPeerIPThread->m_nativeName == "") || (m_pGetPeerIPThread->m_nativeIP == "")))
	{
		usleep(200000);
	}

	//return;

	while (m_bRun)
	{
		if (m_bAvoid)
		{
			goto jump;
		}

		local_idx = -1;
		str = m_pGetPeerIPThread->m_nativeName;
		len = str.length();
		strcpy(strPeerName, str.toLocal8Bit().data());
		if (str.at(len - 2) == '-')
		{
			QChar ch = str.at(len - 1);
			if (ch.isDigit())
			{
				strPeerName[len - 2] = '\0';
				local_idx = ch.toAscii() - '0';
			}
		}
		else
		{
			goto jump;
		}

		m_bBusy = true;

		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		if (sockfd < 0)
		{
			printf("socket creating error\n");
			goto jump;
		}
		socklen = sizeof(struct sockaddr_in);

		memset(&myaddr, 0, socklen);
		myaddr.sin_family = AF_INET;

		myaddr.sin_addr.s_addr = INADDR_ANY;

		if (bind(sockfd, (struct sockaddr *) &myaddr, sizeof(struct sockaddr_in)) == -1)
		{
			printf("Bind error\n");
			close(sockfd);
			goto jump;
		}

		len = 0;

		for (i = 0; i < 8; i++)
		{
			int num = i + 1;
			ret = -1;
			if (num == local_idx)
			{
				strcpy(recmsg, m_pGetPeerIPThread->m_nativeIP.toLocal8Bit().data());
				ret = 0;
			}
			else
			{
				sprintf(recmsg, "%s-%d", strPeerName, num);

				//printf("%d:%s\n", i, recmsg);
				memset(&peeraddr, 0, socklen);
				peeraddr.sin_family = AF_INET;
				peeraddr.sin_port = htons(port);

				if (inet_pton(AF_INET, group_addr, &peeraddr.sin_addr) <= 0)
				{
					printf("wrong group address!\n");
					close(sockfd);
					goto jump;
				}

				if (sendto(sockfd, recmsg, strlen(recmsg), 0,
					(struct sockaddr *) &peeraddr,
					sizeof(struct sockaddr_in)) < 0)
				{
					printf("sendto error!\n");
					close(sockfd);
					goto jump;
				}

				fcntl(sockfd, F_SETFL, O_NONBLOCK);

				fd_set rd;
				struct timeval tv;
				int j;
				for (j = 0; j < 4; j++)
				{
					if (m_bAvoid || (!m_bRun)) break;

					tv.tv_sec = 0;
					tv.tv_usec = 100000;

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
							//strcpy(strPeerIP, recmsg);
							//printf("%s\n", recmsg);

							ret = 0;

							break;
						}

					}
				}

				if (m_bAvoid || (!m_bRun)) break;
			}

			if (ret == 0)
			{
				char *ptr;
				ptr = strstr(recmsg, "?");
				if (ptr)
				{
					//iModalType = atoi(ptr + strlen("?"));
					*ptr = '\0';
				}

				if (len == 0)
				{
					sprintf(strSubMachineIPs + len, "%s", recmsg);
					len += strlen(recmsg);
				}
				else
				{
					sprintf(strSubMachineIPs + len, ",%s", recmsg);
					len += strlen(recmsg) + 1;
				}
			}
		}

		close(sockfd);

		if ((!m_bAvoid) && m_bRun)
		{
			strcpy(m_pGetPeerIPThread->m_strSubMachineIPs, strSubMachineIPs);
			printf("strSubMachineIPs:%s\n", strSubMachineIPs);
		}

		m_bBusy = false;

jump:
		for (i = 0; i < 15000000/1000000; i++)
		{
			if (!m_bRun) break;
			usleep(1000000);
		}
	}
}
