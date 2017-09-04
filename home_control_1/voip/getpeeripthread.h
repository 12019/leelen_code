#ifndef GETPEERIPTHREAD_H
#define GETPEERIPTHREAD_H

#include <QThread>
#include "getsubmachineipthread.h"

/*
#define BASIC_MODAL_TYPE		(1 << 0)    //E60\E75
#define THIN_MODAL_TYPE			(1 << 1)  //E-26
#define DOORWAY_MODAL_TYPE_3_5	(1 << 2) //10型
#define DOORWAY_MODAL_TYPE_10	(1 << 3) //1型
#define MANCENTER_MODAL_TYPE	(1 << 4) //管理机
#define DOORWAY_MODAL_TYPE_8	(1 << 5) //8型
#define DOORWAY_MODAL_TYPE		(DOORWAY_MODAL_TYPE_3_5 \
    | DOORWAY_MODAL_TYPE_10 | DOORWAY_MODAL_TYPE_8 \
    | DOORWAY_MODAL_TYPE_4   \
    | DOORWAY_MODAL_TYPE_16)        //zhuji
#define IP_SWITCH_TYPE (1 << 6)
#define DOORWAY_MODAL_TYPE_4  (1 << 7) //4型
#define DOORWAY_MODAL_TYPE_16  (1 << 8) //16型
*/

#define BASIC_MODAL_TYPE		(1 << 0)  //E60\E75
#define THIN_MODAL_TYPE			(1 << 1)
#define DOORWAY_MODAL_TYPE_3_5	(1 << 2)
#define DOORWAY_MODAL_TYPE_10	(1 << 3)
#define MANCENTER_MODAL_TYPE	(1 << 4)
#define DOORWAY_MODAL_TYPE_8	(1 << 5)
#define DOORWAY_MODAL_TYPE		(DOORWAY_MODAL_TYPE_3_5 | DOORWAY_MODAL_TYPE_10 | DOORWAY_MODAL_TYPE_8)
#define IP_SWITCH_TYPE (1 << 6)

class GetPeerIPThread : public QThread
{
    Q_OBJECT
public:
	explicit GetPeerIPThread(QObject *parent = 0);
	~GetPeerIPThread();

	QString m_nativeName; //本机网络名称
	QString m_nativeIP; //本机IP地址

    int m_iModalType;

	bool m_bRun; //线程是否正在运行
	void run();

	int GetPeerIP(char *strPeerName, char *strPeerIP);

    int CallForPeerName(QString strPeerName);

    int CallByAddrList(QString strPeerName);

    int getIPFromAddrList(QString strPeerName,char *strPeerIP);

    void enableTimeValid();
signals:

public slots:

};

#endif // GETPEERIPTHREAD_H
