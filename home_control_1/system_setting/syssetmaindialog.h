#ifndef SYSSETMAINDIALOG_H
#define SYSSETMAINDIALOG_H

#include <QDialog>
//#include "addresssetdialog.h"
#include "datetimesetdialog.h"
//#include "devicesetmaindialog.h"
#include "adddefensedialog.h"
#include "languagesetdialog.h"
#include "passwordsetdialog.h"
#include "ringandvolumedialog.h"
#include "securityparamsetdialog.h"
#include "versiondialog.h"
#include "commonpushbutton.h"

namespace Ui {
    class SysSetMainDialog;
}

#define CONFIG_FILE "/mnt/disk/config/config"

class SysSetMainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SysSetMainDialog(QWidget *parent = 0);
    ~SysSetMainDialog();

	QDialog *m_curDlg; //当前子对话框

	void ResetState();

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	void deleteCurDialog();
	void hideCurDialog();

	//void setAllButtonsEnabled(bool bEnable);

	//bool m_bCanDelSubDlg;
	//AddressSetDialog *m_pAddressSetDlg;
	LanguageSetDialog *m_pLanguageSetDlg; //语言设置对话框
	PasswordSetDialog *m_pPasswordSetDlg; //系统密码设置对话框
	DatetimeSetDialog *m_pDatetimeSetDlg; //日期时间设置对话框
    RingAndVolumeDialog *m_pRingAndVolumeDlg; //铃声和设音量置对话框
	////lg
	//DeviceSetMainDialog *m_pDeviceSetMainDlg; //设备设置对话框
	AddDefenseDialog *m_pAddDefenseDlg;
	////lg
	SecurityParamSetDialog *m_pSecParamSetDlg; //安防报警参数设置对话框
	VersionDialog *m_pVersionDlg; //版本设置对话框
	//SecurityPhoneSetDialog *m_pSecPhoneSetDlg;

	CommonPushButton *m_pCurSetPushButton;

//private:
	Ui::SysSetMainDialog *ui;

	void on_pushButtonSysSetMainVersion_pressed();
	void Showme();

private slots:
        void on_pushButtonSysSetMainReturn_pressed();
        void on_pushButtonSysSetMainSecurity_pressed();
        void on_pushButtonSysSetMainDevice_pressed();
        void on_pushButtonSysSetMainRing_pressed();
        void on_pushButtonSysSetMainTime_pressed();
        void on_pushButtonSysSetMainPwd_pressed();
        void on_pushButtonSysSetMainLag_pressed();
};

#endif // SYSSETMAINDIALOG_H
