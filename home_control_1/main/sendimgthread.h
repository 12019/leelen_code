#ifndef SENDIMGTHREAD_H
#define SENDIMGTHREAD_H

#include <QThread>
#include <QTcpSocket>

class SendImgThread : public QThread
{
    Q_OBJECT
public:

    explicit SendImgThread(QObject *parent = 0);
    ~SendImgThread();

    int ConnectToManCenter();

    QTcpSocket *m_pTcpSock; //连接到两个管理机的TCP Socket

    bool m_bRun; //线程是否在运行

    void run();

};

#endif // SENDIMGTHREAD_H
