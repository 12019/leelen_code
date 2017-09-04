#ifndef TIMEMODE3DIALOG_H
#define TIMEMODE3DIALOG_H

#include <QDialog>

namespace Ui {
    class TimeMode3Dialog;
}

class TimeMode3Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit TimeMode3Dialog(QWidget *parent = 0);
    ~TimeMode3Dialog();

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

private:
    Ui::TimeMode3Dialog *ui;

private slots:
	void on_pushButtonCancel_pressed();
	void on_pushButtonSave_pressed();

public:
	void on_pushButtonCancel_clicked();
	void on_pushButtonSave_clicked();
};

#endif // TIMEMODE3DIALOG_H
