#include "syssetpassworddialog.h"
#include "ui_syssetpassworddialog.h"
#include <QSettings>
#include <QMessageBox>
#include <syssetmaindialog.h>
#include <securitymaindialog.h>
#include "maindialog.h"
#include "ui_maindialog.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
/************************************************************
  该文件为系统设置密码输入对话框主程序
************************************************************/

/************************************************************
描述：系统设置密码输入对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
SysSetPasswordDialog::SysSetPasswordDialog(QWidget *parent) :
    QDialog(parent),
	ui(new Ui::SysSetPasswordDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

	int res = g_pScaleSrc->get_screen_version();
	if ( res == 2 )  // 1024x600 
	{
		ui->pushButtonSysPwdClear->setGeometry(QRect(775, 369, 200, 100));
		ui->frame->setGeometry(QRect(48, 22, 941, 90));
		ui->labelPwdInfo->setGeometry(QRect(104, 35, 151, 61));
		ui->pushButtonSysPwdOK->setGeometry(QRect(775, 259, 200, 100));
		ui->pushButtonSysPwdBack->setGeometry(QRect(840, 27, 135, 80));
		ui->pushButtonSysPwdReturn->setGeometry(QRect(775, 479, 200, 100));
		ui->lineEditPassword->setGeometry(QRect(334, 40, 411, 50));
		ui->pushButtonSysPwdNum1->setGeometry(QRect(50, 150, 220, 100));
		ui->pushButtonSysPwdNum6->setGeometry(QRect(530, 260, 220, 100));
		ui->pushButtonSysPwdNum4->setGeometry(QRect(50, 260, 220, 100));
		ui->pushButtonSysPwdNum3->setGeometry(QRect(530, 150, 220, 100));
		ui->pushButtonSysPwdNum9->setGeometry(QRect(530, 370, 220, 100));
		ui->pushButtonSysPwdNum2->setGeometry(QRect(290, 150, 220, 100));
		ui->pushButtonSysPwdNum7->setGeometry(QRect(50, 370, 220, 100));
		ui->pushButtonSysPwdNum8->setGeometry(QRect(290, 370, 220, 100));
		ui->pushButtonSysPwdNum5->setGeometry(QRect(290, 260, 220, 100));
		ui->pushButtonSysPwdNum0->setGeometry(QRect(290, 480, 220, 100));
		ui->pushButtonSysPwdDot->setGeometry(QRect(50, 480, 220, 100));
		ui->pushButtonSysPwdShop->setGeometry(QRect(530, 480, 220, 100));
	}
	else if ( res == 3 )  // 1280x800
	{
		ui->pushButtonSysPwdShop->setGeometry(QRect(660, 640, 272, 123));
        ui->pushButtonSysPwdClear->setGeometry(QRect(1000, 530, 246, 112));
		ui->pushButtonSysPwdNum6->setGeometry(QRect(660, 360, 272, 123));
		ui->frame->setGeometry(QRect(40, 39, 1211, 121));
		ui->pushButtonSysPwdDot->setGeometry(QRect(40, 640, 272, 123));
        ui->pushButtonSysPwdOK->setGeometry(QRect(1000, 410, 246, 112));
		ui->pushButtonSysPwdBack->setGeometry(QRect(1070, 50, 165, 102));
		ui->pushButtonSysPwdNum1->setGeometry(QRect(40, 220, 272, 123));
        ui->pushButtonSysPwdReturn->setGeometry(QRect(1000, 650, 246, 112));
		ui->pushButtonSysPwdNum4->setGeometry(QRect(40, 360, 272, 123));
		ui->pushButtonSysPwdNum3->setGeometry(QRect(660, 220, 272, 123));
		ui->pushButtonSysPwdNum9->setGeometry(QRect(660, 500, 272, 123));
		ui->pushButtonSysPwdNum2->setGeometry(QRect(350, 220, 272, 123));
		ui->pushButtonSysPwdNum7->setGeometry(QRect(40, 500, 272, 123));
		ui->pushButtonSysPwdNum8->setGeometry(QRect(350, 500, 272, 123));
		ui->pushButtonSysPwdNum5->setGeometry(QRect(350, 360, 272, 123));
		ui->pushButtonSysPwdNum0->setGeometry(QRect(350, 640, 272, 123));
		ui->labelPwdInfo->setGeometry(QRect(60, 50, 191, 102));
		ui->lineEditPassword->setGeometry(QRect(260, 50, 771, 101));
	}

    /*ui->pushButtonSysPwdNum0->setText("");
      ui->pushButtonSysPwdNum1->setText("");
      ui->pushButtonSysPwdNum2->setText("");
      ui->pushButtonSysPwdNum3->setText("");
      ui->pushButtonSysPwdNum4->setText("");
      ui->pushButtonSysPwdNum5->setText("");
      ui->pushButtonSysPwdNum6->setText("");
      ui->pushButtonSysPwdNum7->setText("");
      ui->pushButtonSysPwdNum8->setText("");
      ui->pushButtonSysPwdNum9->setText("");
	  ui->pushButtonOK->setText("");
      ui->pushButtonSysPwdBack->setText("");
	  */

	ui->pushButtonSysPwdNum0->m_offsetX = 0;
	ui->pushButtonSysPwdNum1->m_offsetX = 0;
	ui->pushButtonSysPwdNum2->m_offsetX = 0;
	ui->pushButtonSysPwdNum3->m_offsetX = 0;
	ui->pushButtonSysPwdNum4->m_offsetX = 0;
	ui->pushButtonSysPwdNum5->m_offsetX = 0;
	ui->pushButtonSysPwdNum6->m_offsetX = 0;
	ui->pushButtonSysPwdNum7->m_offsetX = 0;
	ui->pushButtonSysPwdNum8->m_offsetX = 0;
	ui->pushButtonSysPwdNum9->m_offsetX = 0;
	ui->pushButtonSysPwdDot->m_offsetX = 0;
	ui->pushButtonSysPwdShop->m_offsetX = 0;

	ui->pushButtonSysPwdBack->setText("");

	ui->lineEditPassword->setEchoMode(QLineEdit::Password);
}

/************************************************************
描述：系统设置密码输入对话框析构函数
参数：无
返回：无
************************************************************/
SysSetPasswordDialog::~SysSetPasswordDialog()
{
	delete ui;
}

/************************************************************
描述：按了密码输入对话框的"0"按钮，在已输入密码后面添加"0"
参数：无
返回：无
************************************************************/
void SysSetPasswordDialog::on_pushButtonSysPwdNum0_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "0");
}

/************************************************************
描述：按了密码输入对话框的"1"按钮，在已输入密码后面添加"1"
参数：无
返回：无
************************************************************/
void SysSetPasswordDialog::on_pushButtonSysPwdNum1_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "1");
}

/************************************************************
描述：按了密码输入对话框的"2"按钮，在已输入密码后面添加"2"
参数：无
返回：无
************************************************************/
void SysSetPasswordDialog::on_pushButtonSysPwdNum2_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "2");
}

/************************************************************
描述：按了密码输入对话框的"3"按钮，在已输入密码后面添加"3"
参数：无
返回：无
************************************************************/
void SysSetPasswordDialog::on_pushButtonSysPwdNum3_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "3");
}

/************************************************************
描述：按了密码输入对话框的"4"按钮，在已输入密码后面添加"4"
参数：无
返回：无
************************************************************/
void SysSetPasswordDialog::on_pushButtonSysPwdNum4_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "4");
}

/************************************************************
描述：按了密码输入对话框的"5"按钮，在已输入密码后面添加"5"
参数：无
返回：无
************************************************************/
void SysSetPasswordDialog::on_pushButtonSysPwdNum5_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "5");
}

/************************************************************
描述：按了密码输入对话框的"6"按钮，在已输入密码后面添加"6"
参数：无
返回：无
************************************************************/
void SysSetPasswordDialog::on_pushButtonSysPwdNum6_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "6");
}

/************************************************************
描述：按了密码输入对话框的"7"按钮，在已输入密码后面添加"7"
参数：无
返回：无
************************************************************/
void SysSetPasswordDialog::on_pushButtonSysPwdNum7_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "7");
}

/************************************************************
描述：按了密码输入对话框的"8"按钮，在已输入密码后面添加"8"
参数：无
返回：无
************************************************************/
void SysSetPasswordDialog::on_pushButtonSysPwdNum8_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "8");
}

/************************************************************
描述：按了密码输入对话框的"9"按钮，在已输入密码后面添加"9"
参数：无
返回：无
************************************************************/
void SysSetPasswordDialog::on_pushButtonSysPwdNum9_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "9");
}

/************************************************************
描述：按了对话框的"."按钮，在拨号号码显示控件的文本后面添加"."
参数：无
返回：无
************************************************************/
void SysSetPasswordDialog::on_pushButtonSysPwdDot_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + ".");
}

/************************************************************
描述：按了对话框的"#"按钮，在拨号号码显示控件的文本后面添加"-"
参数：无
返回：无
************************************************************/
void SysSetPasswordDialog::on_pushButtonSysPwdShop_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "-");
}

/************************************************************
描述：按了对话框的"退格"按钮，删除拨号号码显示控件的文本最后面的数字
参数：无
返回：无
************************************************************/
void SysSetPasswordDialog::on_pushButtonSysPwdClear_pressed()
{
	ui->lineEditPassword->setText("");
}

void SysSetPasswordDialog::on_pushButtonSysPwdBack_pressed()
{
	QString str = ui->lineEditPassword->text();
	str = str.left(str.length() - 1);
	ui->lineEditPassword->setText(str);
}

/************************************************************
描述：关闭本窗口并返回RET_CANCEL
参数：无
返回：无
************************************************************/
void SysSetPasswordDialog::on_pushButtonSysPwdReturn_pressed()
{
	MainDialog *pMainDlg = (MainDialog *)parent();

	if (pMainDlg->m_pCurActiveDlg == pMainDlg)
	{
		pMainDlg->ui->pushButtonSystemSetting->setFocus(Qt::TabFocusReason);
	}

	done(RET_CANCEL);
}

/************************************************************
描述：将输入的密码和config文件中的密码比较，如果和工程密码一致就打开工
	 程参数设置对话框，否则如果和系统密码一致就打开系统参数设置对话框
参数：无
返回：无
************************************************************/
void SysSetPasswordDialog::on_pushButtonSysPwdOK_pressed()
{
	if (!isVisible()) return;

	QString strPw, strEngiPw;
	QSettings settings(CONFIG_FILE, QSettings::IniFormat);
	strPw = settings.value("system/passwd", "123456").toString();
	strEngiPw = settings.value("system/engineer_passwd", "999999").toString();
	QString inputPw;
	inputPw = ui->lineEditPassword->text();

	strPw.replace('*','.');			// 将密码的字符串进行转换
	strPw.replace('#','-');

	strEngiPw.replace('*','.');		// 将密码的字符串进行转换
	strEngiPw.replace('#','-');

	if(strPw == inputPw)
	{
		MainDialog *pMainDlg = (MainDialog *)parent();

		pMainDlg->m_pSysSetMainDlg->show();
		pMainDlg->m_pSysSetMainDlg->ResetState();

		done(RET_PASSWORDOK);
	}
	else if (strEngiPw == inputPw)
	{
		MainDialog *pMainDlg = (MainDialog *)parent();

		pMainDlg->m_pSysSetEngineerDlg->show();
		pMainDlg->m_pSysSetEngineerDlg->ResetState();

		done(RET_PASSWORDOK);
	}
	else
	{
		setAllButtonsEnabled(this, false);
		ui->frame->setFocus(Qt::TabFocusReason);
		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		//Q_UNUSED(okButton);
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("密码错误!"));
		messBox.exec();
		setAllButtonsEnabled(this, true);
		ui->pushButtonSysPwdOK->setFocus(Qt::TabFocusReason);
	}
}

/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void SysSetPasswordDialog::showEvent(QShowEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)parent();

	ui->lineEditPassword->setText("");

	pMainDlg->showRefreshTabFocus(this);

	ui->pushButtonSysPwdNum1->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void SysSetPasswordDialog::hideEvent(QHideEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)parent();

	pMainDlg->hideRefreshTabFocus(this);
}
