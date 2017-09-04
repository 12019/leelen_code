#include "linrcvsever.h"
#include"homecontrolmaindlg.h"
#include"MyBuffer.h"
#include"data_types.h"
#include"syssetengineerdialog.h"
#include"maindialog.h"
#include<QLinkedList>

extern MainDialog *g_pMainDlg;

/***********************************************************************************
函数名 : LinRcvSever
描  述 : 服务器监听客户端
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
LinRcvSever::LinRcvSever(QObject *parent) :
    QTcpServer(parent)
{
    setMaxPendingConnections(80);     //最大连接数为100
}

/***********************************************************************************
函数名 : incomingConnection
描  述 : 当客户端有连接情况时则调到该函数
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void LinRcvSever::incomingConnection(int socketId)
{
    LinRcvClientSocket *socket = new LinRcvClientSocket(this);
    printf("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTbcom LinRcvSever::incomingConnectionTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\n");
    socket->setSocketDescriptor(socketId);
}


/***********************************************************************************
函数名 : LinRcvClientSocket
描  述 : 客户端类
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
LinRcvClientSocket::LinRcvClientSocket(QObject *parent)
    : QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(this, SIGNAL(disconnected()), this, SLOT(offline()));
}

LinRcvClientSocket::~LinRcvClientSocket()
{

}

/***********************************************************************************
函数名 : readClient
描  述 : 读取网络端口数据
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void LinRcvClientSocket::readClient()
{
    if(bytesAvailable() <= 0)
        return;
    QByteArray pRcv = readAll();
    if(pRcv.isEmpty() || pRcv.size() < 6)
        return;
    //MyMsgLog<<pRcv.toHex();
    __pRcvFromSvr(pRcv);
    pRcv.clear();
}

/***********************************************************************************
函数名 : __RcvFromSvr
描  述 : 所有数据进行拆包解析
输入参数说明 :pRcv：所有数据，RcvLen：所有数据总和
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void LinRcvClientSocket::__pRcvFromSvr(QByteArray pArray)
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
                __pDataFrameAna(pArray.left(size));
                pArray.remove(0,size);
            }
            else
            {
                pArray.remove(0,1);
            }
        }
    }
}


/***********************************************************************************
函数名 : DataFrameAna
描  述 : 对一帧数据进行解析
输入参数说明 :Frame：一帧数据，size：一帧数据的长度
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void LinRcvClientSocket::__pDataFrameAna(QByteArray Frame)
{
    if(Frame.data()[1] == 0xff && Frame.data()[2] == 0xff && Frame.data()[3] == 0xff && Frame.data()[4] == 0xff)   //心跳包
    {
        char response[6] = {0x06, 0x00, 0x00, 0x00, 0x10,0x16};
        write(response,6);
    }
    else if(Frame.data()[1] == 0x00 && Frame.data()[2] == 0x00 && Frame.data()[3] == 0x00)   //
    {
        ClientVerify(Frame.data(),Frame.size());
    }
    else   //接受遠程數據並存儲到鏈表中
    {
        g_pMainDlg->m_pHomeControlDlg->slotRcvFromSvr(Frame,this->peerAddress().toString(),Frame.size());
    }
}

/***********************************************************************************
函数名 : ClientVerify
描  述 : 客户端登录时的账号密码验证
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void LinRcvClientSocket::ClientVerify(char *Frame, int size)
{
    QSettings Settings(home_control_config, QSettings::IniFormat);

    Settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

    QString username_get = Settings.value(Ter_Account, "1111-2222").toString();

    QString password_get = Settings.value(Ter_Pwd, "666666").toString();

    QString netPWD;
    QString PWD = username_get + password_get;
    for(int i=5;i<size-1;i++)
    {
        netPWD.append(Frame[i]);
    }
    MyMsgLog<<"pwd="<<netPWD;
    MyMsgLog<<"pwd="<<PWD;
    if(PWD == netPWD)   //手機端登錄並記錄該IP
    {
        g_pMainDlg->m_pHomeControlDlg->saveHouseMapXmlFile();
        char response[6] = {0x06, 0x00, 0x00, 0x00, 0x10,0x16};
        write(response, 6);
        flush();
        //g_pMainDlg->m_pHomeControlDlg->m_pRcvDataThread->ObjUplowdDataToClient(NULL,NULL,NULL,"append",this);
    }
    else
    {
        char response[6] = {0x06, 0x00, 0x00, 0x00, 0x1e,0x24};
        write(response, 6);
        flush();
    }
}

void LinRcvClientSocket::offline()
{
    //g_pMainDlg->m_pHomeControlDlg->m_pRcvDataThread->ObjUplowdDataToClient(NULL,NULL,NULL,"delete",this);
    deleteLater();
}

























/***********************************************************************************
 该类暂时没用
 ***************************************************************************************/
RcvFromClientDataThread::RcvFromClientDataThread(QObject *parent)
    : QThread(parent)
{
    connect(this,SIGNAL(sigRmFirst()),this,SLOT(slotRmFirst()),Qt::BlockingQueuedConnection);
}


/***********************************************************************************
函数名 : run
描  述 : 专门接收客户端发过来的数据，防止一旦数据过多时界面卡主现象
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void RcvFromClientDataThread::run()
{

}


/***********************************************************************************
函数名 : __RcvFromSvr
描  述 : 所有数据进行拆包解析
输入参数说明 :pRcv：所有数据，RcvLen：所有数据总和
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void RcvFromClientDataThread::__RcvFromSvr(char *pRcv, int RcvLen)
{

}

/***********************************************************************************
函数名 : DataFrameAna
描  述 : 对一帧数据进行解析
输入参数说明 :Frame：一帧数据，size：一帧数据的长度
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void RcvFromClientDataThread::DataFrameAna(char *Frame, int size)
{

}

/***********************************************************************************
函数名 : slotRmFirst
描  述 : 解析完后进行空间释放
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void RcvFromClientDataThread::slotRmFirst()
{

}
