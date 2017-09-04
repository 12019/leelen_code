#include "ictdialog.h"
#include "ui_ictdialog.h"
#include "lpcore.h"
#include "command.h"
#include <QtGui>
#include "qconfig.h"
#include "mediastreamer2/mediastream.h"
#include "exosip_main.h"
#include "maindialog.h"
//#include "callcenterdialog.h"
#include "voipmaindialog.h"
//#include "terminalhelpmaindialog.h"
//#include "helpdialog.h"
#include "application.h"
#include "ui_maindialog.h"
#include "ui_ictrcvhookdialog.h"
#include "core/leelen_comdef.h"
#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
/************************************************************
  该文件为可视对讲拨号对话框主程序
************************************************************/
extern RESULT_E g_check_computercenter_udp_state;
extern LinphoneCore *lphone;
extern eXosip_element_t *eXosipElement;
extern eXosip_t eXosip;
extern char g_strNativeName[50];
extern char g_strLphoneName[3][50];
extern char g_strDialRemoteName[50];
extern MainDialog *g_pMainDlg;
extern int g_bIstOrAlertBusy;

int g_iRcvACKBUSY = 0;

extern int g_iIST_state;
//david
QString callCode;

static bool Dia_Start = false;								// 防止反复拨号
static bool Call_Manage_Machine_Flag = false;						// 如果是呼叫管理机，该标志位为true,反之为false
//static bool videoon = false;								// 临时保存视频是否是打开还是关闭状态
/************************************************************
描述：可视对讲拨号对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
IctDialog::IctDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IctDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

	//ui->pushButtonAdressBook->hide();

    int res = g_pScaleSrc->get_screen_version();
    if 	( res == 2 )  // 1024x600
    {
        ui->pushButtonIctBack->setGeometry(QRect(838, 37, 135, 80));
        ui->pushButtonIctHangUp->setGeometry(QRect(768, 141, 200, 86));
        ui->pushButtonIctDial->setGeometry(QRect(768, 141, 200, 86));
        ui->pushButtonIctReturn->setGeometry(QRect(768, 489, 200, 86));
        ui->pushButtonIctRecord->setGeometry(QRect(768, 402, 200, 86));
        ui->pushButtonIctVideoOpen->setGeometry(QRect(768, 315, 200, 86));
        ui->pushButtonIctVideoClose->setGeometry(QRect(768, 315, 200, 86));
        ui->lineEditInfo->setGeometry(QRect(90, 47, 151, 60));
        ui->pushButtonIctNum2->setGeometry(QRect(283, 149, 220, 95));
        ui->pushButtonIctNum5->setGeometry(QRect(283, 259, 220, 95));
        ui->pushButtonIctNum6->setGeometry(QRect(507, 259, 220, 95));
        ui->pushButtonIctNum8->setGeometry(QRect(283, 369, 220, 95));
        ui->pushButtonIctNum7->setGeometry(QRect(59, 369, 220, 95));
        ui->lineEditDisplay->setGeometry(QRect(260, 52, 521, 50));
        ui->pushButtonIctNum4->setGeometry(QRect(59, 259, 220, 95));
        ui->pushButtonIctDot->setGeometry(QRect(59, 479, 220, 95));
        ui->pushButtonIctNum9->setGeometry(QRect(507, 369, 220, 95));
        ui->pushButtonIctShop->setGeometry(QRect(507, 479, 220, 95));
        ui->pushButtonIctNum1->setGeometry(QRect(59, 149, 220, 95));
        ui->pushButtonIctNum3->setGeometry(QRect(507, 149, 220, 95));
        ui->pushButtonIctNum0->setGeometry(QRect(283, 479, 220, 95));
        ui->frame->setGeometry(QRect(44, 32, 941, 91));
        ui->pushButtonIctAddressBook->setGeometry(QRect(768, 228, 200, 86));
    }
    else if ( res == 3 )  // 1280x800
    {
        ui->pushButtonIctBack->setGeometry(QRect(1042, 56, 165, 102));
        ui->pushButtonIctHangUp->setGeometry(QRect(976, 191, 246, 115));
        ui->pushButtonIctDial->setGeometry(QRect(976, 191, 246, 115));
        ui->pushButtonIctReturn->setGeometry(QRect(976, 653, 246, 115));
        ui->pushButtonIctVideoOpen->setGeometry(QRect(976, 422, 246, 115));
        ui->pushButtonIctRecord->setGeometry(QRect(976, 537, 246, 115));
        ui->pushButtonIctVideoClose->setGeometry(QRect(976, 422, 246, 115));
        ui->lineEditInfo->setGeometry(QRect(70, 56, 151, 102));
        ui->pushButtonIctNum2->setGeometry(QRect(353, 197, 272, 123));
        ui->pushButtonIctNum5->setGeometry(QRect(353, 345, 272, 123));
        ui->pushButtonIctNum6->setGeometry(QRect(649, 345, 272, 123));
        ui->pushButtonIctNum8->setGeometry(QRect(353, 493, 272, 123));
        ui->pushButtonIctNum7->setGeometry(QRect(58, 493, 272, 123));
        ui->lineEditDisplay->setGeometry(QRect(240, 65, 781, 81));
        ui->pushButtonIctNum4->setGeometry(QRect(58, 345, 272, 123));
        ui->pushButtonIctDot->setGeometry(QRect(58, 641, 272, 123));
        ui->pushButtonIctNum9->setGeometry(QRect(649, 493, 272, 123));
        ui->pushButtonIctShop->setGeometry(QRect(649, 641, 272, 123));
        ui->pushButtonIctNum1->setGeometry(QRect(58, 197, 272, 123));
        ui->pushButtonIctNum3->setGeometry(QRect(649, 197, 272, 123));
        ui->pushButtonIctNum0->setGeometry(QRect(353, 641, 272, 123));
        ui->frame->setGeometry(QRect(58, 45, 1176, 121));
        ui->pushButtonIctAddressBook->setGeometry(QRect(976, 307, 246, 115));
    }

    ui->pushButtonIctNum0->m_offsetX = 0;
    ui->pushButtonIctNum1->m_offsetX = 0;
    ui->pushButtonIctNum2->m_offsetX = 0;
    ui->pushButtonIctNum3->m_offsetX = 0;
    ui->pushButtonIctNum4->m_offsetX = 0;
    ui->pushButtonIctNum5->m_offsetX = 0;
    ui->pushButtonIctNum6->m_offsetX = 0;
    ui->pushButtonIctNum7->m_offsetX = 0;
    ui->pushButtonIctNum8->m_offsetX = 0;
    ui->pushButtonIctNum9->m_offsetX = 0;
    ui->pushButtonIctDot->m_offsetX = 0;
    ui->pushButtonIctShop->m_offsetX = 0;
    ui->pushButtonIctBack->setText("");

	ui->pushButtonIctDial->show();
	ui->pushButtonIctDial->setEnabled(true);
	ui->pushButtonIctHangUp->hide();
	ui->pushButtonIctHangUp->setEnabled(true);
	ui->pushButtonIctRecord->setEnabled(false);

	ui->pushButtonIctVideoOpen->show();
	ui->pushButtonIctVideoClose->hide();

	m_strRemoteIp[0] = '\0';

	m_bHookCloseIctDlg = false;

	m_iModalType = BASIC_MODAL_TYPE;

	m_pRecvHookDlg = new IctRcvHookDialog(this);
	m_pRecvHookDlg->hide();
	m_pRecvHookDlg->setDisplayWindow(false);
	
	ui->lineEditDisplay->setMaxLength(11);
	m_Record_Flag = false;
}

/************************************************************
描述：可视对讲拨号对话框析构函数
参数：无
返回：无
************************************************************/
IctDialog::~IctDialog()
{
	delete m_pRecvHookDlg;
	delete ui;
}

/************************************************************
描述：重置本对话框
参数：address - 对话框拨号号码显示控件中的初始化网络地址
返回：无
************************************************************/
void IctDialog::ResetState(QString address)
{
	if (address != "")
	{
		ui->lineEditDisplay->setText(address);//"192.168.1.236");
	}

	setAllButtonsEnabled(this, true);

	ui->lineEditInfo->setText(tr("准备好"));
	ui->pushButtonIctDial->show();

	ui->pushButtonIctHangUp->hide();

	VoipMainDialog *pVoipMainDlg = (VoipMainDialog *)parent();
	if (!pVoipMainDlg->m_pMonDlg->isVisible()&&isVisible())
	{
        ui->pushButtonIctNum1->setFocus(Qt::TabFocusReason);
	}

	ui->pushButtonIctRecord->setEnabled(false);

	m_bHookCloseIctDlg = false;

	ui->pushButtonIctVideoOpen->setEnabled(true);
	ui->pushButtonIctVideoClose->setEnabled(true);

	if (((VoipMainDialog *)parent())->m_bVideoOpen)
	{
		ui->pushButtonIctVideoOpen->hide();
		ui->pushButtonIctVideoClose->show();
		m_VideoOn = true;
	}
	else
	{
		ui->pushButtonIctVideoOpen->show();
		ui->pushButtonIctVideoClose->hide();
		m_VideoOn = false;
	}
}

/************************************************************
描述：按了对话框的"0"按钮，在拨号号码显示控件的文本后面添加"0"
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctNum0_pressed()
{
	Modify_Input_Parameters();
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "0");
}

/************************************************************
描述：按了对话框的"1"按钮，在拨号号码显示控件的文本后面添加"1"
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctNum1_pressed()
{
	Modify_Input_Parameters();
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "1");
}

/************************************************************
描述：按了对话框的"2"按钮，在拨号号码显示控件的文本后面添加"2"
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctNum2_pressed()
{
	Modify_Input_Parameters();
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "2");
}

/************************************************************
描述：按了对话框的"3"按钮，在拨号号码显示控件的文本后面添加"3"
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctNum3_pressed()
{
	Modify_Input_Parameters();
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "3");
}

/************************************************************
描述：按了对话框的"4"按钮，在拨号号码显示控件的文本后面添加"4"
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctNum4_pressed()
{
	Modify_Input_Parameters();
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "4");
}

/************************************************************
描述：按了对话框的"5"按钮，在拨号号码显示控件的文本后面添加"5"
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctNum5_pressed()
{
	Modify_Input_Parameters();
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "5");
}

/************************************************************
描述：按了对话框的"6"按钮，在拨号号码显示控件的文本后面添加“6"
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctNum6_pressed()
{
	Modify_Input_Parameters();
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "6");
}

/************************************************************
描述：按了对话框的"7"按钮，在拨号号码显示控件的文本后面添加"7"
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctNum7_pressed()
{
	Modify_Input_Parameters();
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "7");
}

/************************************************************
描述：按了对话框的"8"按钮，在拨号号码显示控件的文本后面添加"8"
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctNum8_pressed()
{
	Modify_Input_Parameters();
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "8");
}

/************************************************************
描述：按了对话框的"9"按钮，在拨号号码显示控件的文本后面添加"9"
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctNum9_pressed()
{
	Modify_Input_Parameters();
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "9");
}

/************************************************************
描述：按了对话框的"."按钮，在拨号号码显示控件的文本后面添加"."
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctDot_pressed()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + ".");
}

/************************************************************
描述：按了对话框的"#"按钮，在拨号号码显示控件的文本后面添加"-"
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctShop_pressed()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "-");
}

/************************************************************
描述：根据拨号号码显示控件中输入的远端机网络名查询其IP地址，然后对这个
	 IP地址进行拨号
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctDial_pressed()
{
	if ( g_iIST_state == 1 )
 		return;			// 防止两台终端机器同时进行互相呼叫
	else
	{
		usleep(200000);		// 休眠 200ms
		qApp->processEvents();
		if ( g_iIST_state == 1 )
 			return;			// 防止两台终端机器同时进行互相呼叫
	}

	g_iRcvACKBUSY = 0;

	if (!ui->pushButtonIctDial->isEnabled()) return;

	GetPeerIPThread *pGetPeerIPThread = ((MainDialog *)(parent()->parent()))->m_pVoipMainDlg->m_pGetPeerIPThread;

	if (ui->lineEditDisplay->text() != "")
	{
		int ret = 0;

        ui->pushButtonIctAddressBook->setEnabled(false);

		QString strPeerName = ui->lineEditDisplay->text();

		int len = strPeerName.length();

		QString strTemp;

		if(len == 1)
		{
			int mlen = pGetPeerIPThread->m_nativeName.length();

			if(mlen == 9)
				strTemp = pGetPeerIPThread->m_nativeName + QString("-")
					+ ui->lineEditDisplay->text();
			else
				strTemp = pGetPeerIPThread->m_nativeName.left(9) + QString("-")
					+ ui->lineEditDisplay->text();
		}
		else if ( len == 2 || len == 3 )				// 如果长度是两位或三位进行屏蔽，防止和安卓进行呼叫
		{
			setAllButtonsEnabled(this, false);

			ui->frame->setFocus(Qt::TabFocusReason);

			MyMessageBox messBox(this);

			CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));

			okButton->setFocus(Qt::TabFocusReason);

			messBox.setText(tr("您所拨打的号码为空号"));	/// 111

			messBox.exec();

			setAllButtonsEnabled(this, true);

			ui->pushButtonIctRecord->setEnabled(false);

			ui->pushButtonIctDial->setFocus(Qt::TabFocusReason);

			return;
		}
		else if(len == 4)
		{
			strTemp = pGetPeerIPThread->m_nativeName.left(5) + strPeerName;
		}
		else
		{
			strTemp = strPeerName;
		}

		strPeerName = strTemp;

		if(strPeerName == pGetPeerIPThread->m_nativeName) return;

		if ( Dia_Start == true ) return;				// 防止反复拨号

		Dia_Start = true;

		strcpy(g_strDialRemoteName,strPeerName.toLocal8Bit().data());

		if(eXosipElement->state != IST_IDLE)

			eXosipElement->state = IDLE;

		callCode = strPeerName;

		if ( Is_Manage_Machine(callCode) )
		{
			Call_Manage_Machine_Flag = true;
			m_pRecvHookDlg->m_Call_Manage_Machine = true;
		}
		else
		{
			Call_Manage_Machine_Flag = false;
			m_pRecvHookDlg->m_Call_Manage_Machine = false;
		}

		//david 2014.4.16
		((MainDialog *)(parent()->parent()))->m_pVoipMainDlg->m_bCallCenter = false;

		ret = pGetPeerIPThread->CallForPeerName(strPeerName.toLocal8Bit().data());

		m_iModalType = pGetPeerIPThread->m_iModalType;

		if(ret == 0)
		{
			if(eXosipElement->state == IDLE)    goto Exit;

			if(eXosipElement->state == ICT_RINGBACK) goto Exit;

			if(eXosipElement->state == ICT_PREREC) goto Exit;

			((Application *)qApp)->setLCDOn();

			//((MainDialog *)(parent()->parent()))->m_pVoipMainDlg->m_bCallCenter = false;

			ui->lineEditInfo->setText(tr("连接"));

			setAllButtonsEnabled(this, false);

			ui->pushButtonIctVideoOpen->setEnabled(true);

			ui->pushButtonIctVideoClose->setEnabled(true);

			ui->frame->setFocus(Qt::TabFocusReason);
		}
		else if(ret == -1)
		{
			setAllButtonsEnabled(this, false);

			ui->frame->setFocus(Qt::TabFocusReason);

			MyMessageBox messBox(this);

			CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));

			okButton->setFocus(Qt::TabFocusReason);

			messBox.setText(tr("您拨打的号码是空号"));			// 1-1

			messBox.exec();

			setAllButtonsEnabled(this, true);

			ui->pushButtonIctRecord->setEnabled(false);

			ui->pushButtonIctDial->setFocus(Qt::TabFocusReason);
		}

	}
Exit:
	Dia_Start = false;
}

/************************************************************
描述：挂断当前拨号后正在呼叫的可视对讲应用
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctHangUp_pressed()
{
	if (!ui->pushButtonIctHangUp->isVisible()) return;
	if (!ui->pushButtonIctHangUp->isEnabled()) return;

	ui->pushButtonIctHangUp->setEnabled(false);
	ui->frame->setFocus(Qt::TabFocusReason);

	lpc_cmd_terminate(lphone, NULL);

	//ResetState("");

	m_Record_Flag = false;
	((Application *)qApp)->setLCDAndTimerOn();
}

/************************************************************
描述：按了对话框的"退格"按钮，删除拨号号码显示控件的文本最后面的数字
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctBack_pressed()
{
	if ( ui->lineEditDisplay->hasSelectedText() == false )
	{
		QString str = ui->lineEditDisplay->text();

		if ( str.length() == 6 || str.length() == 11 )
		{
			str = str.left(str.length() - 2);
		}
		else
		{
			str = str.left(str.length() - 1);
		}

		ui->lineEditDisplay->setText(str);
	}
	else
		ui->lineEditDisplay->del();
}

/************************************************************
描述：挂断当前呼叫并关闭本窗口
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctReturn_pressed()
{
	if (!isVisible()) return;
	if (m_pRecvHookDlg->isVisible()) return;

	lpc_cmd_terminate(lphone, NULL);

	ui->pushButtonIctDial->show();
	ui->pushButtonIctDial->setEnabled(true);
	ui->pushButtonIctHangUp->hide();
	ui->pushButtonIctDial->setFocus(Qt::TabFocusReason);

	ui->pushButtonIctRecord->setEnabled(false);

    hide();
    ((QDialog *)parent())->hide();

	((Application *)qApp)->setLCDAndTimerOn();
	m_Record_Flag = false;
}

/************************************************************
描述：可视对讲拨号窗口事件处理函数，实现lphone库ICT上发消息的处理
参数：event - 需处理的事件
返回：true - 事件处理成功
	 false - 事件处理失败
************************************************************/
bool IctDialog::event(QEvent *event)
{
	event->accept();

	switch (event->type() - 5000)
	{
	case NID_ICT_NONE:
		{
			ui->lineEditInfo->setText(tr("无"));
			break;
		}
	case NID_ICT_TIMEOUT:
		{
			if (m_pRecvHookDlg->m_pFrameFullshow->isVisible())
			{
				m_pRecvHookDlg->setDisplayWindow(false);
				usleep(100000);

				m_pRecvHookDlg->ui->lineEditInfo->show();
				m_pRecvHookDlg->m_pFrameFullshow->hide();
				m_pRecvHookDlg->m_pFrameFullshow->lower();
			}

			ui->lineEditInfo->setText(tr("超时"));
            //ResetState("");
			break;
		}
	case NID_ICT_OPENTERM:
		{
			ui->lineEditInfo->setText(tr("开锁退出"));
            //ResetState(""); //david 2014.4.19
			break;
		}
	case NID_ICT_RCVVIDEOOPEN:
		{
			m_pRecvHookDlg->setDisplayWindow(true);
			break;
		}
	case NID_ICT_RCVVIDEOCLOSE:
		{
			m_pRecvHookDlg->setDisplayWindow(false);
			usleep(120000);

			m_pRecvHookDlg->ui->lineEditInfo->show();
			m_pRecvHookDlg->m_pFrameFullshow->hide();
			m_pRecvHookDlg->m_pFrameFullshow->lower();

			m_pRecvHookDlg->ui->frameVideoShow->repaint();

			break;
		}
	case NID_ICT_RCVBUSY:
		{
			g_iRcvACKBUSY = 1;
			ui->lineEditInfo->setText(tr("现在正忙"));
			ui->lineEditInfo->repaint();
			break;
		}
	case NID_ICT_CONNECTED:
        {

            if(((MainDialog*)(parent()->parent()))->m_pSecurityMainDlg->m_pSecurityAlertDlg->isVisible()&&(eXosipElement->state==ICT_RINGBACK))
            {
                ((MainDialog*)(parent()->parent()))->m_pSecurityMainDlg->m_pSecurityAlertDlg->on_pushButtonSecAlertReturn_pressed();
            }

			if ( Call_Manage_Machine_Flag == true )
				on_pushButtonIctVideoClose_pressed();

			((Application *)qApp)->setLCDOn();
			((Application *)qApp)->setOnCamera(true);

			my_ring_set_level(((VoipMainDialog *)parent())->m_nRingVolume);

			int i;
			bool bVideoOn = false;
			GetPeerIPThread *pGetPeerIPThread = ((MainDialog *)(parent()->parent()))->m_pVoipMainDlg->m_pGetPeerIPThread;

            m_iModalType = pGetPeerIPThread->m_iModalType;

			if (m_iModalType & DOORWAY_MODAL_TYPE)
			{
				bVideoOn = false;
                ((VoipMainDialog *)parent())->m_bVideoOpen = bVideoOn;
			}
			else
			{
				bVideoOn = ((VoipMainDialog *)parent())->m_bVideoOpen;
			}

            if ( Call_Manage_Machine_Flag == true )
            {
                bVideoOn = false ;
                ((VoipMainDialog *)parent())->m_bVideoOpen = bVideoOn;
            }

			if (eXosip.callflag == 0)
			{
				if (eXosipElement->videostream != NULL)
				{
					if (!bVideoOn)
					{
						lpc_cmd_video(lphone, (char *)"close");
						video_stream_mute_send(eXosipElement->videostream);
					}
					else
					{
						lpc_cmd_video(lphone, (char *)"open");
						video_stream_unmute_send(eXosipElement->videostream);
					}
				}
			}
			else
			{
				QString strRemoteIp = (dynamic_cast <Levent *> (event))->msg;

				for (i = 0; i < MAX_EXTENSION_MACHINE_CNT; i++) {
					if (strRemoteIp == eXosip.remip[i].rem_ip /*lphone->video_params.remoteaddr) == 0*/) {
						if (eXosip.remip[i].videostream != NULL) {
							if (!bVideoOn)
							{
                                lpc_cmd_video(lphone, (char *)"close");
								video_stream_mute_send(eXosip.remip[i].videostream);
							}
							else
							{
                                lpc_cmd_video(lphone, (char *)"open");
								video_stream_unmute_send(eXosip.remip[i].videostream);
							}
						}
					}
				}
			}

			ui->lineEditInfo->setText(tr("已连接"));//:") + ui->lineEditDisplay->text());

			setAllButtonsEnabled(this, false);//david 2014.4.16
			ui->pushButtonIctDial->hide();
			ui->pushButtonIctVideoOpen->setEnabled(true);
			ui->pushButtonIctVideoClose->setEnabled(true);
			ui->pushButtonIctReturn->setEnabled(true);
			ui->pushButtonIctHangUp->setEnabled(true);
			ui->pushButtonIctHangUp->show();
			ui->pushButtonIctHangUp->setFocus(Qt::TabFocusReason);

            CallInfoUploadThread *pCallInfoUploadThread = ((VoipMainDialog *)parent())->m_pCallInfoUploadThread;
            if(g_check_computercenter_udp_state == RESULT_NULL)
            {
                if( pCallInfoUploadThread->m_bRun == false)
                {
                    pCallInfoUploadThread->m_upInfoType = 1;
                    pCallInfoUploadThread->getCallCode(callCode);
                    pCallInfoUploadThread->m_IsUpImage = 0;
                    pCallInfoUploadThread->start();
                }
            }

			if (!(m_iModalType & DOORWAY_MODAL_TYPE))
			{
				ui->pushButtonIctRecord->setEnabled(true);
			}
			break;
		}

	case NID_ICT_NOANSWER:
		{
			ui->lineEditInfo->setText(tr("无应答"));
			break;
		}
	case NID_ICT_RCVCANCEL:
		{
			if (m_pRecvHookDlg->isVisible())
			{
				m_pRecvHookDlg->setDisplayWindow(false);
				usleep(120000);

				m_pRecvHookDlg->ui->lineEditInfo->show();
				m_pRecvHookDlg->m_pFrameFullshow->hide();
				m_pRecvHookDlg->m_pFrameFullshow->lower();

				m_pRecvHookDlg->hide();
				hide();
				((VoipMainDialog *)parent())->hide();

				((Application *)qApp)->setLCDAndTimerOn();
			}
			ui->lineEditInfo->setText(tr("接收取消"));
			break;
		}
	case NID_ICT_RCVOPEN:
		{
			ui->lineEditInfo->setText(tr("接收打开"));
            break;
		}
	case NID_ICT_TALKING:
		{
			((Application *)qApp)->setLCDOn();

			((Application *)qApp)->setUnmuteMic(true);

			if (eXosipElement->audiostream != NULL)
			{
				audio_stream_set_level(eXosipElement->audiostream, ((VoipMainDialog *)parent())->m_nVolume);
			}

			int i;
			bool bVideoOn = false;
			if (m_iModalType & DOORWAY_MODAL_TYPE)
			{
				bVideoOn = false;
                ((VoipMainDialog *)parent())->m_bVideoOpen = bVideoOn;
			}
			else
			{
				bVideoOn = ((VoipMainDialog *)parent())->m_bVideoOpen;
			}

            if ( Call_Manage_Machine_Flag == true )
            {
                 bVideoOn = false;
                ((VoipMainDialog *)parent())->m_bVideoOpen = bVideoOn;
            }

			if (eXosip.callflag == 0)
			{
				if (eXosipElement->videostream != NULL)
				{
					if (!bVideoOn)
					{
						lpc_cmd_video(lphone, (char *)"close");
						video_stream_mute_send(eXosipElement->videostream);
					}
					else
					{
						lpc_cmd_video(lphone, (char *)"open");
						video_stream_unmute_send(eXosipElement->videostream);
					}
				}
			}
			else
			{
				for (i = 0; i < MAX_EXTENSION_MACHINE_CNT; i++) {
					if (eXosip.remip[i].videostream != NULL) {
						if (!bVideoOn)
						{
							lpc_cmd_video(lphone, (char *)"close");
							video_stream_mute_send(eXosip.remip[i].videostream);
						}
						else
						{
							lpc_cmd_video(lphone, (char *)"open");
							video_stream_unmute_send(eXosip.remip[i].videostream);
						}
					}
				}
			}

			QString strRemoteNetName;
			strRemoteNetName = (dynamic_cast <Levent *> (event))->msg;
			int pos = strRemoteNetName.indexOf("?");
			//m_iModalType = BASIC_MODAL_TYPE;
			if (pos != -1)
			{
				m_iModalType = strRemoteNetName.mid(pos + strlen("?")).toInt();

				strRemoteNetName = strRemoteNetName.left(pos);
			}

			QStringList &addressList = g_pMainDlg->m_pAddressBookDlg->m_addressListNetName;
			for (i = 0; i < addressList.count(); i++)
			{
				QString str = addressList.at(i);

				if (strRemoteNetName.length() > str.length())
				{
					if ((strRemoteNetName.left(str.length()) == str) && (strRemoteNetName.at(str.length()) == QChar('-')))
					{
						strRemoteNetName = g_pMainDlg->m_pAddressBookDlg->m_addressListName.at(i) + strRemoteNetName.mid(str.length());
						break;
					}
				}
				else if (strRemoteNetName == str)
				{
					strRemoteNetName = g_pMainDlg->m_pAddressBookDlg->m_addressListName.at(i);
					break;
				}
			}

			if (i >= addressList.count())
			{
				pos = strRemoteNetName.indexOf("-");
				if ((pos != -1) && ((pos != strRemoteNetName.length() - 2) || (strRemoteNetName.at(strRemoteNetName.length() - 1).isDigit())))
				{
					QString strTemp = g_strNativeName;//g_strLphoneName[0];
					int pos2 = strTemp.indexOf("-");
					if ((pos2 != -1) && ((pos2 != strTemp.length() - 2) || (strTemp.at(strTemp.length() - 1).isDigit())))
					{
						strTemp = strTemp.left(pos2);
						if (strRemoteNetName.left(pos) == strTemp)
						{
							strRemoteNetName = strRemoteNetName.mid(pos + strlen("-"));
						}
					}
				}
			}

			ui->lineEditInfo->setText(tr("通话中"));


			m_pRecvHookDlg->show();
			m_pRecvHookDlg->ResetState(tr("呼出电话") + " - " + strRemoteNetName + tr("通话中"));//m_strRemoteIp);
			m_pRecvHookDlg->repaint();
			if ( m_iModalType == BASIC_MODAL_TYPE )  // 如果是分机，启动计时功能
				m_pRecvHookDlg->Start_Timeout_Count();	// 建立对讲之后开始计时

			if (isVisible() && (((QDialog *)parent())->isVisible()))
			{
				m_pRecvHookDlg->setDisplayWindow(true);
			}

			ui->lineEditDisplay->setText("");

			CallInfoUploadThread *pCallInfoUploadThread = ((VoipMainDialog *)parent())->m_pCallInfoUploadThread;

			if( pCallInfoUploadThread->m_bRun == false)
			{
                if(g_check_computercenter_udp_state == RESULT_SUCCESS)
                {
                    pCallInfoUploadThread->m_upInfoType |= RICT_ANSER;
                    pCallInfoUploadThread->getCallCode(callCode);
                    pCallInfoUploadThread->m_TalkTime = 0;
                    pCallInfoUploadThread->m_TalkTimer->start(1000);
                }
                else
                {
                    pCallInfoUploadThread->m_upInfoType = 2;
                    pCallInfoUploadThread->getCallCode(callCode);
                    pCallInfoUploadThread->start();
                }
			}
			break;
		}
	case NID_ICT_END:
		{
			printf("NID_ICT_END\n");
            if(g_check_computercenter_udp_state == RESULT_SUCCESS)
            {
                CallInfoUploadThread *pCallInfoUploadThread = ((VoipMainDialog *)parent())->m_pCallInfoUploadThread;
                pCallInfoUploadThread->m_TalkTimer->stop();
                pCallInfoUploadThread->getCallCode(callCode);
                pCallInfoUploadThread->SendRecordByUdp();
                g_check_computercenter_udp_state = RESULT_NULL;
            }

			Call_Manage_Machine_Flag = false;
			m_pRecvHookDlg->m_Call_Manage_Machine = false;
			m_pRecvHookDlg->Stop_Timeout_Count();		// 停止计时间

			if (m_bHookCloseIctDlg)
			{
				hide();
				((VoipMainDialog *)parent())->hide();
			}

			systemRunLog(2);

			m_pRecvHookDlg->ui->lineEditInfo->show();
			m_pRecvHookDlg->m_pFrameFullshow->hide();
			m_pRecvHookDlg->m_pFrameFullshow->lower();

			m_pRecvHookDlg->hide();
			m_bHookCloseIctDlg = false;

			/*char strTemp[80];
			  sprintf(strTemp, "/sbin/arp -d %s", m_strRemoteIp);
			  system(strTemp);
			  printf("%s\n", strTemp);
			  */
			((Application *)qApp)->setUnmuteMic(false);
			printf(" g_bIstOrAlertBusy:%d\n",g_bIstOrAlertBusy);

			extern WriteAVI *g_pWriteAVI;
			if(g_pWriteAVI == NULL)
				((Application *)qApp)->setOnCamera(false);

			((Application *)qApp)->setLCDAndTimerOn();

			printf("g_iRcvACKBUSY:%d\n",g_iRcvACKBUSY);
			if(g_iRcvACKBUSY)  usleep(2000000);

			ui->lineEditInfo->setText(tr("通话结束"));

			ResetState("");
			ui->lineEditDisplay->setText("");
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
描述：本机视频打开，即允许远端设备观看本机摄像头视频
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctVideoOpen_pressed()
{
	if (!ui->pushButtonIctVideoOpen->isVisible()) return;

	if (eXosipElement->avistream != NULL)
	{
		return;
	}

	if ( Call_Manage_Machine_Flag == false )
		((VoipMainDialog *)parent())->m_bVideoOpen = true;

	m_VideoOn = true;			// 同步记录

	int i;
	if (!(m_iModalType & DOORWAY_MODAL_TYPE))
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
			for (i = 0; i < MAX_EXTENSION_MACHINE_CNT; i++) 
			{
				if (eXosip.remip[i].videostream != NULL) 
				{
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

	ui->pushButtonIctVideoOpen->hide();
	ui->pushButtonIctVideoClose->show();
	ui->pushButtonIctVideoClose->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：本机视频关闭，即不允许远端设备观看本机摄像头视频
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctVideoClose_pressed()
{
	if (!ui->pushButtonIctVideoClose->isVisible()) return;

	if (eXosipElement->avistream != NULL)
	{
		return;
	}

	if ( Call_Manage_Machine_Flag == false )
		((VoipMainDialog *)parent())->m_bVideoOpen = false;

	m_VideoOn = false;			// 同步记录

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

	ui->pushButtonIctVideoOpen->show();
	ui->pushButtonIctVideoClose->hide();
	ui->pushButtonIctVideoOpen->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：呼叫远端后在对方摘机前进行留言
参数：无
返回：无
************************************************************/
void IctDialog::on_pushButtonIctRecord_pressed()
{
	if (!ui->pushButtonIctRecord->isEnabled()) return;

	((Application *)qApp)->setUnmuteMic(true);

	lpc_cmd_record(lphone, NULL);

	ui->lineEditInfo->setText(tr("留言"));//:") + ui->lineEditDisplay->text());
    if(g_check_computercenter_udp_state == RESULT_SUCCESS)
    {
        CallInfoUploadThread *pCallInfoUploadThread = ((VoipMainDialog *)parent())->m_pCallInfoUploadThread;

        if( pCallInfoUploadThread->m_bRun == false)
        {
            pCallInfoUploadThread->m_upInfoType |= RICT_RECORD;
            pCallInfoUploadThread->getCallCode(callCode);
        }
    }
	////lg
	ui->pushButtonIctVideoOpen->setEnabled(false);
	ui->pushButtonIctVideoClose->setEnabled(false);
	////lg

	ui->pushButtonIctRecord->setEnabled(false);
	ui->pushButtonIctHangUp->setFocus(Qt::TabFocusReason);
	m_Record_Flag = true;
}

/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void IctDialog::showEvent(QShowEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());
	pMainDlg->showRefreshTabFocus(this);
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void IctDialog::hideEvent(QHideEvent *)
{
	ui->pushButtonIctAddressBook->setEnabled(true);

	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());

	pMainDlg->m_pAddressBookDlg->hide();
	pMainDlg->hideRefreshTabFocus(this);
	if (pMainDlg->m_pCurActiveDlg == pMainDlg)
	{
		pMainDlg->ui->pushButtonVoip->setFocus(Qt::TabFocusReason);
	}
}

void IctDialog::on_pushButtonIctAddressBook_pressed()
{
	if (g_pMainDlg->m_pAddressBookDlg->isVisible()) return;

	Disable_Buttons();
	g_pMainDlg->m_pAddressBookDlg->raise();
	g_pMainDlg->m_pAddressBookDlg->show();
}

/************************************************************
描述：进入地址簿的时候，屏蔽按钮
参数：无
返回：无
************************************************************/
void IctDialog::Disable_Buttons()
{
	ui->pushButtonIctAddressBook->setEnabled(false);
	ui->pushButtonIctDial->setEnabled(false);
	ui->pushButtonIctHangUp->setEnabled(false);
	ui->pushButtonIctRecord->setEnabled(false);

	ui->pushButtonIctNum1->setEnabled(false);
	ui->pushButtonIctNum2->setEnabled(false);
	ui->pushButtonIctNum3->setEnabled(false);
	ui->pushButtonIctNum4->setEnabled(false);
	ui->pushButtonIctNum5->setEnabled(false);
	ui->pushButtonIctNum6->setEnabled(false);
	ui->pushButtonIctNum8->setEnabled(false);
	ui->pushButtonIctNum7->setEnabled(false);
	ui->pushButtonIctNum9->setEnabled(false);
	ui->pushButtonIctNum0->setEnabled(false);

	ui->pushButtonIctVideoOpen->setEnabled(false);
	ui->pushButtonIctVideoClose->setEnabled(false);

	ui->pushButtonIctDot->setEnabled(false);
	ui->pushButtonIctShop->setEnabled(false);
	ui->pushButtonIctBack->setEnabled(false);
}

/************************************************************
描述：退出地址簿的时候，使能按钮
参数：无
返回：无
************************************************************/
void IctDialog::Enable_Buttons()
{
	ui->pushButtonIctAddressBook->setEnabled(true);
	ui->pushButtonIctDial->setEnabled(true);
	ui->pushButtonIctHangUp->setEnabled(true);

	ui->pushButtonIctNum1->setEnabled(true);
	ui->pushButtonIctNum2->setEnabled(true);
	ui->pushButtonIctNum3->setEnabled(true);
	ui->pushButtonIctNum4->setEnabled(true);
	ui->pushButtonIctNum5->setEnabled(true);
	ui->pushButtonIctNum6->setEnabled(true);
	ui->pushButtonIctNum8->setEnabled(true);
	ui->pushButtonIctNum7->setEnabled(true);
	ui->pushButtonIctNum9->setEnabled(true);
	ui->pushButtonIctNum0->setEnabled(true);

	ui->pushButtonIctVideoOpen->setEnabled(true);
	ui->pushButtonIctVideoClose->setEnabled(true);

	ui->pushButtonIctDot->setEnabled(true);
	ui->pushButtonIctShop->setEnabled(true);
	ui->pushButtonIctBack->setEnabled(true);

}

/************************************************************
描述：判断当前拨出的号码是否是管理中心，如果是则返回true
参数：无
返回：无
************************************************************/
bool IctDialog::Is_Manage_Machine(const QString& name)
{
	QSettings settings(CONFIG_FILE, QSettings::IniFormat);

	if ( settings.value("system/manage_center1_name", "0").toString() == name )
		return true;
	else
		return false;
}

/************************************************************
描述：当用户进行输入的时候，进行修正
参数：无
返回：无
************************************************************/
void IctDialog::Modify_Input_Parameters()
{
	QString str = ui->lineEditDisplay->text();
	int len = str.length();

	if ( len == 4 )
		ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "-");
	else if ( len == 9 )
		ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "-");
}
