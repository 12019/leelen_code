#include "istdialog.h"
#include "ui_istdialog.h"
#include "lpcore.h"
#include "command.h"
#include "qconfig.h"
#include "mediastreamer2/mediastream.h"
#include "mediastreamer2/msvideo.h"
#include "maindialog.h"
#include "exosip_main.h"
#include "voipmaindialog.h"
#include "application.h"
#include "ui_maindialog.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

extern "C"
{
    #include "mediastreamer2/tvout.h"
}

/************************************************************
  该文件为来电接听对话框主程序
************************************************************/
int g_iIST_state = 0;
extern LinphoneCore *lphone;
extern eXosip_element_t *eXosipElement;
extern Laumix *laumix;

extern int g_video_channel;
extern Lvicet *lvicet;
extern Lvirdd *lvirdd;
extern ViewCam *g_pViewCam;
extern char g_strNativeName[50];
extern char g_strLphoneName[3][50];
extern char g_strDialRemoteName[50];
extern MainDialog *g_pMainDlg;

//#define PLAY_AREA_BGCOLOR QColor(0, 0, 0)
//#define AUTO_ANSWER 1
/************************************************************
描述：来电接听对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
IstDialog::IstDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IstDialog)
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
    if 	( res == 2 )  // 1024x600
    {
        ui->pushButtonIstVideoOpen->setGeometry(QRect(764, 340, 230, 80));
        ui->pushButtonIstVideoClose->setGeometry(QRect(764, 340, 230, 80));
        ui->pushButtonIstHangUp->setGeometry(QRect(764, 500, 230, 80));
        ui->pushButtonIstOpen->setGeometry(QRect(764, 420, 230, 80));
        ui->pushButtonIstAnswer->setGeometry(QRect(764, 260, 230, 80));
        ui->labelExtension1->setGeometry(QRect(800, 100, 151, 31));
        ui->labelExtension2->setGeometry(QRect(800, 130, 151, 31));
    }
    else if ( res == 3 )  // 1280x800
    {
        ui->pushButtonIstVideoOpen->setGeometry(QRect(959, 461, 279, 99));
        ui->pushButtonIstVideoClose->setGeometry(QRect(959, 461, 279, 99));
        ui->pushButtonIstHangUp->setGeometry(QRect(959, 677, 279, 99));
        ui->pushButtonIstOpen->setGeometry(QRect(959, 569, 279, 99));
        ui->pushButtonIstAnswer->setGeometry(QRect(959, 353, 279, 99));
        ui->labelExtension1->setGeometry(QRect(980, 120, 241, 40));
        ui->labelExtension2->setGeometry(QRect(980, 170, 241, 40));
    }

 	ui->pushButtonIstVideoOpen->show();
	ui->pushButtonIstVideoClose->hide();

	m_bTalking = false;
	m_strRemoteNetName = "";
	m_strRemoteIp[0] = '\0';

	m_iModalType = BASIC_MODAL_TYPE;

	m_pFrameFullshow = new QFrame(this);
	m_pFrameFullshow->setObjectName("frameFullshow");
	m_pFrameFullshow->setGeometry(QRect(0, 0, qApp->desktop()->size().width(),qApp->desktop()->size().height()));
	m_pFrameFullshow->setFrameShadow(QFrame::Plain);
	m_pFrameFullshow->setFrameShape(QFrame::Panel);

	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	m_pFrameFullshow->installEventFilter(this);
	ui->frameVideoShow->installEventFilter(this);

	m_pJpgSaveTestThread = NULL;

	m_bExtensionUnitAnswer = false;
	m_iExtensionUnitAddr = 0x30;

	//ui->labelExtension->setText("");
    	//ui->labelExtension->hide();
#ifdef AUTO_ANSWER
	m_pAutoAnswerTimer = new QTimer(this);

	connect(m_pAutoAnswerTimer,SIGNAL(timeout()),this,SLOT(on_pushButtonIstAnswer_pressed()));

	m_pAutoHungupTimer = new QTimer(this);

	connect(m_pAutoHungupTimer,SIGNAL(timeout()),this,SLOT(on_pushButtonIstHangUp_pressed()));
#endif

	m_pFilterTimer = new QTimer(this);

	m_pFilterTimer->setSingleShot(true);
	
	m_pTimeOutAutoHungupTimer = new QTimer(this);
	connect(m_pTimeOutAutoHungupTimer,SIGNAL(timeout()),this,SLOT(Do_Record_Timeout_Hungup()));
}

/************************************************************
描述：来电接听对话框析构函数
参数：无
返回：无
************************************************************/
IstDialog::~IstDialog()
{
	if(m_pTimeOutAutoHungupTimer->isActive())
	{
		m_pTimeOutAutoHungupTimer->stop();
		delete m_pTimeOutAutoHungupTimer;
	}

	delete m_pFrameFullshow;
	delete ui;
}

/************************************************************
描述：重置本对话框
参数：无
返回：无
************************************************************/
void IstDialog::ResetState()
{
	if (((VoipMainDialog *)parent())->m_bVideoOpen)
	{
        ui->pushButtonIstVideoOpen->hide();
        ui->pushButtonIstVideoClose->show();
	}
	else
	{
        ui->pushButtonIstVideoOpen->show();
        ui->pushButtonIstVideoClose->hide();
	}

	ui->labelExtension1->setText("");
	ui->labelExtension2->setText("");
}

/************************************************************
描述：接听来电
参数：无
返回：无
************************************************************/
void IstDialog::on_pushButtonIstAnswer_pressed()
{
    #ifdef AUTO_ANSWER
        m_pAutoAnswerTimer->stop();
    #endif

	if (m_pTimeOutAutoHungupTimer->isActive())   // 防止自动挂断
		m_pTimeOutAutoHungupTimer->stop();

	ui->pushButtonIstAnswer->setEnabled(false);
	ui->lineEditInfo->setFocus(Qt::TabFocusReason);

	lpc_cmd_answer(lphone, NULL);

	if (m_bExtensionUnitAnswer)
	{
		int indx = m_iExtensionUnitAddr & 0x0f;
		ui->labelExtension1->setText(tr("扩展分机") + QString::number(indx + 1));
		ui->labelExtension2->setText(tr("　接听　"));

		g_video_channel = 1;

		ms_filter_call_method(lvicet->capture, MS_FILTER_SET_VIDEO_CHANNEL, &g_video_channel);

#ifdef ARM11
		tvout_start();
#endif
	}
	else
	{
		ui->labelExtension1->setText(tr("本 机"));
		ui->labelExtension2->setText(tr("　接听　"));
		//ui->labelExtension->show();

		g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->waitForVacant();
		g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iExtensionUnitAddr = m_iExtensionUnitAddr; //0x00;
		g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iExtensionUnitCallAddr = 0x02;
		g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iExtensionUnitCmd = 0xfb;
		g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->waitForVacant();

#ifdef ARM11
		tvout_stop();
#endif
	}

}

/************************************************************
描述：本机视频打开，即允许远端设备观看本机摄像头视频
参数：无
返回：无
************************************************************/
void IstDialog::on_pushButtonIstVideoOpen_pressed()
{
	((VoipMainDialog *)parent())->m_bVideoOpen = true;

    //if (m_bTalking && ((g_strLphoneName[0][0] == '\0') || (m_strRemoteNetName != g_strLphoneName[0])) && (eXosipElement->videostream != NULL))
	if (m_bTalking && (!(m_iModalType & DOORWAY_MODAL_TYPE)) && (eXosipElement->videostream != NULL))
	{
		lpc_cmd_video(lphone, (char *)"open");
		video_stream_unmute_send(eXosipElement->videostream);
	}

    ui->pushButtonIstVideoOpen->hide();
    ui->pushButtonIstVideoClose->show();

	/*
    ui->pushButtonIstVideoClose->setFocus(Qt::TabFocusReason);
	*/
}

/************************************************************
描述：本机视频关闭，即不允许远端设备观看本机摄像头视频
参数：无
返回：无
************************************************************/
void IstDialog::on_pushButtonIstVideoClose_pressed()
{
	((VoipMainDialog *)parent())->m_bVideoOpen = false;

	if (m_bTalking && (eXosipElement->videostream != NULL))
	{
		lpc_cmd_video(lphone, (char *)"close");
		video_stream_mute_send(eXosipElement->videostream);
	}

	ui->pushButtonIstVideoOpen->show();
	ui->pushButtonIstVideoClose->hide();

	/*
    	ui->pushButtonIstVideoOpen->setFocus(Qt::TabFocusReason);
	*/
}

/************************************************************
描述：留言如果超时就自己挂断
参数：无
返回：无
************************************************************/
void IstDialog::Do_Record_Timeout_Hungup()
{
	on_pushButtonIstHangUp_pressed();
	m_pTimeOutAutoHungupTimer->stop();
}

/************************************************************
描述：挂断正在通话的可视对讲应用，关闭本对话框
参数：无
返回：无
************************************************************/
void IstDialog::on_pushButtonIstHangUp_pressed()
{
	if (!isVisible()) return;

	setDisplayWindow(false);

    usleep(1000000);

	int cnt = 0;

	if(eXosipElement->state == IST_TALKING)
	{
		while(!m_bTalking)
		{
			qApp->processEvents();

			cnt++;

			usleep(200000);

			if(cnt > 20) break;
		}
	}

	ui->lineEditInfo->show();
	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();
//	lpc_cmd_terminate(lphone, NULL);
 
	QKeyEvent *modifiedEvent = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
	QApplication::postEvent((QObject *)ui->pushButtonIstOpen, modifiedEvent);

    hide();
    ((VoipMainDialog *)parent())->hide();

	((Application *)qApp)->setLCDAndTimerOn();

	g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->waitForVacant();
	g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iExtensionUnitAddr = m_iExtensionUnitAddr; //0x00;
	g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iExtensionUnitCallAddr = 0x02;
	g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iExtensionUnitCmd = 0xfa;
	g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->waitForVacant();

	if (!m_bExtensionUnitAnswer)
	{
		((Application *)qApp)->setUnmuteMic(false);
		((Application *)qApp)->setOnCamera(false);
	}
	else
	{
		((Application *)qApp)->setUnmuteSpeaker(true);
		((Application *)qApp)->setGPH7(false);
		((Application *)qApp)->setGPE0(true);

		m_bExtensionUnitAnswer = false;
	}

    	usleep(400000);
	lpc_cmd_terminate(lphone, NULL);				// 防止挂断后立即被呼入出现界面被卡的状态

#ifdef ARM11
    tvout_stop();
#endif

}

/************************************************************
描述：可视对讲来电接听窗口事件处理函数，实现lphone库IST上发消息的处理
参数：event - 需处理的事件
返回：true - 事件处理成功
	 false - 事件处理失败
************************************************************/
bool IstDialog::event(QEvent *event)
{
	switch (event->type() - 5000)
	{
	case NID_IDLE_RCVINV:
		{
            printf("\tNID_IDLE_RCVINV\n");


            g_iIST_state = 1;

#ifdef AUTO_ANSWER
            if(m_pAutoAnswerTimer->isActive())
            {
                m_pAutoAnswerTimer->stop();
            }

            m_pAutoAnswerTimer->start(2000);

            if(m_pAutoHungupTimer->isActive())
            {
                m_pAutoHungupTimer->stop();
            }

            m_pAutoHungupTimer->start(8000);
#endif
			((Application *)qApp)->setLCDOn();

			if (1) {
				MainDialog *pMainDlg = (MainDialog *)(parent()->parent());

				if (!pMainDlg->m_bInterFree)
				{
					my_ring_set_level(((VoipMainDialog *)parent())->m_nRingVolume);
				}
				else
				{
					my_ring_set_level(0);
				}
			}

			strcpy(m_strRemoteIp, eXosipElement->rem_ip);
			m_strRemoteNetName = (dynamic_cast <Levent *> (event))->msg;
			m_strRemoteFullNetName = m_strRemoteNetName;
			int pos = m_strRemoteNetName.indexOf("?");
			m_iModalType = BASIC_MODAL_TYPE;
			if (pos != -1)
			{
				m_iModalType = m_strRemoteNetName.mid(pos + strlen("?")).toInt();
				m_strRemoteNetName = m_strRemoteNetName.left(pos);
			}

			QStringList &addressList = g_pMainDlg->m_pAddressBookDlg->m_addressListNetName;
			int i;
			for (i = 0; i < addressList.count(); i++)
			{
				QString str = addressList.at(i);

				if (m_strRemoteNetName.length() > str.length())
				{
					if ((m_strRemoteNetName.left(str.length()) == str) && (m_strRemoteNetName.at(str.length()) == QChar('-')))
					{
						m_strRemoteNetName = g_pMainDlg->m_pAddressBookDlg->m_addressListName.at(i) + m_strRemoteNetName.mid(str.length());
						break;
					}
				}
				else if (m_strRemoteNetName == str)
				{
					m_strRemoteNetName = g_pMainDlg->m_pAddressBookDlg->m_addressListName.at(i);
					break;
				}
			}

			if (i >= addressList.count())
			{
				pos = m_strRemoteNetName.indexOf("-");
				if ((pos != -1) && ((pos != m_strRemoteNetName.length() - 2) || (m_strRemoteNetName.at(m_strRemoteNetName.length() - 1).isDigit())))
				{
					QString strTemp = g_strNativeName;//g_strLphoneName[0];
					int pos2 = strTemp.indexOf("-");
					if ((pos2 != -1) && ((pos2 != strTemp.length() - 2) || (strTemp.at(strTemp.length() - 1).isDigit())))
					{
						strTemp = strTemp.left(pos2);
						if (m_strRemoteNetName.left(pos) == strTemp)
						{
							m_strRemoteNetName = m_strRemoteNetName.mid(pos + strlen("-"));
						}
					}
				}
			}

            ui->lineEditInfo->setText(tr("呼入电话") + " - " + m_strRemoteNetName);

            ui->pushButtonIstAnswer->setEnabled(true);

			m_bTalking = false;

            ((QDialog *)parent())->raise();
            ((QDialog *)parent())->show();
            raise();
			show();
			ResetState();
            repaint();
			setDisplayWindow(true);

#ifdef ARM11
            tvout_start();
#endif

			m_bExtensionUnitAnswer = false;

			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->waitForVacant();
			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iExtensionUnitAddr = m_iExtensionUnitAddr; //0x00;
			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iExtensionUnitCallAddr = 0x02;
			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iExtensionUnitCmd = 0xfc;
			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->waitForVacant();

			break;
		}
	case NID_IST_RECORDING:
		{
			ui->lineEditInfo->setText(tr("呼入电话") + " - " + m_strRemoteNetName + tr("留言中"));

			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->waitForVacant();
			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iExtensionUnitAddr = m_iExtensionUnitAddr; //0x00;
			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iExtensionUnitCallAddr = 0x02;
			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iExtensionUnitCmd = 0xfa;
			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->waitForVacant();

			if (m_pTimeOutAutoHungupTimer->isActive())   // 防止再入
				m_pTimeOutAutoHungupTimer->stop();

			m_pTimeOutAutoHungupTimer->start(40000);   // 测试的时候用6s
			break;
		}
	case NID_IST_NONE:
		{
			ui->lineEditInfo->setText(tr("呼入电话") + " - " + m_strRemoteNetName);
			break;
		}
	case NID_IST_TIMEOUT:
		{
			/*if (m_pFrameFullshow->isVisible())
			{
				setDisplayWindow(false);
				usleep(100000);

				ui->lineEditInfo->show();
				m_pFrameFullshow->hide();
				m_pFrameFullshow->lower();
			}*/

			ui->lineEditInfo->setText(tr("呼入电话") + " - " + m_strRemoteNetName + tr("超时"));
            		g_iIST_state = 0;
			break;
		}
	case NID_IST_OPENTERM:
		{
			ui->lineEditInfo->setText(tr("呼入电话") + " - " + m_strRemoteNetName + tr("开锁退出"));

			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->waitForVacant();
			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iExtensionUnitAddr = m_iExtensionUnitAddr; //0x00;
			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iExtensionUnitCallAddr = 0x02;
			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iExtensionUnitCmd = 0xfa;
			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->waitForVacant();

			break;
		}
	case NID_IST_RCVVIDEOOPEN:
		{
			setDisplayWindow(true);
			break;
		}
	case NID_IST_RCVVIDEOCLOSE:
		{
			setDisplayWindow(false);
			usleep(120000);

			ui->lineEditInfo->show();
			m_pFrameFullshow->hide();
			m_pFrameFullshow->lower();

			ui->frameVideoShow->repaint();

			break;
		}
	case NID_IST_TALKING:
        {
            printf("\tNID_IST_TALKING\n");

            if(eXosipElement->state != IST_TALKING) break;  //david

            my_ring_stop();

			if (!m_bExtensionUnitAnswer)
			{
				((Application *)qApp)->setUnmuteMic(true);
				((Application *)qApp)->setOnCamera(true);
			}
			else
			{
				((Application *)qApp)->setUnmuteSpeaker(false);
				((Application *)qApp)->setGPH7(true);
				((Application *)qApp)->setGPE0(false);
			}

			if (eXosipElement->audiostream != NULL)
			{
				audio_stream_set_level(eXosipElement->audiostream, ((VoipMainDialog *)parent())->m_nVolume);
			}

			if (m_iModalType & DOORWAY_MODAL_TYPE)
			{
				if (eXosipElement->videostream != NULL)
                {
                    lpc_cmd_video(lphone, (char *)"close");
					video_stream_mute_send(eXosipElement->videostream);
				}
			}
			else
			{
				if (((VoipMainDialog *)parent())->m_bVideoOpen)
				{
					if (eXosipElement->videostream != NULL)
                    {
                        lpc_cmd_video(lphone, (char *)"open");
						video_stream_unmute_send(eXosipElement->videostream);
					}
				}
				else
				{
					if (eXosipElement->videostream != NULL)
                    {
                        lpc_cmd_video(lphone, (char *)"close");
						video_stream_mute_send(eXosipElement->videostream);
					}
				}
			}

			if (m_strRemoteFullNetName.left(5) == QString(g_strNativeName).left(5))
			{
                		ui->pushButtonIstOpen->setEnabled(true);//show();
			}

			ui->lineEditInfo->setText(tr("呼入电话") + " - " + m_strRemoteNetName + tr("通话中"));

			m_bTalking = true;

			break;
		}
	case NID_IST_RCVCANCEL:
		{
			printf("NID_IST_RCVCANCEL\n");

			if (isVisible())
			{
				ui->lineEditInfo->setText(tr("呼入电话") + " - " + m_strRemoteNetName + tr("接收取消"));

				m_bTalking = false;

				setDisplayWindow(false);
				usleep(120000);

                hide();
                ((VoipMainDialog *)parent())->hide();

				ui->lineEditInfo->show();
				m_pFrameFullshow->hide();
				m_pFrameFullshow->lower();
			}

			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->waitForVacant();
			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iExtensionUnitAddr = m_iExtensionUnitAddr; //0x00;
			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iExtensionUnitCallAddr = 0x02;
			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_iExtensionUnitCmd = 0xfa;
			g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->waitForVacant();

#ifdef ARM11
            tvout_stop();
#endif

			if (!m_bExtensionUnitAnswer)
			{
				((Application *)qApp)->setUnmuteMic(false);
				((Application *)qApp)->setOnCamera(false);
			}
			else
			{

				((Application *)qApp)->setUnmuteSpeaker(true);
				((Application *)qApp)->setGPH7(false);
				((Application *)qApp)->setGPE0(true);

				m_bExtensionUnitAnswer = false;
			}

            g_iIST_state = 0;
			break;
		}
    case NID_IST_END:
        {
            printf("NID_IST_END\n");

	    if (m_pTimeOutAutoHungupTimer->isActive())   // 当通话结束后需要结束因留言引起的自动挂断
		    m_pTimeOutAutoHungupTimer->stop();

             systemRunLog(3);

			if (isVisible())
			{
				m_bTalking = false;

				setDisplayWindow(false);

                hide();
                ((VoipMainDialog *)parent())->hide();

			ui->lineEditInfo->show();

			m_pFrameFullshow->hide();

			m_pFrameFullshow->lower();
		}

		m_bExtensionUnitAnswer = false;

		((Application *)qApp)->setLCDAndTimerOn();

		g_iIST_state = 0;

		sync();
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
描述：来电接听对话框事件过滤函数，实现播放远端视频窗口全屏/非全屏切换
参数：o - 产生事件的控件
	 e - 需处理的事件
返回：true - 事件处理成功，不需进一步处理
	 false - 需要进一步处理
************************************************************/
bool IstDialog::eventFilter(QObject *o, QEvent *e)
{
	if (e->type() == QEvent::MouseButtonPress)
	{
        if(m_pFilterTimer->isActive()){
            return false;
        }
        m_pFilterTimer->start(800);
        //david 2014/10/18
        /* david 2014/11/17
		setDisplayWindow(false);
        usleep(10000);
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
//extern Lvirdd *lvirdd;
int IstDialog::setDisplayWindow(bool bShow)
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
			rect = QRect(rect.x() /*+ 2*/, rect.y() /*+ 2*/, rect.width() - 1 /*- 4*/, rect.height() - 1/*- 4*/);
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

/*void IstDialog::on_pushButtonReturn_pressed()
{
	setDisplayWindow(false);
	usleep(120000);

	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	lpc_cmd_terminate(lphone, NULL);
	close();
}*/

/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void IstDialog::showEvent(QShowEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());

	pMainDlg->showRefreshTabFocus(this);

	//setAllButtonsFocusPolicy(this, Qt::NoFocus);
	ui->pushButtonIstAnswer->setFocus();
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void IstDialog::hideEvent(QHideEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());

	pMainDlg->hideRefreshTabFocus(this);

	if (pMainDlg->m_pCurActiveDlg == pMainDlg)
	{
		pMainDlg->ui->pushButtonVoip->setFocus(Qt::TabFocusReason);
	}
}

/************************************************************
描述：如果通话对方是门口机，向对方机发送开锁命令
参数：无
返回：无
************************************************************/
void IstDialog::on_pushButtonIstOpen_pressed()
{
	if ((m_iModalType & DOORWAY_MODAL_TYPE) ||
			(m_iModalType & IP_SWITCH_TYPE)  )
	{
		lpc_cmd_open(lphone, NULL);
	}
	else
    {
        //g_pMainDlg->m_pVoipMainDlg->authorizeElevator(m_strRemoteFullNetName);        
        GetPeerIPThread *pGetPeerIPThread = ((VoipMainDialog *)parent())->m_pGetPeerIPThread;
        AuthorizeElevatorThread *pAuthorizeElevatorThread = ((VoipMainDialog *)parent())->m_pAuthorizeElevatorThread;
        //g_pMainDlg->m_pVoipMainDlg->authorizeElevator(strDialRemoteName);
        if(m_strRemoteFullNetName.left(4) == pGetPeerIPThread->m_nativeName.left(4))
        {
            if(pAuthorizeElevatorThread->m_bRun)
            {
                pAuthorizeElevatorThread->m_bRun = false;
                pAuthorizeElevatorThread->quit();
                pAuthorizeElevatorThread->wait();
            }
            pAuthorizeElevatorThread->m_talkRemoteName = m_strRemoteFullNetName.left(m_strRemoteFullNetName.indexOf("?")) ;
            pAuthorizeElevatorThread->start();

        }

	}
}

/************************************************************
描述：按键处理函，用于测试使用
参数：无
按键数值显示：
C：0x43
9: 0x39
#: 0x2d
保/A: 0x1000030
*：0x2e
0: 0x30
上：0x1000013
下：0x1000015
开：0x1000031
免：?
************************************************************/
void IstDialog::keyPressEvent(QKeyEvent *k)
{
	int kv = k->key();
	qDebug("ist key value %x",kv);

	switch ( kv ) 
	{
		case 0x43:
			on_pushButtonIstAnswer_pressed();
			break;

		case 0x2d:
			on_pushButtonIstHangUp_pressed();
			break;

		case 0x1000031:
			on_pushButtonIstOpen_pressed();
			break;

		default:	
			break;
	}				/* -----  end switch  ----- */
}

/************************************************************
描述：当有门铃的时候，进行信息显示
参数：无
返回：无
************************************************************/
void IstDialog::Door_Bell_Ring_Event(QString& str)
{
    ui->lineEditInfo->setText(str);
    repaint();
}

/************************************************************
描述：从全屏状态退出
参数：无
返回：无
************************************************************/
void IstDialog::Exit_From_Full_Screen()
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
