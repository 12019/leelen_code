#ifndef MCTDIALOG_H
#define MCTDIALOG_H

#include <QDialog>
#include <QtGui/QLineEdit>
#include "mcthookdialog.h"

namespace Ui {
    class MctDialog;
}

class MctDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MctDialog(QWidget *parent = 0);
    ~MctDialog();

	bool event(QEvent *event);

	void ResetState();

	QString m_strDialIpAddress;
	int m_videoChannel;

	MctHookDialog *m_pHookDlg;

	bool m_bHookCloseMctDlg;

private:
    Ui::MctDialog *ui;

private slots:
	void on_pushButtonHelp_clicked();
 void on_pushButtonEmergency_clicked();
 void on_pushButtonCallCenter_clicked();
 void on_pushButton_num_0_clicked();
	void on_pushButton_num_1_clicked();
	void on_pushButton_num_2_clicked();
	void on_pushButton_num_3_clicked();
	void on_pushButton_num_4_clicked();
	void on_pushButton_num_5_clicked();
	void on_pushButton_num_6_clicked();
	void on_pushButton_num_7_clicked();
	void on_pushButton_num_8_clicked();
	void on_pushButton_num_9_clicked();
	void on_pushButton_dot_clicked();
	void on_pushButton_shop_clicked();
	void on_pushButtonReturn_clicked();
	void on_pushButtonClear_clicked();
	void on_pushButtonDial_clicked();
};

#endif // MCTDIALOG_H
