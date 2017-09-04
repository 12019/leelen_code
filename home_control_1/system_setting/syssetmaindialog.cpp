#include "syssetmaindialog.h"
#include "ui_syssetmaindialog.h"
#include "maindialog.h"
#include "languagesetdialog.h"
#include "datetimesetdialog.h"
#include "passwordsetdialog.h"
#include "ringandvolumedialog.h"
#include "devicesetmaindialog.h"
#include "securityparamsetdialog.h"
#include "ui_maindialog.h"
#include "ui_ringandvolumedialog.h"
#include "ui_adddefensedialog.h"
#include "ui_securityparamsetdialog.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

/************************************************************
  该文件为系统参数设置对话框主程序
************************************************************/

/************************************************************
描述：系统参数设置对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
SysSetMainDialog::SysSetMainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SysSetMainDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

	m_curDlg = NULL;

	m_pLanguageSetDlg = new LanguageSetDialog(this);
	m_pPasswordSetDlg = new PasswordSetDialog(this);
	m_pPasswordSetDlg->set_flag();
	m_pDatetimeSetDlg   = new DatetimeSetDialog(this);
	m_pRingAndVolumeDlg = new RingAndVolumeDialog(this);
	m_pAddDefenseDlg = new AddDefenseDialog(this);
	m_pSecParamSetDlg = new SecurityParamSetDialog(this);
	m_pVersionDlg = new VersionDialog(this);

	int res = g_pScaleSrc->get_screen_version();
	if ( res == 2 )  // 1024x600 
	{
		ui->pushButtonSysSetMainLag->setGeometry(QRect(777, 86, 220, 70));
		ui->pushButtonSysSetMainPwd->setGeometry(QRect(777, 159, 220, 70));
		ui->pushButtonSysSetMainTime->setGeometry(QRect(777, 232, 220, 70));
		ui->pushButtonSysSetMainSecurity->setGeometry(QRect(777, 451, 220, 70));
		ui->pushButtonSysSetMainDevice->setGeometry(QRect(777, 378, 220, 70));
		ui->pushButtonSysSetMainRing->setGeometry(QRect(777, 305, 220, 70));
		ui->pushButtonSysSetMainVersion->setGeometry(QRect(777, 13, 220, 70));
		ui->framePanel->setGeometry(QRect(44, 12, 700, 580));
		ui->pushButtonSysSetMainReturn->setGeometry(QRect(777, 524, 220, 70));
	}
	else if ( res == 3 )  // 1280x800
	{
        ui->pushButtonSysSetMainLag->setGeometry(QRect(962, 121, 270, 86));
        ui->pushButtonSysSetMainPwd->setGeometry(QRect(962, 216, 270, 86));
        ui->pushButtonSysSetMainTime->setGeometry(QRect(962, 311, 270, 86));
        ui->pushButtonSysSetMainSecurity->setGeometry(QRect(962, 596, 270, 86));
        ui->pushButtonSysSetMainDevice->setGeometry(QRect(962, 501, 270, 86));
        ui->pushButtonSysSetMainRing->setGeometry(QRect(962, 406, 270, 86));
        ui->pushButtonSysSetMainVersion->setGeometry(QRect(962, 26, 270, 86));
        ui->framePanel->setGeometry(QRect(59, 30, 858, 745));
        ui->pushButtonSysSetMainReturn->setGeometry(QRect(962, 691, 270, 86));
	}

    m_pCurSetPushButton = NULL;
    on_pushButtonSysSetMainVersion_pressed();
}

/************************************************************
描述：系统参数设置对话框析构函数
参数：无
返回：无
************************************************************/
SysSetMainDialog::~SysSetMainDialog()
{
	delete m_pLanguageSetDlg;
	delete m_pPasswordSetDlg;
	delete m_pDatetimeSetDlg;
	delete m_pRingAndVolumeDlg;

	delete m_pAddDefenseDlg;

	delete m_pSecParamSetDlg;
	delete m_pVersionDlg;

	delete ui;
}

/************************************************************
描述：重置本对话框
参数：无
返回：无
************************************************************/
void SysSetMainDialog::ResetState()
{
	if (m_pCurSetPushButton)
	{
		m_pCurSetPushButton->setFlat(false);
	}

	if (m_curDlg == m_pLanguageSetDlg)
	{
		m_pCurSetPushButton = ui->pushButtonSysSetMainLag;
	}
	else if (m_curDlg == m_pPasswordSetDlg)
	{
		m_pCurSetPushButton = ui->pushButtonSysSetMainPwd;
	}
	else if (m_curDlg == m_pDatetimeSetDlg)
	{
		m_pCurSetPushButton = ui->pushButtonSysSetMainTime;
	}
	else if (m_curDlg == m_pRingAndVolumeDlg)
	{
		m_pCurSetPushButton = ui->pushButtonSysSetMainRing;
		m_pRingAndVolumeDlg->m_pRingFileSetDlg->hide();
		m_pRingAndVolumeDlg->show();
	}
	else if (m_curDlg == m_pAddDefenseDlg) ////lg m_pDeviceSetMainDlg)
	{
		m_pCurSetPushButton = ui->pushButtonSysSetMainDevice;
	}
	else if (m_curDlg == m_pSecParamSetDlg)
	{
		m_pCurSetPushButton = ui->pushButtonSysSetMainSecurity;
	}
	else if (m_curDlg == m_pVersionDlg)
	{
		m_pCurSetPushButton = ui->pushButtonSysSetMainVersion;
	}

	m_pCurSetPushButton->setFocus(Qt::TabFocusReason);
	m_pCurSetPushButton->setFlat(true);
}
/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void SysSetMainDialog::showEvent(QShowEvent *)
{
	Showme();
}
    	
void SysSetMainDialog::Showme()
{
    MainDialog *pMainDlg = (MainDialog *)parent();
    pMainDlg->showRefreshTabFocus(this);
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void SysSetMainDialog::hideEvent(QHideEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)parent();
	pMainDlg->hideRefreshTabFocus(this);

    //lg 12.22
    on_pushButtonSysSetMainVersion_pressed();
    //lg

	if (pMainDlg->m_pCurActiveDlg == pMainDlg)
	{
		pMainDlg->ui->pushButtonSystemSetting->setFocus(Qt::TabFocusReason);
	}
}

/************************************************************
描述：删除当前子对话框
参数：无
返回：无
************************************************************/
void SysSetMainDialog::deleteCurDialog()
{
	if (m_curDlg)
	{
		m_curDlg->hide();
		delete m_curDlg;
		m_curDlg = NULL;
	}
}

/************************************************************
描述：隐藏当前子对话框
参数：无
返回：无
************************************************************/
void SysSetMainDialog::hideCurDialog()
{
	if (m_curDlg) m_curDlg->hide();

	if (m_pCurSetPushButton)
	{
		m_pCurSetPushButton->setFlat(false);
	}

	m_pRingAndVolumeDlg->m_pRingFileSetDlg->hide();

	m_pSecParamSetDlg->m_pSecPhoneSetDlg->hide();

	m_curDlg = NULL;
}

/************************************************************
描述：打开语言设置子对话框
参数：无
返回：无
************************************************************/
void SysSetMainDialog::on_pushButtonSysSetMainLag_pressed()
{
	hideCurDialog();
	m_pLanguageSetDlg->show();
	m_curDlg = m_pLanguageSetDlg;
	m_pCurSetPushButton = ui->pushButtonSysSetMainLag;
	m_pCurSetPushButton->setFlat(true);
}

/************************************************************
描述：打开系统密码设置子对话框
参数：无
返回：无
************************************************************/
void SysSetMainDialog::on_pushButtonSysSetMainPwd_pressed()
{
	/*deleteCurDialog();
	m_curDlg = (QDialog *)new PasswordSetDialog(this);
	m_curDlg->show();
	*/
	hideCurDialog();
	m_pPasswordSetDlg->show();
//	ui->pushButtonSettingReturn->hide();

	m_curDlg = m_pPasswordSetDlg;
	m_pCurSetPushButton = ui->pushButtonSysSetMainPwd;
	m_pCurSetPushButton->setFlat(true);
}

/************************************************************
描述：打开日期时间设置子对话框
参数：无
返回：无
************************************************************/
void SysSetMainDialog::on_pushButtonSysSetMainTime_pressed()
{
	/*deleteCurDialog();

	m_curDlg = (QDialog *)new DatetimeSetDialog(this);
	m_curDlg->show();
	((DatetimeSetDialog *)m_curDlg)->loadDateTime();
	*/
	hideCurDialog();
	//m_pDatetimeSetDlg->loadDateTime();
	m_pDatetimeSetDlg->show();

	m_curDlg = m_pDatetimeSetDlg;
	m_pCurSetPushButton = ui->pushButtonSysSetMainTime;
	m_pCurSetPushButton->setFlat(true);
}

/************************************************************
描述：打开铃声和音量设置子对话框
参数：无
返回：无
************************************************************/
void SysSetMainDialog::on_pushButtonSysSetMainRing_pressed()
{
	if (m_curDlg == m_pRingAndVolumeDlg) return;

	hideCurDialog();

    	m_pRingAndVolumeDlg->m_pRingFileSetDlg->m_pBackFromSave = false;

	m_pRingAndVolumeDlg->show();
	m_curDlg = m_pRingAndVolumeDlg;
	m_pCurSetPushButton = ui->pushButtonSysSetMainRing;
	m_pCurSetPushButton->setFlat(true);
}

/************************************************************
描述：打开设备参数设置子对话框
参数：无
返回：无
************************************************************/
void SysSetMainDialog::on_pushButtonSysSetMainDevice_pressed()
{
	/*
	deleteCurDialog();

	m_curDlg = (QDialog *)new DeviceSetMainDialog(this);
	m_curDlg->show();
	*/
	if (m_curDlg == m_pAddDefenseDlg) ////lg
	{
		return;
	}

	hideCurDialog();
	m_pAddDefenseDlg->show(); ////lg
	m_curDlg = m_pAddDefenseDlg; ////lg
	m_pCurSetPushButton = ui->pushButtonSysSetMainDevice;
	m_pCurSetPushButton->setFlat(true);
}

/************************************************************
描述：打开安防报警参数设置子对话框
参数：无
返回：无
************************************************************/
void SysSetMainDialog::on_pushButtonSysSetMainSecurity_pressed()
{
    //ui->pushButtonSysSetMainReturn->raise();
	/*
	deleteCurDialog();

	m_curDlg = (QDialog *)new SecurityParamSetDialog(this);
	m_curDlg->show();
	*/
	hideCurDialog();
	m_pSecParamSetDlg->show();
	m_curDlg = m_pSecParamSetDlg;
	m_pCurSetPushButton = ui->pushButtonSysSetMainSecurity;
	m_pCurSetPushButton->setFlat(true);
}

/************************************************************
描述：打开版本设置子对话框
参数：无
返回：无
************************************************************/
void SysSetMainDialog::on_pushButtonSysSetMainVersion_pressed()
{
	hideCurDialog();
	m_pVersionDlg->show();
	m_curDlg = m_pVersionDlg;
	m_pCurSetPushButton = ui->pushButtonSysSetMainVersion;
	m_pCurSetPushButton->setFlat(true);
}

/************************************************************
描述：关闭本对话框
参数：无
返回：无
************************************************************/
void SysSetMainDialog::on_pushButtonSysSetMainReturn_pressed()
{
	hide();
}
