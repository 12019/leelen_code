#include "mediaplayerdialog.h"
#include "ui_mediaplayerdialog.h"
#include <QMessageBox>
#include "mediastreamer2/AVI_read.h"
#include "maindialog.h"
#include "exosip_main.h"
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/mman.h>
//#include "callcenterdialog.h"
//#include "helpdialog.h"
#include "application.h"
#include "ui_mediamandialog.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

/************************************************************
  该文件为对讲录像播放对话框主程序
************************************************************/

/************************************************************
  视频显示区域的背景色
************************************************************/
////#define PLAY_AREA_BGCOLOR QColor(0, 0, 0)

//extern eXosip_element_t *eXosipElement;
extern ReadAVI *g_pReadAVI;

//static Ui::MediaPlayerDialog *ui_media_player = NULL;
static MediaPlayerDialog *media_player_dlg = NULL;
//static int condition = 0;

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

	//condition = 1;
	//printf("\nbcom userdata:%p, id: %d, arg:%p\n\n", userdata, id, arg);

	if (id == MS_AVI_PLAYER_EOF)
	{
		if (media_player_dlg)
		{
			media_player_dlg->emitSigStopPlayer();
		}
	}
}

/************************************************************
描述：对讲录像播放对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
MediaPlayerDialog::MediaPlayerDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MediaPlayerDialog)
{
	ui->setupUi(this);

	////m_pImageBuf = NULL;

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

	int x,y,w,h;
	g_pScaleSrc->get_info_framepanel_geometry(x,y,w,h);
	ui->framePanel->setGeometry(x,y,w,h);

	media_player_dlg = this;
	g_pReadAVI = NULL;

	ui->pushButtonMediaPlayerPlay->show();
	ui->pushButtonMediaPlayerPause->hide();

	m_pFrameFullshow = new QFrame(this);
	m_pFrameFullshow->setObjectName("frameFullshow");
	m_pFrameFullshow->setGeometry(QRect(0, 0, qApp->desktop()->size().width(),qApp->desktop()->size().height()));
	m_pFrameFullshow->setFrameShadow(QFrame::Plain);
	m_pFrameFullshow->setFrameShape(QFrame::Panel);

	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	m_pFrameFullshow->installEventFilter(this);

    	g_pScaleSrc->get_videoshow_geometry(x,y,w,h);
	ui->frameVideoShow->setGeometry(x,y,w,h);
	ui->frameVideoShow->installEventFilter(this);

	g_pScaleSrc->get_info_label_geometry(x,y,w,h);
	ui->labelInfo->setGeometry(x,y,w,h);

    int res = g_pScaleSrc->get_screen_version();
    if ( res == 2 )  // 1024x600
    {
        ui->pushButtonMediaPlayerPause->setGeometry(QRect(764, 260, 230, 80));
        ui->pushButtonMediaPlayerPlay->setGeometry(QRect(764, 260, 230, 80));
        ui->pushButtonMediaPlayerStop->setGeometry(QRect(764, 340, 230, 80));
        ui->pushButtonMediaPlayerCall->setGeometry(QRect(764, 420, 230, 80));
        ui->pushButtonMediaPlayerReturn->setGeometry(QRect(764, 500, 230, 80));
    }
    else if ( res == 3 ) // 1280x800
    {
        ui->pushButtonMediaPlayerPause->setGeometry(QRect(959, 353, 279, 99));
        ui->pushButtonMediaPlayerPlay->setGeometry(QRect(959, 353, 279, 99));
        ui->pushButtonMediaPlayerStop->setGeometry(QRect(959, 461, 279, 99));
        ui->pushButtonMediaPlayerCall->setGeometry(QRect(959, 569, 279, 99));
        ui->pushButtonMediaPlayerReturn->setGeometry(QRect(959, 677, 279, 99));
    }


	m_pPlayEndTimer = new QTimer(this);

	connect(this, SIGNAL(sigStopPlayer()), this, SLOT(slotStopPlayer()));
	connect(m_pPlayEndTimer, SIGNAL(timeout()), this, SLOT(stopPlayer()));
}

/************************************************************
描述：对讲录像播放对话框析构函数
参数：无
返回：无
************************************************************/
MediaPlayerDialog::~MediaPlayerDialog()
{
	stopPlayer();

	/*if (m_pImageBuf)
	{
		delete [] m_pImageBuf;
	}*/

	delete m_pPlayEndTimer;
	delete m_pFrameFullshow;

	delete ui;
}

/************************************************************
描述：设置播放器窗口的参数，包括呼叫地址、AVI文件路径
参数：address - 对讲对方的网络名
	 aviurl - 录像AVI文件的路径
返回：无
************************************************************/
void MediaPlayerDialog::setDetail(QString address, QString netAddress, QString aviurl)
{
	m_address = address;
	m_VideoPath = aviurl;

	m_netAddress = netAddress;
	int pos = m_netAddress.indexOf("?");
	if (pos >= 0)
	{
		m_netAddress = m_netAddress.left(pos);
	}

	ui->labelInfo->setText(tr("对讲详情") + " - " + address);

	//g_pReadAVI = NULL;

	////QPalette palette;
	////palette.setBrush(ui->frameVideoShow->backgroundRole(), QBrush(PLAY_AREA_BGCOLOR));
	////ui->frameVideoShow->setPalette(palette);
	////ui->frameVideoShow->setAutoFillBackground(true);
	////ui->frameVideoShow->repaint();

	ui->pushButtonMediaPlayerPlay->show();
	ui->pushButtonMediaPlayerPause->hide();
	ui->pushButtonMediaPlayerPlay->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：启动AVI录像播放
参数：无
返回：无
************************************************************/
void MediaPlayerDialog::startPlayer()
{
	MSSndCard *sc;
	const char * card_id = NULL;
	char pathname[256];

	printf("MediaPlayerDialog::startPlayer\n");

	QFile avi_file(m_VideoPath);
	if (!avi_file.open(QIODevice::ReadOnly))
	{
		setAllButtonsEnabled(this, false);
		ui->framePanel->setFocus(Qt::TabFocusReason);
		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		//Q_UNUSED(okButton);
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("AVI文件不存在"));
		messBox.exec();
		setAllButtonsEnabled(this, true);
        ui->pushButtonMediaPlayerPlay->setFocus(Qt::TabFocusReason);

		return;
	}

	avi_file.close();

	//pathname = "/mnt/disk/synchronism.avi";
	strcpy(pathname, m_VideoPath.toUtf8().data());
	sc = ms_snd_card_manager_get_card(ms_snd_card_manager_get(), card_id);
    //printf("bcom pushButtonMediaPlayerPlay_clicked:%p-%s\n", pathname, pathname);
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
		//ui->framePanel->setFocus(Qt::TabFocusReason);
		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		//Q_UNUSED(okButton);
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("打开AVI文件失败"));
		messBox.exec();
		setAllButtonsEnabled(this, true);
		ui->pushButtonMediaPlayerPlay->setFocus(Qt::TabFocusReason);

		return;
	}

	((Application *)qApp)->setLCDOn();

	MainDialog *pMDlg = (MainDialog *)(parent()->parent());
	avi_set_audio_level(g_pReadAVI, pMDlg->m_pVoipMainDlg->m_nVolume);

	setDisplayWindow(true);

	ui->pushButtonMediaPlayerPlay->hide();
	ui->pushButtonMediaPlayerPause->show();
	ui->pushButtonMediaPlayerPause->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：停止AVI录像播放
参数：无
返回：无
************************************************************/
void MediaPlayerDialog::stopPlayer(bool bResetFocus)
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

	ui->pushButtonMediaPlayerPlay->show();
	ui->pushButtonMediaPlayerPause->hide();
	if (bResetFocus)
	{
        	ui->pushButtonMediaPlayerPlay->setFocus(Qt::TabFocusReason);
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
描述：停止AVI录像播放并关闭该对话框
参数：无
返回：无
************************************************************/
void MediaPlayerDialog::on_pushButtonMediaPlayerReturn_pressed()
{
	if (!isVisible()) return;

	stopPlayer();

	hide();
}

/************************************************************
描述：开始或恢复AVI录像播放
参数：无
返回：无
************************************************************/
void MediaPlayerDialog::on_pushButtonMediaPlayerPlay_pressed()
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
			//condition = 0;
			if (g_pReadAVI)
			{
				ReadAVI *tmp_pReadAVI = g_pReadAVI;
				g_pReadAVI = (ReadAVI *)-1;
				avi_stop(tmp_pReadAVI);
				g_pReadAVI = NULL;
			}
			//m_bStopi = 0;
		}
		else
		{
			ms_filter_call_method_noarg(g_pReadAVI->readavi,MS_FILE_readavi_START);

			ui->pushButtonMediaPlayerPlay->hide();
			ui->pushButtonMediaPlayerPause->show();
			ui->pushButtonMediaPlayerPause->setFocus(Qt::TabFocusReason);

			return;
		}
	}

	startPlayer();
}

/*void MediaPlayerDialog::on_pushButtonExport_pressed()
{

}*/

/************************************************************
描述：关闭AVI录像播放，打开VoIP拨号窗口对对讲对方进行回呼
参数：无
返回：无
************************************************************/
void MediaPlayerDialog::on_pushButtonMediaPlayerCall_pressed()
{
	stopPlayer();
	hide();

	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());
    pMainDlg->m_pVoipMainDlg->show();
    pMainDlg->m_pVoipMainDlg->raise();
    pMainDlg->m_pVoipMainDlg->m_pIctDlg->show();
	pMainDlg->m_pVoipMainDlg->m_pIctDlg->ResetState(m_netAddress);
}

/************************************************************
描述：暂停AVI录像播放
参数：无
返回：无
************************************************************/
void MediaPlayerDialog::on_pushButtonMediaPlayerPause_pressed()
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

	/*
	//去掉窗口左下方的背景色小方块
	// LCD frame buffer open
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

	QRect rc = ui->frameVideoShow->geometry();
	rc.setWidth(rc.width() + 1);
	rc.setHeight(rc.height() + 1);
	//uint8_t *image_buf = (uint8_t *)malloc(rc.width() * rc.height() * 2);

	int i;
	for (i = 0; i < rc.height(); i++)
	{
		memcpy(m_pImageBuf + i * rc.width() * 2, fb_addr + ((i + rc.y()) * 800 + rc.x()) * 2, rc.width() * 2);
	}

	munmap(fb_addr, fb_size);
	::close(fb_fd);

	QImage image((uchar *)m_pImageBuf, rc.width(), rc.height(), QImage::Format_RGB16);

	QPalette palette;
	palette.setBrush(ui->frameVideoShow->backgroundRole(), QBrush(image));//QPixmap("/mnt/disk/images/main/background-1.jpg"));
	ui->frameVideoShow->setPalette(palette);
	ui->frameVideoShow->setAutoFillBackground(true);

	ui->frameVideoShow->setStyleSheet("");
	//printf("bcom free 6\n");
	//free(image_buf);
	*/
	ms_filter_call_method_noarg(g_pReadAVI->readavi, MS_FILE_readavi_STOP);

	ui->pushButtonMediaPlayerPause->hide();
	ui->pushButtonMediaPlayerPlay->show();
	ui->pushButtonMediaPlayerPlay->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：停止AVI录像播放
参数：无
返回：无
************************************************************/
void MediaPlayerDialog::on_pushButtonMediaPlayerStop_pressed()
{
	stopPlayer(false);
}

/************************************************************
描述：音量滑动条按动触发回调函数
参数：value - 音量，范围为0-60，
返回：无
************************************************************/
/*void MediaPlayerDialog::on_horizontalSliderVolume_valueChanged(int value)
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
描述：设置播放视频的显示窗口坐标，坐标值为0则停止视频显示
参数：bShow - true: 设置播放视频的显示窗口坐标并开启视频显示
			 false: 停止视频显示
返回：0 - 成功
	 -1 - 失败
************************************************************/
int MediaPlayerDialog::setDisplayWindow(bool bShow)
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
bool MediaPlayerDialog::eventFilter(QObject *o, QEvent *e)
{
	if (e->type() == QEvent::MouseButtonPress)
	{
		if (!ui->pushButtonMediaPlayerPause->isVisible())
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
void MediaPlayerDialog::showEvent(QShowEvent *)
{
	//QRect rc = ui->frameVideoShow->geometry();
	//rc.setWidth(rc.width() + 1);
	//rc.setHeight(rc.height() + 1);

	/*if (m_pImageBuf)
	{
		delete [] m_pImageBuf;
	}

	m_pImageBuf = new char[rc.width() * rc.height() * 2];
	*/

	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());
	pMainDlg->showRefreshTabFocus(this);
}

/************************************************************
描述：窗口隐藏时，释放播放区域背景图像缓存区，刷新Tab焦点
参数：无
返回：无
************************************************************/
void MediaPlayerDialog::hideEvent(QHideEvent *)
{
	/*if (m_pImageBuf)
	{
		delete [] m_pImageBuf;
		m_pImageBuf = NULL;
	}*/

	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());
	pMainDlg->hideRefreshTabFocus(this);

	((MediaManDialog *)parent())->ui->pushButtonMediaManDetail->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：音量减小按钮回调函数，将音量减小10(范围为0-60)
参数：无
返回：无
************************************************************/
/*void MediaPlayerDialog::on_pushButtonVolumeSub_pressed()
{
	ui->horizontalSliderVolume->triggerAction(QAbstractSlider::SliderPageStepSub);
}*/

/************************************************************
描述：音量增大按钮回调函数，将音量增大10(范围为0-60)
参数：无
返回：无
************************************************************/
/*void MediaPlayerDialog::on_pushButtonVolumeAdd_pressed()
{
	ui->horizontalSliderVolume->triggerAction(QAbstractSlider::SliderPageStepAdd);
}*/

/************************************************************
描述：发送播放器停止信号
参数：无
返回：无
************************************************************/
void MediaPlayerDialog::emitSigStopPlayer()
{
	emit sigStopPlayer();
}

/************************************************************
描述：播放器停止信号的槽函数，启动200ms的定时器，定时器超时时停止播放
参数：无
返回：无
************************************************************/
void MediaPlayerDialog::slotStopPlayer()
{
	m_pPlayEndTimer->start(200);
}
