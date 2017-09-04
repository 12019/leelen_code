#include "languagesetdialog.h"
#include "ui_languagesetdialog.h"
#include <QMessageBox>
#include <pthread.h>
#include "maindialog.h"
#include "syssetmaindialog.h"
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/mman.h>
#include <QImage>
#include <Qt/qwindowsystem_qws.h> //lg lg
#include "ui_syssetmaindialog.h"

#include "ui_maindialog.h"
#include "ui_callcenterdialog.h"
#include "ui_helpdialog.h"

#include "ui_ictdialog.h"
#include "ui_ictrcvhookdialog.h"
#include "ui_istdialog.h"
#include "ui_mondialog.h"
#include "ui_monlistdialog.h"
////#include "ui_mondialdialog.h"
#include "ui_mstdialog.h"

#include "ui_mediamandialog.h"
#include "ui_mediaplayerdialog.h"

#include "ui_infomanmaindialog.h"
#include "ui_infomandetaildialog.h"
#include "ui_infomanviewattachmentdialog.h"
#include "ui_infowritedialog.h"

#include "ui_securitymaindialog.h"
#include "ui_securityplayerdialog.h"
#include "ui_securityalertdialog.h"
#include "ui_securityabnormaldialog.h"
#include "ui_securitypassworddialog.h"

//#include "ui_terminalhelpmaindialog.h"
#include <Qt/qstringlistmodel.h>

//#include "ui_interfreesetdialog.h"

#include "ui_doorbelldialog.h"

#include "ui_callelevatordialog.h"

#include "ui_syssetpassworddialog.h"
#include "ui_syssetmaindialog.h"
#include "ui_addresssetdialog.h"
#include "ui_languagesetdialog.h"
#include "ui_passwordsetdialog.h"
#include "ui_datetimesetdialog.h"
#include "ui_ringandvolumedialog.h"
#include "ui_ringfilesetdialog.h"
#include "ui_devicesetmaindialog.h"
#include "ui_addlampdialog.h"
#include "ui_adddefensedialog.h"
#include "ui_securityparamsetdialog.h"
#include "ui_securityphonesetdialog.h"
#include "ui_versiondialog.h"
#include "ui_syssetengineerdialog.h"
#include "ui_doormachinedialog.h"
#include "ui_ipcameradialog.h"
#include "ui_otherdialog.h"

#include "ui_addressbookdialog.h"

#include "ui_photomaindialog.h"
#include "ui_photoudiskdialog.h"

#include "ui_inputinfodialog.h"

#include <sys/ioctl.h>
#include <linux/watchdog.h>

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
/************************************************************
  该文件为语言设置对话框主程序
************************************************************/

extern MainDialog *g_pMainDlg;
//extern char *g_pDesktopImageData;

extern QTranslator *g_pTranslatorApp;
extern bool g_bTranslatorFound;
extern int g_doorbell_on;

extern const char *strDeviceTypeTranslateName[HC_CNT];

/************************************************************
描述：语言设置对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
LanguageSetDialog::LanguageSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LanguageSetDialog)
{
    ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

    QFont digitfont("arial", 30);
    ui->pushButtonLagSetEn->setFont(digitfont);
    resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

	int res = g_pScaleSrc->get_screen_version();
	if 	( res == 1 )  // 800x480
	{

	}
	else if ( res == 2 )  // 1024x600 
	{
        ui->pushButtonLagSetEn->setGeometry(QRect(230, 140, 450, 90));
        ui->pushButtonLagSetSimpCh->setGeometry(QRect(230, 230, 450, 90));
        ui->pushButtonLagSetTraCh->setGeometry(QRect(230, 320, 450, 90));
        ui->pushButtonLagSetReturn->setGeometry(QRect(770, 490, 205, 86));
    }
	else if ( res == 3 )  // 1280x800 
	{
        ui->pushButtonLagSetEn->setGeometry(QRect(270, 200, 555, 109));
        ui->pushButtonLagSetSimpCh->setGeometry(QRect(270, 346, 555, 109));
        ui->pushButtonLagSetTraCh->setGeometry(QRect(270, 492, 555, 109));
        ui->pushButtonLagSetReturn->setGeometry(QRect(1020, 650, 246, 112));

        ui->pushButtonLagSetEn->setStyleSheet(QString::fromUtf8("font: 46px;"));
        ui->pushButtonLagSetSimpCh->setStyleSheet(QString::fromUtf8("font: 46px;"));
        ui->pushButtonLagSetTraCh->setStyleSheet(QString::fromUtf8("font: 46px;"));
	}

    setAttribute(Qt::WA_TranslucentBackground, false);

    ui->pushButtonLagSetEn->m_offsetX = 0;
    ui->pushButtonLagSetSimpCh->m_offsetX = 0;
    ui->pushButtonLagSetTraCh->m_offsetX = 0;

    ui->pushButtonLagSetEn->m_offsetY = 3;
    ui->pushButtonLagSetSimpCh->m_offsetY = 3;
    ui->pushButtonLagSetTraCh->m_offsetY = 3;

	m_bIsSettingLanguage = false;
}

/************************************************************
描述：语言设置对话框析构函数
参数：无
返回：无
************************************************************/
LanguageSetDialog::~LanguageSetDialog()
{
    delete ui;
}

/*void LanguageSetDialog::copyScreenToDesktop()
{
	if (g_pDesktopImageData) delete g_pDesktopImageData;
	g_pDesktopImageData = new char[800 * 480 * 2];

	int fb_fd = ::open("/dev/fb0", O_RDWR|O_NDELAY);
	if(fb_fd < 0)
	{
		printf("LCD frame buffer open error\n");
		return;
	}

	int fb_size = 800 * 480 * 2;
	uint8_t *fb_addr = (uint8_t*)mmap(0, fb_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
	if ((int)fb_addr == -1) {
		printf("LCD frame buffer mmap failed\n");
		return;
	}

	memcpy(g_pDesktopImageData, fb_addr, 800 * 480 * 2);

	munmap(fb_addr, fb_size);
	::close(fb_fd);

	QImage image((uchar *)g_pDesktopImageData, 800, 480, QImage::Format_RGB16);
	QBrush brush(image);

	//if (strstr(QT_BUILD_KEY, "arm") == 0) //arm-linux-gcc
	{
		QWSServer::setBackground(brush); //lg lg
	}
}
*/

/************************************************************
描述：根据设置的语言对整个系统的语言模式进行切换
参数：sLang - "en":英文
			 “zh_Cn":简体中文
			 "zh_TW":繁体中文
返回：0 - 成功
	 -1 - 失败
************************************************************/
int LanguageSetDialog::setLanguage(QString sLang)
{
	m_bIsSettingLanguage = true;
	g_doorbell_on = 1; //借用g_doorbell_on来阻止来电和报警

	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());

	sync();
	system("/bin/echo 3 > /proc/sys/vm/drop_caches");
	pMainDlg->m_ticks = 0;

	int timeout = 30;
	if (g_pMainDlg->m_watchdog_fd >= 0)
	{
		ioctl(g_pMainDlg->m_watchdog_fd, WDIOC_SETTIMEOUT, &timeout);
	}

	if (g_bTranslatorFound)
	{
		qApp->removeTranslator(g_pTranslatorApp);
	}
	////delete g_pTranslatorApp;

	////g_pTranslatorApp = new QTranslator;
	g_bTranslatorFound = g_pTranslatorApp->load(":/" + sLang + ".qm");
	if (g_bTranslatorFound)
	{
		qApp->installTranslator(g_pTranslatorApp);
	}

	ui->retranslateUi(this);

	//if (0) {
	//printf("lg 1:%s\n", sLang.toLocal8Bit().data());
	pMainDlg->ui->retranslateUi(pMainDlg);
	/*pMainDlg->ui->labelIconAtHome->setText("");
	pMainDlg->ui->labelIconDisArm->setText("");
	pMainDlg->ui->labelIconGoOut->setText("");
	pMainDlg->ui->labelIconInterFree->setText("");
	pMainDlg->ui->labelIconNonInterFree->setText("");
	pMainDlg->ui->labelIconDefenseCnt->setText("\n\n" + QString::number(pMainDlg->m_pSecurityMainDlg->m_newDefenseCnt));
	pMainDlg->ui->labelIconInfoCnt->setText("\n\n" + QString::number(pMainDlg->m_pInfoManMainDlg->m_newInfoCnt));
	pMainDlg->ui->labelIconVisitorCnt->setText("\n\n" + QString::number(pMainDlg->m_pMediaManDlg->m_newVisitorCnt));
	*/
	pMainDlg->translateTableViewNofifier();

	pMainDlg->m_pCallCenterDlg->ui->retranslateUi(pMainDlg->m_pCallCenterDlg);
	pMainDlg->m_pHelpDlg->ui->retranslateUi(pMainDlg->m_pHelpDlg);

	pMainDlg->m_pInputMethodWidget->ui->retranslateUi(pMainDlg->m_pInputMethodWidget);

	//printf("lg 2\n");
	IctDialog *pIctDlg = pMainDlg->m_pVoipMainDlg->m_pIctDlg;
	pIctDlg->ui->retranslateUi(pIctDlg);

	pIctDlg->ui->pushButtonIctBack->setText("");
	pIctDlg->m_pRecvHookDlg->ui->retranslateUi(pIctDlg->m_pRecvHookDlg);
	pMainDlg->m_pVoipMainDlg->m_pIstDlg->ui->retranslateUi(pMainDlg->m_pVoipMainDlg->m_pIstDlg);
	pMainDlg->m_pVoipMainDlg->m_pIstDlg->ui->labelExtension1->setText("");
	pMainDlg->m_pVoipMainDlg->m_pIstDlg->ui->labelExtension2->setText("");
	pMainDlg->m_pVoipMainDlg->m_pMonDlg->ui->retranslateUi(pMainDlg->m_pVoipMainDlg->m_pMonDlg);
	pMainDlg->m_pVoipMainDlg->m_pMonDlg->ui->labelExtension1->setText("");
	pMainDlg->m_pVoipMainDlg->m_pMonDlg->ui->labelExtension2->setText("");
	pMainDlg->m_pVoipMainDlg->m_pMonDlg->m_pMonListDlg->ui->retranslateUi(pMainDlg->m_pVoipMainDlg->m_pMonDlg->m_pMonListDlg);
	pMainDlg->m_pVoipMainDlg->m_pMonDlg->m_pMonListDlg->reloadList();

	pMainDlg->m_pVoipMainDlg->m_pMstDlg->ui->retranslateUi(pMainDlg->m_pVoipMainDlg->m_pMstDlg);

	//printf("lg 3\n");
	pMainDlg->m_pMediaManDlg->ui->retranslateUi(pMainDlg->m_pMediaManDlg);
	pMainDlg->m_pMediaManDlg->m_pMediaPlayerDlg->ui->retranslateUi(pMainDlg->m_pMediaManDlg->m_pMediaPlayerDlg);
	pMainDlg->m_pMediaManDlg->ReloadMediaList(((char *)"/mnt/disk/media/record.xml"));

	//printf("lg 4\n");
	pMainDlg->m_pInfoManMainDlg->ui->retranslateUi(pMainDlg->m_pInfoManMainDlg);
	pMainDlg->m_pInfoManMainDlg->m_pInfoManDetailDlg->ui->retranslateUi(pMainDlg->m_pInfoManMainDlg->m_pInfoManDetailDlg);
	pMainDlg->m_pInfoManMainDlg->m_pInfoManDetailDlg->m_pViewAttachemntDlg->ui->retranslateUi(pMainDlg->m_pInfoManMainDlg->m_pInfoManDetailDlg->m_pViewAttachemntDlg);
	pMainDlg->m_pInfoManMainDlg->m_pInfoWriteDlg->ui->retranslateUi(pMainDlg->m_pInfoManMainDlg->m_pInfoWriteDlg);
	pMainDlg->m_pInfoManMainDlg->ReloadInfoList();

	//printf("lg 4-4\n");

	pMainDlg->m_pSecurityMainDlg->ui->retranslateUi(pMainDlg->m_pSecurityMainDlg);
	pMainDlg->m_pSecurityMainDlg->m_pSecurityPlayerDlg->ui->retranslateUi(pMainDlg->m_pSecurityMainDlg->m_pSecurityPlayerDlg);
	pMainDlg->m_pSecurityMainDlg->m_pSecurityAlertDlg->ui->retranslateUi(pMainDlg->m_pSecurityMainDlg->m_pSecurityAlertDlg);
	pMainDlg->m_pSecurityMainDlg->m_pSecurityAbnormalDlg->ui->retranslateUi(pMainDlg->m_pSecurityMainDlg->m_pSecurityAbnormalDlg);
	pMainDlg->m_pSecurityMainDlg->m_pSecurityPasswordDlg->ui->retranslateUi(pMainDlg->m_pSecurityMainDlg->m_pSecurityPasswordDlg);
	pMainDlg->m_pSecurityMainDlg->m_pSecurityPasswordDlg->ui->pushButtonSecPwdBack->setText("");
	//pMainDlg->m_pSecurityMainDlg->showCurrentTime();
	pMainDlg->m_pSecurityMainDlg->setSecurityState(pMainDlg->m_pSecurityMainDlg->m_securityState);
	pMainDlg->m_pSecurityMainDlg->reloadSecurityList();
	////pMainDlg->m_pSecurityMainDlg->m_pSecurityAbnormalDlg->resetState();//可不调用

	pMainDlg->m_pDoorBellDlg->ui->retranslateUi(pMainDlg->m_pDoorBellDlg);
	pMainDlg->m_pDoorBellDlg->ui->labelExtension1->setText("");
	pMainDlg->m_pDoorBellDlg->ui->labelExtension2->setText("");

	//printf("lg 5\n");

	CallElevatorDialog *pCallElevatorDlg = pMainDlg->m_pCallElevatorDlg;
	pCallElevatorDlg->ui->retranslateUi(pCallElevatorDlg);
	pCallElevatorDlg->ui->pushButtonCallElevatorDot->setText("");
	pCallElevatorDlg->ui->pushButtonCallElevatorNum0->setText("");
	pCallElevatorDlg->ui->pushButtonCallElevatorNum1->setText("");
	pCallElevatorDlg->ui->pushButtonCallElevatorNum2->setText("");
	pCallElevatorDlg->ui->pushButtonCallElevatorNum3->setText("");
	pCallElevatorDlg->ui->pushButtonCallElevatorNum4->setText("");
	pCallElevatorDlg->ui->pushButtonCallElevatorNum5->setText("");
	pCallElevatorDlg->ui->pushButtonCallElevatorNum6->setText("");
	pCallElevatorDlg->ui->pushButtonCallElevatorNum7->setText("");
	pCallElevatorDlg->ui->pushButtonCallElevatorNum8->setText("");
	pCallElevatorDlg->ui->pushButtonCallElevatorNum9->setText("");
	pCallElevatorDlg->ui->pushButtonCallElevatorShop->setText("");

	//lg
	/*TerminalHelpMainDialog *pTermHelpMainDlg = pMainDlg->m_pTermHelpMainDlg;
	pTermHelpMainDlg->ui->retranslateUi(pMainDlg->m_pTermHelpMainDlg);
	QStringList data;
	data << tr("连接管理中心计算机...");
	data += tr("连接管理中心机...");
	data += tr("获取管理中心机状态...");
	pTermHelpMainDlg->model->setStringList(data);
	*/
	//lg

	//lg
	//pMainDlg->m_pInterFreeSetDlg->ui->retranslateUi(pMainDlg->m_pInterFreeSetDlg);
	//pMainDlg->m_pInterFreeSetDlg->setInterFreeState(pMainDlg->m_pInterFreeSetDlg->m_interFreeState);
	//lg

	SysSetPasswordDialog *pSysSetPwdDlg = pMainDlg->m_pSysSetPwdDlg;
	pSysSetPwdDlg->ui->retranslateUi(pMainDlg->m_pSysSetPwdDlg);
	pSysSetPwdDlg->ui->pushButtonSysPwdBack->setText("");

	pMainDlg->m_pSysSetMainDlg->ui->retranslateUi(pMainDlg->m_pSysSetMainDlg);
	//pMainDlg->m_pSysSetMainDlg->m_pAddressSetDlg->ui->retranslateUi(pMainDlg->m_pSysSetMainDlg->m_pAddressSetDlg);
	//pMainDlg->m_pSysSetMainDlg->m_pLanguageSetDlg->ui->retranslateUi(pMainDlg->m_pSysSetMainDlg->m_pLanguageSetDlg);
	pMainDlg->m_pSysSetMainDlg->m_pPasswordSetDlg->ui->retranslateUi(pMainDlg->m_pSysSetMainDlg->m_pPasswordSetDlg);
	pMainDlg->m_pSysSetMainDlg->m_pDatetimeSetDlg->ui->retranslateUi(pMainDlg->m_pSysSetMainDlg->m_pDatetimeSetDlg);
	pMainDlg->m_pSysSetMainDlg->m_pRingAndVolumeDlg->ui->retranslateUi(pMainDlg->m_pSysSetMainDlg->m_pRingAndVolumeDlg);
	pMainDlg->m_pSysSetMainDlg->m_pRingAndVolumeDlg->m_pRingFileSetDlg->ui->retranslateUi(pMainDlg->m_pSysSetMainDlg->m_pRingAndVolumeDlg->m_pRingFileSetDlg);
	pMainDlg->m_pSysSetMainDlg->m_pRingAndVolumeDlg->m_pDoorBellRingFileSetDlg->ui->retranslateUi(pMainDlg->m_pSysSetMainDlg->m_pRingAndVolumeDlg->m_pDoorBellRingFileSetDlg);
	//pMainDlg->m_pSysSetMainDlg->m_pDeviceSetMainDlg->ui->retranslateUi(pMainDlg->m_pSysSetMainDlg->m_pDeviceSetMainDlg);
	//pMainDlg->m_pSysSetMainDlg->m_pDeviceSetMainDlg->m_pAddLampDlg->ui->retranslateUi(pMainDlg->m_pSysSetMainDlg->m_pDeviceSetMainDlg->m_pAddLampDlg);
	pMainDlg->m_pSysSetMainDlg->m_pAddDefenseDlg->ui->retranslateUi(pMainDlg->m_pSysSetMainDlg->m_pAddDefenseDlg);
	pMainDlg->m_pSysSetMainDlg->m_pSecParamSetDlg->ui->retranslateUi(pMainDlg->m_pSysSetMainDlg->m_pSecParamSetDlg);
	pMainDlg->m_pSysSetMainDlg->m_pSecParamSetDlg->m_pSecPhoneSetDlg->ui->retranslateUi(pMainDlg->m_pSysSetMainDlg->m_pSecParamSetDlg->m_pSecPhoneSetDlg);
	pMainDlg->m_pSysSetMainDlg->m_pVersionDlg->ui->retranslateUi(pMainDlg->m_pSysSetMainDlg->m_pVersionDlg);

	RingAndVolumeDialog *pRingAndVolumeDlg = pMainDlg->m_pSysSetMainDlg->m_pRingAndVolumeDlg;
	pRingAndVolumeDlg->ui->pushButtonRingVolumeDail->setText("");
	pRingAndVolumeDlg->ui->pushButtonRingVolumeCallIn->setText("");
	pRingAndVolumeDlg->ui->pushButtonRingVolumeDoor->setText("");
	pRingAndVolumeDlg->ui->pushButtonRingVolumeCenter1->setText("");
	pRingAndVolumeDlg->ui->pushButtonRingVolumeCenter2->setText("");

	pMainDlg->m_pSysSetEngineerDlg->ui->retranslateUi(pMainDlg->m_pSysSetEngineerDlg);
	pMainDlg->m_pSysSetEngineerDlg->m_pAddressSetDlg->ui->retranslateUi(pMainDlg->m_pSysSetEngineerDlg->m_pAddressSetDlg);
	pMainDlg->m_pSysSetEngineerDlg->m_pIpCameraDlg->ui->retranslateUi(pMainDlg->m_pSysSetEngineerDlg->m_pIpCameraDlg);
	pMainDlg->m_pSysSetEngineerDlg->m_pDoorMachineDlg->ui->retranslateUi(pMainDlg->m_pSysSetEngineerDlg->m_pDoorMachineDlg);
	pMainDlg->m_pSysSetEngineerDlg->m_pPasswordSetDlg->ui->retranslateUi(pMainDlg->m_pSysSetEngineerDlg->m_pPasswordSetDlg);
	pMainDlg->m_pSysSetEngineerDlg->m_pOtherDlg->ui->retranslateUi(pMainDlg->m_pSysSetEngineerDlg->m_pOtherDlg);

    pMainDlg->m_pHomeControlDlg->Translate();

	pMainDlg->m_pPhotoMainDlg->ui->retranslateUi(pMainDlg->m_pPhotoMainDlg);
	pMainDlg->m_pPhotoMainDlg->translateTableViewPhotoList();
	pMainDlg->m_pPhotoMainDlg->m_pPhotoUDiskDlg->ui->retranslateUi(pMainDlg->m_pPhotoMainDlg->m_pPhotoUDiskDlg);
	pMainDlg->m_pPhotoMainDlg->m_pPhotoUDiskDlg->translateTableViewPhotoList();

	pMainDlg->m_pAddressBookDlg->ui->retranslateUi(pMainDlg->m_pAddressBookDlg);

	timeout = 8;
	if (g_pMainDlg->m_watchdog_fd >= 0)
	{
		ioctl(g_pMainDlg->m_watchdog_fd, WDIOC_SETTIMEOUT, &timeout);
	}

	if (sLang == "en")
	{
		pMainDlg->setInputMethodMode(0);
	}
	else if (sLang == "zh_CN")
	{
		pMainDlg->setInputMethodMode(1);
	}
	else
	{
		pMainDlg->setInputMethodMode(2);
	}

	pMainDlg->ui->widgetCalendar->Generate(true);
	//}

	//printf("lg 6\n");
	QSettings Settings(CONFIG_FILE, QSettings::IniFormat);
	QString sOldLang = Settings.value("language/lang").toString();

	Settings.setValue("language/lang", sLang);

	if (sOldLang == "en")
	{
        ui->pushButtonLagSetEn->setFocus(Qt::TabFocusReason);
	}
	else if (sOldLang == "zh_CN")
	{
        ui->pushButtonLagSetSimpCh->setFocus(Qt::TabFocusReason);
	}
	else if (sOldLang == "zh_TW")
	{
        ui->pushButtonLagSetTraCh->setFocus(Qt::TabFocusReason);
	}

	g_pScaleSrc->Backup_Config_File();

	sync();
	system("/bin/echo 3 > /proc/sys/vm/drop_caches");
	pMainDlg->m_ticks = 0;

	m_bIsSettingLanguage = false;
	g_doorbell_on = 0; //借用g_doorbell_on来阻止来电和报警

	return 0;
}

/************************************************************
描述：选择英文为使用的语言
参数：无
返回：无
************************************************************/
void LanguageSetDialog::on_pushButtonLagSetEn_pressed()
{
    if (!(ui->pushButtonLagSetEn->hasFocus())) return;

	setLanguage("en");

    ui->pushButtonLagSetEn->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：选择简体中文为使用的语言
参数：无
返回：无
************************************************************/
void LanguageSetDialog::on_pushButtonLagSetSimpCh_pressed()
{
    if (!(ui->pushButtonLagSetSimpCh->hasFocus())) return;

	setLanguage("zh_CN");

    ui->pushButtonLagSetSimpCh->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：选择繁体中文为使用的语言
参数：无
返回：无
************************************************************/
void LanguageSetDialog::on_pushButtonLagSetTraCh_pressed()
{
    if (!(ui->pushButtonLagSetTraCh->hasFocus())) return;

	setLanguage("zh_TW");

    ui->pushButtonLagSetTraCh->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：窗口显示时的回调函数
参数：无
返回：无
************************************************************/
void LanguageSetDialog::showEvent(QShowEvent *)
{
    g_pMainDlg->showRefreshTabFocus(this);

    QSettings Settings(CONFIG_FILE, QSettings::IniFormat);
    QString lang = Settings.value("language/lang").toString();

    if (lang == "zh_CN")
    {
        ui->pushButtonLagSetSimpCh->setFocus(Qt::TabFocusReason);
    }
    else if (lang == "en")
    {
        ui->pushButtonLagSetEn->setFocus(Qt::TabFocusReason);
    }
    else if (lang == "zh_TW")
    {
        ui->pushButtonLagSetTraCh->setFocus(Qt::TabFocusReason);
    }


}

/************************************************************
描述：窗口隐藏时的回调函数
参数：无
返回：无
************************************************************/
void LanguageSetDialog::hideEvent(QHideEvent *)
{
    g_pMainDlg->hideRefreshTabFocus(this);

	//SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();
	//pSysSetMainDlg->ui->pushButtonLanguageSet->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：返回调用者
参数：无
返回：无
************************************************************/
void LanguageSetDialog::on_pushButtonLagSetReturn_pressed()
{
    hide();
    SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();
    pSysSetMainDlg->on_pushButtonSysSetMainVersion_pressed();
}
