#ifndef NETRECVSOCKET_H
#define NETRECVSOCKET_H

#include <Qt/qtcpsocket.h>
#include <Qt/qtcpserver.h>
#include "data_types.h"

class NetRecvClientSocket : public QTcpSocket
{
	Q_OBJECT
public:
	NetRecvClientSocket(QObject *parent = 0);
	~NetRecvClientSocket();

	REQUEST_FRAME_T m_RequestFrame; //请求帧头

	int m_PackageLen; //包长度
	char *m_Buf; //接收到的数据缓存区
	int m_BufReadLen; //已接收的数据长度

private slots:
	void readClient();
};

class NetRecvServer : public QTcpServer
{
	Q_OBJECT
public:
	NetRecvServer(QObject *parent = 0);

private:
	void incomingConnection(int socketId);
};

#endif // NETRECVSOCKET_H
