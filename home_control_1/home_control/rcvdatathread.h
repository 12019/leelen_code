#ifndef RCVDATATHREAD_H
#define RCVDATATHREAD_H

#include <QThread>
#include<MyBuffer.h>
#include<QMutex>
#include"linrcvsever.h"
#include"server_client/luxdomoclient.h"

class RcvDataThread : public QThread
{
    Q_OBJECT
public:
    explicit RcvDataThread(QObject *parent = 0);
    RcvFromObj MyRcv;
    QByteArray __ObjToSvrData;
    QMutex mutex;
    void run();
    void ObjUplowdDataToLinSvr(char *buf,int len,QString SvrIP);
    void ObjUplowdDataToClient(char *buf,int len,QString SvrIP,QString action,LuxdomoClient *psock);
signals:
public slots:
};

#endif // RCVDATATHREAD_H
