#ifndef SECURITYPLAYERDIALOG_H
#define SECURITYPLAYERDIALOG_H

#include <QDialog>
#include <QFrame>
#include <QTimer>

namespace Ui {
    class SecurityPlayerDialog;
}

class SecurityPlayerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SecurityPlayerDialog(QWidget *parent = 0);
    ~SecurityPlayerDialog();

	void setDetail(QString strAviPath, QString strInfo);

	void startPlayer();
	void emitSigStopPlayer();

	QString m_VideoPath; //报警AVI存储路径

	QFrame *m_pFrameFullshow; //全屏播放控件
	int setDisplayWindow(bool);

	//char *m_pImageBuf; //背景图缓存区
	QTimer *m_pPlayEndTimer; //播放结束后用该定时器触发停止播放操作

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

protected:
	bool eventFilter(QObject *o, QEvent *e);

//private:
public:
    Ui::SecurityPlayerDialog *ui;

signals:
	void sigStopPlayer();

public slots:
	void slotStopPlayer();
	void stopPlayer(bool bResetFocus = true);

private slots:
	void on_pushButtonSecPlayerReturn_pressed();
	void on_pushButtonSecPlayerStop_pressed();
	void on_pushButtonSecPlayerPause_pressed();
	void on_pushButtonSecPlayerPlay_pressed();
};

#endif // SECURITYPLAYERDIALOG_H
