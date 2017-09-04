#ifndef MONLISTDIALOG_H
#define MONLISTDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QModelIndex>

namespace Ui {
    class MonListDialog;
}

class MonListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MonListDialog(QWidget *parent = 0);
    ~MonListDialog();

	int reloadList();

	//QTimer *m_pTimer;

	int m_doorMachineCnt;
	int m_ipCameraCnt;
	int m_wallMachineCnt;

	QStringList m_monListName;
	QStringList m_monListNetName;

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	int get_max_rows();
//private:
    Ui::MonListDialog *ui;

public slots:
	//void slotMonListPressed();

private slots:
    	void on_tableViewMonList_pressed(QModelIndex index);
    	void on_pushButtonMonListCancel_pressed();
};

#endif // MONLISTDIALOG_H
