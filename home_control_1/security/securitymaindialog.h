#ifndef SECURITYMAINDIALOG_H
#define SECURITYMAINDIALOG_H

#include <QDialog>
#include "serial1thread.h"
#include <QTableView>
#include "securityalertdialog.h"
#include "securityplayerdialog.h"
#include "securityabnormaldialog.h"
#include "securitypassworddialog.h"
#include "jpegsendthread.h"
#include <QObjectList>
#include "securityrecvdoorwaysocket.h"
#include "defensegpioscanthread.h"
#include "uploaddeviceinfothread.h"
#include "uploadsetsecuritystatethread.h"

#define RET_PASSWORDOK		0
#define RET_CANCEL			1
#define RET_IGNORE			3
#define RET_UNDO			4
#define RET_AUTOIGNORE		5

#define PROPERTY_NORMAL		0
#define PROPERTY_GOOUT		1
#define PROPERTY_ATHOME		2

#define STATUS_DISARM		0
#define STATUS_GOOUT		1
#define STATUS_ATHOME		2

namespace Ui {
    class SecurityMainDialog;
}

class DelayAlarmClass : public QObject
{
	Q_OBJECT

public:
	explicit DelayAlarmClass(QObject *parent, unsigned char defenseType, int isDelayAlarm);
	~DelayAlarmClass();

	QTimer *m_pDelayTimer; //延迟防区的延迟定时器
	QList <QDateTime> m_alertDateTimeList; //在延迟定时器超时前该防区多次触发的时间列表
	QDateTime m_repeatAlarmLastDateTime; //该防区上一次报警时刻，用于判断重复报警过滤
	unsigned char m_defenseType; //防区类型(有线、无线)
	int m_isTimeOut; //延迟定时器是否超时
};

class SecurityMainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SecurityMainDialog(QWidget *parent = 0);
    ~SecurityMainDialog();

	QTableView *getTableViewSecurityList();


	//int m_isSecurityStateShifting; //是否设防状态转移

	//void showCurrentTime();
	int reloadSecurityList();
	void refresh_list_containt();
	int securityListAddOne(char *security_file_name);

	void delSecurityRecord(int row);
	void dellastsecurityavifile();		   // 删除最旧的那条AVI文件

	SecurityAlertDialog *m_pSecurityAlertDlg; //报警录像对话框
	SecurityPlayerDialog *m_pSecurityPlayerDlg; //报警录像播放对话框
	SecurityAbnormalDialog *m_pSecurityAbnormalDlg;
	SecurityPasswordDialog *m_pSecurityPasswordDlg;

	//david 2014.2.11
	DefenseGPIOScanThread *m_pDefenseGPIOScanThread;
	int m_securityState; //设防状态

	int m_allDefenseNumber[56 + 56]; //各个防区的防区号

	DelayAlarmClass *m_delayAlarmClass[56 + 56]; //该数组处理延迟报警、重复报警过滤时间

	Serial1Thread *m_pSerial1Thread; //串口报警数据接收处理线程
    UploadDeviceInfoThread *m_pUploadDeviceInfoThread;//上传防拆防移离信息线程
    UploadSetSecurityStateThread *m_pUploadSetSecurityStateThread;//上传布撤防记录线程
	int m_iEnterDelay; //进入延迟
	int m_iExitDelay; //退出延迟
	int m_iRepeatAlarmFilterInterval; //重复报警过滤时间
	int m_iSendPicToCenter; //是否发送JPEG图片到管理机

	int m_wireDefenseCnt; //有线防区数量
	int m_wirelessDefenseCnt; //无线防区数量

	int m_newDefenseCnt; //新报警数量
    bool m_bRecording;
    bool m_bSecurityMainDlgAloneOpened; //是否报警录像窗口没打开
    QTcpSocket *m_DeviceInfoSocket; //上传防拆防移信息socket

	//JpegSendThread *m_pJpegSendThread;

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	QTimer *m_pTimer;

    SecurityRecvDoorwayServer *m_pSecurityRecvDoorwayServer;

    void emitSigPlayWav(int type);
    void emitUploadDeviceInfo(int infotype);
    void CheckUdpState();
    void SendDeviceInfoByUdp(int infotype);
    void SendDeviceInfoByTcp(int infotype);
    int connectToCenter();
    void ReceiveUdpData(unsigned char receive_data);
    void emitsigSetSecurityStatueToPhone(int set_state);


//private:
    Ui::SecurityMainDialog *ui;

signals:
    void sigPlayWav(int num);
    void sigUploadDeviceInfo(int infotype);
    void sigHandleReceiveUdpData(unsigned char receive_data);
    void sigSetSecurityStatueToPhone(int set_state);

public slots:
	void slotSecurityListPressed();
	void createJpegSendThread();
    void startRecorder(int num,int idx, QString strDateTime, int number, int property, int channel, int name,
					   int type, QString strLocation, QString strAviPath);
	////void setTableViewSecurityListColor(QColor color);
    void slotHandleReceiveUdpdata(unsigned char receive_data);
	void setSecurityState(int state);
	void slotSetSecRowHeight(int ID, int row);
	void slotRefreshList(QDateTime,int,int,int,QString);
    void on_pushButtonSecMainSetArm_pressed();
    void slotPlayWav(int type);
    void slotUploadDeviceInfo(int infotype);

private slots:
	void on_tableViewSecurityList_pressed(QModelIndex index);
	//void tvSecurityListSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
	void on_pushButtonSecMainDelAll_pressed();
	void on_pushButtonSecMainDel_pressed();
	void on_pushButtonSecMainDetail_pressed();
	void on_pushButtonSecMainNext_pressed();
	void on_pushButtonSecMainPrev_pressed();
	void on_pushButtonSecMainReturn_pressed();
private:
	void remove_oldest_avi_file();
};

#endif // SECURITYMAINDIALOG_H
