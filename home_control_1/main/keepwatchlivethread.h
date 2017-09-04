#ifndef KEEPWATCHLIVETHREAD_H
#define KEEPWATCHLIVETHREAD_H

#include <QThread>

class KeepWatchLiveThread : public QThread
{
    Q_OBJECT
public:
    explicit KeepWatchLiveThread(QObject *parent = 0);
    ~KeepWatchLiveThread();
    void run();

    int m_watchdog_fd;

signals:

};

#endif // KEEPWATCHLIVETHREAD_H
