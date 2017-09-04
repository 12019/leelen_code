#ifndef ADDRESSSETDIALOG_H
#define ADDRESSSETDIALOG_H

#include <QDialog>
#include <QRegExp>

namespace Ui {
    class AddressSetDialog;
}

class AddressSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddressSetDialog(QWidget *parent = 0);
    ~AddressSetDialog();

	int loadData();

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	int MessageBox(QString text, QString button_caption);

//private:
    	Ui::AddressSetDialog *ui;
signals:
        void sigIpHasChange();
private:
	void Del_Space(QString&);

public slots:
 	void pushButtonSave_pressed();

private slots:
};

#endif // ADDRESSSETDIALOG_H
