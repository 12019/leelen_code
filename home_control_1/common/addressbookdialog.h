#ifndef ADDRESSBOOKDIALOG_H
#define ADDRESSBOOKDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QModelIndex>

namespace Ui {
	class AddressBookDialog;
}

class AddressBookDialog : public QDialog
{
    Q_OBJECT

public:
	explicit AddressBookDialog(QWidget *parent = 0);
	~AddressBookDialog();

	int reloadList();

	QStringList m_addressListName;
	QStringList m_addressListNetName;

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	QTimer *m_pTimer;

//private:
	Ui::AddressBookDialog *ui;

public slots:
	void slotAddressListPressed();

private slots:
	void on_tableViewAddressList_pressed(QModelIndex index);

public slots:
 	void on_pushButtonAddressBookCancel_pressed();
};

#endif // ADDRESSBOOKDIALOG_H
