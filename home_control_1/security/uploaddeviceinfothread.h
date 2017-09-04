#ifndef UPLOADDEVICEINFOTHREAD_H
#define UPLOADDEVICEINFOTHREAD_H

#include <QThread>

class UploadDeviceInfoThread : public QThread
{
    Q_OBJECT
public:
    explicit UploadDeviceInfoThread(QObject *parent = 0);
    ~UploadDeviceInfoThread();

    bool m_bRun;
    int m_infotype;

    void run();
    
signals:
    
public slots:
    
};

#endif // UPLOADDEVICEINFOTHREAD_H
