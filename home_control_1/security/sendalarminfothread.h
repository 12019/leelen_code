#ifndef SENDALARMINFOTHREAD_H
#define SENDALARMINFOTHREAD_H

#include <QThread>

class SendAlarmInfoThread : public QThread
{
    Q_OBJECT
public:
    explicit SendAlarmInfoThread(QObject *parent = 0);
    ~SendAlarmInfoThread();

    bool m_bRun;
    bool m_StarSendJpegThreadFlag;
    int m_num; //本次报警防区号
    void run();
    
signals:
    
public slots:
    
};

#endif // SENDALARMINFOTHREAD_H
