#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include "terminalhelpmaindialog.h"
#include "voipmaindialog.h"
#include "infomanmaindialog.h"
#include "mediamandialog.h"
#include "datetimesetdialog.h"
#include "syssetmaindialog.h"
#include "qconfig.h"
#include "mypushbutton.h"
#include "commonpushbutton.h"
//#include <QMessageBox>
#include "mymessagebox.h"
#include "data_types.h"
#include "netrecvsocket.h"
#include "clock.h"
#include "calendar.h"
#include "callcenterdialog.h"
#include "securitymaindialog.h"
#include "helpdialog.h"
#include "inputmethod.h"
#include "keypadreadthread.h"
#include <QTimer>
#include "updownloadsocket.h"
#include "syssetpassworddialog.h"
#include "syssetengineerdialog.h"
#include "callelevatordialog.h"
#include "doorbelldialog.h"
#include "terminalhelpthread.h"
#include "calibrationdialog.h"
#include "tscalibratethread.h"
#include "homecontrolscenedialog.h"
#include "addressbookdialog.h"
#include "photomaindialog.h"
#include "testdialog.h"
#include "inputinfodialog.h"
#include "detectnetworkthread.h"
#include "doorbellringthread.h"
#include "homecontrolmaindlg.h"
//#include "screensaverdialog.h"
#include "callelevatorthread.h"


namespace Ui {
    class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
	    explicit MainDialog(QWidget *parent = 0);
	    ~MainDialog();

	NetRecvServer *m_pNetRcvSrv; //在554号端口上监听网络信息连接
    //UpDownloadServer *m_pUpDownloadSrv;

	TerminalHelpThread *m_pTermHelpThread;

    //TerminalHelpMainDialog *m_pTermHelpMainDlg; //lg

	VoipMainDialog *m_pVoipMainDlg; //VoIP主对话框
	InfoManMainDialog *m_pInfoManMainDlg; //信息管理对话框
	MediaManDialog *m_pMediaManDlg; //对讲管理对话框
	SysSetMainDialog *m_pSysSetMainDlg; //系统设置主对话框
	SysSetEngineerDialog *m_pSysSetEngineerDlg; //工程参数设置主对话框
	CallCenterDialog *m_pCallCenterDlg; //呼叫中心对话框
	SecurityMainDialog *m_pSecurityMainDlg; //安防管理对话框
	//InterFreeSetDialog *m_pInterFreeSetDlg; //lg
	CallElevatorDialog *m_pCallElevatorDlg;
	DoorBellDialog *m_pDoorBellDlg;
	HelpDialog *m_pHelpDlg; //帮助对话框
    HomeControlMainDlg *m_pHomeControlDlg;
	AddressBookDialog *m_pAddressBookDlg;
	PhotoMainDialog *m_pPhotoMainDlg;
	//TestDialog *m_pTestDlg;

	SysSetPasswordDialog *m_pSysSetPwdDlg; //系统设置密码输入对话框

    CalibrationDialog *m_pCalibrationDlg; //触摸屏校准对话框

	//ScreenSaverDialog *m_pScreenSaverDlg;

	MyMessageBox *m_pAbnormalMsgBox; //系统启动时异常防区信息显示对话框
	CommonPushButton *m_pAbnormalMsgBoxOkBtn; //系统启动时异常防区信息显示对话框的"确定"按钮

	MyMessageBox *m_pIpConflictMsgBox; //IP地址和其它设备相重信息显示对话框
	CommonPushButton *m_pIpConflictMsgBoxOkBtn; //IP地址和其它设备相重信息显示对话框的"确定"按钮

	MyMessageBox *m_pTerminalHelpMsgBox; //终端求助返回信息显示对话框
	CommonPushButton *m_pTerminalHelpMsgBoxOkBtn; //终端求助返回信息显示对话框的"确定"按钮

	MyMessageBox *m_pInterFreeMsgBox; //免扰状态信息显示对话框
	CommonPushButton *m_pInterFreeMsgBoxOkBtn; //免扰状态信息显示对话框的"确定"按钮

	MyMessageBox *m_pCallElevatorMsgBox; //呼叫电梯信息显示对话框
	CommonPushButton *m_pCallElevatorMsgBoxOkBtn; //呼叫电梯信息显示对话框的"确定"按钮

    	InputInfoDialog *m_pInputMethodWidget; //输入法窗口部件
	int m_inputMethodMode; //输入法模式，0:English, 1:Ch+English, 2:Big5+English
	InputMethod *m_pInputMethod; //输入法对象
	int setInputMethodMode(int mode); //0:English, 1:Ch+English, 2:Big5+English

	KeypadReadThread *m_pKeypadReadThread; //终端右边按键接收处理线程
    TSCalibrateThread *m_pTSCalibrateThread;

    DetectNetworkThread *m_pDetectNetworkThread;//检测网络工作状态线程
    DoorBellRingThread *m_pDoorBellRingThread;//门铃pwm音频输出线程
    CallElevatorThread *m_pCallElevatorThread;//呼叫电梯线程
    void hideWidgets();

	void refreshTabFocus();
	void showRefreshTabFocus(QDialog *pDlg);
	void hideRefreshTabFocus(QDialog *pDlg);

	QDialog *m_pCurActiveDlg;
    //unsigned int m_nShowSubDlgCnt;

    int m_iCurModalType; //本机的模型类型，为BASIC_MODAL_TYPE

	int m_wipeMouseX;

	Clock *getClock();
	Calendar *getCalendar();

	Ui::MainDialog *getUi();

	uint32_t m_nTimeMode; //1 clock mode
						  //2 digital 12 hours mode
						  //3 digital 24 hours mode

	void setTimeMode(uint32_t nTimeMode);

	MyPushButton **m_pFunctionModule; //主窗口上的功能按钮
	int m_nDispFirstFMIdx; //主窗口上当前显示的第一个功能按钮的序号
	QPoint m_nDispFirstFMTopLeft; //主窗口上当前显示的第一个功能按钮的左上角坐标
	int m_nDispFMSpan; //主窗口上显示相邻两个功能按钮的横坐标差

	int ReDisplayButtons();

	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

	int m_watchdog_fd; //看门狗的设备句柄
	QTimer *m_pWatchDogTimer; //该定时器每0.5秒触发一次喂看门狗
    	int m_ticks; //控制每20秒清空一次Cache


	void translateTableViewNofifier();
	bool m_bInterFree;

    //int m_bEnableTVOut;

	void emitSigSetNotify(int indx);
	void emitSigSetRowHeight(int ID, int row);
	void emitSigSetNetWork(int mode);
	void emitSigResetTimeVaraiable();							// 当授时发生后进行触发

	void emitCallRecordInfoManager(QString,QString,QString,QString,QString);		// 调用信号量进行信息更新

	bool event(QEvent *e);


	void setIndicatorLEDState(bool);

	void start_pwm_ring();
	void stop_pwm_ring();

    void emitSigTcpCallElevator();
    void emitSigShowCallElevatorMsgBox(QString);
//private:
    Ui::MainDialog *ui;

public:
    	void Test_Fun1();
	void AlarmLEDTimerTimeout();
private:

signals:
	void sigSetNotify(int indx);
    	void sigSetRowHeight(int ID, int row);		//增加此信号用来在线程中设置表格中的信息框高度 lidh 20141018
	void sigCallRecordInfoManager(QString,QString,QString,QString,QString);      		// 信号量用于更新呼叫对讲GUI界面
    	void sigSetNetWork(int mode);   		//增加此信号用来在线程中设置主界面信息提示框的网络状态 lidh 20150123
	void sigResetTimeVariable();			// 重新设置时间变量
    void sigTcpCallElevator();
    void sigShowCallElevatorMsgBox(QString);
public slots:
	void openCalibration();
	void sendTerminalHelp();
	void watchDogTimerTimeout();

	void showTermHelpResult(QString strInfo);
	void showAbnormalMsgBox(QString strText);
	void Do_Door_Bell_Event(QString strText);			// 处理门铃响的事件
	void setNotify(int indx);
    	//void slotLCDBacklightOn(bool bOn);
    	void on_pushButtonCallElevator_pressed();

	void slotCallRecordInfoManager(QString,QString,QString,QString,QString);		// 信号槽
    	void slotSetNetWork(int mode);
	void slotResetTimeVariable();
    void slotTcpCallElevator();
    void slotShowCallElevatorMsgBox(QString);
private slots:
 void on_pushButtonPhoto_pressed();
 void on_pushButtonOther_pressed();
 void on_pushButtonHomeControl_pressed();
 void on_pushButtonSystemSetting_pressed();

 void on_pushButtonInterFreeSetting_clicked();
 void on_pushButtonTerminalHelp_pressed();
 void on_pushButtonCallCenter_pressed();
 void on_pushButtonSecurityMan_pressed();
 void on_pushButtonVideoMon_pressed();
 void on_pushButtonInfoMan_pressed();
 void on_pushButtonIntercomMan_pressed();
 void on_pushButtonVoip_pressed();
 void on_pushButtonSendImg_pressed();
 void on_tableViewNotifier_pressed(const QModelIndex &index);
};

void setAllButtonsFocusPolicy(QDialog *pDlg, Qt::FocusPolicy policy);
void setAllButtonsEnabled(QDialog *pDlg, bool bEnable);
//void hideAllSubWidget(QDialog *pDlg, QWidget *pWidgetException);
void closeMessBox(QDialog *pDlg);

int setIndicatorState(int state);

#endif // MAINDIALOG_H
