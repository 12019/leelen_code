#include "application.h"
#include <stdio.h>
#include <stdlib.h>
#include "mediastreamer2/mediastream.h"
#include "maindialog.h"
#include <QKeyEvent>
#include "exosip_main.h"
#include <sys/ioctl.h>
#include "ui_maindialog.h"
#include "data_types.h"

/************************************************************
  该文件为LCD背光、摄像头电源、扬声器、麦克风管理主程序
************************************************************/
extern MainDialog *g_pMainDlg;
//extern QImage *g_pAppBackImage;
extern ViewCam *g_pViewCam;
extern eXosip_element_t *eXosipElement;
extern char eventLoopWatchVariable;

extern int g_iDeviceType;
extern bool g_Digital_Channel_Flag;				// 标识数字通道的变量

int g_gm7113_fd = -1;

bool g_ScreenSave_Flag;      // 屏保功能旁路标志
bool g_IsLCDLightOn;	     // 当前是否处于屏保状态
void setGM7113Channel(int channel)
{
    if(g_gm7113_fd == -1) return;

    int ret;

    ret = ioctl(g_gm7113_fd,channel,0);

    printf("ioctl gm7113 ret:%d\n",ret);
}

/************************************************************
描述：获得校验和代码
参数：
************************************************************/
char get_sum(char *data,int len)
{
	int sum = 0;

	for (int i = 0; i < len; i += 1 ) 
	{
		sum += data[i];
	}

	return ((-sum) & 0xff);
}

/************************************************************
描述：应用的构造函数
参数：argc - 程序命令行参数的个数
	 argv - 程序命令行参数
返回：无
 测试按键说明：
    c: 接听/挂断
    9：监视
    开：开锁
************************************************************/
Application::Application(int &argc, char **argv)
	:  QApplication(argc,argv)
{
#ifdef  ARM11
	gpio_fd = open("/dev/GPIO_Driver", O_RDWR);

	if (gpio_fd == -1)
	{
		printf("GPIO_Driver open failed\n");
	}
#endif
	setOnCamera(false);
	setLCDBacklight(true);

	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()));

	m_bTimerOff = true;

	m_pKeyTimer = new QTimer(this);
#ifdef ARM11
    g_gm7113_fd = open("/dev/GM7113_Driver", O_RDWR);

    if(g_gm7113_fd == -1)
    {
        printf("GM7113_Driver open failed\n");
    }
 #endif
    m_pTouchEnableTimer = new QTimer(this);

    m_pTouchEnableTimer->setSingleShot(true);

    setLCDLumi(255);
    
    g_ScreenSave_Flag = false;

 	m_ptcpserver = new QTcpServer(this);
	m_ptcpserver->listen(QHostAddress::Any,17722);
	connect(m_ptcpserver,SIGNAL(newConnection()),this,SLOT(NewConnect()));
	m_tcpsocket = NULL;
//    m_pudpsocket = new QUdpSocket(this);
//    m_pudpsocket->bind(QHostAddress::LocalHost,9981);
//    connect(m_pudpsocket,SIGNAL(readyRead()),this,SLOT(Read_data_process()));
}

/************************************************************
描述：TCP 服务端的连接
参数：无
返回：无
************************************************************/
void Application::NewConnect()
{
	if ( m_tcpsocket != NULL  )
	{
		if ( m_tcpsocket->isOpen() )
			m_tcpsocket->close();
		delete m_tcpsocket;
		m_tcpsocket = NULL;
	}
	m_tcpsocket = m_ptcpserver->nextPendingConnection();

	connect(m_tcpsocket,SIGNAL(readyRead()),this,SLOT(Read_data_process()));
}

/************************************************************
描述：UDP处理函数
参数：无
返回：无
************************************************************/
void Application::Read_data_process()
{
	QByteArray datagram = m_tcpsocket->readAll();
	
	char *tmp = datagram.data();

	if ( tmp[15] == get_sum(tmp,15) && tmp[5] == 0x00 && tmp[6] == 0xaf )  // 判断命令
	{
		char *data = new char[16];
		data[0] = 0xd1;				// 报文头
		data[1] = 0xd2;
		data[2] = 0x5d;	

		data[3] = 0x00;				// 版本 0
		data[4] = 0x00;

		data[5] = 0xff;				// 命令 
		data[6] = 0xff;

		data[7] = 0;				// 非加密

		data[8] = 0x00;
		data[9] = 0x00;
		data[10]= 0x00;
		data[11]= 0x0d;				// 长度 不包含报文头

		if ( g_IsLCDLightOn )
			data[12] = 0x00;		// 非待机
		else
			data[12] = 0x01;

		data[13] = 0x00;			// 预留
		data[14] = 0x00;

		data[15] = get_sum(data,15);

		int ret = m_tcpsocket->write(data,16);

		if ( ret != 16 )
			printf ( "kevin send data error\n" );

		delete[] data;
		data = NULL;
	}
}

/************************************************************
描述：应用析构函数
参数：无
返回：无
************************************************************/
Application::~Application()
{
//	delete m_pudpsocket;
//	m_pudpsocket = NULL;



	if (gpio_fd != -1)
	{
		close(gpio_fd);
	}

	if (m_pTimer)
	{
		delete m_pTimer;
	}

	if (m_pKeyTimer)
	{
		delete m_pKeyTimer;
	}
}

void Application::setGpioDirection(int iGpioNum, bool bIsOut)
{
	FILE *fp = fopen("/sys/class/gpio/export", "wt");
	char str[80];
	if (fp)
	{
		fprintf(fp, "%d", iGpioNum);
		fclose(fp);
	}
	else
	{
		return;
	}
	sprintf(str, "/sys/class/gpio/gpio%d/direction", iGpioNum);
	fp = fopen(str, "wt");
	if (fp)
	{
		fprintf(fp, "%s", bIsOut ? "out" : "in");
		fclose(fp);
	}
	else
	{
		return;
	}
	fp = fopen("/sys/class/gpio/unexport", "wt");
	if (fp)
	{
		fprintf(fp, "%d", iGpioNum);
		fclose(fp);
	}
}

void Application::setGpioValue(int iGpioNum, bool bHigh)
{
	FILE *fp = fopen("/sys/class/gpio/export", "wt");
	char str[80];
	if (fp)
	{
		fprintf(fp, "%d", iGpioNum);
		fclose(fp);
	}
	else
	{
		return;
	}

	sprintf(str, "/sys/class/gpio/gpio%d/value", iGpioNum);
	fp = fopen(str, "wt");
	if (fp)
	{
		fprintf(fp, "%s", bHigh ? "1" : "0");
		fclose(fp);
	}
	else
	{
		return;
	}

	fp = fopen("/sys/class/gpio/unexport", "wt");
	if (fp)
	{
		fprintf(fp, "%d", iGpioNum);
		fclose(fp);
	}
}

void Application::setGpioValue(char GPIO_char, int GPIO_idx, int GPIO_value)
{
	if (gpio_fd == -1) return;

	unsigned int value;
	value = (GPIO_char << 16) + (GPIO_idx << 8) + GPIO_value;

	ioctl(gpio_fd, 0, value);
}

/************************************************************
描述：打开/关闭麦克风
参数：bOn - 1: 打开麦克风
		   0: 关闭麦克风
返回：无
************************************************************/
void Application::setUnmuteMic(bool bOn)
{
	setGpioValue('H', 6, !bOn);
}

/************************************************************
描述：打开/关闭扬声器
参数：bOn - 1: 打开扬声器
		   0: 关闭扬声器
返回：无
************************************************************/
void Application::setUnmuteSpeaker(bool bOn)
{
	setGpioValue('O', 3, !bOn);
}
void Application::setLCDLumi(int lumi)
{
	FILE *fp;
	fp = fopen("/sys/class/disp/disp/attr/lcd_bl", "wt");
	if (fp)
	{
		fprintf(fp, "%d",lumi);
		fclose(fp);
	}
}
/************************************************************
描述：打开/关闭LCD背光
参数：bOn - 1: 打开LCD背光
		   0: 关闭LCD背光
返回：无
************************************************************/
void Application::setOnLCD(bool bOn)
{
#ifdef CORTEXA7
    FILE *fp = fopen("/sys/power/state","wr");
    if(fp)
    {
        fprintf(fp,"%s",bOn?"on":"mem");
        fclose(fp);
    }
#else

    if(g_iDeviceType == TOUCHZ)
    {
        setGpioValue('E', 4, bOn);
    }
    else
    {
        setGpioValue('F',15,bOn);
    }
#endif

}

/************************************************************
描述：设置GPH7电平
参数：bHigh - 1: GPH7电平设为1
			 0: GPH7电平设为0
返回：无
************************************************************/
void Application::setGPH7(bool bHigh)
{
	//setGpioValue(70, bHigh);
	setGpioValue('H', 7, bHigh);
}

/************************************************************
描述：设置GPH8电平
参数：bHigh - 1: GPH8电平设为1
			 0: GPH8电平设为0
返回：无
************************************************************/
void Application::setGPH8(bool bHigh)
{
	//setGpioValue(71, bHigh);
	setGpioValue('H', 8, bHigh);
}


/************************************************************
描述：设置GPE0电平
参数：bHigh - 1: GPE0电平设为1
			 0: GPE0电平设为0
返回：无
************************************************************/
void Application::setGPE0(bool bHigh)
{
	setGpioValue('E', 0, bHigh);
}

/************************************************************
描述：打开/关闭摄像头电源
参数：bOn - 1: 打开摄像头电源
		   0: 关闭摄像头电源
返回：无
************************************************************/
void Application::setOnCamera(bool bOn)
{
    if(g_iDeviceType == TOUCHZ)
    {
        setGpioValue('E', 3, !bOn);
    }
    else
    {
        setGpioValue('O', 2, !bOn);
    }

}

/************************************************************
描述：应用事件过滤函数，当鼠标、触摸屏、按键按下时打开LCD背光和麦克风，
	 播放按键音，并启动屏保定时器
参数：obj - 发送事件的对象
	 e - 事件
返回：true: 事件处理成功
	 false: 时间处理失败
************************************************************/
bool Application::notify(QObject *obj, QEvent *e)
{
//	return QApplication::notify(obj, e);               // modify by kevin 204.07.22
	bool bIgnore = (!m_bLCDBacklightOn) &&
		((e->type() == QEvent::MouseButtonPress)
		 || (e->type() == QEvent::MouseButtonRelease)
		 || (e->type() == QEvent::MouseMove)
		 || (e->type() == QEvent::KeyPress)
		 ||  (e->type() == QEvent::KeyRelease));

	if ((e->type() == QEvent::MouseButtonPress)
			|| (e->type() == QEvent::MouseButtonRelease)
			|| (e->type() == QEvent::MouseMove)
			|| (e->type() == QEvent::KeyPress)
			|| (e->type() == QEvent::KeyRelease))
	{

		if((e->type() == QEvent::KeyPress) ||
				(e->type() == QEvent::KeyRelease) )
		{
			QKeyEvent *ke = (QKeyEvent *)e;

			// printf("notify key:%d\n",ke->key());  0x1000030
			if(ke->key()==0x1000030) return  true;
		}

		if (!m_bTimerOff)
		{
			setLCDAndTimerOn();
		}

		if( (e->type() == QEvent::MouseButtonPress)
				&& (eXosipElement->state == IDLE))
		{
			int volume = 1024;
			if (g_pMainDlg)
			{
				volume = g_pMainDlg->m_pVoipMainDlg->m_nRingVolume;
			}
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);
			playWaveFile((char *)"wav/keytone.wav", volume, 1);
		}

		if(!bIgnore &&(m_pTouchEnableTimer->isActive()))
		{
			return true;
		}
	}

	if (bIgnore)
	{
		m_pTouchEnableTimer->start(500);
		return true;
	}

	if (e->type() == QEvent::KeyPress)
	{
		QKeyEvent *ke = (QKeyEvent *)e;

		if (!ke->isAutoRepeat())
		{
			printf("ww key pressed:%d\n", ke->key());

			switch ( ke->key() ) 
			{
				case 0x39:  	 // 9   // 监控
					g_pMainDlg->Test_Fun1();
					break;

				default:
					break;
			}				/* -----  end switch  ----- */
		}
		else
		{
			return true;
		}
	}
	else if (e->type() == QEvent::KeyRelease)
	{
//		QKeyEvent *ke = (QKeyEvent *)e;

//		int KeyValue = -1;
//
//		//printf("key release:%d\n", ke->key());
//		if (ke->key() == 45)
//		{
//			m_pKeyTimer->stop();
//		}
//		else if (ke->key() == 51)
//		{
//			KeyValue = 3;
//		}
//		else if (ke->key() == 52)
//		{
//			KeyValue = 4;
//		}
//		else if (ke->key() == 0x1000000)
//		{
//			KeyValue = 2;
//		}
//		else if (ke->key() == 49)
//		{
//			KeyValue = 1;
//		}
//
//		if (KeyValue >= 0)
//		{
//			g_pMainDlg->m_pKeypadReadThread->processKey(1, KeyValue);
//			return true;
//		}
	}
	return QApplication::notify(obj, e);
}

/************************************************************
描述：停止屏保定时器并打开LCD背光和扬声器，使LCD常亮。这个在播放、对讲、安防录
	 像时用到
参数：无
返回：无
************************************************************/
void Application::setLCDOn()
{
	printf("\tstop timer\n");
	m_pTimer->stop();
	m_bTimerOff = true;

	if (!m_bLCDBacklightOn)
	{
		setLCDBacklight(true);
	}
}

/************************************************************
描述： 重新开启屏保
参数：无
返回：无
************************************************************/
void Application::Re_Enable_Screen_Save()
{
	setLCDAndTimerOn();
}

/************************************************************
描述：打开LCD背光和扬声器并开启屏保定时器，屏保定时器启动到超时过程中如果没有
	 触摸/按键则关闭LCD背光和扬声器
参数：无
返回：无
************************************************************/
void Application::setLCDAndTimerOn()
{
	if (!m_bLCDBacklightOn)
	{
		setLCDBacklight(true);
	}

	if ( m_pTimer->isActive() )
		m_pTimer->stop();
	m_pTimer->start(SCREEN_SAVER_TIME);
	m_bTimerOff = false;
}

/************************************************************
描述：设置LCD背光和扬声器，如果是关闭LCD背光则LCD界面退回到主窗口后关
	 闭LCD背光和扬声器
参数：bLCDBacklightOn - 1: 打开LCD背光和扬声器
					   0: 关闭LCD背光和扬声器
返回：无
************************************************************/
void Application::setLCDBacklight(bool bLCDBacklightOn)
{
	//解决进入屏保同时，返回到主界面的bug
	//david
	
	if ( g_ScreenSave_Flag ) return;			// 屏幕保护旁路

	if (!bLCDBacklightOn)
	{
		printf("david screesaver\n");

		closeMessBox(g_pMainDlg);
		if ( g_pMainDlg->m_pSysSetMainDlg->m_pRingAndVolumeDlg->m_pDoorBellRingFileSetDlg->isVisible() )
			g_pMainDlg->m_pSysSetMainDlg->m_pRingAndVolumeDlg->m_pDoorBellRingFileSetDlg->Remote_Call_Return();

		if (g_pMainDlg->m_pDoorBellDlg->isVisible())
		{
			g_pMainDlg->m_pDoorBellDlg->on_pushButtonDoorBellReturn_pressed();
		}

		if (g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->isVisible()
                && ((eventLoopWatchVariable == 0) || g_pViewCam || (eXosipElement && (eXosipElement->state == MCT_MONITOR || eXosipElement->state == IDLE ))))
		{
			g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->on_pushButtonMonReturn_pressed();
		}
		g_pMainDlg->m_pInputMethodWidget->hide();
		QObjectList q = g_pMainDlg->children();
		for (int i = q.length() - 1; i >= 0; i-- )
		{
			QObject* o = q.at(i);

			if (o->objectName() == "MyMessageBox")
			{
				continue;
			}

			if(o->inherits(("QDialog")))
			{
				QDialog* d = qobject_cast<QDialog*>(o);
				if (d->isVisible())
				{
					d->hide();
				}
			}
		}
		g_pMainDlg->m_nDispFirstFMIdx = 0;
		g_pMainDlg->ReDisplayButtons();
		g_pMainDlg->ui->pushButtonVoip->setFocus(Qt::TabFocusReason);
        g_pMainDlg->m_pHomeControlDlg->setMainFace();
	}
	else
	{
		/*if (g_pMainDlg && g_pMainDlg->m_pScreenSaverDlg->isVisible())
		  {
		  g_pMainDlg->m_pScreenSaverDlg->hide();
		  g_pMainDlg->repaint();
		  }*/
	}
	//david

	setOnLCD(bLCDBacklightOn); //GPE4

	setUnmuteMic(false); //GPH6
	setUnmuteSpeaker(bLCDBacklightOn); //GPO3
	setGPH7(!bLCDBacklightOn);
	setGPH8(!bLCDBacklightOn);
	setGPE0(true);

	/*char cmd[100];
	system("/bin/echo 36 > /sys/class/gpio/export");
	//system("/bin/echo out > /sys/class/gpio/gpio36/direction");
	sprintf(cmd, "/bin/echo %d > /sys/class/gpio/gpio36/value", bLCDBacklightOn == true);
	system(cmd);
	system("/bin/echo 36 > /sys/class/gpio/unexport");

	system("/bin/echo 70 > /sys/class/gpio/export");
	sprintf(cmd, "/bin/echo %d > /sys/class/gpio/gpio70/value", !(bLCDBacklightOn == true));
	system(cmd);
	system("/bin/echo 70 > /sys/class/gpio/unexport");
	*/
	/*if (!bLCDBacklightOn)
	{
		system("/bin/echo 69 > /sys/class/gpio/export");
		system("/bin/echo 1 > /sys/class/gpio/gpio69/value");
		system("/bin/echo 69 > /sys/class/gpio/unexport");
	}*/

	m_bLCDBacklightOn = bLCDBacklightOn;

	g_IsLCDLightOn = bLCDBacklightOn;
}

/************************************************************
描述：屏保定时器超时回调函数，关闭LCD背光和扬声器
参数：无
返回：无
************************************************************/
void Application::timerTimeout()
{
	setLCDBacklight(false);

	m_pTimer->stop();

	printf("screen saver timeout\n");
}
//david 2014.3.31
int psystem(const char *cmd)
{
    FILE *fp;

    fp = popen(cmd,"r");

    if(NULL == fp)
    {
        printf("popen failed value\n");
        return -1;
    }

    pclose(fp);

    sync();				// 保证有进行文件操作的进行磁盘同步

    return 0;
}
