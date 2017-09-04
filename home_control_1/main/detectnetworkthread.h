#ifndef DETECTNETWORK_H
#define DETECTNETWORK_H

#include <QThread>

class DetectNetworkThread : public QThread
{
    Q_OBJECT
public:
    explicit DetectNetworkThread(QObject *parent = 0);

    void run();
    int oldnetwork;

    bool m_bRun; //线程是否正在运行

signals:

public slots:

};

#endif // DETECTNETWORK_H
