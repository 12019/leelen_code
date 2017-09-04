#ifndef DOORBELLDIALOG_H
#define DOORBELLDIALOG_H

#include <QDialog>
#include <QFrame>
#include <QLabel>

namespace Ui {
    class DoorBellDialog;
}

class DoorBellDialog : public QDialog
{
    Q_OBJECT

public:
    	explicit DoorBellDialog(QWidget *parent = 0);
    	~DoorBellDialog();

	bool m_bExtensionUnitAnswer;
	int m_iExtensionUnitAddr;

	QFrame *m_pFrameFullshow; //全屏播放控件
	QTimer *m_pTimer; //等待超时定时器
    QTimer *m_pCloseLockTimer;
    bool m_OpenDoorFlag;
	QTimer *m_pInfoLabelTimer; //门铃响了信息显示定时器
	QTimer *m_pDisplayTimer;
	//int m_iPeerAddr; //当前户门口机地址

	void sendDoorBellCmd(int iCmd, int iPeerAddr = -1);

	QLabel *m_pInfoLabel; //门铃响了信息控件

	void stopViewer();
	int setDisplayWindow(bool bShow);

	bool event(QEvent *event);
	bool eventFilter(QObject *o, QEvent *e);

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	void Remote_Call_Open_Door_Bell();
	void Remote_Call_Interrupt_Wait();

public slots:
    void slotCloseLockTimerTimeout();
	void OpenDoorBellDlg();
	void CloseDoorBellDlg();
	void InfoLabelTimerTimeout();
	void DisplayTimerTimeout();

//private:
public:
    	Ui::DoorBellDialog *ui;

private slots:
	void on_pushButtonDoorBellOpen_pressed();

public slots:
	void on_pushButtonDoorBellReturn_pressed();
	void on_pushButtonDoorBellAnswer_pressed();
protected:
	virtual void keyPressEvent(QKeyEvent *k);
};

#endif // DOORBELLDIALOG_H
