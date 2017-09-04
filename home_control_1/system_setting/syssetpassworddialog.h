#ifndef SYSSETPASSWORDDIALOG_H
#define SYSSETPASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
    class SysSetPasswordDialog;
}

class SysSetPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SysSetPasswordDialog(QWidget *parent = 0);
    ~SysSetPasswordDialog();

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
    Ui::SysSetPasswordDialog *ui;

private slots:
	void on_pushButtonSysPwdShop_pressed();
	void on_pushButtonSysPwdDot_pressed();
	void on_pushButtonSysPwdClear_pressed();
	void on_pushButtonSysPwdBack_pressed();
	void on_pushButtonSysPwdNum9_pressed();
	void on_pushButtonSysPwdNum8_pressed();
	void on_pushButtonSysPwdNum7_pressed();
	void on_pushButtonSysPwdNum6_pressed();
	void on_pushButtonSysPwdNum5_pressed();
	void on_pushButtonSysPwdNum4_pressed();
	void on_pushButtonSysPwdNum3_pressed();
	void on_pushButtonSysPwdNum2_pressed();
	void on_pushButtonSysPwdNum1_pressed();
	void on_pushButtonSysPwdNum0_pressed();
	void on_pushButtonSysPwdOK_pressed();
	void on_pushButtonSysPwdReturn_pressed();
};

#endif // SYSSETPASSWORDDIALOG_H
