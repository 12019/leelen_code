#ifndef DATETIMESETDIALOG_H
#define DATETIMESETDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include "datetimesetthread.h"

#define TO_MANAGER_MACHINE 0
#define TO_COMPUTER_CENTER 1
namespace Ui {
    class DatetimeSetDialog;
}

class DatetimeSetDialog : public QDialog
{
    Q_OBJECT

public:
    	explicit DatetimeSetDialog(QWidget *parent = 0);
    	~DatetimeSetDialog();

	void loadDateTime();
	void appendNum(QString strnum);

	QLineEdit *m_lineEditSel; //当前选中的行编辑控件

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);
    void ReceiveUdpTimeDate(char *buf);
    void CheckUdpState();
    int SetTimeByUdp(int destination);
    int SetTimeByTcp(int destination);
    QTimer *m_pAutoUpdateTimer;
    DateTimeSetThread *m_pDateTimeSetThread;

    Ui::DatetimeSetDialog *ui;

    int m_nGetTimeCnt;
    void emitSigTcpSetTime(int destination);
    void emitSigShowMessage();
signals:
    void sigTcpSetTime(int destination);

private slots:
	void slot_update_timeout();
    void slotTcpSetTime(int destination);

	void on_lineEditSecond_lostFocus();
	void on_lineEditMinute_lostFocus();
	void on_lineEditHour_lostFocus();
	void on_lineEditDay_lostFocus();
	void on_lineEditMonth_lostFocus();
	void on_lineEditYear_lostFocus();
    void on_pushButtonDateTimeSetSave_pressed();
	void on_pushButtonDateTimeSetClear_pressed();
	void on_pushButtonDateTimeSetBack_pressed();
	void on_pushButtonDateTimeSetReturn_pressed();
	void on_pushButtonDateTimeSetNum0_pressed();
	void on_pushButtonDateTimeSetNum9_pressed();
	void on_pushButtonDateTimeSetNum8_pressed();
	void on_pushButtonDateTimeSetNum7_pressed();
	void on_pushButtonDateTimeSetNum6_pressed();
	void on_pushButtonDateTimeSetNum5_pressed();
	void on_pushButtonDateTimeSetNum4_pressed();
	void on_pushButtonDateTimeSetNum3_pressed();
	void on_pushButtonDateTimeSetNum2_pressed();
	void on_pushButtonDateTimeSetNum1_pressed();
	void on_pushButtonDateTimeSetTime_pressed();
};

#endif // DATETIMESETDIALOG_H
