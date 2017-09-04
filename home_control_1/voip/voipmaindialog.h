#ifndef VOIPMAINDIALOG_H
#define VOIPMAINDIALOG_H

#include <QDialog>
#include "ictdialog.h"
#include "istdialog.h"
//#include "mctdialog.h"
#include "mondialog.h"
#include "mstdialog.h"
#include "getpeeripthread.h"
#include "sendimgthread.h"
#include "mymessagebox.h"

#include "voip/callinfouploadthread.h"
#include "keepwatchlivethread.h"
#include "authorizeelevatorthread.h"


namespace Ui {
    class VoipMainDialog;
}

class VoipMainDialog : public QDialog
{
    Q_OBJECT

public:
	    explicit VoipMainDialog(QWidget *parent = 0);
	    ~VoipMainDialog();

	    bool event(QEvent *event);

	    IctDialog *m_pIctDlg; //拨号通话对话框
	    IstDialog *m_pIstDlg; //来电接听对话框
	    //MctDialog *m_pMctDlg;
	    MonDialog *m_pMonDlg; //视频监控对话框
	    MstDialog *m_pMstDlg;

	    bool m_bCallCenter; //是否来电接听管理机电话

	    GetPeerIPThread *m_pGetPeerIPThread; //把网络名称转换成IP地址的线程
        AuthorizeElevatorThread * m_pAuthorizeElevatorThread;//发送户户互访梯控信息线程

	    QTimer *m_pTimerOpen; //开门定时器

	    bool m_bVideoOpen; //是否打开本地视频
	    unsigned int m_nRingVolume; //铃声音量
	    unsigned int m_nVolume; //通话音量

	    void closeOtherRelatedDialog();
	    void closeVoipDialog();

        void authorizeElevator(QString strRemoteName);

	    void showEvent(QShowEvent *);
	    void hideEvent(QHideEvent *);

	    //david
	    SendImgThread  *m_pSendImgThread;
	    CallInfoUploadThread *m_pCallInfoUploadThread;

	    KeepWatchLiveThread *m_pKeepWatchLiveThread;

	    bool Do_Door_Bell_Event(QString& str);  // 处理正在通话时候的门铃事件
private:
	    Ui::VoipMainDialog *ui;

	    public slots:
		    void openTimerTimeout();

	    private slots:
	    void on_pushButtonVoipMainDial_pressed();
	    void on_pushButtonVoipMainMon_pressed();
	    void on_pushButtonVoipMainReturn_pressed();
};

#endif // VOIPMAINDIALOG_H
