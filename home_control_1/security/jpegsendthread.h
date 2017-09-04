#ifndef JPEGSENDTHREAD_H
#define JPEGSENDTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include "core/leelen_comdef.h"
#include "leelen_protocol.h"
#include "core/leelen_big_packet.h"
#include "leelen_big.h"
#include "leelen_protocol.h"
#include <QList>

class JpegSendThread : public QThread
{
    Q_OBJECT
public:
    explicit JpegSendThread(QObject *parent = 0);
	~JpegSendThread();

    int ConnectToManCenter(int i);
    int SendAlertInfo(int type, int number, int name, int channel, int property,
                      QString strLocation, QString strDateTime, int destination);

	QTcpSocket *m_pTcpSock[2]; //连接到两个管理机的TCP Socket
	bool m_bRun; //线程是否在运行
	bool m_bBusy; //线程函数是否正忙
    void SendPicByTcp(char *buf, int buf_len , int jpg_num , int num);
    int SendPicByUdp(big_file_t *big_file , int num);
    int m_alarm_num; //本次报警的防区号
    QList<QString> m_FileNameList; // 保存上传的图片名字队列
    QList<QString> m_FileDirList; //保存上传的图片的路径+名字队列

protected:
	void run();

signals:

public slots:
	//void sockDisconnect();
};

#endif // JPEGSENDTHREAD_H
