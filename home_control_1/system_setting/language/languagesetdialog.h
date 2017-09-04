#ifndef LANGUAGESETDIALOG_H
#define LANGUAGESETDIALOG_H

#include <QDialog>

namespace Ui {
    class LanguageSetDialog;
}

class LanguageSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LanguageSetDialog(QWidget *parent = 0);
    ~LanguageSetDialog();

	bool m_bIsSettingLanguage; //是否正在设置语言

	//void copyScreenToDesktop();

	int setLanguage(QString sLang);

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
    Ui::LanguageSetDialog *ui;

signals:
	//void sigChangeLanguage(QString sLang);

public slots:
	//void changeLanguage(QString sLang);

private slots:
	void on_pushButtonLagSetTraCh_pressed();
	void on_pushButtonLagSetSimpCh_pressed();
	void on_pushButtonLagSetEn_pressed();
	void on_pushButtonLagSetReturn_pressed();
};

#endif // LANGUAGESETDIALOG_H
