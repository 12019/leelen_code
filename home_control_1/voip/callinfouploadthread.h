#ifndef CALLINFOUPLOADTHREAD_H
#define CALLINFOUPLOADTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QTimer>

class CallInfoUploadThread : public QThread
{
    Q_OBJECT
public:
    explicit CallInfoUploadThread(QObject *parent = 0);
    ~CallInfoUploadThread();

    QTcpSocket *m_InfoUploadSocket;
    int connectToCenter();

    bool m_bRun;

    void run();

    void getCallCode(QString str);
    void CheckUdpState();
    void SendRecordByUdp();
    QString strPeerName;
    int m_TalkTime;  //通话时长
    QTimer *m_TalkTimer;// 一秒超时定时器

    int m_IsUpImage;//当m_IsUpImage为1上传图片，当为2不上传
    int m_upInfoType;//当m_uoInfoType为1，则上传呼叫信息；2为接听信息，3，为开锁信息
signals:

public slots:
    void slotPhoneTime();//
};

#endif // CALLINFOUPLOADTHREAD_H
