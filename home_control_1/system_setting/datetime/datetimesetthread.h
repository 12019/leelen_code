#ifndef DATETIMESETTHREAD_H
#define DATETIMESETTHREAD_H

#include <QThread>

#define TIME_RESULT_INIT    -2
#define TIME_RESULT_FALSE   -1
#define TIME_RESULT_SUCCESS  0

class DateTimeSetThread : public QThread
{
    Q_OBJECT
public:
    explicit DateTimeSetThread(QObject *parent = 0);
    ~DateTimeSetThread();

    bool m_bRun;
    int m_TcpSetCmmputerTimeResult;
    bool m_autoUpdateTimeFlag; //该线程是否由十二点后五分钟内自动校时或者开机五分钟内校时时候调用
    void run();

signals:
    
public slots:
    
};

#endif // DATETIMESETTHREAD_H
