#ifndef ICTDIALOG_H
#define ICTDIALOG_H

#include <QDialog>
#include <QtGui/QLineEdit>
#include "ictrcvhookdialog.h"

#define RICT_ANSER		(1)
#define RICT_OPEND	    (1 << 1)
#define RICT_RECORD	    (1 << 2)

namespace Ui {
    class IctDialog;
}

class IctDialog : public QDialog
{
    Q_OBJECT

public:
    	explicit IctDialog(QWidget *parent = 0);
    	~IctDialog();

	bool event(QEvent *event);

	void ResetState(QString address = "null");

	//QString m_strDialIpAddress;

	char m_strRemoteIp[200]; //远端机IP地址

	IctRcvHookDialog *m_pRecvHookDlg; //拨打后通话对话框
	bool m_bHookCloseIctDlg; //是否关闭本ICT对话框

	int m_iModalType; //远端机模型类型

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	void Disable_Buttons();
	void Enable_Buttons();

	//bool m_bConnected;

	QTimer *m_pAutoAnswerTimer;
	QTimer *m_pAutoHungUpTimer;
//private:
	Ui::IctDialog *ui;

	bool m_VideoOn;
	bool m_Record_Flag;			// 如果 进行留言，那么就为true，给二次门铃使用
private:
	void Modify_Input_Parameters();		// 对输入进行调整的函数

public slots:
	void on_pushButtonIctAddressBook_pressed();
	void on_pushButtonIctRecord_pressed();
	void on_pushButtonIctVideoOpen_pressed();
	void on_pushButtonIctVideoClose_pressed();
	void on_pushButtonIctNum0_pressed();
	void on_pushButtonIctNum1_pressed();
	void on_pushButtonIctNum2_pressed();
	void on_pushButtonIctNum3_pressed();
	void on_pushButtonIctNum4_pressed();
	void on_pushButtonIctNum5_pressed();
	void on_pushButtonIctNum6_pressed();
	void on_pushButtonIctNum7_pressed();
	void on_pushButtonIctNum8_pressed();
	void on_pushButtonIctNum9_pressed();
	void on_pushButtonIctDot_pressed();
	void on_pushButtonIctShop_pressed();
	void on_pushButtonIctBack_pressed();
	void on_pushButtonIctHangUp_pressed();
	void on_pushButtonIctDial_pressed();

public slots:
    	void on_pushButtonIctReturn_pressed();

	bool Is_Manage_Machine(const QString&);
};

#endif // ICTDIALOG_H
