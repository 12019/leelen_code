#ifndef ADDLAMPDIALOG_H
#define ADDLAMPDIALOG_H

#include <QDialog>

namespace Ui {
    class AddLampDialog;
}

class AddLampDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddLampDialog(QWidget *parent = 0);
    ~AddLampDialog();

	//QDialog *m_curDlg;

	void ResetState();

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
    Ui::AddLampDialog *ui;

private slots:
	void on_pushButtonSave_pressed();
 void on_pushButtonCancel_pressed();

public:
 void on_pushButtonSave_clicked();
void on_pushButtonCancel_clicked();
};

#endif // ADDLAMPDIALOG_H
