#include "luxdomomail.h"
#include"MyBuffer.h"
char base64_table[64]=
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T','U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't',  'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};

LuxDomoMail::LuxDomoMail(QObject *parent) : QObject(parent)
{

}

int   LuxDomoMail::getRcvFromHost()
{
    int ret; //判断符号
    char buffer[1024] = {0};
    memset(buffer,0,sizeof(buffer));
    ret = recv(sockfd,buffer,1024,0);
    if(-1 == ret)  //接收超时或者失败
    {
        MyMsgLog<<"receive nothing\n";
        return -1;
    }
    buffer[ret]='\0';
    if(*buffer == '5')
    {
        MyMsgLog<<"the order is not support smtp host";
        qDebug("%s",buffer);
        return -1;
    }
    MyMsgLog<<"recieved from mailhost buffer:"<<buffer;
    return 0;
}

//base64加密算法
int    LuxDomoMail::base64_encode(char *pBase64, int nLen, char *pOutBuf, int nBufSize)
{
    int i = 0;
    int j = 0;
    int nOutStrLen = 0;

    /* nOutStrLen does not contain null terminator. */
    nOutStrLen = nLen / 3 * 4 + (0 == (nLen % 3) ? 0 : 4);
    if ( pOutBuf && nOutStrLen < nBufSize )
    {
        char cTmp = 0;
        for ( i = 0, j = 0; i < nLen; i += 3, j += 4 )
        {
            /* the first character: from the first byte. */
            pOutBuf[j] = base64_table[pBase64[i] >> 2];

            /* the second character: from the first & second byte. */
            cTmp = (char)((pBase64[i] & 0x3) << 4);
            if ( i + 1 < nLen )
            {
                cTmp |= ((pBase64[i + 1] & 0xf0) >> 4);
            }
            pOutBuf[j+1] = base64_table[(int)cTmp];

            /* the third character: from the second & third byte. */
            cTmp = '=';
            if ( i + 1 < nLen )
            {
                cTmp = (char)((pBase64[i + 1] & 0xf) << 2);
                if ( i + 2 < nLen )
                {
                    cTmp |= (pBase64[i + 2] >> 6);
                }
                cTmp = base64_table[(int)cTmp];
            }
            pOutBuf[j + 2] = cTmp;

            /* the fourth character: from the third byte. */
            cTmp = '=';
            if ( i + 2 < nLen )
            {
                cTmp = base64_table[pBase64[i + 2] & 0x3f];
            }
            pOutBuf[j + 3] = cTmp;
        }
        pOutBuf[j] = '\0';
    }
    return nOutStrLen + 1;
}

//连接服务器,smtpaddr为服务器地址或者域名
int    LuxDomoMail::connectHost(const char *smtpaddr)
{
    struct hostent *host = NULL;
    host = gethostbyname(smtpaddr);//解析IP地址
    if(NULL == host)
    {
        return -1;
    }
    printf("name: %s\naddrtype; %d\naddrlength: %d\n",
                host->h_name, host->h_addrtype, host->h_length);
    printf("ip address: %s\n",
        inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));
    MyMsgLog<<"sfsdfsd";
    char m_SmtpIp[100] = {0};
    sprintf(m_SmtpIp,"%s",inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));
    printf("转换后的IP为:%s\n",m_SmtpIp);
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0)
    {
        printf("Create socket error!\n");
        return -1;
    }
    struct timeval timeout={5,0};//设置五秒为超时时间
    setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeout));//发送五秒超时
    setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));//接受五秒超时
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(25);
    if(inet_pton(AF_INET, m_SmtpIp, &servaddr.sin_addr) <= 0 )
    {
        printf("inet_pton error!\n");
        return -1;
    }
    if (::connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)   //五秒的连接时间
    {
        printf("Connect failed... \n");
        return -1;
    }
    printf("Connect to %s successful\n",m_SmtpIp);
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    if(recv(sockfd, buffer, sizeof(buffer), 0) < 0)  //连接邮件服务器后等待确认信息
    {
        printf("receive failed... \n");
        return -1;
    }
    printf("the code rcv from smtp is:%s\n", buffer);
    return sockfd;
}

//登陆服务器，账号和密码验证
int  LuxDomoMail::loginToHost(char *user, char *pwd)
{
    if(user == "" || pwd == "")
    {
        return -1;
    }
    int ret;   //判断位
    char *send_data;  //要发送的数据
    send_data = "HELO 163.com\r\n";  //smtp第一步:HELO是客户向对方邮件服务器发出的标识自己的身份的命令
    ret = send(sockfd, send_data, strlen(send_data), 0);
    if(-1 == ret)
    {
        return -1;
    }
    if(getRcvFromHost() < 0)//从服务器中获取信息,理论回复"250 OK",
    {
        return -1;  //错误
    }
    send_data = "AUTH LOGIN\r\n";  //smtp第二步:请求服务器登陆命令
    ret = send(sockfd, send_data, strlen(send_data), 0);
    if(-1 == ret)
    {
        return -1;
    }
    if(getRcvFromHost() < 0) //等待服务器回复，理论值回复"334 dXNlcm5hbWU6"
    {
        return -1;
    }

    //接下来开始传账号和密码，smtp格式要求账号和密码以base64的编码方式
    char m_username[1500] = {0};  //账号名称
    char m_passwd[1500] = {0}; //密码
    char out[1500] = {0}; //加密后的
    sprintf(m_username,"%s","13338457944");  //要登陆的账号，只需要@之前的字符
    sprintf(m_passwd,"%s","wmwwgchzupxunnbl");  //要登陆的密码
    base64_encode(m_username,strlen(m_username),out,1500);
    memset(m_username,0,1500);
    sprintf(m_username,"%s\r\n",out);//以回车结束的base64用户名，标准格式

    memset(out,0,1500);
    base64_encode(m_passwd,strlen(m_passwd),out,1500);
    memset(m_passwd,0,1500);
    sprintf(m_passwd,"%s\r\n",out);//以回车结束的base64密码，标准格式

    //用户名验证
    ret = send(sockfd, (char *)m_username, strlen(m_username),0); //先发送加密的用户名过去
    if(-1 == ret)
    {
        return -1;
    }
    if(getRcvFromHost() < 0)  //用户名发送之后则等待服务器的回复，理论值250
    {
        return -1;
    }

    //密码验证
    ret = send(sockfd,(char *)m_passwd,strlen(m_passwd),0);  //再发送加密的密码过去验证
    if(-1 == ret)
    {
        return -1;
    }
    if(getRcvFromHost() < 0)
    {
        return -1;
    }
    //以上两个步骤通过之后则证明已经登陆成功，后面可直接发送邮件
    return 0;
}

int  LuxDomoMail::sendEmail(char *from, char *to, char *subject,const char *context)
{
    if(from =="" ||to == ""||subject == ""||context == "")
    {
        return -1;
    }
    int ret;
    char m_from[128] = {0};
    char m_to[128] = {0};

    sprintf(m_from,"MAIL FROM: <%s>\r\n",from);   //smtp的协议标准
    ret = send(sockfd, m_from, strlen(m_from), 0);
    if(-1 == ret)
    {
        return -1;
    }
    if(getRcvFromHost() < 0)
    {
        return -1;
    }

    sprintf(m_to, "RCPT TO: <%s>\r\n", to);  //目的地址，标准协议
    ret = send(sockfd,m_to, strlen(m_to),0);
    if(-1 == ret)
    {
        return -1;
    }
    if(getRcvFromHost() < 0)
    {
        return -1;
    }

    //开始准备数据
    char *send_data = "DATA\r\n";
    ret = send(sockfd,send_data,strlen(send_data),0);
    if(-1 == ret)
    {
        return -1;
    }
    if(getRcvFromHost() < 0)
    {
        return -1;
    }

    //开始发送数据内容
    char Subject[512] = {0};   //标题
    char Context[6000] = {0}; //内容
    sprintf(Subject,"Subject: %s",subject);
    strcat(Context,Subject);
    strcat(Context,"\r\n\r\n");
    strcat(Context,context);
    strcat(Context,"\r\n.\r\n");  //内容的结束符号"\r\n.\r\n"
    ret = send(sockfd, Context, strlen(Context), 0);
    if(-1 == ret)
    {
        return -1;
    }
    if(getRcvFromHost() < 0)
    {
        return -1;
    }

    //发送退出，整封邮件的结束
    ret = send(sockfd,"QUIT\r\n",strlen("QUIT\r\n"), 0);
    if(-1 == ret)
    {
        return -1;
    }
    if(getRcvFromHost() < 0)
    {
        return -1;
    }
    qDebug("Email send successful\n");
    return 0;
}

//直接发送邮件内容,返回0表示成功，返回-1表示发送失败
int  LuxDomoMail::pSmtpMailContext(const char *context)
{
    int ret = -1;
    ret = connectHost("220.181.12.11");
    if(-1 == ret)
    {
        close(sockfd);
        return -1;
    }
    ret = loginToHost("13338457944","wmwwgchzupxunnbl");
    if(-1 == ret)
    {
        close(sockfd);
        return -1;
    }
    ret = sendEmail("13338457944@163.com","693253221@qq.com","LuxDomo终端机反馈",context);
    if(-1 == ret)
    {
        close(sockfd);
        return -1;
    }
    close(sockfd);
    return 0;
}
