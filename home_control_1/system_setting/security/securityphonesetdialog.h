#ifndef SECURITYPHONESETDIALOG_H
#define SECURITYPHONESETDIALOG_H

#include <QDialog>
#include "securityphonesetdialog.h"

namespace Ui {
    class SecurityPhoneSetDialog;
}

class SecurityPhoneSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SecurityPhoneSetDialog(QWidget *parent = 0);
    ~SecurityPhoneSetDialog();

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
    Ui::SecurityPhoneSetDialog *ui;

private slots:
 void on_pushButtonSet_clicked();
 void on_pushButtonNumberAdd_clicked();
 void on_pushButtonNumberSub_clicked();
 void on_pushButtonSecurityParam_clicked();
};

#endif // SECURITYPHONESETDIALOG_H
