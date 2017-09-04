#include "adddefensedialog.h"
#include "ui_adddefensedialog.h"
#include "syssetmaindialog.h"
#include <QSettings>
#include <QTextCodec>
#include "serial1thread.h"
#include "maindialog.h"
#include <termios.h>
#include "securitymaindialog.h"
#include "ui_syssetmaindialog.h"
#include "ui_devicesetmaindialog.h"
#include "lpcore.h"
#include "ui_maindialog.h"
#include "ui_securitymaindialog.h"
#include "ui_mediamandialog.h"
#include "ui_infomanmaindialog.h"
#include <linux/watchdog.h>
#include <sys/ioctl.h>
#include "data_types.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
extern int g_door_bell_type;
/************************************************************
  该文件为添加/删除/修改防区对话框主程序
************************************************************/

extern MainDialog *g_pMainDlg;
extern LinphoneCore *lphone;
extern LinphoneCoreVTable linphonec_vtable;

extern char g_strNativeName[50];
extern char g_strLphoneName[3][50];
extern char g_strLphoneRingFile[5][50];
extern int g_mainMonitorFlag;

/************************************************************
 恢复备份文件 
************************************************************/
extern void restore_config_from_last_file();

/************************************************************
  防区类型字符串
************************************************************/
const char *g_strDefenseType[2] = {QT_TRANSLATE_NOOP("QObject", "有线防区"), QT_TRANSLATE_NOOP("QObject", "无线防区")};
/************************************************************
  防区属性字符串
************************************************************/
const char *g_strProperty[3] = {QT_TRANSLATE_NOOP("QObject", "常设"), QT_TRANSLATE_NOOP("QObject", "外部"), QT_TRANSLATE_NOOP("QObject", "内部")};
/************************************************************
  防区名称字符串
************************************************************/
const char *g_strType[6] = { QT_TRANSLATE_NOOP("QObject", "紧急求助"), QT_TRANSLATE_NOOP("QObject", "烟感"), QT_TRANSLATE_NOOP("QObject", "煤气泄漏"),
							QT_TRANSLATE_NOOP("QObject", "红外"), QT_TRANSLATE_NOOP("QObject", "门磁"), QT_TRANSLATE_NOOP("QObject", "秘密挟持")};
/************************************************************
描述：防区修改对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
AddDefenseDialog::AddDefenseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDefenseDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

    resize(qApp->desktop()->size().width(),qApp->desktop()->size().height());

	int res = g_pScaleSrc->get_screen_version();
	if 	( res == 1 )  // 800x480
    {

	}
	else if ( res == 2 )  // 1024x600 
	{
        ui->framePanel->setGeometry(QRect(44, 12, 700, 580));
        ui->frameDefenseSetting->setGeometry(QRect(44, 12, 700, 580));

        //标题
        ui->label_title->setGeometry(QRect(139, 22, 510, 50));

        //第一列
        ui->label_adddefense_1->setGeometry(QRect(44, 85, 175, 70));
        ui->label_adddefense_2->setGeometry(QRect(44, 155, 175, 70));
        ui->label_adddefense_3->setGeometry(QRect(44, 225, 175, 70));
        ui->label_adddefense_4->setGeometry(QRect(44, 295, 175, 70));
        ui->label_adddefense_5->setGeometry(QRect(44, 365, 175, 70));

        //第二列
        ui->labelDefenseNumber->setGeometry(QRect(219, 85, 175, 70));
        ui->pushButtonAddDefChannel->setGeometry(QRect(219, 155, 175, 70));
        ui->pushButtonAddDefSubType->setGeometry(QRect(219, 225, 175, 70));
        ui->lineEditDefenseLocation->setGeometry(QRect(219, 295, 525, 70));
        ui->pushButtonAddDefDelayAlarm->setGeometry(QRect(219, 365, 175, 70));

        //第三列
        ui->label_adddefense_6->setGeometry(QRect(394, 85, 175, 70));
        ui->label_adddefense_7->setGeometry(QRect(394, 155, 175, 70));
        ui->label_adddefense_8->setGeometry(QRect(394, 225, 175, 70));

        //第四列
        ui->pushButtonAddDefType->setGeometry(QRect(569, 85, 175, 70));
        ui->pushButtonAddDefProperty->setGeometry(QRect(569, 155, 175, 70));
        ui->pushButtonAddDefBypass->setGeometry(QRect(569, 225, 175, 70));

        //右侧按键
        ui->pushButtonAddDefCalibrate->setGeometry(QRect(777, 13, 220, 70));
        ui->pushButtonAddDefReset->setGeometry(QRect(777, 86, 220, 70));
        ui->pushButtonAddDefReboot->setGeometry(QRect(777, 159, 220, 70));
        ui->pushButtonAddDefPrev->setGeometry(QRect(777, 232, 220, 70));
        ui->pushButtonAddDefNext->setGeometry(QRect(777, 305, 220, 70));
        ui->pushButtonAddDefDel->setGeometry(QRect(777, 378, 220, 70));
        ui->pushButtonAddDefSave->setGeometry(QRect(777, 451, 220, 70));
        ui->pushButtonAddDefReturn->setGeometry(QRect(777, 524, 220, 70));
	}
	else if ( res == 3 )  // 1280x800
	{

        ui->framePanel->setGeometry(QRect(59, 30, 858, 745));
        ui->frameDefenseSetting->setGeometry(QRect(59, 30, 858, 745));

        //标题
        ui->label_title->setGeometry(QRect(219, 40, 510, 70));

        //第一列
        ui->label_adddefense_1->setGeometry(QRect(59, 120, 214, 93));
        ui->label_adddefense_2->setGeometry(QRect(59, 213, 214, 93));
        ui->label_adddefense_3->setGeometry(QRect(59, 306, 214, 93));
        ui->label_adddefense_4->setGeometry(QRect(59, 399, 214, 93));
        ui->label_adddefense_5->setGeometry(QRect(59, 492, 214, 93));

        //第二列
        ui->labelDefenseNumber->setGeometry(QRect(273, 120, 214, 93));
        ui->pushButtonAddDefChannel->setGeometry(QRect(273, 213, 214, 93));
        ui->pushButtonAddDefSubType->setGeometry(QRect(273, 306, 214, 93));
        ui->lineEditDefenseLocation->setGeometry(QRect(273, 399, 644, 93));
        ui->pushButtonAddDefDelayAlarm->setGeometry(QRect(273, 492, 214, 93));

        //第三列
        ui->label_adddefense_6->setGeometry(QRect(487, 120, 214, 93));
        ui->label_adddefense_7->setGeometry(QRect(487, 213, 214, 93));
        ui->label_adddefense_8->setGeometry(QRect(487, 306, 214, 93));

        //第四列
        ui->pushButtonAddDefType->setGeometry(QRect(701, 120, 216, 93));
        ui->pushButtonAddDefProperty->setGeometry(QRect(701, 213, 216, 93));
        ui->pushButtonAddDefBypass->setGeometry(QRect(701, 306, 216, 93));

        //右侧按键
        ui->pushButtonAddDefCalibrate->setGeometry(QRect(962, 26, 270, 86));
        ui->pushButtonAddDefReset->setGeometry(QRect(962, 121, 270, 86));
        ui->pushButtonAddDefReboot->setGeometry(QRect(962, 216, 270, 86));
        ui->pushButtonAddDefPrev->setGeometry(QRect(962, 311, 270, 86));
        ui->pushButtonAddDefNext->setGeometry(QRect(962, 406, 270, 86));
        ui->pushButtonAddDefDel->setGeometry(QRect(962, 501, 270, 86));
        ui->pushButtonAddDefSave->setGeometry(QRect(962, 596, 270, 86));
        ui->pushButtonAddDefReturn->setGeometry(QRect(962, 691, 270, 86));

		ui->frameDefenseSetting->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->lineEditDefenseLocation->setStyleSheet(QString::fromUtf8("border: 0px;\n"
					"background-color: transparent;\n"
					"font: 26px"));
        ui->pushButtonAddDefSubType->setStyleSheet(QString::fromUtf8("font: 27px;"));
        ui->pushButtonAddDefDelayAlarm->setStyleSheet(QString::fromUtf8("font: 27px;"));
        ui->pushButtonAddDefChannel->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->labelDefenseNumber->setStyleSheet(QString::fromUtf8("border: 0px;\n"
					"background-color: transparent;\n"
					"font: 26px"));
        ui->pushButtonAddDefProperty->setStyleSheet(QString::fromUtf8("font: 27px;"));
        ui->pushButtonAddDefType->setStyleSheet(QString::fromUtf8("font: 27px;"));
        ui->pushButtonAddDefBypass->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->label_adddefense_2->setStyleSheet(QApplication::translate("AddDefenseDialog", "font: 27px;", 0, QApplication::UnicodeUTF8));
		ui->label_adddefense_5->setStyleSheet(QApplication::translate("AddDefenseDialog", "font: 27px;", 0, QApplication::UnicodeUTF8));
		ui->label_adddefense_3->setStyleSheet(QApplication::translate("AddDefenseDialog", "font: 27px;", 0, QApplication::UnicodeUTF8));
		ui->label_adddefense_1->setStyleSheet(QApplication::translate("AddDefenseDialog", "font: 27px;", 0, QApplication::UnicodeUTF8));
		ui->label_adddefense_4->setStyleSheet(QApplication::translate("AddDefenseDialog", "font: 27px;", 0, QApplication::UnicodeUTF8));
		ui->label_adddefense_7->setStyleSheet(QApplication::translate("AddDefenseDialog", "font: 27px;", 0, QApplication::UnicodeUTF8));
		ui->label_adddefense_6->setStyleSheet(QApplication::translate("AddDefenseDialog", "font: 27px;", 0, QApplication::UnicodeUTF8));
		ui->label_adddefense_8->setStyleSheet(QApplication::translate("AddDefenseDialog", "font: 27px;", 0, QApplication::UnicodeUTF8));
	}

    setAttribute(Qt::WA_TranslucentBackground, false);

    ui->pushButtonAddDefType->m_offsetX = 0;
    ui->pushButtonAddDefChannel->m_offsetX = 0;
    ui->pushButtonAddDefProperty->m_offsetX = 0;
    ui->pushButtonAddDefSubType->m_offsetX = 0;
    ui->pushButtonAddDefDelayAlarm->m_offsetX = 0;
    ui->pushButtonAddDefBypass->m_offsetX = 0;

    ui->pushButtonAddDefCalibrate->m_offsetX = 0;
    ui->pushButtonAddDefReset->m_offsetX = 0;

//	ui->pushButtonAddDefCalibrate->m_offsetY = 3;
//	ui->pushButtonAddDefReset->m_offsetY = 3;

    ui->pushButtonAddDefCalibrate->m_offsetX = 20;
    ui->pushButtonAddDefReset->m_offsetX = 20;
    ui->pushButtonAddDefReboot->m_offsetX = 20;

    ui->pushButtonAddDefPrev->m_offsetX = 15;
    ui->pushButtonAddDefNext->m_offsetX = 15;

    ui->pushButtonAddDefCalibrate->hide();
	//showCurrentItem(0, 1);

	/*m_defenseType = 0;
	  m_channel = 0;
	  m_property = 0;
	  m_type = 0;

	  ui->labelDefenseType->setText(QString::fromUtf8(g_strDefenseType[m_defenseType]));
	  ui->labelChannel->setText(QString::fromUtf8("通道") + QString::number(m_channel + 1));
	  ui->labelProperty->setText(QString::fromUtf8(g_strProperty[m_property]));
	  ui->labelType->setText(QString::fromUtf8(g_strType[m_type]));
	  */
	/*int i;
	  QString defense;
	  QString strNumber;
	  QSettings settings("/mnt/disk/config/defense.conf", QSettings::IniFormat);
	  settings.setIniCodec(QTextCodec::codecForName("UTF8"));

	  SecurityMainDialog *pSecurityMainDlg = ((MainDialog *)(parent->parent()))->m_pSecurityMainDlg;

	//m_wireDefenseCnt = 0;
	//m_wirelessDefenseCnt = 0;
	*/
	/*for (i = 0; i < 56; i++)
	  {
	  defense = QString("wiredefense") + QString::number(i + 1);
	  strNumber = settings.value(defense + "/number", "").toString();
	  if (strNumber != "")
	  {
	  pSecurityMainDlg->m_allDefenseNumber[pSecurityMainDlg->m_wireDefenseCnt++] = strNumber;
	  }

	  defense = QString("wirelessdefense") + QString::number(i + 1);
	  strNumber = settings.value(defense + "/number", "").toString();
	  if (strNumber != "")
	  {
	  pSecurityMainDlg->m_allDefenseNumber[56 + (pSecurityMainDlg->m_wirelessDefenseCnt++)] = strNumber;
	  }
	  }*/

	m_pAbnormalMsgBox = new MyMessageBox(this);
	m_pAbnormalMsgBoxOkBtn = m_pAbnormalMsgBox->addButton(QObject::tr("确定"));
	m_pAbnormalMsgBox->hide();

    ui->pushButtonAddDefType->setDisabled(TRUE);
}

/************************************************************
描述：防区修改对话框析构函数
参数：无
返回：无
************************************************************/
AddDefenseDialog::~AddDefenseDialog()
{
	delete m_pAbnormalMsgBox;
    delete ui;
}

/************************************************************
描述：防区修改后防区异常的状态显示消息框
参数：strText:防区异常的状态信息
返回：无
************************************************************/
void AddDefenseDialog::showAbnormalMsgBox(QString strText)
{
	m_pAbnormalMsgBox->setText(strText);
	m_pAbnormalMsgBoxOkBtn->setText(QObject::tr("确定"));
	m_pAbnormalMsgBox->show();
	m_pAbnormalMsgBoxOkBtn->setFocus(Qt::TabFocusReason);
	QRect rc = m_pAbnormalMsgBox->geometry();
	QRect rc2 = geometry();
	rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
	m_pAbnormalMsgBox->setGeometry(rc);
	//messBox.exec();
}

/************************************************************
描述：根据给定的字符串创建消息框并显示消息框
参数：parent - (该参数未用到)
	 text - 消息框显示的文本信息
	 button_caption - 消息框按钮中的文本信息
返回：0 - 成功
	 -1 - 失败
************************************************************/
int AddDefenseDialog::MessageBox(QDialog *parent, QString text, QString button_caption)
{
	Q_UNUSED(parent);

	setAllButtonsEnabled(this, false);
	setAllButtonsEnabled((QDialog *)(this->parent()), false);

	MyMessageBox messBox(this);
	CommonPushButton *pButton = messBox.addButton(button_caption);
	//Q_UNUSED(pButton);
	pButton->setFocus(Qt::TabFocusReason);
	messBox.setText(text);
	messBox.show();
	QRect rc = messBox.geometry();
	QRect rc2 = this->geometry();
	rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
	messBox.setGeometry(rc);
	messBox.exec();

	setAllButtonsEnabled((QDialog *)(this->parent()), true);
	setAllButtonsEnabled(this, true);

    ui->pushButtonAddDefType->setDisabled(TRUE);

	return 0;
}

/************************************************************
描述：向前切换防区类型(支持回滚)，同时根据新的防区类型显示防区信息
参数：无
返回：无
************************************************************/
/*void AddDefenseDialog::on_pushButtonDefenseTypeSub_pressed()
{
	if (m_defenseType > 0)
	{
		m_defenseType--;
	}
	else
	{
		m_defenseType = 1;
	}

	ui->labelDefenseType->setText(QObject::tr(g_strDefenseType[m_defenseType]));
	showCurrentItem(m_defenseType, ui->labelDefenseNumber->text().toInt());
}
*/
/************************************************************
描述：向后切换防区类型(支持回滚)，同时根据新的防区类型显示防区信息
参数：无
返回：无
************************************************************/
/*void AddDefenseDialog::on_pushButtonDefenseTypeAdd_pressed()
{
	if (m_defenseType < 1)
	{
		m_defenseType++;
	}
	else
	{
		m_defenseType = 0;
	}

	ui->labelDefenseType->setText(QObject::tr(g_strDefenseType[m_defenseType]));
	showCurrentItem(m_defenseType, ui->labelDefenseNumber->text().toInt());
}
*/
/************************************************************
描述：向前更换修改防区摄像头通道号(支持回滚)
参数：无
返回：无
************************************************************/
/*void AddDefenseDialog::on_pushButtonChannelSub_pressed()
{
	if (m_channel > 0)
	{
		m_channel--;
	}
	else
	{
		m_channel = 3;
	}

	ui->labelChannel->setText(tr("通道") + QString::number(m_channel + 1));
}
*/
/************************************************************
描述：向后更换修改防区摄像头通道号(支持回滚)
参数：无
返回：无
************************************************************/
/*void AddDefenseDialog::on_pushButtonChannelAdd_pressed()
{
	if (m_channel < 3)
	{
		m_channel++;
	}
	else
	{
		m_channel = 0;
	}

	ui->labelChannel->setText(tr("通道") + QString::number(m_channel + 1));
}
*/
/************************************************************
描述：向前更换修改防区属性(支持回滚)，如果防区属性是常设防区，则该防区
	 不支持延迟报警
参数：无
返回：无
************************************************************/
/*void AddDefenseDialog::on_pushButtonPropertySub_pressed()
{
	if (m_property > 0)
	{
		m_property--;
	}
	else
	{
		m_property = 2;
	}

	if (m_property == 0)
	{
		ui->checkBoxDelayAlarm->setChecked(false);
		ui->checkBoxDelayAlarm->setEnabled(false);
	}
	else
	{
		ui->checkBoxDelayAlarm->setEnabled(true);
	}

	ui->labelProperty->setText(QObject::tr(g_strProperty[m_property]));
}
*/
/************************************************************
描述：向后更换修改防区属性(支持回滚)，如果防区属性是常设防区，则该防区
	 不支持延迟报警
参数：无
返回：无
************************************************************/
/*void AddDefenseDialog::on_pushButtonPropertyAdd_pressed()
{
	if (m_property < 2)
	{
		m_property++;
	}
	else
	{
		m_property = 0;
	}

	if (m_property == 0)
	{
		ui->checkBoxDelayAlarm->setChecked(false);
		ui->checkBoxDelayAlarm->setEnabled(false);
	}
	else
	{
		ui->checkBoxDelayAlarm->setEnabled(true);
	}

	ui->labelProperty->setText(QObject::tr(g_strProperty[m_property]));
}
*/
/************************************************************
描述：向前更换修改防区名称(支持回滚)
参数：无
返回：无
************************************************************/
/*void AddDefenseDialog::on_pushButtonTypeSub_pressed()
{
	if (m_type > 0)
	{
		m_type--;
	}
	else
	{
		m_type = 5;
	}
	ui->labelType->setText(QObject::tr(g_strType[m_type]));
}
*/
/************************************************************
描述：向后更换修改防区名称(支持回滚)
参数：无
返回：无
************************************************************/
/*void AddDefenseDialog::on_pushButtonTypeAdd_pressed()
{
	if (m_type < 5)
	{
		m_type++;
	}
	else
	{
		m_type = 0;
	}
	ui->labelType->setText(QObject::tr(g_strType[m_type]));
}*/

/************************************************************
描述：保存修改后的防区，重新启动串口报警信息接收处理线程
参数：无
返回：无
************************************************************/
void AddDefenseDialog::on_pushButtonAddDefSave_pressed()
{
	if (!isVisible()) return;

    if ((ui->pushButtonAddDefChannel->text() == tr("空"))
        || (ui->pushButtonAddDefProperty->text() == tr("空"))
        || (ui->pushButtonAddDefSubType->text() == tr("空"))
		|| (ui->lineEditDefenseLocation->text() == ""))
	{
		ui->frameDefenseSetting->setFocus(Qt::TabFocusReason);
		MessageBox(this, tr("输入信息不完整"), QObject::tr("确定"));
        ui->pushButtonAddDefSave->setFocus(Qt::TabFocusReason);
		return;
	}

	QString strNumber = ui->labelDefenseNumber->text();
	QString strLocation = ui->lineEditDefenseLocation->text();
    int bDelayAlarm = ui->pushButtonAddDefDelayAlarm->text() == tr("是");
    int bBypass = ui->pushButtonAddDefBypass->text() == tr("是");
	int i;

	QSettings settings("/mnt/disk/config/defense.conf", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF8"));

	SecurityMainDialog *pSecurityMainDlg = ((MainDialog *)(parent()->parent()))->m_pSecurityMainDlg;

	pSecurityMainDlg->m_pSerial1Thread->m_bRun = false;
	pSecurityMainDlg->m_pSerial1Thread->quit();
	pSecurityMainDlg->m_pSerial1Thread->wait();

	if (m_defenseType == 0)
	{
		for (i = 0; i < pSecurityMainDlg->m_wireDefenseCnt; i++)
		{
			if (pSecurityMainDlg->m_allDefenseNumber[i] == strNumber.toInt())
			{
				break;
			}
		}

		if (i == pSecurityMainDlg->m_wireDefenseCnt)
		{
			pSecurityMainDlg->m_wireDefenseCnt++;
		}
		else
		{
			delete pSecurityMainDlg->m_delayAlarmClass[i];
		}

        pSecurityMainDlg->m_delayAlarmClass[i] = new DelayAlarmClass(pSecurityMainDlg, (unsigned char)0xa0, (!bBypass) && bDelayAlarm);
		if (pSecurityMainDlg->m_delayAlarmClass[i]->m_pDelayTimer)
		{
			connect(pSecurityMainDlg->m_delayAlarmClass[i]->m_pDelayTimer, SIGNAL(timeout()), pSecurityMainDlg->m_pSerial1Thread, SLOT(enterDelayTimeOut()));
		}
		pSecurityMainDlg->m_allDefenseNumber[i] = strNumber.toInt();

		QString defense;
		defense = QString("wiredefense") + strNumber;

		settings.setValue(defense + "/property", QString::number(m_property));//lg tr(g_strProperty[m_property]));
		settings.setValue(defense + "/channel", QString::number(m_channel));//tr("通道") + QString::number(m_channel + 1));
		settings.setValue(defense + "/type", QString::number(m_type));//lg tr(g_strType[m_type]));
		settings.setValue(defense + "/number", strNumber);
		settings.setValue(defense + "/location", strLocation);
		settings.setValue(defense + "/delay_alarm", bDelayAlarm);
		settings.setValue(defense + "/bypass", bBypass);

		QSettings settings2("/mnt/disk/config/defense", QSettings::IniFormat);
		settings2.setValue("wiredefense/defense" + strNumber, "-1");

        pSecurityMainDlg->m_pSerial1Thread->m_bRun = -1;
        pSecurityMainDlg->m_pSerial1Thread->start();

        extern int g_iDeviceType;

        if(g_iDeviceType == TOUCHR)
        {
            printf("TOUCHR enable\n");
            pSecurityMainDlg->m_pDefenseGPIOScanThread->m_bCopyDataOneTime = true;
        }

		ui->frameDefenseSetting->setFocus(Qt::TabFocusReason);
		MessageBox(this, tr("有线防区修改成功\n该防区为第") + strNumber + tr("个有线防区"), QObject::tr("确定"));
        ui->pushButtonAddDefSave->setFocus(Qt::TabFocusReason);
	}
	else if (m_defenseType == 1)
	{
		for (i = 0; i < pSecurityMainDlg->m_wirelessDefenseCnt; i++)
		{
			if (pSecurityMainDlg->m_allDefenseNumber[56 + i] == strNumber.toInt())
			{
				break;
			}
		}

		int identity;
		identity = getWirelessDefenseIdentity();

		printf("lg identity:%08x m_wirelessDefenseCnt:%d\n", identity, pSecurityMainDlg->m_wirelessDefenseCnt);

		if (identity != 0)
		{
			int j;
			for (j = 0; j < pSecurityMainDlg->m_wirelessDefenseCnt; j++)
			{
				QString defense;
				int tmp;

				printf("lg j:%d pSecurityMainDlg->m_allDefenseNumber[56 + j]=%d strNumber:%d\n", j,
					 pSecurityMainDlg->m_allDefenseNumber[56 + j], strNumber.toInt());

				if (pSecurityMainDlg->m_allDefenseNumber[56 + j] == strNumber.toInt())  continue; //不包含本防区

				defense = QString("wirelessdefense") + QString::number(pSecurityMainDlg->m_allDefenseNumber[56 + j]);

				tmp = settings.value(defense + "/identity", "0").toInt();

				printf("lg %d tmp_identity:%08x\n", j, tmp);

				if (tmp == identity)
				{
					setAllButtonsEnabled(this, false);
					setAllButtonsEnabled((QDialog *)parent(), false);
					ui->frameDefenseSetting->setFocus(Qt::TabFocusReason);

                    pSecurityMainDlg->m_pSerial1Thread->m_bRun = -1;
                    pSecurityMainDlg->m_pSerial1Thread->start();

                    extern int g_iDeviceType;

                    if(g_iDeviceType == TOUCHR)
                    {
                        printf("TOUCHR enable\n");
                        pSecurityMainDlg->m_pDefenseGPIOScanThread->m_bCopyDataOneTime = true;
                    }

					MyMessageBox messBox(this);
					CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
					okButton->setFocus(Qt::TabFocusReason);
					messBox.setText(tr("该无线模块已在其他防区注册"));
					messBox.show();
					QRect rc = messBox.geometry();
					QRect rc2 = geometry();
					rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
					messBox.setGeometry(rc);
					messBox.exec();
					messBox.hide();
					repaint();

					setAllButtonsEnabled(this, true);
					setAllButtonsEnabled((QDialog *)parent(), true);
                    ui->pushButtonAddDefType->setDisabled(TRUE);

                    ui->pushButtonAddDefSave->setFocus(Qt::TabFocusReason);

					break;
				}
			}

			if (j >= pSecurityMainDlg->m_wirelessDefenseCnt)
			{
				if (i == pSecurityMainDlg->m_wirelessDefenseCnt)
				{
					pSecurityMainDlg->m_wirelessDefenseCnt++;
				}
				else
				{
					delete pSecurityMainDlg->m_delayAlarmClass[56 + i];
				}

				pSecurityMainDlg->m_delayAlarmClass[56 + i] = new DelayAlarmClass(pSecurityMainDlg, (unsigned char)0xa1, (!bBypass) && bDelayAlarm);;
				if (pSecurityMainDlg->m_delayAlarmClass[56 + i]->m_pDelayTimer)
				{
					connect(pSecurityMainDlg->m_delayAlarmClass[56 + i]->m_pDelayTimer, SIGNAL(timeout()), pSecurityMainDlg->m_pSerial1Thread, SLOT(enterDelayTimeOut()));
				}
				pSecurityMainDlg->m_allDefenseNumber[56 + i] = strNumber.toInt();

				QString defense;
				defense = QString("wirelessdefense") + strNumber;

				settings.setValue(defense + "/property", QString::number(m_property));//lg tr(g_strProperty[m_property]));
				settings.setValue(defense + "/channel", QString::number(m_channel));//lg tr("通道") + QString::number(m_channel + 1));
				settings.setValue(defense + "/type", QString::number(m_type));//lg tr(g_strType[m_type]));
				settings.setValue(defense + "/number", strNumber);
				settings.setValue(defense + "/location", strLocation);

				settings.setValue(defense + "/identity", QString::number(identity));
				settings.setValue(defense + "/bypass", bBypass);
				settings.setValue(defense + "/delay_alarm", bDelayAlarm);

                pSecurityMainDlg->m_pSerial1Thread->m_bRun = -1;
                pSecurityMainDlg->m_pSerial1Thread->start();

                extern int g_iDeviceType;

                if(g_iDeviceType == TOUCHR)
                {
                    printf("TOUCHR enable\n");
                    pSecurityMainDlg->m_pDefenseGPIOScanThread->m_bCopyDataOneTime = true;
                }

                ui->frameDefenseSetting->setFocus(Qt::TabFocusReason);
				MessageBox(this, tr("无线防区修改成功\n该防区为第") + strNumber + tr("个无线防区"), QObject::tr("确定"));
                ui->pushButtonAddDefSave->setFocus(Qt::TabFocusReason);
			}
		}
	}
#if 0
	pSecurityMainDlg->m_pSerial1Thread->m_bRun = -1;
	pSecurityMainDlg->m_pSerial1Thread->start();

    extern int g_iDeviceType;

    if(g_iDeviceType == TOUCHR)
    {
        printf("TOUCHR enable\n");
        pSecurityMainDlg->m_pDefenseGPIOScanThread->m_bCopyDataOneTime = true;
    }
#endif
    if (m_property == 0)
    {
        ui->pushButtonAddDefDelayAlarm->setText(tr("否"));
        ui->pushButtonAddDefDelayAlarm->setEnabled(false);
    }
    else
    {
        ui->pushButtonAddDefDelayAlarm->setEnabled(true);
    }

    ui->pushButtonAddDefType->setDisabled(TRUE);
}

/************************************************************
描述：取消防区信息修改并关闭该对话框
参数：无
返回：无
************************************************************/
/*void AddDefenseDialog::on_pushButtonCancel_pressed()
{
	if (!isVisible()) return;

	((SysSetMainDialog *)parent())->m_curDlg->show();
	hide();
}*/

/************************************************************
描述：读取无线防区的识别码
参数：无
返回：非-1 - 无线防区的识别码
	 -1 - 读取无线防区的识别码失败
************************************************************/
int AddDefenseDialog::getWirelessDefenseIdentity()
{
	unsigned char buf[14];
	int d_identity = 0;
	int cnt, cnt1, head_pos;
	int i ;

	setAllButtonsEnabled(this, false);
	setAllButtonsEnabled((QDialog *)parent(), false);
	ui->frameDefenseSetting->setFocus(Qt::TabFocusReason);

	//MessageBox(this, "请按确定后在30秒内打开待\n添加的无线设备以检测ID", "\n　确定　\n");
	MyMessageBox messBox(this);
	CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
	//Q_UNUSED(okButton);
	okButton->setFocus(Qt::TabFocusReason);
	messBox.setText(tr("请按确定后在20秒内打开待\n添加的无线设备以检测ID"));
	messBox.show();
	QRect rc = messBox.geometry();
	QRect rc2 = geometry();
	rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
	messBox.setGeometry(rc);
	messBox.exec();
	messBox.hide();
	repaint();

	Serial1Thread *pSerial1Thread = ((MainDialog *)(parent()->parent()))->m_pSecurityMainDlg->m_pSerial1Thread;
	//pSerial1Thread->m_bRun = 0;
	//pSerial1Thread->quit();
	//pSerial1Thread->wait();
	tcflush(pSerial1Thread->m_serial1_fd, TCIOFLUSH);

	i = 0;
	cnt = 0;
	head_pos = -1;
	memset(buf, 0, 14);
	while (i < 20000/200) //20 seconds
	{
		if (g_pMainDlg->m_watchdog_fd >= 0)
		{
			ioctl(g_pMainDlg->m_watchdog_fd, WDIOC_KEEPALIVE, 0);
		}

		int j;

		cnt1 = read(pSerial1Thread->m_serial1_fd, buf + cnt, 14 - cnt);

		/*for (j = 0; j < cnt1; j++)
		{
			printf("%02x ", buf[cnt + j]);
		}
		if (cnt1 > 0)
		{
			printf("\n");
		}*/

		if (head_pos == -1)
		{
			for (j = 0; j < cnt1; j++)
			{
				if (buf[cnt + j] == (unsigned char)0xf7)
				{
					head_pos = cnt + j;
					break;
				}
			}
		}

		cnt += cnt1;

		if ((head_pos != -1) && (cnt - head_pos >= 14))
		{
			break;
		}

		if (cnt >= 128)
		{
			break;
		}

		i++;
		usleep(200000);
	}

	//pSerial1Thread->start();

	if ((head_pos != -1) && (cnt - head_pos >= 14))
	{
		int sum = 0;
		for (i = 0; i < 14; i++)
		{
			sum += buf[head_pos + i];
		}

		printf("register:");
		for (i = 0; i < 14; i++)
		{
			printf("%02x ", buf[i]);
		}
		printf("\n");

		printf("send 0xfa\n");
		char ch = 0xfa;
		write(pSerial1Thread->m_serial1_fd, &ch, 1);

		if (((sum & 0xff) == 0) && (buf[head_pos] == (unsigned char)0xf7) && (buf[head_pos + 1] == (unsigned char)0x00)
			&& (buf[head_pos + 3] == (unsigned char)0xff) && (buf[head_pos + 4] == (unsigned char)0x01))
		{
			d_identity = (buf[head_pos + 5] << 24) + (buf[head_pos + 6] << 16) + (buf[head_pos + 7] << 8) + buf[head_pos + 8];
		}
		else
		{
			MessageBox(this, tr("接收数据检校失败\n无法获取无线防区ID！"), QObject::tr("确定"));
		}
	}
	else
	{
		MessageBox(this, tr("超时，无法获取无线防区ID！"), QObject::tr("确定"));
	}

	setAllButtonsEnabled(this, true);
	setAllButtonsEnabled((QDialog *)parent(), true);
    ui->pushButtonAddDefSave->setFocus(Qt::TabFocusReason);

	return d_identity;
}

/************************************************************
描述：删除当前显示的防区，删除后除了防区类型栏其它防区信息显示栏被清空
参数：无
返回：无
************************************************************/
void AddDefenseDialog::on_pushButtonAddDefDel_pressed()
{
    if ((ui->pushButtonAddDefChannel->text() == tr("空"))
        || (ui->pushButtonAddDefProperty->text() == tr("空"))
        || (ui->pushButtonAddDefSubType->text() == tr("空"))
		|| (ui->lineEditDefenseLocation->text() == ""))
	{
		return;
	}

	setAllButtonsEnabled(this, false);
	setAllButtonsEnabled((QDialog *)parent(), false);
	ui->frameDefenseSetting->setFocus(Qt::TabFocusReason);

	MyMessageBox messBox(this);
	CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
    CommonPushButton *cancelButton = messBox.addButton(QObject::tr("取消"), "pushButtonMyMessageBoxCancel");
	Q_UNUSED(okButton);
	cancelButton->setFocus(Qt::TabFocusReason);
	messBox.setText(tr("将删除当前防区数据，确定删除?"));
	messBox.show();
	QRect rc = messBox.geometry();
	QRect rc2 = geometry();
	rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
	messBox.setGeometry(rc);
	messBox.exec();

	setAllButtonsEnabled((QDialog *)parent(), true);
	setAllButtonsEnabled(this, true);

    ui->pushButtonAddDefType->setDisabled(TRUE);
    ui->pushButtonAddDefDel->setFocus(Qt::TabFocusReason);

	if(messBox.clickedButton() == cancelButton)
	{
		return;
	}

	QSettings settings_defense_conf("/mnt/disk/config/defense.conf", QSettings::IniFormat);
	settings_defense_conf.setIniCodec(QTextCodec::codecForName("UTF8"));

	QSettings settings_defense("/mnt/disk/config/defense", QSettings::IniFormat);
	SecurityMainDialog *pSecurityMainDlg = ((MainDialog *)(parent()->parent()))->m_pSecurityMainDlg;
	int i;
	int number = ui->labelDefenseNumber->text().toInt();

	if (m_defenseType == 0)
	{
		settings_defense_conf.remove("wiredefense" + QString::number(number));
		settings_defense.remove("wiredefense/defense" + QString::number(number));

		for (i = 0; i < pSecurityMainDlg->m_wireDefenseCnt; i++)
		{
			if (pSecurityMainDlg->m_allDefenseNumber[i] == number)
			{
				break;
			}
		}

		if (i < pSecurityMainDlg->m_wireDefenseCnt)
		{
			delete pSecurityMainDlg->m_delayAlarmClass[i];
			pSecurityMainDlg->m_delayAlarmClass[i] = NULL;

			pSecurityMainDlg->m_wireDefenseCnt--;

			for (; i < pSecurityMainDlg->m_wireDefenseCnt; i++)
			{
				pSecurityMainDlg->m_allDefenseNumber[i] = pSecurityMainDlg->m_allDefenseNumber[i + 1];
				pSecurityMainDlg->m_delayAlarmClass[i] = pSecurityMainDlg->m_delayAlarmClass[i + 1];
			}

			pSecurityMainDlg->m_delayAlarmClass[pSecurityMainDlg->m_wireDefenseCnt] = NULL;
		}

		for (i = 0; i < pSecurityMainDlg->m_wireDefenseCnt; i++)
		{
			printf("wire number %d %d\n", i, pSecurityMainDlg->m_allDefenseNumber[i]);
		}
	}
	else
	{
		settings_defense_conf.remove("wirelessdefense" + QString::number(number));
		settings_defense.remove("wirelessdefense/defense" + QString::number(number));

		for (i = 0; i < pSecurityMainDlg->m_wirelessDefenseCnt; i++)
		{
			if (pSecurityMainDlg->m_allDefenseNumber[56 + i] == number)
			{
				break;
			}
		}

		if (i < pSecurityMainDlg->m_wirelessDefenseCnt)
		{
			pSecurityMainDlg->m_wirelessDefenseCnt--;

			for (; i < pSecurityMainDlg->m_wirelessDefenseCnt; i++)
			{
				pSecurityMainDlg->m_allDefenseNumber[56 + i] = pSecurityMainDlg->m_allDefenseNumber[56 + i + 1];
			}
		}

		for (i = 0; i < pSecurityMainDlg->m_wirelessDefenseCnt; i++)
		{
			printf("wireless number %d %d\n", i, pSecurityMainDlg->m_allDefenseNumber[56 + i]);
		}
	}

	m_channel = -1;
	m_property = -1;
	m_type = -1;

    ui->pushButtonAddDefChannel->setText(tr("空"));
    ui->pushButtonAddDefProperty->setText(tr("空"));
    ui->pushButtonAddDefSubType->setText(tr("空"));
	ui->lineEditDefenseLocation->setText("");
    ui->pushButtonAddDefDelayAlarm->setText("否");
    ui->pushButtonAddDefDelayAlarm->setEnabled(true);
    ui->pushButtonAddDefBypass->setText("否");
}

/************************************************************
描述：向前切换防区号码(支持回滚，范围为1-56)，同时根据新的防区号显示防
	 区信息
参数：无
返回：无
************************************************************/
void AddDefenseDialog::on_pushButtonAddDefPrev_pressed()
{
	int number = ui->labelDefenseNumber->text().toInt();
	if (number > 1)
	{
		number--;
	}
	else
	{
		number = 56;
	}
	showCurrentItem(m_defenseType, number);
}

/************************************************************
描述：向后切换防区号码(支持回滚，范围为1-56)，同时根据新的防区号显示防
	 区信息
参数：无
返回：无
************************************************************/
void AddDefenseDialog::on_pushButtonAddDefNext_pressed()
{
	int number = ui->labelDefenseNumber->text().toInt();
	if (number < 56)
	{
		number++;
	}
	else
	{
		number = 1;
	}

	showCurrentItem(m_defenseType, number);
}

/************************************************************
描述：根据防区类型和防区号从配置文件中读取防区信息并显示在本窗口的相关
	 栏上
参数：defenseType - 防区类型(有线/无线)
	 defenseNumber - 防区号码
返回：无
************************************************************/
void AddDefenseDialog::showCurrentItem(int defenseType, int defenseNumber)
{
	QSettings settings("/mnt/disk/config/defense.conf", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF8"));

	QString sChannel;
	QString sProperty;
	QString sType;
	QString sLocation;
	QString sDefense;
	int isDelayAlarm;
	int isBypass;

	printf("lg defenseNumber:%d\n", defenseNumber);

	if (defenseType == 0)
	{
		sDefense = QString("wiredefense") + QString::number(defenseNumber);
	}
	else
	{
		sDefense = QString("wirelessdefense") + QString::number(defenseNumber);
	}

	m_channel = settings.value(sDefense + "/channel", "-1").toInt();
	m_property = settings.value(sDefense + "/property", "-1").toInt();
	m_type = settings.value(sDefense + "/type", "-1").toInt();
	sLocation = settings.value(sDefense + "/location", "").toString();
	isDelayAlarm = settings.value(sDefense + "/delay_alarm", "0").toInt();
	isBypass = settings.value(sDefense + "/bypass", "0").toInt();

	if (m_channel != -1)
	{
		sChannel = tr("通道") + QString::number(m_channel + 1);
	}
	else
	{
		sChannel = tr("空");
	}

	if (m_property != -1)
	{
		sProperty = QObject::tr(g_strProperty[m_property]);
	}
	else
	{
		sProperty = tr("空");
	}

	if (m_type != -1)
	{
		sType = QObject::tr(g_strType[m_type]);
	}
	else
	{
		sType = tr("空");
	}

    ui->pushButtonAddDefChannel->setText(sChannel);
    ui->pushButtonAddDefProperty->setText(sProperty);
    ui->pushButtonAddDefSubType->setText(sType);

    ui->pushButtonAddDefType->setText(defenseType == 0 ? tr("有线防区") : tr("无线防区"));//[defenseType]));
	ui->labelDefenseNumber->setText(QString::number(defenseNumber));

	ui->lineEditDefenseLocation->setText(sLocation);

    if (m_property == 0)
	{
        ui->pushButtonAddDefDelayAlarm->setText(tr("否"));
        ui->pushButtonAddDefDelayAlarm->setEnabled(false);
	}
	else
	{
        ui->pushButtonAddDefDelayAlarm->setText(isDelayAlarm ? tr("是") : tr("否"));
        ui->pushButtonAddDefDelayAlarm->setEnabled(true);
	}

    ui->pushButtonAddDefBypass->setText(isBypass ? tr("是") : tr("否"));

	m_defenseType = defenseType;
}

/************************************************************
描述：窗口显示时，从配置文件读入第一个有线防区的信息并显示
参数：无
返回：无
************************************************************/
void AddDefenseDialog::showEvent(QShowEvent *)
{
	//MainDialog *pMainDlg = (MainDialog *)(parent()->parent());

    g_pMainDlg->showRefreshTabFocus(this);

	//pMainDlg->refreshTabFocus();
	showCurrentItem(0, 1);

    //ui->pushButtonAddDefNext->setFocus(Qt::TabFocusReason);
	//ui->pushButtonDefenseTypeSub->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：窗口隐藏时，设备设置对话框中的"防区"按钮设置为焦点控件
参数：无
返回：无
************************************************************/
void AddDefenseDialog::hideEvent(QHideEvent *)
{
    g_pMainDlg->hideRefreshTabFocus(this);

	//MainDialog *pMainDlg = (MainDialog *)(parent()->parent());
	//pMainDlg->refreshTabFocus();

	//SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();
	//pSysSetMainDlg->ui->pushButtonDeviceSet->setFocus(Qt::TabFocusReason);
	m_pAbnormalMsgBox->hide();

	////lg
	//if (QApplication::focusWidget()->parent() != this) return;
	//((SysSetMainDialog *)parent())->m_pDeviceSetMainDlg->ui->pushButtonDefense->setFocus(Qt::TabFocusReason);
	////lg
}

void AddDefenseDialog::on_pushButtonAddDefType_pressed()
{
	if (m_defenseType < 1)
	{
		m_defenseType++;
	}
	else
	{
		m_defenseType = 0;
	}

	showCurrentItem(m_defenseType, ui->labelDefenseNumber->text().toInt());
}

void AddDefenseDialog::on_pushButtonAddDefChannel_pressed()
{
	if (m_channel < 3)
	{
		m_channel++;
	}
	else
	{
		m_channel = 0;
	}

    ui->pushButtonAddDefChannel->setText(tr("通道") + QString::number(m_channel + 1));
}

void AddDefenseDialog::on_pushButtonAddDefProperty_pressed()
{
	if (m_property < 2)
	{
		m_property++;
	}
	else
	{
		m_property = 0;
	}

	if (m_property == 0)
	{
        ui->pushButtonAddDefDelayAlarm->setText(tr("否"));
        ui->pushButtonAddDefDelayAlarm->setEnabled(false);
	}
	else
	{
        ui->pushButtonAddDefDelayAlarm->setEnabled(true);
	}

	ui->pushButtonAddDefProperty->setText(QObject::tr(g_strProperty[m_property]));
}

void AddDefenseDialog::on_pushButtonAddDefSubType_pressed()
{
	if (m_type < 5)
	{
		m_type++;
	}
	else
	{
		m_type = 0;
	}

	ui->pushButtonAddDefSubType->setText(QObject::tr(g_strType[m_type]));
}

void AddDefenseDialog::on_pushButtonAddDefDelayAlarm_pressed()
{
	if (ui->pushButtonAddDefDelayAlarm->text() == tr("是"))
	{
		ui->pushButtonAddDefDelayAlarm->setText(tr("否"));
	}
	else
	{
		ui->pushButtonAddDefDelayAlarm->setText(tr("是"));
	}
}

void AddDefenseDialog::on_pushButtonAddDefBypass_pressed()
{
	if (ui->pushButtonAddDefBypass->text() == tr("是"))
	{
		ui->pushButtonAddDefBypass->setText(tr("否"));
	}
	else
	{
		ui->pushButtonAddDefBypass->setText(tr("是"));
	}
}

void AddDefenseDialog::on_pushButtonAddDefCalibrate_pressed()
{
#ifdef ARM11
	g_pMainDlg->m_pCalibrationDlg->exec();					// A20 使用电容屏去掉
#endif
}

void AddDefenseDialog::on_pushButtonAddDefReset_pressed()
{
	setAllButtonsEnabled(this, false);
	setAllButtonsEnabled((QDialog *)parent(), false);
	ui->frameDefenseSetting->setFocus(Qt::TabFocusReason);

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
    ui->pushButtonAddDefType->setDisabled(TRUE);

	ui->pushButtonAddDefReset->setFocus(Qt::TabFocusReason);

	if(messBox.clickedButton() == cancelButton)
	{
		return;
	}

	restore_config_from_last_file();				// 进行配置文件的出场设置的回复，注意工程参数不变

	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());
	{
		QSettings settings_config(CONFIG_FILE, QSettings::IniFormat);

		int ringVolume = 70;
		int volume     = 70;

		QString strDialRingFile       = settings_config.value("rings/dial").toString();
		QString strCallInRingFile     = settings_config.value("rings/callin").toString();
		QString strDoorRingFile       = settings_config.value("rings/doorway").toString();
		QString strManCenter1RingFile = settings_config.value("rings/man_center1").toString();
		QString strManCenter2RingFile = settings_config.value("rings/man_center2").toString();

		ringVolume = settings_config.value("rings/ringvolume").toInt();
		ringVolume = static_cast<int>(static_cast<float>(ringVolume)*1024.0/100); 
		volume = settings_config.value("rings/volume").toInt();
		g_pScaleSrc->set_doorbell_ring_value(volume);			// 设置二次门铃的音量
		volume = static_cast<int>(static_cast<float>(volume)*1024.0/100); 

		strcpy(g_strLphoneRingFile[0], ("rings/" + strDialRingFile + ".wav").toLatin1().data());
		strcpy(g_strLphoneRingFile[1], ("rings/" + strCallInRingFile + ".wav").toLatin1().data());
		strcpy(g_strLphoneRingFile[2], ("rings/" + strDoorRingFile).toLatin1().data());
		strcpy(g_strLphoneRingFile[3], ("rings/" + strManCenter1RingFile + ".wav").toLatin1().data());
		strcpy(g_strLphoneRingFile[4], ("rings/" + strManCenter2RingFile + ".wav").toLatin1().data());

		pMainDlg->m_pVoipMainDlg->m_nRingVolume = static_cast<int>(static_cast<float>(ringVolume) * Kevin::Global::RATIO);
		pMainDlg->m_pVoipMainDlg->m_nVolume = volume;

        QString lang = settings_config.value("language/lang").toString();
        pMainDlg->m_pSysSetMainDlg->m_pLanguageSetDlg->setLanguage(lang);
        ui->pushButtonAddDefChannel->setText("空");
        ui->pushButtonAddDefProperty->setText("空");
        ui->pushButtonAddDefSubType->setText("空");
        ui->pushButtonAddDefDelayAlarm->setText("否");
        ui->pushButtonAddDefBypass->setText("否");
        ui->lineEditDefenseLocation->setText("");

		int timeMode = 1;
		timeMode = settings_config.value("datetime/time_mode").toInt();
		pMainDlg->setTimeMode(timeMode);
		pMainDlg->ui->lcdNumberDigClock->hide();
		pMainDlg->ui->widgetClock->hide();

		int state = STATUS_GOOUT;
		state = settings_config.value("alert_param/security_state").toInt();
		pMainDlg->m_pSecurityMainDlg->m_newDefenseCnt = 0;
		pMainDlg->m_pMediaManDlg->m_newVisitorCnt = 0;
		pMainDlg->m_pInfoManMainDlg->m_newInfoCnt = 0;

		pMainDlg->m_pSecurityMainDlg->m_securityState = state;
		pMainDlg->m_pSecurityMainDlg->setSecurityState(state);
        pMainDlg->m_pSecurityMainDlg->emitsigSetSecurityStatueToPhone(state);
        if(pMainDlg->m_pSecurityMainDlg->m_pUploadSetSecurityStateThread->m_bRun)
        {
            pMainDlg->m_pSecurityMainDlg->m_pUploadSetSecurityStateThread->m_bRun = false;
            pMainDlg->m_pSecurityMainDlg->m_pUploadSetSecurityStateThread->quit();
            pMainDlg->m_pSecurityMainDlg->m_pUploadSetSecurityStateThread->wait();
        }
        pMainDlg->m_pSecurityMainDlg->m_pUploadSetSecurityStateThread->start();

		QStandardItemModel *model = (QStandardItemModel *)g_pMainDlg->ui->tableViewNotifier->model();
		model->item(3, 1)->setIcon(QIcon(":/images/main/call_none.png"));
		model->item(3, 2)->setText(MainDialog::tr("无未接呼叫"));
		model->item(4, 1)->setIcon(QIcon(":/images/main/alarm_none.png"));
		model->item(4, 2)->setText(MainDialog::tr("无未接警报"));
		model->item(5, 1)->setIcon(QIcon(":/images/main/information_none.png"));
		model->item(5, 2)->setText(MainDialog::tr("无未查看信息"));

		int iEnterDelay = 40;
		int iExitDelay = 60;
		int iRepeatAlarmFilterInterval = 60;

		iEnterDelay = settings_config.value("alert_param/enter_delay").toInt();
		iExitDelay = settings_config.value("alert_param/exit_delay").toInt();
		iRepeatAlarmFilterInterval= settings_config.value("alert_param/repeat_alarm_filter_interval").toInt();

		pMainDlg->m_pSecurityMainDlg->m_iEnterDelay = iEnterDelay;
		pMainDlg->m_pSecurityMainDlg->m_iExitDelay = iExitDelay;
		pMainDlg->m_pSecurityMainDlg->m_iRepeatAlarmFilterInterval = iRepeatAlarmFilterInterval;

		settings_config.setValue("alert_param/special_defense","0");			// 当恢复出厂设置的时候设置为无

        settings_config.setValue("alert_param/doorbell_type","0");			// 当恢复出厂设置的时候设置为无

		g_door_bell_type = 0;							        // 立即生效
	}

	{
		QSettings settings("/mnt/disk/config/defense.conf", QSettings::IniFormat);
		settings.setIniCodec(QTextCodec::codecForName("UTF8"));
		settings.clear();

        g_pMainDlg->m_pSecurityMainDlg->m_wireDefenseCnt=0;
        g_pMainDlg->m_pSecurityMainDlg->m_wirelessDefenseCnt=0;          //防区设置清空

		int rowCnt = g_pMainDlg->m_pSecurityMainDlg->ui->tableViewSecurityList->model()->rowCount();
		int i;
		for (i = 0; i < rowCnt; i++)
		{
			g_pMainDlg->m_pSecurityMainDlg->delSecurityRecord(0);
		}
		system("/bin/rm /mnt/disk/sec_info/*.txt");
		system("/bin/rm /mnt/disk/sec_media/*.avi");
	}

	{
		QStandardItemModel *model = (QStandardItemModel *)g_pMainDlg->m_pInfoManMainDlg->ui->tableViewInfoList->model();

		model->removeRows(0, model->rowCount());

		system("/bin/rm /mnt/disk/info/*.*");
	}
///  内部使用，如果用户按下系统复位按钮，生成某个表识文件
	char* cmd = new char[200];
	sprintf(cmd,"touch /%s","System_Reset_Already");
	psystem(cmd);
	sync();
	delete[] cmd;
	cmd = NULL;
///  内部使用，如果用户按下系统复位按钮，生成某个表识文件
	{
		int i;

		QFile file("/mnt/disk/media/record.xml");

        QDomDocument doc;

        if (!file.open(QIODevice::ReadOnly)) return;

        if (!doc.setContent(&file))
		{
			file.close();
            //return;
            //david 2014.5.12
            FILE *fp;

            char msg[] = "<?xml version='1.0' encoding='UTF-8'?>\n<sysrecord>\n<record duration= \"duration\" datetime= \"datetime\" aviurl= \"aviurl\" type=\"type\" ipaddress=\"ipaddress\" state=\"state\" />\n</sysrecord>";

            fp = fopen("/mnt/disk/media/record.xml","wb+");

            if(NULL == fp)
            {
                printf("open record.xml failed\n");
                return;
            }

            fwrite(msg,strlen(msg),1,fp);

            fclose(fp);
		}

		file.close();

		QDomElement root = doc.documentElement();
		QDomNodeList list = doc.elementsByTagName("record");
		int cnt = list.count();

		for (i = 1; i < cnt; i++)
		{
			root.removeChild(list.at(1));
		}

		if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return;
		QTextStream out(&file);
		doc.save(out, 4);
		file.close();

		QStandardItemModel *model;
		model = (QStandardItemModel *)g_pMainDlg->m_pMediaManDlg->ui->tableViewMediaList->model();
		model->removeRows(0, model->rowCount());

		system("/bin/rm /mnt/disk/media/*.avi");
	}
	g_pScaleSrc->Backup_Config_File();
}

void AddDefenseDialog::on_pushButtonAddDefReboot_pressed()
{
	setAllButtonsEnabled(this, false);
	setAllButtonsEnabled((QDialog *)parent(), false);
	ui->frameDefenseSetting->setFocus(Qt::TabFocusReason);

	MyMessageBox messBox(this);
	CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
	CommonPushButton *cancelButton = messBox.addButton(QObject::tr("取消"), "pushButtonMyMessageBoxCancel");
    //Q_UNUSED(okButton);
	cancelButton->setFocus(Qt::TabFocusReason);
	messBox.setText(tr("重启系统，确定重启?"));
	messBox.show();
	QRect rc = messBox.geometry();
	QRect rc2 = geometry();
	rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
	messBox.setGeometry(rc);
	messBox.exec();

	setAllButtonsEnabled((QDialog *)parent(), true);
	setAllButtonsEnabled(this, true);

    ui->pushButtonAddDefType->setDisabled(TRUE);
	ui->pushButtonAddDefReset->setFocus(Qt::TabFocusReason);

//	if(messBox.clickedButton() == cancelButton)
//	{
//		return;
//	}
    if(messBox.clickedButton()!=okButton)
    {
        return ;
    }

	systemRunLog(1);

	system("reboot");
}

/************************************************************
描述：返回调用者
参数：无
返回：无
************************************************************/
void AddDefenseDialog::on_pushButtonAddDefReturn_pressed()
{
    hide();
    SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();
    pSysSetMainDlg->on_pushButtonSysSetMainVersion_pressed();
}
