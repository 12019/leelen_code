#ifndef MYQBYTEARRAY_H
#define MYQBYTEARRAY_H

#include <QObject>
#include"MyBuffer.h"
#include<QMutex>
#include<QLinkedList>

class MyQByteArray : public QObject
{
    Q_OBJECT
public:
    explicit MyQByteArray(QObject *parent = 0);

    QMutex RmLBFirstMutex;
    QMutex RmSDFirstMutex;
    QMutex RmPushToLBMutex;

    QLinkedList<RcvFromObj>pRcvFromLeeBus;
    RcvFromObj RcvLeeBus(QByteArray buf,QString SvrIP,QString action);
    void LeebusApend(QByteArray buf,QString SvrIP);
    RcvFromObj LeeBusFirst();
    void rmLeeBusFirst();

    QLinkedList<RcvFromObj>pRcvFromSvr;
    RcvFromObj SvrData(QByteArray buf,QString SvrIP,QString action);
    void SvrDataApend(QByteArray buf,QString SvrIP);
    RcvFromObj SvrDataFirst();
    void rmSvrDataFirst();

    QLinkedList<RcvFromObj>pPushToLeeBus;
    RcvFromObj PushToLeeBus(QByteArray buf, QString SvrIP,QString action);
    void PushToLeeBusApend(QByteArray buf,QString SvrIP);
    RcvFromObj PushToLeeBusFirst();
    void rmPushToLeeBusFirst();

signals:

public slots:

};

#endif // MYQBYTEARRAY_H
