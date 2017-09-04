#include "ringandvolumedialog.h"
#include "ui_ringandvolumedialog.h"
#include "syssetmaindialog.h"
#include "maindialog.h"
#include "ui_syssetmaindialog.h"
#include "ui_ringfilesetdialog.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

/************************************************************
  该文件为铃声和音量设置对话框主程序
************************************************************/

extern MainDialog *g_pMainDlg;

static int 	MAXVALUE      = 60;		// 根据不同的分辨率大小不同
static float    RINGADJVALUE  = 1024.0;		// 最大的音量1024.0 其他根据调整,这个是播放音乐的音频
static float    VOICEADJVALUE = 1024.0;		// 语音的播放保持原来不变

extern int psystem(const char *cmd);

/************************************************************
描述：铃声和音量设置对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
RingAndVolumeDialog::RingAndVolumeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RingAndVolumeDialog)
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
        ui->frameRingAmdVolumeSetting->setGeometry(QRect(44, 12, 700, 580));

        //标题
        ui->label_title->setGeometry(QRect(139, 22, 510, 50));

        //第一列
        ui->label_ringvalue_1->setGeometry(QRect(44, 85, 280, 70));
        ui->label_ringvalue_2->setGeometry(QRect(44, 155, 280, 70));
        ui->label_ringvalue_3->setGeometry(QRect(44, 225, 280, 70));
        ui->label_ringvalue_4->setGeometry(QRect(44, 295, 280, 70));
        ui->label_ringvalue_5->setGeometry(QRect(44, 365, 280, 70));
        ui->label_ringvalue_6->setGeometry(QRect(44, 435, 280, 70));
        ui->label_ringvalue_7->setGeometry(QRect(44, 505, 280, 70));

        //第二列
        ui->horizontalSliderRingVolume->setGeometry(QRect(373, 85, 321, 70));
        ui->horizontalSliderVolume->setGeometry(QRect(373, 155, 321, 70));
        ui->pushButtonRingVolumeDail->setGeometry(QRect(324, 225, 420, 70));
        ui->pushButtonRingVolumeCallIn->setGeometry(QRect(324, 295, 420, 70));
        ui->pushButtonRingVolumeDoor->setGeometry(QRect(324, 365, 420, 70));
        ui->pushButtonRingVolumeCenter1->setGeometry(QRect(324, 435, 420, 70));
        ui->pushButtonRingVolumeCenter2->setGeometry(QRect(324, 505, 420, 70));

        //右侧按键
        ui->pushButtonRingVolumeSave->setGeometry(QRect(790, 380, 200, 100));
        ui->pushButtonRingVolumeReturn->setGeometry(QRect(790, 480, 200, 100));
    }
	else if ( res == 3 )  // 1280x800
	{
		resize(1280,800);
        ui->framePanel->setGeometry(QRect(59, 30, 858, 745));
        ui->frameRingAmdVolumeSetting->setGeometry(QRect(59, 30, 858, 745));

        //标题
        ui->label_title->setGeometry(QRect(219, 40, 510, 70));

        //第一列
        ui->label_ringvalue_1->setGeometry(QRect(59, 120, 330, 93));
        ui->label_ringvalue_2->setGeometry(QRect(59, 213, 330, 93));
        ui->label_ringvalue_3->setGeometry(QRect(59, 306, 330, 93));
        ui->label_ringvalue_4->setGeometry(QRect(59, 399, 330, 93));
        ui->label_ringvalue_5->setGeometry(QRect(59, 492, 330, 93));
        ui->label_ringvalue_6->setGeometry(QRect(59, 585, 330, 93));
        ui->label_ringvalue_7->setGeometry(QRect(59, 678, 330, 93));

        //第二列
        ui->horizontalSliderRingVolume->setGeometry(QRect(436, 120, 441, 93));
        ui->horizontalSliderVolume->setGeometry(QRect(436, 213, 441, 93));
        ui->pushButtonRingVolumeDail->setGeometry(QRect(389, 306, 528, 93));
        ui->pushButtonRingVolumeCallIn->setGeometry(QRect(389, 399, 528, 93));
        ui->pushButtonRingVolumeDoor->setGeometry(QRect(389, 492, 528, 93));
        ui->pushButtonRingVolumeCenter1->setGeometry(QRect(389, 585, 528, 93));
        ui->pushButtonRingVolumeCenter2->setGeometry(QRect(389, 678, 528, 93));

        //右侧按键
        ui->pushButtonRingVolumeSave->setGeometry(QRect(980, 530, 246, 112));
        ui->pushButtonRingVolumeReturn->setGeometry(QRect(980, 660, 246, 112));

		ui->pushButtonRingVolumeDail->setStyleSheet(QString::fromUtf8("font: 26px;"));
		ui->label_ringvalue_3->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->pushButtonRingVolumeCallIn->setStyleSheet(QString::fromUtf8("font: 26px;"));
		ui->pushButtonRingVolumeDoor->setStyleSheet(QString::fromUtf8("font: 26px;"));
		ui->label_ringvalue_1->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->label_ringvalue_2->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->label_ringvalue_5->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->label_ringvalue_7->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->label_ringvalue_6->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->label_ringvalue_4->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->pushButtonRingVolumeCenter2->setStyleSheet(QString::fromUtf8("font: 26px;"));
		ui->pushButtonRingVolumeCenter1->setStyleSheet(QString::fromUtf8("font: 26px;"));
	}

	ui->pushButtonRingVolumeCallIn->setText("");
	ui->pushButtonRingVolumeDail->setText("");
	ui->pushButtonRingVolumeDoor->setText("");
	ui->pushButtonRingVolumeCenter1->setText("");
	ui->pushButtonRingVolumeCenter2->setText("");

	ui->pushButtonRingVolumeCallIn->m_offsetX = 0;
	ui->pushButtonRingVolumeDail->m_offsetX = 0;
	ui->pushButtonRingVolumeDoor->m_offsetX = 0;
	ui->pushButtonRingVolumeCenter1->m_offsetX = 0;
	ui->pushButtonRingVolumeCenter2->m_offsetX = 0;

	setAttribute(Qt::WA_TranslucentBackground, false);

	if ( g_pScaleSrc->get_screen_version() == 1 )
	{
		MAXVALUE = 60;
		ui->horizontalSliderRingVolume->setRange(0, MAXVALUE);
		ui->horizontalSliderVolume->setRange(0, MAXVALUE);
	}
	else if ( g_pScaleSrc->get_screen_version() == 2 )
	{
		MAXVALUE = 80;
		ui->horizontalSliderRingVolume->setRange(0, MAXVALUE);
		ui->horizontalSliderVolume->setRange(0, MAXVALUE);
	}
	else if ( g_pScaleSrc->get_screen_version() == 3 )
	{
		MAXVALUE = 100;
		ui->horizontalSliderRingVolume->setRange(0, MAXVALUE);
		ui->horizontalSliderVolume->setRange(0, MAXVALUE);
	}

	ui->horizontalSliderRingVolume->setFocusPolicy(Qt::NoFocus);
	ui->horizontalSliderVolume->setFocusPolicy(Qt::NoFocus);

	m_pRingFileSetDlg = new RingFileSetDialog(parent);
	m_pRingFileSetDlg->loadRingFile();

	m_pDoorBellRingFileSetDlg = new RingFileSetDialog(parent);
	m_pDoorBellRingFileSetDlg->loadDoorBellRingFile();
}

/************************************************************
描述：铃声和音量设置对话框析构函数
参数：无
返回：无
************************************************************/
RingAndVolumeDialog::~RingAndVolumeDialog()
{
	delete m_pRingFileSetDlg;
	delete m_pDoorBellRingFileSetDlg;
	delete ui;
}

/************************************************************
描述：从config文件读入铃声设置，更新到行编辑控件中。从系统读取对讲和
	 铃声音量，更新到滑块控件中
参数：无
返回：无
************************************************************/
void RingAndVolumeDialog::loadData()
{
	QSettings settings(CONFIG_FILE, QSettings::IniFormat);
	QString strDialRingFile;
	QString strCallInRingFile;
	QString strDoorRingFile;
	QString strManCenter1RingFile;
	QString strManCenter2RingFile;
	int ringVolume;
	int volume;
	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());

	strDialRingFile = settings.value("rings/dial", "ringback").toString();
	strCallInRingFile = settings.value("rings/callin", "oldphone").toString();
	strDoorRingFile = settings.value("rings/doorway", "ring1").toString();
	strManCenter1RingFile = settings.value("rings/man_center1", "oldphone").toString();
	strManCenter2RingFile = settings.value("rings/man_center2", "oldphone").toString();

	ringVolume = static_cast<int>(static_cast<float>(pMainDlg->m_pVoipMainDlg->m_nRingVolume) / Kevin::Global::RATIO);
	if ( ringVolume > (RINGADJVALUE-5) ) ringVolume = static_cast<int>(RINGADJVALUE);   // -5是为了四舍五入

	volume = pMainDlg->m_pVoipMainDlg->m_nVolume;

	ui->pushButtonRingVolumeDail->setText(strDialRingFile);
	ui->pushButtonRingVolumeCallIn->setText(strCallInRingFile);
	ui->pushButtonRingVolumeDoor->setText(strDoorRingFile);
	ui->pushButtonRingVolumeCenter1->setText(strManCenter1RingFile);
	ui->pushButtonRingVolumeCenter2->setText(strManCenter2RingFile);

	ui->horizontalSliderRingVolume->setValue((int)(ringVolume * MAXVALUE/RINGADJVALUE));
	ui->horizontalSliderVolume->setValue((int)(volume *MAXVALUE/VOICEADJVALUE));

	int t = (int)(ringVolume * MAXVALUE/RINGADJVALUE);
	if ( t == 0 )								//  与默认数值相等，需要强制设置数值  kevin
		on_horizontalSliderRingVolume_valueChanged(0);

	t = (int)(volume * MAXVALUE/VOICEADJVALUE);
	if ( t == 0 )
		on_horizontalSliderVolume_valueChanged(0);
}

/************************************************************
描述：打开拨号铃声选择对话框
参数：无
返回：无
************************************************************/
void RingAndVolumeDialog::on_pushButtonRingVolumeDail_pressed()
{
	if (!isVisible()) return;

	hide();

	m_pRingFileSetDlg->selectRingFile(ui->pushButtonRingVolumeDail->text() + ".wav");

	m_pRingFileSetDlg->m_pPushButton = ui->pushButtonRingVolumeDail;
	m_pRingFileSetDlg->show();
    //m_pRingFileSetDlg->ui->pushButtonRingFileSetSave->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：打开来电铃声选择对话框
参数：无
返回：无
************************************************************/
void RingAndVolumeDialog::on_pushButtonRingVolumeCallIn_pressed()
{
	if (!isVisible()) return;

	hide();

//	if ( g_pScaleSrc->get_screen_version() != 1 )					// kevin 发现没意义 暂时屏蔽
//	{
//		SysSetMainDialog *pMainDlg = (SysSetMainDialog *)(parent());
//	}

	m_pRingFileSetDlg->selectRingFile(ui->pushButtonRingVolumeCallIn->text() + ".wav");
	//m_pRingFileSetDlg->m_pLineEdit = ui->lineEditCallInRingFile;
	m_pRingFileSetDlg->m_pPushButton = ui->pushButtonRingVolumeCallIn;
	m_pRingFileSetDlg->show();
    //m_pRingFileSetDlg->ui->pushButtonRingFileSetSave->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：打开门口机来电铃声选择对话框
参数：无
返回：无
************************************************************/
void RingAndVolumeDialog::on_pushButtonRingVolumeDoor_pressed()
{
	if (!isVisible()) return;

	hide();

	m_pDoorBellRingFileSetDlg->selectDoorBellFile(ui->pushButtonRingVolumeDoor->text());
        m_pDoorBellRingFileSetDlg->m_pPushButton = ui->pushButtonRingVolumeDoor;
    m_pDoorBellRingFileSetDlg->show();

//	m_pRingFileSetDlg->selectRingFile(ui->pushButtonRingVolumeDoor->text() + ".wav");
//	m_pRingFileSetDlg->m_pPushButton = ui->pushButtonRingVolumeDoor;
//	m_pRingFileSetDlg->show();
}

/************************************************************
描述：打开管理中心机来电铃声选择对话框
参数：无
返回：无
************************************************************/
void RingAndVolumeDialog::on_pushButtonRingVolumeCenter1_pressed()
{
	if (!isVisible()) return;

	hide();

//	if ( g_pScaleSrc->get_screen_version() != 1 )					 // kevin 发现没意义 暂时屏蔽
//	{
//		SysSetMainDialog *pMainDlg = (SysSetMainDialog *)(parent());
//	}

	m_pRingFileSetDlg->selectRingFile(ui->pushButtonRingVolumeCenter1->text() + ".wav");
	//m_pRingFileSetDlg->m_pLineEdit = ui->lineEditManCenter1RingFile;
	m_pRingFileSetDlg->m_pPushButton = ui->pushButtonRingVolumeCenter1;
	m_pRingFileSetDlg->show();
    //m_pRingFileSetDlg->ui->pushButtonRingFileSetSave->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：打开管理中心计算机来电铃声选择对话框
参数：无
返回：无
************************************************************/
void RingAndVolumeDialog::on_pushButtonRingVolumeCenter2_pressed()
{
	if (!isVisible()) return;

	hide();

//	if ( g_pScaleSrc->get_screen_version() != 1 )					 // kevin 发现没意义 暂时屏蔽
//	{
//		SysSetMainDialog *pMainDlg = (SysSetMainDialog *)(parent());
//	}

	m_pRingFileSetDlg->selectRingFile(ui->pushButtonRingVolumeCenter2->text() + ".wav");
	//m_pRingFileSetDlg->m_pLineEdit = ui->lineEditManCenter2RingFile;
	m_pRingFileSetDlg->m_pPushButton = ui->pushButtonRingVolumeCenter2;
	m_pRingFileSetDlg->show();
    //m_pRingFileSetDlg->ui->pushButtonRingFileSetSave->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：铃声音量滑动条按动触发回调函数
参数：value - 音量，范围为0-60，
返回：无
************************************************************/
void RingAndVolumeDialog::on_horizontalSliderRingVolume_valueChanged(int value)
{
	m_nRingVolume = (unsigned int)(value * RINGADJVALUE/MAXVALUE);
}

/************************************************************
描述：对讲音量滑动条按动触发回调函数
参数：value - 音量，范围为0-60，
返回：无
************************************************************/
void RingAndVolumeDialog::on_horizontalSliderVolume_valueChanged(int value)
{
    m_nTalkVolume = (unsigned int)(value * VOICEADJVALUE/MAXVALUE);
}

/************************************************************
描述：铃声音量减小按钮回调函数，将铃声音量减小10(范围为0-60)
参数：无
返回：无
************************************************************/
/*void RingAndVolumeDialog::on_pushButtonRingVolumeSub_pressed()
{
	ui->horizontalSliderRingVolume->triggerAction(QAbstractSlider::SliderPageStepSub);
}
*/
/************************************************************
描述：铃声音量增大按钮回调函数，将铃声音量增大10(范围为0-60)
参数：无
返回：无
************************************************************/
/*void RingAndVolumeDialog::on_pushButtonRingVolumeAdd_pressed()
{
	ui->horizontalSliderRingVolume->triggerAction(QAbstractSlider::SliderPageStepAdd);
}
*/
/************************************************************
描述：对讲音量减小按钮回调函数，将对讲音量减小10(范围为0-60)
参数：无
返回：无
************************************************************/
/*void RingAndVolumeDialog::on_pushButtonVolumeSub_pressed()
{
	ui->horizontalSliderVolume->triggerAction(QAbstractSlider::SliderPageStepSub);
}
*/
/************************************************************
描述：对讲音量增大按钮回调函数，将对讲音量增大10(范围为0-60)
参数：无
返回：无
************************************************************/
/*void RingAndVolumeDialog::on_pushButtonVolumeAdd_pressed()
{
	ui->horizontalSliderVolume->triggerAction(QAbstractSlider::SliderPageStepAdd);
}
*/

void RingAndVolumeDialog::on_pushButtonRingVolumeReturn_pressed()
{
	hide();
	SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();
	pSysSetMainDlg->on_pushButtonSysSetMainVersion_pressed();
}

/************************************************************
描述：将修改后的铃声和音量写入config文件
参数：无
返回：无
************************************************************/
void RingAndVolumeDialog::on_pushButtonRingVolumeSave_pressed()
{
    if(!isVisible())
        return ;
	VoipMainDialog *pVMDlg = ((MainDialog *)(parent()->parent()))->m_pVoipMainDlg;
	QSettings settings(CONFIG_FILE, QSettings::IniFormat);
	QString strDialRingFile;
	QString strCallInRingFile;
	QString strDoorRingFile;
	QString strManCenter1RingFile;
	QString strManCenter2RingFile;

    ui->horizontalSliderRingVolume->setEnabled(false);
    ui->horizontalSliderVolume->setEnabled(false);
	setAllButtonsEnabled(this, false);
	setAllButtonsEnabled((QDialog *)parent(), false);
	ui->frameRingAmdVolumeSetting->setFocus(Qt::TabFocusReason);

	strDialRingFile = ui->pushButtonRingVolumeDail->text();
	strCallInRingFile = ui->pushButtonRingVolumeCallIn->text();
	strDoorRingFile = ui->pushButtonRingVolumeDoor->text();
	strManCenter1RingFile = ui->pushButtonRingVolumeCenter1->text();
	strManCenter2RingFile = ui->pushButtonRingVolumeCenter2->text();

	settings.setValue("rings/dial", strDialRingFile);
	settings.setValue("rings/callin", strCallInRingFile);
	settings.setValue("rings/doorway", strDoorRingFile);
	settings.setValue("rings/man_center1", strManCenter1RingFile);
	settings.setValue("rings/man_center2", strManCenter2RingFile);

	pVMDlg->m_nRingVolume = static_cast<int>(static_cast<float>(m_nRingVolume) * Kevin::Global::RATIO);
	pVMDlg->m_nVolume     = m_nTalkVolume;

	settings.setValue("rings/ringvolume", static_cast<int>(static_cast<float>(m_nRingVolume) * 100 / 1024.0) );
	settings.setValue("rings/volume", static_cast<int>(static_cast<float>(m_nTalkVolume) * 100 / 1024.0));

	int v = static_cast<int>(static_cast<float>(m_nTalkVolume) * 100 / 1024.0);
	g_pScaleSrc->set_doorbell_ring_value(v);				// 设置二次门铃的音量

	extern char g_strLphoneRingFile[5][50];
	strcpy(g_strLphoneRingFile[0], ("rings/" + strDialRingFile + ".wav").toLatin1().data());
	strcpy(g_strLphoneRingFile[1], ("rings/" + strCallInRingFile + ".wav").toLatin1().data());
	strcpy(g_strLphoneRingFile[2], ("rings/" + strDoorRingFile).toLatin1().data());
	strcpy(g_strLphoneRingFile[3], ("rings/" + strManCenter1RingFile + ".wav").toLatin1().data());
	strcpy(g_strLphoneRingFile[4], ("rings/" + strManCenter2RingFile + ".wav").toLatin1().data());

	MyMessageBox messBox(this);
	CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
	//Q_UNUSED(okButton);
	okButton->setFocus(Qt::TabFocusReason);
	messBox.setText(tr("铃声参数修改成功"));
	messBox.show();
	QRect rc = messBox.geometry();
	QRect rc2 = geometry();
	rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
	messBox.setGeometry(rc);
	messBox.exec();

	setAllButtonsEnabled((QDialog *)parent(), true);
	setAllButtonsEnabled(this, true);
    ui->horizontalSliderVolume->setEnabled(true);
    ui->horizontalSliderRingVolume->setEnabled(true);
	ui->pushButtonRingVolumeSave->setFocus(Qt::TabFocusReason);

	g_pScaleSrc->Backup_Config_File();
}

/************************************************************
描述：窗口显示时，从config文件中读铃声设置，更新到行编辑控件中，从系
	 统读取对讲和铃声音量，更新到滑块控件中
参数：无
返回：无
************************************************************/
void RingAndVolumeDialog::showEvent(QShowEvent *)
{
    g_pMainDlg->showRefreshTabFocus(this);

    //david 2014.4.10
	if(!m_pRingFileSetDlg->m_pBackFromSave)
	{
		loadData();
	}

	m_pRingFileSetDlg->m_pBackFromSave = false;
}

/************************************************************
描述：窗口隐藏时的回调函数
参数：无
返回：无
************************************************************/
void RingAndVolumeDialog::hideEvent(QHideEvent *)
{
	//SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();
	//pSysSetMainDlg->ui->pushButtonRingSet->setFocus(Qt::TabFocusReason);

    g_pMainDlg->hideRefreshTabFocus(this);
}
