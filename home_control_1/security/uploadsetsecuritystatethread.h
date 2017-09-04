#ifndef UPLOADSETSECURITYSTATETHREAD_H
#define UPLOADSETSECURITYSTATETHREAD_H

#include <QThread>
#include <QTcpSocket>
class UploadSetSecurityStateThread : public QThread
{
    Q_OBJECT
public:
    explicit UploadSetSecurityStateThread(QObject *parent = 0);
    ~UploadSetSecurityStateThread();
    bool m_bRun;

    void TcpUploadSetSecurityState();
    void UdpUploadSetSecurityState();
    void run();


    
signals:
    
public slots:
    
};

#endif // UPLOADSETSECURITYSTATETHREAD_H
