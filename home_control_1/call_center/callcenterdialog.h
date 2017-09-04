#ifndef CALLCENTERDIALOG_H
#define CALLCENTERDIALOG_H

#include <QDialog>
#include <QString>
#include <QFrame>
#include <QListView>

namespace Ui {
    class CallCenterDialog;
}

class CallCenterDialog : public QDialog
{
    Q_OBJECT

public:
        explicit CallCenterDialog(QWidget *parent = 0);
        ~CallCenterDialog();

	QString m_CallCenterName[2]; //管理中心机和管理中心机算机网络名
	QFrame *m_pFrameFullshow; //全屏播放控件

	char m_strRemoteIp[50]; //对讲的管理机IP地址

	bool m_bIctState; //是否处于接听状态
	bool m_bIctTalking; //是否处于拨号后通话状态
	bool m_bIstTalking; //是否处于接听后通话状态

	////QListView *getListViewCenterAddress();

	void ResetState();////bool bInitVolume = true);
	void setCallCenterName(QString addr1, QString addr2);

	bool event(QEvent *event);

	int setDisplayWindow(bool);

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	static void showCallCenterDialog();
	static CallCenterDialog *m_pDlg; //呼叫中心对话框指针
	void Door_Bell_Ring_Event(QString& str);
	void Exit_From_Full_Screen();
//private:
	Ui::CallCenterDialog *ui;

private slots:
	void on_pushButtonCallCenterRecord_pressed();
	void on_pushButtonCallCenterAnswer_pressed();
	void on_pushButtonCallCenterVideoClose_pressed();
	void on_pushButtonCallCenterVideoOpen_pressed();
	void on_pushButtonCallCenterDial_pressed();
	void on_pushButtonCallCenterHangUp_pressed();

public slots:
	void on_pushButtonCallCenterReturn_pressed();

protected:
	bool eventFilter(QObject *o, QEvent *e);
};

#endif // CALLCENTERDIALOG_H
