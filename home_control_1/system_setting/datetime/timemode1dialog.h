#ifndef TIMEMODE1DIALOG_H
#define TIMEMODE1DIALOG_H

#include <QDialog>
#include "clock.h"

namespace Ui {
	class TimeMode1Dialog;
}

class TimeMode1Dialog : public QDialog
{
    Q_OBJECT

public:
	explicit TimeMode1Dialog(QWidget *parent = 0);
	~TimeMode1Dialog();

	Clock *getClock();

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

private:
	Ui::TimeMode1Dialog *ui;

private slots:
	void on_pushButtonCancel_pressed();
	void on_pushButtonSave_pressed();

public:
	void on_pushButtonCancel_clicked();
	void on_pushButtonSave_clicked();
};

#endif // TIMEMODE1DIALOG_H
