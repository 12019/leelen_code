#include "luxdomoserver.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include"maindialog.h"
#include"MyBuffer.h"
void   *AcceptThread(void *arg);
int       ListenInit(char  *ip,int  port);
void   ClientVerify(char *Frame, int size,int client_socket);
void   ParseClientData(QByteArray Frame,int client_socket);
void   CheckClientData(QByteArray pArray,int client_socket);
void   *RcvThread(void *arg);
extern MainDialog *g_pMainDlg;
extern QList<LuxdomoClient*> m_clients;
LuxdomoServer::LuxdomoServer(QObject *parent) : QObject(parent)
{
    m_socketfd=-1;
}

int   ListenInit(char  *ip,int  port)
{
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr)); //把一段内存区的内容全部设置为0
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);
    //创建用于internet的流协议(TCP)socket,用server_socket代表服务器socket
    int server_socket = socket(PF_INET,SOCK_STREAM,0);
    if( server_socket < 0)
    {
        MyMsgLog <<"the socket create failed";
        return -1;
    }
    int opt =1;
    setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    //把socket和socket地址结构联系起来
    if(bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
    {
        MyMsgLog<<"Server Bind Port : 555 Failed!";
        return -1;
    }
    //server_socket用于监听
    if(listen(server_socket,20))
    {
        MyMsgLog<<"Server Listen Failed!";
        return -1;
    }
    return server_socket;
}

void  *AcceptThread(void *arg)
{
    int server_socket =  *(int*)arg;
    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);
        //接受一个到server_socket代表的socket的一个连接
        //如果没有连接请求,就等待到有连接请求--这是accept函数的特性
        //accept函数返回一个新的socket,这个socket(new_server_socket)用于同连接到的客户的通信
        //new_server_socket代表了服务器和客户端之间的一个通信通道
        //accept函数把连接到的客户端信息填写到客户端的socket地址结构client_addr中
        int new_server_socket = accept(server_socket,(struct sockaddr*)&client_addr,&length);
        if ( new_server_socket < 0)
        {
            MyMsgLog<<"Server Accept Failed!";
        }
        else //有新的客户端进来
        {
            pthread_t tid;
            pthread_create(&tid,NULL,RcvThread,&new_server_socket);
        }
    }
    return 0;
}

void   LuxdomoServer::OpenListen(QString ip, QString  port)
{
    QByteArray  temp=ip.toLatin1();
    m_socketfd=::ListenInit(temp.data(),port.toInt());
    if(m_socketfd<0)
        return;

    pthread_create(&m_server,NULL,AcceptThread,&m_socketfd);
}

bool  LuxdomoServer::CloseListen()
{
    if(m_socketfd<0)
        return  true;

    pthread_cancel(m_server);
    ::close(m_socketfd);
    m_socketfd=-1;
    return  true;
}

LuxdomoClient*   LuxdomoServer::FindClient(int client_socket)
{
    LuxdomoClient  *client=NULL;
    QListIterator<LuxdomoClient*>  i(m_clients);
    while (i.hasNext())
    {
        client=i.next();
        if(client->m_socketfd==client_socket)
            return  client;
    }
    return  client;
}

void  ClientVerify(char *Frame, int size,int client_socket)
{
    QSettings Settings(home_control_config, QSettings::IniFormat);
    Settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
    QString  username_get=Settings.value(Ter_Account, "1111-2222").toString();
    QString  password_get=Settings.value(Ter_Pwd, "666666").toString();
    QString  netPWD;
    QString  PWD = username_get + password_get;
    for(int i=5;i<size-1;i++)
    {
        netPWD.append(Frame[i]);
    }
    MyMsgLog<<"pwd="<<netPWD;
    MyMsgLog<<"pwd="<<PWD;
    if(PWD == netPWD)
    {
        char response[6] = {0x06, 0x00, 0x00, 0x00, 0x10,0x16};
        ::send(client_socket,response,6,0);
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);
        ::getpeername(client_socket,(struct sockaddr*)&client_addr,&length);
        char *clientAddr = inet_ntoa(client_addr.sin_addr);
        LuxdomoClient  *client=new  LuxdomoClient;
        client->m_addr= QString(QLatin1String(clientAddr));
        client->m_port=client_addr.sin_port;
        client->m_socketfd=client_socket;
        MyMsgLog<<client->m_addr<<client->m_port<<client->m_socketfd;
        g_pMainDlg->m_pHomeControlDlg->m_pRcvDataThread->ObjUplowdDataToClient(NULL,NULL,NULL,"append",client);
    }
    else
    {
        char response[6] = {0x06, 0x00, 0x00, 0x00, 0x1e,0x24};
        ::send(client_socket,response,6,0);
    }
}

void  ParseClientData(QByteArray Frame,int client_socket)
{
    if(Frame.data()[1] == 0xff && Frame.data()[2] == 0xff && Frame.data()[3] == 0xff && Frame.data()[4] == 0xff)   //心跳包
    {
        char response[6] = {0x06, 0x00, 0x00, 0x00, 0x10,0x16};
        ::send(client_socket,response,6,0);
    }
    else if(Frame.data()[1] == 0x00 && Frame.data()[2] == 0x00 && Frame.data()[3] == 0x00)
    {
        ClientVerify(Frame.data(),Frame.size(),client_socket);
    }
    else
    {
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);
        ::getpeername(client_socket,(struct sockaddr*)&client_addr,&length);
        char *clientAddr = inet_ntoa(client_addr.sin_addr);
        QString  ip=QString(QLatin1String(clientAddr));
        g_pMainDlg->m_pHomeControlDlg->slotRcvFromSvr(Frame,ip,Frame.size());
    }
}

void  CheckClientData(QByteArray pArray,int client_socket)
{
    while (!pArray.isEmpty())
    {
        int size = (int)pArray.data()[0];
        if(size > pArray.size() || size < 6)
        {
            pArray.remove(0,1);
        }
        else
        {
            int sum = 0;
            for(int i = 0;i < size - 1;i++)
            {
                sum += (int)pArray.data()[i];
            }
            sum = sum&0xff;
            if(sum == (int)pArray.data()[size-1])
            {
                ParseClientData(pArray.left(size),client_socket);
                pArray.remove(0,size);
            }
            else
            {
                pArray.remove(0,1);
            }
        }
    }
}

void *RcvThread(void *arg)
{
    int Max_Buffer_Size = 2048;
    int client_socket = *(int*)arg;
    MyMsgLog<<client_socket;
    struct timeval timeout={15,0};//设置五秒为超时时间
    setsockopt(client_socket,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeout));//发送和链接五秒超时
    setsockopt(client_socket,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));//接受五秒超时

    while (1)
    {
        QByteArray buffer;
        buffer.resize(Max_Buffer_Size);
        int length = recv(client_socket,buffer.data(),Max_Buffer_Size,0);
        if (length < 0)
        {
            qDebug("%dServer Recieve Data Failed!\n",client_socket);
            LuxdomoClient  *client=g_pMainDlg->m_pHomeControlDlg->m_pServer->FindClient(client_socket);
            if(client!=NULL)
            {
                g_pMainDlg->m_pHomeControlDlg->m_pRcvDataThread->ObjUplowdDataToClient(NULL,NULL,NULL,"delete",client);
                ::close(client_socket);
                client->deleteLater();
            }
            return NULL;
        }
        else if(length == 0)
        {
            qDebug("%dServer Recieve Data zero!\n",client_socket);
            LuxdomoClient  *client=g_pMainDlg->m_pHomeControlDlg->m_pServer->FindClient(client_socket);
            if(client!=NULL)
            {
                g_pMainDlg->m_pHomeControlDlg->m_pRcvDataThread->ObjUplowdDataToClient(NULL,NULL,NULL,"delete",client);
                ::close(client_socket);
                client->deleteLater();
            }
            return NULL;
        }
        else
        {
            MyMsgLog<<buffer.mid(0,length).toHex();
            CheckClientData(buffer.mid(0,length),client_socket);
        }
    }
    return NULL;
}
