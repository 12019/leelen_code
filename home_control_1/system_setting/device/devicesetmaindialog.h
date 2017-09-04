#ifndef DEVICESETMAINDIALOG_H
#define DEVICESETMAINDIALOG_H

#include <QDialog>
#include "addlampdialog.h"
#include "adddefensedialog.h"

namespace Ui {
    class DeviceSetMainDialog;
}

class DeviceSetMainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceSetMainDialog(QWidget *parent = 0);
    ~DeviceSetMainDialog();

	QDialog *m_pCurDlg; //当前子对话框

	AddLampDialog *m_pAddLampDlg; //电灯设置对话框
	AddDefenseDialog *m_pAddDefenseDlg; //防区设置对话框

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
    Ui::DeviceSetMainDialog *ui;

private slots:
	void on_pushButtonCalibration_pressed();
 void on_pushButtonSystemReset_pressed();
	void on_pushButtonDefense_pressed();
	void on_pushButtonLamp_pressed();

public:
	void on_pushButtonCalibration_clicked();
 void on_pushButtonSystemReset_clicked();
	void on_pushButtonDefense_clicked();
	void on_pushButtonLamp_clicked();
};

#endif // DeviceSetMainDialog_H
