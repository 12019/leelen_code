#include "securityplayerdialog.h"
#include "ui_securityplayerdialog.h"
#include "mediastreamer2/mediastream.h"
#include "maindialog.h"
#include "mediastreamer2/AVI_read.h"
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/mman.h>
//#include "terminalhelpmaindialog.h"
//#include "helpdialog.h"
#include "application.h"
#include "ui_securitymaindialog.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
/************************************************************
  该文件为报警录像播放对话框主程序
************************************************************/

/************************************************************
  视频显示区域的背景色
************************************************************/
//#define PLAY_AREA_BGCOLOR QColor(0, 0, 0)
extern ReadAVI *g_pReadAVI;
static SecurityPlayerDialog *security_player_dlg = NULL;

/************************************************************
描述：播放的回调函数，当接收到播放结束信号时关闭播放
参数：id - 底层传给回调函数的ID参数
返回：无
************************************************************/
static void notifyfunc(void *userdata , unsigned int id, void *arg)
{
	Q_UNUSED(userdata);
	//Q_UNUSED(id);
	Q_UNUSED(arg);

	if (id == MS_AVI_PLAYER_EOF)
	{
		if (security_player_dlg)
		{
			security_player_dlg->emitSigStopPlayer();
		}
	}
}

/************************************************************
描述：报警录像播放对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
SecurityPlayerDialog::SecurityPlayerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecurityPlayerDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

	security_player_dlg = this;
	//ui_security_player = ui;
	////ui->frameVideoShow->setObjectName("frameVideoShowPlayer");

	int x,y,w,h;
	g_pScaleSrc->get_info_framepanel_geometry(x,y,w,h);
	ui->framePanel->setGeometry(x,y,w,h);

	g_pScaleSrc->get_videoshow_geometry(x,y,w,h);//get_tabviewinfolist_geometry(x,y,w,h);
	ui->frameVideoShow->setGeometry(x,y,w,h);

	g_pScaleSrc->get_info_label_geometry(x,y,w,h);
	ui->labelInfo->setGeometry(x,y,w,h);

    int res = g_pScaleSrc->get_screen_version();
    if 	( res == 2 )  // 1024x600
    {
        ui->pushButtonSecPlayerPlay->setGeometry(QRect(764, 340, 230, 80));
        ui->pushButtonSecPlayerPause->setGeometry(QRect(764, 340, 230, 80));
        ui->pushButtonSecPlayerStop->setGeometry(QRect(764, 420, 230, 80));
        ui->pushButtonSecPlayerReturn->setGeometry(QRect(764, 500, 230, 80));
    }
    else if ( res == 3 )  // 1280x800
    {
        ui->pushButtonSecPlayerPlay->setGeometry(QRect(959, 461, 279, 99));
        ui->pushButtonSecPlayerPause->setGeometry(QRect(959, 461, 279, 99));
        ui->pushButtonSecPlayerStop->setGeometry(QRect(959, 569, 279, 99));
        ui->pushButtonSecPlayerReturn->setGeometry(QRect(959, 677, 279, 99));
    }

	g_pReadAVI = NULL;

	ui->pushButtonSecPlayerPlay->show();
	ui->pushButtonSecPlayerPause->hide();

	m_pFrameFullshow = new QFrame(this);
	m_pFrameFullshow->setObjectName("frameFullshow");
	m_pFrameFullshow->setGeometry(QRect(0, 0, qApp->desktop()->size().width(),qApp->desktop()->size().height()));
	m_pFrameFullshow->setFrameShadow(QFrame::Plain);
	m_pFrameFullshow->setFrameShape(QFrame::Panel);

	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	m_pFrameFullshow->installEventFilter(this);
	ui->frameVideoShow->installEventFilter(this);

	m_pPlayEndTimer = new QTimer(this);

	connect(this, SIGNAL(sigStopPlayer()), this, SLOT(slotStopPlayer()));
	connect(m_pPlayEndTimer, SIGNAL(timeout()), this, SLOT(stopPlayer()));
}

/************************************************************
描述：报警录像播放对话框析构函数
参数：无
返回：无
************************************************************/
SecurityPlayerDialog::~SecurityPlayerDialog()
{
	stopPlayer();

	delete m_pFrameFullshow;
	delete m_pPlayEndTimer;

	delete ui;
}

/************************************************************
描述：设置报警录像播放器窗口的参数，包括AVI文件路径、报警时间等
参数：strAviPath - 报警AVI录像文件路径
	 strInfo - 报警信息
返回：无
************************************************************/
void SecurityPlayerDialog::setDetail(QString strAviPath, QString strInfo)
{
	m_VideoPath = strAviPath;

	ui->labelInfo->setText(tr("报警录像播放") + " - " + strInfo);

	//g_pReadAVI = NULL;

	////QPalette palette;
	////palette.setBrush(ui->frameVideoShow->backgroundRole(), QBrush(PLAY_AREA_BGCOLOR));
	////ui->frameVideoShow->setPalette(palette);
	////ui->frameVideoShow->setAutoFillBackground(true);

	//ui->frame->setFocus(Qt::TabFocusReason);
	ui->pushButtonSecPlayerPlay->show();
	ui->pushButtonSecPlayerPause->hide();
	ui->pushButtonSecPlayerPlay->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：开始报警录像播放
参数：无
返回：无
************************************************************/
void SecurityPlayerDialog::startPlayer()
{
	MSSndCard *sc;
	const char * card_id = NULL;
	char pathname[256];

	if (m_VideoPath == "") return;
	/*QFile avi_file(m_VideoPath);
	if (avi_file.open(QIODevice::ReadOnly))
	{
		avi_file.close();
	}
	else
	{
		setAllButtonsEnabled(this, false);
		MyMessageBox messBox(this);
		messBox.setObjectName("mymsgbox");
		messBox.setWindowFlags(Qt::FramelessWindowHint) ;
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		//Q_UNUSED(okButton);
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("\n没有录制的AVI文件！\n"));
		messBox.exec();
		setAllButtonsEnabled(this, true);
        ui->pushButtonSecPlayerPlay->setFocus(Qt::TabFocusReason);

		return;
	}*/

	strcpy(pathname, m_VideoPath.toUtf8().data());
	sc = ms_snd_card_manager_get_card(ms_snd_card_manager_get(), card_id);

	while (g_pReadAVI == (ReadAVI *)-1)
	{
		usleep(10000);
	}
	g_pReadAVI = (ReadAVI *)-1;
	g_pReadAVI = avi_start(pathname, sc, notifyfunc);

	if (!g_pReadAVI)
	{
		//extern int AVI_errno;
		setAllButtonsEnabled(this, false);
		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		//Q_UNUSED(okButton);
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("没有录制的AVI文件"));
		messBox.exec();
		setAllButtonsEnabled(this, true);
		ui->pushButtonSecPlayerPlay->setFocus(Qt::TabFocusReason);

		return;
	}

	//((Application *)qApp)->setLCDOn();

	MainDialog *pMDlg = (MainDialog *)(parent()->parent());
	avi_set_audio_level(g_pReadAVI, pMDlg->m_pVoipMainDlg->m_nVolume);

	setDisplayWindow(true);

	//ui->frame->setFocus(Qt::TabFocusReason);
	ui->pushButtonSecPlayerPlay->hide();
	ui->pushButtonSecPlayerPause->show();
	ui->pushButtonSecPlayerPause->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：停止报警录像播放
参数：无
返回：无
************************************************************/
void SecurityPlayerDialog::stopPlayer(bool bResetFocus)
{
	m_pPlayEndTimer->stop();

	setDisplayWindow(false);

	while (g_pReadAVI == (ReadAVI *)-1)
	{
		usleep(10000);
	}
	if (g_pReadAVI)
	{
		ReadAVI *tmp_pReadAVI = g_pReadAVI;
		g_pReadAVI = (ReadAVI *)-1;
		avi_stop(tmp_pReadAVI);
		g_pReadAVI = NULL;
	}

	ui->pushButtonSecPlayerPlay->show();
	ui->pushButtonSecPlayerPause->hide();
	if (bResetFocus)
	{
		ui->pushButtonSecPlayerPlay->setFocus(Qt::TabFocusReason);
	}

	if (m_pFrameFullshow->isVisible())
	{
		m_pFrameFullshow->hide();
		m_pFrameFullshow->lower();
	}
	else
	{
		ui->frameVideoShow->repaint();
	}

	((Application *)qApp)->setLCDAndTimerOn();
}

/************************************************************
描述：播放/继续播放AVI录像
参数：无
返回：无
************************************************************/
void SecurityPlayerDialog::on_pushButtonSecPlayerPlay_pressed()
{
	while (g_pReadAVI == (ReadAVI *)-1)
	{
		usleep(10000);
	}
	if (g_pReadAVI)
	{
		int bIsClosed;

		ms_filter_call_method(g_pReadAVI->readavi, MS_FILE_readavi_DONE, &bIsClosed);

		//printf("bcom bIsStopped:%d", bIsClosed);

		if (bIsClosed)
		{
            while (g_pReadAVI == (ReadAVI *)-1)
			{
				usleep(5000);
            }
			if (g_pReadAVI)
			{
				ReadAVI *tmp_pReadAVI = g_pReadAVI;
				g_pReadAVI = (ReadAVI *)-1;
				avi_stop(tmp_pReadAVI);
				g_pReadAVI = NULL;
			}
		}
		else
		{
			ms_filter_call_method_noarg(g_pReadAVI->readavi, MS_FILE_readavi_START);

			//ui->frame->setFocus(Qt::TabFocusReason);
			ui->pushButtonSecPlayerPlay->hide();
			ui->pushButtonSecPlayerPause->show();
			ui->pushButtonSecPlayerPause->setFocus(Qt::TabFocusReason);

			return;
		}
	}

	startPlayer();
}

/************************************************************
描述：暂停播放AVI录像
参数：无
返回：无
************************************************************/
void SecurityPlayerDialog::on_pushButtonSecPlayerPause_pressed()
{
	while (g_pReadAVI == (ReadAVI *)-1)
	{
		usleep(10000);
	}

	if (g_pReadAVI)
	{
		int bIsClosed;

		ms_filter_call_method(g_pReadAVI->readavi, MS_FILE_readavi_DONE, &bIsClosed);

		if (bIsClosed)
		{
			return;
		}
	}
	else
	{
		return;
	}

	ms_filter_call_method_noarg(g_pReadAVI->readavi, MS_FILE_readavi_STOP);

	//ui->frame->setFocus(Qt::TabFocusReason);
	ui->pushButtonSecPlayerPause->hide();
	ui->pushButtonSecPlayerPlay->show();
	ui->pushButtonSecPlayerPlay->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：停止播放AVI录像
参数：无
返回：无
************************************************************/
void SecurityPlayerDialog::on_pushButtonSecPlayerStop_pressed()
{
	stopPlayer(false);
}

/************************************************************
描述：停止播放AVI录像，并关闭本窗口
参数：无
返回：无
************************************************************/
void SecurityPlayerDialog::on_pushButtonSecPlayerReturn_pressed()
{
	if (!isVisible()) return;

	stopPlayer();
	hide();
}

/************************************************************
描述：设置播放视频的显示窗口坐标，坐标值为0则停止视频显示
参数：bShow - true: 设置播放视频的显示窗口坐标并开启视频显示
			 false: 停止视频显示
返回：0 - 成功
	 -1 - 失败
************************************************************/
int SecurityPlayerDialog::setDisplayWindow(bool bShow)
{
	int vrect[4];
	QRect rect;

	while (g_pReadAVI == (ReadAVI *)-1)
	{
		usleep(10000);
	}

	if (!g_pReadAVI) return -1;

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

	ms_filter_call_method(g_pReadAVI->videoout, MS_FILTER_SET_VIDEO_RECT, vrect);

	return 0;
}

/************************************************************
描述：窗口事件过滤函数，实现播放视频窗口全屏/非全屏切换
参数：o - 产生事件的控件
	 e - 需处理的事件
返回：true - 事件处理成功，不需进一步处理
	 false - 需要进一步处理
************************************************************/
bool SecurityPlayerDialog::eventFilter(QObject *o, QEvent *e)
{
	if (e->type() == QEvent::MouseButtonPress)
	{
		if (!ui->pushButtonSecPlayerPause->isVisible())
		{
			return false;
		}

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
描述：窗口显示时，创建播放区域背景图像缓存区，刷新Tab焦点
参数：无
返回：无
************************************************************/
void SecurityPlayerDialog::showEvent(QShowEvent *)
{
	//QRect rc = ui->frameVideoShow->geometry();
	//rc.setWidth(rc.width() + 1);
	//rc.setHeight(rc.height() + 1);

	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());
	pMainDlg->showRefreshTabFocus(this);
}

/************************************************************
描述：窗口隐藏时，释放播放区域背景图像缓存区，刷新Tab焦点
参数：无
返回：无
************************************************************/
void SecurityPlayerDialog::hideEvent(QHideEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());
	pMainDlg->hideRefreshTabFocus(this);

    ((SecurityMainDialog *)parent())->ui->pushButtonSecMainDetail->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：音量减小按钮回调函数，将音量减小10(范围为0-60)
参数：无
返回：无
************************************************************/
/*void SecurityPlayerDialog::on_pushButtonVolumeSub_pressed()
{
	ui->horizontalSliderVolume->triggerAction(QAbstractSlider::SliderPageStepSub);
}*/

/************************************************************
描述：音量增大按钮回调函数，将音量增大10(范围为0-60)
参数：无
返回：无
************************************************************/
/*void SecurityPlayerDialog::on_pushButtonVolumeAdd_pressed()
{
	ui->horizontalSliderVolume->triggerAction(QAbstractSlider::SliderPageStepAdd);
}*/

/************************************************************
描述：音量滑动条按动触发回调函数
参数：value - 音量，范围为0-60，
返回：无
************************************************************/
/*void SecurityPlayerDialog::on_horizontalSliderVolume_valueChanged(int value)
{
	MainDialog *pMDlg = (MainDialog *)(parent()->parent());

	pMDlg->m_pVoipMainDlg->m_nVolume = (unsigned int)(value * 1024.0/60.0);

	while (g_pReadAVI == (ReadAVI *)-1)
	{
		usleep(10000);
	}

	if (isVisible() && (g_pReadAVI != NULL))
	{
		avi_set_audio_level(g_pReadAVI, pMDlg->m_pVoipMainDlg->m_nVolume);
	}
}*/

/************************************************************
描述：发送播放器停止信号
参数：无
返回：无
************************************************************/
void SecurityPlayerDialog::emitSigStopPlayer()
{
	emit sigStopPlayer();
}

/************************************************************
描述：播放器停止信号的槽函数，启动200ms的定时器，定时器超时时停止播放
参数：无
返回：无
************************************************************/
void SecurityPlayerDialog::slotStopPlayer()
{
	m_pPlayEndTimer->start(200);
}
