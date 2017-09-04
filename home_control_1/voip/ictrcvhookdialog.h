#ifndef ICTRCVHOOKDIALOG_H
#define ICTRCVHOOKDIALOG_H

#include <QDialog>
#include <QFrame>
#include <QLineEdit>

#define MAX_TIMEOUT_DIALOG 30*60*1000   // 30分钟的分机对讲通话时间

namespace Ui {
    class IctRcvHookDialog;
}

class IctRcvHookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IctRcvHookDialog(QWidget *parent = 0);
    ~IctRcvHookDialog();

	//QLineEdit *getLineEditDisplay();

	void ResetState(QString strInfo);

	void Start_Timeout_Count();	// 开始超时记时
	void Stop_Timeout_Count();	// 停止超时记时

	QFrame *m_pFrameFullshow; 	//全屏播放控件

    	QTimer *m_pFilterTimer; 	//david 2014/10/18
	QTimer *m_pTimeoutDialog;  	//30分钟超时定时器

	int setDisplayWindow(bool);

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	bool m_Call_Manage_Machine;  	// 本次呼叫的对象是管理机

	void Exit_From_Full_Screen();   // 从全屏状态退出
//private:
    Ui::IctRcvHookDialog *ui;

private slots:
    void on_pushButtonIctRcvHookOpen_pressed();
    void on_pushButtonIctRcvHookVideoOpen_pressed();
    void on_pushButtonIctRcvHookVideoClose_pressed();
    void Dialog_Timeout_Event();	// 通话超时处理函数

public slots:
    void on_pushButtonIctRcvHookReturn_pressed();
    void on_pushButtonIctRcvHookHangUp_pressed();

protected:
	bool eventFilter(QObject *o, QEvent *e);

};

#endif // ICTRCVHOOKDIALOG_H
