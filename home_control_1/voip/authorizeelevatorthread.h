#ifndef AUTHORIZEELEVATORTHREAD_H
#define AUTHORIZEELEVATORTHREAD_H

#include <QThread>

class AuthorizeElevatorThread : public QThread
{
    Q_OBJECT
public:
    explicit AuthorizeElevatorThread(QObject *parent = 0);
    ~AuthorizeElevatorThread();

    void TcpAuthorizeElevator();
    void UdpAuthorizeElevator();
    bool CheckUdpState();
    bool m_bRun;
    QString m_talkRemoteName;
    void run();
    
signals:
    
public slots:
    
};

#endif // AUTHORIZEELEVATORTHREAD_H
