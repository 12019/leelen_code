#ifndef TERMINALHELPMAINDIALOG_H
#define TERMINALHELPMAINDIALOG_H

#include <QDialog>
#include <QtGui>
#include <Qt/qtcpsocket.h>
#include "terminalhelpthread.h"

class QListView;
class QStringListModel;
class QMessageBox;

namespace Ui {
	class TerminalHelpMainDialog;
}

class TerminalHelpMainDialog : public QDialog
{
    Q_OBJECT

public:
	explicit TerminalHelpMainDialog(QWidget *parent = 0);
	~TerminalHelpMainDialog();

	QStringListModel *model;
	QStringList data;

	TerminalHelpThread *m_pThread;

	static TerminalHelpMainDialog *m_pDlg;
	static void showTerminalHelpDialog();

//private:
	Ui::TerminalHelpMainDialog *ui;

public slots:
	void sendHelp();
	void on_pushButtonEmergency_clicked();

public:
	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

private slots:
	void on_pushButtonHelp_clicked();
 void on_pushButtonCallCenter_clicked();
 void on_pushButtonReturn_clicked();
};

#endif // TERMINALHELPMAINDIALOG_H
