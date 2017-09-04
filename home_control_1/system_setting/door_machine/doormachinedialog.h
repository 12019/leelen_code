#ifndef DOORMACHINEDIALOG_H
#define DOORMACHINEDIALOG_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
    class DoorMachineDialog;
}

class DoorMachineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DoorMachineDialog(QWidget *parent = 0);
    ~DoorMachineDialog();

	int loadData();

	QLineEdit *m_pNameLineEdit[6];
	QLineEdit *m_pNetNameLineEdit[6];

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	int MessageBox(QString text, QString button_caption);

	void pushButtonSave_pressed();
//private:
    Ui::DoorMachineDialog *ui;

public slots:
    //void on_pushButtonDoorMachineSave_pressed();
};

#endif // DOORMACHINEDIALOG_H
