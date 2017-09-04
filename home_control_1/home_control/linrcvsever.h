#ifndef LINRCVSEVER_H
#define LINRCVSEVER_H

#include <QTcpServer>
#include<QTcpSocket>
#include<QMutex>
#include<QThread>
#include<QTimer>

class RcvFromClientDataThread : public QThread
{
    Q_OBJECT
public:
    explicit RcvFromClientDataThread(QObject *parent = 0);
    void run();
    void __RcvFromSvr(char *pRcv,int RcvLen);
    void DataFrameAna(char *Frame,int size);
signals:
    void sigRmFirst();
public slots:
    void slotRmFirst();
};

class LinRcvSever : public QTcpServer
{
    Q_OBJECT
public:
    explicit LinRcvSever(QObject *parent = 0);

signals:

public slots:
private:
    void incomingConnection(int socketId);
};

class LinRcvClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    LinRcvClientSocket(QObject *parent = 0);
    ~LinRcvClientSocket();
    void __pRcvFromSvr(QByteArray pArray);
    void __pDataFrameAna(QByteArray Frame);
    void ClientVerify(char *Frame,int size);
    //QTimer *timer;

private slots:
    void readClient();
    void offline();
signals:
};



#endif // LINRCVSEVER_H
