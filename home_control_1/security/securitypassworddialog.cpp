#include "securitypassworddialog.h"
#include "ui_securitypassworddialog.h"
#include <QSettings>
#include <syssetmaindialog.h>
#include <QMessageBox>
#include "maindialog.h"

/************************************************************
  该文件为设防状态切换时密码输入对话框主程序
************************************************************/
extern MainDialog *g_pMainDlg;
extern bool g_Digital_Channel_Flag;				// 标识数字通道的变量

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
int b_ring_play_passwd = 0;//add by lidh 20141119 消息框(messagebox)关闭铃声时，判断是否securitypassworddialog调用

/************************************************************
描述：设防状态切换时密码输入对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
SecurityPasswordDialog::SecurityPasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecurityPasswordDialog)
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
        ui->pushButtonSecPwdReturn->setGeometry(QRect(775, 479, 200, 100));
        ui->pushButtonSecPwdNum7->setGeometry(QRect(50, 370, 220, 100));
        ui->pushButtonSecPwdNum0->setGeometry(QRect(290, 480, 220, 100));
        ui->pushButtonSecPwdNum4->setGeometry(QRect(50, 260, 220, 100));
        ui->pushButtonSecPwdShop->setGeometry(QRect(530, 480, 220, 100));
        ui->pushButtonSecPwdNum5->setGeometry(QRect(290, 260, 220, 100));
        ui->pushButtonSecPwdBack->setGeometry(QRect(840, 27, 135, 80));
        ui->pushButtonSecPwdNum2->setGeometry(QRect(290, 150, 220, 100));
        ui->lineEditPassword->setGeometry(QRect(334, 40, 411, 50));
        ui->pushButtonSecPwdClear->setGeometry(QRect(775, 369, 200, 100));
        ui->pushButtonSecPwdNum6->setGeometry(QRect(530, 260, 220, 100));
        ui->pushButtonSecPwdNum1->setGeometry(QRect(50, 150, 220, 100));
        ui->pushButtonSecPwdOK->setGeometry(QRect(775, 259, 200, 100));
        ui->pushButtonSecPwdDot->setGeometry(QRect(50, 480, 220, 100));
        ui->frame->setGeometry(QRect(48, 22, 941, 90));
        ui->pushButtonSecPwdNum8->setGeometry(QRect(290, 370, 220, 100));
        ui->pushButtonSecPwdNum3->setGeometry(QRect(530, 150, 220, 100));
        ui->pushButtonSecPwdNum9->setGeometry(QRect(530, 370, 220, 100));
        ui->labelPwdInfo->setGeometry(QRect(104, 35, 151, 61));
    }
    else if ( res == 3 ) // 1280x800
    {
        ui->pushButtonSecPwdReturn->setGeometry(QRect(990, 650, 246, 112));
        ui->pushButtonSecPwdNum7->setGeometry(QRect(40, 500, 272, 123));
        ui->pushButtonSecPwdNum0->setGeometry(QRect(350, 640, 272, 123));
        ui->pushButtonSecPwdNum4->setGeometry(QRect(40, 360, 272, 123));
        ui->pushButtonSecPwdShop->setGeometry(QRect(660, 640, 272, 123));
        ui->pushButtonSecPwdNum5->setGeometry(QRect(350, 360, 272, 123));
        ui->pushButtonSecPwdBack->setGeometry(QRect(1060, 50, 165, 102));
        ui->pushButtonSecPwdNum2->setGeometry(QRect(350, 220, 272, 123));
        ui->lineEditPassword->setGeometry(QRect(240, 50, 781, 102));
        ui->pushButtonSecPwdClear->setGeometry(QRect(990, 530, 246, 112));
        ui->pushButtonSecPwdNum6->setGeometry(QRect(660, 360, 272, 123));
        ui->pushButtonSecPwdOK->setGeometry(QRect(990, 410, 246, 112));
        ui->pushButtonSecPwdDot->setGeometry(QRect(40, 640, 272, 123));
        ui->frame->setGeometry(QRect(40, 39, 1201, 121));
        ui->pushButtonSecPwdNum1->setGeometry(QRect(40, 220, 272, 123));
        ui->pushButtonSecPwdNum8->setGeometry(QRect(350, 500, 272, 123));
        ui->pushButtonSecPwdNum3->setGeometry(QRect(660, 220, 272, 123));
        ui->pushButtonSecPwdNum9->setGeometry(QRect(660, 500, 272, 123));
        ui->labelPwdInfo->setGeometry(QRect(70, 50, 161, 102));
    }

    ui->pushButtonSecPwdNum0->m_offsetX = 0;
    ui->pushButtonSecPwdNum1->m_offsetX = 0;
    ui->pushButtonSecPwdNum2->m_offsetX = 0;
    ui->pushButtonSecPwdNum3->m_offsetX = 0;
    ui->pushButtonSecPwdNum4->m_offsetX = 0;
    ui->pushButtonSecPwdNum5->m_offsetX = 0;
    ui->pushButtonSecPwdNum6->m_offsetX = 0;
    ui->pushButtonSecPwdNum7->m_offsetX = 0;
    ui->pushButtonSecPwdNum8->m_offsetX = 0;
    ui->pushButtonSecPwdNum9->m_offsetX = 0;
    ui->pushButtonSecPwdDot->m_offsetX = 0;
    ui->pushButtonSecPwdShop->m_offsetX = 0;

    ui->pushButtonSecPwdBack->setText("");

	ui->lineEditPassword->setEchoMode(QLineEdit::Password);

	m_goingSecurityState = 0;
}

/************************************************************
描述：设防状态切换时密码输入对话框析构函数
参数：无
返回：无
************************************************************/
SecurityPasswordDialog::~SecurityPasswordDialog()
{
	delete ui;
}

void SecurityPasswordDialog::resetState(int goingSecurityState, int flag)
{
	m_goingSecurityState = goingSecurityState;
	m_flag = flag;

	ui->lineEditPassword->setText("");
}


/************************************************************
描述：按了密码输入对话框的"0"按钮，在已输入密码后面添加"0"
参数：无
返回：无
************************************************************/
void SecurityPasswordDialog::on_pushButtonSecPwdNum0_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "0");
}

/************************************************************
描述：按了密码输入对话框的"1"按钮，在已输入密码后面添加"1"
参数：无
返回：无
************************************************************/
void SecurityPasswordDialog::on_pushButtonSecPwdNum1_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "1");
}

/************************************************************
描述：按了密码输入对话框的"2"按钮，在已输入密码后面添加"2"
参数：无
返回：无
************************************************************/
void SecurityPasswordDialog::on_pushButtonSecPwdNum2_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "2");
}

/************************************************************
描述：按了密码输入对话框的"3"按钮，在已输入密码后面添加"3"
参数：无
返回：无
************************************************************/
void SecurityPasswordDialog::on_pushButtonSecPwdNum3_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "3");
}

/************************************************************
描述：按了密码输入对话框的"4"按钮，在已输入密码后面添加"4"
参数：无
返回：无
************************************************************/
void SecurityPasswordDialog::on_pushButtonSecPwdNum4_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "4");
}

/************************************************************
描述：按了密码输入对话框的"5"按钮，在已输入密码后面添加"5"
参数：无
返回：无
************************************************************/
void SecurityPasswordDialog::on_pushButtonSecPwdNum5_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "5");
}

/************************************************************
描述：按了密码输入对话框的"6"按钮，在已输入密码后面添加"6"
参数：无
返回：无
************************************************************/
void SecurityPasswordDialog::on_pushButtonSecPwdNum6_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "6");
}

/************************************************************
描述：按了密码输入对话框的"7"按钮，在已输入密码后面添加"7"
参数：无
返回：无
************************************************************/
void SecurityPasswordDialog::on_pushButtonSecPwdNum7_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "7");
}

/************************************************************
描述：按了密码输入对话框的"8"按钮，在已输入密码后面添加"8"
参数：无
返回：无
************************************************************/
void SecurityPasswordDialog::on_pushButtonSecPwdNum8_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "8");
}

/************************************************************
描述：按了密码输入对话框的"9"按钮，在已输入密码后面添加"9"
参数：无
返回：无
************************************************************/
void SecurityPasswordDialog::on_pushButtonSecPwdNum9_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "9");
}

/************************************************************
描述：按了对话框的"."按钮，在拨号号码显示控件的文本后面添加"."
参数：无
返回：无
************************************************************/
void SecurityPasswordDialog::on_pushButtonSecPwdDot_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + ".");
}

/************************************************************
描述：按了对话框的"#"按钮，在拨号号码显示控件的文本后面添加"-"
参数：无
返回：无
************************************************************/
void SecurityPasswordDialog::on_pushButtonSecPwdShop_pressed()
{
	ui->lineEditPassword->setText(ui->lineEditPassword->text() + "-");
}

/************************************************************
描述：按了对话框的"退格"按钮，删除拨号号码显示控件的文本最后面的数字
参数：无
返回：无
************************************************************/
void SecurityPasswordDialog::on_pushButtonSecPwdClear_pressed()
{
	ui->lineEditPassword->setText("");
}

/************************************************************
描述：按了密码输入对话框的退格按钮，删除已输入密码最后一个数字
参数：无
返回：无
************************************************************/
void SecurityPasswordDialog::on_pushButtonSecPwdBack_pressed()
{
	QString str = ui->lineEditPassword->text();
	str = str.left(str.length() - 1);
	ui->lineEditPassword->setText(str);
}

/************************************************************
描述：将输入的密码和config文件中的密码比较，如果一致就关闭本窗口并返
	 回0
参数：无
返回：无
************************************************************/
void SecurityPasswordDialog::on_pushButtonSecPwdOK_pressed()
{
	if (!isVisible()) return;

	QString strPw;
	QSettings settings(CONFIG_FILE, QSettings::IniFormat);
	strPw = settings.value("system/passwd", "123456").toString();
	QString inputPw;
	inputPw = ui->lineEditPassword->text();
	SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();

	if(strPw == inputPw)
	{
		if (m_flag == 0)
		{
			QString strWaveFile;

			if (m_goingSecurityState == STATUS_ATHOME)
			{
				strWaveFile = "athome";
			}
			else
			{
				strWaveFile = "disarm";
			}

			if (g_pMainDlg->m_inputMethodMode == 0)
			{
				strWaveFile += "-en";
			}

			strWaveFile = "wav/" + strWaveFile + ".wav";

			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
			playWaveFile(strWaveFile.toLocal8Bit().data(), g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);

			pSecurityMainDlg->setSecurityState(m_goingSecurityState);
            pSecurityMainDlg->emitsigSetSecurityStatueToPhone(m_goingSecurityState);
            if(pSecurityMainDlg->m_pUploadSetSecurityStateThread->m_bRun)
            {
                pSecurityMainDlg->m_pUploadSetSecurityStateThread->m_bRun = false;
                pSecurityMainDlg->m_pUploadSetSecurityStateThread->quit();
                pSecurityMainDlg->m_pUploadSetSecurityStateThread->wait();
            }
            pSecurityMainDlg->m_pUploadSetSecurityStateThread->start();
			pSecurityMainDlg->m_pSecurityAbnormalDlg->resetState();
        }

		done(RET_PASSWORDOK);
	}
	else
	{
		setAllButtonsEnabled(this, false);
		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		//Q_UNUSED(okButton);
		okButton->setFocus(Qt::TabFocusReason);
        b_ring_play_passwd = 1;
		messBox.setText(tr("密码错误!"));
		messBox.exec();
		setAllButtonsEnabled(this, true);
	}
}

/************************************************************
描述：关闭本窗口并返回1
参数：无
返回：无
************************************************************/
void SecurityPasswordDialog::on_pushButtonSecPwdReturn_pressed()
{
	if (m_flag == 0)
	{
		SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();
		pSecurityMainDlg->m_pSecurityAbnormalDlg->resetState();
	}

	done(1);
}

/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void SecurityPasswordDialog::showEvent(QShowEvent *)
{
	g_pMainDlg->showRefreshTabFocus(this);

    ui->pushButtonSecPwdNum1->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void SecurityPasswordDialog::hideEvent(QHideEvent *)
{
	g_pMainDlg->hideRefreshTabFocus(this);
}
