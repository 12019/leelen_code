#ifndef SECURITYABNORMALDIALOG_H
#define SECURITYABNORMALDIALOG_H

#include <QDialog>

namespace Ui {
    class SecurityAbnormalDialog;
}

class SecurityAbnormalDialog : public QDialog
{
    Q_OBJECT

public:
    	explicit SecurityAbnormalDialog(QWidget *parent = 0);
    	~SecurityAbnormalDialog();

	void resetState();

	void getAllDefenseStatus(int *pAllDefenseStatus);
	void getRegisteredDefenseProperty(int *pAllDefenseProperty);

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
	Ui::SecurityAbnormalDialog *ui;

public slots:
	void on_pushButtonSecAbnormalDisArm_pressed();
	void on_pushButtonSecAbnormalHome_pressed();
	void on_pushButtonSecAbnormalOut_pressed();
	void on_pushButtonSecAbnormalReturn_pressed();
	void on_pushButtonSecAbnormalNext_pressed();
	void on_pushButtonSecAbnormalPrev_pressed();
};

#endif // SECURITYABNORMALDIALOG_H
