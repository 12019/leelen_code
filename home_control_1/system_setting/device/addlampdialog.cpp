#include "addlampdialog.h"
#include "ui_addlampdialog.h"
#include "syssetmaindialog.h"
#include "maindialog.h"
#include "ui_syssetmaindialog.h"
#include "ui_devicesetmaindialog.h"

/************************************************************
  该文件为电灯设备设置对话框主程序
************************************************************/

/************************************************************
描述：电灯设备设置对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
AddLampDialog::AddLampDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLampDialog)
{
    ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	move(150, 0);

	setAttribute(Qt::WA_TranslucentBackground, true);

	ui->checkBoxAdjustLamp->setFocusPolicy(Qt::NoFocus);
}

/************************************************************
描述：电灯设备设置对话框析构函数
参数：无
返回：无
************************************************************/
AddLampDialog::~AddLampDialog()
{
    delete ui;
}

/************************************************************
描述：关闭本窗口
参数：无
返回：无
************************************************************/
void AddLampDialog::on_pushButtonCancel_clicked()
{
	if (!isVisible()) return;

	((SysSetMainDialog *)parent())->m_curDlg->show();
	hide();
}

/************************************************************
描述：窗口显示时的回调函数
参数：无
返回：无
************************************************************/
void AddLampDialog::showEvent(QShowEvent *)
{
	//MainDialog *pMainDlg = (MainDialog *)(parent()->parent());

	//pMainDlg->refreshTabFocus();
	//((SysSetMainDialog *)parent())->ui->pushButtonDeviceSet->setFocus(Qt::TabFocusReason);
	//ui->pushButtonCancel->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：窗口隐藏时，设备设置对话框中的"电灯"按钮设置为焦点控件
参数：无
返回：无
************************************************************/
void AddLampDialog::hideEvent(QHideEvent *)
{
	//MainDialog *pMainDlg = (MainDialog *)(parent()->parent());
	//pMainDlg->refreshTabFocus();

	//SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();
	//pSysSetMainDlg->ui->pushButtonDeviceSet->setFocus(Qt::TabFocusReason);
	if (QApplication::focusWidget()->parent() != this) return;
	////lg ((SysSetMainDialog *)parent())->m_pDeviceSetMainDlg->ui->pushButtonLamp->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：保存电灯设置信息
参数：无
返回：无
************************************************************/
void AddLampDialog::on_pushButtonSave_clicked()
{
	if (!isVisible()) return;
}

void AddLampDialog::on_pushButtonSave_pressed()
{
	on_pushButtonSave_clicked();
}

void AddLampDialog::on_pushButtonCancel_pressed()
{
	on_pushButtonCancel_clicked();
}
