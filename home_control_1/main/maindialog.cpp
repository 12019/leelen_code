#include "maindialog.h"
#include "ui_maindialog.h"
#include "helpdialog.h"
#include <QEvent>
#include "syssetmaindialog.h"
#include "mediastreamer2/mediastream.h"
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/if_arp.h>
#include "exosip_main.h"
#include "Big5pinyinpanel.h"
#include <QObjectList>
#include <linux/watchdog.h>
#include "application.h"
#include "ui_ictdialog.h"
#include "ui_big5pinyinpanel.h"
#include "checkip.h"
#include "ui_mondialog.h"
#include "voipmaindialog.h"
#include "inputinfodialog.h"
#include "ui_inputinfodialog.h"
#include "digitpanel.h"
#include "ui_digitpanel.h"
#include "ui_mediamandialog.h"


#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

/************************************************************
  该文件为系统主窗口主程序
************************************************************/

/************************************************************
  如果该宏为1则程序为NFS调试模式，为0则程序为发布模式
************************************************************/
#define NFS_DEBUG	 	0

int g_HomeControlSetShow = 0;

#define _HOME_CONTROL 1

#ifdef   _SEND_MMS
    #ifdef _HOME_CONTROL
        #define MAX_FUNCTION_MODULES_CNT	13
    #else
        #define MAX_FUNCTION_MODULES_CNT	12
    #endif

#else
    #ifdef _HOME_CONTROL
        #define MAX_FUNCTION_MODULES_CNT	12
    #else
        #define MAX_FUNCTION_MODULES_CNT	11
    #endif
#endif

int g_mainMonitorFlag = 1;

/************************************************************
  本机网络名，格式为“网路名?设备类型号"
************************************************************/
extern char g_strNativeName[50];
/************************************************************
  g_strLphoneName[0]:门口主机网络名
  g_strLphoneName[1]:管理中心机网络名
  g_strLphoneName[2]:管理中心计算机网络名
************************************************************/
extern char g_strLphoneName[3][50];
extern char g_strNativeIP[50];
/************************************************************
  每次可视对讲最长时间
************************************************************/
extern int g_iVoipMaxTime;
/************************************************************
  每次视频监控最长时间
************************************************************/
extern int g_iMonMaxTime;
extern int g_bIstOrAlertBusy;
extern MainDialog *g_pMainDlg;

extern ViewCam *g_pViewCam;

extern int g_vFlagmessage;

static bool flash_val  = true;	 // 进行闪烁的标示
static bool flag_alarm = false;  // 是否进行LED报警指示

//lg for test
/************************************************************
描述：获取本机IP地址的函数
参数：无
返回：本机IP地址
************************************************************/
QString GetLocalIp()
{
	int sock_get_ip;
	char ipaddr[50];

	struct   sockaddr_in *sin;
	struct   ifreq ifr_ip;

	if ((sock_get_ip=socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("socket create failse...GetLocalIp!/n");
		return "";
	}

	memset(&ifr_ip, 0, sizeof(ifr_ip));
	strncpy(ifr_ip.ifr_name, "eth0", sizeof(ifr_ip.ifr_name) - 1);

	if( ioctl( sock_get_ip, SIOCGIFADDR, &ifr_ip) < 0 )
	{
		return "";
	}
	sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;
	strcpy(ipaddr,inet_ntoa(sin->sin_addr));

	printf("local ip:%s \n",ipaddr);
	close( sock_get_ip );

	return QString( ipaddr );
}
//lg

/************************************************************
描述：系统主窗口对话框构造函数，完成网络、子窗口、子线程、定时器的初始
	 化
参数：parent - 该对话框的父窗口指针
返回：无
************************************************************/
MainDialog::MainDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MainDialog)
{
	ui->setupUi(this);

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.22
										  // make the dialog full screen

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
	}

	g_pMainDlg = this;

	//connect((Application *)qApp, SIGNAL(sigSetLCDAndTimerOn(bool)), this, SLOT(slotLCDBacklightOn(bool)));

	//防止播放视频时出现主界面按钮位置的黑色背景方框
	ui->pushButtonDummy->hide();
	RESCALESCREEN(ui->pushButtonDummy)
	setIndicatorState(0);
	setIndicatorLEDState(false);

	m_pCurActiveDlg = NULL;

	g_iVoipMaxTime = 30 * 24 * 3600; //90;
	g_iMonMaxTime = 10 * 3600;
	m_iCurModalType = BASIC_MODAL_TYPE;

	//QFont font("FZCaiYun-M09T", 34);
	//font.setBold(true);
	//ui->pushButtonVoip->setFont(font);

	if (g_strNativeName[0] == '\0')
	{
		//printf("lg ReloadMediaList\n");
		QSettings settings(CONFIG_FILE, QSettings::IniFormat);
		QString nativeName = settings.value("system/native_name", "1111-2222").toString();

		sprintf(g_strNativeName, "%s?%d", nativeName.toLatin1().data(), g_pMainDlg->m_iCurModalType);
	}

	int i;
	{
		QStandardItemModel *model = new QStandardItemModel(6, 3);

		int x,y,w,h;
		g_pScaleSrc->get_maindialog_tableViewNotifier_geometry(x,y,w,h);
		ui->tableViewNotifier->setGeometry(x,y,w,h);

		ui->tableViewNotifier->setModel(model);
		ui->tableViewNotifier->horizontalHeader()->hide();
		ui->tableViewNotifier->verticalHeader()->hide();
		ui->tableViewNotifier->setSelectionMode(QAbstractItemView::NoSelection);
		ui->tableViewNotifier->setEditTriggers(QAbstractItemView::NoEditTriggers);
		ui->tableViewNotifier->setSortingEnabled(false);
		ui->tableViewNotifier->setShowGrid(false);
		ui->tableViewNotifier->setFocusPolicy(Qt::NoFocus);

		for (i = 0; i < 6; i++)
		{
			ui->tableViewNotifier->setRowHeight(i,g_pScaleSrc->get_maindialog_tableViewNotifier_lineheight());
		}
		ui->tableViewNotifier->setColumnWidth(0, g_pScaleSrc->get_w_value(20));
		ui->tableViewNotifier->setColumnWidth(1, g_pScaleSrc->get_w_value(40));
		ui->tableViewNotifier->setColumnWidth(2, ui->tableViewNotifier->geometry().width() - g_pScaleSrc->get_w_value(60));

		ui->tableViewNotifier->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		ui->tableViewNotifier->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

		int ix,iy;
		g_pScaleSrc->get_maindialog_tableViewNotifier_iconsize(ix,iy);
		ui->tableViewNotifier->setIconSize(QSize(ix, iy));

		QStandardItem *item;
		item = new QStandardItem(QIcon(":/images/main/arm.png"), "");
		model->setItem(0, 1, item);
		item = new QStandardItem("");
		model->setItem(0, 2, item);

		item = new QStandardItem(QIcon(":/images/main/interfree.png"), "");
		model->setItem(1, 1, item);
		item = new QStandardItem("");
		model->setItem(1, 2, item);

        item = new QStandardItem(QIcon(":/images/main/net.png"), "");
        model->setItem(2, 1, item);
        item = new QStandardItem("");
        model->setItem(2, 2, item);

		item = new QStandardItem(QIcon(":/images/main/call.png"), "");
        model->setItem(3, 1, item);
		item = new QStandardItem("");
        model->setItem(3, 2, item);

		item = new QStandardItem(QIcon(":/images/main/alarm.png"), "");
        model->setItem(4, 1, item);
		item = new QStandardItem("");
        model->setItem(4, 2, item);

		item = new QStandardItem(QIcon(":/images/main/information.png"), "");
        model->setItem(5, 1, item);
		item = new QStandardItem("");
        model->setItem(5, 2, item);

		/*ui->labelIconAtHome->setText("");
		ui->labelIconDisArm->setText("");
		ui->labelIconGoOut->setText("");
		ui->labelIconInterFree->setText("");
		ui->labelIconNonInterFree->setText("");
		*/
	}

	m_pFunctionModule = new MyPushButton *[MAX_FUNCTION_MODULES_CNT];

	int cnt = 0;
	m_pFunctionModule[cnt++] = ui->pushButtonVoip;
	m_pFunctionModule[cnt++] = ui->pushButtonInfoMan;
	m_pFunctionModule[cnt++] = ui->pushButtonVideoMon;
	m_pFunctionModule[cnt++] = ui->pushButtonSecurityMan;
	m_pFunctionModule[cnt++] = ui->pushButtonIntercomMan;
	m_pFunctionModule[cnt++] = ui->pushButtonCallCenter;
	m_pFunctionModule[cnt++] = ui->pushButtonTerminalHelp;
	m_pFunctionModule[cnt++] = ui->pushButtonCallElevator;
	m_pFunctionModule[cnt++] = ui->pushButtonInterFreeSetting;
	m_pFunctionModule[cnt++] = ui->pushButtonPhoto;

    RESCALESCREEN(ui->pushButtonOther)

	for ( int i = 0; i < cnt; i += 1 ) 
	{
		RESCALESCREEN(m_pFunctionModule[i])
	}

    //david
#ifdef _HOME_CONTROL
	m_pFunctionModule[cnt++] = ui->pushButtonHomeControl;

    RESCALESCREEN(ui->pushButtonHomeControl)

#else
	ui->pushButtonHomeControl->hide();
#endif

#ifdef   _SEND_MMS

	m_pFunctionModule[cnt++] = ui->pushButtonSendImg;

	RESCALESCREEN(ui->pushButtonSendImg)
#else
	ui->pushButtonSendImg->hide();
#endif
	m_pFunctionModule[cnt++] = ui->pushButtonSystemSetting;

	RESCALESCREEN(ui->pushButtonSystemSetting)

	m_nDispFirstFMIdx	 = 0;
	m_nDispFirstFMTopLeft	 = m_pFunctionModule[0]->geometry().topLeft();
	m_nDispFMSpan		 = m_pFunctionModule[1]->geometry().x() - m_pFunctionModule[0]->geometry().x();

	ReDisplayButtons();

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
	}

	m_pAbnormalMsgBox = new MyMessageBox(this);
	m_pAbnormalMsgBoxOkBtn = m_pAbnormalMsgBox->addButton(tr("确定"));
	m_pAbnormalMsgBox->hide();

	m_pIpConflictMsgBox = new MyMessageBox(this);
	m_pIpConflictMsgBoxOkBtn = m_pIpConflictMsgBox->addButton(tr("确定"));
	m_pIpConflictMsgBox->hide();

	m_pTerminalHelpMsgBox = new MyMessageBox(this);
	m_pTerminalHelpMsgBoxOkBtn = m_pTerminalHelpMsgBox->addButton(tr("确定"));
	m_pTerminalHelpMsgBox->hide();

	m_pInterFreeMsgBox = new MyMessageBox(this);
	m_pInterFreeMsgBoxOkBtn = m_pInterFreeMsgBox->addButton(tr("确定"));
	m_pInterFreeMsgBox->hide();

	m_pCallElevatorMsgBox = new MyMessageBox(this);
	m_pCallElevatorMsgBoxOkBtn = m_pCallElevatorMsgBox->addButton(tr("确定"));
	m_pCallElevatorMsgBox->hide();


	m_pAddressBookDlg = new AddressBookDialog(this);

	//m_pTermHelpMainDlg = new TerminalHelpMainDialog(this);
	m_pVoipMainDlg = new VoipMainDialog(this);
	m_pInfoManMainDlg = new InfoManMainDialog(this);
	m_pMediaManDlg = new MediaManDialog(this);
	m_pDoorBellDlg = new DoorBellDialog(this);
	m_pSysSetMainDlg = new SysSetMainDialog(this);
	m_pSecurityMainDlg = new SecurityMainDialog(this);
	//m_pInterFreeSetDlg = new InterFreeSetDialog(this);
	m_pSysSetEngineerDlg = new SysSetEngineerDialog(this);
	m_pCallElevatorDlg = new CallElevatorDialog(this);
	m_pCallCenterDlg = new CallCenterDialog(this);
	m_pHelpDlg = new HelpDialog(this);
    m_pHomeControlDlg = new HomeControlMainDlg;
    m_pHomeControlDlg->Init();
	m_pPhotoMainDlg = new PhotoMainDialog(this);
	//m_pTestDlg = new TestDialog(this);

	m_pSysSetPwdDlg = new SysSetPasswordDialog(this);

	m_pCalibrationDlg = new CalibrationDialog(this);

	//m_pScreenSaverDlg = new ScreenSaverDialog(this);
	//m_pScreenSaverDlg->show();
	//m_pScreenSaverDlg->raise();

	m_pCurActiveDlg = this;

	if ((m_pMediaManDlg->m_newVisitorCnt > 0) || (m_pSecurityMainDlg->m_newDefenseCnt > 0) || (m_pInfoManMainDlg->m_newInfoCnt > 0))
	{
		setIndicatorState(1);
		setIndicatorLEDState(true);
	}
	else if ((m_pMediaManDlg->m_newVisitorCnt == 0) && (m_pSecurityMainDlg->m_newDefenseCnt == 0) && (m_pInfoManMainDlg->m_newInfoCnt == 0))
	{
		setIndicatorState(0);
		setIndicatorLEDState(false);
	}

	m_pTermHelpThread = new TerminalHelpThread(this);
	connect(m_pTermHelpThread, SIGNAL(sigShowTermHelpResult(QString)), this, SLOT(showTermHelpResult(QString)));

	startWaveThread();

	m_pKeypadReadThread = new KeypadReadThread(this);
	m_pKeypadReadThread->start();

	m_pTSCalibrateThread = new TSCalibrateThread(this);
	m_pTSCalibrateThread->start();

    	connect(g_pMainDlg,SIGNAL(sigSetNetWork(int)),this,SLOT(slotSetNetWork(int)));
        connect(g_pMainDlg,SIGNAL(sigResetTimeVariable()),this,SLOT(slotResetTimeVariable()));

    m_pDetectNetworkThread = new DetectNetworkThread(this);
    m_pDetectNetworkThread->start();

    m_pDoorBellRingThread = new DoorBellRingThread(this);
    m_pDoorBellRingThread->start();
    m_pCallElevatorThread = new CallElevatorThread(this);

	connect(((Application *)qApp)->m_pKeyTimer, SIGNAL(timeout()), this, SLOT(openCalibration()));

	QSettings settings(CONFIG_FILE, QSettings::IniFormat);
	{
		m_bInterFree = settings.value("voip/inter_free", "0").toInt();

		QStandardItemModel *model = (QStandardItemModel *)ui->tableViewNotifier->model();
		if (m_bInterFree)
		{
			model->item(1, 1)->setIcon(QIcon(":/images/main/interfree.png"));
			model->item(1, 2)->setText(tr("免扰"));
		}
		else
		{
			model->item(1, 1)->setIcon(QIcon(":/images/main/non-interfree.png"));
			model->item(1, 2)->setText(tr("非免扰"));
		}

		/*if (interFree == 1)
		{
			ui->labelIconInterFree->show();
			ui->labelIconNonInterFree->hide();
		}
		else
		{
			ui->labelIconInterFree->hide();
			ui->labelIconNonInterFree->show();
		}*/
	}

	/*{
		m_bEnableTVOut = settings.value("system/enable_tvout", "1").toInt();
	}*/

	{
		QString nativeIP;
		QString netMask;
		QString gateWay;
		QString macAddr;
		QString nativeName;
		//QString doorWayName;
		QString manageCenter1Name;
		QString manageCenter2Name;
		char cmd[256];

		nativeIP = settings.value("system/native_ip", "192.168.109.1").toString();
		netMask = settings.value("system/net_mask", "255.255.0.0").toString();
		gateWay = settings.value("system/gate_way", "192.168.1.1").toString();
		macAddr = settings.value("system/mac_addr", "00:01:02:03:04:05").toString();
		nativeName = settings.value("system/native_name", "1111-2222").toString();
		//doorWayName = settings.value("system/doorway_name", "1111-0001").toString();
		manageCenter1Name = settings.value("system/manage_center1_name", "9999-0001").toString();
		manageCenter2Name = settings.value("system/manage_center2_name", "9999-0002").toString();

		//lg
#if NFS_DEBUG == 1
		nativeIP = GetLocalIp();
		if (nativeIP == "192.168.1.236")
		{
			nativeName = "1111-1227";
		}
		else
		{
			nativeName = "1111-2328";//nativeIP.mid(nativeIP.lastIndexOf(".") + 1);
		}
#endif
		if (ip_check(nativeIP.toLatin1().data()) == -1)
		{
			m_pIpConflictMsgBox->setText(tr("本机IP地址和网络中其它某台设备相重"));
			m_pIpConflictMsgBoxOkBtn->setText(tr("确定"));
			m_pIpConflictMsgBox->show();
			m_pIpConflictMsgBoxOkBtn->setFocus(Qt::TabFocusReason);
		}

		m_pVoipMainDlg->m_pGetPeerIPThread->m_nativeIP = nativeIP;
		m_pVoipMainDlg->m_pGetPeerIPThread->m_nativeName = nativeName;
		printf("native name: %s\n", m_pVoipMainDlg->m_pGetPeerIPThread->m_nativeName.toLatin1().data());
		//lg

#if NFS_DEBUG == 0
        /*system("/sbin/ifconfig eth0 down");
        sprintf(cmd, "/sbin/ifconfig eth0 hw ether %s", macAddr.toLatin1().data());
        system(cmd);
		system("/sbin/ifconfig eth0 up");
        */
		sprintf(cmd, "/sbin/ifconfig eth0 %s netmask %s", nativeIP.toLatin1().data(),
				netMask.toLatin1().data());
		system(cmd);
#endif
		system("/sbin/route del default eth0");
		sprintf(cmd, "/sbin/route add default gw %s dev eth0", gateWay.toLatin1().data());
		system(cmd);

		m_pCallCenterDlg->setCallCenterName(manageCenter1Name, manageCenter2Name);

		sprintf(g_strNativeName, "%s?%d", nativeName.toLatin1().data(), m_iCurModalType);
		strcpy(g_strNativeIP, nativeIP.toLocal8Bit().data());
		//strcpy(g_strLphoneName[0], doorWayName.toLatin1().data());
		strcpy(g_strLphoneName[1], manageCenter1Name.toLatin1().data());
		strcpy(g_strLphoneName[2], manageCenter2Name.toLatin1().data());
	}

	{
		QString strDialRingFile;
		QString strCallInRingFile;
		QString strDoorRingFile;
		QString strManCenter1RingFile;
		QString strManCenter2RingFile;

		strDialRingFile = settings.value("rings/dial", "ringback").toString();
		strCallInRingFile = settings.value("rings/callin", "oldphone").toString();
		strDoorRingFile = settings.value("rings/doorway", "ring1").toString();
		strManCenter1RingFile = settings.value("rings/man_center1", "oldphone").toString();
		strManCenter2RingFile = settings.value("rings/man_center2", "oldphone").toString();

		m_pVoipMainDlg->m_nRingVolume = settings.value("rings/ringvolume", "70").toInt();
		if ( m_pVoipMainDlg->m_nRingVolume > 100 ) m_pVoipMainDlg->m_nRingVolume = 100;
		m_pVoipMainDlg->m_nRingVolume = static_cast<int>(static_cast<float>(m_pVoipMainDlg->m_nRingVolume)*1024.0/100 * Kevin::Global::RATIO);
		m_pVoipMainDlg->m_nVolume     = settings.value("rings/volume", "70").toInt();
		if ( m_pVoipMainDlg->m_nVolume > 100 ) m_pVoipMainDlg->m_nVolume = 100;
		g_pScaleSrc->set_doorbell_ring_value(m_pVoipMainDlg->m_nVolume);			// 设置二次门铃的音量
		m_pVoipMainDlg->m_nVolume     = static_cast<int>(static_cast<float>(m_pVoipMainDlg->m_nVolume)*1024.0/100);

		extern char g_strLphoneRingFile[5][50];
		strcpy(g_strLphoneRingFile[0], ("rings/" + strDialRingFile + ".wav").toLatin1().data());
		strcpy(g_strLphoneRingFile[1], ("rings/" + strCallInRingFile + ".wav").toLatin1().data());
		strcpy(g_strLphoneRingFile[2], ("rings/" + strDoorRingFile).toLatin1().data());		// 二次门铃的铃声
		strcpy(g_strLphoneRingFile[3], ("rings/" + strManCenter1RingFile + ".wav").toLatin1().data());
		strcpy(g_strLphoneRingFile[4], ("rings/" + strManCenter2RingFile + ".wav").toLatin1().data());
	}
	
	int x,y,w,h;
	g_pScaleSrc->get_maindialog_calendar_geometry(x,y,w,h);
	ui->widgetCalendar->setGeometry(x,y,w,h);

	g_pScaleSrc->get_maindialog_clock_geometry(x,y,w,h);
	ui->widgetClock->setGeometry(x,y,w,h);

	{
		setTimeMode(1);
		ui->lcdNumberDigClock->setParent(this);
		QRect rc = ui->lcdNumberDigClock->geometry();
		{
			QRect rc2 = ui->widgetClock->geometry();
			rc = QRect(rc2.x(), rc2.y() + g_pScaleSrc->get_h_value(60), rc.width(), rc.height());
			ui->lcdNumberDigClock->setGeometry(rc);
		}
	}

	{
		QString lang = settings.value("language/lang").toString();
		int imMode = 0;

		if (lang == "en")
		{
			imMode = 0;
		}
		else if (lang == "zh_CN")
		{
			imMode = 1;
        }
		else if (lang == "zh_TW")
		{
			imMode = 2;
		}

		m_inputMethodMode = -1;
		m_pInputMethodWidget = NULL;
		setInputMethodMode(imMode);
		m_pInputMethod = new InputMethod;
	}

	m_wipeMouseX = -1;

	m_pNetRcvSrv = new NetRecvServer(this);
	if (!m_pNetRcvSrv->listen(QHostAddress::Any, 554)) {
		MyMessageBox messBox(NULL);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("打开554端口网络接收套接字错误"));
		messBox.exec();
	}

	connect(this, SIGNAL(sigSetNotify(int)), this, SLOT(setNotify(int)));

	m_watchdog_fd = -1;
	m_ticks = 0;
	m_pWatchDogTimer = new QTimer(this);
	connect(m_pWatchDogTimer, SIGNAL(timeout()), this, SLOT(watchDogTimerTimeout()));
	m_pWatchDogTimer->start(500);

	g_vFlagmessage = 1;

	g_mainMonitorFlag = 1;

	m_pVoipMainDlg->m_pGetPeerIPThread->start();
	connect(this, SIGNAL(sigCallRecordInfoManager(QString,QString,QString,QString,QString)), this, SLOT(slotCallRecordInfoManager(QString,QString,QString,QString,QString)));
    connect(this,SIGNAL(sigTcpCallElevator()),this,SLOT(slotTcpCallElevator()));
    connect(this,SIGNAL(sigShowCallElevatorMsgBox(QString)),this,SLOT(slotShowCallElevatorMsgBox(QString)));

}

/************************************************************
描述：系统主窗口对话框析构函数
参数：无
返回：无
************************************************************/
MainDialog::~MainDialog()
{
	if (m_watchdog_fd >= 0)
	{
		write(m_watchdog_fd, "V", 1);
		::close(m_watchdog_fd);
		m_watchdog_fd = -1;
	}
	m_pWatchDogTimer->stop();
	delete m_pWatchDogTimer;

	delete m_pSecurityMainDlg;

	m_pNetRcvSrv->close();
	delete m_pNetRcvSrv;

	//m_pUpDownloadSrv->close();
	//delete m_pUpDownloadSrv;

	m_pKeypadReadThread->m_bRun = false;
	m_pKeypadReadThread->quit();
	m_pKeypadReadThread->wait();
	delete m_pKeypadReadThread;

	m_pTSCalibrateThread->m_bRun = false;
	m_pTSCalibrateThread->quit();
	m_pTSCalibrateThread->wait();
	delete m_pTSCalibrateThread;

    m_pDetectNetworkThread->m_bRun = false;
    m_pDetectNetworkThread->quit();
    m_pDetectNetworkThread->wait();
    delete m_pDetectNetworkThread;

    m_pDoorBellRingThread->m_bRun = false;
    m_pDoorBellRingThread->quit();
    m_pDoorBellRingThread->wait();
    delete m_pDoorBellRingThread;

	stopWaveThread();

	delete m_pInputMethod;
	if (m_pInputMethodWidget != NULL)
	{
		delete m_pInputMethodWidget;
	}

	//delete m_pTermHelpMainDlg;
	delete m_pVoipMainDlg;
	delete m_pInfoManMainDlg;
	delete m_pMediaManDlg;
	//delete m_pInterFreeSetDlg;
	delete m_pSysSetMainDlg;
	delete m_pSysSetEngineerDlg;
	delete m_pCallElevatorDlg;
	delete m_pCallCenterDlg;
	delete m_pDoorBellDlg;
	delete m_pHelpDlg;
    delete m_pHomeControlDlg;
	delete m_pPhotoMainDlg;

	delete m_pCalibrationDlg;

	delete m_pAddressBookDlg;

	delete m_pSysSetPwdDlg;
	delete m_pAbnormalMsgBox;
	delete m_pIpConflictMsgBox;
	delete m_pTerminalHelpMsgBox;
	delete m_pInterFreeMsgBox;
	delete m_pCallElevatorMsgBox;

	//delete m_pScreenSaverDlg;

	delete [] m_pFunctionModule;

	m_pTermHelpThread->quit();
	m_pTermHelpThread->wait();
	delete m_pTermHelpThread;

	delete ui;
}
	
void MainDialog::setIndicatorLEDState(bool state)
{
	if ( state == 0 )	// 正常
	{
		((Application *)qApp)->setGpioValue(172,false);		// 打开常规灯
		flag_alarm = false;
		((Application *)qApp)->setGpioValue(173,true);		// 关闭报警灯
	}
	else			// 异常
	{
		((Application *)qApp)->setGpioValue(172,true);		// 关闭常规灯
		flag_alarm = true;
	}
}

/************************************************************
描述：看门狗定时器超时回调函数，实现定时喂看门狗的功能，还实现每30秒清
	 除一次系统Cache
参数：无
返回：无
************************************************************/
void MainDialog::watchDogTimerTimeout()
{
    m_pWatchDogTimer->start(500);					// 为了与报警灯指示相匹配

	if ( flag_alarm == true )   // 进行报警
	{
		AlarmLEDTimerTimeout();
	}

    g_mainMonitorFlag = 1;
    m_ticks++;
    //david 2014.5.19
    if(m_ticks >= 40)
    {
        psystem("/bin/sync");

        psystem("/bin/echo 3 > /proc/sys/vm/drop_caches");

        m_ticks = 0;
    }
}

/************************************************************
描述： 进行LED设置
	
参数：无
返回：无
************************************************************/
void MainDialog::AlarmLEDTimerTimeout()
{
	if ( flash_val == true )
		flash_val = false;
	else
		flash_val = true;
	((Application *)qApp)->setGpioValue(173,flash_val);
}

/************************************************************
描述：获取主窗口时钟控件
参数：无
返回：主窗口时钟控件的指针
************************************************************/
Clock *MainDialog::getClock()
{
	return ui->widgetClock;
}

/************************************************************
描述：获取主窗口日历控件
参数：无
返回：主窗口日历控件的指针
************************************************************/
Calendar *MainDialog::getCalendar()
{
	return ui->widgetCalendar;
}

/************************************************************
描述：获取主窗口Ui
参数：无
返回：主窗口Ui的指针
************************************************************/
Ui::MainDialog *MainDialog::getUi()
{
	return ui;
}

/************************************************************
描述：系统启动后防区异常的状态显示消息框
参数：strText:防区异常的状态信息
返回：无
************************************************************/
void MainDialog::showAbnormalMsgBox(QString strText)
{
    //m_pHomeControlDlg->hide();
	if(m_pAbnormalMsgBox->isVisible())  m_pAbnormalMsgBox->done(0);

	m_pAbnormalMsgBox->setText(strText);
	m_pAbnormalMsgBoxOkBtn->setText(tr("确定"));

	m_pAbnormalMsgBox->show();
	m_pAbnormalMsgBox->raise();

	if ((m_pCurActiveDlg == this) && (!m_pIpConflictMsgBox->isVisible()))
	{
		m_pAbnormalMsgBoxOkBtn->setFocus(Qt::TabFocusReason);
	}
}

/************************************************************
描述：处理门铃响的事件
参数：strText:防区异常的状态信息
返回：无
************************************************************/
void MainDialog::Do_Door_Bell_Event(QString strText)
{
	if ( m_pVoipMainDlg->Do_Door_Bell_Event(strText) == false )       // 如果没有处于可视对讲状态
	{
		m_pVoipMainDlg->m_pMonDlg->Do_Door_Bell_Close_Video();    // 先关闭视频，再显示信息
		m_pSecurityMainDlg->m_pSerial1Thread->Recall_Ring();      // 关闭视频之后再响铃声
		showAbnormalMsgBox(strText);
	}
}

/************************************************************
描述：设置主窗口时间显示模式
参数：nTimeMode - 1:模拟时钟
				 2:12小时制数字时钟
				 3:24小时制数字时钟
返回：无
************************************************************/
void MainDialog::setTimeMode(uint32_t nTimeMode)
{
	if (nTimeMode == 1)
	{
		ui->lcdNumberDigClock->hide();
		ui->lcdNumberDigClock->ClockStop();
		ui->widgetClock->show();
		ui->widgetClock->ClockRun();
	}
	else
	{
		ui->widgetClock->hide();
		ui->widgetClock->ClockStop();
		ui->lcdNumberDigClock->show();
		ui->lcdNumberDigClock->ClockRun(nTimeMode == 3);
	}

	m_nTimeMode = nTimeMode;
}

/************************************************************
描述：向管理机发送终端求助信息
参数：无
返回：无
************************************************************/
void MainDialog::on_pushButtonTerminalHelp_pressed()
{
	if (m_pTermHelpThread->isRunning() || m_pTerminalHelpMsgBox->isVisible())
	{
		return;
	}

	m_pTerminalHelpMsgBox->hide();
	m_pInterFreeMsgBox->hide();

	m_pTermHelpThread->quit();
	m_pTermHelpThread->wait();
	m_pTermHelpThread->start();
}

/************************************************************
描述：用消息框显示管理机返回的终端求助的状态信息
参数：strInfo - 管理机返回的终端求助的状态信息
返回：无
************************************************************/
void MainDialog::showTermHelpResult(QString strInfo)
{
	m_pTerminalHelpMsgBox->setText(/*"\n" +*/ strInfo /*+ "\n"*/);
	m_pTerminalHelpMsgBoxOkBtn->setText(tr("确定"));
	m_pTerminalHelpMsgBox->show();
	m_pTerminalHelpMsgBox->raise();

	if ((m_pCurActiveDlg == this))
	{
		m_pTerminalHelpMsgBoxOkBtn->setFocus(Qt::TabFocusReason);
	}
}

/************************************************************
描述：按了终端右上方终端求助按键后，关闭当前窗口回到主窗口，然后向管理
	 机发送终端求助信息
参数：无
返回：无
************************************************************/
void MainDialog::sendTerminalHelp()
{
	int loop_cnt = 0;
	while (g_bIstOrAlertBusy)
	{
		usleep(5000);

		if (loop_cnt++ >= 8) //40 ms
		{
			qApp->processEvents();
			loop_cnt = 0;
		}
	}

	g_bIstOrAlertBusy = 5;

	m_pVoipMainDlg->closeVoipDialog();
	m_pVoipMainDlg->closeOtherRelatedDialog();
	m_pVoipMainDlg->hide();

	QObjectList q = children();
	for (int i = q.length() - 1; i >= 0; i-- )
	{
		QObject* o = q.at(i);

		if (o->inherits("QDialog") && (o->objectName() != "MyMessageBox"))
		{
			QDialog* d = qobject_cast<QDialog*>(o);
			if (d->isVisible())
			{
				d->hide();
			}
		}
	}

	g_bIstOrAlertBusy = 0;

	on_pushButtonTerminalHelp_pressed();
}

/************************************************************
描述：打开可视对讲拨号窗口
参数：无
返回：无
************************************************************/
void MainDialog::on_pushButtonVoip_pressed()
{
	if (m_pVoipMainDlg->isVisible() && m_pVoipMainDlg->m_pIctDlg->isVisible()) return; //bug

    m_pVoipMainDlg->show();
    m_pVoipMainDlg->m_pIctDlg->show();
    m_pVoipMainDlg->m_pIctDlg->ResetState("");
	m_pVoipMainDlg->m_pIctDlg->ui->lineEditDisplay->setText("");
}

/************************************************************
描述：打开信息管理窗口
参数：无
返回：无
************************************************************/
void MainDialog::on_pushButtonInfoMan_pressed()
{
	if (m_pInfoManMainDlg->isVisible()) return;

	//hideAllSubWidget(this, NULL);

	m_pInfoManMainDlg->show();
}

/************************************************************
描述：打开安防管理窗口
参数：无
返回：无
************************************************************/
void MainDialog::on_pushButtonSecurityMan_pressed()
{
	//ui->pushButtonOther->setFocus(Qt::TabFocusReason);

	//if (m_pTestDlg->isVisible()) return;
	//m_pTestDlg->show();
	//return;

	if (m_pSecurityMainDlg->isVisible()) return;
	m_pSecurityMainDlg->show();
}

/************************************************************
描述：打开对讲管理窗口
参数：无
返回：无
************************************************************/
void MainDialog::on_pushButtonIntercomMan_pressed()
{
	if (m_pMediaManDlg->isVisible()) return;
	m_pMediaManDlg->show();
}

/************************************************************
描述：打开呼叫中心窗口
参数：无
返回：无
************************************************************/
void MainDialog::on_pushButtonCallCenter_pressed()
{
	if (m_pCallCenterDlg->isVisible()) return;

	m_pCallCenterDlg->ResetState();
	m_pCallCenterDlg->show();
	m_pCallCenterDlg->raise();
}

/************************************************************
描述：切换系统免扰/非免扰状态
参数：无
返回：无
************************************************************/
void MainDialog::on_pushButtonInterFreeSetting_clicked()
{
	printf("on_pushButtonInterFreeSetting_pressed\n");
	m_pInterFreeMsgBox->hide();

    QStandardItemModel *model = (QStandardItemModel *)ui->tableViewNotifier->model();
    {
        QSettings settings(CONFIG_FILE, QSettings::IniFormat);
        m_bInterFree = !m_bInterFree;
        settings.setValue("voip/inter_free", (int)m_bInterFree);
    }

	if (m_bInterFree)
	{
		m_pInterFreeMsgBox->setText(tr("进入免打扰模式"));
		model->item(1, 1)->setIcon(QIcon(":/images/main/interfree.png"));
		model->item(1, 2)->setText(tr("免扰"));
	}
	else
	{
		m_pInterFreeMsgBox->setText(tr("进入正常状态, 即非免打扰模式"));
		model->item(1, 1)->setIcon(QIcon(":/images/main/non-interfree.png"));
		model->item(1, 2)->setText(tr("非免扰"));
	}

	m_pInterFreeMsgBoxOkBtn->setText(tr("确定"));

	m_pInterFreeMsgBox->show();

	if ((m_pCurActiveDlg == this))
	{
		m_pInterFreeMsgBoxOkBtn->setFocus(Qt::TabFocusReason);
	}
	g_pScaleSrc->Backup_Config_File();
}

/************************************************************
描述：打开视频监控窗口
参数：无
返回：无
************************************************************/
void MainDialog::on_pushButtonVideoMon_pressed()
{
    if (m_pVoipMainDlg->isVisible() && m_pVoipMainDlg->m_pMonDlg->isVisible()) return;

    extern int g_doorbell_on;

    if(m_pSecurityMainDlg->m_pSecurityAlertDlg->isVisible()||(g_doorbell_on==1))
    {
         return;
    }

	m_pVoipMainDlg->show();
	m_pVoipMainDlg->m_pMonDlg->show();
	m_pVoipMainDlg->m_pMonDlg->ResetState(true);

	if ( m_pVoipMainDlg->m_pMonDlg->m_pMonListDlg->m_doorMachineCnt == 0 && m_pVoipMainDlg->m_pMonDlg->m_pMonListDlg->m_ipCameraCnt != 0 )
		m_pVoipMainDlg->m_pMonDlg->m_pIPCTimer->start(100);
	else
		m_pVoipMainDlg->m_pMonDlg->m_pFirstCamTimer->start(100);
}

void MainDialog::slotTcpCallElevator()
{
    if (m_pCallElevatorMsgBox->isVisible()) return;


    if(!m_pSecurityMainDlg->m_pSerial1Thread->m_iCallElevatorCmd)
    {
        m_pSecurityMainDlg->m_pSerial1Thread->m_iCallElevatorCmd = 1;
    }

    char strPeerIP[200] = "192.168.1.107?4";
    int ret = 0;
    GetPeerIPThread *pGetPeerIPThread = m_pVoipMainDlg->m_pGetPeerIPThread;

    printf("lg doorway:%s\n", g_strLphoneName[0]);
    ret = pGetPeerIPThread->GetPeerIP(g_strLphoneName[0], strPeerIP);

    char *ptr;
    if (ret == 0)
    {
        int iModalType = BASIC_MODAL_TYPE;
        ptr = strstr(strPeerIP, "?");
        if (ptr)
        {
            iModalType = atoi(ptr + strlen("?"));
            *ptr = '\0';
        }

        if (!(iModalType & DOORWAY_MODAL_TYPE))
        {
            m_pCallElevatorMsgBox->setText(tr("门口机设置错误"));
            m_pCallElevatorMsgBoxOkBtn->setText(tr("确定"));
            m_pCallElevatorMsgBox->show();

            if ((m_pCurActiveDlg == this))
            {
                m_pCallElevatorMsgBoxOkBtn->setFocus(Qt::TabFocusReason);
            }

            return;
        }

        ptr = strstr(strPeerIP, ":");
        if (ptr)
        {
            ptr += strlen(":");
        }
        else
        {
            ptr = strPeerIP;
        }

        if (!ptr[0]) ret = -1;
    }

    if (ret != 0)
    {
        m_pCallElevatorMsgBox->setText(tr("获取门口机IP失败"));
        m_pCallElevatorMsgBoxOkBtn->setText(tr("确定"));
        m_pCallElevatorMsgBox->show();

        if ((m_pCurActiveDlg == this))
        {
            m_pCallElevatorMsgBoxOkBtn->setFocus(Qt::TabFocusReason);
        }

        return;
    }

    QTcpSocket socket;

    socket.connectToHost(ptr, 1206);

    if (!socket.waitForConnected(750))
    {
        m_pCallElevatorMsgBox->setText(tr("连接门口机失败"));
        m_pCallElevatorMsgBoxOkBtn->setText(tr("确定"));
        m_pCallElevatorMsgBox->show();

        if ((m_pCurActiveDlg == this))
        {
            m_pCallElevatorMsgBoxOkBtn->setFocus(Qt::TabFocusReason);
        }

        return;
    }

    QString strNativeName(g_strNativeName);
    int iNativeName = strNativeName.mid(strNativeName.indexOf("-") + 1,4).toInt();
    int iFloor = (int)iNativeName/100;
    int iRoom = (int)iNativeName%100;

    int iDestFloor = 1;//ui->lineEditDisplay->text().toInt();

    unsigned char szBuf[7];
    szBuf[0] = (unsigned char)0xf7;
    szBuf[1] = (unsigned char)(iFloor >> 8);
    szBuf[2] = (unsigned char)(iFloor & 0xff);
    szBuf[3] = (unsigned char)iRoom;
    szBuf[4] = (unsigned char)(iDestFloor >> 8);
    szBuf[5] = (unsigned char)(iDestFloor & 0xff);
    szBuf[6] = (unsigned char)0x01;

    socket.write((const char *)szBuf, 7);
    socket.flush();

    int loop = 0;
    int cnt = 0;
    while (socket.waitForReadyRead(1200))
    {
        cnt += socket.read((char *)szBuf + cnt, 2 - cnt);
        if (cnt >= 2) break;
        if (++loop >= 2) break;
    }

    socket.close();

    if ((cnt == 2) && (szBuf[0] == (unsigned char)0xf7) && szBuf[1] == (unsigned char)0x01)
    {
        m_pCallElevatorMsgBox->setText(tr("电梯已呼叫, 请等待..."));
        m_pCallElevatorMsgBoxOkBtn->setText(tr("确定"));
        m_pCallElevatorMsgBox->show();

        if ((m_pCurActiveDlg == this))
        {
            m_pCallElevatorMsgBoxOkBtn->setFocus(Qt::TabFocusReason);
        }
    }
    else
    {
        m_pCallElevatorMsgBox->setText(tr("呼叫电梯失败"));
        m_pCallElevatorMsgBoxOkBtn->setText(tr("确定"));
        m_pCallElevatorMsgBox->show();

        if ((m_pCurActiveDlg == this))
        {
            m_pCallElevatorMsgBoxOkBtn->setFocus(Qt::TabFocusReason);
        }
    }
}

void MainDialog::emitSigTcpCallElevator()
{
    emit sigTcpCallElevator();
}

void MainDialog::emitSigShowCallElevatorMsgBox(QString str)
{
    emit sigShowCallElevatorMsgBox(str);
}

void MainDialog::slotShowCallElevatorMsgBox(QString str)
{
    m_pCallElevatorMsgBox->setText(str);
    m_pCallElevatorMsgBoxOkBtn->setText(tr("确定"));
    m_pCallElevatorMsgBox->show();

    if ((m_pCurActiveDlg == this))
    {
        m_pCallElevatorMsgBoxOkBtn->setFocus(Qt::TabFocusReason);
    }
}

/************************************************************
描述：打开呼叫电梯窗口
参数：无
返回：无
************************************************************/
void MainDialog::on_pushButtonCallElevator_pressed()
{
    if(m_pCallElevatorMsgBox->isVisible())
    {
        return ;
    }
    if(m_pCallElevatorThread->m_bRun)
    {
        m_pCallElevatorThread->m_bRun = false;
        m_pCallElevatorThread->quit();
        m_pCallElevatorThread->wait();
    }

    m_pCallElevatorThread->start();
//	if (m_pCallElevatorMsgBox->isVisible()) return;


//	if(!m_pSecurityMainDlg->m_pSerial1Thread->m_iCallElevatorCmd)
//	{
//		m_pSecurityMainDlg->m_pSerial1Thread->m_iCallElevatorCmd = 1;
//	}

//	char strPeerIP[200] = "192.168.1.107?4";
//	int ret = 0;
//	GetPeerIPThread *pGetPeerIPThread = m_pVoipMainDlg->m_pGetPeerIPThread;

//	printf("lg doorway:%s\n", g_strLphoneName[0]);
//	ret = pGetPeerIPThread->GetPeerIP(g_strLphoneName[0], strPeerIP);

//	char *ptr;
//	if (ret == 0)
//	{
//		int iModalType = BASIC_MODAL_TYPE;
//		ptr = strstr(strPeerIP, "?");
//		if (ptr)
//		{
//			iModalType = atoi(ptr + strlen("?"));
//			*ptr = '\0';
//		}

//		if (!(iModalType & DOORWAY_MODAL_TYPE))
//		{
//			m_pCallElevatorMsgBox->setText(tr("门口机设置错误"));
//			m_pCallElevatorMsgBoxOkBtn->setText(tr("确定"));
//			m_pCallElevatorMsgBox->show();

//			if ((m_pCurActiveDlg == this))
//			{
//				m_pCallElevatorMsgBoxOkBtn->setFocus(Qt::TabFocusReason);
//			}

//			return;
//		}

//		ptr = strstr(strPeerIP, ":");
//		if (ptr)
//		{
//			ptr += strlen(":");
//		}
//		else
//		{
//			ptr = strPeerIP;
//		}

//		if (!ptr[0]) ret = -1;
//	}

//	if (ret != 0)
//	{
//		m_pCallElevatorMsgBox->setText(tr("获取门口机IP失败"));
//		m_pCallElevatorMsgBoxOkBtn->setText(tr("确定"));
//		m_pCallElevatorMsgBox->show();

//		if ((m_pCurActiveDlg == this))
//		{
//			m_pCallElevatorMsgBoxOkBtn->setFocus(Qt::TabFocusReason);
//		}

//		return;
//	}

//	QTcpSocket socket;

//	socket.connectToHost(ptr, 1206);

//	if (!socket.waitForConnected(750))
//	{
//		m_pCallElevatorMsgBox->setText(tr("连接门口机失败"));
//		m_pCallElevatorMsgBoxOkBtn->setText(tr("确定"));
//		m_pCallElevatorMsgBox->show();

//		if ((m_pCurActiveDlg == this))
//		{
//			m_pCallElevatorMsgBoxOkBtn->setFocus(Qt::TabFocusReason);
//		}

//		return;
//	}

//	QString strNativeName(g_strNativeName);
//	int iNativeName = strNativeName.mid(strNativeName.indexOf("-") + 1,4).toInt();
//	int iFloor = (int)iNativeName/100;
//	int iRoom = (int)iNativeName%100;

//	int iDestFloor = 1;//ui->lineEditDisplay->text().toInt();

//	unsigned char szBuf[7];
//	szBuf[0] = (unsigned char)0xf7;
//	szBuf[1] = (unsigned char)(iFloor >> 8);
//	szBuf[2] = (unsigned char)(iFloor & 0xff);
//	szBuf[3] = (unsigned char)iRoom;
//	szBuf[4] = (unsigned char)(iDestFloor >> 8);
//	szBuf[5] = (unsigned char)(iDestFloor & 0xff);
//	szBuf[6] = (unsigned char)0x01;

//	socket.write((const char *)szBuf, 7);
//	socket.flush();

//	int loop = 0;
//	int cnt = 0;
//	while (socket.waitForReadyRead(1200))
//	{
//		cnt += socket.read((char *)szBuf + cnt, 2 - cnt);
//		if (cnt >= 2) break;
//		if (++loop >= 2) break;
//	}

//	socket.close();

//	if ((cnt == 2) && (szBuf[0] == (unsigned char)0xf7) && szBuf[1] == (unsigned char)0x01)
//	{
//		m_pCallElevatorMsgBox->setText(tr("电梯已呼叫, 请等待..."));
//		m_pCallElevatorMsgBoxOkBtn->setText(tr("确定"));
//		m_pCallElevatorMsgBox->show();

//		if ((m_pCurActiveDlg == this))
//		{
//			m_pCallElevatorMsgBoxOkBtn->setFocus(Qt::TabFocusReason);
//		}
//	}
//	else
//	{
//		m_pCallElevatorMsgBox->setText(tr("呼叫电梯失败"));
//		m_pCallElevatorMsgBoxOkBtn->setText(tr("确定"));
//		m_pCallElevatorMsgBox->show();

//		if ((m_pCurActiveDlg == this))
//		{
//			m_pCallElevatorMsgBoxOkBtn->setFocus(Qt::TabFocusReason);
//		}
//	}
}

void MainDialog::on_pushButtonHomeControl_pressed()
{
    if (m_pHomeControlDlg->isVisible()) return;
    m_pHomeControlDlg->show();
}

void MainDialog::on_pushButtonPhoto_pressed()
{
	if (m_pPhotoMainDlg->isVisible()) return;

	m_pPhotoMainDlg->load("/mnt/disk/photo");
	m_pPhotoMainDlg->show();
}

/************************************************************
描述：打开系统设置窗口
参数：无
返回：无
************************************************************/
void MainDialog::on_pushButtonSystemSetting_pressed()
{
	if (m_pSysSetPwdDlg->isVisible()) return;
	m_pSysSetPwdDlg->show();
	m_pSysSetPwdDlg->raise();
}

/************************************************************
描述：重新设置主窗口上该显示的按钮坐标并显示这些按钮
参数：无
返回：0:成功
	 -1:失败
************************************************************/
int MainDialog::ReDisplayButtons()
{
	int i, j;
	for (i = 0; i < m_nDispFirstFMIdx; i++)
	{
		m_pFunctionModule[i]->hide();
	}

	ui->pushButtonOther->show();

	if (m_nDispFirstFMIdx == 0)
	{
		ui->widgetClock->show();
		ui->widgetCalendar->show();
		ui->tableViewNotifier->show();

		QRect rc;
		int x, y ;
        y = m_pFunctionModule[0]->geometry().y();
		for (i = 0; i < MIN(MAX_FUNCTION_MODULES_CNT, 4); i++, j++)
		{
			rc = m_pFunctionModule[i]->geometry();
			x = m_nDispFirstFMTopLeft.x() + (i - m_nDispFirstFMIdx) * m_nDispFMSpan;
            //y = rc.y();

			rc.moveTopLeft(QPoint(x, y));
			m_pFunctionModule[i]->setGeometry(rc);
			m_pFunctionModule[i]->show();
		}

		rc = ui->pushButtonOther->geometry();
		rc.moveTopLeft(QPoint(rc.left(), y));
		ui->pushButtonOther->setGeometry(rc);
        ui->pushButtonOther->setText(tr("\n\n\n其他"));
	}
	else
	{
		ui->widgetClock->hide();
		ui->widgetCalendar->hide();
		ui->tableViewNotifier->hide();

		QRect rc = m_pFunctionModule[0]->geometry();
        int x;
        //int sy = rc.y();
        int sy =g_pScaleSrc->get_h_value(290);
        for (i = m_nDispFirstFMIdx, j = 0; i < MIN(MAX_FUNCTION_MODULES_CNT, m_nDispFirstFMIdx + 9); i++, j++)
		{
			rc = m_pFunctionModule[i]->geometry();
			x = m_nDispFirstFMTopLeft.x() + (i - m_nDispFirstFMIdx - ((j < 5) ? 0 : 5)) * m_nDispFMSpan;

            int yy = (j < 5) ? g_pScaleSrc->get_h_value(70) : sy;

			rc.moveTopLeft(QPoint(x, yy));//m_nDispFirstFMTopLeft.y() - ));

			m_pFunctionModule[i]->setGeometry(rc);
			m_pFunctionModule[i]->show();
		}

		rc = ui->pushButtonOther->geometry();
		rc.moveTopLeft(QPoint(rc.left(), sy));
		ui->pushButtonOther->setGeometry(rc);

        ui->pushButtonOther->setText(tr("\n\n\n返回"));
	}

	for (; i < MAX_FUNCTION_MODULES_CNT; i++)
	{
		m_pFunctionModule[i]->hide();
	}

	return 0;
}

/************************************************************
描述：主窗口触摸屏/鼠标按下回调函数
参数：e - 鼠标事件
返回：无
************************************************************/
void MainDialog::mousePressEvent(QMouseEvent *e)
{
	//if (m_nShowSubDlgCnt > 0) return;
	if (m_pCurActiveDlg != this) return;

	if (e->button() == Qt::LeftButton)
	{
		m_wipeMouseX = e->globalX();
	}
}

/************************************************************
描述：主窗口触摸屏/鼠标放开回调函数
参数：e - 鼠标事件
返回：无
************************************************************/
void MainDialog::mouseReleaseEvent(QMouseEvent *e)
{
	//if (m_nShowSubDlgCnt > 0) return;
	if (m_pCurActiveDlg != this) return;

	if (m_wipeMouseX == -1) return;

	if (e->button() == Qt::LeftButton)
	{
#if 0
        int curMouseX;

		curMouseX = e->globalX();
		if (e->globalY() <= m_nDispFirstFMTopLeft.y())
		{
			if ((curMouseX - m_wipeMouseX) > 50)
			{
				if (m_nDispFirstFMIdx >= DISP_FUNCTION_MODULES_CNT)
				{
					m_nDispFirstFMIdx -= DISP_FUNCTION_MODULES_CNT;
					ReDisplayButtons();
				}
			}
			else if ((curMouseX - m_wipeMouseX) < -50)
			{
				if (m_nDispFirstFMIdx < MAX_FUNCTION_MODULES_CNT - DISP_FUNCTION_MODULES_CNT)
				{
					m_nDispFirstFMIdx += DISP_FUNCTION_MODULES_CNT;

					ReDisplayButtons();
				}
			}
		}
#endif
	}
}

/************************************************************
描述：根据语言模式设置输入法状态
参数：mode - 0:英文模式
			1:简体中文模式
			2:繁体中文模式
返回：0:成功
	 -1:失败
************************************************************/
int MainDialog::setInputMethodMode(int mode)
{
	if (m_inputMethodMode != mode)
	{
		if (m_pInputMethodWidget != NULL)
		{
			delete m_pInputMethodWidget;
		}

		if ((mode == 0) || (mode == 1))
		{

			m_pInputMethodWidget = new InputInfoDialog(this);

			if (mode == 0)
			{
				m_pInputMethodWidget->ui->DigitWidget->py->m_bEnglishMode = true;
				m_pInputMethodWidget->ui->DigitWidget->ui->btnchangeCHINESE->show();
				m_pInputMethodWidget->ui->DigitWidget->ui->btnchangeENGLISH->hide();
			}
			else
			{
				m_pInputMethodWidget->ui->DigitWidget->py->m_bEnglishMode = false;
				m_pInputMethodWidget->ui->DigitWidget->ui->btnchangeENGLISH->show();
				m_pInputMethodWidget->ui->DigitWidget->ui->btnchangeCHINESE->hide();
			}
		}
		else
		{
			m_pInputMethodWidget = new InputInfoDialog(this);
			m_pInputMethodWidget->ui->DigitWidget->py->m_bEnglishMode = false;
			m_pInputMethodWidget->ui->DigitWidget->ui->btnchangeENGLISH->show();
			m_pInputMethodWidget->ui->DigitWidget->ui->btnchangeCHINESE->hide();
		}

		m_inputMethodMode = mode;
	}

	return 0;
}

/************************************************************
描述：关闭VoIP、视频监控、安防录像、录像播放后打开触摸屏校准窗口
参数：无
返回：无
************************************************************/
void MainDialog::openCalibration()
{
	m_pKeypadReadThread->m_pTimer->stop();
	((Application *)qApp)->m_pKeyTimer->stop();

	m_pVoipMainDlg->closeVoipDialog();
	m_pVoipMainDlg->closeOtherRelatedDialog();
	m_pVoipMainDlg->hide();

    //CalibrationDialog calibrationDlg;
	m_pCalibrationDlg->exec();
	//calibrationDlg.exec();
}

void MainDialog::translateTableViewNofifier()
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewNotifier->model();
	QString str;

	switch (m_pSecurityMainDlg->m_securityState)
	{
	case STATUS_DISARM:
		str = SecurityMainDialog::tr("撤防");
		break;
	case STATUS_GOOUT:
		str = SecurityMainDialog::tr("外出");
		break;
	case STATUS_ATHOME:
		str = SecurityMainDialog::tr("居家");
		break;
	}
	model->item(0, 2)->setText(str);

	if (m_bInterFree)
	{
		str = tr("免扰");
	}
	else
	{
		str = tr("非免扰");
	}
	model->item(1, 2)->setText(str);

    if (m_pDetectNetworkThread->oldnetwork)
    {
        model->item(2, 2)->setText(tr("网络已断开"));
    }
    else
    {
        model->item(2, 2)->setText(tr("网络已连接"));
    }

	if (m_pMediaManDlg->m_newVisitorCnt == 0)
	{
		str = MainDialog::tr("无未接呼叫");
	}
	else
	{
		str = MainDialog::tr("有") + QString::number(m_pMediaManDlg->m_newVisitorCnt) + MainDialog::tr("个呼叫");
	}
    model->item(3, 2)->setText(str);

	if (m_pSecurityMainDlg->m_newDefenseCnt == 0)
	{
		str = MainDialog::tr("无未接警报");
	}
	else
	{
		str = MainDialog::tr("有") + QString::number(m_pSecurityMainDlg->m_newDefenseCnt) + MainDialog::tr("条未接警报");
	}
    model->item(4, 2)->setText(str);

	if (m_pInfoManMainDlg->m_newInfoCnt == 0)
	{
		str = MainDialog::tr("无未查看信息");
	}
	else
	{
		str = MainDialog::tr("有") + QString::number(m_pInfoManMainDlg->m_newInfoCnt) + MainDialog::tr("条未查看信息");
	}
    model->item(5, 2)->setText(str);
}

void MainDialog::emitSigSetNotify(int indx)
{
	emit sigSetNotify(indx);
}

void MainDialog::emitSigSetRowHeight(int ID, int row)
{
	emit sigSetRowHeight(ID, row);
}

void MainDialog::emitSigSetNetWork(int mode)
{
    emit sigSetNetWork(mode);
}

void MainDialog::emitSigResetTimeVaraiable()
{
    emit sigResetTimeVariable();
}

void MainDialog::emitCallRecordInfoManager(QString call_type,QString strTemp,QString strDateTime,QString have_avifile,QString unread)
{
	emit sigCallRecordInfoManager(call_type,strTemp,strDateTime,have_avifile,unread);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  slotCallRecordInfoManager
 *  Description:  将呼叫对讲的信息记录到GUI界面上
 * =====================================================================================
 */
void MainDialog::slotCallRecordInfoManager(QString call_type,QString strTemp,QString strDateTime,QString have_avifile,QString unread)
{
	QStandardItemModel *model = (QStandardItemModel *)m_pMediaManDlg->getTableViewMediaList()->model();

	QStandardItem *newItem0 = new QStandardItem("");
	newItem0->setIcon(QIcon(call_type));
	QStandardItem *newItem1 = new QStandardItem(strTemp);
	QStandardItem *newItem2 = new QStandardItem(strDateTime);
	QStandardItem *newItem3 = new QStandardItem(have_avifile);
	QStandardItem *newItem4 = new QStandardItem(unread);

	model->insertRow(0);
	model->setItem(0, 0, newItem0);
	model->setItem(0, 1, newItem1);
	model->setItem(0, 2, newItem2);
	model->setItem(0, 3, newItem3);
	model->setItem(0, 4, newItem4);

	newItem0->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	newItem1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	newItem2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	newItem3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

	emit sigSetRowHeight(0, 0);

	if ((m_pMediaManDlg->ui->pushButtonMediaManDel->isEnabled())
			&& (!m_pMediaManDlg->m_pMediaPlayerDlg->isVisible()))
	{
		m_pMediaManDlg->getTableViewMediaList()->selectRow(0);
		m_pMediaManDlg->slotMediaListPressed();
	}
}

void MainDialog::setNotify(int indx)
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewNotifier->model();

	if (indx == 0) //呼叫
	{
		if (m_pMediaManDlg->m_newVisitorCnt == 0)
		{
            model->item(3, 1)->setIcon(QIcon(":/images/main/call_none.png"));
            model->item(3, 2)->setText(MainDialog::tr("无未接呼叫"));
		}
		else
		{
			if (m_pMediaManDlg->m_newVisitorCnt == 1)
			{
                model->item(3, 1)->setIcon(QIcon(":/images/main/call.png"));
			}

            model->item(3, 2)->setText(tr("有") + QString::number(m_pMediaManDlg->m_newVisitorCnt) + tr("个呼叫"));
		}
	}
	else if (indx == 1) //报警
	{
		//bug
		if (g_pMainDlg->m_pVoipMainDlg->isVisible() && g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->isVisible())
		{
			g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->setDisplayWindow(false);
			usleep(120000);
			g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->ui->frameVideoShow->repaint();
		}
		//bug end

		if (m_pSecurityMainDlg->m_newDefenseCnt == 0)
		{
            model->item(4, 1)->setIcon(QIcon(":/images/main/alarm_none.png"));
            model->item(4, 2)->setText(MainDialog::tr("无未接警报"));
		}
		else
		{
			if (m_pSecurityMainDlg->m_newDefenseCnt == 1)
			{
                model->item(4, 1)->setIcon(QIcon(":/images/main/alarm.png"));
			}

            model->item(4, 2)->setText(tr("有") + QString::number(m_pSecurityMainDlg->m_newDefenseCnt) + tr("条未接警报"));
		}
	}
	else if (indx == 2) //信息
	{
		if (m_pInfoManMainDlg->m_newInfoCnt == 0)
		{
            model->item(5, 1)->setIcon(QIcon(":/images/main/information_none.png"));
            model->item(5, 2)->setText(MainDialog::tr("无未查看信息"));
		}
		else
		{
			if (m_pInfoManMainDlg->m_newInfoCnt == 1)
			{
                model->item(5, 1)->setIcon(QIcon(":/images/main/information.png"));
			}

            model->item(5, 2)->setText(tr("有") + QString::number(m_pInfoManMainDlg->m_newInfoCnt) + tr("条未查看信息"));
		}
	}

	if ((m_pMediaManDlg->m_newVisitorCnt > 0) || (m_pSecurityMainDlg->m_newDefenseCnt > 0) || (m_pInfoManMainDlg->m_newInfoCnt > 0))
	{
		setIndicatorState(1);
		setIndicatorLEDState(true);
	}
	else if ((m_pMediaManDlg->m_newVisitorCnt == 0) && (m_pSecurityMainDlg->m_newDefenseCnt == 0) && (m_pInfoManMainDlg->m_newInfoCnt == 0))
	{
		setIndicatorState(0);
		setIndicatorLEDState(false);
	}
}

/************************************************************
描述：迭代遍历某窗口及其子窗口，将所有按钮的焦点模式设置为指定模式
参数：q - 该窗口的子控件列表
	 focusPolicy - 需设置的按钮的焦点模式
返回：无
************************************************************/
void traversalControl(const QObjectList& q, Qt::FocusPolicy focusPolicy)
{
	int i;
	QObject *o;
	for (i = 0; i < q.length(); i++)
	{
		o = q.at(i);

		if (!o->children().empty())
		{
			if (o->inherits("QDialog"))
			{
				QDialog *d = qobject_cast<QDialog*>(o);
				if (d->isVisible())
				{
					traversalControl(d->children(), focusPolicy);
				}
			}
		}
		else
		{
			if (o->inherits("QPushButton"))
			{
				QPushButton* b = qobject_cast<QPushButton*>(o);
				//printf("pushButton: %s\n", b->text().toLatin1().data());
				b->setFocusPolicy(focusPolicy);
			}
		}
	}
}

/************************************************************
描述：显示/隐藏一个窗口时更新系统的Tab焦点模式(该函数已被废弃)
参数：无
返回：无
************************************************************/
void MainDialog::refreshTabFocus()
{
	traversalControl(children(), Qt::NoFocus);

	int i, j;
	QDialog *pDlg;
	QObjectList q;

	pDlg = NULL;
	q = children();
	for (i = q.length() - 1; i >= 0; i--)
	{
		QObject* o = q.at(i);

		if (o->inherits("QDialog"))
		{
			QDialog *d = qobject_cast <QDialog *>(o);
			if (d->isVisible())
			{
				pDlg = d;
				break;
			}
		}
	}

	if ((m_pSysSetMainDlg == pDlg) || (m_pSysSetEngineerDlg == pDlg))
	{
		for (j = 0; j < 2; j++)
		{
			q = pDlg->children();
			for (i = 0; i < q.length(); i++)
			{
				QObject *o = q.at(i);

				if (o->inherits("QPushButton"))
				{
					QPushButton* b = qobject_cast<QPushButton*>(o);
					b->setFocusPolicy(Qt::StrongFocus);
				}
			}

			if (m_pSysSetMainDlg == pDlg)
			{
				pDlg = m_pSysSetMainDlg->m_curDlg;
			}
			else
			{
				pDlg = m_pSysSetEngineerDlg->m_curDlg;
			}

			if (!pDlg) break;
		}
	}

	pDlg = this;

	do
	{
		q = pDlg->children();
		for (i = q.length() - 1; i >= 0; i--)
		{
			QObject* o = q.at(i);

			if (o->inherits("QDialog"))
			{
				QDialog *d = qobject_cast <QDialog *>(o);
				if (d->isVisible())
				{
					pDlg = d;
					break;
				}
			}
		}
	}
	while (i >= 0);

	m_pCurActiveDlg = pDlg;

	//printf("lg %s\n", pDlg->objectName().toLatin1().data());

	q = m_pCurActiveDlg->children();
	for (i = 0; i < q.length(); i++)
	{
		QObject *o = q.at(i);

		if (o->inherits("QPushButton"))
		{
			QPushButton* b = qobject_cast<QPushButton*>(o);
			b->setFocusPolicy(Qt::StrongFocus);
		}
	}
}

/************************************************************
描述：设置指定窗口的所有按钮的使能模式
参数：pDlg - 指定窗口
	 bEnable - true:按钮使能
			   false:按钮不使能
返回：无
************************************************************/
void setAllButtonsEnabled(QDialog *pDlg, bool bEnable)
{
	int i;
	QObjectList q = pDlg->children();

	for (i = 0; i < q.length(); i++)
	{
		QObject *o = q.at(i);

		if (o->inherits("QPushButton"))
		{
			QPushButton* b = qobject_cast<QPushButton*>(o);
			b->setEnabled(bEnable);
		}
	}
}

/************************************************************
描述：设置指定窗口的所有按钮的焦点模式
参数：pDlg - 指定窗口
	 policy - 按钮的目标焦点模式
返回：无
************************************************************/
void setAllButtonsFocusPolicy(QDialog *pDlg, Qt::FocusPolicy policy)
{
	int i;
	QObjectList q = pDlg->children();

	for (i = 0; i < q.length(); i++)
	{
		QObject *o = q.at(i);

		if (o->inherits("QPushButton"))
		{
			QPushButton *b = qobject_cast<QPushButton*>(o);
			b->setFocusPolicy(policy);
		}
		else if (o->inherits("QCheckBox"))
		{
			QCheckBox *b = qobject_cast<QCheckBox*>(o);
			b->setFocusPolicy(policy);
		}
	}
}

void hideAllSubWidget(QDialog *pDlg, QWidget *pWidgetException)
{
    int i;
    QObjectList q = pDlg->children();

    for (i = 0; i < q.length(); i++)
    {
        QObject *o = q.at(i);

        if (!o->inherits("QWidget")) return;

        QWidget *w = (QWidget *)o;

        if (w != pWidgetException)
        {
            w->setProperty("isVisible", w->isVisible());
            w->hide();
        }
        else
        {
            w->setProperty("isVisible", true);
        }
    }
}

void showAllSubWidget(QDialog *pDlg, QWidget *pWidgetException)
{
    int i;
    QObjectList q = pDlg->children();

    for (i = 0; i < q.length(); i++)
    {
        QObject *o = q.at(i);

        if (!o->inherits("QWidget")) return;

        //printf("lg %s\n", o->objectName().toLocal8Bit().data());

        QWidget *w = (QWidget *)o;

        //printf("lg 2 %s\n", o->objectName().toLocal8Bit().data());

        QVariant var = w->property("isVisible");

        if (!var.isNull())
        {
            //printf("\tlg %s:%d\n", w->objectName().toLocal8Bit().data(),
            //       var.toBool() ? 1 : 0);

            if (w != pWidgetException)
                w->setVisible(var.toBool());
            var.clear();
            w->setProperty("isVisible", var);
        }
    }
}
/************************************************************
描述：显示一个窗口时更新系统的Tab焦点模式
参数：pDlg - 显示的窗口
返回：无
************************************************************/
void MainDialog::showRefreshTabFocus(QDialog *pDlg)
{
	setAllButtonsFocusPolicy(m_pCurActiveDlg, Qt::NoFocus);

    if ((m_pCurActiveDlg == m_pSysSetMainDlg) || (m_pCurActiveDlg == m_pSysSetEngineerDlg))
	{
		QObjectList q;
		int i;

		q = m_pCurActiveDlg->children();
		for (i = q.length() - 1; i >= 0; i--)
		{
			QObject* o = q.at(i);

			if (o->inherits("QDialog"))
			{
				QDialog *d = qobject_cast <QDialog *>(o);
				if (d->isVisible())
				{
					setAllButtonsFocusPolicy(d, Qt::NoFocus);
				}
			}
		}
	}

	setAllButtonsFocusPolicy(pDlg, Qt::StrongFocus);

    QWidget *pWidget = pDlg;
    do
    {
        //printf("lg %s\n", pWidget->parent()->objectName().toLocal8Bit().data());
        if (m_pCurActiveDlg == pWidget->parent())
        {
            hideAllSubWidget(m_pCurActiveDlg, pWidget);
            break;
        }
        else
        {
            pWidget = (QWidget *)pWidget->parent();
        }
    }
    while (pWidget != this);

    if (pWidget == this)
    {
        hideAllSubWidget(m_pCurActiveDlg, NULL);
    }

    showAllSubWidget(pDlg, NULL);

	m_pCurActiveDlg = pDlg;
}

/************************************************************
描述：隐藏一个窗口时更新系统的Tab焦点模式
参数：pDlg - 隐藏的窗口
返回：无
************************************************************/
void MainDialog::hideRefreshTabFocus(QDialog *pDlg)
{
    //m_pHomeControlDlg->hide();
    QDialog *pOldDlg = pDlg;

	setAllButtonsFocusPolicy(pDlg, Qt::NoFocus);

    int i;
	QObjectList q;
	q = pDlg->children();
	for (i = q.length() - 1; i >= 0; i--)
	{
		QObject* o = q.at(i);
        if (o->inherits("QDialog"))
		{
			QDialog *d = qobject_cast <QDialog *>(o);
			if (d->isVisible())
			{
				setAllButtonsFocusPolicy(d, Qt::NoFocus);
			}
		}
	}

	pDlg = NULL;
	q = children();
	for (i = q.length() - 1; i >= 0; i--)
	{
		QObject* o = q.at(i);

        if (o->inherits("QDialog")&& (o->objectName() != "MyMessageBox"))
		{
			QDialog *d = qobject_cast <QDialog *>(o);
			if (d->isVisible())
			{
				pDlg = d;
				break;
			}
		}
	}

    if ((m_pSysSetMainDlg == pDlg) || (m_pSysSetEngineerDlg == pDlg))
    {
        m_pCurActiveDlg = pDlg;
		setAllButtonsFocusPolicy(pDlg, Qt::StrongFocus);
		q = pDlg->children();
		for (i = q.length() - 1; i >= 0; i--)
		{
			QObject* o = q.at(i);

			if (o->inherits("QDialog"))
			{
				QDialog *d = qobject_cast <QDialog *>(o);
				if (d->isVisible())
				{
					setAllButtonsFocusPolicy(d, Qt::StrongFocus);
				}
			}
		}

        showAllSubWidget(pDlg, pOldDlg);

		return;
	}

	pDlg = this;

	do
	{
		q = pDlg->children();
		for (i = q.length() - 1; i >= 0; i--)
		{
			QObject* o = q.at(i);

			if (o->inherits("QDialog") && (o->objectName() != "MyMessageBox"))
			{
				QDialog *d = qobject_cast <QDialog *>(o);
				if (d->isVisible())
				{
					pDlg = d;
					break;
				}
			}
		}
	}
	while (i >= 0);

	//printf("MainDialog::hideRefreshTabFocus old:%s\n", m_pCurActiveDlg->objectName().toLocal8Bit().data());

    /*if (pDlg == this)
	{
        //ReDisplayButtons();

        QDialog *pDlg2 = (QDialog *)pOldDlg->parent();
        if (pDlg2 != this)
        {
            //printf("lg %s\n", pDlg2->objectName().toLocal8Bit().data());
            showAllSubWidget(pDlg2, pOldDlg);
        }
    }
    else*/

	QWidget *pWidget = pOldDlg;
	do
	{
		if (pDlg == pWidget->parent())
		{
			break;
		}

		pWidget = (QWidget *)pWidget->parent();
	}
	while (pWidget != this);

	if (pWidget != this) //lg pDlg is the ancestor of pOldDlg
	{
		showAllSubWidget((QDialog *)pOldDlg->parent(), pOldDlg);
	}
	else //lg pDlg is not the ancestor of pOldDlg
	{
		showAllSubWidget(pDlg, NULL);
	}

    /*printf("lg %s ---- \n", pOldDlg->objectName().toLocal8Bit().data());
    q = m_pInfoManMainDlg->children();
    for (i = 0; i < q.length(); i++)
    {
        QObject *o = q.at(i);
        if (!o->inherits("QWidget")) continue;

        QWidget *w = (QWidget *)o;
        printf("lg %s %d\n", w->objectName().toLocal8Bit().data(),
               w->isVisible() ? 1 : 0);
    }*/

    m_pCurActiveDlg = pDlg;

    /*pDlg = (QDialog *)(pOldDlg->parent());
    if (!pDlg->isVisible())
    {
        showAllSubWidget(pDlg, NULL);
    }*/

    //printf("MainDialog::hideRefreshTabFocus new:%s\n", pDlg->objectName().toLocal8Bit().data());

    setAllButtonsFocusPolicy(m_pCurActiveDlg, Qt::StrongFocus);
}

void MainDialog::on_pushButtonOther_pressed()
{
    if(m_pVoipMainDlg->m_pIstDlg->isVisible()
     ||m_pCallCenterDlg->isVisible()
     ||m_pSecurityMainDlg->m_pSecurityAlertDlg->isVisible()
     ||m_pDoorBellDlg->isVisible())
    {
        return;
    }

	if (m_nDispFirstFMIdx == 0)
	{
		m_nDispFirstFMIdx += MIN(MAX_FUNCTION_MODULES_CNT - 4, 4);
	}
	else
	{
		if (m_nDispFirstFMIdx >= MAX_FUNCTION_MODULES_CNT - 9)
		{
			m_nDispFirstFMIdx = 0;
		}
		else
		{
			m_nDispFirstFMIdx += 9;
		}
	}

	ReDisplayButtons();
}

bool MainDialog::event(QEvent *e)
{
	/*if (g_pMainDlg && g_pMainDlg->m_pCurActiveDlg)
	{
		if ((g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pSecurityMainDlg->m_pSecurityPlayerDlg)
			|| (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pSecurityMainDlg->m_pSecurityAlertDlg)
			|| (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pMediaManDlg->m_pMediaPlayerDlg))
		{
			printf("lg event 1:%d\n", e->type());

			if ((e->type() == 12)
				|| (e->type() == 3))
			{
				return true;
			}
		}
	}*/

	return QDialog::event(e);
}

/*void MainDialog::slotLCDBacklightOn(bool bOn)
{
	if (bOn)
	{
		if (m_pScreenSaverDlg->isVisible())
		{
			m_pScreenSaverDlg->hide();
			repaint();
		}
	}
	else
	{
		if (!m_pScreenSaverDlg->isVisible())
		{
			m_pScreenSaverDlg->show();
		}
	}
}*/

void closeMessBox(QDialog *pDlg)
{
    QObjectList q = pDlg->children();
    int i;
    for (i = 0; i < q.length(); i++)
    {
        QObject *o = q.at(i);

        if (o->inherits("QDialog") && (o->objectName() != "MyMessageBox"))
        {
            QDialog *d = qobject_cast <QDialog *>(o);
            closeMessBox(d);
        }
        else if (o->objectName() == "MyMessageBox")
        {
            QDialog *d = qobject_cast <QDialog *>(o);
            if (d->isVisible())
            {
                d->hide();
            }
        }
    }
}

int setIndicatorState(int state)
{

	int fd = open("/dev/YiChang_Driver", 0);
	if (fd < 0) return -1;

	printf("lg setIndicatorState:%d\n", state);
	ioctl(fd, state, 0);

	close(fd);

	return 0;
}

void MainDialog::on_pushButtonSendImg_pressed()
{
	if(m_pVoipMainDlg->m_pSendImgThread->m_bRun == false)
	{

		if (m_watchdog_fd >= 0)
		{
			write(m_watchdog_fd, "V", 1);
			::close(m_watchdog_fd);
			m_watchdog_fd = -1;
		}

		((Application *)qApp)->setOnCamera(true);


		MSWebCam *cam;
		cam = ms_web_cam_manager_get_default_cam(ms_web_cam_manager_get());
		printf("viewCam_start\n");
		while (g_pViewCam == (ViewCam *)-1)
		{
			usleep(10000);
		}

		if (g_pViewCam)
		{
			if (g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->isVisible())
			{
				g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->stopViewer(true);
			}
			else
			{
				ViewCam *tmpViewCam = g_pViewCam;
				g_pViewCam = (ViewCam *)-1;
				viewCam_stop(tmpViewCam);
				g_pViewCam = NULL;
			}
		}

		g_pViewCam = (ViewCam *)-1;
		g_pViewCam = viewCam_start(cam, 0);

		usleep(1000);

		setGM7113Channel(0);

		int ret;
		ret  = m_pVoipMainDlg->m_pSendImgThread->ConnectToManCenter();

		if(ret == -1)
		{
			/*
			   m_pVoipMainDlg->m_pSendImgMsgBox->setText(tr("无法连接管理中心机"));
			   m_pVoipMainDlg->m_pSendImgMsgBoxOkBtn->setText(tr("确定"));
			   m_pVoipMainDlg->m_pSendImgMsgBox->show();
			   */
			m_pIpConflictMsgBox->setText(tr("无法连接管理中心机"));
			m_pIpConflictMsgBoxOkBtn->setText(tr("确定"));
			m_pIpConflictMsgBox->show();
			//   m_pIpConflictMsgBoxOkBtn->setFocus(Qt::TabFocusReason);
		}
		else
		{
			m_pVoipMainDlg->m_pSendImgThread->start();
		}

	}
}

void MainDialog::slotSetNetWork(int mode)
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewNotifier->model();

	m_pVoipMainDlg->m_pMonDlg->Network_Status_Chang(mode);   		// 网络状态发生改变

	if (mode)
	{
		model->item(2, 1)->setIcon(QIcon(":/images/main/net_none.png"));
		model->item(2, 2)->setText(tr("网络已断开"));

		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("网络已断开"));
		messBox.Start_Auto_Close();
		messBox.exec();
	}
	else
	{
		model->item(2, 1)->setIcon(QIcon(":/images/main/net.png"));
		model->item(2, 2)->setText(tr("网络已连接"));

		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("网络已连接"));
		messBox.Start_Auto_Close();
		messBox.exec();
	}
}

void MainDialog::slotResetTimeVariable()
{
	QDateTime dateTime = QDateTime::currentDateTime().addSecs(-(255 + 1));

	for (int i = 0; i < m_pSecurityMainDlg->m_wireDefenseCnt; i++)
	{
		m_pSecurityMainDlg->m_delayAlarmClass[i]->m_repeatAlarmLastDateTime = dateTime;
	}

	for (int i = 0; i < m_pSecurityMainDlg->m_wirelessDefenseCnt; i++)
	{
		m_pSecurityMainDlg->m_delayAlarmClass[56 + i]->m_repeatAlarmLastDateTime = dateTime;
	}
}

void MainDialog::on_tableViewNotifier_pressed(const QModelIndex &index)
{
    //printf("ldh MonListDialog::on_tableViewNotifier_pressed:%d-%d\n",
    //       index.row(), index.column());

    if (index.row() == 0)
    {
        on_pushButtonSecurityMan_pressed();
        m_pSecurityMainDlg->on_pushButtonSecMainSetArm_pressed();
    }
    else if (index.row() == 1)
    {
        on_pushButtonInterFreeSetting_clicked();
    }
    else if (index.row() == 3)
    {
        on_pushButtonIntercomMan_pressed();
    }
    else if (index.row() == 4)
    {
        on_pushButtonSecurityMan_pressed();
    }
    else if (index.row() == 5)
    {
        on_pushButtonInfoMan_pressed();
    }
}
 
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Test_Fun1
 *  Description:  
 * =====================================================================================
 */
void MainDialog::Test_Fun1()
{
	on_pushButtonVideoMon_pressed();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  start_pwm_ring
 *  Description:  
 * =====================================================================================
 */
void MainDialog::start_pwm_ring()
{
	m_pDoorBellRingThread->start_pwm_export();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  stop_pwm_ring
 *  Description:  
 * =====================================================================================
 */
void MainDialog::stop_pwm_ring()
{
	m_pDoorBellRingThread->stop_pwm_export();
}
