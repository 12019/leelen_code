#ifndef DOORBELLRINGTHREAD_H
#define DOORBELLRINGTHREAD_H

#include <QThread>

class DoorBellRingThread : public QThread
{
    Q_OBJECT
public:
    explicit DoorBellRingThread(QObject *parent = 0);

    void run();

    void start_pwm_export(void);
    void stop_pwm_export(void);

    bool m_bRun; //线程是否正在运行
    int m_doorbellring;

signals:

public slots:

};

#endif // DOORBELLRINGTHREAD_H
