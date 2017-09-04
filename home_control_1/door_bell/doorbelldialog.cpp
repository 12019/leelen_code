#include "doorbelldialog.h"
#include "ui_doorbelldialog.h"
#include "mediastreamer2/mediastream.h"
#include "exosip_main.h"
#include "maindialog.h"
#include "application.h"
//#include "helpdialog.h"
#include "ui_maindialog.h"
#include "core/leelen_comdef.h"
#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
extern int g_mainMonitorFlag;
extern "C"
{
    #include "mediastreamer2/tvout.h"
}
//#include "ui_mondialog.h"

/************************************************************
  该文件为户门口机门铃可视对讲对话框主程序
************************************************************/

extern eXosip_t eXosip;
extern eXosip_element_t *eXosipElement;
extern int b_ring_play_run;
extern int g_bIstOrAlertBusy;
extern ViewCam *g_pViewCam;
extern MainDialog *g_pMainDlg;
extern int g_doorbell_on;
extern RESULT_E g_check_computercenter_udp_state ;
extern char g_strNativeName[50];
extern int g_door_bell_type;	    		// 二次门铃的类型的全局变量

static bool m_interrupt;			// 如果在开门脉冲等待的同时按退出按钮进行退出
/************************************************************
  视频显示区域的背景色
************************************************************/
//#define PLAY_AREA_BGCOLOR QColor(0, 0, 0)

/************************************************************
描述：户门口机门铃可视对讲对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
DoorBellDialog::DoorBellDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DoorBellDialog)
{
	ui->setupUi(this);

	//去掉 Dialog 标题栏
	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

	int x,y,w,h;
	g_pScaleSrc->get_info_framepanel_geometry(x,y,w,h);
	ui->framePanel->setGeometry(x,y,w,h);

    g_pScaleSrc->get_videoshow_geometry(x,y,w,h);
    ui->frameVideoShow->setGeometry(x,y,w,h);

	g_pScaleSrc->get_info_label_geometry(x,y,w,h);
	ui->labelInfo->setGeometry(x,y,w,h);

    int res = g_pScaleSrc->get_screen_version();
    if ( res == 2 )  // 1024x600
    {
        ui->pushButtonDoorBellReturn->setGeometry(QRect(764, 500, 230, 80));
        ui->pushButtonDoorBellOpen->setGeometry(QRect(764, 420, 230, 80));
        ui->pushButtonDoorBellAnswer->setGeometry(QRect(764, 340, 230, 80));
        ui->labelExtension1->setGeometry(QRect(781, 225, 193, 39));
        ui->labelExtension2->setGeometry(QRect(781, 262, 193, 39));
    }
    else if ( res == 3 )  // 1280x800
    {
        ui->pushButtonDoorBellReturn->setGeometry(QRect(959, 677, 279, 99));
        ui->pushButtonDoorBellOpen->setGeometry(QRect(959, 569, 279, 99));
        ui->pushButtonDoorBellAnswer->setGeometry(QRect(959, 461, 279, 99));
        ui->labelExtension1->setGeometry(QRect(976, 300, 241, 51));
        ui->labelExtension2->setGeometry(QRect(976, 350, 241, 51));
    }

	m_pFrameFullshow = new QFrame(this);
	m_pFrameFullshow->setObjectName(QString::fromUtf8("frameFullshow"));
	m_pFrameFullshow->setGeometry(QRect(0, 0, qApp->desktop()->size().width(), qApp->desktop()->size().height()));
	m_pFrameFullshow->setFrameShadow(QFrame::Plain);
	m_pFrameFullshow->setFrameShape(QFrame::Panel);

	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	m_pFrameFullshow->installEventFilter(this);
	ui->frameVideoShow->installEventFilter(this);

	m_pInfoLabel = new QLabel(g_pMainDlg);
	m_pInfoLabel->setObjectName("doorbell_label");
	m_pInfoLabel->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool);
	m_pInfoLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    if ( res == 1 )  //800x480
        m_pInfoLabel->setGeometry(590, 50, 193, 80);
    else if ( res == 2 )  // 1024x600
        m_pInfoLabel->setGeometry(755, 62, 247, 100);
    else if ( res == 3 )  // 1280x800
        m_pInfoLabel->setGeometry(944, 83, 309, 133);
	m_pInfoLabel->setFocusPolicy(Qt::NoFocus);
	m_pInfoLabel->hide();

	//m_iPeerAddr = 0;

	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(on_pushButtonDoorBellReturn_pressed()));

    m_pCloseLockTimer = new QTimer(this);
    connect(m_pCloseLockTimer,SIGNAL(timeout()),this,SLOT(slotCloseLockTimerTimeout()));

	m_pInfoLabelTimer = new QTimer(this);
	connect(m_pInfoLabelTimer, SIGNAL(timeout()), this, SLOT(InfoLabelTimerTimeout()));

	m_pDisplayTimer = new QTimer(this);
	connect(m_pDisplayTimer, SIGNAL(timeout()), this, SLOT(DisplayTimerTimeout()));

	m_bExtensionUnitAnswer = false;
	m_iExtensionUnitAddr = 0x30;
    m_OpenDoorFlag = false;
}

/************************************************************
描述：户门口机门铃可视对讲对话框析构函数
参数：无
返回：无
************************************************************/
DoorBellDialog::~DoorBellDialog()
{
	delete m_pFrameFullshow;
	delete m_pInfoLabel;
	delete m_pTimer;
	delete m_pInfoLabelTimer;
	delete m_pDisplayTimer;

	delete ui;
}

void DoorBellDialog::sendDoorBellCmd(int iCmd, int iPeerAddr)
{
	g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->waitForVacant();

	/*while (g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iDoorBellCmd != 0)
	{
		usleep(50000);
	}*/

	if (iPeerAddr != -1)
	{
		g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iDoorBellAddr = iPeerAddr;
	}

	g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iDoorBellCmd = iCmd;

	g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->waitForVacant();

	/*while (g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iDoorBellCmd != 0)
	{
		usleep(50000);
	}*/
}


/************************************************************
描述：户门口机可视对讲对话框事件过滤函数，实现播放监控视频窗口全屏/非
	 全屏切换
参数：o - 产生事件的控件
	 e - 需处理的事件
返回：true - 事件处理成功，不需进一步处理
	 false - 需要进一步处理
************************************************************/
bool DoorBellDialog::eventFilter(QObject *o, QEvent *e)
{
	if (e->type() == QEvent::MouseButtonPress)
	{
		setDisplayWindow(false);
		usleep(120000);

		if (o == ui->frameVideoShow)
		{
			m_pFrameFullshow->show();
			m_pFrameFullshow->raise();
		}
		else if (o == m_pFrameFullshow)
		{
			m_pFrameFullshow->hide();
			m_pFrameFullshow->lower();
		}

		repaint();
		setDisplayWindow(true);
	}

	return false;
}

bool DoorBellDialog::event(QEvent *e)
{
	return QDialog::event(e);
}

/************************************************************
描述：当来客按了门铃或用户查看门铃视频时，调用该函数，打开本对话框
参数：bCallIn - true: 来客按了门铃触发该函数
			   false: 用户查看门铃视频触发该函数
返回：无
************************************************************/
void DoorBellDialog::OpenDoorBellDlg()
{
	MainDialog *pMainDlg = (MainDialog *)parent();

	if (g_bIstOrAlertBusy == 3) return;

	g_doorbell_on = 1;
    g_mainMonitorFlag = 1;
	m_pInfoLabel->hide();

	int loop_cnt = 0;
	while (g_bIstOrAlertBusy)
	{
		usleep(5000);
		if (g_bIstOrAlertBusy == 3) return;
		if (loop_cnt++ >= 8) //40 ms
		{
			qApp->processEvents();
			loop_cnt = 0;
		}
	}

	g_bIstOrAlertBusy = 3;

	bool bTmp = false;
	if ((eXosipElement->state == ICT_CALLING)
		|| (eXosipElement->state == ICT_RINGBACK)
		|| (eXosipElement->state == IST_RING))
	{
		bTmp = true;
	}

	if (eXosip.callflag == 0)
	{
		if ((eXosipElement->videostream != NULL) || (eXosipElement->audiostream != NULL))
		{
			bTmp = true;
		}
	}
	else
	{
		int i;
		for (i = 0; i < MAX_EXTENSION_MACHINE_CNT; i++) {
			if ((eXosip.remip[i].videostream != NULL)
				|| (eXosip.remip[i].audiostream != NULL))
			{
				bTmp = true;
				break;
			}
		}
	}

	//if (b_ring_play_run || (eXosipElement->videostream != NULL) || (eXosipElement->audiostream != NULL))
	if (b_ring_play_run || bTmp)
	{
		//m_pInfoLabelTimer->start(5375);

        if ((g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->isVisible())
            || (g_pMainDlg->m_pVoipMainDlg->m_pIstDlg->isVisible())
            //&& (!g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->m_pFrameFullshow->isVisible())
                || (g_pMainDlg->m_pCallCenterDlg->isVisible())||(g_pMainDlg->m_pSecurityMainDlg->m_pSecurityAlertDlg->isVisible()))
		{
            m_pInfoLabel->setText(tr("门铃") + /*QString::number((pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iDoorBellAddr & 0x0f) + 1) +*/ tr("响了"));

			m_pInfoLabel->raise();
			m_pInfoLabel->show();
			m_pInfoLabelTimer->start(5500);

			sendDoorBellCmd(0xf9);
			g_doorbell_on = 0;
			g_bIstOrAlertBusy = 0;

			return;
		}

            //sendDoorBellCmd(0xf9);
            //g_doorbell_on = 0;
            //g_bIstOrAlertBusy = 0;


	}

	pMainDlg->m_pVoipMainDlg->closeOtherRelatedDialog();

	if (!pMainDlg->m_bInterFree)
	{
		my_ring_start((char *)"wav/doorbell.wav", 2000);

		my_ring_set_level(((MainDialog *)parent())->m_pVoipMainDlg->m_nRingVolume);//david 2014.4.8
	}

	ui->pushButtonDoorBellAnswer->setEnabled(true);
	////ui->pushButtonDoorBellAnswer->setFocus(Qt::TabFocusReason);

    //门铃Door_Bell_UNLOCK设为空闲状态
    QFileInfo file_gpio193("/sys/class/gpio/gpio193");
    bool exist= file_gpio193.isDir();

    if (!exist)
        system("echo '193'>/sys/class/gpio/export");
    system("echo 'out'>/sys/class/gpio/gpio193/direction");//设为输出

    system("echo '1'>/sys/class/gpio/gpio193/value");

    //门铃上电
    QFileInfo file_gpio194("/sys/class/gpio/gpio194");
    if (!exist)
        system("echo '194'>/sys/class/gpio/export");
    system("echo 'out'>/sys/class/gpio/gpio194/direction");//设为输出
    system("echo '1'>/sys/class/gpio/gpio194/value");//打开门铃电源

    //门铃pwm音频输出
    g_pMainDlg->m_pDoorBellRingThread->start_pwm_export();

	sendDoorBellCmd(0xfc);

	/*QPalette palette;
	palette.setBrush(ui->frameVideoShow->backgroundRole(), QBrush(PLAY_AREA_BGCOLOR));
	ui->frameVideoShow->setPalette(palette);
	ui->frameVideoShow->setAutoFillBackground(true);
	*/

    ui->labelInfo->setText(tr("门铃") + /*QString::number((pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iDoorBellAddr & 0x0f) + 1) +*/ tr("响了"));
	ui->labelExtension1->setText("");
	ui->labelExtension2->setText("");

	m_bExtensionUnitAnswer = false;

	show();
	raise();

	repaint();

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

	setDisplayWindow(false);			// 防止显示异常
	usleep(1000);

	setGM7113Channel(1);
	//tvout_start();

	m_pTimer->start(50000);//50秒
	((Application *)qApp)->setLCDOn();

	m_pDisplayTimer->start(400);
	//setDisplayWindow(true);

	g_bIstOrAlertBusy = 0;

    	//david


        CallInfoUploadThread *pCallInfoUploadThread = g_pMainDlg->m_pVoipMainDlg->m_pCallInfoUploadThread;

        printf("doorbelldialog openDoorBellDlg m_bRun:%d\n",pCallInfoUploadThread->m_bRun);
        if(g_check_computercenter_udp_state == RESULT_NULL)
        {          
            if( pCallInfoUploadThread->m_bRun == false)
            {
                pCallInfoUploadThread->m_upInfoType = 1;
                pCallInfoUploadThread->getCallCode(QString(g_strNativeName));
                pCallInfoUploadThread->m_IsUpImage = 1;
                pCallInfoUploadThread->start();
            }
        }

}

/************************************************************
描述：设置播放监控视频的显示窗口坐标，坐标值为0则停止视频显示
参数：bShow - true: 设置播放监控视频的显示窗口坐标并开启视频显示
			 false: 停止视频显示
返回：0 - 成功
	 -1 - 失败
************************************************************/
int DoorBellDialog::setDisplayWindow(bool bShow)
{
	int vrect[4];
	QRect rect;

	while (g_pViewCam == (ViewCam *)-1)
	{
		usleep(10000);
	}

	if ((!g_pViewCam) || (!g_pViewCam->videoout))
	{
		return -1;
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

	ms_filter_call_method(g_pViewCam->videoout, MS_FILTER_SET_VIDEO_RECT, vrect);

	return 0;
}

/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void DoorBellDialog::showEvent(QShowEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)parent();

	pMainDlg->showRefreshTabFocus(this);

//	setAllButtonsFocusPolicy(this, Qt::NoFocus);
	ui->pushButtonDoorBellOpen->setFocus();
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void DoorBellDialog::hideEvent(QHideEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)parent();

	stopViewer(); //bug

	pMainDlg->hideRefreshTabFocus(this);
}

/************************************************************
描述：停止查看门铃视频
参数：无
返回：无
************************************************************/
void DoorBellDialog::stopViewer()
{
	setDisplayWindow(false);

	usleep(120000);

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

	if (m_pFrameFullshow->isVisible())
	{
		m_pFrameFullshow->hide();
		m_pFrameFullshow->lower();
	}
    else
        ui->frameVideoShow->update();


	/*QPalette palette;
	palette.setBrush(ui->frameVideoShow->backgroundRole(), QBrush(PLAY_AREA_BGCOLOR));
	ui->frameVideoShow->setPalette(palette);
	ui->frameVideoShow->setAutoFillBackground(true);
	*/
    //ui->frameVideoShow->repaint();

	((Application *)qApp)->setLCDAndTimerOn();
}

/************************************************************
描述：开始和户门口机进行语音对讲
参数：无
返回：无
************************************************************/
void DoorBellDialog::on_pushButtonDoorBellAnswer_pressed()
{
    if (!ui->pushButtonDoorBellAnswer->isEnabled()) return;

	printf("lg DoorBell answer\n");

	ui->pushButtonDoorBellAnswer->setEnabled(false);
	////ui->pushButtonDoorBellOpen->setFocus(Qt::TabFocusReason);

	my_ring_stop();
    g_pMainDlg->m_pDoorBellRingThread->stop_pwm_export();

    system("echo '1'>/sys/kernel/debug/fm1188");//转换为模拟音频

	if (!m_bExtensionUnitAnswer)
	{
		//设置GPIO和户门口机音频接通
		((Application *)qApp)->setUnmuteMic(true);
		((Application *)qApp)->setGPH8(true);
		((Application *)qApp)->setGPE0(false);

		sendDoorBellCmd(0xfb);

		ui->labelExtension1->setText(IstDialog::tr("本 机"));
		ui->labelExtension2->setText(IstDialog::tr("　接听　"));
	}
	else
	{
		//设置GPIO和户门口机音频接通
		//((Application *)qApp)->setUnmuteMic(false);
		((Application *)qApp)->setUnmuteSpeaker(false);
		((Application *)qApp)->setGPH7(true);
		((Application *)qApp)->setGPH8(true);
		((Application *)qApp)->setGPE0(false);

		int indx = m_iExtensionUnitAddr & 0x0f;
		ui->labelExtension1->setText(IstDialog::tr("扩展分机") + QString::number(indx + 1));
		ui->labelExtension2->setText(IstDialog::tr("　接听　"));
	}

    ui->labelInfo->setText(tr("门铃") + /*QString::number((g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iDoorBellAddr & 0x0f) + 1) + */tr("通话中"));

	m_pTimer->start(75000);//75秒

	//david

    CallInfoUploadThread *pCallInfoUploadThread = g_pMainDlg->m_pVoipMainDlg->m_pCallInfoUploadThread;

    if( pCallInfoUploadThread->m_bRun == false)
    {
        if(g_check_computercenter_udp_state == RESULT_SUCCESS)
        {
            pCallInfoUploadThread->m_upInfoType |= RICT_ANSER;
            pCallInfoUploadThread->getCallCode(QString(g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread->m_nativeName));
            pCallInfoUploadThread->m_TalkTime = 0;
            pCallInfoUploadThread->m_TalkTimer->start(1000);
        }
        else
        {
            pCallInfoUploadThread->m_upInfoType = 2;
            pCallInfoUploadThread->getCallCode(QString(g_strNativeName));
            pCallInfoUploadThread->start();
        }
    }
}

/**********************************
**************************
描述：停止和户门口机的可视对讲，关闭本对话框
参数：无
返回：无
************************************************************/
void DoorBellDialog::CloseDoorBellDlg()
{
    if (!isVisible()) return;

	sendDoorBellCmd(0xfa);
    stopViewer();

    if(!m_OpenDoorFlag)
    {
        if ( g_door_bell_type != 3 )     		  // 如果不是可视带刷卡门铃的类型
        {
            system("echo '0'>/sys/class/gpio/gpio194/value"); //关闭门铃电源
        }
    }

	system("echo '0'>/sys/kernel/debug/fm1188");	  //转换为数字音频

	if (!m_bExtensionUnitAnswer)
	{
		//设置GPIO取消和户门口机音频接通
		((Application *)qApp)->setUnmuteMic(false);
		((Application *)qApp)->setGPH8(false);
		((Application *)qApp)->setGPE0(true);
	}
	else
	{
#ifdef ARM11
		tvout_stop();
#endif

		//设置GPIO取消和户门口机音频接通
		//((Application *)qApp)->setUnmuteMic(false);
		((Application *)qApp)->setUnmuteSpeaker(true);
		((Application *)qApp)->setGPH7(false);
		((Application *)qApp)->setGPH8(false);
		((Application *)qApp)->setGPE0(true);
	}

	m_pTimer->stop();
	my_ring_stop();
    g_pMainDlg->m_pDoorBellRingThread->stop_pwm_export();
    //stopViewer();

	QKeyEvent *modifiedEvent = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
	QApplication::postEvent((QObject *)ui->pushButtonDoorBellOpen, modifiedEvent);

	hide();

	m_bExtensionUnitAnswer = false;

	if (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg)
	{
		g_pMainDlg->repaint();
	}

    if(g_check_computercenter_udp_state == RESULT_SUCCESS)
    {
        CallInfoUploadThread *pCallInfoUploadThread = g_pMainDlg->m_pVoipMainDlg->m_pCallInfoUploadThread;
        pCallInfoUploadThread->m_TalkTimer->stop();
        pCallInfoUploadThread->getCallCode(g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread->m_nativeName);
        pCallInfoUploadThread->SendRecordByUdp();
        g_check_computercenter_udp_state = RESULT_NULL;
    }
	g_doorbell_on = 0;
}

/************************************************************
描述：发送关闭户门口机摄像头命令，停止和户门口机的可视对讲，关闭本对话
	 框
参数：无
返回：无
************************************************************/
void DoorBellDialog::on_pushButtonDoorBellReturn_pressed()
{
	if (!isVisible()) return;

	printf("timeout\n");

	m_interrupt = true;			// 停止等待
	usleep(1000*200);
	CloseDoorBellDlg();
}

/************************************************************
描述：给监控模块调用
参数：无
返回：无
************************************************************/
void DoorBellDialog::Remote_Call_Open_Door_Bell()
{
	on_pushButtonDoorBellOpen_pressed();
}

/************************************************************
描述：给监控模块调用
参数：无
返回：无
************************************************************/
void DoorBellDialog::Remote_Call_Interrupt_Wait()
{
	m_interrupt = true;			// 停止等待
	usleep(1000*200);
	if ( m_pTimer->isActive() == true )
		m_pTimer->stop();
}

void DoorBellDialog::slotCloseLockTimerTimeout()
{
    if(m_pCloseLockTimer->isActive())
    {
        m_pCloseLockTimer->stop();
    }

    m_OpenDoorFlag = false;
    g_pScaleSrc->doorbell_lock(false);
    usleep(1000*10);
    if ( g_door_bell_type != 3 )     		  // 如果不是可视带刷卡门铃的类型
        system("echo '0'>/sys/class/gpio/gpio194/value"); //关闭门铃电源

}

/************************************************************
描述：对户门口机发送开锁命令
参数：无
返回：无
************************************************************/
void DoorBellDialog::on_pushButtonDoorBellOpen_pressed()
{
	sendDoorBellCmd(0xf8);

	QSettings settings(CONFIG_FILE, QSettings::IniFormat);
	int intdoortimeout = settings.value("alert_param/door_timeout_time", "0").toInt();
    m_pTimer->start(5000/*+1000*intdoortimeout*/);
    m_OpenDoorFlag = true;
    if(intdoortimeout >5)
        m_pCloseLockTimer->start(1000*intdoortimeout);
	CallInfoUploadThread *pCallInfoUploadThread = g_pMainDlg->m_pVoipMainDlg->m_pCallInfoUploadThread;

	//开门

	//开门脉冲
	m_interrupt = false;
	g_pScaleSrc->doorbell_lock(true);
	/*-----------------------------------------------------------------------------
	 *  根据door_timeout_time参数进行延迟处理
	 *-----------------------------------------------------------------------------*/
	if ( intdoortimeout == 0 )
		usleep(500000);
	else
	{
		for ( int i = 0; i < intdoortimeout*50; i += 1 ) 
		{
			if ( m_interrupt == true )
			{
				goto Exit;
			}
			qApp->processEvents();
			usleep(1000*20);
		}
	}
    m_OpenDoorFlag = false;
	g_pScaleSrc->doorbell_lock(false);

	//david
	if( pCallInfoUploadThread->m_bRun == false)
    {
        if(g_check_computercenter_udp_state == RESULT_SUCCESS)
        {
            pCallInfoUploadThread->m_upInfoType |= RICT_OPEND;;
            pCallInfoUploadThread->getCallCode(QString(g_strNativeName));
        }
        else
        {
            pCallInfoUploadThread->m_upInfoType = 3;
            pCallInfoUploadThread->getCallCode(QString(g_strNativeName));
            pCallInfoUploadThread->start();
        }
	}
	return;
Exit:
	printf ( "interrupt wating\n" );
    //g_pScaleSrc->doorbell_lock(false);		// 关闭锁
}

/************************************************************
描述：门铃响了的消息框显示定时器超时，关闭该消息框
参数：无
返回：无
************************************************************/
void DoorBellDialog::InfoLabelTimerTimeout()
{
	m_pInfoLabelTimer->stop();
	m_pInfoLabel->hide();
}

/************************************************************
描述：查看上一个门铃(1-4)的视频
参数：无
返回：无
************************************************************/
/*void DoorBellDialog::on_pushButtonPrevDoor_pressed()
{
	m_iPeerAddr = ((m_iPeerAddr & 0x0f) + 4 - 1) % 4 + 0xf0;
	ui->lineEditDoorbellIdx->setText(QString::number((m_iPeerAddr & 0x0f) + 1));

	sendDoorBellCmd(0xfa, 0);
	sendDoorBellCmd(0xf7, m_iPeerAddr);
}
*/
/************************************************************
描述：查看下一个门铃(1-4)的视频
参数：无
返回：无
************************************************************/
/*void DoorBellDialog::on_pushButtonNextDoor_pressed()
{
	m_iPeerAddr = ((m_iPeerAddr & 0x0f) + 1) % 4 + 0xf0;
	ui->lineEditDoorbellIdx->setText(QString::number((m_iPeerAddr & 0x0f) + 1));

	sendDoorBellCmd(0xfa, 0);
	sendDoorBellCmd(0xf7, m_iPeerAddr);
}
*/

void DoorBellDialog::DisplayTimerTimeout()
{
	m_pDisplayTimer->stop();

	if (g_pViewCam)
	{
		setDisplayWindow(true);

#ifdef ARM11
        tvout_start();
#endif
	}
}

/************************************************************
 * 测试
************************************************************/
void DoorBellDialog::keyPressEvent(QKeyEvent *k)
{
	int kv = k->key();
	qDebug("DoorBellDialog dialog key value %x",kv);

	switch ( kv )
	{
		case 0x43:
			{
				on_pushButtonDoorBellAnswer_pressed();
			}
			break;

		case 0x2d:
			{
				on_pushButtonDoorBellReturn_pressed();
			}
			break;

		case 0x1000031:
			{
				on_pushButtonDoorBellOpen_pressed();
			}
			break;

		default:	
			break;
	}				/* -----  end switch  ----- */
}
