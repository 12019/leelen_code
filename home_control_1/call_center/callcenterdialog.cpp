#include "callcenterdialog.h"
#include "ui_callcenterdialog.h"
#include <QStringListModel>
#include "maindialog.h"
#include "voipmaindialog.h"
#include "exosip_main.h"
#include "lpcore.h"
#include "command.h"
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/mman.h>
//#include "terminalhelpmaindialog.h"
//#include "helpdialog.h"
#include "application.h"
#include "ui_maindialog.h"
#include "core/leelen_comdef.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
/************************************************************
  该文件为呼叫中心主程序，包括拨打管理中心和接听管理中心来电的功能
************************************************************/
extern eXosip_t eXosip;
extern LinphoneCore *lphone;
extern eXosip_element_t *eXosipElement;
extern char g_strLphoneName[3][50];
extern MainDialog *g_pMainDlg;
extern RESULT_E g_check_computercenter_udp_state;
extern char g_strDialRemoteName[50];

//#define PLAY_AREA_BGCOLOR QColor(0, 0, 0)

/************************************************************
用该静态变量存储拨打管理中心和接听管理中心来电的对话框指针
************************************************************/
CallCenterDialog *CallCenterDialog::m_pDlg = NULL;

/************************************************************
描述：呼叫中心对话框构造函数
参数：parent - 该对话框的父窗口指针
返回：无
************************************************************/
CallCenterDialog::CallCenterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CallCenterDialog)
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

	CallCenterDialog::m_pDlg = this;

	/*QStringListModel *model = new QStringListModel();
	  ui->listViewCenterAddress->setModel(model);
	  ui->listViewCenterAddress->setSelectionMode(QListView::SingleSelection);
	  ui->listViewCenterAddress->setEditTriggers(QListView::NoEditTriggers);
	  */
    //ui->pushButtonCallCenterHangUp->hide();

    ui->pushButtonCallCenterAnswer->hide();
    ui->pushButtonCallCenterVideoOpen->hide();

	m_strRemoteIp[0] = '\0';

	m_bIctState   = true;
	m_bIctTalking = false;
	m_bIstTalking = false;

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
	ui->lineEditInfo->setGeometry(x,y,w,h);

    int res = g_pScaleSrc->get_screen_version();
    if ( res == 3 )  // 1280x800
    {
        ui->pushButtonCallCenterAnswer->setGeometry(QRect(959, 245, 279, 99));
        ui->pushButtonCallCenterDial->setGeometry(QRect(959, 245, 279, 99));
        ui->pushButtonCallCenterHangUp->setGeometry(QRect(959, 353, 279, 99));
        ui->pushButtonCallCenterVideoClose->setGeometry(QRect(959, 461, 279, 99));
        ui->pushButtonCallCenterVideoOpen->setGeometry(QRect(959, 461, 279, 99));
        ui->pushButtonCallCenterRecord->setGeometry(QRect(959, 569, 279, 99));
        ui->pushButtonCallCenterReturn->setGeometry(QRect(959, 677, 279, 99));
    }
    else if (res == 2) // 1024x600
    {
        ui->pushButtonCallCenterAnswer->setGeometry(QRect(764, 180, 230, 80));
        ui->pushButtonCallCenterDial->setGeometry(QRect(764, 180, 230, 80));
        ui->pushButtonCallCenterHangUp->setGeometry(QRect(764, 260, 230, 80));
        ui->pushButtonCallCenterVideoClose->setGeometry(QRect(764, 340, 230, 80));
        ui->pushButtonCallCenterVideoOpen->setGeometry(QRect(764, 340, 230, 80));
        ui->pushButtonCallCenterRecord->setGeometry(QRect(764, 420, 230, 80));
        ui->pushButtonCallCenterReturn->setGeometry(QRect(764, 500, 230, 80));
    }
}

/************************************************************
描述：呼叫中心对话框析构函数
参数：无
返回：无
************************************************************/
CallCenterDialog::~CallCenterDialog()
{
	delete m_pFrameFullshow;
	delete ui;
}

/************************************************************
描述：获取呼叫中心地址列表控件
参数：无
返回：呼叫中心地址列表控件的指针
************************************************************/
/*QListView *CallCenterDialog::getListViewCenterAddress()
{
	return ui->listViewCenterAddress;
}
*/

/************************************************************
描述：呼叫中心对话框初始化
参数：bInitVolume - true: 根据VoipMainDialog中m_nVolume设置呼叫
						 中心对话框音量控件的值
				   false: 不设置呼叫中心对话框音量控件的值
返回：无
************************************************************/
void CallCenterDialog::ResetState()//bool bInitVolume)
{
	VoipMainDialog *pVoipMainDlg = ((MainDialog *)parent())->m_pVoipMainDlg;

	m_bIctState = true;
	m_bIctTalking = false;
	m_bIstTalking = false;

	ui->lineEditInfo->show();
	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	if (pVoipMainDlg->m_bVideoOpen)
	{
        ui->pushButtonCallCenterVideoOpen->hide();
        ui->pushButtonCallCenterVideoClose->show();
	}
	else
	{
        ui->pushButtonCallCenterVideoOpen->show();
        ui->pushButtonCallCenterVideoClose->hide();
	}

	if (m_CallCenterName[0] != "")
	{
		ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0]);
	}
	else
	{
		ui->lineEditInfo->setText(tr("呼叫中心"));
	}

    ui->pushButtonCallCenterDial->show();
    ui->pushButtonCallCenterDial->setEnabled(m_CallCenterName[0] != "");
    ui->pushButtonCallCenterAnswer->hide();
    //ui->pushButtonCallCenterAnswer->setEnabled(true);
	if (!pVoipMainDlg->m_pMonDlg->isVisible())
	{
        ui->pushButtonCallCenterDial->setFocus(Qt::TabFocusReason);
	}
    ui->pushButtonCallCenterRecord->setEnabled(false);
    ui->pushButtonCallCenterHangUp->setEnabled(false);
	/*QRect rc;
    rc = ui->pushButtonCallCenterVideoOpen->geometry();
    rc.moveLeft(ui->pushButtonCallCenterRecord->geometry().x());
    ui->pushButtonCallCenterVideoOpen->setGeometry(rc);
    ui->pushButtonCallCenterVideoClose->setGeometry(rc);
	*/
	/*if (bInitVolume)
	{
		ui->horizontalSliderVolume->setValue((int)(pVoipMainDlg->m_nVolume * 60.0/1024.0));
	}
	*/

	/*QPalette palette;
	palette.setBrush(ui->frameVideoShow->backgroundRole(), QBrush(PLAY_AREA_BGCOLOR));
	ui->frameVideoShow->setPalette(palette);
	ui->frameVideoShow->setAutoFillBackground(true);
	*/
}

/************************************************************
描述：设置两个呼叫中心地址名称
参数：addr1 - 呼叫中心机的地址名称
	 addr2 - 呼叫中心计算机的地址名称
返回：无
************************************************************/
void CallCenterDialog::setCallCenterName(QString addr1, QString addr2)
{
	/*QModelIndex index;
	QStringList addresses;
	QStringListModel *model = (QStringListModel *)ui->listViewCenterAddress->model();
	*/
	m_CallCenterName[0] = addr1;
	m_CallCenterName[1] = addr2;

	/*ui->listViewCenterAddress->reset();

	addresses.append(m_CallCenterName[0]);
	addresses.append(m_CallCenterName[1]);
	model->setStringList(addresses);

	index = model->index(0, 0);
	ui->listViewCenterAddress->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
	*/
}

/************************************************************
描述：呼叫中心拨号按钮回调函数，实现对呼叫中心拨号
参数：无
返回：无
************************************************************/
void CallCenterDialog::on_pushButtonCallCenterDial_pressed()
{
    if (!ui->pushButtonCallCenterDial->isEnabled()) return;

	QString strName = m_CallCenterName[0];////row];

	strcpy(g_strDialRemoteName,strName.toLocal8Bit().data());

	if (strName != "")
	{
		char strPeerIP[200] = "192.168.1.107?16";
		int ret = 0;
		GetPeerIPThread *pGetPeerIPThread = ((MainDialog *)parent())->m_pVoipMainDlg->m_pGetPeerIPThread;

		ret = pGetPeerIPThread->GetPeerIP(strName.toLatin1().data(), strPeerIP);

		//printf("lg call center lpc_cmd_call:%s\n", strPeerIP);

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

			if (0)//iModalType != MANCENTER_MODAL_TYPE)
			{
				setAllButtonsEnabled(this, false);
				ui->lineEditInfo->setFocus(Qt::TabFocusReason);
				MyMessageBox messBox(this);
				CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
				okButton->setFocus(Qt::TabFocusReason);
				messBox.setText(tr("对方不是管理中心机"));
				messBox.exec();
				setAllButtonsEnabled(this, true);
                ui->pushButtonCallCenterHangUp->setEnabled(false);
                ui->pushButtonCallCenterRecord->setEnabled(false);
                ui->pushButtonCallCenterDial->setFocus(Qt::TabFocusReason);

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
			((Application *)qApp)->setLCDOn();

			((MainDialog *)parent())->m_pVoipMainDlg->m_bCallCenter = true;

			if(eXosipElement->state != IST_IDLE)

				eXosipElement->state = IDLE; //lg bug

			if (lpc_cmd_call(lphone, ptr) != 1)//strPeerIP);
			{
				setAllButtonsEnabled(this, false);
				ui->lineEditInfo->setFocus(Qt::TabFocusReason);
				MyMessageBox messBox(this);
				CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
				okButton->setFocus(Qt::TabFocusReason);
				messBox.setText(tr("呼叫发送失败"));
				messBox.exec();
				setAllButtonsEnabled(this, true);
                ui->pushButtonCallCenterHangUp->setEnabled(false);
                ui->pushButtonCallCenterRecord->setEnabled(false);
                ui->pushButtonCallCenterDial->setFocus(Qt::TabFocusReason);

				return;
			}

			usleep(1000);

			setGM7113Channel(0);

			ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0] + tr("呼叫中..."));

			strcpy(m_strRemoteIp, ptr);

            ui->pushButtonCallCenterDial->setEnabled(false);
			ui->lineEditInfo->setFocus(Qt::TabFocusReason);
            //ui->pushButtonCallCenterHangUp->show();
		}
		else
		{
			setAllButtonsEnabled(this, false);
			ui->lineEditInfo->setFocus(Qt::TabFocusReason);
			MyMessageBox messBox(this);
			CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
			okButton->setFocus(Qt::TabFocusReason);
			messBox.setText(tr("获取对方IP地址失败"));
			messBox.exec();
			setAllButtonsEnabled(this, true);
            ui->pushButtonCallCenterHangUp->setEnabled(false);
            ui->pushButtonCallCenterRecord->setEnabled(false);
            ui->pushButtonCallCenterDial->setFocus(Qt::TabFocusReason);
		}
	}
}

/************************************************************
描述：呼叫中心接听来电按钮回调函数，实现呼叫中心来电接听
参数：无
返回：无
************************************************************/
void CallCenterDialog::on_pushButtonCallCenterAnswer_pressed()
{
    if (!ui->pushButtonCallCenterAnswer->isEnabled()) return;

	lpc_cmd_answer(lphone, NULL);

    ui->pushButtonCallCenterAnswer->setEnabled(false);
	ui->lineEditInfo->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：呼叫中心挂断按钮回调函数，实现挂断与呼叫中心的通话
参数：无
返回：无
************************************************************/
void CallCenterDialog::on_pushButtonCallCenterHangUp_pressed()
{
    if (ui->pushButtonCallCenterAnswer->isVisible())
	{
		if (!isVisible()) return;
	}

	setDisplayWindow(false);

#ifdef ARM11
	usleep(120000);
#endif

#ifdef CORTEXA7
	usleep(530000);
#endif
	ui->lineEditInfo->show();
	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	/*QPalette palette;
	palette.setBrush(ui->frameVideoShow->backgroundRole(), QBrush(PLAY_AREA_BGCOLOR));
	ui->frameVideoShow->setPalette(palette);
	ui->frameVideoShow->setAutoFillBackground(true);
	*/
	ui->frameVideoShow->repaint();
	lpc_cmd_terminate(lphone, NULL);

    if (ui->pushButtonCallCenterAnswer->isVisible())
	{
		hide();
	}
	//ResetState();

	((Application *)qApp)->setLCDAndTimerOn();
}

/************************************************************
描述：呼叫中心返回按钮回调函数，实现挂断与呼叫中心的通话然后关闭呼叫中
	 心对话框
参数：无
返回：无
************************************************************/
void CallCenterDialog::on_pushButtonCallCenterReturn_pressed()
{
	if (!isVisible()) return;

	setDisplayWindow(false);
	usleep(120000);

	ui->lineEditInfo->show();
	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	/*QPalette palette;
	palette.setBrush(ui->frameVideoShow->backgroundRole(), QBrush(PLAY_AREA_BGCOLOR));
	ui->frameVideoShow->setPalette(palette);
	ui->frameVideoShow->setAutoFillBackground(true);
	*/
	ui->frameVideoShow->repaint();

	lpc_cmd_terminate(lphone, NULL);
	hide();

	//ResetState();

	((Application *)qApp)->setLCDAndTimerOn();
}

/************************************************************
描述：视频打开按钮回调函数，实现打开本地摄像头的视频（对方可看这边图像）
参数：无
返回：无
************************************************************/
void CallCenterDialog::on_pushButtonCallCenterVideoOpen_pressed()
{
    if (!ui->pushButtonCallCenterVideoOpen->isVisible()) return;

	((MainDialog *)parent())->m_pVoipMainDlg->m_bVideoOpen = true;

	/*QString strName;
	int row = ui->listViewCenterAddress->currentIndex().row();
	if (row != -1)
	{
		strName = m_CallCenterName[row];
	}*/

	if ((m_bIctState || m_bIstTalking) /*&& (strName != "")
		&& (strName != g_strLphoneName[0])*/
        /*&& (eXosipElement->videostream != NULL)*/)
	{
        if(eXosip.callflag == 0)
        {
            if(eXosipElement->videostream != NULL)
            {
                lpc_cmd_video(lphone, (char *)"open");
                video_stream_unmute_send(eXosipElement->videostream);
            }
        }
        else
        {
            bool bTmp = false;
            for (int i = 0; i < MAX_EXTENSION_MACHINE_CNT; i++) {
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

    ui->pushButtonCallCenterVideoOpen->hide();
    ui->pushButtonCallCenterVideoClose->show();
    ui->pushButtonCallCenterVideoClose->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：视频关闭按钮回调函数，实现关闭本地摄像头的视频（对方不可看这边图
	 像）
参数：无
返回：无
************************************************************/
void CallCenterDialog::on_pushButtonCallCenterVideoClose_pressed()
{
    if (!ui->pushButtonCallCenterVideoClose->isVisible()) return;

	((MainDialog *)parent())->m_pVoipMainDlg->m_bVideoOpen = false;

	if ((m_bIctState || m_bIstTalking) /*&& (eXosipElement->videostream != NULL)*/)
	{
		if(eXosip.callflag == 0)
		{
			if(eXosipElement->videostream != NULL)
			{
				lpc_cmd_video(lphone, (char *)"close");
				video_stream_mute_send(eXosipElement->videostream);
			}
		}
		else
		{
			bool bTmp = false;
			for (int i = 0; i < MAX_EXTENSION_MACHINE_CNT; i++) {
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

	}

    ui->pushButtonCallCenterVideoOpen->show();
    ui->pushButtonCallCenterVideoClose->hide();
    ui->pushButtonCallCenterVideoOpen->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：呼叫中心窗口事件处理函数，实现lphone库ICT/IST上发消息的处理
参数：event - 需处理的事件
返回：true - 事件处理成功
	 false - 事件处理失败
************************************************************/
bool CallCenterDialog::event(QEvent *event)
{
	switch (event->type() - 5000)
	{
		case NID_ICT_NONE:
			{
				ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0]);
				break;
			}
		case	NID_ICT_TIMEOUT:
			{
				setDisplayWindow(false);
				usleep(100000);

				ResetState();
				ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0] + tr("呼叫超时"));
				break;
			}
		case	NID_ICT_OPENTERM:
			{
				ResetState();
				ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0] + tr("开锁退出"));
				break;
			}
		case	NID_ICT_RCVBUSY:
			{
				ResetState();
				ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0] + tr("呼叫忙"));
				break;
			}
		case	NID_ICT_CONNECTED:
			{
				((Application *)qApp)->setLCDOn();
				((Application *)qApp)->setOnCamera(true);

				my_ring_set_level(((MainDialog *)parent())->m_pVoipMainDlg->m_nRingVolume);

				/*QString strName;
				  int row = ui->listViewCenterAddress->currentIndex().row();
				  if (row != -1)
				  {
				  strName = m_CallCenterName[row];
				  }

				  if ((strName != "") && (strName == g_strLphoneName[0])) //doorway machine
				  {
				  if (eXosipElement->videostream != NULL)
				  {
				  video_stream_mute_send(eXosipElement->videostream);
				  }
				  }
				  else
				  {*/
                bool bVideoOn = ((MainDialog *)parent())->m_pVoipMainDlg->m_bVideoOpen;
                if (eXosip.callflag == 0)
                {
                    if (eXosipElement->videostream != NULL)
                    {
                        if (!bVideoOn)
                        {
                            video_stream_mute_send(eXosipElement->videostream);
                        }
                        else
                        {
                            video_stream_unmute_send(eXosipElement->videostream);
                        }
                    }
                }
                else
                {
                    QString strRemoteIp = (dynamic_cast <Levent *> (event))->msg;
                    int i;
                    for (i = 0; i < MAX_EXTENSION_MACHINE_CNT; i++) {
                        if (strRemoteIp == eXosip.remip[i].rem_ip /*lphone->video_params.remoteaddr) == 0*/) {
                            if (eXosip.remip[i].videostream != NULL) {
                                if (!bVideoOn)
                                {
                                    video_stream_mute_send(eXosip.remip[i].videostream);
                                }
                                else
                                {
                                    video_stream_unmute_send(eXosip.remip[i].videostream);
                                }
                            }
                        }
                    }
                }

                ui->pushButtonCallCenterHangUp->setEnabled(true);
                ui->pushButtonCallCenterRecord->setEnabled(true);

				ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0] + tr("已连接"));

				CallInfoUploadThread *pCallInfoUploadThread = ((MainDialog *)parent())->m_pVoipMainDlg->m_pCallInfoUploadThread;
                if(g_check_computercenter_udp_state == RESULT_NULL)
                {                  
                    if( pCallInfoUploadThread->m_bRun == false)
                    {
                        pCallInfoUploadThread->m_upInfoType = 1;
                        pCallInfoUploadThread->getCallCode(QString(g_strLphoneName[1]));
                        pCallInfoUploadThread->start();
                    }
                }

				break;
			}

		case	NID_ICT_NOANSWER:
			{
				ResetState();
				ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0] + tr("无应答"));
				break;
			}
		case	NID_ICT_RCVCANCEL:
			{
				ResetState();
				ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0] + tr("接收取消"));
				break;
			}
		case	NID_ICT_RCVOPEN:
			{
				ResetState();
				ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0] + tr("接收打开"));
				break;
			}
		case	NID_ICT_TALKING:
			{
				((Application *)qApp)->setUnmuteMic(true);

				m_bIctTalking = true;

                ui->pushButtonCallCenterRecord->setEnabled(false);

				//printf("lg call center 1\n");
				if (eXosipElement->audiostream != NULL)
				{
					audio_stream_set_level(eXosipElement->audiostream, ((MainDialog *)parent())->m_pVoipMainDlg->m_nVolume);
				}

				/*QString strName;
				  int row = ui->listViewCenterAddress->currentIndex().row();
				  if (row != -1)
				  {
				  strName = m_CallCenterName[row];
				  }

				  if ((strName != "") && (strName == g_strLphoneName[0])) //doorway machine
				  {
				  if (eXosipElement->videostream != NULL)
				  {
				  video_stream_mute_send(eXosipElement->videostream);
				  }
				  }
				  else
				  {*/
                bool bVideoOn = ((MainDialog *)parent())->m_pVoipMainDlg->m_bVideoOpen;
                if (eXosip.callflag == 0)
                {
                    if (eXosipElement->videostream != NULL)
                    {
                        if (!bVideoOn)
                        {
                            video_stream_mute_send(eXosipElement->videostream);
                        }
                        else
                        {
                            video_stream_unmute_send(eXosipElement->videostream);
                        }
                    }
                }
                else
                {
                    QString strRemoteIp = (dynamic_cast <Levent *> (event))->msg;
                    int i;
                    for (i = 0; i < MAX_EXTENSION_MACHINE_CNT; i++) {
                        if (strRemoteIp == eXosip.remip[i].rem_ip /*lphone->video_params.remoteaddr) == 0*/) {
                            if (eXosip.remip[i].videostream != NULL) {
                                if (!bVideoOn)
                                {
                                    video_stream_mute_send(eXosip.remip[i].videostream);
                                }
                                else
                                {
                                    video_stream_unmute_send(eXosip.remip[i].videostream);
                                }
                            }
                        }
                    }
                }

				//int row = ui->listViewCenterAddress->currentIndex().row();
				/*QString strRemoteName;
				  if (row != -1)
				  {
				  strRemoteName = m_CallCenterName[row];
				  }*/
				ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0] + tr("通话中"));
				setDisplayWindow(true);

				CallInfoUploadThread *pCallInfoUploadThread = ((MainDialog *)parent())->m_pVoipMainDlg->m_pCallInfoUploadThread;

				if( pCallInfoUploadThread->m_bRun == false)
                {
                    if(g_check_computercenter_udp_state == RESULT_SUCCESS)
                    {
                        pCallInfoUploadThread->m_upInfoType |= RICT_ANSER;
                        pCallInfoUploadThread->getCallCode(QString(g_strLphoneName[1]));
                        pCallInfoUploadThread->m_TalkTime = 0;
                        pCallInfoUploadThread->m_TalkTimer->start(1000);
                    }
                    else
                    {
                        pCallInfoUploadThread->m_upInfoType = 2;
                        pCallInfoUploadThread->getCallCode(QString(g_strLphoneName[1]));
                        pCallInfoUploadThread->start();
                    }
				}

				break;
			}
		case NID_ICT_RCVVIDEOOPEN:
			{
				setDisplayWindow(true);
				break;
			}
		case NID_ICT_RCVVIDEOCLOSE:
			{
				setDisplayWindow(false);
				usleep(120000);

				ui->lineEditInfo->show();
				m_pFrameFullshow->hide();
				m_pFrameFullshow->lower();

				ui->frameVideoShow->repaint();

				break;
			}
		case	NID_ICT_END:
            {
                if(g_check_computercenter_udp_state == RESULT_SUCCESS)
                {
                    CallInfoUploadThread *pCallInfoUploadThread =  ((MainDialog *)parent())->m_pVoipMainDlg->m_pCallInfoUploadThread;                    
                    pCallInfoUploadThread->m_TalkTimer->stop();
                    pCallInfoUploadThread->getCallCode(g_strLphoneName[1]);
                    pCallInfoUploadThread->SendRecordByUdp();
                    g_check_computercenter_udp_state = RESULT_NULL;
                }
				usleep(120000);
				ResetState();
				////ui->lineEditInfo->setText(tr("结束"));
				//ui->frameVideoShow->update();

				//hide();
                ui->pushButtonCallCenterRecord->setEnabled(false);

				/*char strTemp[80];
				  sprintf(strTemp, "/sbin/arp -d %s", m_strRemoteIp);
				  system(strTemp);
				  printf("%s\n", strTemp);
				  */
				((Application *)qApp)->setUnmuteMic(false);
				((Application *)qApp)->setOnCamera(false);
				((Application *)qApp)->setLCDAndTimerOn();

				break;
			}

		case NID_IDLE_RCVINV:
			{
				VoipMainDialog *pVoipMainDlg = ((MainDialog *)parent())->m_pVoipMainDlg;

				//if (eXosipElement->ringstream != NULL)
				if (1) {
					MainDialog *pMainDlg = (MainDialog *)parent();
					if (!pMainDlg->m_bInterFree)
					{
						//audio_stream_ring_set_level(eXosipElement->ringstream, pVoipMainDlg->m_nRingVolume);
						my_ring_set_level(pVoipMainDlg->m_nRingVolume);
					}
					else
					{
						//audio_stream_ring_set_level(eXosipElement->ringstream, 0);
						my_ring_set_level(0);
					}
				}

				////ui->horizontalSliderVolume->setValue((int)(pVoipMainDlg->m_nVolume * 60.0/1024.0));

				m_bIctState = false;

				strcpy(m_strRemoteIp, eXosipElement->rem_ip);

                ui->pushButtonCallCenterDial->hide();
                //ui->pushButtonCallCenterRecord->hide();
                ui->pushButtonCallCenterAnswer->show();
                ui->pushButtonCallCenterAnswer->setEnabled(true);
                ui->pushButtonCallCenterAnswer->setFocus(Qt::TabFocusReason);
                ui->pushButtonCallCenterHangUp->setEnabled(true);
                ui->pushButtonCallCenterRecord->setEnabled(false);
                //ui->pushButtonCallCenterReturn->setEnabled(false);
                /*ui->pushButtonCallCenterHangUp->hide();
				  QRect rc;
                  rc = ui->pushButtonCallCenterVideoOpen->geometry();
                  rc.moveLeft(ui->pushButtonCallCenterAnswer->geometry().x());
                  ui->pushButtonCallCenterVideoOpen->setGeometry(rc);
                  ui->pushButtonCallCenterVideoClose->setGeometry(rc);
				  */
				//QString strRemoteIpAddress = (dynamic_cast <Levent *> (event))->msg;
				/*int row = ui->listViewCenterAddress->currentIndex().row();
				  QString strRemoteName;
				  if (row != -1)
				  {
				  strRemoteName = m_CallCenterName[row];
				  }*/

				ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0] + tr("来电中") + "...");

				/*if (strRemoteName == g_strLphoneName[0])
				  {
				  ui->pushButtonOpen->show();
				  ui->pushButtonEmergency->hide();

				  }*/

				raise();
				show();
				//ResetState();
				repaint();
				setDisplayWindow(true);

				/*QPalette palette;
				  palette.setBrush(ui->frameVideoShow->backgroundRole(), QBrush(PLAY_AREA_BGCOLOR));
				  ui->frameVideoShow->setPalette(palette);
				  ui->frameVideoShow->setAutoFillBackground(true);
				  */
				((Application *)qApp)->setLCDOn();

				break;
			}
		case NID_IST_RECORDING:
			{
				/*int row = ui->listViewCenterAddress->currentIndex().row();
				  QString strRemoteName;
				  if (row != -1)
				  {
				  strRemoteName = m_CallCenterName[row];
				  }*/
				ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0] + tr("来电留言中"));

				break;
			}
		case NID_IST_NONE:
			{
				ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0]);
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

				ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0] + tr("来电超时"));
				break;
			}
		case NID_IST_OPENTERM:
			{
				ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0] + tr("开锁退出"));
				break;
			}
		case NID_IST_RCVCANCEL:
			{
				//printf("NID_IST_RCVCANCEL\n");
				ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0] + tr("来电取消"));

				if (isVisible())
				{
					setDisplayWindow(false);
					usleep(120000);
					ResetState();//false);

					hide();
				}
				break;
			}
		case NID_IST_TALKING:
			{
				if(eXosipElement->state != IST_TALKING)
				{
					break;
				}

				((Application *)qApp)->setUnmuteMic(true);

				((Application *)qApp)->setOnCamera(true);

				if (eXosipElement->audiostream != NULL)
				{
					audio_stream_set_level(eXosipElement->audiostream, ((MainDialog *)parent())->m_pVoipMainDlg->m_nVolume);
				}

				/*QString strName;
				  int row = ui->listViewCenterAddress->currentIndex().row();
				  if (row != -1)
				  {
				  strName = m_CallCenterName[row];
				  }

				  if ((strName != "") && (strName == g_strLphoneName[0])) //doorway machine
				  {
				  if (eXosipElement->videostream != NULL)
				  {
				  video_stream_mute_send(eXosipElement->videostream);
				  }
				  }
				  else
				  {
				  */
				if (((MainDialog *)parent())->m_pVoipMainDlg->m_bVideoOpen)
				{
					if (eXosipElement->videostream != NULL)
					{
						video_stream_unmute_send(eXosipElement->videostream);
					}
				}
				else
				{
					if (eXosipElement->videostream != NULL)
					{
						video_stream_mute_send(eXosipElement->videostream);
					}
				}
				//}

				m_bIstTalking = true;

				//QString tmpIp = (dynamic_cast <Levent *> (event))->msg;
				//int row = ui->listViewCenterAddress->currentIndex().row();
				/*QString strRemoteName;
				  if (row != -1)
				  {
				  strRemoteName = m_CallCenterName[row];
				  }*/
				ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0] + tr("通话中"));
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
		case NID_IST_END:
			{
				if (isVisible())
				{
					setDisplayWindow(false);
					usleep(120000);
					ResetState();//false);

					hide();
				}

				systemRunLog(3);
				/*char strTemp[80];
				  sprintf(strTemp, "/sbin/arp -d %s", m_strRemoteIp);
				  system(strTemp);
				  printf("%s\n", strTemp);
				  */
				((Application *)qApp)->setUnmuteMic(false);
				((Application *)qApp)->setOnCamera(false);
				((Application *)qApp)->setLCDAndTimerOn();
				//lower();

				//ui->frameVideoShow->update();

				break;
			}
		default:
			{
				return QWidget::event(event);
			}

	}
	return true;
}

/************************************************************
描述：呼叫中心窗口事件过滤函数，实现播放远端视频窗口全屏/非全屏切换
参数：o - 产生事件的控件
	 e - 需处理的事件
返回：true - 事件处理成功，不需进一步处理
	 false - 需要进一步处理
************************************************************/
bool CallCenterDialog::eventFilter(QObject *o, QEvent *e)
{
	if (m_bIctState && (!m_bIctTalking)) return false;

	if (e->type() == QEvent::MouseButtonPress)
	{
		setDisplayWindow(false);
		usleep(120000);

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
int CallCenterDialog::setDisplayWindow(bool bShow)
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

		vrect[2] = ( rect.width() ) & 0x7ffffffe;
		vrect[3] = rect.height();
	}

	ms_filter_call_method(lvirdd->branch.display, MS_FILTER_SET_VIDEO_RECT, vrect);

	return 0;
}

void CallCenterDialog::showCallCenterDialog()
{
	CallCenterDialog::m_pDlg->ResetState();
	CallCenterDialog::m_pDlg->raise();
	CallCenterDialog::m_pDlg->show();
}

/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void CallCenterDialog::showEvent(QShowEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)parent();
	pMainDlg->showRefreshTabFocus(this);
	on_pushButtonCallCenterVideoClose_pressed();			// 默认关闭视频显示界面
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void CallCenterDialog::hideEvent(QHideEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)parent();

	pMainDlg->hideRefreshTabFocus(this);

	if (pMainDlg->m_pCurActiveDlg == pMainDlg)
	{
		pMainDlg->ui->pushButtonCallCenter->setFocus(Qt::TabFocusReason);
	}
}

/************************************************************
描述：门口机开锁回调函数，对门口机开锁
参数：无
返回：无
************************************************************/
/*void CallCenterDialog::on_pushButtonOpen_pressed()
{
	lpc_cmd_open(lphone, NULL);
}*/

/************************************************************
描述：拨号留言回调函数，按后开始留言
参数：无
返回：无
************************************************************/
void CallCenterDialog::on_pushButtonCallCenterRecord_pressed()
{
    if (!ui->pushButtonCallCenterRecord->isEnabled()) return;

	((Application *)qApp)->setUnmuteMic(true);

	lpc_cmd_record(lphone, NULL);

	ui->lineEditInfo->setText(tr("呼叫中心") + " - " + m_CallCenterName[0] + tr("呼出留言中"));
    if(g_check_computercenter_udp_state == RESULT_SUCCESS)
    {
        CallInfoUploadThread *pCallInfoUploadThread =  ((MainDialog *)parent())->m_pVoipMainDlg->m_pCallInfoUploadThread;

        if( pCallInfoUploadThread->m_bRun == false)
        {
            pCallInfoUploadThread->m_upInfoType |= RICT_RECORD;
            pCallInfoUploadThread->getCallCode(g_strLphoneName[1]);
        }
    }
    ui->pushButtonCallCenterVideoOpen->setEnabled(false);
    ui->pushButtonCallCenterVideoClose->setEnabled(false);
    ui->pushButtonCallCenterRecord->setEnabled(false);
    ui->pushButtonCallCenterHangUp->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：当有门铃的时候，进行信息显示
参数：无
返回：无
************************************************************/
void CallCenterDialog::Door_Bell_Ring_Event(QString& str)
{
    ui->lineEditInfo->setText(str);
    repaint();
}

/************************************************************
描述：从全屏状态退出
参数：无
返回：无
************************************************************/
void CallCenterDialog::Exit_From_Full_Screen()
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
