#ifndef MEDIAPLAYERDIALOG_H
#define MEDIAPLAYERDIALOG_H

#include <QDialog>
#include <QFrame>
#include <QTimer>

#include "mediastreamer2/mediastream.h"

namespace Ui {
	class MediaPlayerDialog;
}

class MediaPlayerDialog : public QDialog
{
    Q_OBJECT

public:
	explicit MediaPlayerDialog(QWidget *parent);

	~MediaPlayerDialog();

	void setDetail(QString address, QString netAddress, QString aviurl);

	void startPlayer();
	void emitSigStopPlayer();

	QFrame *m_pFrameFullshow; //全屏播放控件
	int setDisplayWindow(bool);

	//int m_bStopi;
	QString m_VideoPath; //播放的AVI文件路径
	//ReadAVI *m_pReadAVI;

	QString m_address; //对讲对方网络名
	QString m_netAddress;

	////char *m_pImageBuf; //背景图缓存区
	QTimer *m_pPlayEndTimer; //播放结束后用该定时器触发停止播放操作

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
	Ui::MediaPlayerDialog *ui;

protected:
	bool eventFilter(QObject *o, QEvent *e);

signals:
	void sigStopPlayer();

public slots:
	void slotStopPlayer();
	void stopPlayer(bool bResetFocus = true);

	void on_pushButtonMediaPlayerPause_pressed();
	void on_pushButtonMediaPlayerStop_pressed();
	void on_pushButtonMediaPlayerCall_pressed();
	void on_pushButtonMediaPlayerPlay_pressed();
	void on_pushButtonMediaPlayerReturn_pressed();
};

#endif // MEDIAPLAYERDIALOG_H
