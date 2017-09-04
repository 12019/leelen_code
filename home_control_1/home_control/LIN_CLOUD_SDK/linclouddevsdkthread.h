#ifndef LINCLOUDDEVSDKTHREAD_H
#define LINCLOUDDEVSDKTHREAD_H

#include <QThread>
#include<QTimerEvent>
#include<MyBuffer.h>
#include<QMutex>

class LINCloudDevSDKThread : public QThread
{
    Q_OBJECT
public:
    explicit LINCloudDevSDKThread(QObject *parent = 0);
    ~LINCloudDevSDKThread();
    void run();

    static LIN_ExceptionEnm sOnRcvLeeBusData(
                                             LINCloudDevSDKThread *pWorkObj,
                                             LINExp_cpByte pRcv,
                                             LINExp_Uint RcvLen);
    static void sOnNetClose(
                            LINCloudDevSDKThread *pWorkObj,
                            LIN_ExceptionEnm CloseReason);


    void LIN_LoginToCloud();
    LIN_ExceptionEnm __sOnRcvLeeBusData(LINExp_cpByte pRcv,LINExp_Uint RcvLen);
    void __UplowdDataToLinCloud(char *pRcv,int RcvLen,QString SvrIP,QString flag);
    void __OnNetClose(LIN_ExceptionEnm CloseReason);

    int    GetUpdateInfo();
    bool DownloadUpdateFile();
    char*  updatePath;

    int ReCntTime;
    bool pClientInitIsValid;
    bool LinCloudIsValid;
    LINExp_NetHandle __HNet;
    QMutex mutex;
protected:
    void timerEvent(QTimerEvent *e);
signals:
    void   sigCloudCondition(QString);
public slots:
};

#endif // LINCLOUDDEVSDKTHREAD_H
