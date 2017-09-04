#ifndef PASSWORDSETDIALOG_H
#define PASSWORDSETDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QMessageBox>

namespace Ui {
    class PasswordSetDialog;
}

class PasswordSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordSetDialog(QWidget *parent = 0);
    ~PasswordSetDialog();

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	void set_flag();

//private:
	Ui::PasswordSetDialog *ui;

	QLineEdit *m_pLineEditSel; //当前行编辑控件

private slots:
	void on_lineEditCheckPassword_lostFocus();
	void on_lineEditPassword_lostFocus();
	void on_pushButtonPwdSetClear_pressed();
	void on_pushButtonPwdSetBack_pressed();
	void on_pushButtonPwdSetReturn_pressed();

	void on_pushButtonPwdSetDot_pressed();
	void on_pushButtonPwdSetShop_pressed();
	void on_pushButtonPwdSetNum9_pressed();
	void on_pushButtonPwdSetNum8_pressed();
	void on_pushButtonPwdSetNum7_pressed();
	void on_pushButtonPwdSetNum6_pressed();
	void on_pushButtonPwdSetNum5_pressed();
	void on_pushButtonPwdSetNum4_pressed();
	void on_pushButtonPwdSetNum3_pressed();
	void on_pushButtonPwdSetNum2_pressed();
	void on_pushButtonPwdSetNum1_pressed();
	void on_pushButtonPwdSetNum0_pressed();
	void on_pushButtonPwdSetSave_pressed();
	void on_lineEditPassword_textChanged(const QString &arg1);
private:
	bool                 m_issysmaindialog;
};

#endif // PASSWORDSETDIALOG_H
