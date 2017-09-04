#ifndef SECURITYALERTDIALOG_H
#define SECURITYALERTDIALOG_H

#include <QDialog>
#include <QFrame>
#include <QTimer>
//#include "securitypassworddialog.h"
#include <QMessageBox>
//#include "jpegsendthread.h"

namespace Ui {
    class SecurityAlertDialog;
}

class SecurityAlertDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SecurityAlertDialog(QWidget *parent = 0);
    ~SecurityAlertDialog();

	//void setDetail(QString strDateTime, QString strNumber, QString strProperty, QString strChannel, QString strName,
	//				QString strType, QString strLocation, QString strAviPath);
	void setDetail(QString strDateTime, int number, int property, int channel, int name,
		 int type, QString strLocation, QString strAviPath);

	void startRecorder();
	void stopRecorder(int bReleaseJpegSendThread = true);

	bool m_bRecorderStopping; //防止重入stopRecorder()
    bool doorbell_power_on_flag; // 二次门铃打开供电的标志

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	//JpegSendThread *m_pJpegSendThread;

	QString m_strAviPath; //AVI录像文件路径
	int m_channel; //录像视频通道号(0-3)

	QFrame *m_pFrameFullshow; //全屏显示控件
	int setDisplayWindow(bool);

	QTimer *m_pTimer; //定时器控制录像30秒超出后退出录像
	int m_timerSec; //30秒剩余的时间

	//SecurityPasswordDialog *m_pSecurityPasswordDlg;
	//QDialog *m_pSubDlg; //子对话框

	int m_property;

	//QMessageBox *PreMessageBox(QDialog *parent, QString text, QString button_caption);
    //david
	//void setStateText();

protected:
	bool eventFilter(QObject *o, QEvent *e);

//private:
public:
	Ui::SecurityAlertDialog *ui;

public slots:
	void timerTimeout();
	void on_pushButtonSecAlertDisArm_pressed();
	void on_pushButtonSecAlertHome_pressed();
	void on_pushButtonSecAlertOut_pressed();
	void on_pushButtonSecAlertReturn_pressed();
};

#endif // SECURITYALERTDIALOG_H
