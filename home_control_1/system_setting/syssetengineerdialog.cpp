#include "syssetengineerdialog.h"
#include "ui_syssetengineerdialog.h"
#include "maindialog.h"
#include "ui_maindialog.h"
#include "mymessagebox.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
/************************************************************
  该文件为工程参数设置对话框主程序
************************************************************/

#define OFFSET		(-10)

extern int g_HomeControlSetShow;
/************************************************************
描述：工程参数设置对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
SysSetEngineerDialog::SysSetEngineerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SysSetEngineerDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

	int res = g_pScaleSrc->get_screen_version();
	if ( res == 2 )  	// 1024x600 
	{
		ui->framePanel->setGeometry(QRect(44, 12, 700, 580));
		ui->pushButtonSysSetEngineerNetAddress->setGeometry(QRect(764, 20, 230, 80));
		ui->pushButtonSysSetEngineerDoorMachine->setGeometry(QRect(764, 100, 230, 80));
		ui->pushButtonSysSetEngineerIpCamera->setGeometry(QRect(764, 180, 230, 80));
		ui->pushButtonSysSetEngineerRemoteCheck->setGeometry(QRect(764, 180, 230, 80));
		ui->pushButtonSysSetEngineerPwd->setGeometry(QRect(764, 260, 230, 80));
		ui->pushButtonSysSetEngineerOther->setGeometry(QRect(764, 340, 230, 80));
		ui->pushButtonSysSetEngineerSave->setGeometry(QRect(764, 420, 230, 80));
		ui->pushButtonSysSetEngineerReturn->setGeometry(QRect(764, 500, 230, 80));
    	}
	else if ( res == 3 )  // 1280x800
	{
		ui->framePanel->setGeometry(QRect(59, 30, 858, 745));
		ui->pushButtonSysSetEngineerNetAddress->setGeometry(QRect(970, 50, 246, 112));
		ui->pushButtonSysSetEngineerDoorMachine->setGeometry(QRect(970, 150, 246, 112));
		ui->pushButtonSysSetEngineerIpCamera->setGeometry(QRect(970, 250, 246, 112));
		ui->pushButtonSysSetEngineerRemoteCheck->setGeometry(QRect(970, 250, 246, 112));
		ui->pushButtonSysSetEngineerPwd->setGeometry(QRect(970, 350, 246, 112));
		ui->pushButtonSysSetEngineerOther->setGeometry(QRect(970, 450, 246, 112));
		ui->pushButtonSysSetEngineerSave->setGeometry(QRect(970, 550, 246, 112));
		ui->pushButtonSysSetEngineerReturn->setGeometry(QRect(970, 650, 246, 112));
	}

	m_curDlg = NULL;

	m_pAddressSetDlg  = new AddressSetDialog(this);
	m_pDoorMachineDlg = new DoorMachineDialog(this);
	m_pIpCameraDlg    = new IpCameraDialog(this);
	m_pPasswordSetDlg = new PasswordSetDialog(this);
	m_pRemoteCheckDlg = new RemoteCheckDialog(this);
	m_pOtherDlg       = new OtherDialog(this);

	m_pCurSetPushButton = NULL;
	on_pushButtonSysSetEngineerNetAddress_pressed();
}

/************************************************************
描述：工程参数设置对话框析构函数
参数：无
返回：无
************************************************************/
SysSetEngineerDialog::~SysSetEngineerDialog()
{
	delete m_pAddressSetDlg;
	delete m_pDoorMachineDlg;
	delete m_pIpCameraDlg;
	delete m_pPasswordSetDlg;
	delete m_pRemoteCheckDlg;
	delete m_pOtherDlg;
	delete ui;
}

/************************************************************
描述：重置本对话框
参数：无
返回：无
************************************************************/
void SysSetEngineerDialog::ResetState()
{
	if (m_pCurSetPushButton)
	{
		m_pCurSetPushButton->setFlat(false);
	}

	//on_pushButtonAddressSet_clicked();
	if (m_curDlg == m_pAddressSetDlg)
	{
        	m_pCurSetPushButton = ui->pushButtonSysSetEngineerNetAddress;
	}
	else if (m_curDlg == m_pDoorMachineDlg)
	{
        	m_pCurSetPushButton = ui->pushButtonSysSetEngineerDoorMachine;
	}
	else if (m_curDlg == m_pIpCameraDlg)
	{
		m_pCurSetPushButton = ui->pushButtonSysSetEngineerIpCamera;
	}
	else if (m_curDlg == m_pPasswordSetDlg)
	{
        	m_pCurSetPushButton = ui->pushButtonSysSetEngineerPwd;
	} //david 2014.5.23
	else if(m_curDlg == m_pRemoteCheckDlg)
	{
        	m_pCurSetPushButton = ui->pushButtonSysSetEngineerRemoteCheck;
	}

	m_pCurSetPushButton->setFocus(Qt::TabFocusReason);
	m_pCurSetPushButton->setFlat(true);
}

/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void SysSetEngineerDialog::showEvent(QShowEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)parent();
	pMainDlg->showRefreshTabFocus(this);

	if(g_HomeControlSetShow)
	{
        	ui->pushButtonSysSetEngineerRemoteCheck->show();
	}
	else
	{
        	ui->pushButtonSysSetEngineerRemoteCheck->hide();
	}
}
	
/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void SysSetEngineerDialog::hideEvent(QHideEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)parent();
	pMainDlg->hideRefreshTabFocus(this);

    //lg 12.22
    on_pushButtonSysSetEngineerNetAddress_pressed();
    //lg

    if (pMainDlg->m_pCurActiveDlg == pMainDlg)
	{
		pMainDlg->ui->pushButtonSystemSetting->setFocus(Qt::TabFocusReason);
	}
}

/************************************************************
描述：隐藏当前子对话框
参数：无
返回：无
************************************************************/
void SysSetEngineerDialog::hideCurDialog()
{
	if (m_curDlg) m_curDlg->hide();

	if (m_pCurSetPushButton)
	{
		m_pCurSetPushButton->setFlat(false);
	}

	m_curDlg = NULL;
}

/************************************************************
描述：打开网络地址设置子对话框
参数：无
返回：无
************************************************************/
void SysSetEngineerDialog::on_pushButtonSysSetEngineerNetAddress_pressed()
{
	hideCurDialog();
	//m_pAddressSetDlg->loadData();
	m_pAddressSetDlg->show();
	m_curDlg = m_pAddressSetDlg;

	m_pCurSetPushButton = ui->pushButtonSysSetEngineerNetAddress;
	m_pCurSetPushButton->setFlat(true);
}

/************************************************************
描述：将行编辑控件上修改后的本机网络地址信息存入config文件中
参数：无
返回：无
************************************************************/
void SysSetEngineerDialog::on_pushButtonSysSetEngineerSave_pressed()
{
	if(m_curDlg==m_pAddressSetDlg)
	{
		m_pAddressSetDlg->pushButtonSave_pressed();
	}
    	else if (m_curDlg == m_pDoorMachineDlg)
	{
		m_pDoorMachineDlg->pushButtonSave_pressed();
	}
	else if (m_curDlg == m_pIpCameraDlg)
	{
		m_pIpCameraDlg->pushButtonSave_pressed();
	}
	else if (m_curDlg == m_pOtherDlg)
	{
		m_pOtherDlg->pushButtonSave_pressed();
	}
    ui->pushButtonSysSetEngineerSave->setFocus(Qt::TabFocusReason);
	g_pScaleSrc->Backup_Config_File();
}

/************************************************************
描述：打开工程密码设置子对话框
参数：无
返回：无
************************************************************/
void SysSetEngineerDialog::on_pushButtonSysSetEngineerPwd_pressed()
{
	hideCurDialog();
	m_pPasswordSetDlg->show();
	m_curDlg = m_pPasswordSetDlg;
	m_pCurSetPushButton = ui->pushButtonSysSetEngineerPwd;
	m_pCurSetPushButton->setFlat(true);
}

void SysSetEngineerDialog::on_pushButtonSysSetEngineerDoorMachine_pressed()
{
	hideCurDialog();
	m_pDoorMachineDlg->show();
	m_curDlg = m_pDoorMachineDlg;
	m_pCurSetPushButton = ui->pushButtonSysSetEngineerDoorMachine;
	m_pCurSetPushButton->setFlat(true);
}

void SysSetEngineerDialog::on_pushButtonSysSetEngineerIpCamera_pressed()
{
	hideCurDialog();
	m_pIpCameraDlg->show();
	m_curDlg = m_pIpCameraDlg;
	m_pCurSetPushButton = ui->pushButtonSysSetEngineerIpCamera;
	m_pCurSetPushButton->setFlat(true);
}

void SysSetEngineerDialog::on_pushButtonSysSetEngineerOther_pressed()
{
	hideCurDialog();
	m_pOtherDlg->show();
	m_curDlg = m_pOtherDlg;
	m_pCurSetPushButton = ui->pushButtonSysSetEngineerOther;
	m_pCurSetPushButton->setFlat(true);
}

/************************************************************
描述：关闭本对话框
参数：无
返回：无
************************************************************/
void SysSetEngineerDialog::on_pushButtonSysSetEngineerReturn_pressed()
{
	//hideCurDialog();
	hide();
}

void SysSetEngineerDialog::on_pushButtonSysSetEngineerRemoteCheck_pressed()
{
	hideCurDialog();
	m_pRemoteCheckDlg->show();
	m_curDlg = m_pRemoteCheckDlg;
    m_pCurSetPushButton = ui->pushButtonSysSetEngineerRemoteCheck;
}

/************************************************************
描述：现实网络地址
参数：无
返回：无
************************************************************/
void SysSetEngineerDialog::showme()
{
	on_pushButtonSysSetEngineerNetAddress_pressed();
}
