#ifndef SENDINFOTHREAD_H
#define SENDINFOTHREAD_H

#include <QThread>

class SendInfoThread : public QThread
{
    Q_OBJECT
public:
    explicit SendInfoThread(QObject *parent = 0);
    ~SendInfoThread();
    bool m_bRun;
    void run();
    void CheckUdpState();
    QString m_sendToName; //信息接收发号码
signals:
    
public slots:
    
};

#endif // SENDINFOTHREAD_H
