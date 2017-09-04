#include "securityalertdialog.h"
#include "ui_securityalertdialog.h"
#include "mediastreamer2/mediastream.h"
#include "securitymaindialog.h"
#include "securitypassworddialog.h"
//#include "callcenterdialog.h"
//#include "helpdialog.h"
//#include "terminalhelpmaindialog.h"
#include "maindialog.h"
#include "application.h"
#include "adddefensedialog.h"
#include "securitymaindialog.h"
#include "securityalertdialog.h"
#include "securityabnormaldialog.h"
#include <QApplication>

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
extern bool g_Digital_Channel_Flag;				// 标识数字通道的变量
extern int g_door_bell_type;                 // 二次门铃的类型的全局变量
/************************************************************
  该文件为安防报警录像对话框主程序
************************************************************/

/************************************************************
  视频显示区域的背景色
************************************************************/
//#define PLAY_AREA_BGCOLOR QColor(0, 0, 0)

extern WriteAVI *g_pWriteAVI;
extern MainDialog *g_pMainDlg;
extern int g_bIstOrAlertBusy;

/************************************************************
描述：安防报警录像对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
SecurityAlertDialog::SecurityAlertDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecurityAlertDialog)
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

    	g_pScaleSrc->get_videoshow_geometry(x,y,w,h);//get_tabviewinfolist_geometry(x,y,w,h);
	ui->frameVideoShow->setGeometry(x,y,w,h);

	g_pScaleSrc->get_info_label_geometry(x,y,w,h);
	ui->labelInfo->setGeometry(x,y,w,h);

    int res = g_pScaleSrc->get_screen_version();
    if ( res == 3 )  // 1280x800
    {
        ui->pushButtonSecAlertOut->setGeometry(QRect(959, 353, 279, 99));
        ui->pushButtonSecAlertHome->setGeometry(QRect(959, 461, 279, 99));
        ui->pushButtonSecAlertDisArm->setGeometry(QRect(959, 569, 279, 99));
        ui->pushButtonSecAlertReturn->setGeometry(QRect(959, 677, 279, 99));
    }
    else if (res == 2) // 1024x600
    {
        ui->pushButtonSecAlertOut->setGeometry(QRect(764, 260, 230, 80));
        ui->pushButtonSecAlertHome->setGeometry(QRect(764, 340, 230, 80));
        ui->pushButtonSecAlertDisArm->setGeometry(QRect(764, 420, 230, 80));
        ui->pushButtonSecAlertReturn->setGeometry(QRect(764, 500, 230, 80));
    }

	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()));

	m_pFrameFullshow = new QFrame(this);
	m_pFrameFullshow->setObjectName("frameFullshow");
	m_pFrameFullshow->setGeometry(QRect(0, 0, qApp->desktop()->size().width(),qApp->desktop()->size().height()));
	m_pFrameFullshow->setFrameShadow(QFrame::Plain);
	m_pFrameFullshow->setFrameShape(QFrame::Panel);

	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	m_pFrameFullshow->installEventFilter(this);
	ui->frameVideoShow->installEventFilter(this);

	g_pWriteAVI = NULL;
	m_channel = 0;


	m_timerSec = -1;

	//m_pSubDlg = NULL;
	m_bRecorderStopping = false;

	m_property = 0;
    doorbell_power_on_flag=false;
}

/************************************************************
描述：安防报警录像对话框析构函数
参数：无
返回：无
************************************************************/
SecurityAlertDialog::~SecurityAlertDialog()
{
	stopRecorder();

	delete m_pTimer;

	delete m_pFrameFullshow;
	delete ui;
}

/************************************************************
描述：刷新显示30秒的定时器剩余秒数，30秒后如果没有按返回按钮则自动结束
	 录像并关闭本窗口
参数：无
返回：无
************************************************************/
void SecurityAlertDialog::timerTimeout()
{
	SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();

	//printf("SecurityAlertDialog::timerTimeout:%d\n", m_timerSec);
	m_timerSec--;

	//ui->labelTimeSec->setText(tr("剩余") + QString::number(m_timerSec) + tr("秒"));

	if (m_timerSec == 0)
	{
		m_pTimer->stop();

		stopRecorder();

		if ((!pSecurityMainDlg->m_pSecurityPasswordDlg->isVisible())
			|| (pSecurityMainDlg->m_pSecurityPasswordDlg->m_flag != 1))
		{            
            hide();
            SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();
			if (!pSecurityMainDlg->m_bSecurityMainDlgAloneOpened)
			{
				pSecurityMainDlg->hide();
			}

		}
	}
}

/************************************************************
描述：设置报警录像参数，包括报警时间、防区类型、录像AVI存储路径等
参数：strDateTime - 报警时间
	 number - 防区号码
	 property - 防区属性
	 channel - 防区摄像头通道号(0-3)
	 name - 防区名称
	 type - 防区类型(有线/无线)
	 strLocation - 防区安装位置
	 strAviPath - 录像AVI存储路径
返回：无
************************************************************/
void SecurityAlertDialog::setDetail(QString strDateTime, int number, int property, int channel, int name,
									 int type, QString strLocation, QString strAviPath)
 {
	Q_UNUSED(strDateTime);
	//Q_UNUSED(number);
	Q_UNUSED(name);
	Q_UNUSED(type);
	Q_UNUSED(strLocation);

	m_strAviPath = strAviPath;
	m_channel = channel;//strChannel.right(1).toInt() - 1;
    m_property = property;

    if(m_channel==0)
    {
        if(doorbell_power_on_flag==false&&g_door_bell_type==2)
        {
            g_pScaleSrc->doorbell_power(true);
            doorbell_power_on_flag = true;
        }
    }

	ui->labelInfo->setText(tr("报警录像") + " - " + QString::number(number) + tr("号")
						   + ((type == DEFENSE_TYPE_WIRE) ? QObject::tr("有线防区") : QObject::tr("无线防区")));

	//ui->pushButtonSecAlertOut->setEnabled(true);
	//ui->pushButtonSecAlertHome->setEnabled(true);
	//ui->pushButtonSecAlertDisArm->setEnabled(true);

	SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();
	switch (pSecurityMainDlg->m_securityState)
	{
	case STATUS_GOOUT:
        ui->pushButtonSecAlertOut->setFocus(Qt::TabFocusReason);
        //ui->pushButtonSecAlertHome->setFocus(Qt::TabFocusReason);
		break;
	case STATUS_ATHOME:
        ui->pushButtonSecAlertHome->setFocus(Qt::TabFocusReason);
        //ui->pushButtonSecAlertOut->setFocus(Qt::TabFocusReason);
		break;
	case STATUS_DISARM:
        ui->pushButtonSecAlertDisArm->setFocus(Qt::TabFocusReason);
        //ui->pushButtonSecAlertOut->setFocus(Qt::TabFocusReason);
		break;
	}

	////ui->pushButtonSecAlertReturn->setFocus(Qt::TabFocusReason);

	/*QPalette palette;
	palette.setBrush(ui->frameVideoShow->backgroundRole(), QBrush(PLAY_AREA_BGCOLOR));
	ui->frameVideoShow->setPalette(palette);
	ui->frameVideoShow->setAutoFillBackground(true);
	*/
}

/************************************************************
描述：打开摄像头电源和麦克风，启动报警AVI录像
参数：无
返回：无
************************************************************/
void SecurityAlertDialog::startRecorder()
{
	////////////////////////////////
	//MSSndCard *sc;
	//const char *card_id = NULL;
	MSWebCam *cam;

	stopRecorder(false);

	((Application *)qApp)->setUnmuteMic(true);
	((Application *)qApp)->setOnCamera(true);

	//sc = ms_snd_card_manager_get_card(ms_snd_card_manager_get(), card_id);
	cam = ms_web_cam_manager_get_default_cam(ms_web_cam_manager_get());

	char aviPath[PATH_MAX];
	strcpy(aviPath, m_strAviPath.toLatin1().data());

	while (g_pWriteAVI == (WriteAVI *)-1)
	{
		usleep(10000);
	}

	g_pWriteAVI = (WriteAVI *)-1;
#ifdef ARM11
    g_pWriteAVI = writeAVI_start(cam, /*sc,*/ aviPath, m_channel, 100);
#endif

#ifdef CORTEXA7
    //g_pWriteAVI = writeAVI_start(aviPath,m_channel,720,576,25.0,30);
     g_pWriteAVI = writeAVI_start(aviPath, m_channel, 720 - TVD_LEFT_SPAN - TVD_RIGHT_SPAN, 576 - TVD_TOP_SPAN - TVD_BOTTOM_SPAN, 25.0, 30);
#endif

	my_ring_start((char *)"wav/alert.wav", 2000);
	my_ring_set_level(g_pMainDlg->m_pVoipMainDlg->m_nRingVolume);

	setDisplayWindow(true);

	m_timerSec = 30;

	m_pTimer->start(1000);

}

/************************************************************
描述：关闭摄像头电源和麦克风，停止报警AVI录像
参数：bReleaseJpegSendThread - 1:结束JPEG图像向管理机发送线程
							  0:不结束JPEG图像向管理机发送线程
返回：无
************************************************************/
void SecurityAlertDialog::stopRecorder(int bReleaseJpegSendThread)
{
	while (m_bRecorderStopping)
	{
		usleep(2000);
	}

	m_bRecorderStopping = true;

	m_pTimer->stop();
	m_timerSec = 0;

	if (bReleaseJpegSendThread)
	{
		Serial1Thread *pSerial1Thread = ((SecurityMainDialog *)parent())->m_pSerial1Thread;
		if (pSerial1Thread && pSerial1Thread->m_pJpegSendThread)
		{
			//int loop_cnt = 0;
			while (pSerial1Thread->m_pJpegSendThread->m_bBusy)
			{
				usleep(10000);
			}

			if (pSerial1Thread->m_pJpegSendThread->m_bRun)
			{
				pSerial1Thread->m_pJpegSendThread->m_bRun = false;
				pSerial1Thread->m_pJpegSendThread->quit();
			}

			pSerial1Thread->m_pJpegSendThread->wait();

			int i;
			for (i = 0; i < 2; i++)
			{
				if (pSerial1Thread->m_pJpegSendThread->m_pTcpSock[i])
				{
					pSerial1Thread->m_pJpegSendThread->m_pTcpSock[i]->close();
					delete pSerial1Thread->m_pJpegSendThread->m_pTcpSock[i];
					pSerial1Thread->m_pJpegSendThread->m_pTcpSock[i] = NULL;
				}
			}
		}
	}

	setDisplayWindow(false);

	my_ring_stop();

    //printf("SecurityAlertDialog::stopRecorder 3:%p\n", g_pWriteAVI);
	while (g_pWriteAVI == (WriteAVI *)-1)
	{
		usleep(10000);
	}

	if (g_pWriteAVI)
	{
		WriteAVI *tmp_pWriteAVI = g_pWriteAVI;
		g_pWriteAVI = (WriteAVI *)-1;
		writeAVI_stop(tmp_pWriteAVI);
		g_pWriteAVI = NULL;
	}

	if (m_pFrameFullshow->isVisible())
	{
		m_pFrameFullshow->hide();
		m_pFrameFullshow->lower();
	}
	else
	{
		//ui->frameVideoShow->repaint(); david 2014.7.4
		ui->frameVideoShow->update();
	}

	((Application *)qApp)->setUnmuteMic(false);
	((Application *)qApp)->setOnCamera(false);

	m_bRecorderStopping = false;

	sync();
}

/************************************************************
描述：设置安防录像视频回放窗口坐标，坐标值为0则停止视频显示
参数：bShow - true: 设置视频的显示窗口坐标并开启视频显示
			 false: 停止视频显示
返回：0 - 成功
	 -1 - 失败
************************************************************/
int SecurityAlertDialog::setDisplayWindow(bool bShow)
{
	int vrect[4];
	QRect rect;

	while (g_pWriteAVI == (WriteAVI *)-1)
	{
		usleep(10000);
	}

	if (!g_pWriteAVI) return -1;

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

	ms_filter_call_method(g_pWriteAVI->videoout, MS_FILTER_SET_VIDEO_RECT, vrect);

	return 0;
}

/************************************************************
描述：窗口事件过滤函数，实现播放视频窗口全屏/非全屏切换
参数：o - 产生事件的控件
	 e - 需处理的事件
返回：true - 事件处理成功，不需进一步处理
	 false - 需要进一步处理
************************************************************/
bool SecurityAlertDialog::eventFilter(QObject *o, QEvent *e)
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

/************************************************************
描述：停止报警录像并关闭本窗口
参数：无
返回：无
************************************************************/
void SecurityAlertDialog::on_pushButtonSecAlertReturn_pressed()
{
	if (!isVisible()) return;

	m_pTimer->stop();

    //printf("SecurityAlertDialog::on_pushButtonSecAlertReturn_clicked\n");
	stopRecorder();
    //printf("SecurityAlertDialog::on_pushButtonSecAlertReturn_clicked 2\n");

    hide();
	SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();
    if (!pSecurityMainDlg->m_bSecurityMainDlgAloneOpened)
	{
		pSecurityMainDlg->hide();
	}

    if(doorbell_power_on_flag==true&&g_door_bell_type==2)
    {
        g_pScaleSrc->doorbell_power(false);
        doorbell_power_on_flag = false;
    }


	//bug
	/*if (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg)
	{
		g_pMainDlg->repaint();
	}*/
	//bug
}

/************************************************************
描述：切换设防状态到外出设防状态，如果有异常防区则打开设防状态异常防区
	 忽略撤销窗口
参数：无
返回：无
************************************************************/
void SecurityAlertDialog::on_pushButtonSecAlertOut_pressed()
{
	//if (m_pSubDlg) return;
	SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();

	if (pSecurityMainDlg->m_securityState == STATUS_GOOUT) return;

	if (pSecurityMainDlg->m_pSecurityPasswordDlg->isVisible()
		&& (pSecurityMainDlg->m_pSecurityPasswordDlg->m_flag == 1))
	{
		return;
	}

	if (g_pMainDlg->m_inputMethodMode == 0)
	{
		if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
			usleep(50000);;
		writeAVI_playWaveFile((char *)"wav/goout-en.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1, 0);
	}
	else
	{
		if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
			usleep(50000);;
		writeAVI_playWaveFile((char *)"wav/goout.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1, 0);
	}

	pSecurityMainDlg->setSecurityState(STATUS_GOOUT);
    pSecurityMainDlg->emitsigSetSecurityStatueToPhone(STATUS_GOOUT);
    if(pSecurityMainDlg->m_pUploadSetSecurityStateThread->m_bRun)
    {
        pSecurityMainDlg->m_pUploadSetSecurityStateThread->m_bRun = false;
        pSecurityMainDlg->m_pUploadSetSecurityStateThread->quit();
        pSecurityMainDlg->m_pUploadSetSecurityStateThread->wait();
    }
    pSecurityMainDlg->m_pUploadSetSecurityStateThread->start();

	//ui->pushButtonSecAlertOut->setEnabled(false);
	//ui->pushButtonSecAlertHome->setEnabled(true);
	//ui->pushButtonSecAlertDisArm->setEnabled(true);
	ui->pushButtonSecAlertOut->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：切换设防状态到居家设防状态，若原状态为外出设防状态则需要输入密码，
	 如果有异常防区则打开设防状态异常防区忽略撤销窗口
参数：无
返回：无
************************************************************/
void SecurityAlertDialog::on_pushButtonSecAlertHome_pressed()
{
	//if (m_pSubDlg) return;
	SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();

	if (pSecurityMainDlg->m_securityState == STATUS_ATHOME) return;

	if (pSecurityMainDlg->m_pSecurityPasswordDlg->isVisible()
		&& (pSecurityMainDlg->m_pSecurityPasswordDlg->m_flag == 1))
	{
		return;
	}

	if (pSecurityMainDlg->m_securityState == STATUS_GOOUT)
	{
		int ret;

		setDisplayWindow(false);
		usleep(120000);

		//m_pSubDlg = (QDialog*) new SecurityPasswordDialog(this);
		pSecurityMainDlg->m_pSecurityPasswordDlg->resetState(STATUS_ATHOME, 1);
		pSecurityMainDlg->m_pSecurityPasswordDlg->raise();
		ret = pSecurityMainDlg->m_pSecurityPasswordDlg->exec();

		if ((m_timerSec == 0) || ((ret == RET_PASSWORDOK) && (m_property == PROPERTY_ATHOME)))
		{
			stopRecorder();

            hide();
			if (!pSecurityMainDlg->m_bSecurityMainDlgAloneOpened)
			{
				pSecurityMainDlg->hide();
			}
		}
		else
		{
			repaint();
			setDisplayWindow(true);
		}

		if (ret != RET_PASSWORDOK)
		{
			ui->pushButtonSecAlertHome->setFocus(Qt::TabFocusReason);
			return;
		}
	}

	if (m_timerSec > 0)
	{
		if (g_pMainDlg->m_inputMethodMode == 0)
		{
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
			writeAVI_playWaveFile((char *)"wav/athome-en.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1, 0);
		}
		else
		{
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
			writeAVI_playWaveFile((char *)"wav/athome.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1, 0);
		}
	}
	else
	{
		if (g_pMainDlg->m_inputMethodMode == 0)
		{
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
			playWaveFile((char *)"wav/athome-en.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
		}
		else
		{
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
			playWaveFile((char *)"wav/athome.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
		}
	}

	pSecurityMainDlg->setSecurityState(STATUS_ATHOME);
    pSecurityMainDlg->emitsigSetSecurityStatueToPhone(STATUS_ATHOME);
    if(pSecurityMainDlg->m_pUploadSetSecurityStateThread->m_bRun)
    {
        pSecurityMainDlg->m_pUploadSetSecurityStateThread->m_bRun = false;
        pSecurityMainDlg->m_pUploadSetSecurityStateThread->quit();
        pSecurityMainDlg->m_pUploadSetSecurityStateThread->wait();
    }
    pSecurityMainDlg->m_pUploadSetSecurityStateThread->start();
	//ui->pushButtonSecAlertOut->setEnabled(true);
	//ui->pushButtonSecAlertHome->setEnabled(false);
	//ui->pushButtonSecAlertDisArm->setEnabled(true);
	ui->pushButtonSecAlertHome->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：切换设防状态到撤防状态，需要输入密码，如果有异常防区则打开设防状
	 态异常防区忽略撤销窗口
参数：无
返回：无
************************************************************/
void SecurityAlertDialog::on_pushButtonSecAlertDisArm_pressed()
{
	//if (m_pSubDlg) return;
	SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();

	if (pSecurityMainDlg->m_securityState == STATUS_DISARM) return;

	if (pSecurityMainDlg->m_pSecurityPasswordDlg->isVisible()
		&& (pSecurityMainDlg->m_pSecurityPasswordDlg->m_flag == 1))
	{
		return;
	}

	int ret;

	setDisplayWindow(false);
	usleep(120000);

	pSecurityMainDlg->m_pSecurityPasswordDlg->resetState(STATUS_DISARM, 1);
	pSecurityMainDlg->m_pSecurityPasswordDlg->raise();
	ret = pSecurityMainDlg->m_pSecurityPasswordDlg->exec();

	if ((m_timerSec == 0) || ((ret == RET_PASSWORDOK) && (m_property != PROPERTY_NORMAL)))
	{
		stopRecorder();

        hide();
        if (!pSecurityMainDlg->m_bSecurityMainDlgAloneOpened)
		{
			pSecurityMainDlg->hide();
		}
	}
	else
	{
		repaint();
		setDisplayWindow(true);
	}

	if (ret != RET_PASSWORDOK)
	{
		ui->pushButtonSecAlertDisArm->setFocus(Qt::TabFocusReason);
		return;
	}

	if (m_timerSec > 0)
	{
		if (g_pMainDlg->m_inputMethodMode == 0)
		{
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
			writeAVI_playWaveFile((char *)"wav/disarm-en.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1, 0);
		}
		else
		{
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
			writeAVI_playWaveFile((char *)"wav/disarm.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1, 0);
		}
	}
	else
	{
		if (g_pMainDlg->m_inputMethodMode == 0)
		{
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
			playWaveFile((char *)"wav/disarm-en.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
		}
		else
		{
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
			playWaveFile((char *)"wav/disarm.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
		}
	}

	pSecurityMainDlg->setSecurityState(STATUS_DISARM);
    pSecurityMainDlg->emitsigSetSecurityStatueToPhone(STATUS_DISARM);
    if(pSecurityMainDlg->m_pUploadSetSecurityStateThread->m_bRun)
    {
        pSecurityMainDlg->m_pUploadSetSecurityStateThread->m_bRun = false;
        pSecurityMainDlg->m_pUploadSetSecurityStateThread->quit();
        pSecurityMainDlg->m_pUploadSetSecurityStateThread->wait();
    }
    pSecurityMainDlg->m_pUploadSetSecurityStateThread->start();
	//ui->pushButtonSecAlertOut->setEnabled(true);
	//ui->pushButtonSecAlertHome->setEnabled(true);
	//ui->pushButtonSecAlertDisArm->setEnabled(false);
	ui->pushButtonSecAlertDisArm->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void SecurityAlertDialog::showEvent(QShowEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());
	pMainDlg->showRefreshTabFocus(this);

	((Application *)qApp)->setLCDOn();
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void SecurityAlertDialog::hideEvent(QHideEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());
	pMainDlg->hideRefreshTabFocus(this);

	((Application *)qApp)->setLCDAndTimerOn();

	stopRecorder();//david 2014.4.22 play alert.wav twice
}

/*void SecurityAlertDialog::setStateText()
{
     ui->labelState->setText(tr("外出设防"));
}*/
