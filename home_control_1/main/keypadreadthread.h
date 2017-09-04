#ifndef KEYPADREADTHREAD_H
#define KEYPADREADTHREAD_H

#include <QThread>
#include <QTimer>

class KeypadReadThread : public QThread
{
    Q_OBJECT
public:
    explicit KeypadReadThread(QObject *parent = 0);
	~KeypadReadThread();

	void run();

	void processKey(int tmp, int KeyValue);

	bool m_bRun; //线程是否正在运行

	QTimer *m_pTimer; //定时器，用于判断长按触摸屏校准按键是否超过5秒钟，如果超过则打开触摸屏校准窗口

    	void callElevator();

    	bool mainIsShow();

signals:
	void showMonDlg();
	void sigSendTerminalHelp();
	////void sigCamListPressTab();
	////void sigCamListPressEnter();
	void sigSetLCDAndTimerOn();
	//void sigPostEvent(QObject *receiver, QEvent *event);
	//void sigSendEvent(QObject *receiver, QEvent *event);

	void sigCloseDoorBellDlg();

public slots:

};

#endif // KEYPADREADTHREAD_H
