#ifndef CALLELEVATORDIALOG_H
#define CALLELEVATORDIALOG_H

#include <QDialog>

namespace Ui {
    class CallElevatorDialog;
}

class CallElevatorDialog : public QDialog
{
    Q_OBJECT

public:
	explicit CallElevatorDialog(QWidget *parent = 0);
	~CallElevatorDialog();

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
	Ui::CallElevatorDialog *ui;

private slots:
	void on_pushButtonCallElevatorReturn_pressed();
	void on_pushButtonCallElevatorClear_pressed();
	void on_pushButtonCallElevatorElevator_pressed();
	void on_pushButtonCallElevatorNum0_pressed();
	void on_pushButtonCallElevatorNum1_pressed();
	void on_pushButtonCallElevatorNum2_pressed();
	void on_pushButtonCallElevatorNum3_pressed();
	void on_pushButtonCallElevatorNum4_pressed();
	void on_pushButtonCallElevatorNum5_pressed();
	void on_pushButtonCallElevatorNum6_pressed();
	void on_pushButtonCallElevatorNum7_pressed();
	void on_pushButtonCallElevatorNum8_pressed();
	void on_pushButtonCallElevatorNum9_pressed();
	void on_pushButtonCallElevatorDot_pressed();
	void on_pushButtonCallElevatorShop_pressed();

public:
	void on_pushButtonCallElevatorReturn_clicked();
	void on_pushButtonCallElevatorClear_clicked();
	void on_pushButtonCallElevatorElevator_clicked();
	void on_pushButtonCallElevatorNum0_clicked();
	void on_pushButtonCallElevatorNum1_clicked();
	void on_pushButtonCallElevatorNum2_clicked();
	void on_pushButtonCallElevatorNum3_clicked();
	void on_pushButtonCallElevatorNum4_clicked();
	void on_pushButtonCallElevatorNum5_clicked();
	void on_pushButtonCallElevatorNum6_clicked();
	void on_pushButtonCallElevatorNum7_clicked();
	void on_pushButtonCallElevatorNum8_clicked();
	void on_pushButtonCallElevatorNum9_clicked();
	void on_pushButtonCallElevatorDot_clicked();
	void on_CallElevatorShop_clicked();
};

#endif // CALLELEVATORDIALOG_H
