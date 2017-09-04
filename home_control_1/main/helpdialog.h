#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>

namespace Ui {
    class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = 0);
    ~HelpDialog();

	QLineEdit *getLineEditHelpTile();
	QTextEdit *getTextEditHelp();

	//static HelpDialog *CreateHelpDialog(QDialog *pparent, QString help_file_name);

	static void showHelpDialog(QDialog *pCurDlg);//QString help_file_name);
	static HelpDialog *m_pDlg; //帮助对话框指针

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
    Ui::HelpDialog *ui;

private slots:
	void on_pushButtonReturn_pressed();
};

#endif // HELPDIALOG_H
