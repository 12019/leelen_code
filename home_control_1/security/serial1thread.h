#ifndef SERIAL1THREAD_H
#define SERIAL1THREAD_H

#include <QThread>
#include <QDateTime>
#include <QTimer>
#include "jpegsendthread.h"
#include <QColor>
#include <QTimer>
#include "core/leelen_comdef.h"
#include "leelen_protocol.h"
#include "leelen_system.h"
#include "leelen_security.h"
#include "sendalarminfothread.h"

class Serial1Thread : public QThread
{
    Q_OBJECT
public:
    explicit Serial1Thread(QObject *parent = 0);
	~Serial1Thread();

	bool uartInit();
	int alertRecord(unsigned char defenseType, int defenseIdx, int bIsRecord, QDateTime dateTime);
	void beginRecord();

	void run();

	void emitFunc();

	void Recall_Ring();

    void SendAlarmInfo(int num);

    void CheckUdpState();

    void SendByUdp(int num, int destination);

    void SendByTcp(int num, int i);

	QTimer *m_pDoorRingTimer;

    QDateTime m_AlarmDateTime;

	struct {
		unsigned char sign;
		unsigned char DetinationAddress;
		unsigned char SourceAddress;
		unsigned char command;
		unsigned char parameter[9];
		unsigned char CheckSum;
	} m_abnormalInfo; //串口报警信息的结构数据

	//bool m_bAlertDlgOpened;

	JpegSendThread *m_pJpegSendThread; //向管理机发送报警信息和JPEG图片的线程
    SendAlarmInfoThread *m_pSendAlarmInfoThread;
	int m_serial1_fd; //串口1设备句柄
	int m_bRun; //本线程是否正在运行

	int m_iDoorBellCmd;
	int m_iDoorBellAddr; //m_iPeerAddr;

	int m_iExtensionUnitCmd;
	int m_iExtensionUnitAddr;
	int m_iExtensionUnitCallAddr;

	int m_iHomeControlCmd;

	int m_iCallElevatorCmd;
	//int m_watchdog_fd;

	QString m_strDateTime; //报警时间
	int m_number; //防区号
	int m_property; //防区属性
	int m_channel; //防区通道(0-3)
	int m_name; //防区名称
	int m_type; //防区类型(有线/无线)
	bool m_bIsRecord;
	/*QString m_strNumber;
	QString m_strProperty;
	QString m_strChannel;
	QString m_strName;
	QString m_strType;*/
	QString m_strLocation; //报警所在位置
	QString m_strAviPath; //报警录像的AVI文件路径

	QTimer *m_pExitDelayTimer; //退出延迟定时器

	QTimer *m_pDeviceStartTimer;
	int m_bEnterDelayTimeOut; //是否有延迟防区定时器超时
	//int m_bIsExitDelay;

	int m_openRecorderCnt;

	void waitForVacant();

	QTimer *m_pAutoQuitTimer;

    int openRecorderCnt[8];
    QString strDateTime[8];
    int number[8];
    int property[8];
    int channel[8];
    int name[8];
    int stype[8];
    QString strLocation[8];
    QString strAViPath[8];

signals:

    void openRecorder(int,int, QString, int, int, int, int, int, QString, QString);
	void sigCreateJpegThread();

	void sigOpenDoorBellDlg();
	void sigExtensionUnitAnswerDoorBell();
	void sigCloseDoorBellDlg();
	void sigShowAbnormalMsgBox1(QString strText);
	void sigShowAbnormalMsgBox2(QString strText);
	void sigShowAbnormalMsgBox3(QString strText);    // 门铃响应函数

	void sigExtensionUnitAnswer();
	void sigExtensionUnitHangUp();
	void sigExtensionUnitOpen();

	void sigExtensionUnitMon();
	void sigCloseMonDlg();

	void sigRefrashList(QDateTime,int,int,int,QString);
	////void sigSetSecurityNotify();
	//void sigSetTableViewSecurityListColor(QColor color);

public slots:
	void enterDelayTimeOut();
    void stopDoorRing();
private:
};


#endif // SERIAL1THREAD_H
