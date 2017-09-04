#include "voipmaindialog.h"
#include "ui_voipmaindialog.h"
#include "lpcore.h"
#include "command.h"
#include "mediastreamer2/mediastream.h"
#include <QtGui>
#include "qconfig.h"
#include "data_types.h"
#include "maindialog.h"
#include "application.h"
#include "ui_maindialog.h"
#include "exosip_main.h"
#include "ui_mondialog.h"
#include "ui_ictrcvhookdialog.h"
#include "ui_securityplayerdialog.h"
#include "ui_securitymaindialog.h"
#include "ui_mediaplayerdialog.h"
#include "ui_mediamandialog.h"
#include "ui_securityalertdialog.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
extern bool g_Digital_Channel_Flag;				// 标识数字通道的变量
/************************************************************
  该文件为VoIP主对话框主程序
************************************************************/

extern MainDialog *g_pMainDlg;
extern LinphoneCore *lphone;
extern eXosip_element_t *eXosipElement;

extern char g_strNativeName[50];
extern char g_strLphoneName[3][50];

extern int g_bIstOrAlertBusy; //lg
extern int g_doorbell_on;

/************************************************************
描述：VoIP主对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
VoipMainDialog::VoipMainDialog(QWidget *parent) :
	QDialog(parent),
    ui(new Ui::VoipMainDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

	RESCALESCREEN(ui->pushButtonVoipMainMon)
	RESCALESCREEN(ui->pushButtonVoipMainDial)
	RESCALESCREEN(ui->pushButtonVoipMainReturn)

	//ui->pushButtonVoipMainReturn->setText("");
	//ui->pushButtonVoipMainDial->setText("");
	//ui->pushButtonVoipMainMon->setText("");

	ui->pushButtonVoipMainReturn->hide();
	ui->pushButtonVoipMainDial->hide();
	ui->pushButtonVoipMainMon->hide();

	m_pTimerOpen = new QTimer(this);
	connect(m_pTimerOpen, SIGNAL(timeout()), this, SLOT(openTimerTimeout()));

	m_bVideoOpen = true;
	m_nVolume = 1024;
	m_nRingVolume = 1024;

	m_pIctDlg = new IctDialog(this);
	m_pIstDlg = new IstDialog(this);
	m_pMonDlg = new MonDialog(this);
	//m_pMctDlg = new MctDialog(this);
	m_pMstDlg = new MstDialog(this);

	m_bCallCenter = false;

	m_pGetPeerIPThread = new GetPeerIPThread(this);
	//bcom
	/*
	   snprintf(configfile_name, PATHMAX, "linphonerc");
	   lphone = linphone_core_init (&linphonec_vtable, configfile_name);
	   QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
	   */
	//bcom
#ifdef ARM11
	system("/bin/echo 194 > /sys/class/gpio/export");
	system("/bin/echo out > /sys/class/gpio/gpio194/direction");
	system("/bin/echo 194 > /sys/class/gpio/unexport");
#endif
	//david
	m_pKeepWatchLiveThread = new KeepWatchLiveThread(this);

	m_pKeepWatchLiveThread->start();

	m_pSendImgThread = new SendImgThread(this);

	m_pCallInfoUploadThread = new CallInfoUploadThread(this);

    m_pAuthorizeElevatorThread = new AuthorizeElevatorThread(this);
}

/************************************************************
描述：VoIP主对话框析构函数
参数：无
返回：无
************************************************************/
VoipMainDialog::~VoipMainDialog()
{
	m_pGetPeerIPThread->m_bRun = false;
	m_pGetPeerIPThread->quit();
	m_pGetPeerIPThread->wait();
	delete m_pGetPeerIPThread;

	delete m_pIctDlg;
	delete m_pIstDlg;
	delete m_pMonDlg;
	//delete m_pMctDlg;
	delete m_pMstDlg;

	delete m_pTimerOpen;

	delete ui;
}

void VoipMainDialog::on_pushButtonVoipMainDial_pressed()
{
	m_pIctDlg->show();
	m_pIctDlg->ResetState("");
}

void VoipMainDialog::on_pushButtonVoipMainMon_pressed()
{
	//bcom m_pMctDlg->getLineEditDisplay()->setText("");
	m_pMonDlg->show();
	//m_pMctDlg->show();
	//m_pMctDlg->ResetState();
}

void VoipMainDialog::on_pushButtonVoipMainReturn_pressed()
{
	done(0);
}

void VoipMainDialog::openTimerTimeout()
{
	m_pTimerOpen->stop();

	system("/bin/echo 194 > /sys/class/gpio/export");
	//system("/bin/echo out > /sys/class/gpio/gpio194/direction");
	system("/bin/echo 1 > /sys/class/gpio/gpio194/value");
	system("/bin/echo 194 > /sys/class/gpio/unexport");

	printf("Lock open stop\n");
}

/************************************************************
描述：关闭可视对讲相关的所有对话框(如果当前正在可视对讲则强制退出)
参数：无
返回：无
************************************************************/
void VoipMainDialog::closeVoipDialog()
{
	if (isVisible())
	{
		if (m_pIctDlg->isVisible())
		{
			if (m_pIctDlg->m_pRecvHookDlg->isVisible())
			{
				//m_pIctDlg->m_pRecvHookDlg->on_pushButtonHangUp_pressed();
				usleep(1000000);//david 2014.6.19
                		m_pIctDlg->m_pRecvHookDlg->ui->pushButtonIctRcvHookHangUp->click();
			}
			else
			{
                		m_pIctDlg->on_pushButtonIctReturn_pressed();
			}

			usleep(300000);
		}

		if (m_pIstDlg->isVisible())
		{
            		m_pIstDlg->on_pushButtonIstHangUp_pressed();
			usleep(300000);
		}

		if (m_pMonDlg->isVisible())
		{
		    	//m_pMonDlg->on_pushButtonMonReturn_pressed();
		    	m_pMonDlg->ui->pushButtonMonReturn->click();//david 2014.6.19
			usleep(300000);
		}
	}

	if (g_pMainDlg->m_pCallCenterDlg->isVisible())
	{
        	g_pMainDlg->m_pCallCenterDlg->on_pushButtonCallCenterReturn_pressed();
		usleep(300000);
	}
}

/************************************************************
描述：关闭除可视对讲外的其它对话框(如果有安防报警录像或视频回放则强制
	 退出)
参数：无
返回：无
************************************************************/
void VoipMainDialog::closeOtherRelatedDialog()
{
	MainDialog *pMainDlg = (MainDialog *)parent();
	//david 2014.4.19
    if (pMainDlg->m_pPhotoMainDlg->m_pFrameFullshow->isVisible())
	{
        //pMainDlg->m_pPhotoMainDlg->m_pFrameFullshow->hide();
        //((Application *)qApp)->setLCDAndTimerOn();
        //lg 2014.12.22
        g_pMainDlg->m_pPhotoMainDlg->m_pTimer->stop();
        g_pMainDlg->m_pPhotoMainDlg->m_pFrameFullshow->hide();
        g_pMainDlg->m_pPhotoMainDlg->m_pFrameFullshow->lower();
        //lg
	}

    //lg 12.22
    if ( g_pMainDlg->m_pSysSetMainDlg->m_pRingAndVolumeDlg->m_pDoorBellRingFileSetDlg->isVisible() )
	    g_pMainDlg->m_pSysSetMainDlg->m_pRingAndVolumeDlg->m_pDoorBellRingFileSetDlg->Remote_Call_Return();

    if (g_pMainDlg->m_pSysSetMainDlg->isVisible())
    {
        g_pMainDlg->m_pSysSetMainDlg->on_pushButtonSysSetMainVersion_pressed();
    }

    if (g_pMainDlg->m_pSysSetEngineerDlg->isVisible())
    {
        g_pMainDlg->m_pSysSetEngineerDlg->on_pushButtonSysSetEngineerNetAddress_pressed();
    }

    if (g_pMainDlg->m_pPhotoMainDlg->m_pPhotoUDiskDlg->isVisible())
    {
        g_pMainDlg->m_pPhotoMainDlg->m_pPhotoUDiskDlg->on_pushButtonPhotoUDiskReturn_pressed();
    }

    if (g_pMainDlg->m_pAddressBookDlg->isVisible())
    {
        g_pMainDlg->m_pAddressBookDlg->on_pushButtonAddressBookCancel_pressed();
    }
    //lg

    /*if(pMainDlg->m_pPhotoMainDlg->isVisible())
	if (pMainDlg->m_pHelpDlg->isVisible())
	{
		pMainDlg->m_pHelpDlg->hide();
    }*/

	if (pMainDlg->m_pDoorBellDlg->isVisible())
	{
        pMainDlg->m_pDoorBellDlg->on_pushButtonDoorBellReturn_pressed();
	}

	if (m_pMonDlg->isVisible())
	{
        m_pMonDlg->on_pushButtonMonReturn_pressed();
	}

	if (pMainDlg->m_pMediaManDlg->m_pMediaPlayerDlg->isVisible())
	{
		//pMainDlg->m_pMediaManDlg->m_pMediaPlayerDlg->stopPlayer();
        pMainDlg->m_pMediaManDlg->m_pMediaPlayerDlg->ui->pushButtonMediaPlayerReturn->click();

		usleep(1000);

		if(pMainDlg->m_pMediaManDlg->isVisible()){
            pMainDlg->m_pMediaManDlg->ui->pushButtonMediaManReturn->click();
		}

	}
	//bug
	pMainDlg->m_pMediaManDlg->m_pMediaPlayerDlg->hide();

	if (pMainDlg->m_pInfoManMainDlg->isVisible() && pMainDlg->m_pInfoManMainDlg->m_pInfoManDetailDlg->isVisible())
	{
		if (pMainDlg->m_pInfoManMainDlg->m_pInfoManDetailDlg->m_pViewAttachemntDlg->isVisible())
		{
			pMainDlg->m_pInfoManMainDlg->m_pInfoManDetailDlg->m_pViewAttachemntDlg->hide();
		}
	}

	if (pMainDlg->m_pSecurityMainDlg->m_pSecurityAlertDlg->isVisible())
	{
		//pMainDlg->m_pSecurityMainDlg->m_pSecurityAlertDlg->stopRecorder();
        pMainDlg->m_pSecurityMainDlg->m_pSecurityAlertDlg->ui->pushButtonSecAlertReturn->click();//david 2014.6.19
        pMainDlg->m_pSecurityMainDlg->m_pSecurityAlertDlg->hide();
        pMainDlg->m_pSecurityMainDlg->hide();
	}


	if (pMainDlg->m_pSecurityMainDlg->m_pSecurityPlayerDlg->isVisible())
	{
		//     pMainDlg->m_pSecurityMainDlg->m_pSecurityPlayerDlg->stopPlayer();

        pMainDlg->m_pSecurityMainDlg->m_pSecurityPlayerDlg->ui->pushButtonSecPlayerReturn->click();//david 2014.6.19
        pMainDlg->m_pSecurityMainDlg->ui->pushButtonSecMainReturn->click();//david 2014.6.19
		/*
		   pMainDlg->m_pSecurityMainDlg->m_pSecurityPlayerDlg->stopPlayer();
		   pMainDlg->m_pSecurityMainDlg->hide();
		   pMainDlg->m_pSecurityMainDlg->m_pSecurityPlayerDlg->done(0);*/
	}

	//lg
	if (pMainDlg->m_pInputMethodWidget)
	{
		pMainDlg->m_pInputMethodWidget->hide();
	}
	//lg
    if(pMainDlg->m_pHomeControlDlg->isVisible())
    {
        pMainDlg->m_pHomeControlDlg->hide();
    }
}

/************************************************************
描述：VoIP和视频监控事件处理函数，实现lphone库ICT/IST/MCT上发消息的
	 处理。如果收到IST的NID_IDLE_RCVINV，如果对方是管理机，则打开接
	 听管理机的对话框，否则打开接听普通来电的对话框
参数：event - 需处理的事件
返回：true - 事件处理成功
	 false - 事件处理失败
************************************************************/
bool VoipMainDialog::event(QEvent *event)
{
	CallCenterDialog *pCallCenterDlg = ((MainDialog *)parent())->m_pCallCenterDlg;

	event->accept();

	switch (event->type() - 5000)
	{
	case NID_IDLE_RCVINV:
		{
			MainDialog *pMainDlg = (MainDialog *)parent();

			if (g_doorbell_on || pMainDlg->m_pCalibrationDlg->isVisible())
			{
				lpc_cmd_terminate(lphone, NULL);
				return true;
			}

			m_bCallCenter = false;

			QString message = (dynamic_cast <Levent *> (event))->msg;
			int i = 0;

            if (!pMainDlg->m_bInterFree)
            {
                my_ring_set_level(m_nRingVolume);
            }
            else
            {
                my_ring_set_level(0);
            }
			closeOtherRelatedDialog();

            //david 2014.6.27
            extern char g_strLphoneRingFile[5][50];
            extern int b_ring_play_run;

            if(!b_ring_play_run)
            {
                my_ring_start(g_strLphoneRingFile[1], 2000);
             }

			g_bIstOrAlertBusy = 0; //lg

			QString strTemp = message;
			int iModalType = BASIC_MODAL_TYPE;
			int idx = strTemp.indexOf("?");
			if (idx != -1)
			{
				iModalType = strTemp.mid(idx + 1).toInt();
				strTemp = strTemp.left(idx);
			}

			if ((strTemp != "") && (iModalType == MANCENTER_MODAL_TYPE))
			{
				for (i = 0; i < 1/*2*/; i++)
				{
					printf("\nlg %s %s\n", strTemp.toLocal8Bit().data(), g_strLphoneName[i + 1]);
					if (strTemp == g_strLphoneName[i + 1])
					{
						m_pIstDlg->hide();
						hide();

						PostEventWithMessage(pCallCenterDlg, event->type(), message);

						m_bCallCenter = true;

						break;
					}
				}
			}

            if (!m_bCallCenter)
			{
				pCallCenterDlg->hide();

				PostEventWithMessage(m_pIstDlg, event->type(), message);
			}
			break;
		}
	case	NID_IDLE_RCVMON:
		{
			closeOtherRelatedDialog();

			QString message = (dynamic_cast <Levent *> (event))->msg;
			PostEventWithMessage(m_pMstDlg, event->type(), message);
			break;
		}
	case	NID_IDLE_FAILURE:
	case	NID_ICT_NONE:
	case	NID_ICT_TIMEOUT:
	//lg
	case	NID_ICT_OPENTERM:
	//lg
	case	NID_ICT_RCVBUSY:
	case	NID_ICT_CONNECTED:
	case	NID_ICT_NOANSWER:
	case	NID_ICT_RCVCANCEL:
	case	NID_ICT_TALKING:
	case	NID_ICT_RCVVIDEOOPEN:
	case	NID_ICT_RCVVIDEOCLOSE:
	case	NID_ICT_END:
		{

			// for ictWindowv
			QString message = (dynamic_cast <Levent *> (event))->msg;

			if (m_bCallCenter)
			{
				PostEventWithMessage(pCallCenterDlg, event->type(), message);
			}
			else
            {
				PostEventWithMessage(m_pIctDlg, event->type(), message);
			}

			break;
		}
	case	NID_ICT_RECORDING:
		{
            break;
		}
	case	NID_IST_NONE:
	case	NID_IST_TIMEOUT:
	//lg
	case	NID_IST_OPENTERM:
	case	NID_IST_RCVVIDEOOPEN:
	case	NID_IST_RCVVIDEOCLOSE:
	//lg
	case	NID_IST_RCVCANCEL:
	case	NID_IST_TALKING:
	case	NID_IST_END:
		{
            printf("\tNID_IST_END %d\n",m_bCallCenter);
            printf("event type :%d\n",event->type() - 5000);
			QString message = (dynamic_cast <Levent *> (event))->msg;

			if (m_bCallCenter)
			{
				PostEventWithMessage(pCallCenterDlg, event->type(), message);
			}
			else
			{
				PostEventWithMessage(m_pIstDlg, event->type(), message);
            }

			break;
		}
	case	NID_IST_RECORDING:
		{
			if (eXosipElement->audiostream != NULL)
			{
				audio_stream_set_level(eXosipElement->audiostream, 0);
			}

			QString message = (dynamic_cast <Levent *> (event))->msg;

			if (m_bCallCenter)
			{
				PostEventWithMessage(pCallCenterDlg, event->type(), message);
			}
			else
			{
				PostEventWithMessage(m_pIstDlg, event->type(), message);
			}

			break;
		}
	case	NID_MCT_NONE:
	case	NID_MCT_TIMEOUT:
	case	NID_MCT_RCVBUSY:
	case	NID_MCT_CONNECTED:
	case	NID_MCT_RCVCANCEL:
	case	NID_MCT_OPENTERM:
	case	NID_MCT_END:
		{
			QString message = (dynamic_cast <Levent *> (event))->msg;
			PostEventWithMessage(m_pMonDlg/*m_pMctDlg*/, event->type(), message);
			break;
		}
	case	NID_MST_NONE:
	case	NID_MST_TIMEOUT:
	case	NID_MST_RCVCANCEL:
	case	NID_MST_END:
		{
			QString message = (dynamic_cast <Levent *> (event))->msg;
			PostEventWithMessage(m_pMstDlg, event->type(), message);
			break;
		}

	case	NID_ICT_RCVOPEN:
	case	NID_IST_RCVOPEN:
	case	NID_MST_RCVOPEN:
		{
			system("/bin/echo 194 > /sys/class/gpio/export");
			//system("/bin/echo out > /sys/class/gpio/gpio194/direction");
			system("/bin/echo 0 > /sys/class/gpio/gpio194/value");
			system("/bin/echo 194 > /sys/class/gpio/unexport");

			printf("Lock open start\n");

			m_pTimerOpen->start(500);

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
描述：窗口显示时的回调函数
参数：无
返回：无
************************************************************/
void VoipMainDialog::showEvent(QShowEvent *)
{
    g_pMainDlg->showRefreshTabFocus(this);

	ui->pushButtonVoipMainDial->setFocusPolicy(Qt::NoFocus);
	ui->pushButtonVoipMainMon->setFocusPolicy(Qt::NoFocus);
	ui->pushButtonVoipMainReturn->setFocusPolicy(Qt::NoFocus);

	//pMainDlg->refreshTabFocus();
}

/************************************************************
描述：窗口隐藏时的回调函数
参数：无
返回：无
************************************************************/
void VoipMainDialog::hideEvent(QHideEvent *)
{
	//MainDialog *pMainDlg = (MainDialog *)parent();

	m_pIctDlg->hide();
	m_pMonDlg->hide();

    	g_pMainDlg->hideRefreshTabFocus(this);

	/*if (pMainDlg->m_pCurActiveDlg == pMainDlg)
	{
		if (m_pIctDlg->isVisible() || m_pIstDlg->isVisible())
		{
			pMainDlg->ui->pushButtonVoip->setFocus(Qt::TabFocusReason);
		}
		else if (m_pMonDlg->isVisible() || m_pMstDlg->isVisible())
		{
			//printf("lg 1\n");
			pMainDlg->ui->pushButtonVideoMon->setFocus(Qt::TabFocusReason);
		}
	}*/
}

void VoipMainDialog::authorizeElevator(QString strRemoteName)
{
	char strPeerIP[200] = "192.168.1.107?4";
	int ret = 0;
    int iDestFloor = strRemoteName.mid(5,4).toInt();
    QString NativeUnitName;
	GetPeerIPThread *pGetPeerIPThread = m_pGetPeerIPThread;

    NativeUnitName = pGetPeerIPThread->m_nativeName.left(4);    
    if(NativeUnitName != strRemoteName.left(4))
    {
        return ;
    }

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
			if (g_pMainDlg->m_inputMethodMode == 0)
			{
				if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
					usleep(50000);;
				voip_playWaveFile((char *)"wav/authorize_elevator_failed-en.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
			}
			else
			{
				if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
					usleep(50000);;
				voip_playWaveFile((char *)"wav/authorize_elevator_failed.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
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
		if (g_pMainDlg->m_inputMethodMode == 0)
		{
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
			voip_playWaveFile("wav/authorize_elevator_failed-en.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
		}
		else
		{
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
			voip_playWaveFile("wav/authorize_elevator_failed.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
		}

		return;
	}

	QTcpSocket socket;

	socket.connectToHost(ptr, 1208);

	if (!socket.waitForConnected(750))
	{
		if (g_pMainDlg->m_inputMethodMode == 0)
		{
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
			voip_playWaveFile("wav/authorize_elevator_failed-en.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
		}
		else
		{
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
			voip_playWaveFile("wav/authorize_elevator_failed.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
		}

		return;
	}

	QString strNativeName(g_strNativeName);
	QString strDoorwayName(g_strLphoneName[0]);
	int pos1 = strNativeName.indexOf("-");
	int pos2 = strDoorwayName.indexOf("-");

    //david 2014.4.4
    if(strNativeName.count() > 9)

        strNativeName = strNativeName.left(9);

	if ((pos1 >= 0) && (pos2 >= 0) && (strNativeName.left(pos1) == strDoorwayName.left(pos2)))
	{
        strNativeName = strNativeName.mid(pos1 + 1);
	}

    pos1 = strNativeName.indexOf("-");
    if (pos1 >= 0) strNativeName = strNativeName.right(pos1);//david 2014.4.12 left(pos1);

	int iFloor = strNativeName.left(2).toInt();  
	char szBuf[13];
	memset(szBuf, 0, sizeof(szBuf));
    szBuf[0] = 0xf7;
    szBuf[6] = (int)iDestFloor%100;
    szBuf[7] = (int)iDestFloor/100;
    szBuf[10] = (char)strNativeName.mid(2,2).toInt();
	szBuf[11] = (char)iFloor;
    szBuf[12] = 0x96;

	int sum = 0;
	int i;
	for (i = 2; i < 13; i++)
	{
		sum += szBuf[i];
	}
	szBuf[1] = (-sum) & 0xff;

	socket.write((const char *)szBuf, sizeof(szBuf));
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
		if (g_pMainDlg->m_inputMethodMode == 0)
		{
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
			voip_playWaveFile("wav/authorize_elevator_ok-en.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
		}
		else
		{
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
			voip_playWaveFile("wav/authorize_elevator_ok.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
		}
		//hide();
	}
	else
	{
		if (g_pMainDlg->m_inputMethodMode == 0)
		{
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
			voip_playWaveFile("wav/authorize_elevator_failed-en.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
		}
		else
		{
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
			voip_playWaveFile("wav/authorize_elevator_failed.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
		}
    }
}

/************************************************************
描述：处理通话时候的门铃时间
参数：无
返回：如果正处于通话状态，就返回true 否则返回false
************************************************************/
bool VoipMainDialog::Do_Door_Bell_Event(QString& str)
{
	if (isVisible())
	{
		if (m_pIctDlg->isVisible())
		{
			if ( m_pIctDlg->m_pRecvHookDlg->isVisible() )
			{
				m_pIctDlg->m_pRecvHookDlg->Exit_From_Full_Screen();
				m_pIctDlg->m_pRecvHookDlg->ResetState(str);
				m_pIctDlg->m_pRecvHookDlg->repaint();
				return true;
			}
		}
		if (m_pIstDlg->isVisible())
		{
			m_pIstDlg->Exit_From_Full_Screen();
			m_pIstDlg->Door_Bell_Ring_Event(str);
			return true;
		}
	}

	if (g_pMainDlg->m_pCallCenterDlg->isVisible() && (g_pMainDlg->m_pCallCenterDlg->m_bIctTalking == true || g_pMainDlg->m_pCallCenterDlg->m_bIstTalking == true ) )    //  如果是呼叫中心
	{
		g_pMainDlg->m_pCallCenterDlg->Exit_From_Full_Screen();
		g_pMainDlg->m_pCallCenterDlg->Door_Bell_Ring_Event(str);
		return true;
	}

	return false;
}
