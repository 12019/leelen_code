#include "securityparamsetdialog.h"
#include "ui_securityparamsetdialog.h"
#include "syssetmaindialog.h"
#include "securityphonesetdialog.h"
#include <QSettings>
//#include <QMessageBox>
#include "ui_syssetmaindialog.h"
#include "maindialog.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

/************************************************************
  该文件为安防报警参数设置对话框主程序
************************************************************/

extern MainDialog *g_pMainDlg;

/************************************************************
描述：安防报警参数设置对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
SecurityParamSetDialog::SecurityParamSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecurityParamSetDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	int res = g_pScaleSrc->get_screen_version();
	if 	( res == 1 )  // 800x480
	{
	}
	else if ( res == 2 )  // 1024x600 
	{
        resize(1024,600);

        ui->framePanel->setGeometry(QRect(44, 12, 700, 580));
        ui->frameSecuritySetting->setGeometry(QRect(44, 12, 700, 580));

        //标题
        ui->label_title->setGeometry(QRect(139, 22, 510, 50));

        //第一列
        ui->label_securityset_1->setGeometry(QRect(44, 85, 278, 70));
        ui->label_securityset_2->setGeometry(QRect(44, 155, 278, 70));
        ui->label_securityset_3->setGeometry(QRect(44, 225, 278, 70));
        ui->label_securityset_4->setGeometry(QRect(44, 295, 278, 70));
        ui->label_securityset_5->setGeometry(QRect(44, 365, 278, 70));

        //第二列
        ui->lineEditEnterDelay->setGeometry(QRect(322, 85, 422, 70));
        ui->lineEditExitDelay->setGeometry(QRect(322, 155, 422, 70));
        ui->lineEditRepeatAlarmFilterInterval->setGeometry(QRect(322, 225, 422, 70));
        ui->pushButtonSecParaSetDefense->setGeometry(QRect(322, 295, 422, 70));
        ui->pushButtonSecParaSetSendPic->setGeometry(QRect(322, 365, 422, 70));

        //右侧按键
        ui->pushButtonSecParaSetSave->setGeometry(QRect(770, 380, 200, 96));
        ui->pushButtonSecParaSetReturn->setGeometry(QRect(770, 480, 200, 96));

        ui->label_securityset_1->setStyleSheet(QString::fromUtf8("font: 24px;"));
        ui->label_securityset_2->setStyleSheet(QString::fromUtf8("font: 24px;"));
        ui->label_securityset_3->setStyleSheet(QString::fromUtf8("font: 24px;"));
        ui->label_securityset_4->setStyleSheet(QString::fromUtf8("font: 24px;"));
        ui->label_securityset_5->setStyleSheet(QString::fromUtf8("font: 24px;"));

        ui->lineEditEnterDelay->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                                                "background-color: transparent;\n"
                                                                "font: 24px"));

        ui->lineEditExitDelay->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                                               "background-color: transparent;\n"
                                                               "font: 24px"));

        ui->lineEditRepeatAlarmFilterInterval->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                                                               "background-color: transparent;\n"
                                                                               "font: 24px"));

        ui->pushButtonSecParaSetDefense->setStyleSheet(QString::fromUtf8("font: 24px;"));
        ui->pushButtonSecParaSetSendPic->setStyleSheet(QString::fromUtf8("font: 24px;"));
	}
	else if ( res == 3 )   // 1280 x 800 
	{
        resize(1280,800);

        ui->framePanel->setGeometry(QRect(59, 30, 858, 745));
        ui->frameSecuritySetting->setGeometry(QRect(59, 30, 858, 745));

        //标题
        ui->label_title->setGeometry(QRect(219, 40, 510, 70));

        //第一列
        ui->label_securityset_1->setGeometry(QRect(59, 120, 330, 93));
        ui->label_securityset_2->setGeometry(QRect(59, 213, 330, 93));
        ui->label_securityset_3->setGeometry(QRect(59, 306, 330, 93));
        ui->label_securityset_4->setGeometry(QRect(59, 399, 330, 93));
        ui->label_securityset_5->setGeometry(QRect(59, 492, 330, 93));

        //第二列
        ui->lineEditEnterDelay->setGeometry(QRect(389, 120, 528, 93));
        ui->lineEditExitDelay->setGeometry(QRect(389, 213, 528, 93));
        ui->lineEditRepeatAlarmFilterInterval->setGeometry(QRect(389, 306, 528, 93));
        ui->pushButtonSecParaSetDefense->setGeometry(QRect(389, 399, 528, 93));
        ui->pushButtonSecParaSetSendPic->setGeometry(QRect(389, 492, 528, 93));

        //右侧按键
        ui->pushButtonSecParaSetSave->setGeometry(QRect(980, 530, 246, 112));
        ui->pushButtonSecParaSetReturn->setGeometry(QRect(980, 660, 246, 112));

		ui->label_securityset_1->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->label_securityset_2->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->label_securityset_3->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->label_securityset_4->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->label_securityset_5->setStyleSheet(QString::fromUtf8("font: 27px;"));

		ui->lineEditEnterDelay->setStyleSheet(QString::fromUtf8("border: 0px;\n"
					"background-color: transparent;\n"
					"font: 26px"));

		ui->lineEditExitDelay->setStyleSheet(QString::fromUtf8("border: 0px;\n"
					"background-color: transparent;\n"
					"font: 26px"));

		ui->lineEditRepeatAlarmFilterInterval->setStyleSheet(QString::fromUtf8("border: 0px;\n"
					"background-color: transparent;\n"
					"font: 26px"));

		ui->pushButtonSecParaSetDefense->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->pushButtonSecParaSetSendPic->setStyleSheet(QString::fromUtf8("font: 27px;"));
	}

    setAttribute(Qt::WA_TranslucentBackground, false);

	//setAttribute(Qt::WA_DeleteOnClose);
	ui->pushButtonSecParaSetDefense->m_offsetX = 0;

	//david
	ui->pushButtonSecParaSetSendPic->m_offsetX = 0;
	m_pSecPhoneSetDlg = new SecurityPhoneSetDialog(parent);
}

/************************************************************
描述：安防报警参数设置对话框析构函数
参数：无
返回：无
************************************************************/
SecurityParamSetDialog::~SecurityParamSetDialog()
{
	delete m_pSecPhoneSetDlg;
	delete ui;
}

/************************************************************
描述：根据给定的字符串创建消息框并显示消息框
参数：text - 消息框显示的文本信息
	 button_caption - 消息框按钮中的文本信息
返回：0 - 成功
	 -1 - 失败
************************************************************/
int SecurityParamSetDialog::MessageBox(QString text, QString button_caption)
{
	setAllButtonsEnabled(this, false);
	setAllButtonsEnabled((QDialog *)parent(), false);
	ui->frameSecuritySetting->setFocus(Qt::TabFocusReason);

	MyMessageBox messBox(this);
	CommonPushButton *pButton = messBox.addButton(button_caption);
	//Q_UNUSED(pButton);
	pButton->setFocus(Qt::TabFocusReason);
	messBox.setText(text);
	messBox.show();
	QRect rc = messBox.geometry();
	QRect rc2 = geometry();
	rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
	messBox.setGeometry(rc);
	messBox.exec();

	setAllButtonsEnabled((QDialog *)parent(), true);
	setAllButtonsEnabled(this, true);
	ui->pushButtonSecParaSetSave->setFocus(Qt::TabFocusReason);

	return 0;
}

/************************************************************
描述：将进入延迟、退出延迟、重复报警过滤时间、是否发送报警图像到管理机
	 设置写入config文件
参数：无
返回：无
************************************************************/
void SecurityParamSetDialog::on_pushButtonSecParaSetSave_pressed()
{
    if(!isVisible())
        return ;
	QString strEnterDelay, strExitDelay, strRepeatAlarmFilterInterval;

	int iVal;
	bool bOk;
	strEnterDelay = ui->lineEditEnterDelay->text();
	iVal = strEnterDelay.toInt(&bOk);
	if ((!bOk) || (iVal < 0) || (iVal > 255))
	{
		MessageBox(tr("进入延迟应为0-255的整数!"), QObject::tr("确定"));
		return;
	}

	strExitDelay = ui->lineEditExitDelay->text();
	iVal = strExitDelay.toInt(&bOk);
	if ((!bOk) || (iVal < 0) || (iVal > 255))
	{
		MessageBox(tr("退出延迟应为0-255的整数!"), QObject::tr("确定"));
		return;
	}

	strRepeatAlarmFilterInterval = ui->lineEditRepeatAlarmFilterInterval->text();
	iVal = strRepeatAlarmFilterInterval.toInt(&bOk);
	if ((!bOk) || (iVal < 0) || (iVal > 255))
	{
		MessageBox(tr("重复报警过滤时间\n应为0-255的整数!"), QObject::tr("确定"));
		return;
	}

	QSettings settings(CONFIG_FILE, QSettings::IniFormat);

	settings.setValue("alert_param/enter_delay", strEnterDelay);
	settings.setValue("alert_param/exit_delay", strExitDelay);
	settings.setValue("alert_param/repeat_alarm_filter_interval", strRepeatAlarmFilterInterval);
	iVal = (ui->pushButtonSecParaSetSendPic->text() == tr("是"));
	settings.setValue("alert_param/send_pic_to_center", iVal);

	g_pMainDlg->m_pSecurityMainDlg->m_iEnterDelay = strEnterDelay.toInt();
	g_pMainDlg->m_pSecurityMainDlg->m_iExitDelay = strExitDelay.toInt();
	g_pMainDlg->m_pSecurityMainDlg->m_iRepeatAlarmFilterInterval = strRepeatAlarmFilterInterval.toInt();
	g_pMainDlg->m_pSecurityMainDlg->m_iSendPicToCenter = iVal;

	/*
	   g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_pExitDelayTimer->stop();
	   g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_pExitDelayTimer->start(strExitDelay.toInt() * 1000);
	   *///david 2014.6.10

	//by david 2014.1.13 defense exitDelay and
	iVal = (ui->pushButtonSecParaSetDefense->text() == tr("是"));
	settings.setValue("alert_param/special_defense",iVal);

	MessageBox(tr("报警参数设置成功"), QObject::tr("确定"));

	g_pScaleSrc->Backup_Config_File();
}

void SecurityParamSetDialog::on_pushButtonSecParaSetReturn_pressed()
{
    hide();
    SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();
    pSysSetMainDlg->on_pushButtonSysSetMainVersion_pressed();
}
/************************************************************
描述：窗口显示时，从config文件中读取安防报警参数，更新到窗口控件中
参数：无
返回：无
************************************************************/
void SecurityParamSetDialog::showEvent(QShowEvent *)
{
    g_pMainDlg->showRefreshTabFocus(this);

	QString strEnterDelay, strExitDelay, strRepeatAlarmFilterInterval;
	int iSendPicToCenter;
	QSettings settings(CONFIG_FILE, QSettings::IniFormat);

	strEnterDelay = settings.value("alert_param/enter_delay", "40").toString();
	strExitDelay = settings.value("alert_param/exit_delay", "60").toString();
	strRepeatAlarmFilterInterval = settings.value("alert_param/repeat_alarm_filter_interval", "60").toString();
	iSendPicToCenter = settings.value("alert_param/send_pic_to_center", "0").toInt();

	int iSpecialDefense;

	iSpecialDefense = settings.value("alert_param/special_defense","0").toInt();

	ui->lineEditEnterDelay->setText(strEnterDelay);
	ui->lineEditExitDelay->setText(strExitDelay);
	ui->lineEditRepeatAlarmFilterInterval->setText(strRepeatAlarmFilterInterval);

    ui->pushButtonSecParaSetDefense->setText( (iSpecialDefense == 1) ? tr("是") : tr("否"));

	ui->pushButtonSecParaSetSendPic->setText( iSendPicToCenter ? tr("是") : tr(" 否"));
}

/************************************************************
描述：窗口隐藏时的回调函数
参数：无
返回：无
************************************************************/
void SecurityParamSetDialog::hideEvent(QHideEvent *)
{
    g_pMainDlg->hideRefreshTabFocus(this);

	//m_pSecPhoneSetDlg->hide();

	//SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();
	//pSysSetMainDlg->ui->pushButtonSecuritySet->setFocus(Qt::TabFocusReason);
}

void SecurityParamSetDialog::on_pushButtonSecParaSetDefense_pressed()
{
	if (ui->pushButtonSecParaSetDefense->text() == tr("是"))
	{
		ui->pushButtonSecParaSetDefense->setText(tr("否"));
	}
	else
	{
		ui->pushButtonSecParaSetDefense->setText(tr("是"));
	}
}

void SecurityParamSetDialog::on_pushButtonSecParaSetSendPic_pressed()
{
	if (ui->pushButtonSecParaSetSendPic->text() == tr("是"))
	{
		ui->pushButtonSecParaSetSendPic->setText(tr("否"));
	}
	else
	{
		ui->pushButtonSecParaSetSendPic->setText(tr("是"));
	}
}
