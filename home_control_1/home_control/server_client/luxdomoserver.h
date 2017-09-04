#ifndef LUXDOMOSERVER_H
#define LUXDOMOSERVER_H

#include <QObject>
#include<pthread.h>
#include"server_client/luxdomoclient.h"
class LuxdomoServer : public QObject
{
    Q_OBJECT
public:
    explicit LuxdomoServer(QObject *parent = 0);
    bool   CloseListen();
    void   OpenListen(QString,QString);
    LuxdomoClient*  FindClient(int);
signals:
public slots:
public:
    pthread_t   m_server;
    int   m_socketfd;
};

#endif // LUXDOMOSERVER_H
