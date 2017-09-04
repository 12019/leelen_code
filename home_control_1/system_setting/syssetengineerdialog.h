#ifndef SYSSETENGINEERDIALOG_H
#define SYSSETENGINEERDIALOG_H

#include <QDialog>
#include "addresssetdialog.h"
#include "doormachinedialog.h"
#include "ipcameradialog.h"
#include "otherdialog.h"
#include "passwordsetdialog.h"
#include "commonpushbutton.h"
#include "remotecheck/remotecheckdialog.h"

#define CONFIG_FILE "/mnt/disk/config/config"

namespace Ui {
    class SysSetEngineerDialog;
}

class SysSetEngineerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SysSetEngineerDialog(QWidget *parent = 0);
    ~SysSetEngineerDialog();

	AddressSetDialog *m_pAddressSetDlg; //网络地址设置对话框
	DoorMachineDialog *m_pDoorMachineDlg;
    IpCameraDialog *m_pIpCameraDlg;
    OtherDialog *m_pOtherDlg;		  // 二次铃声的设置对话框

    PasswordSetDialog *m_pPasswordSetDlg; //工程密码设置对话框

    RemoteCheckDialog *m_pRemoteCheckDlg; //david 2014.5.23
	QDialog *m_curDlg; //当前子对话框

	void ResetState();

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	void hideCurDialog();

	CommonPushButton *m_pCurSetPushButton;

	void showme();

//private:
    Ui::SysSetEngineerDialog *ui;

    public slots:
	    void on_pushButtonSysSetEngineerDoorMachine_pressed();
	    void on_pushButtonSysSetEngineerReturn_pressed();
	    void on_pushButtonSysSetEngineerPwd_pressed();
	    void on_pushButtonSysSetEngineerNetAddress_pressed();
	    void on_pushButtonSysSetEngineerSave_pressed();
    private slots:
        void on_pushButtonSysSetEngineerRemoteCheck_pressed();
        void on_pushButtonSysSetEngineerIpCamera_pressed();
        void on_pushButtonSysSetEngineerOther_pressed();
};

#endif // SYSSETENGINEERDIALOG_H
