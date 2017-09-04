#ifndef TIMEMODE2DIALOG_H
#define TIMEMODE2DIALOG_H

#include <QDialog>

namespace Ui {
	class TimeMode2Dialog;
}

class TimeMode2Dialog : public QDialog
{
    Q_OBJECT

public:
	explicit TimeMode2Dialog(QWidget *parent = 0);
	~TimeMode2Dialog();

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

private:
	Ui::TimeMode2Dialog *ui;

private slots:
	void on_pushButtonCancel_pressed();
	void on_pushButtonSave_pressed();

public:
	void on_pushButtonCancel_clicked();
	void on_pushButtonSave_clicked();
};

#endif // TIMEMODE2DIALOG_H
