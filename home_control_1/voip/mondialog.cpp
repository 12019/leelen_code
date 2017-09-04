#include "mondialog.h"
#include "ui_mondialog.h"
#include "lpcore.h"
#include "command.h"
#include "mediastreamer2/mediastream.h"
#include "exosip_main.h"
#include "application.h"
#include "data_types.h"
#include "maindialog.h"
#include "voipmaindialog.h"
//#include "terminalhelpmaindialog.h"
//#include "callcenterdialog.h"
//#include "helpdialog.h"
#include "ui_maindialog.h"
#include "ui_monlistdialog.h"
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "UsageEnvironment.hh"
#include "packetHandler.h"
#include "testRTSPClient.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

#define MAX_MON_TIME 10*60*1000      		// 监控10分钟
#define MAX_DIS_NET_TIMEOUT 40*1000

extern bool g_ScreenSave_Flag;      		// 屏保功能旁路标志
extern int g_door_bell_type;	    		// 二次门铃的类型的全局变量

static bool doorbell_power_open_flag = false;   // 二次门铃打开供电的标志
//static int current_channel = 0;			// 二次门铃的通道
static bool doorbell_channel_flag = false;		// 是否当前的通道是二次门铃的通道
extern QDateTime g_last_ipcdate_arrive_time;

extern "C"
{
#include "mediastreamer2/tvout.h"
}

/************************************************************
  该文件为视频监控对话框主程序，可对本机四个摄像头和门口机进行视频监控
************************************************************/
extern LinphoneCore *lphone;
extern eXosip_element_t *eXosipElement;
extern char g_strLphoneName[3][50];
extern ViewCam *g_pViewCam;
extern MainDialog *g_pMainDlg;
extern int g_bIstOrAlertBusy;

extern RTSPClient *g_pRTSPClient;
extern char eventLoopWatchVariable;
extern int g_rtsp_player_vrect[4];
extern int g_vrect_changed;
pthread_t g_rtspThread;

static int network_status = -1;  // 初始化网络为断开状态

static void *rtspPlayerThread(void *param)
{
    // Begin by setting up our usage environment:
    TaskScheduler* scheduler = BasicTaskScheduler::createNew();
    UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);

    initHandler();

    eventLoopWatchVariable = 0;

    // There are argc-1 URLs: argv[1] through argv[argc-1].  Open and start streaming each one:
    //openURL(*env, "rtsp_player", "rtsp://192.168.1.64:554/PSIA/streaming/channels/101");
    
    openURL(*env, "rtsp_player", (char *)param);

    delete (char *)param;

    // All subsequent activity takes place within the event loop:
    env->taskScheduler().doEventLoop(&eventLoopWatchVariable);
    // This function call does not return, unless, at some point in time, "eventLoopWatchVariable" gets set to something non-zero.

    if (g_pRTSPClient)
    {
        shutdownStream(g_pRTSPClient, 0);
    }

    env->reclaim(); env = NULL;
    delete scheduler; scheduler = NULL;

    unInitHandler();

    if (eventLoopWatchVariable != 3)
    {
        g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->emitSigIpCamFinish();
    }

    return 0;

    // If you choose to continue the application past thicxs point (i.e., if you comment out the "return 0;" statement above),
    // and if you don't intend to do anything more with the "TaskScheduler" and "UsageEnvironment" objects,
    // then you can also reclaim the (small) memory used by these objects by uncommenting the following code:
    /*
    env->reclaim(); env = NULL;
    delete scheduler; scheduler = NULL;
    */
}


//static bool ip_flag = false;
/************************************************************
  视频显示区域的背景色
************************************************************/
//#define PLAY_AREA_BGCOLOR QColor(0, 0, 0)

/************************************************************
描述：视频监控对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
MonDialog::MonDialog(QWidget *parent) :
    QDialog(parent),
	ui(new Ui::MonDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

	int x,y,w,h;
	g_pScaleSrc->get_info_framepanel_geometry(x,y,w,h);
	ui->framePanel->setGeometry(x,y,w,h);

	g_pScaleSrc->get_info_label_geometry(x,y,w,h);
	ui->lineEditInfo->setGeometry(x,y,w,h);

    ui->pushButtonMonOpen->setEnabled(false);

    int res = g_pScaleSrc->get_screen_version();
    if ( res == 2 )  // 1024x600
    {
        ui->pushButtonMonReturn->setGeometry(QRect(764, 500, 205, 90));
        ui->pushButtonMonHangUp->setGeometry(QRect(764, 340, 205, 90));
        ui->pushButtonMonOpen->setGeometry(QRect(764, 420, 205, 90));
        ui->pushButtonMonDial->setGeometry(QRect(764, 340, 205, 90));
        ui->framePanel->setGeometry(QRect(38, 17, 700, 570));
        ui->lineEditInfo->setGeometry(QRect(250, 30, 301, 51));
        ui->pushButtonMonList->setGeometry(QRect(764, 260, 205, 90));
        ui->labelExtension2->setGeometry(QRect(760, 110, 151, 31));
        ui->labelExtension1->setGeometry(QRect(760, 80, 151, 31));
    }
    else if ( res == 3 )  // 1280*800
    {
        ui->pushButtonMonReturn->setGeometry(QRect(959, 677, 279, 99));
        ui->pushButtonMonHangUp->setGeometry(QRect(959, 461, 279, 99));
        ui->pushButtonMonOpen->setGeometry(QRect(959, 569, 279, 99));
        ui->pushButtonMonDial->setGeometry(QRect(959, 461, 279, 99));
        ui->framePanel->setGeometry(QRect(56, 30, 858, 745));
        ui->lineEditInfo->setGeometry(QRect(220, 40, 511, 71));
        ui->pushButtonMonList->setGeometry(QRect(959, 353, 279, 99));
        ui->labelExtension2->setGeometry(QRect(1030, 170, 151, 31));
        ui->labelExtension1->setGeometry(QRect(1030, 130, 151, 31));
    }
    ////m_strRemoteIp[0] = '\0';

	m_pFrameFullshow = new QFrame(this);
	m_pFrameFullshow->setObjectName(QString::fromUtf8("frameFullshow"));
	m_pFrameFullshow->setGeometry(QRect(0, 0, qApp->desktop()->size().width(), qApp->desktop()->size().height()));
	m_pFrameFullshow->setFrameShadow(QFrame::Plain);
	m_pFrameFullshow->setFrameShape(QFrame::Panel);

	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	m_pFrameFullshow->installEventFilter(this);

    	g_pScaleSrc->get_videoshow_geometry(x,y,w,h);
	ui->frameVideoShow->setGeometry(x,y,w,h);

	ui->frameVideoShow->installEventFilter(this);

	//m_pMonDialDlg = new MonDialDialog(this);
	m_pMonListDlg = new MonListDialog(this);

	m_monType = LOCAL_CAMERA;

	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(timerCamListTimeout()));
	
	m_pIPCTimer = new QTimer(this);
	connect(m_pIPCTimer, SIGNAL(timeout()), this, SLOT(showIPCam()));

	m_pMonStopTimer = new QTimer(this);
	connect(m_pMonStopTimer, SIGNAL(timeout()), this, SLOT(Stop_Monitor()));

	m_bExtensionUnitMon = false;
	m_iExtensionUnitAddr = 0x30;
	m_iMonitoredAddr = 0;

	ui->labelExtension1->setText("");
	ui->labelExtension2->setText("");

	m_pFirstCamTimer = new QTimer(this);
	connect(m_pFirstCamTimer,SIGNAL(timeout()),this,SLOT(showFirstCam()));

	connect(this, SIGNAL(sigIpCamFinish()), this, SLOT(ipCamFinish()));

    m_CameraTimer = new QTimer(this);
    connect(m_CameraTimer, SIGNAL(timeout()), this, SLOT(Camera_Net_Disable()));

    m_pHandUpDoorViewTimer = new QTimer();
    connect(m_pHandUpDoorViewTimer,SIGNAL(timeout()),this,SLOT(slotHandUpdoorViewTimer()));

    m_pUnlockTimer = new QTimer(this);
    connect(m_pUnlockTimer,SIGNAL(timeout()),this,SLOT(slotUnlockTimerTimeout()));

    m_hasStopDoorbellViewFlag = false;
}

/************************************************************
描述：视频监控对话框析构函数
参数：无
返回：无
************************************************************/
MonDialog::~MonDialog()
{
	delete m_pTimer;
	delete m_pFrameFullshow;
	delete m_pIPCTimer;
	delete m_pMonStopTimer;

	//delete m_pMonDialDlg;
	delete m_pMonListDlg;

	delete ui;
}

/************************************************************
描述：重置本对话框
参数：无
返回：无
************************************************************/
void MonDialog::ResetState(bool bResetNetName)
{
	if (bResetNetName) m_strMonAddr = "";

	if (m_strMonAddr != "")
	{
		ui->lineEditInfo->setText(tr("视频监控") + " - " + m_strMonAddr);
	}
	else
	{
		ui->lineEditInfo->setText(tr("视频监控"));
	}

	/*QPalette palette;
	palette.setBrush(ui->frameVideoShow->backgroundRole(), QBrush(PLAY_AREA_BGCOLOR));
	ui->frameVideoShow->setPalette(palette);
	ui->frameVideoShow->setAutoFillBackground(true);
	*/
    ui->pushButtonMonDial->setEnabled(m_strMonAddr != "");
    ui->pushButtonMonDial->show();
    ui->pushButtonMonHangUp->hide();

    ui->pushButtonMonList->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：视频监控窗口监控门口机事件处理函数，实现lphone库MCT上发消息的处
	 理
参数：event - 需处理的事件
返回：true - 事件处理成功
	 false - 事件处理失败
************************************************************/
bool MonDialog::event(QEvent *event)
{
	switch (event->type() - 5000)
	{
	case NID_MCT_NONE:
		{
			ui->lineEditInfo->setText(tr("视频监控") + " - " + m_strMonAddr);
			break;
		}
	case NID_MCT_TIMEOUT:
		{
			/*setDisplayWindow(false);
			usleep(100000);

			if (m_pFrameFullshow->isVisible())
			{
				m_pFrameFullshow->hide();
				m_pFrameFullshow->lower();
			}*/

			ui->lineEditInfo->setText(tr("视频监控") + " - " + m_strMonAddr + tr("超时"));
			private_func2();
			break;
		}
	case NID_MCT_RCVBUSY:
		{
			ui->lineEditInfo->setText(tr("视频监控") + " - " + m_strMonAddr + tr("忙"));
			private_func2();
			break;
		}
	case NID_MCT_RCVCANCEL:
		{
            /*ui->pushButtonMonOpen->setEnabled(false);

            ui->pushButtonMonDial->show();
            ui->pushButtonMonHangUp->hide();
            ui->pushButtonMonDial->setFocus(Qt::TabFocusReason);

			((Application *)qApp)->setLCDAndTimerOn();
			*/
			ui->lineEditInfo->setText(tr("视频监控") + " - " + m_strMonAddr + tr("接收取消"));
			private_func2();
			break;
		}
	case	NID_MCT_CONNECTED:
		{
			////((Application *)qApp)->setLCDOn();
			////
			////((Application *)qApp)->setLCDAndTimerOn();
			////

			if (eXosipElement->audiostream != NULL)
			{
				audio_stream_set_level(eXosipElement->audiostream, ((VoipMainDialog *)parent())->m_nVolume);
			}

			//setAllButtonsEnabled(this, true);
			//ui->tableWidgetCamList->setEnabled(true);
            ui->pushButtonMonDial->hide();
            ui->pushButtonMonHangUp->setEnabled(true);
            ui->pushButtonMonHangUp->show();
            ui->pushButtonMonHangUp->setFocus(Qt::TabFocusReason);
            ui->pushButtonMonOpen->setEnabled(true);

			ui->lineEditInfo->setText(tr("视频监控") + " - " + m_strMonAddr + tr("已连接"));

			setDisplayWindow(true);
			break;
		}
	case	NID_IST_OPENTERM:
		{
			ui->lineEditInfo->setText(tr("视频监控") + " - " + m_strMonAddr + tr("开锁退出"));
			break;
		}
	case	NID_MCT_END:
		{
			if (m_pFrameFullshow->isVisible())
			{
				ui->lineEditInfo->show();
				m_pFrameFullshow->hide();
				m_pFrameFullshow->lower();
			}

			ui->frameVideoShow->repaint();

			ui->lineEditInfo->setText(tr("视频监控") + " - " + m_strMonAddr);

			setAllButtonsEnabled(this, true);
			ui->pushButtonMonOpen->setEnabled(false);

			ui->pushButtonMonDial->setEnabled(true);
			ui->pushButtonMonDial->show();
			ui->pushButtonMonHangUp->hide();
			//printf("lg 2\n");
			if(isVisible()) ui->pushButtonMonDial->setFocus(Qt::TabFocusReason);

			/*char strTemp[80];
			sprintf(strTemp, "/sbin/arp -d %s", m_strRemoteIp);
			system(strTemp);
			printf("%s\n", strTemp);
			*/
			////((Application *)qApp)->setLCDAndTimerOn();

			private_func2();
			break;
		}
	default:
		{
			return QDialog::event(event);
		}
	}

	return true;
}

/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void MonDialog::showEvent(QShowEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());

    pMainDlg->showRefreshTabFocus(this);

    int cnt = m_pMonListDlg->m_monListNetName.count();

    if(!cnt) return;

    QModelIndex index;

    QStringListModel *model = (QStringListModel *)m_pMonListDlg->ui->tableViewMonList->model();

//    index = model->index(2, 1);
    index = model->index(4, 0);

    m_pMonListDlg->ui->tableViewMonList->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);

    pMainDlg->ui->pushButtonVideoMon->setEnabled(false);
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void MonDialog::hideEvent(QHideEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());

	m_pMonListDlg->hide();

	stopViewer(true);

	pMainDlg->hideRefreshTabFocus(this);

	if (pMainDlg->m_pCurActiveDlg == pMainDlg)
	{
		pMainDlg->ui->pushButtonVideoMon->setFocus(Qt::TabFocusReason);
	}

    pMainDlg->ui->pushButtonVideoMon->setEnabled(true);
}

/************************************************************
描述：停止当前视频监控并关闭本窗口
参数：无
返回：无
************************************************************/
void MonDialog::on_pushButtonMonReturn_pressed()
{
	if (!isVisible()) return;

	private_func2();

	if (m_monType != IP_CAMERA)
	{
		stopViewer(true);

		if ( doorbell_power_open_flag == true && ( g_door_bell_type == 2 ) )	// 如果是第一通道则打开二次门铃电源
        {
            if(m_pHandUpDoorViewTimer->isActive())
            {
                m_hasStopDoorbellViewFlag = true;
                m_pHandUpDoorViewTimer->stop();
            }

            if(!m_pUnlockTimer->isActive())
            {
                g_pScaleSrc->doorbell_power(false);				// 当监控离开的时候关闭门口机电源
                doorbell_power_open_flag = false;
            }         
		}

		if ( g_door_bell_type == 1 || g_door_bell_type == 2 || g_door_bell_type == 3 )
			g_pMainDlg->m_pDoorBellDlg->Remote_Call_Interrupt_Wait();	// 如果有进行开门脉冲，退出的时候，要关闭
	}
	else
    {
        g_last_ipcdate_arrive_time = QDateTime::currentDateTime();

		if (eventLoopWatchVariable == 0)
		{
			setDisplayWindow(false);

			eventLoopWatchVariable = 3;

			void *ret;
			pthread_join(g_rtspThread, &ret);
		}
		else
		{
			eventLoopWatchVariable = 3;
		}

		if (m_pFrameFullshow->isVisible())
		{
			ui->lineEditInfo->show();
			m_pFrameFullshow->hide();
			m_pFrameFullshow->lower();

			//repaint();
		}

		ipCamFinish();
	}

	hide();
	((VoipMainDialog *)parent())->hide();
}

/************************************************************
描述：设置播放监控视频的显示窗口坐标，坐标值为0则停止视频显示
参数：bShow - true: 设置播放监控视频的显示窗口坐标并开启视频显示
			 false: 停止视频显示
返回：0 - 成功
	 -1 - 失败
************************************************************/
extern Lvirdd *lvirdd;
int MonDialog::setDisplayWindow(bool bShow)
{
	int vrect[4];
	QRect rect;

    if (m_monType != IP_CAMERA)
    {
        while (g_pViewCam == (ViewCam *)-1)
        {
            usleep(10000);
        }
        if (((!g_pViewCam) || (!g_pViewCam->videoout)) &&
            ((!lvirdd) || (!lvirdd->branch.display)))
        {
            return -1;
        }
    }

	if (!bShow)
	{
		vrect[0] = vrect[1] = 1;
		vrect[2] = vrect[3] = 1;
	}
	else
	{
		if (!m_pFrameFullshow->isVisible())
		{
			rect = ui->frameVideoShow->geometry();
			rect = QRect(rect.x() /*+ 2*/, rect.y() /*+ 2*/, rect.width() - 1 /*- 4*/, rect.height() - 1 /*- 4*/);
		}
		else
		{
			rect = m_pFrameFullshow->geometry();
			rect = QRect(rect.x() + FULLSCREEN_BORDER_WIDTH, rect.y() + FULLSCREEN_BORDER_WIDTH,
						 rect.width() - FULLSCREEN_BORDER_WIDTH * 2, rect.height() - FULLSCREEN_BORDER_WIDTH * 2);
		}

		vrect[0] = (rect.x()) & 0x7ffffffe;
		vrect[1] = rect.y();

		vrect[2] = rect.width() & 0x7ffffffe;
		vrect[3] = rect.height();
	}

    if (m_monType != IP_CAMERA)
    {
        if (g_pViewCam)
        {
            ms_filter_call_method(g_pViewCam->videoout, MS_FILTER_SET_VIDEO_RECT, vrect);
        }
        else
        {
            ms_filter_call_method(lvirdd->branch.display, MS_FILTER_SET_VIDEO_RECT, vrect);
        }
    }
    else
    {
        QDateTime datetime = QDateTime::currentDateTime();
        if(g_last_ipcdate_arrive_time.msecsTo(datetime) < 200)
        {
            memcpy(g_rtsp_player_vrect, vrect, sizeof(vrect));
            g_vrect_changed = 1;
        }
        else
        {
            closeVidevShow();

        }
    }

	return 0;
}

/************************************************************
描述：停止视频监控(停止本机摄像头监控和门口机监控)
参数：bMctTerminate - true:退出门口机的监控
					 false:不退出门口机的监控
返回：无
************************************************************/
void MonDialog::stopViewer(bool bMctTerminate)
{
#ifdef ARM11
	tvout_stop();
#endif

	((Application *)qApp)->setOnCamera(false);
	////((Application *)qApp)->setLCDAndTimerOn();
    setDisplayWindow(false);
    usleep(120000);

	if (bMctTerminate)
	{
		bMctTerminate = (eXosipElement->state == MCT_MONITOR);//(eXosipElement->videostream != NULL);
	}

	while (g_pViewCam == (ViewCam *)-1)
	{
		usleep(10000);
	}

	if ((m_monType != BELL_CAMERA) && (g_pViewCam == NULL) && (!bMctTerminate)) return;

    ui->pushButtonMonHangUp->setEnabled(false);
	ui->lineEditInfo->setFocus(Qt::TabFocusReason);

	if (m_monType == BELL_CAMERA)
	{
		g_pMainDlg->m_pDoorBellDlg->sendDoorBellCmd(0xfa);
		if (((Application *)qApp)->m_bTimerOff)
		{
			((Application *)qApp)->setLCDAndTimerOn();
		}
	}
	m_monType = LOCAL_CAMERA;

	if (m_pFrameFullshow->isVisible())
	{
		ui->lineEditInfo->show();
		m_pFrameFullshow->hide();
		m_pFrameFullshow->lower();
	}

	while (g_pViewCam == (ViewCam *)-1)
	{
		usleep(10000);
	}
	if (g_pViewCam)
	{
		ViewCam *tmp_pViewCam = g_pViewCam;
		g_pViewCam = (ViewCam *)-1;
		viewCam_stop(tmp_pViewCam);
		g_pViewCam = NULL;
	}

	if (bMctTerminate)
    {
		lpc_cmd_terminate(lphone, NULL);
		usleep(300000);
	}

	/*QPalette palette;
	palette.setBrush(ui->frameVideoShow->backgroundRole(), QBrush(PLAY_AREA_BGCOLOR));
	ui->frameVideoShow->setPalette(palette);
	ui->frameVideoShow->setAutoFillBackground(true);
	*/
	if (isVisible())
	{
		ui->frameVideoShow->repaint();
	}

    ui->pushButtonMonDial->setEnabled(true);
    ui->pushButtonMonDial->show();
    ui->pushButtonMonHangUp->hide();
    ui->pushButtonMonOpen->setEnabled(false);
	if (isVisible())
	{
		//printf("lg 3\n");
        ui->pushButtonMonDial->setFocus(Qt::TabFocusReason);
	}

	ui->labelExtension1->setText("");
	ui->labelExtension2->setText("");
	m_bExtensionUnitMon = false;
}

/************************************************************
描述：视频监控对话框事件过滤函数，实现播放监控视频窗口全屏/非全屏切换
参数：o - 产生事件的控件
	 e - 需处理的事件
返回：true - 事件处理成功，不需进一步处理
	 false - 需要进一步处理
************************************************************/
bool MonDialog::eventFilter(QObject *o, QEvent *e)
{
	if (e->type() == QEvent::MouseButtonPress)
	{
		if (!ui->pushButtonMonHangUp->isVisible())
		{
			return false;
		}
		if (o == ui->frameVideoShow)
		{   
			if ( network_status == -1 && m_monType == IP_CAMERA )			//  如果网络断线了，那么直接关闭rtsp功能
			{
				on_pushButtonMonHangUp_pressed();
				return false;
			}

			bool bTmp = g_pMainDlg->m_pDoorBellDlg->m_pInfoLabel->isVisible();

			if (bTmp) g_pMainDlg->m_pDoorBellDlg->m_pInfoLabel->hide();

			ui->lineEditInfo->hide();
			m_pFrameFullshow->show();
			m_pFrameFullshow->raise();

			if (!bTmp) repaint();
		}
		else if (o == m_pFrameFullshow)
		{
			ui->lineEditInfo->show();
			m_pFrameFullshow->hide();
			m_pFrameFullshow->lower();

            if(g_pViewCam == NULL && m_monType != IP_CAMERA )
            {
                ms_filter_call_method(lvirdd->branch.display,MS_FILTER_SET_VIDEO_HIDE,0);

            }
			repaint();
		}

		setDisplayWindow(true);
		
		if ( network_status == -1 && o == m_pFrameFullshow && m_monType == IP_CAMERA )   // 如果处于全屏幕状态 并且 网络断开 就挂断
		{
			on_pushButtonMonHangUp_pressed();
		}
	}

	return false;
}

void MonDialog::Do_Door_Bell_Close_Video()
{
	if ( m_monType == IP_CAMERA && m_pFrameFullshow->isVisible() )   // 如果是网络监控状态，并且是全屏幕状态，就退出全屏状态
	{
		ui->lineEditInfo->show();
		m_pFrameFullshow->hide();
		m_pFrameFullshow->lower();

		repaint();

		setDisplayWindow(true);
	}
	private_func1();
}

void MonDialog::showFirstCam()
{
    m_pFirstCamTimer->stop();
    extern int g_doorbell_on;
    if(((MainDialog*)(parent()->parent()))->m_pSecurityMainDlg->m_pSecurityAlertDlg->isVisible()||(g_doorbell_on==1))
    {
        hide();
        ((VoipMainDialog *)parent())->hide();
        return;
    }

    QString strRemoteNetName;

#if 0
    QSettings settings_config(CONFIG_FILE, QSettings::IniFormat);

    QString doorWay = settings_config.value("system/doorway_name", "1111-0001").toString();

    m_strMonAddr = QString("门口机");

    strRemoteNetName = doorWay;

#else
    int cnt = m_pMonListDlg->m_monListNetName.count();

    if(!cnt) return;

    m_strMonAddr = QString(m_pMonListDlg->m_monListName.at(4));

    doorbell_channel_flag = false;			// 当前通道不是门口机
//    if ( doorbell_power_open_flag == true )
//    {
//	    g_pScaleSrc->doorbell_power(false);		// 当监控开始关闭门口机电源
//        doorbell_power_open_flag = false;
//    }

    m_monType = DOOR_MACHINE;

    strRemoteNetName = QString(m_pMonListDlg->m_monListNetName.at(0));

#endif
    char strPeerIP[200] = "";
    int ret = 0;
    GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;

    ret = pGetPeerIPThread->GetPeerIP(strRemoteNetName.toLocal8Bit().data(), strPeerIP);

    printf("showFirstCam:%s\n",strPeerIP);
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

        //lg bug
        if (!(iModalType & DOORWAY_MODAL_TYPE))
        {
            setAllButtonsEnabled(this, false);
            ui->lineEditInfo->setFocus(Qt::TabFocusReason);
            MyMessageBox messBox(this);
            CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
            //Q_UNUSED(okButton);
            okButton->setFocus(Qt::TabFocusReason);
            messBox.setText(tr("对方不是门口机"));
            messBox.exec();
            setAllButtonsEnabled(this, true);

            ui->pushButtonMonOpen->setEnabled(false);
            ui->pushButtonMonDial->setEnabled(true);
            ui->pushButtonMonDial->show();
            ui->pushButtonMonHangUp->hide();
            ui->pushButtonMonList->setFocus(Qt::TabFocusReason);

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

    if (ret == 0)
    {
        //((Application *)qApp)->setLCDOn();

        printf("lg mct lpc_cmd_mon\n");
        if (lpc_cmd_mon(lphone, ptr) != 1)
        {
            ui->pushButtonMonDial->setEnabled(true);
            ui->pushButtonMonDial->setFocus(Qt::TabFocusReason);


            return;
        }

        ////strcpy(m_strRemoteIp, ptr);

        ui->lineEditInfo->setText(tr("视频监控") + " - " + m_strMonAddr + tr("正连接..."));

	if ( m_pMonStopTimer->isActive() )	  // 防止重新进入
		m_pMonStopTimer->stop();

	m_pMonStopTimer->start(MAX_MON_TIME);
	g_ScreenSave_Flag = true;   // 取消屏幕保护
    }
    else
    {
        setAllButtonsEnabled(this, false);
        ui->lineEditInfo->setFocus(Qt::TabFocusReason);
        MyMessageBox messBox(this);
        CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
        //Q_UNUSED(okButton);
        okButton->setFocus(Qt::TabFocusReason);
        messBox.setText(tr("获取") + m_strMonAddr + tr("的IP地址失败"));
        messBox.exec();

        setAllButtonsEnabled(this, true);
        ui->pushButtonMonOpen->setEnabled(false);

        ui->pushButtonMonDial->setEnabled(true);
        ui->pushButtonMonDial->show();
        ui->pushButtonMonHangUp->hide();
    }

    	systemRunLog(4);

    	ui->pushButtonMonList->setFocus(Qt::TabFocusReason);

}

/************************************************************
描述：10分钟后关闭监控
参数：无
返回：无
************************************************************/
void MonDialog::Stop_Monitor()
{
	m_pMonStopTimer->stop();
	g_ScreenSave_Flag = false;		// 注意顺序 一定要
	Application *app = (Application *)qApp;
	app->Re_Enable_Screen_Save();

	if ( m_pFrameFullshow->isVisible() )  // 全屏幕状态要先退出
	{
		ui->lineEditInfo->show();
		m_pFrameFullshow->hide();
		m_pFrameFullshow->lower();
		repaint();
		setDisplayWindow(true);
	}

	private_func1();   // 关闭监视
}

/************************************************************
描述：模仿timerCamListTimeout()函数生成IPCamera的函数
参数：无
返回：无
************************************************************/
void MonDialog::showIPCam()
{
	m_pIPCTimer->stop();
    //防止监控网络摄像机的同时被呼入的问题
    if ( eXosipElement->state != IDLE)
    {
        return;
    }
	if (!isVisible()) return;
	if (m_pMonListDlg->isVisible())
	{
		m_pMonListDlg->hide();
		repaint();
	}
	m_monType = IP_CAMERA; 

	QModelIndex index;
	index = m_pMonListDlg->ui->tableViewMonList->currentIndex();
	QStandardItemModel *model = (QStandardItemModel *)m_pMonListDlg->ui->tableViewMonList->model();
	if ((index.row() != -1) && (index.column() != -1))
	{
		m_strMonAddr =	model->item(index.row(), index.column())->text();
	}

	ui->lineEditInfo->setText(tr("视频监控") + " - " + m_strMonAddr);

	int cnt = m_pMonListDlg->m_ipCameraCnt;
	int i;
	for (i = 0; i < cnt; i++)
	{
		if (m_strMonAddr == m_pMonListDlg->m_monListName.at(i + 4 + m_pMonListDlg->m_doorMachineCnt))
		{
			if ( i + 4 + m_pMonListDlg->m_doorMachineCnt == index.row()+index.column()*m_pMonListDlg->get_max_rows()  )   // 5是因为每列为5个元素
				break;
		}
	}

	if (i >= cnt) return;

	doorbell_channel_flag = false;			// 当前通道不是门口机
	if ( doorbell_power_open_flag == true )
	{
		g_pScaleSrc->doorbell_power(false);	// 当监控开始关闭门口机电源 
        doorbell_power_open_flag = false;
	}

	QString st = "rtsp://";
	QString strUrl = st+m_pMonListDlg->m_monListNetName.at(i + m_pMonListDlg->m_doorMachineCnt);
	char *rtspUrl = new char[128];
	strcpy(rtspUrl, strUrl.toLocal8Bit().data());
    g_last_ipcdate_arrive_time = QDateTime::currentDateTime();
	pthread_create(&g_rtspThread, NULL, rtspPlayerThread, rtspUrl);
	setDisplayWindow(true);

	ui->pushButtonMonDial->hide();
	ui->pushButtonMonHangUp->setEnabled(true);
	ui->pushButtonMonHangUp->show();
	ui->pushButtonMonHangUp->setFocus(Qt::TabFocusReason);
	
	if ( m_pMonStopTimer->isActive() )	  // 防止重新进入
		m_pMonStopTimer->stop();

	m_pMonStopTimer->start(MAX_MON_TIME);
	g_ScreenSave_Flag = true;   // 取消屏幕保护
}

/************************************************************
描述：打开和摄像机和门口机列表控件中的条目相应的视频监控
参数：无
返回：无
************************************************************/
void MonDialog::timerCamListTimeout()
{
	m_pTimer->stop();
	if (!isVisible()) return;

	if (m_pMonListDlg->isVisible())
	{
		m_pMonListDlg->hide();
		repaint();
	}

	QModelIndex index;
	index = m_pMonListDlg->ui->tableViewMonList->currentIndex();

	QStandardItemModel *model = (QStandardItemModel *)m_pMonListDlg->ui->tableViewMonList->model();

	if ((index.row() != -1) && (index.column() != -1))
	{
		m_strMonAddr =	model->item(index.row(), index.column())->text();
	}

	ui->lineEditInfo->setFocus(Qt::TabFocusReason);
	ui->pushButtonMonDial->setEnabled(false);

	QString strModel1 = MonListDialog::tr("本地摄像头");
	QString strModel2 = MonListDialog::tr("门铃摄像头");

	ui->lineEditInfo->setText(tr("视频监控"));
	if (m_strMonAddr.left(strModel1.length()) == strModel1)
	{
		m_monType = LOCAL_CAMERA;

		int channel = m_strMonAddr.mid(strModel1.length()).toInt() - 1;
		if ( channel == 0 )            					// 是监视二次门口机
        {
            m_hasStopDoorbellViewFlag = false;
			doorbell_channel_flag = true;
			if ( doorbell_power_open_flag == false && g_door_bell_type == 2 )
			{
				g_pScaleSrc->doorbell_power(true);		// 当监控开始关闭门口机电源 
				doorbell_power_open_flag = true;
			}
		}

		if ( doorbell_channel_flag == true && g_door_bell_type != 0 )	// 如果是监视二次门铃，也要能开锁
		{
			ui->pushButtonMonOpen->setEnabled(true);
		}
		else
			ui->pushButtonMonOpen->setEnabled(false);

		MSWebCam *cam;
		cam = ms_web_cam_manager_get_default_cam(ms_web_cam_manager_get());

		printf("viewCam_start\n");
		while (g_pViewCam == (ViewCam *)-1)
		{
			usleep(10000);
		}

		if (g_pViewCam)
		{
			ViewCam *tmpViewCam = g_pViewCam;
			g_pViewCam = (ViewCam *)-1;
			viewCam_stop(tmpViewCam);
			g_pViewCam = NULL;
		}

		g_pViewCam = (ViewCam *)-1;
		g_pViewCam = viewCam_start(cam, channel);

		usleep(1000);

		setGM7113Channel(channel);

		if (g_pViewCam)
		{
            if ( channel == 0 && g_door_bell_type == 2 )		// 如果是第一通道则打开二次门铃电源
            {
                g_pScaleSrc->doorbell_power(true);		// 当监控开始打开门口机电源
                doorbell_power_open_flag = true;
            }

			ui->lineEditInfo->setText(tr("视频监控") + " - " + m_strMonAddr);

			ui->pushButtonMonDial->hide();
			ui->pushButtonMonHangUp->setEnabled(true);
			ui->pushButtonMonHangUp->show();
			//ui->pushButtonMonHangUp->setFocus(Qt::TabFocusReason);

			setDisplayWindow(true);

			((Application *)qApp)->setOnCamera(true);
			////((Application *)qApp)->setLCDOn();
		}
		else
		{
			ui->pushButtonMonDial->setEnabled(true);
			ui->pushButtonMonDial->show();
			ui->pushButtonMonHangUp->hide();
			//ui->pushButtonList->setFocus(Qt::TabFocusReason);
		}

		ui->pushButtonMonList->setFocus(Qt::TabFocusReason);
	}
	else if (m_strMonAddr.left(strModel2.length()) == strModel2)
	{
        doorbell_channel_flag = false;			// 非二次门铃
//        if ( doorbell_power_open_flag == true )
//        {
//            g_pScaleSrc->doorbell_power(false);		// 当监控开始关闭门口机电源
//            doorbell_power_open_flag = false;
//        }

		m_monType = BELL_CAMERA;

		int channel = m_strMonAddr.mid(strModel2.length()).toInt() - 1;
		ui->pushButtonMonOpen->setEnabled(false);

		if (!m_bExtensionUnitMon)
		{
			g_pMainDlg->m_pDoorBellDlg->sendDoorBellCmd(0xf7, 0x20 + channel);
		}

		MSWebCam *cam;
		cam = ms_web_cam_manager_get_default_cam(ms_web_cam_manager_get());

		printf("viewCam_start\n");
		while (g_pViewCam == (ViewCam *)-1)
		{
			usleep(10000);
		}
		g_pViewCam = (ViewCam *)-1;
		g_pViewCam = viewCam_start(cam, 1);

		usleep(1000);

		setGM7113Channel(1);

		if (g_pViewCam)
		{
			ui->lineEditInfo->setText(tr("视频监控") + " - " + m_strMonAddr);

			ui->pushButtonMonDial->hide();
			ui->pushButtonMonHangUp->setEnabled(true);
			ui->pushButtonMonHangUp->show();
			ui->pushButtonMonOpen->setEnabled(true);
			ui->pushButtonMonList->setFocus(Qt::TabFocusReason);

			repaint();

			setDisplayWindow(true);

			((Application *)qApp)->setOnCamera(true);
			////((Application *)qApp)->setLCDOn();
		}
		else
		{
			ui->pushButtonMonDial->setEnabled(true);
			ui->pushButtonMonDial->show();
			ui->pushButtonMonHangUp->hide();
			ui->pushButtonMonList->setFocus(Qt::TabFocusReason);
		}
	}
	else
	{
		int i;
		//int cnt = m_pMonListDlg->m_doorMachineCnt + m_pMonListDlg->m_wallMachineCnt;
		int cnt = m_pMonListDlg->m_doorMachineCnt;
		for (i = 0; i < cnt; i++)
		{
			if (m_strMonAddr == m_pMonListDlg->m_monListName.at(i + 4))
			{
				break;
			}
		}

		if (i >= cnt) return;


		m_monType = DOOR_MACHINE;

		/*QString strMess;
		  if (i < m_pMonListDlg->m_doorMachineCnt)
		  {
		  m_monType = DOOR_MACHINE;
		  strMess = tr("门口机");
		  }
		  else
		  {
		  m_monType = WALL_MACHINE;
		  strMess = tr("围墙机");
		  }*/

		QString strRemoteNetName = m_pMonListDlg->m_monListNetName.at(i);

		char strPeerIP[200] = "";
		int ret = 0;
		GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;

		ret = pGetPeerIPThread->GetPeerIP(strRemoteNetName.toLocal8Bit().data(), strPeerIP);

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

			//lg bug
			if (!(iModalType & DOORWAY_MODAL_TYPE))
			{
				setAllButtonsEnabled(this, false);
				ui->lineEditInfo->setFocus(Qt::TabFocusReason);
				MyMessageBox messBox(this);
				CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
				//Q_UNUSED(okButton);
				okButton->setFocus(Qt::TabFocusReason);
				messBox.setText(tr("对方不是门口机"));
				messBox.exec();
				setAllButtonsEnabled(this, true);

				ui->pushButtonMonOpen->setEnabled(false);
				ui->pushButtonMonDial->setEnabled(true);
				ui->pushButtonMonDial->show();
				ui->pushButtonMonHangUp->hide();
				ui->pushButtonMonList->setFocus(Qt::TabFocusReason);

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

		if (ret == 0)
		{
			//((Application *)qApp)->setLCDOn();

			printf("lg mct lpc_cmd_mon\n");
			if (lpc_cmd_mon(lphone, ptr) != 1)
			{
				ui->pushButtonMonDial->setEnabled(true);
				ui->pushButtonMonDial->setFocus(Qt::TabFocusReason);

				return;
			}

			////strcpy(m_strRemoteIp, ptr);

			ui->lineEditInfo->setText(tr("视频监控") + " - " + m_strMonAddr + tr("正连接..."));

			if ( m_pMonStopTimer->isActive() )	  // 防止重新进入
				m_pMonStopTimer->stop();

			m_pMonStopTimer->start(MAX_MON_TIME);
			g_ScreenSave_Flag = true;   // 取消屏幕保护
		}
		else
		{
			setAllButtonsEnabled(this, false);
			ui->lineEditInfo->setFocus(Qt::TabFocusReason);
			MyMessageBox messBox(this);
			CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
			//Q_UNUSED(okButton);
			okButton->setFocus(Qt::TabFocusReason);
			messBox.setText(tr("获取") + m_strMonAddr + tr("的IP地址失败"));
			messBox.exec();

			setAllButtonsEnabled(this, true);
			ui->pushButtonMonOpen->setEnabled(false);

			ui->pushButtonMonDial->setEnabled(true);
			ui->pushButtonMonDial->show();
			ui->pushButtonMonHangUp->hide();
		}

		systemRunLog(4);

		ui->pushButtonMonList->setFocus(Qt::TabFocusReason);

        doorbell_channel_flag = false;			// 非二次门铃
//        if ( doorbell_power_open_flag == true )
//        {
//            g_pScaleSrc->doorbell_power(false);	// 当监控开始关闭门口机电源
//            doorbell_power_open_flag = false;
//        }
	}

}
	
/************************************************************
描述：关闭视频的核心函数,不必判断当前是否已经处于关闭状态
参数：无
返回：无
************************************************************/
void MonDialog::private_func1()
{
	private_func2();

	if (m_monType == IP_CAMERA)
	{
		if (eventLoopWatchVariable == 0)
		{
			setDisplayWindow(false);

			eventLoopWatchVariable = 3;

			void *ret;
			pthread_join(g_rtspThread, &ret);
		}
		else
		{
			eventLoopWatchVariable = 3;
		}

		ipCamFinish();
	}
	else
	{
        //printf("focus:%s\n",qPrintable(qApp->focusWidget()->objectName()));
        if ( doorbell_channel_flag == true && g_door_bell_type == 2 && !m_pUnlockTimer->isActive() )	// 如果是第一通道则关闭二次门铃电源
        {
            g_pScaleSrc->doorbell_power(false);
			doorbell_power_open_flag = false;
			g_pMainDlg->m_pDoorBellDlg->Remote_Call_Interrupt_Wait();			// 如果有进行开门脉冲，退出的时候，要关闭
		}

		stopViewer(true);
	}
}
void MonDialog::private_func2()
{
	g_ScreenSave_Flag = false;				// 结束屏保旁路状态
	if ( m_pMonStopTimer->isActive() )			// 结束超时定时器
		m_pMonStopTimer->stop();
	Application *app = (Application *)qApp;
	app->Re_Enable_Screen_Save();				// 再次开始45秒超时屏保计时
}

/************************************************************
描述：停止当前的视频监控
参数：无
返回：无
************************************************************/
void MonDialog::on_pushButtonMonHangUp_pressed()
{
	if (!ui->pushButtonMonHangUp->isVisible()) return;

    if(m_pHandUpDoorViewTimer->isActive())
    {
        m_hasStopDoorbellViewFlag = true;
        m_pHandUpDoorViewTimer->stop();
    }

	private_func1();
}
/************************************************************
描述：闭合二次门铃锁定时器
参数：无
返回：无
************************************************************/
void MonDialog::slotUnlockTimerTimeout()
{
    if(m_pUnlockTimer->isActive())
    {
        m_pUnlockTimer->stop();
    }

    g_pScaleSrc->doorbell_lock(false);

    if(m_hasStopDoorbellViewFlag)
    {
        g_pScaleSrc->doorbell_power(false);
        doorbell_power_open_flag = false;
        m_hasStopDoorbellViewFlag = false;
    }

}
/************************************************************
描述：监视二次门铃开锁五秒后挂断视频
参数：无
返回：无
************************************************************/
void MonDialog::slotHandUpdoorViewTimer()
{
    if(m_pHandUpDoorViewTimer->isActive())
    {
        m_pHandUpDoorViewTimer->stop();
    }

    m_hasStopDoorbellViewFlag = true;
    stopViewer(true);

}
/************************************************************
描述：如果正在监控门口机，向门口机发送开锁命令
参数：无
返回：无
************************************************************/
void MonDialog::on_pushButtonMonOpen_pressed()
{
	if (m_monType != BELL_CAMERA)
    {
		if ( doorbell_channel_flag == true && g_door_bell_type != 0 )	// 如果是监视二次门铃，也要能开锁
        {
            m_hasStopDoorbellViewFlag = false;
            m_pHandUpDoorViewTimer->start(5*1000);
            g_pScaleSrc->doorbell_lock(true);

            QSettings settings(CONFIG_FILE, QSettings::IniFormat);
            int intdoortimeout = settings.value("alert_param/door_timeout_time", "0").toInt();
            m_pUnlockTimer->start(1000*intdoortimeout);
            //g_pMainDlg->m_pDoorBellDlg->Remote_Call_Open_Door_Bell();
		}
		else
			lpc_cmd_open(lphone, NULL);
	}
	else
    {
		g_pMainDlg->m_pDoorBellDlg->sendDoorBellCmd(0xf8);
	}
}

/************************************************************
描述：打开和摄像机和门口机列表控件中的条目相应的视频监控
参数：无
返回：无
************************************************************/
void MonDialog::on_pushButtonMonDial_pressed()
{
	if (m_monType == IP_CAMERA)
	{
		m_pIPCTimer->start(100);
	}
	else
	{
		while (g_pViewCam == (ViewCam *)-1)
		{
			usleep(10000);
		}

		if ((eXosipElement->videostream != NULL) || (g_pViewCam != NULL)) return;
        if(m_pUnlockTimer->isActive())
        {
            m_pUnlockTimer->stop();
            slotUnlockTimerTimeout();
        }

        m_hasStopDoorbellViewFlag = false;

		m_pTimer->start(500);
	}
}

/************************************************************
描述：关闭其它窗口，打开视频监控对话框
参数：无
返回：无
************************************************************/
void MonDialog::showMonDlg()
{
	VoipMainDialog *pVoipMainDlg = (VoipMainDialog *)parent();

	if (pVoipMainDlg->isVisible())
	{
		if (pVoipMainDlg->m_pIctDlg->m_pRecvHookDlg->isVisible()) return;
		if (pVoipMainDlg->m_pIstDlg->isVisible()) return;
	}
	if (g_pMainDlg->m_pCallCenterDlg->isVisible()) return;

	if (!pVoipMainDlg->isVisible() || !isVisible())
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

		g_bIstOrAlertBusy = 4;

		pVoipMainDlg->closeVoipDialog();
		pVoipMainDlg->closeOtherRelatedDialog();

		ResetState();
        pVoipMainDlg->show();
        pVoipMainDlg->raise();
        show();
		raise();

		g_bIstOrAlertBusy = 0;
	}
	else
	{
		bool bExtensionUnitMon = m_bExtensionUnitMon;
		stopViewer(true);
		m_bExtensionUnitMon = bExtensionUnitMon;
	}

	if (m_bExtensionUnitMon)
	{
		QStandardItemModel *model = (QStandardItemModel *)m_pMonListDlg->ui->tableViewMonList->model();
		int i = 0, j;
		bool bFound = false;
		for (j = 0; j < model->columnCount(); j++)
		{
			for (i = 0; i < model->rowCount(); i++)
			{
				QStandardItem *item = model->item(i, j);
				if (!item) continue;

				if (item->text() == MonListDialog::tr("门铃摄像头") + "1")
				{
					bFound = true;
					break;
				}
			}

			if (bFound) break;
		}

		if (bFound)
		{
			m_pMonListDlg->ui->tableViewMonList->setCurrentIndex(model->index(i, j));

			int indx = m_iExtensionUnitAddr & 0x0f;
			ui->labelExtension1->setText(IstDialog::tr("扩展分机") + QString::number(indx + 1));
			ui->labelExtension2->setText(IstDialog::tr("监控中"));

			timerCamListTimeout();

#ifdef ARM11
			tvout_start();
#endif
		}
	}
}
void MonDialog::on_pushButtonMonList_pressed()
{
    if (m_pMonListDlg->isVisible()) return;

    if (m_monType == IP_CAMERA)
    {
         g_last_ipcdate_arrive_time = QDateTime::currentDateTime();
         if (eventLoopWatchVariable == 0)
         {
             setDisplayWindow(false);

             eventLoopWatchVariable = 3;

             void *ret;
             pthread_join(g_rtspThread, &ret);

             ipCamFinish();
         }
    }
    else
    {
        stopViewer(true);
    }

    m_pMonListDlg->show();
}

/************************************************************
 * 测试
************************************************************/
void MonDialog::keyPressEvent(QKeyEvent *k)
{
	qDebug("mon dialog key value %x",k->key());
	if ( k->key() == 0x1000031 )
		on_pushButtonMonOpen_pressed();
}

void MonDialog::emitSigIpCamFinish()
{
    emit sigIpCamFinish();
}

void MonDialog::ipCamFinish()
{
    if (eventLoopWatchVariable != 3)
    {
        g_last_ipcdate_arrive_time = QDateTime::currentDateTime();
        setDisplayWindow(false);

        void *ret;
        pthread_join(g_rtspThread, &ret);

        setAllButtonsEnabled(this, false);
        ui->lineEditInfo->setFocus(Qt::TabFocusReason);
        MyMessageBox messBox(this);
        CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
        //Q_UNUSED(okButton);
        okButton->setFocus(Qt::TabFocusReason);
        messBox.setText(tr("连接网络摄像头失败"));
        messBox.exec();
        setAllButtonsEnabled(this, true);

	private_func2();
    }

    ui->pushButtonMonDial->setEnabled(true);
    ui->pushButtonMonDial->show();
    ui->pushButtonMonHangUp->hide();
    ui->pushButtonMonOpen->setEnabled(false);
    if (isVisible())
    {
        //printf("lg 3\n");
        ui->pushButtonMonDial->setFocus(Qt::TabFocusReason);
    }
}

void MonDialog::Network_Status_Chang(int v)
{
	network_status = v;

    if ( m_CameraTimer->isActive() )
        m_CameraTimer->stop();

	if ( network_status == -1 )	 	// 断网
	{
        m_CameraTimer->start(MAX_DIS_NET_TIMEOUT);   // 40秒定时
	}
}

/************************************************************
描述：当网络断线超时进行处理
参数：无
返回：无
************************************************************/
void MonDialog::Camera_Net_Disable()
{
    m_CameraTimer->stop();
    // 如果当前状态为IP摄像头和门口机
    if ( m_monType == IP_CAMERA || m_monType == DOOR_MACHINE )
	{
		if ( m_pFrameFullshow-> isVisible() )   // 如果是全屏幕状态
		{
			ui->lineEditInfo->show();
			m_pFrameFullshow->hide();
			m_pFrameFullshow->lower();
			repaint();
			setDisplayWindow(true);
		}
		on_pushButtonMonHangUp_pressed();
	}
}

/************************************************************
描述：从全屏状态退出
参数：无
返回：无
************************************************************/
void MonDialog::Exit_From_Full_Screen_Status()
{
	if ( m_pFrameFullshow-> isVisible() )   // 如果是全屏幕状态
	{
		ui->lineEditInfo->show();
		m_pFrameFullshow->hide();
		m_pFrameFullshow->lower();
		repaint();
		setDisplayWindow(true);
	}
}
