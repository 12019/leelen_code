#include "devicesetmaindialog.h"
#include "ui_devicesetmaindialog.h"
#include <QSettings>
#include <QMessageBox>
#include "syssetmaindialog.h"
#include "calibrationdialog.h"
#include "ui_syssetmaindialog.h"
#include "lpcore.h"
#include "maindialog.h"
#include "ui_securitymaindialog.h"
#include "ui_mediamandialog.h"
#include <qstandarditemmodel.h>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QFile>
#include "ui_maindialog.h"
#include "ui_adddefensedialog.h"
#include "ui_addlampdialog.h"
//#include "ui_securityparamsetdialog.h"
#include "ui_infomanmaindialog.h"

/************************************************************
  该文件为设备设置对话框主程序
************************************************************/

extern LinphoneCore *lphone;
extern LinphoneCoreVTable linphonec_vtable;

extern char g_strNativeName[50];
extern char g_strLphoneName[3][50];
extern char g_strLphoneRingFile[5][50];

extern MainDialog *g_pMainDlg;

/************************************************************
描述：设备设置对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
DeviceSetMainDialog::DeviceSetMainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeviceSetMainDialog)
{
    ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	move(150, 0);

	setAttribute(Qt::WA_TranslucentBackground, true);

	m_pCurDlg = NULL;

	m_pAddLampDlg = new AddLampDialog(parent);
	m_pAddDefenseDlg = new AddDefenseDialog(parent);
}

/************************************************************
描述：设备设置对话框析构函数
参数：无
返回：无
************************************************************/
DeviceSetMainDialog::~DeviceSetMainDialog()
{
	//if (m_pCurDlg) delete m_pCurDlg;
	delete m_pAddLampDlg;
	delete m_pAddDefenseDlg;

    delete ui;
}

/************************************************************
描述：打开电灯设备设置对话框
参数：无
返回：无
************************************************************/
void DeviceSetMainDialog::on_pushButtonLamp_clicked()
{
	/*hide();

	if (m_pCurDlg) delete m_pCurDlg;

	m_pCurDlg = (QDialog *)new AddLampDialog((QDialog *)parent());
	m_pCurDlg->show();
	*/
	if (!isVisible()) return;

	hide();
	if (m_pCurDlg) m_pCurDlg->hide();
	m_pAddLampDlg->show();
	m_pAddLampDlg->ui->pushButtonCancel->setFocus(Qt::TabFocusReason);
	m_pCurDlg = m_pAddLampDlg;
}

/************************************************************
描述：打开防区修改设置对话框
参数：无
返回：无
************************************************************/
void DeviceSetMainDialog::on_pushButtonDefense_clicked()
{
	/*
	hide();

	if (m_pCurDlg) delete m_pCurDlg;

	m_pCurDlg = (QDialog *)new AddDefenseDialog((QDialog *)parent());
	m_pCurDlg->show();
	*/
	if (!isVisible()) return;

	hide();
	if (m_pCurDlg) m_pCurDlg->hide();
	m_pAddDefenseDlg->show();
	////lg m_pAddDefenseDlg->ui->pushButtonDefenseTypeSub->setFocus(Qt::TabFocusReason);
	m_pCurDlg = m_pAddDefenseDlg;
}

/************************************************************
描述：系统参数重置为默认值，包括铃声设置、主界面时间显示模式设置、安防设置
	 等，还会删除所有的对讲记录和报警记录(信息发布记录不删除)
参数：无
返回：无
************************************************************/
void DeviceSetMainDialog::on_pushButtonSystemReset_clicked()
{
	setAllButtonsEnabled(this, false);
	setAllButtonsEnabled((QDialog *)parent(), false);
	ui->frame->setFocus(Qt::TabFocusReason);

	MyMessageBox messBox(this);
	CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
	CommonPushButton *cancelButton = messBox.addButton(QObject::tr("取消"), "pushButtonMyMessageBoxCancel");
	Q_UNUSED(okButton);
	cancelButton->setFocus(Qt::TabFocusReason);
	messBox.setText(tr("重置将删除原有配置数据，确定删除?"));
	messBox.show();
	QRect rc = messBox.geometry();
	QRect rc2 = geometry();
	rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
	messBox.setGeometry(rc);
	messBox.exec();

	setAllButtonsEnabled((QDialog *)parent(), true);
	setAllButtonsEnabled(this, true);
	ui->pushButtonSystemReset->setFocus(Qt::TabFocusReason);

	if(messBox.clickedButton() == cancelButton)
	{
		return;
	}
	// 
}

/************************************************************
描述：打开触摸屏校准窗口
参数：无
返回：无
************************************************************/
void DeviceSetMainDialog::on_pushButtonCalibration_clicked()
{
	//CalibrationDialog calibrationDlg(this);
	//calibrationDlg.exec();
	g_pMainDlg->m_pCalibrationDlg->exec();
}

/************************************************************
描述：窗口显示时的回调函数
参数：无
返回：无
************************************************************/
void DeviceSetMainDialog::showEvent(QShowEvent *)
{
}

/************************************************************
描述：窗口隐藏时的回调函数
参数：无
返回：无
************************************************************/
void DeviceSetMainDialog::hideEvent(QHideEvent *)
{
	//SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();
	//pSysSetMainDlg->ui->pushButtonDeviceSet->setFocus(Qt::TabFocusReason);
}

void DeviceSetMainDialog::on_pushButtonCalibration_pressed()
{
	on_pushButtonCalibration_clicked();
}

void DeviceSetMainDialog::on_pushButtonSystemReset_pressed()
{
	on_pushButtonSystemReset_clicked();
}

void DeviceSetMainDialog::on_pushButtonDefense_pressed()
{
	on_pushButtonDefense_clicked();
}

void DeviceSetMainDialog::on_pushButtonLamp_pressed()
{
	on_pushButtonLamp_clicked();
}
