#include "passwordsetdialog.h"
#include "ui_passwordsetdialog.h"
#include <QMessageBox>
#include <QSettings>
#include "syssetmaindialog.h"
#include "syssetengineerdialog.h"

#include <stdio.h>
#include "ui_syssetmaindialog.h"
#include "ui_syssetengineerdialog.h"
#include "maindialog.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
extern MainDialog *g_pMainDlg;

/************************************************************
  该文件为密码设置对话框主程序
************************************************************/

/************************************************************
描述：密码设置对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
PasswordSetDialog::PasswordSetDialog(QWidget *parent) :
    QDialog(parent), m_issysmaindialog(false) ,
    ui(new Ui::PasswordSetDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	int res = g_pScaleSrc->get_screen_version();

    if ( res == 1 )
    {
        ui->label->setStyleSheet(QString::fromUtf8("font: 19px;"));
        ui->label_3->setStyleSheet(QString::fromUtf8("font: 19px;"));
    }
    else if ( res == 2 )  // 1024x600
	{
		resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23
        ui->pushButtonPwdSetBack->setGeometry(QRect(790, 190, 200, 100));
        ui->pushButtonPwdSetNum3->setGeometry(QRect(490, 190, 220, 95));
        ui->pushButtonPwdSetNum9->setGeometry(QRect(490, 390, 220, 95));
        ui->pushButtonPwdSetNum6->setGeometry(QRect(490, 290, 220, 95));
        ui->pushButtonPwdSetNum8->setGeometry(QRect(270, 390, 220, 95));
        ui->pushButtonPwdSetNum5->setGeometry(QRect(270, 290, 220, 95));
        ui->pushButtonPwdSetNum2->setGeometry(QRect(270, 190, 220, 95));
        ui->pushButtonPwdSetNum7->setGeometry(QRect(50, 390, 220, 95));
        ui->pushButtonPwdSetNum1->setGeometry(QRect(50, 190, 220, 95));
        ui->pushButtonPwdSetNum4->setGeometry(QRect(50, 290, 220, 95));

        ui->pushButtonPwdSetDot->setGeometry(QRect(50, 490, 220, 95));
        ui->pushButtonPwdSetShop->setGeometry(QRect(490, 490, 220, 95));

        ui->pushButtonPwdSetClear->setGeometry(QRect(790, 290, 200, 100));
        ui->pushButtonPwdSetNum0->setGeometry(QRect(270, 490, 220, 95));
        ui->framePasswordSetting->setGeometry(QRect(42, 30, 700, 140));
        ui->lineEditPassword->setGeometry(QRect(280, 40, 421, 51));
        ui->lineEditCheckPassword->setGeometry(QRect(280, 110, 431, 51));
        ui->label_3->setGeometry(QRect(51, 120, 181, 40));
        ui->label->setGeometry(QRect(51, 50, 181, 40));
        ui->pushButtonPwdSetReturn->setGeometry(QRect(790, 490, 200, 100));
        ui->pushButtonPwdSetSave->setGeometry(QRect(790, 390, 200, 100));
	}
	else if ( res == 3 )  // 1280x800
	{
		resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23
        ui->pushButtonPwdSetBack->setGeometry(QRect(980, 290, 246, 112));
        ui->pushButtonPwdSetNum3->setGeometry(QRect(660, 220, 272, 123));
        ui->pushButtonPwdSetNum9->setGeometry(QRect(660, 500, 272, 123));
        ui->pushButtonPwdSetNum6->setGeometry(QRect(660, 360, 272, 123));
        ui->pushButtonPwdSetNum8->setGeometry(QRect(350, 500, 272, 123));
        ui->pushButtonPwdSetNum5->setGeometry(QRect(350, 360, 272, 123));
        ui->pushButtonPwdSetNum2->setGeometry(QRect(350, 220, 272, 123));
        ui->pushButtonPwdSetNum7->setGeometry(QRect(40, 500, 272, 123));
        ui->pushButtonPwdSetNum1->setGeometry(QRect(40, 220, 272, 123));
        ui->pushButtonPwdSetNum4->setGeometry(QRect(40, 360, 272, 123));
        ui->pushButtonPwdSetClear->setGeometry(QRect(980, 410, 246, 112));
        ui->pushButtonPwdSetNum0->setGeometry(QRect(350, 640, 272, 123));
		ui->framePasswordSetting->setGeometry(QRect(70, 10, 865, 172));
        ui->pushButtonPwdSetReturn->setGeometry(QRect(980, 650, 246, 112));
        ui->pushButtonPwdSetSave->setGeometry(QRect(980, 530, 246, 112));
		ui->label_3->setGeometry(QRect(72, 100, 251, 86));
		ui->lineEditCheckPassword->setGeometry(QRect(350, 100, 631, 86));
		ui->lineEditPassword->setGeometry(QRect(350, 10, 631, 86));
		ui->label->setGeometry(QRect(72, 10, 251, 86));
        ui->pushButtonPwdSetDot->setGeometry(QRect(40, 640, 272, 123));
        ui->pushButtonPwdSetShop->setGeometry(QRect(660, 640, 272, 123));
	}

    setAttribute(Qt::WA_TranslucentBackground, false);

    ui->pushButtonPwdSetNum0->m_offsetX = 0;
    ui->pushButtonPwdSetNum1->m_offsetX = 0;
    ui->pushButtonPwdSetNum2->m_offsetX = 0;
    ui->pushButtonPwdSetNum3->m_offsetX = 0;
    ui->pushButtonPwdSetNum4->m_offsetX = 0;
    ui->pushButtonPwdSetNum5->m_offsetX = 0;
    ui->pushButtonPwdSetNum6->m_offsetX = 0;
    ui->pushButtonPwdSetNum7->m_offsetX = 0;
    ui->pushButtonPwdSetNum8->m_offsetX = 0;
    ui->pushButtonPwdSetNum9->m_offsetX = 0;
    ui->pushButtonPwdSetDot->m_offsetX  = 0;
    ui->pushButtonPwdSetShop->m_offsetX = 0;

	ui->lineEditPassword->setEchoMode(QLineEdit::Password);
	ui->lineEditCheckPassword->setEchoMode(QLineEdit::Password);

	m_pLineEditSel = ui->lineEditPassword;

	//david 2014/9/17
	connect(ui->lineEditCheckPassword,SIGNAL(lostFocus()),this,SLOT(on_lineEditCheckPassword_lostFocus()));
	connect(ui->lineEditPassword,SIGNAL(lostFocus()),this,SLOT(on_lineEditPassword_lostFocus()));
}

/************************************************************
描述：密码设置对话框析构函数
参数：无
返回：无
************************************************************/
PasswordSetDialog::~PasswordSetDialog()
{
	delete ui;
}

/************************************************************
描述：审核新密码，如果符合要求则存入config文件
参数：无
返回：无
************************************************************/
void PasswordSetDialog::on_pushButtonPwdSetSave_pressed()
{
    if(!isVisible())
        return ;
	QString newPassword;
	QString checkPassword;
	int rnt,cnt;

	setAllButtonsEnabled(this, false);
	setAllButtonsEnabled((QDialog *)parent(), false);

	ui->framePasswordSetting->setFocus(Qt::TabFocusReason);

	newPassword = ui->lineEditPassword->text();
	checkPassword = ui->lineEditCheckPassword->text();
	rnt = newPassword.count();
	cnt = checkPassword.count();

	if(newPassword != checkPassword)
	{
		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));

		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("两次输入密码不一致"));
		messBox.show();
		QRect rc = messBox.geometry();
		QRect rc2 = geometry();
		rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
		messBox.setGeometry(rc);
		messBox.exec();
	}
	else if ((rnt < 6) || (cnt < 6))
	{
		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));

		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("输入密码长度应不小于6"));
		messBox.show();
		QRect rc = messBox.geometry();
		QRect rc2 = geometry();
		rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
		messBox.setGeometry(rc);
		messBox.exec();
	}
	else
	{
		QSettings Settings(CONFIG_FILE, QSettings::IniFormat);
		if (parent()->objectName() == "SysSetMainDialog")
		{
			Settings.setValue("system/passwd", newPassword);
		}
		else
		{
			Settings.setValue("system/engineer_passwd", newPassword);
		}

		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));

		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("密码修改成功"));
		messBox.show();
		QRect rc = messBox.geometry();
		QRect rc2 = geometry();
		rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
		messBox.setGeometry(rc);
		messBox.exec();
		g_pScaleSrc->Backup_Config_File();
	}

	setAllButtonsEnabled((QDialog *)parent(), true);
	setAllButtonsEnabled(this, true);

	ui->pushButtonPwdSetSave->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：按了密码输入对话框的"0"按钮，在已输入密码后面添加"0"
参数：无
返回：无
************************************************************/
void PasswordSetDialog::on_pushButtonPwdSetNum0_pressed()
{
    ui->pushButtonPwdSetNum0->setFocus(Qt::TabFocusReason);
	m_pLineEditSel->setText(m_pLineEditSel->text() + "0");
}

/************************************************************
描述：按了密码输入对话框的"1"按钮，在已输入密码后面添加"1"
参数：无
返回：无
************************************************************/
void PasswordSetDialog::on_pushButtonPwdSetNum1_pressed()
{
        ui->pushButtonPwdSetNum1->setFocus(Qt::TabFocusReason);
	m_pLineEditSel->setText(m_pLineEditSel->text() + "1");
}

/************************************************************
描述：按了密码输入对话框的"2"按钮，在已输入密码后面添加"2"
参数：无
返回：无
************************************************************/
void PasswordSetDialog::on_pushButtonPwdSetNum2_pressed()
{
	ui->pushButtonPwdSetNum2->setFocus(Qt::TabFocusReason);
	m_pLineEditSel->setText(m_pLineEditSel->text() + "2");
}

/************************************************************
描述：按了密码输入对话框的"3"按钮，在已输入密码后面添加"3"
参数：无
返回：无
************************************************************/
void PasswordSetDialog::on_pushButtonPwdSetNum3_pressed()
{
	ui->pushButtonPwdSetNum3->setFocus(Qt::TabFocusReason);
	m_pLineEditSel->setText(m_pLineEditSel->text() + "3");
}

/************************************************************
描述：按了密码输入对话框的"4"按钮，在已输入密码后面添加"4"
参数：无
返回：无
************************************************************/
void PasswordSetDialog::on_pushButtonPwdSetNum4_pressed()
{
    ui->pushButtonPwdSetNum4->setFocus(Qt::TabFocusReason);
	m_pLineEditSel->setText(m_pLineEditSel->text() + "4");
}

/************************************************************
描述：按了密码输入对话框的"5"按钮，在已输入密码后面添加"5"
参数：无
返回：无
************************************************************/
void PasswordSetDialog::on_pushButtonPwdSetNum5_pressed()
{
	ui->pushButtonPwdSetNum5->setFocus(Qt::TabFocusReason);
	m_pLineEditSel->setText(m_pLineEditSel->text() + "5");
}

/************************************************************
描述：按了密码输入对话框的"6"按钮，在已输入密码后面添加"6"
参数：无
返回：无
************************************************************/
void PasswordSetDialog::on_pushButtonPwdSetNum6_pressed()
{
	ui->pushButtonPwdSetNum6->setFocus(Qt::TabFocusReason);
	m_pLineEditSel->setText(m_pLineEditSel->text() + "6");
}

/************************************************************
描述：按了密码输入对话框的"7"按钮，在已输入密码后面添加"7"
参数：无
返回：无
************************************************************/
void PasswordSetDialog::on_pushButtonPwdSetNum7_pressed()
{
	ui->pushButtonPwdSetNum7->setFocus(Qt::TabFocusReason);
	m_pLineEditSel->setText(m_pLineEditSel->text() + "7");
}

/************************************************************
描述：按了密码输入对话框的"8"按钮，在已输入密码后面添加"8"
参数：无
返回：无
************************************************************/
void PasswordSetDialog::on_pushButtonPwdSetNum8_pressed()
{
	ui->pushButtonPwdSetNum8->setFocus(Qt::TabFocusReason);
	m_pLineEditSel->setText(m_pLineEditSel->text() + "8");
}

/************************************************************
描述：按了密码输入对话框的"9"按钮，在已输入密码后面添加"9"
参数：无
返回：无
************************************************************/
void PasswordSetDialog::on_pushButtonPwdSetNum9_pressed()
{
	ui->pushButtonPwdSetNum9->setFocus(Qt::TabFocusReason);
	m_pLineEditSel->setText(m_pLineEditSel->text() + "9");
}

/************************************************************
描述：按了密码输入对话框的"*"按钮，在已输入密码后面添加"*"
参数：无
返回：无
************************************************************/
void PasswordSetDialog::on_pushButtonPwdSetDot_pressed()
{
	ui->pushButtonPwdSetDot->setFocus(Qt::TabFocusReason);
	m_pLineEditSel->setText(m_pLineEditSel->text() + "*");
}

/************************************************************
描述：按了密码输入对话框的"#"按钮，在已输入密码后面添加"#"
参数：无
返回：无
************************************************************/
void PasswordSetDialog::on_pushButtonPwdSetShop_pressed()
{
	ui->pushButtonPwdSetShop->setFocus(Qt::TabFocusReason);
	m_pLineEditSel->setText(m_pLineEditSel->text() + "#");
}

/************************************************************
描述：按了密码输入对话框的退格按钮，删除已输入密码最后一个数字
参数：无
返回：无
************************************************************/
void PasswordSetDialog::on_pushButtonPwdSetBack_pressed()
{
	ui->pushButtonPwdSetBack->setFocus(Qt::TabFocusReason);
	QString str = m_pLineEditSel->text();
	str = str.left(str.length() - 1);
	m_pLineEditSel->setText(str);
}

/************************************************************
描述：返回调用者
参数：无
返回：无
************************************************************/
void PasswordSetDialog::on_pushButtonPwdSetReturn_pressed()
{
    hide();

    if ( m_issysmaindialog )
    {
        SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();
        pSysSetMainDlg->on_pushButtonSysSetMainVersion_pressed();
    }
    else
    {
        SysSetEngineerDialog *pSysSetEngineerDlg = (SysSetEngineerDialog *)parent();
        pSysSetEngineerDlg->on_pushButtonSysSetEngineerDoorMachine_pressed();
    }
}

/************************************************************
描述：输入密码清空
参数：无
返回：无
************************************************************/
void PasswordSetDialog::on_pushButtonPwdSetClear_pressed()
{
	ui->pushButtonPwdSetClear->setFocus(Qt::TabFocusReason);
	m_pLineEditSel->setText("");
}

/************************************************************
描述：密码行编辑控件失去焦点时，m_lineEditSel设为密码行编辑控件
参数：无
返回：无
************************************************************/
void PasswordSetDialog::on_lineEditPassword_lostFocus()
{
	m_pLineEditSel = ui->lineEditPassword;
}

/************************************************************
描述：密码确认行编辑控件失去焦点时，m_lineEditSel设为密码确认行编辑控
	 件
参数：无
返回：无
************************************************************/
void PasswordSetDialog::on_lineEditCheckPassword_lostFocus()
{
	m_pLineEditSel = ui->lineEditCheckPassword;
}

/************************************************************
描述：窗口显示时，行编辑控件文本清空
参数：无
返回：无
************************************************************/
void PasswordSetDialog::showEvent(QShowEvent *)
{
	ui->lineEditPassword->setText("");

	ui->lineEditCheckPassword->setText("");

	m_pLineEditSel = ui->lineEditPassword;//david 2014.6.11

    g_pMainDlg->showRefreshTabFocus(this);
}

/************************************************************
描述：窗口隐藏回调函数
参数：无
返回：无
************************************************************/
void PasswordSetDialog::hideEvent(QHideEvent *)
{
    g_pMainDlg->hideRefreshTabFocus(this);
	//SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();
	//pSysSetMainDlg->ui->pushButtonPasswordSet->setFocus(Qt::TabFocusReason);
}
//DAVID 2014.5.13
void PasswordSetDialog::on_lineEditPassword_textChanged(const QString &arg1)
{
}

/************************************************************
描述：设置标志如果是系统参数调用
参数：无
返回：无
************************************************************/
void PasswordSetDialog::set_flag()
{
	m_issysmaindialog = true;
}
