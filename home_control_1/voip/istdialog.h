#ifndef ISTDIALOG_H
#define ISTDIALOG_H

#include <QDialog>
#include <QFrame>

#include "jpgsavetestthread.h"

namespace Ui {
    class IstDialog;
}

class IstDialog : public QDialog
{
    Q_OBJECT

public:
	    explicit IstDialog(QWidget *parent = 0);
	    ~IstDialog();

	    bool m_bExtensionUnitAnswer;
	    int m_iExtensionUnitAddr;

	    bool m_bTalking; //是否处于通话状态
	    QString m_strRemoteNetName; //远端机网络名称
	    QString m_strRemoteFullNetName;

	    int m_iModalType; //远端机模型类型

	    char m_strRemoteIp[50]; //远端机IP地址

	    void ResetState();

	    bool event(QEvent *event);

	    QFrame *m_pFrameFullshow; //全屏播放控件

	    int setDisplayWindow(bool);

	    void showEvent(QShowEvent *);
	    void hideEvent(QHideEvent *);

	    JpgSaveTestThread *m_pJpgSaveTestThread;

	    QTimer *m_pAutoAnswerTimer;

	    QTimer *m_pAutoHungupTimer;

	    QTimer *m_pFilterTimer;//david 2014/10/18

	    QTimer *m_pTimeOutAutoHungupTimer; // 在留言的时候，如果超时就自动挂断45S

	    //private:
	    Ui::IstDialog *ui;

	    void Door_Bell_Ring_Event(QString&);
	    void Exit_From_Full_Screen();
protected:
	    bool eventFilter(QObject *o, QEvent *e);

	    virtual void keyPressEvent(QKeyEvent *k);
public slots:
	    void on_pushButtonIstOpen_pressed();
	    void on_pushButtonIstVideoOpen_pressed();
	    void on_pushButtonIstVideoClose_pressed();
	    void on_pushButtonIstHangUp_pressed();
	    void on_pushButtonIstAnswer_pressed();

private slots:
	    void Do_Record_Timeout_Hungup();
};

#endif // ISTDIALOG_H
