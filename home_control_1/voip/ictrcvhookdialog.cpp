#include "ictrcvhookdialog.h"
#include "ui_ictrcvhookdialog.h"
#include "lpcore.h"
#include "command.h"
#include "qconfig.h"
#include "mediastreamer2/mediastream.h"
#include "mediastreamer2/msvideo.h"
#include "voipmaindialog.h"
#include "exosip_main.h"
#include "application.h"
#include "ui_ictdialog.h"
#include "maindialog.h"
#include "core/leelen_comdef.h"
#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
/************************************************************
  该文件为可视对讲拨号后通话对话框主程序
************************************************************/
extern LinphoneCore *lphone;
extern eXosip_element_t *eXosipElement;
extern eXosip_t eXosip;
//extern char g_strLphoneName[3][50];
extern MainDialog *g_pMainDlg;
extern RESULT_E g_check_computercenter_udp_state;

extern char g_strNativeName[50];
extern char g_strLphoneName[3][50];
extern char g_strDialRemoteName[50];
extern Lvirdd *lvirdd;
extern ViewCam *g_pViewCam;
extern QString callCode;

//#define PLAY_AREA_BGCOLOR QColor(0, 0, 0)

/************************************************************
描述：可视对讲拨号后通话对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
IctRcvHookDialog::IctRcvHookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IctRcvHookDialog)
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

    g_pScaleSrc->get_videoshow_geometry(x,y,w,h);
	ui->frameVideoShow->setGeometry(x,y,w,h);

	g_pScaleSrc->get_info_label_geometry(x,y,w,h);
	ui->lineEditInfo->setGeometry(x,y,w,h);

    int res = g_pScaleSrc->get_screen_version();
    if ( res == 2 )  // 1024x600
    {
        ui->pushButtonIctRcvHookHangUp->setGeometry(QRect(764, 260, 230, 80));
        ui->pushButtonIctRcvHookVideoOpen->setGeometry(QRect(764, 340, 230, 80));
        ui->pushButtonIctRcvHookVideoClose->setGeometry(QRect(764, 340, 230, 80));
        ui->pushButtonIctRcvHookOpen->setGeometry(QRect(764, 420, 230, 80));
        ui->pushButtonIctRcvHookReturn->setGeometry(QRect(764, 500, 230, 80));
    }
    else if ( res == 3 ) // 1280x800
    {
        ui->pushButtonIctRcvHookHangUp->setGeometry(QRect(959, 353, 279, 99));
        ui->pushButtonIctRcvHookVideoOpen->setGeometry(QRect(959, 461, 279, 99));
        ui->pushButtonIctRcvHookVideoClose->setGeometry(QRect(959, 461, 279, 99));
        ui->pushButtonIctRcvHookOpen->setGeometry(QRect(959, 569, 279, 99));
        ui->pushButtonIctRcvHookReturn->setGeometry(QRect(959, 677, 279, 99));
    }

	ui->pushButtonIctRcvHookVideoClose->hide();

	m_pFrameFullshow = new QFrame(this);
	m_pFrameFullshow->setObjectName("frameFullshow");
	m_pFrameFullshow->setGeometry(QRect(0, 0, qApp->desktop()->size().width(), qApp->desktop()->size().height()));
	m_pFrameFullshow->setFrameShadow(QFrame::Plain);
	m_pFrameFullshow->setFrameShape(QFrame::Panel);

	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	m_pFrameFullshow->installEventFilter(this);
	ui->frameVideoShow->installEventFilter(this);

	m_pFilterTimer = new QTimer(this);

	m_pFilterTimer->setSingleShot(true);
	
	m_pTimeoutDialog = new QTimer(this);
	connect(m_pTimeoutDialog,SIGNAL(timeout()),this,SLOT(Dialog_Timeout_Event()));
}

/************************************************************
描述：可视对讲拨号后通话对话框析构函数
参数：无
返回：无
************************************************************/
IctRcvHookDialog::~IctRcvHookDialog()
{
	if ( m_pTimeoutDialog->isActive() )
	{
		m_pTimeoutDialog->stop();
		delete m_pTimeoutDialog;
	}
	delete m_pFrameFullshow;
	delete ui;
}

/************************************************************
描述：分机对讲超时处理函数
参数：无
返回：无
************************************************************/
void IctRcvHookDialog::Dialog_Timeout_Event()
{
	m_pTimeoutDialog->stop();
	on_pushButtonIctRcvHookReturn_pressed();
}

/************************************************************
描述：重置本对话框
参数：text - 信息显示控件中的显示文本
返回：无
************************************************************/
void IctRcvHookDialog::ResetState(QString strInfo)
{
	ui->lineEditInfo->setText(strInfo);

	if ( m_Call_Manage_Machine == false )
	{
		if (((VoipMainDialog *)(parent()->parent()))->m_bVideoOpen)
		{
		ui->pushButtonIctRcvHookVideoOpen->hide();
		ui->pushButtonIctRcvHookVideoClose->show();
		}
		else
		{
		ui->pushButtonIctRcvHookVideoOpen->show();
		ui->pushButtonIctRcvHookVideoClose->hide();
		}
	}
	else
	{
        if (!(((VoipMainDialog *)(parent()->parent()))->m_bVideoOpen))
		{
        ui->pushButtonIctRcvHookVideoOpen->show();
        ui->pushButtonIctRcvHookVideoClose->hide();
		}
		else
		{
        ui->pushButtonIctRcvHookVideoOpen->hide();
		ui->pushButtonIctRcvHookVideoClose->show();
		}
	}

	ui->pushButtonIctRcvHookHangUp->setFocus(Qt::TabFocusReason);

	QString strNativeName(g_strNativeName);
	QString strDialRemoteName(g_strDialRemoteName);

    /*
	if ((((IctDialog *)parent())->m_iModalType & DOORWAY_MODAL_TYPE)
		|| (strNativeName.left(5) == strDialRemoteName.left(5)))
	{
        ui->pushButtonIctRcvHookOpen->setEnabled(true);//->show();
	}
	else
	{
        ui->pushButtonIctRcvHookOpen->setEnabled(false);//->hide();
	}
    */
}

/************************************************************
描述：本机视频打开，即允许远端设备观看本机摄像头视频
参数：无
返回：无
************************************************************/
void IctRcvHookDialog::on_pushButtonIctRcvHookVideoOpen_pressed()
{
    if (!ui->pushButtonIctRcvHookVideoOpen->isVisible()) return;

    if ( m_Call_Manage_Machine == false )
	((VoipMainDialog *)(parent()->parent()))->m_bVideoOpen = true;

	int i;

	if (!(((IctDialog *)parent())->m_iModalType & DOORWAY_MODAL_TYPE))
	{
		if (eXosip.callflag == 0)
		{
			if (eXosipElement->videostream != NULL)
			{
				lpc_cmd_video(lphone, (char *)"open");
				video_stream_unmute_send(eXosipElement->videostream);
			}
		}
		else
		{
			bool bTmp = false;
			for (i = 0; i < MAX_EXTENSION_MACHINE_CNT; i++) {
				if (eXosip.remip[i].videostream != NULL) {
					if (!bTmp)
					{
						lpc_cmd_video(lphone, (char *)"open");
						bTmp = true;
					}
					video_stream_unmute_send(eXosip.remip[i].videostream);
				}
			}
		}
	}

    ui->pushButtonIctRcvHookVideoOpen->hide();
    ui->pushButtonIctRcvHookVideoClose->show();
    ui->pushButtonIctRcvHookVideoClose->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：本机视频关闭，即不允许远端设备观看本机摄像头视频
参数：无
返回：无
************************************************************/
void IctRcvHookDialog::on_pushButtonIctRcvHookVideoClose_pressed()
{
    if (!ui->pushButtonIctRcvHookVideoClose->isVisible()) return;

    if ( m_Call_Manage_Machine == false )
	((VoipMainDialog *)(parent()->parent()))->m_bVideoOpen = false;

	int i;
	if (eXosip.callflag == 0)
	{
		if (eXosipElement->videostream != NULL)
		{
			lpc_cmd_video(lphone, (char *)"close");
			video_stream_mute_send(eXosipElement->videostream);
		}
	}
	else
	{
		bool bTmp = false;
		for (i = 0; i < MAX_EXTENSION_MACHINE_CNT; i++) {
			if (eXosip.remip[i].videostream != NULL) {
				if (!bTmp)
				{
					lpc_cmd_video(lphone, (char *)"close");
					bTmp = true;
				}
				video_stream_mute_send(eXosip.remip[i].videostream);
			}
		}
	}

    ui->pushButtonIctRcvHookVideoOpen->show();
    ui->pushButtonIctRcvHookVideoClose->hide();
    ui->pushButtonIctRcvHookVideoOpen->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：挂断正在通话的可视对讲应用，关闭本对话框
参数：无
返回：无
************************************************************/
void IctRcvHookDialog::on_pushButtonIctRcvHookHangUp_pressed()
{
	if (!isVisible()) return;

	((IctDialog *)parent())->m_bHookCloseIctDlg = true;

	setDisplayWindow(false);
	usleep(120000);

	ui->lineEditInfo->show();
	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	ui->frameVideoShow->repaint();

	lpc_cmd_terminate(lphone, NULL);

    hide();
    ((IctDialog *)parent())->hide();
    ((VoipMainDialog *)(parent()->parent()))->hide();

	((Application *)qApp)->setLCDAndTimerOn();

	Stop_Timeout_Count();
}

/************************************************************
描述：拨号后通话对话框事件过滤函数，实现播放远端视频窗口全屏/非全屏切换
参数：o - 产生事件的控件
	 e - 需处理的事件
返回：true - 事件处理成功，不需进一步处理
	 false - 需要进一步处理
************************************************************/
bool IctRcvHookDialog::eventFilter(QObject *o, QEvent *e)
{
	if (e->type() == QEvent::MouseButtonPress)
	{
        if(m_pFilterTimer->isActive()){
            return false;
        }
        m_pFilterTimer->start(800);
        //david 2014/11/17
        /*
		setDisplayWindow(false);
		usleep(120000);
        */
		if (o == ui->frameVideoShow)
		{
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

            if(g_pViewCam == NULL)
            {
                ms_filter_call_method(lvirdd->branch.display,MS_FILTER_SET_VIDEO_HIDE,0);
            }

            repaint();
		}

		setDisplayWindow(true);
	}

	return false;
}

/************************************************************
描述：设置播放远端视频的显示窗口坐标，坐标值为0则停止视频显示
参数：bShow - true: 设置播放远端视频的显示窗口坐标并开启视频显示
			 false: 停止视频显示
返回：0 - 成功
	 -1 - 失败
************************************************************/
extern Lvirdd *lvirdd;
int IctRcvHookDialog::setDisplayWindow(bool bShow)
{
	int vrect[4];
	QRect rect;

	if (!lvirdd) return -1;
	if (!lvirdd->branch.display) return -1;

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
            //rect = QRect(rect.x() -2, rect.y() -5, rect.width() + 2, rect.height() + 12);
            		rect = QRect(rect.x()/*+ 2*/, rect.y() /*+ 2*/, rect.width() - 1 /*- 4*/, rect.height() - 1/*- 4*/);
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

	ms_filter_call_method(lvirdd->branch.display, MS_FILTER_SET_VIDEO_RECT, vrect);

	return 0;
}

/************************************************************
描述：挂断通话然后关闭本对话框
参数：无
返回：无
************************************************************/
void IctRcvHookDialog::on_pushButtonIctRcvHookReturn_pressed()
{
	if (!isVisible()) return;

	((IctDialog *)parent())->m_bHookCloseIctDlg = false;

	setDisplayWindow(false);
	usleep(120000);

	ui->lineEditInfo->show();
	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	ui->frameVideoShow->repaint();

	lpc_cmd_terminate(lphone, NULL);

	hide();

	((Application *)qApp)->setLCDAndTimerOn();

	Stop_Timeout_Count();
}

/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void IctRcvHookDialog::showEvent(QShowEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)(parent()->parent()->parent());

	pMainDlg->showRefreshTabFocus(this);
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void IctRcvHookDialog::hideEvent(QHideEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)(parent()->parent()->parent());

	pMainDlg->hideRefreshTabFocus(this);

	if (((IctDialog *)parent())->isVisible())
	{
		((IctDialog *)parent())->ui->pushButtonIctDial->setFocus(Qt::TabFocusReason);
	}
}

/************************************************************
描述：如果通话对方是门口机，向对方机发送开锁命令
参数：无
返回：无
************************************************************/
void IctRcvHookDialog::on_pushButtonIctRcvHookOpen_pressed()
{
    //david 2014.4.4
	if ((((IctDialog *)parent())->m_iModalType & DOORWAY_MODAL_TYPE) ||
			(((IctDialog *)parent())->m_iModalType & IP_SWITCH_TYPE)  )
	{
		lpc_cmd_open(lphone, NULL);

		CallInfoUploadThread *pCallInfoUploadThread = ((VoipMainDialog *)parent()->parent())->m_pCallInfoUploadThread;

		if( pCallInfoUploadThread->m_bRun == false)
		{
            if(g_check_computercenter_udp_state == RESULT_SUCCESS)
            {
                pCallInfoUploadThread->m_upInfoType |= RICT_OPEND;
                pCallInfoUploadThread->getCallCode(callCode);
            }
            else
            {
                pCallInfoUploadThread->m_upInfoType = 3;
                pCallInfoUploadThread->getCallCode(callCode);
                pCallInfoUploadThread->start();
            }
		}
	}
	else
    {
		QString strDialRemoteName(g_strDialRemoteName);
        GetPeerIPThread *pGetPeerIPThread = ((VoipMainDialog *)parent()->parent())->m_pGetPeerIPThread;
        AuthorizeElevatorThread *pAuthorizeElevatorThread = ((VoipMainDialog *)parent()->parent())->m_pAuthorizeElevatorThread;
        //g_pMainDlg->m_pVoipMainDlg->authorizeElevator(strDialRemoteName);
        if(strDialRemoteName.left(4) == pGetPeerIPThread->m_nativeName.left(4))
        {
            if(pAuthorizeElevatorThread->m_bRun)
            {
                pAuthorizeElevatorThread->m_bRun = false;
                pAuthorizeElevatorThread->quit();
                pAuthorizeElevatorThread->wait();
            }

            pAuthorizeElevatorThread->m_talkRemoteName = strDialRemoteName ;
            pAuthorizeElevatorThread->start();
        }
	}
}

void IctRcvHookDialog::Start_Timeout_Count()
{
	if ( m_pTimeoutDialog->isActive() )
		m_pTimeoutDialog->stop();

	m_pTimeoutDialog->start(MAX_TIMEOUT_DIALOG);
}

void IctRcvHookDialog::Stop_Timeout_Count()
{
	if ( m_pTimeoutDialog->isActive() )
		m_pTimeoutDialog->stop();
}

/************************************************************
描述：从全屏状态退出
参数：无
返回：无
************************************************************/
void IctRcvHookDialog::Exit_From_Full_Screen()
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
