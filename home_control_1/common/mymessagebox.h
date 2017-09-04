#ifndef MYMESSAGEBOX_H
#define MYMESSAGEBOX_H

#include <QDialog>
#include "commonpushbutton.h"
#include <QLabel>
//#include <QMessageBox>
#include <QFrame>

class MyMessageBox : public QDialog
{
	Q_OBJECT
public:
	explicit MyMessageBox(QWidget *parent = 0);
	~MyMessageBox();

	void setText(const QString &text);
	CommonPushButton *addButton(const QString &text, const QString objectName = "");

	QLabel *m_pLabel;
	QFrame *m_pFrame;
	QList <QPushButton *> m_pushButtonList;
	QPushButton *m_pClickedButton;

	void updateSize();

	QAbstractButton *clickedButton() const;

	QPushButton *clickedButton();

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	QTimer *m_pAutoCloseTimer;   // 自动关闭对话框
	void Start_Auto_Close();
signals:

public slots:
	void on_buttonClicked();
	void AutoClose(); 	   // 自动关闭该对话框
};

#endif // MYMESSAGEBOX_H
