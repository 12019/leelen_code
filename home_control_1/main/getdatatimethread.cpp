#include "getdatatimethread.h"

GetDataTimeThread::GetDataTimeThread(QObject *parent) :
    QObject(parent)
{

}
GetDataTimeThread::~GetDataTimeThread()
{

}

void GetDataTimeThread::run()
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
    int port = 6788;

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

    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

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

		g_pMainDlg->emitSigResetTimeVaraiable();		// 网络授时结束要变量重设置
            }
            else if (m_nativeName != "" /*&& eXosipElement->state != ICT_CALLING*/)
                //david 2014.4.12 why add the condition
            {
                if ((m_nativeName == recmsg) ||
                        ((m_nativeName.at(9) == '-')&& (m_nativeName.left(9) == recmsg)))
                {
                    printf("eXosip state:%d\n",eXosipElement->state);
                    sprintf(recmsg, "%s?%d", m_nativeIP.toLatin1().data(), g_pMainDlg->m_iCurModalType);
                    printf("local machine:%s\n", recmsg);
                    sendto(sockfd, recmsg, strlen(recmsg), 0, (struct sockaddr *)&peeraddr, socklen);
                }

            }
        }

        //usleep(100000);
    }

    close(sockfd);
}
