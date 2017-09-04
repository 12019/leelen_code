#ifndef MSTDIALOG_H
#define MSTDIALOG_H

#include <QDialog>
#include <QFrame>

namespace Ui {
    class MstDialog;
}

class MstDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MstDialog(QWidget *parent = 0);
    ~MstDialog();

	bool event(QEvent *event);

	void ResetState();

	void SetTableHeader();

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
    Ui::MstDialog *ui;


private slots:
	//void on_pushButtonMstNext_clicked();
	void on_pushButtonMstPrev_clicked();
void on_pushButtonMstHangUp_clicked();

public slots:
    	void on_pushButtonMstReturn_pressed();
};

#endif // MSTDIALOG_H
