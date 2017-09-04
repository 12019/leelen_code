#ifndef LUXDOMOCLIENT_H
#define LUXDOMOCLIENT_H

#include <QObject>
#include<pthread.h>
class LuxdomoClient : public QObject
{
    Q_OBJECT
public:
    explicit  LuxdomoClient(QObject *parent = 0);
    void  SendData(char*,int);
signals:
public slots:
public:
    int   m_socketfd;
    int   m_port;
    QString  m_addr;
};

#endif // LUXDOMOCLIENT_H
