#ifndef SECURITYPASSWORDDIALOG_H
#define SECURITYPASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
    class SecurityPasswordDialog;
}

class SecurityPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SecurityPasswordDialog(QWidget *parent = 0);
    ~SecurityPasswordDialog();

	int m_goingSecurityState;
	int m_flag;

	//flag: 0 - Opened by Abnormal dialog
	//flag: 1 - Opened by Alert dialog
	void resetState(int goingSecurityState, int flag);

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
	Ui::SecurityPasswordDialog *ui;

private slots:
	void on_pushButtonSecPwdShop_pressed();
	void on_pushButtonSecPwdDot_pressed();
	void on_pushButtonSecPwdClear_pressed();
	void on_pushButtonSecPwdReturn_pressed();
	void on_pushButtonSecPwdOK_pressed();
	void on_pushButtonSecPwdBack_pressed();
	void on_pushButtonSecPwdNum0_pressed();
	void on_pushButtonSecPwdNum9_pressed();
	void on_pushButtonSecPwdNum8_pressed();
	void on_pushButtonSecPwdNum7_pressed();
	void on_pushButtonSecPwdNum6_pressed();
	void on_pushButtonSecPwdNum5_pressed();
	void on_pushButtonSecPwdNum4_pressed();
	void on_pushButtonSecPwdNum3_pressed();
	void on_pushButtonSecPwdNum2_pressed();
	void on_pushButtonSecPwdNum1_pressed();
};

#endif // SECURITYPASSWORDDIALOG_H
