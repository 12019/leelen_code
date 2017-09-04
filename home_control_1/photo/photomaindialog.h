#ifndef PHOTOMAINDIALOG_H
#define PHOTOMAINDIALOG_H

#include <QDialog>
#include "photoudiskdialog.h"

namespace Ui {
    class PhotoMainDialog;
}

class PhotoMainDialog : public QDialog
{
Q_OBJECT

public:
	explicit PhotoMainDialog(QWidget *parent = 0);
	~PhotoMainDialog();

	QString m_curDir;
	QFrame *m_pFrameFullshow; //全屏播放控件
	QLabel *m_pPicLabel;

	QTimer *m_pTimer;

	int m_nextPlayRow;

	PhotoUDiskDialog *m_pPhotoUDiskDlg;

	bool load(QString strDir);

	bool deleteDir(const QString dirName);

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	bool eventFilter(QObject *o, QEvent *e);

	//private:
	Ui::PhotoMainDialog *ui;

public slots:
	void translateTableViewPhotoList();
	void slotPlayTimeout();

private slots:
	void on_pushButtonPhotoMainExport_pressed();
	void on_pushButtonPhotoMainDel_pressed();
	void on_pushButtonPhotoMainPrev_pressed();
	void on_pushButtonPhotoMainNext_pressed();
	void on_pushButtonPhotoMainCheck_pressed();
	void on_pushButtonPhotoMainReturn_pressed();
	void on_pushButtonPhotoMainImport_pressed();
};

#endif // PHOTOMAINDIALOG_H
