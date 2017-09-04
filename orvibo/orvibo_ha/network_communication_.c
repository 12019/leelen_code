#include"network_communication_.h"

int  client_broadcast_socekt_(unsigned char* local_ip,int local_port)
{
    int socket_fd= -1;
    struct sockaddr_in local_addr;
    int irtn=-1;
    const int broadcastOpt = 1;
    const int nSendBuf = 0;
    const int nRcvBuf = 0;

    /*创建数据报套接字*/
    socket_fd = socket(AF_INET,SOCK_DGRAM,0);
    if(socket_fd < 0) {

        printf("socket error:%d\n",errno);
        return -1;
    }

    /* 设置自己的端口和IP信息 */
    bzero(&local_addr,sizeof(struct sockaddr_in));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = inet_addr(local_ip);
    local_addr.sin_port = htons(local_port);

    /* 绑定自己的端口和IP信息到socket上 */
    if(bind(socket_fd, (struct sockaddr *) &local_addr, sizeof(struct sockaddr_in)) == -1) {

        printf("Bind error udp\n");
        close(socket_fd);
        return -1;
    }

    /*设置套接字为广播类型*/
    irtn = setsockopt(socket_fd,SOL_SOCKET,SO_BROADCAST,(char *)&broadcastOpt,sizeof(broadcastOpt));
    if(irtn < 0) {

        printf("set socket broadcast error:%d\n",errno);
        close(socket_fd);
        return -1;
    }

    /*设置套接字 发送/接收缓冲区 属性*/
    irtn = setsockopt(socket_fd,SOL_SOCKET,SO_SNDBUF,(char *)&nSendBuf,sizeof(int));
    if(irtn < 0) {

        close(socket_fd);
        printf("setsockopt SO_SNDBUF error\n");
        return -1;
    }

    irtn = setsockopt(socket_fd,SOL_SOCKET,SO_RCVBUF,(char *)&nRcvBuf,sizeof(int));
    if(irtn < 0) {

        close(socket_fd);
        printf("setsockopt SO_RCVBUF error\n");
        return -1;
    }

    return socket_fd;
}

int  client_broadcast_send_(int socket_fd,int server_port,char* data,int len)
{
    struct sockaddr_in destAddr;
    int irtn=-1;

    /*设置广播地址*/
    bzero(&destAddr,sizeof(struct sockaddr_in));
    destAddr.sin_family = AF_INET;
    destAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    destAddr.sin_port = htons(server_port);

    irtn = sendto(socket_fd,data,len,0,(struct sockaddr *)&destAddr,sizeof(destAddr));
    if(irtn < 0) {

        printf("sendto error:%d\n",errno);
        close(socket_fd);
        return -1;
    }

    return irtn;
}

int  client_broadcast_recv_(int socket_fd,int sec,int usec,struct sockaddr_in* server_ip,char* data,int len)
{
    fd_set rFdSet;
    struct timeval tv;
    int irtn=-1;
    socklen_t size=sizeof(struct sockaddr_in);

    fcntl(socket_fd, F_SETFL, O_NONBLOCK);
    FD_ZERO(&rFdSet);
    FD_SET(socket_fd,&rFdSet);

    tv.tv_sec = sec;
    tv.tv_usec = usec;
    irtn = select(socket_fd+1,&rFdSet,NULL,NULL,&tv);

    if(irtn < 0) {

        printf("select error\n");
    }
    else if(irtn == 0) {

        usleep(100000);
        printf("timeout\n");
    }
    else {

        if(FD_ISSET(socket_fd,&rFdSet)) {

            irtn = recvfrom(socket_fd,data,len,0,(struct sockaddr*)server_ip,&size);

            if(irtn <= 0) {

                printf("recvfrom error\n");
            }
            else {

                return irtn;
            }
        }
    }

    return -1;
}

int  client_tcp_socekt_(unsigned char* local_ip,int local_port,unsigned char* server_ip,int server_port)
{
    int sockfd, socklen;
    struct sockaddr_in destAddr,localAddr;
    int opt =1;

    /* 创建一个 socket 用于 tcp 通信 */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {

        printf("socket error:%d\n",errno);
        return -1;
    }

    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    /*指定网卡通信*/
    socklen = sizeof(struct sockaddr_in);
    memset(&localAddr, 0, socklen);
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(local_port);

    if(inet_pton(AF_INET, local_ip, &localAddr.sin_addr) <= 0) {

        close(sockfd);
        return -1;
    }

    if(bind(sockfd, (struct sockaddr *) &localAddr, sizeof(struct sockaddr_in)) == -1) {

        close(sockfd);
        printf("Bind error tcp\n");
        return -1;
    }

    /* 初始化服务器端（对方）的地址和端口信息 */
    memset(&destAddr, 0, socklen);
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(server_port);

    if(inet_aton(server_ip, (struct in_addr *) &destAddr.sin_addr.s_addr) == 0) {

        close(sockfd);
        return -1;
    }

    fcntl(sockfd, F_SETFL, 0);

    if(connect(sockfd, (struct sockaddr *) &destAddr, sizeof(destAddr)) == -1) {

        printf("tcp connect error %d\n", sockfd);
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int  client_tcp_send_(int socket_fd,char* data,int len)
{
    int n = 0;
    n = send(socket_fd,data,len,0);

    if(n<0) {

        return -1;
    }
    else  if(n==len) {

        return len;
    }
    else  if(len>n) {

    }

    return -1;
}

int  client_tcp_recv_(int socket_fd,int sec,int usec,char* data,int len)
{
    struct timeval timeout;
    int length=0;

    fcntl(socket_fd, F_SETFL, 0);
    timeout.tv_sec = sec;
    timeout.tv_usec = usec; //设置超时时间

    //setsockopt(socket_fd,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeout));//发送和链接30秒超时
    setsockopt(socket_fd,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));//接受30秒超时
    length=recv(socket_fd,data,len,0);

    if(length<=0) {

        return -1;
    }
    else{

        return length;
    }
}
