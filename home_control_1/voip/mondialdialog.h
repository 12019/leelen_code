#ifndef MONDIALDIALOG_H
#define MONDIALDIALOG_H

#include <QDialog>

namespace Ui {
    class MonDialDialog;
}

class MonDialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MonDialDialog(QWidget *parent = 0);
    ~MonDialDialog();

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
    Ui::MonDialDialog *ui;

private slots:
	void on_pushButtonReturn_pressed();
 void on_pushButtonClear_pressed();
 void on_pushButtonDial_pressed();
 void on_pushButton_num_0_pressed();
	void on_pushButton_num_1_pressed();
	void on_pushButton_num_2_pressed();
	void on_pushButton_num_3_pressed();
	void on_pushButton_num_4_pressed();
	void on_pushButton_num_5_pressed();
	void on_pushButton_num_6_pressed();
	void on_pushButton_num_7_pressed();
	void on_pushButton_num_8_pressed();
	void on_pushButton_num_9_pressed();
	void on_pushButton_dot_pressed();
	void on_pushButton_shop_pressed();

public:
	void on_pushButtonReturn_clicked();
 void on_pushButtonClear_clicked();
 void on_pushButtonDial_clicked();
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
};

#endif // MONDIALDIALOG_H
