#ifndef CALLELEVATORTHREAD_H
#define CALLELEVATORTHREAD_H

#include <QThread>

class CallElevatorThread : public QThread
{
    Q_OBJECT
public:
    explicit CallElevatorThread(QObject *parent = 0);
    ~CallElevatorThread();
    bool m_bRun;
    void run();
    bool CheckUdpState();
    void UdpCallElevator();
    
signals:
    
public slots:
    
};

#endif // CALLELEVATORTHREAD_H
