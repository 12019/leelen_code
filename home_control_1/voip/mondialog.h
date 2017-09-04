#ifndef MONDIALOG_H
#define MONDIALOG_H

#include <QDialog>
#include <QFrame>
#include <QModelIndex>
#include <QTimer>
#include "mondialdialog.h"
#include "monlistdialog.h"

namespace Ui {
	class MonDialog;
}

#define LOCAL_CAMERA		0
#define BELL_CAMERA		1
#define DOOR_MACHINE		2
//#define WALL_MACHINE		3
#define IP_CAMERA		4

class	MonDialog : public QDialog
{
    Q_OBJECT

public:
	explicit MonDialog(QWidget *parent = 0);
	~MonDialog();

	bool m_bExtensionUnitMon;
	int m_iExtensionUnitAddr;
	int m_iMonitoredAddr;

	void ResetState(bool bResetNetName = false);

	//void translateTableWidget();

	int m_monType;

	////char m_strRemoteIp[50]; //门口机IP地址

	bool event(QEvent *event);

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	void stopViewer(bool bMctTerminate);
	int setDisplayWindow(bool bShow);

	bool eventFilter(QObject *o, QEvent *e);

	//bool m_bResetState;

	//QString m_strRemoteNetName;
	QString m_strMonAddr;

	QFrame *m_pFrameFullshow; //全屏播放控件
	QTimer *m_pTimer; 	  //按了监控对象列表控件后用该定时器延时100毫秒后再启动视频监控
	QTimer *m_pIPCTimer; 	  //按了监控对象列表控件后用该定时器延时100毫秒后再启动视频监控

	QTimer *m_pMonStopTimer;  //进行监控开启的定时器

	QTimer *m_pFirstCamTimer;

    QTimer *m_CameraTimer;  // 当网路断线，进行40秒定时，时间到断开网络摄像头监控
	//MonDialDialog *m_pMonDialDlg;
	MonListDialog *m_pMonListDlg;
    QTimer *m_pHandUpDoorViewTimer;//开锁五秒后挂断监视二次门铃视频定时器
    QTimer *m_pUnlockTimer; //闭合二次门铃锁定时器
    bool m_hasStopDoorbellViewFlag;//已经关闭监视门铃视频标志
    //pthread_t m_rtspThread;

	void emitSigIpCamFinish();

	void Network_Status_Chang(int);
	void Do_Door_Bell_Close_Video();
	void Exit_From_Full_Screen_Status();
signals:
	void sigIpCamFinish();

public slots:
    void slotHandUpdoorViewTimer();
    void slotUnlockTimerTimeout();
	void timerCamListTimeout();
	void showMonDlg();
	void showFirstCam();
	void showIPCam();
    	void ipCamFinish();

	void Stop_Monitor();

    void Camera_Net_Disable();
public:
	Ui::MonDialog *ui;

private slots:
	void on_pushButtonMonList_pressed();
	void on_pushButtonMonDial_pressed();
	void on_pushButtonMonOpen_pressed();
	void on_pushButtonMonHangUp_pressed();

public slots:
	 void on_pushButtonMonReturn_pressed();
protected:
	virtual void keyPressEvent(QKeyEvent *k);
private:
	void private_func1();
	void private_func2();
};

#endif // MONDIALOG_H
