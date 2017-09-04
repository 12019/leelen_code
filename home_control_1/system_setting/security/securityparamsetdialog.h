#ifndef SECURITYPARAMSETDIALOG_H
#define SECURITYPARAMSETDIALOG_H

#include <QDialog>
#include "securityphonesetdialog.h"

namespace Ui 
{
    class SecurityParamSetDialog;
}

class SecurityParamSetDialog : public QDialog
{
    Q_OBJECT

public:
	explicit SecurityParamSetDialog(QWidget *parent = 0);
	~SecurityParamSetDialog();

	SecurityPhoneSetDialog *m_pSecPhoneSetDlg;

	int MessageBox(QString text, QString button_caption);

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
	Ui::SecurityParamSetDialog *ui;

private slots:
    void on_pushButtonSecParaSetDefense_pressed();

    void on_pushButtonSecParaSetSave_pressed();

    void on_pushButtonSecParaSetReturn_pressed();

    void on_pushButtonSecParaSetSendPic_pressed();
};

#endif // SECURITYPARAMSETDIALOG_H
