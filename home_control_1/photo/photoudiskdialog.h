#ifndef PHOTOUDISKDIALOG_H
#define PHOTOUDISKDIALOG_H

#include <QDialog>
#include <QFrame>
#include <QLabel>

namespace Ui {
    class PhotoUDiskDialog;
}

class PhotoUDiskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PhotoUDiskDialog(QWidget *parent = 0);
    ~PhotoUDiskDialog();

	QString m_curDir;
	QFrame *m_pFrameFullshow; //全屏播放控件
	QLabel *m_pPicLabel;

	bool load(QString strDir);
	//bool copyDir(char *source, char *destination);
	void translateTableViewPhotoList();

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
    Ui::PhotoUDiskDialog *ui;

protected:
	bool eventFilter(QObject *o, QEvent *e);

private slots:
	void on_pushButtonPhotoUDiskImport_pressed();
	void on_pushButtonPhotoUDiskNext_pressed();
	void on_pushButtonPhotoUDiskPrev_pressed();
	void on_pushButtonPhotoUDiskCheck_pressed();

public slots:
    void on_pushButtonPhotoUDiskReturn_pressed();
};

#define OVERWRITE_NORMAL		0
#define OVERWRITE_YES			1
#define OVERWRITE_NO			2

extern int copy_overwrite;
bool copyDir(QDialog *pDlg, char *source, char *destination, char *dest_base);

#endif // PHOTOUDISKDIALOG_H
