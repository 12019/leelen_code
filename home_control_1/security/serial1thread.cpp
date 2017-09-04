#include "serial1thread.h"
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <QSettings>
#include <maindialog.h>
#include <QStandardItemModel>
#include "securitymaindialog.h"
//#include <linux/watchdog.h>
#include <sys/ioctl.h>
#include "adddefensedialog.h"
#include "maindialog.h"
#include "ui_securitymaindialog.h"
#include "ui_maindialog.h"
#include "application.h"
#include "ui_doorbelldialog.h"
#include "exosip_main.h"
#include "exosip_message.h"
#include <sys/vfs.h>


#include <global.h>
/************************************************************
  该文件为串口报警信息接收处理线程主程序
************************************************************/
extern int g_iDeviceType;
extern int b_ring_play_run;

extern eXosip_element_t *eXosipElement;
extern bool g_Digital_Channel_Flag;				// 标识数字通道的变量

extern MainDialog *g_pMainDlg;
RESULT_E g_check_managemachine_udp_state = RESULT_NULL; // 检测管理机是否支持udp协议  同时供其他模块协议探测使用
RESULT_E g_check_computercenter_udp_state = RESULT_NULL; // 检测物业软件是否支持udp协议  同时供其他模块协议探测使用
/************************************************************
  来电处理和报警录像启动不可同时进入，如果该变量非0则需要等待其为0才能
  处理来电或启动报警录像
************************************************************/
extern int g_bIstOrAlertBusy;
extern int g_jpg_image_ready;
extern int g_mainMonitorFlag;

char g_hcSerialBuf[14];
int g_hcCmd = 0;

extern int g_doorbell_on;
extern ViewCam *g_pViewCam;

extern char g_strNativeName[50];

extern char g_strLphoneName[3][50];

extern int IsFileEXist(const char *fullname);

extern int g_door_bell_type;	// 二次门铃的类型的全局变量 在main.cpp中定义

void manager_result_callback(RESULT_E result) //回调函数
{
    printf("\n=============[%s -- %d] result = %d\n", __func__, __LINE__, result);
    g_check_managemachine_udp_state = result;
}

void computer_result_callback(RESULT_E result) //回调函数
{
    printf("\n=============[%s -- %d] result = %d\n", __func__, __LINE__, result);
    g_check_computercenter_udp_state = result;
}

/************************************************************
描述：串口报警信息接收处理线程构造函数
参数：parent - 该线程的父对象指针
返回：无
************************************************************/
Serial1Thread::Serial1Thread(QObject *parent) :
    QThread(parent)
{
	m_serial1_fd = -1;

	m_bRun = -1;

	m_pJpegSendThread = NULL;

	//lg
	int i;
	SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent;
	QSettings settings("/mnt/disk/config/defense", QSettings::IniFormat);
	for (i = 0; i < pSecurityMainDlg->m_wireDefenseCnt; i++)
	{
		settings.setValue("wiredefense/defense" + QString::number(pSecurityMainDlg->m_allDefenseNumber[i]), -1);//1);
	}
	for (i = 0; i < pSecurityMainDlg->m_wirelessDefenseCnt; i++)
	{
		settings.setValue("wirelessdefense/defense" + QString::number(pSecurityMainDlg->m_allDefenseNumber[56 + i]), -1);//0);
	}
	//lg

	m_pExitDelayTimer = new QTimer(this);
	m_pExitDelayTimer->setSingleShot(true);

    connect(this, SIGNAL(openRecorder(int,int, QString, int, int, int, int, int, QString, QString)), (SecurityMainDialog *)parent, SLOT(startRecorder(int,int, QString, int, int, int, int, int, QString, QString)));

	connect(this, SIGNAL(sigRefrashList(QDateTime,int,int,int,QString)), (SecurityMainDialog *)parent, SLOT(slotRefreshList(QDateTime,int,int,int,QString)));

	connect(this, SIGNAL(sigCreateJpegThread()), (SecurityMainDialog *)parent, SLOT(createJpegSendThread()));

	connect(this, SIGNAL(sigOpenDoorBellDlg()), g_pMainDlg->m_pDoorBellDlg, SLOT(OpenDoorBellDlg()));
	connect(this, SIGNAL(sigExtensionUnitAnswerDoorBell()), g_pMainDlg->m_pDoorBellDlg, SLOT(on_pushButtonDoorBellAnswer_pressed()));
	connect(this, SIGNAL(sigCloseDoorBellDlg()), g_pMainDlg->m_pDoorBellDlg, SLOT(CloseDoorBellDlg()));
	connect(this, SIGNAL(sigShowAbnormalMsgBox1(QString)), g_pMainDlg->m_pSysSetMainDlg->m_pAddDefenseDlg, SLOT(showAbnormalMsgBox(QString)));
	connect(this, SIGNAL(sigShowAbnormalMsgBox2(QString)), g_pMainDlg, SLOT(showAbnormalMsgBox(QString)));
	connect(this, SIGNAL(sigShowAbnormalMsgBox3(QString)), g_pMainDlg, SLOT(Do_Door_Bell_Event(QString)));

	connect(this, SIGNAL(sigExtensionUnitAnswer()), g_pMainDlg->m_pVoipMainDlg->m_pIstDlg, SLOT(on_pushButtonIstAnswer_pressed()));
	connect(this, SIGNAL(sigExtensionUnitHangUp()), g_pMainDlg->m_pVoipMainDlg->m_pIstDlg, SLOT(on_pushButtonIstHangUp_pressed()));
	connect(this, SIGNAL(sigExtensionUnitOpen()), g_pMainDlg->m_pVoipMainDlg->m_pIstDlg, SLOT(on_pushButtonIstOpen_pressed()));

	connect(this, SIGNAL(sigExtensionUnitMon()), g_pMainDlg->m_pVoipMainDlg->m_pMonDlg, SLOT(showMonDlg()));
	connect(this, SIGNAL(sigCloseMonDlg()), g_pMainDlg->m_pVoipMainDlg->m_pMonDlg, SLOT(on_pushButtonMonReturn_pressed()));

	m_openRecorderCnt = 0;

	m_pDoorRingTimer = new QTimer(this);

	m_pDoorRingTimer->setSingleShot(true);

	connect(m_pDoorRingTimer,SIGNAL(timeout()),this,SLOT(stopDoorRing()));

	m_pAutoQuitTimer = new QTimer(this);

	m_pAutoQuitTimer->setSingleShot(true);

	connect(m_pAutoQuitTimer,SIGNAL(timeout()),g_pMainDlg->m_pAbnormalMsgBox,SLOT(on_buttonClicked()));

	m_pDeviceStartTimer = new QTimer(this);

	m_pDeviceStartTimer->setSingleShot(true);

	m_pDeviceStartTimer->start(20 * 1000);

    m_pSendAlarmInfoThread = new SendAlarmInfoThread(this);

}

/************************************************************
描述：串口报警信息接收处理线程析构函数
参数：无
返回：无
************************************************************/
Serial1Thread::~Serial1Thread()
{
	m_bRun = 0;

	delete m_pExitDelayTimer;

	if (m_serial1_fd != -1)
	{
		::close(m_serial1_fd);
	}
}

void Serial1Thread::stopDoorRing()
{
	if(b_ring_play_run)
	{
		my_ring_stop();
	}
	m_pDoorRingTimer->stop();
}
/************************************************************
描述：延迟防区延迟报警定时器超时，设置m_bEnterDelayTimeOut和延迟防
	 区结构类的m_isTimeOut为1，以便在线程主函数中进行处理
参数：无
返回：无
************************************************************/
void Serial1Thread::enterDelayTimeOut()
{
    usleep(50000);  //防止延时报警防区同时报警时未发送报警信号到管理机

	printf("enter delay timeout\n");

	DelayAlarmClass *pDelayAlarmClass;
	pDelayAlarmClass = (DelayAlarmClass *)(sender()->parent());

	pDelayAlarmClass->m_isTimeOut = 1;

	m_bEnterDelayTimeOut = 1;
}

void Serial1Thread::waitForVacant()
{
	while ((m_iDoorBellCmd != 0) || (m_iExtensionUnitCmd != 0) || (m_iHomeControlCmd != 0))
	{
		usleep(50000);
	}
}

/************************************************************
描述：串口报警信息接收处理线程主函数，该主函数先用串口1向单片机发防区
	 状态扫描信号，然后等待接收串口1的报警信息，如果有报警则记录下来
	 并启动报警录像和向管理机发送JPEG图像，如果检测到延迟防区报警则
	 进行延迟报警处理。该线程还负责门铃响铃对讲处理。
参数：无
返回：无
************************************************************/
void Serial1Thread::run()
{
	m_number = 0;
	m_property = 0;
	m_channel = 0;
	m_name = 0;
	m_type = 0;
	m_strDateTime = "";
	m_strLocation = "";
	m_strAviPath = "";
	m_bEnterDelayTimeOut = 0;
	m_iDoorBellCmd = 0;
	m_iDoorBellAddr = 0;
	m_iExtensionUnitCmd = 0;
	m_iExtensionUnitAddr = 0;
	m_iExtensionUnitCallAddr = 0;
	m_iHomeControlCmd = 0;
    //david 2014.3.25
	m_iCallElevatorCmd = 0;

	g_hcCmd = 0;

	SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();

	while ((!g_pMainDlg) || (!g_pMainDlg->isVisible()))
	{
		usleep(100000);
	}

	tcflush(m_serial1_fd, TCIOFLUSH);

	((Application *)qApp)->setLCDAndTimerOn();

	if (pSecurityMainDlg->m_iExitDelay != 0)
	{
		m_pExitDelayTimer->stop();
        	m_pExitDelayTimer->start(pSecurityMainDlg->m_iExitDelay * 1000);
	}

	char buf[14];
	char buf2[14];
	int cnt, cnt1;
	int i;
	int sum;
	unsigned char ch;

	if (m_bRun == -1)
	{
		memset(buf, 0, 14);

		buf[0] = 0xf7;
		buf[1] = 0xff;
		buf[2] = 0x00;
		buf[3] = 0xfe;

		sum = 0;
		for (i = 0; i < 13; i++)
		{
			sum += buf[i];
		}
		buf[13] = (-sum) & 0xff;

		if(g_iDeviceType == TOUCHZ)

		cnt = write(m_serial1_fd, buf, 14);
	}

	fd_set rd;
	struct timeval tv;
	bool bFound;
	int tmp_iPeerAddr = 0;

	m_bRun = 1;
	while (m_bRun)
	{
		bFound = false;
		cnt = 0;

        while (m_bRun)
        {
            //david 2013.3.26
            if(m_iCallElevatorCmd)
            {
                m_iCallElevatorCmd = 0;

                memset(buf2,0,14);
                buf2[0] = 0xf7;
                buf2[1] = 0x50;
                buf2[3] = 0xf4;

                int sum = 0;

                for(i = 0;i < 13;i++)
                {
                    sum += buf2[i];
                }

                buf2[13] = (-sum) & 0xff;

                write(m_serial1_fd,buf2,14);

            }

			if (m_iDoorBellCmd != 0)
			{
				memset(buf2, 0, 14);

				if (m_iDoorBellCmd == 0xf9) //busy
				{
					buf2[0] = 0xf7;
					buf2[1] = tmp_iPeerAddr;
					buf2[2] = 0x00;
					buf2[3] = 0xf9;
				}
				else if (m_iDoorBellCmd == 0xf7) //Monitor
				{
					buf2[0] = 0xf7;
					buf2[1] = m_iDoorBellAddr;
					buf2[2] = 0x00;//扩展分机可能不同
					buf2[3] = 0xf7;
					buf2[4] = 0x00;
					buf2[5] = 0x00;
				}
				else if (m_iDoorBellCmd == 0xfa) //hang up
				{
					buf2[0] = 0xf7;
					buf2[1] = 0xff;
					buf2[2] = 0x00;//扩展分机可能不同
					buf2[3] = 0xfa;
					buf2[4] = 0xff;
				}
				else /*if (g_pMainDlg->m_pDoorBellDlg->isVisible())
				{*/
					if (m_iDoorBellCmd == 0xfc) //ring
					{
						buf2[0] = 0xf7;
						buf2[1] = 0xff;
						buf2[2] = 0x00;
						buf2[3] = 0xfc;
						buf2[4] = m_iDoorBellAddr;
					}
					else if (m_iDoorBellCmd == 0xfb) //answer
					{
						buf2[0] = 0xf7;
						buf2[1] = 0xff;
						buf2[2] = 0x00;//扩展分机可能不同
						buf2[3] = 0xfb;
                        buf2[4] = m_iDoorBellAddr;
					}
					else if (m_iDoorBellCmd == 0xf8) //Open
					{
						buf2[0] = 0xf7;
						buf2[1] = m_iDoorBellAddr;
						buf2[2] = 0x00;//扩展分机可能不同
						buf2[3] = 0xf8;
					}

				printf("door send: ");
				for (i = 0; i < 14; i++)
				{
					printf("%02x ", buf2[i]);
				}
				printf("\n");

				if (buf2[0] != 0)
				{
					sum = 0;
					for (i = 0; i < 13; i++)
					{
						sum += buf2[i];
					}
					buf2[13] = (-sum) & 0xff;

					write(m_serial1_fd, buf2, 14);
				}

				m_iDoorBellCmd = 0;
			}
			else if (m_iExtensionUnitCmd != 0)
			{
				memset(buf2, 0, 14);

				if (m_iExtensionUnitCmd == 0xfc)
				{
					buf2[0] = 0xf7;
					buf2[1] = 0xff;
					buf2[2] = 0x00;
					buf2[3] = 0xfc;
					buf2[4] = m_iExtensionUnitCallAddr;
				}
				else if (m_iExtensionUnitCmd == 0xfb)
				{
					buf2[0] = 0xf7;
					buf2[1] = 0xff;
					buf2[2] = 0x00;
					buf2[3] = 0xfb;
					buf2[4] = m_iExtensionUnitCallAddr;
				}
				else if (m_iExtensionUnitCmd == 0xfa)
				{
					buf2[0] = 0xf7;
					buf2[1] = 0xff;
					buf2[2] = 0x00;
					buf2[3] = 0xfa;
					buf2[4] = m_iExtensionUnitCallAddr;
				}
				else if (m_iExtensionUnitCmd == 0xf7)
				{
					buf2[0] = 0xf7;
					buf2[1] = g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->m_iMonitoredAddr;
					buf2[2] = 0x00;
					buf2[3] = 0xf7;
					buf2[4] = 0x00;
					buf2[5] = g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->m_iExtensionUnitAddr;
				}

				if (buf2[0] != 0)
				{
					sum = 0;
					for (i = 0; i < 13; i++)
					{
						sum += buf2[i];
					}
					buf2[13] = (-sum) & 0xff;

					printf("send: ");
					for (i = 0; i < 14; i++)
					{
						printf("%02x ", buf2[i]);
					}
					printf("\n");

					write(m_serial1_fd, buf2, 14);
				}

				m_iExtensionUnitCmd = 0;
			}
            else if (g_hcCmd == 1)
			{
				write(m_serial1_fd, g_hcSerialBuf, 14);
				g_hcCmd = 2;
			}

            //有防区延时报警超时
			if (m_bEnterDelayTimeOut)
			{
				break;
			}

			tv.tv_sec = 0;
			tv.tv_usec = 25000;

			FD_ZERO(&rd);
			FD_SET(m_serial1_fd, &rd);

			if (select(m_serial1_fd + 1, &rd, NULL, NULL, &tv) <= 0)
			{
                if(g_iDeviceType == TOUCHR)
                {
                    if(g_pMainDlg->m_pSecurityMainDlg->m_pDefenseGPIOScanThread->m_pHeadGPIODefenseData == NULL)
                    {
                        continue;
                    }
                    else
                    {
                        g_pMainDlg->m_pSecurityMainDlg->m_pDefenseGPIOScanThread->copyArray(buf);

                        cnt = 0;

                        cnt1 = 14;
                    }

                }
                else
                {
                    continue;
                }
			}
            else
            {
                cnt1 = read(m_serial1_fd, buf + cnt, 14 - cnt);

                if (cnt1 == 0)
                {
                    continue;
                }
            }
            /*
            printf("read data:");
            for (i = 0; i < cnt1; i++)
			{
				printf("%02x ", buf[i + cnt]);
			}
			printf("\n");
            */
			if ((g_hcCmd == 2) && (buf[0] != 0xf7) && (buf[cnt] == 0xfa))
			{
				g_hcCmd = 0;
				if (--cnt1 == 0) continue;
				memcpy(buf + cnt, buf + cnt + 1, cnt1);
			}

			if ((cnt == 0) && (buf[0] == (char)0xf7))
			{
				bFound = true;
				//cnt = cnt1;
			}

			if (!bFound)
			{
				for (i = 0; i < cnt1; i++)
				{
					if (buf[cnt + i] == (char)0xf7)
					{
						memcpy(buf, buf + cnt + i, cnt1 - i);
						cnt = cnt1 - i;
						bFound = true;
						break;
					}
				}
			}
			else
			//if (bFound && (i >= cnt1))
			{
				cnt += cnt1;
			}

			if (cnt >= 14)
			{
				break;
			}
		}

		if (!m_bRun) break;

		int bRecord = 0;
		QDateTime dateTime;

		if (m_bEnterDelayTimeOut)
		{
			for (i = 0; i < pSecurityMainDlg->m_wireDefenseCnt; i++)
			{
				if (pSecurityMainDlg->m_delayAlarmClass[i]->m_isTimeOut)
				{
					QList <QDateTime> *pList = &(pSecurityMainDlg->m_delayAlarmClass[i]->m_alertDateTimeList);
					unsigned char defenseType = pSecurityMainDlg->m_delayAlarmClass[i]->m_defenseType;
					int number = pSecurityMainDlg->m_allDefenseNumber[i];

					while (!pList->isEmpty())
					{
						dateTime = pList->takeFirst();

						if (alertRecord(defenseType, number - 1, bRecord == 0, dateTime))
						{
							bRecord = 1;
						}
					}

					pSecurityMainDlg->m_delayAlarmClass[i]->m_isTimeOut = 0;
				}
			}

			for (i = 0; i < pSecurityMainDlg->m_wirelessDefenseCnt;i++)
			{
				if (pSecurityMainDlg->m_delayAlarmClass[56 + i]->m_isTimeOut)
				{
					QList <QDateTime> *pList = &(pSecurityMainDlg->m_delayAlarmClass[56 + i]->m_alertDateTimeList);
					unsigned char defenseType = pSecurityMainDlg->m_delayAlarmClass[56 + i]->m_defenseType;
					int number = pSecurityMainDlg->m_allDefenseNumber[56 + i];

					while (!pList->isEmpty())
					{
						dateTime = pList->takeFirst();

						if (alertRecord(defenseType, number - 1, bRecord == 0, dateTime))
						{
							bRecord = 1;
						}
					}

					pSecurityMainDlg->m_delayAlarmClass[56 + i]->m_isTimeOut = 0;
				}
			}

			m_bEnterDelayTimeOut = 0;
		}

		if (cnt < 14)
		{
			if (bRecord)
			{
				printf("beginRecord\n");
				beginRecord();
			}

			continue;
		}

		sum = 0;
		for (i = 0; i < 14; i++)
		{
			sum += buf[i];
		}

		if ((sum & 0xff) != 0)
		{
			continue;
		}

		for (i = 0; i < 14; i++)
		{
			printf("%02x ", buf[i]);
		}
		printf("\n");

		printf("send 0xfa\n");
		ch = 0xfa;
		write(m_serial1_fd, &ch, 1);

		memcpy(&m_abnormalInfo, buf, 14);

		if (m_abnormalInfo.command != 0xff)
		{
			if ((m_abnormalInfo.SourceAddress >= 0x20) && (m_abnormalInfo.SourceAddress <= 0x2f))
			{
                //防止同时被呼和按门铃时视频铃声中断
                if( (g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->isVisible() == 1) && (eXosipElement->state == IDLE) )
                {
                    sleep(2);
                }
				if (m_abnormalInfo.command == 0xfd)
				{
//					if ( g_door_bell_type == 1 
//							&& ( !g_pMainDlg->m_pSecurityMainDlg->m_pSecurityAlertDlg->isVisible()
//					                && ( g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->isVisible() && g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->m_Record_Flag == false) ) )// 非可视门铃并且不在报警播放状态
//					{
//						printf("david door ring\n");
//						((Application *)qApp)->setLCDAndTimerOn();
//						emit sigShowAbnormalMsgBox3(tr("门铃响了..."));			 // 将实际的响铃放到消息处理函数中
//						if(m_pAutoQuitTimer->isActive()) m_pAutoQuitTimer->stop();
//						m_pAutoQuitTimer->start(20000);
//
//						continue;
//					}

					if ( g_door_bell_type == 1 )
					{
						if ( g_pMainDlg->m_pSecurityMainDlg->m_pSecurityAlertDlg->isVisible() )		// 报警窗体显示
							goto OTHERPROCESS;

						if ( g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->isVisible() && (g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->m_Record_Flag == true) )
							goto OTHERPROCESS;							// 留言状态

						printf("david door ring\n");
//						((Application *)qApp)->setLCDAndTimerOn();
						emit sigShowAbnormalMsgBox3(tr("门铃响了..."));			 // 将实际的响铃放到消息处理函数中
						if(m_pAutoQuitTimer->isActive()) m_pAutoQuitTimer->stop();
						m_pAutoQuitTimer->start(20000);
						continue;
					}

OTHERPROCESS:

                    if ( (g_door_bell_type == 2 || g_door_bell_type == 3) && (/*(g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->isVisible() && g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->m_Record_Flag == false) ||*/ g_pMainDlg->m_pVoipMainDlg->m_pIstDlg->isVisible() || g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->m_pRecvHookDlg->isVisible() ) )
					{
						printf("david1 door ring\n");
						emit sigShowAbnormalMsgBox3(tr("门铃响了..."));			 // 将实际的响铃放到消息处理函数中
						if(m_pAutoQuitTimer->isActive()) m_pAutoQuitTimer->stop();
						m_pAutoQuitTimer->start(20000);

						continue;
					}

					if (!g_pMainDlg->m_pDoorBellDlg->isVisible())////lg lg || g_pMainDlg->m_pDoorBellDlg->ui->pushButtonPrevDoor->isEnabled())
					{
						m_iDoorBellAddr = m_abnormalInfo.SourceAddress;
						
						if ( g_door_bell_type == 3 || g_door_bell_type == 2 || g_door_bell_type == 1 )
							emit sigOpenDoorBellDlg();
					}
					else
					{
						tmp_iPeerAddr = m_abnormalInfo.SourceAddress;
						m_iDoorBellCmd = 0xf9;
						if ( g_door_bell_type == 3 || g_door_bell_type == 2 || g_door_bell_type == 1  )   // 当可视门铃或刷卡可视门铃
							emit sigCloseDoorBellDlg();
					}
				}
				else if (m_abnormalInfo.command == 0xfa)
				{
					if (g_pMainDlg->m_pDoorBellDlg->isVisible())
					{
						if ( g_door_bell_type == 3 || g_door_bell_type == 2 || g_door_bell_type == 1  )
							emit sigCloseDoorBellDlg();
					}
					else if (g_pMainDlg->m_pVoipMainDlg->isVisible() && g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->isVisible())
					{
						emit sigCloseMonDlg();
					}
				}
			}
			else if ((m_abnormalInfo.SourceAddress >= 0x30) && (m_abnormalInfo.SourceAddress <= 0x3f))
			{
				if ((m_abnormalInfo.parameter[0] >= 0x20) && (m_abnormalInfo.parameter[0] <= 0x2f))
				{
					if (m_abnormalInfo.command == 0xfb)
					{
						g_pMainDlg->m_pDoorBellDlg->m_bExtensionUnitAnswer = true;
						g_pMainDlg->m_pDoorBellDlg->m_iExtensionUnitAddr = m_abnormalInfo.SourceAddress;

						emit sigExtensionUnitAnswerDoorBell();
					}
					else if (m_abnormalInfo.command == 0xfa)
					{
						if (g_pMainDlg->m_pDoorBellDlg->isVisible()/* && g_pMainDlg->m_pDoorBellDlg->m_bExtensionUnitAnswer*/)//david 2014.6.26
						{
							g_pMainDlg->m_pDoorBellDlg->m_iExtensionUnitAddr = m_abnormalInfo.SourceAddress;

							if ( g_door_bell_type == 3 || g_door_bell_type == 2 || g_door_bell_type == 1 )
								emit sigCloseDoorBellDlg();
						}
						else if (g_pMainDlg->m_pVoipMainDlg->isVisible() && g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->isVisible()
							&& g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->m_bExtensionUnitMon)
						{
							g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->m_iExtensionUnitAddr = m_abnormalInfo.SourceAddress;

							((Application *)qApp)->setLCDAndTimerOn();
							emit sigCloseMonDlg();
						}
					}
				}
				else if (m_abnormalInfo.command == 0xf7)
				{
					if (g_doorbell_on == 0)
					{
						if ((!g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->isVisible()) || (g_pViewCam == NULL))
						{
							g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->m_bExtensionUnitMon = true;
							g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->m_iExtensionUnitAddr = m_abnormalInfo.SourceAddress;
							g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->m_iMonitoredAddr = m_abnormalInfo.DetinationAddress;

							m_iExtensionUnitCmd = 0xf7;

							((Application *)qApp)->setLCDOn();
							emit sigExtensionUnitMon();
						}
					}

				}
				else
				//{
					//lg
					if (m_abnormalInfo.command == 0xfb)
					{
						g_pMainDlg->m_pVoipMainDlg->m_pIstDlg->m_bExtensionUnitAnswer = true;
						g_pMainDlg->m_pVoipMainDlg->m_pIstDlg->m_iExtensionUnitAddr = m_abnormalInfo.SourceAddress;

						emit sigExtensionUnitAnswer();
					}
					else if (m_abnormalInfo.command == 0xfa)
					{
						if (g_pMainDlg->m_pVoipMainDlg->m_pIstDlg->m_bExtensionUnitAnswer)
						{
							g_pMainDlg->m_pVoipMainDlg->m_pIstDlg->m_iExtensionUnitAddr = m_abnormalInfo.SourceAddress;

							emit sigExtensionUnitHangUp();
						}
					}
					else if (m_abnormalInfo.command == 0xf8)
					{
						if (g_pMainDlg->m_pVoipMainDlg->m_pIstDlg->m_bExtensionUnitAnswer)
						{
							emit sigExtensionUnitOpen();
						}
					}
				//}

				//f7 02 30 f8 00 00 00 00 00 00 00 00 00 df
				//f7 ff 30 fb 02 00 00 00 00 00 00 00 00 dd
				//f7 ff 30 fa 02 00 00 00 00 00 00 00 00 de
				//监控：f7 20 33 f7 00 00 00 00 00 00 00 00 00 bf
			}

			if (bRecord)
			{
				beginRecord();
			}

			continue;
		}

		dateTime = QDateTime::currentDateTime();

		if (m_abnormalInfo.parameter[0] == (unsigned char)0x00)//有线防区
		{
			char para;
			QSettings settings("/mnt/disk/config/defense", QSettings::IniFormat);
			int defenseType = 0xa0;
			int base_addr = 0;

			int specialDefense;
			int doorDefense8;

			QSettings set(CONFIG_FILE,QSettings::IniFormat);

			specialDefense = set.value("alert_param/special_defense","0").toInt();
			doorDefense8 = settings.value("wiredefense/defense" + QString::number(8)).toInt();//用来判断第8防区当前状态

			if (m_abnormalInfo.SourceAddress == 0x01)
			{
				base_addr = 0;
			}
			else if (m_abnormalInfo.SourceAddress >= 0x10)
			{
				base_addr = (m_abnormalInfo.SourceAddress - 0x10 + 1) * 8;
			}

			para = m_abnormalInfo.parameter[1];

			printf("para:%02x base_addr:%02x\n", para, base_addr);

			char data = para;

            if((base_addr == 0) && !(data >>7) &&(doorDefense8 == 1))
            {
                if(m_pExitDelayTimer->isActive() && specialDefense)
                {
                    continue;
                }

                if((!m_pExitDelayTimer->isActive()) && specialDefense)
                {
                    printf("david door ring\n");
                    ((Application *)qApp)->setLCDAndTimerOn();

                    if(!g_pMainDlg->m_bInterFree && eXosipElement->state == IDLE)
                    {
                        if(b_ring_play_run)
                        {
                            continue;
                        }

                        my_ring_start((char *)"wav/doorbell.wav",200);
                        my_ring_set_level(g_pMainDlg->m_pVoipMainDlg->m_nRingVolume);;

                        m_pDoorRingTimer->start(20000);
                    }

                     emit sigShowAbnormalMsgBox2(tr("门铃响了..."));

                     if(m_pAutoQuitTimer->isActive()) m_pAutoQuitTimer->stop();

                     m_pAutoQuitTimer->start(20000);

                     continue;
                }
            }


			QString strAbnormal;
			//int bRecord = 0;

			for (i = 0; i < pSecurityMainDlg->m_wireDefenseCnt; i++)
			{
				int number = pSecurityMainDlg->m_allDefenseNumber[i];

				if ((number - 1 < base_addr) || (number - 1 >= base_addr + 8))
				{
					continue;
				}

				if (((para >> (number - 1 - base_addr)) & 0x01) != 0)
				{
					printf("lg wire %d 1\n", number);
					settings.setValue("wiredefense/defense" + QString::number(number), 1);
					g_pMainDlg->m_pSecurityMainDlg->m_pDefenseGPIOScanThread->DefenseGpioStatus[number-1] = 1;
				}
				else
				{
					int tmp;
                    tmp = settings.value("wiredefense/defense" + QString::number(number)).toInt();
					settings.setValue("wiredefense/defense" + QString::number(number), 0);

					printf("lg wire %d 0\n", number);

					if (tmp == 1)
					{
						//由正常编程异常（即电平由高变低),触发门铃动作

						if(m_pDeviceStartTimer->isActive())
						{
							continue;
						}

						printf("lg wire trigger %d 0\n", number);

						if (pSecurityMainDlg->m_delayAlarmClass[i]->m_repeatAlarmLastDateTime.secsTo(dateTime) < pSecurityMainDlg->m_iRepeatAlarmFilterInterval)
						{
							printf("\tRepeat alarm filter\n");
							continue;
						}
						printf("m_pDelayTimer:%p,i:%d\n",pSecurityMainDlg->m_delayAlarmClass[i]->m_pDelayTimer,i);
						if (pSecurityMainDlg->m_delayAlarmClass[i]->m_pDelayTimer)
						{
							printf("exit delay 0\n");
							if (m_pExitDelayTimer->isActive())
							{
								printf("Exit delay\n");
								continue;
							}

							QSettings settings("/mnt/disk/config/defense.conf", QSettings::IniFormat);
							settings.setIniCodec(QTextCodec::codecForName("UTF8"));

							int property = settings.value("wiredefense" + QString::number(number) + "/property", "-1").toInt();

							if (pSecurityMainDlg->m_securityState == STATUS_DISARM)
							{
								if ((property == DEFENSE_PROPERTY_ATHOME) || (property == DEFENSE_PROPERTY_OUTSIDE))
								{
									continue;
								}
							}
							else if (pSecurityMainDlg->m_securityState == STATUS_ATHOME)
							{
								if (property == DEFENSE_PROPERTY_ATHOME)
								{
									continue;
								}
							}

							pSecurityMainDlg->m_delayAlarmClass[i]->m_repeatAlarmLastDateTime = dateTime;
							pSecurityMainDlg->m_delayAlarmClass[i]->m_alertDateTimeList.append(dateTime);

							if (!pSecurityMainDlg->m_delayAlarmClass[i]->m_pDelayTimer->isActive())
							{
								printf("Delay alarm wait start\n");
								pSecurityMainDlg->m_delayAlarmClass[i]->m_pDelayTimer->start(pSecurityMainDlg->m_iEnterDelay * 1000);
							}
						}
						else
						{
							if (alertRecord(defenseType, number - 1, bRecord == 0, dateTime))
							{
								pSecurityMainDlg->m_delayAlarmClass[i]->m_repeatAlarmLastDateTime = dateTime;

								bRecord = 1;
							}
						}
					}
					else if (tmp == -1)
					{
						printf("lg wire no trigger %d 0\n", number);
						QSettings settings("/mnt/disk/config/defense.conf", QSettings::IniFormat);
						settings.setIniCodec(QTextCodec::codecForName("UTF8"));

						int bDelayAlarm, bBypass;

						bDelayAlarm = settings.value("wiredefense" + QString::number(number) + "/delay_alarm", "0").toInt();
						bBypass = settings.value("wiredefense" + QString::number(number) + "/bypass", "0").toInt();

						if (bBypass)
						{
							printf("\tStart bypass\n");
							continue;
						}

						if (strAbnormal != "") strAbnormal += "\n";
						strAbnormal += QObject::tr("有线防区") + " " + QString::number(number) + " " + tr("启动时处于异常状态");
					}
				}
			}

			if (strAbnormal != "")
			{
				if ((g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pSysSetMainDlg)
					&& (g_pMainDlg->m_pSysSetMainDlg->m_pAddDefenseDlg->isVisible()))
				{
					emit sigShowAbnormalMsgBox1(strAbnormal);
				}
				else
				{
					emit sigShowAbnormalMsgBox2(strAbnormal);
				}

				//emit sigShowAbnormalMsgBox(strAbnormal);

				continue;
			}
			//m_bAlertDlgOpened = false;
		}
		else if (m_abnormalInfo.parameter[0] == (unsigned char)0x01)
		{
			int id;
			int i;
			unsigned char buf3[4];
			QSettings settings2("/mnt/disk/config/defense.conf", QSettings::IniFormat);
			settings2.setIniCodec(QTextCodec::codecForName("UTF8"));
			int defenseType = 0xa1;

			memcpy(buf3, &m_abnormalInfo.parameter[1], 4);
			id = (buf3[0] << 24) + (buf3[1] << 16) + (buf3[2] << 8) + buf3[3];

			for (i = 0; i < pSecurityMainDlg->m_wirelessDefenseCnt; i++)
			{
				int tmp;
				int number = pSecurityMainDlg->m_allDefenseNumber[56 + i];
				tmp = settings2.value("wirelessdefense" + QString::number(number) + "/identity").toInt();

				if (tmp == id)
				{
					/*if (buf[3] == 0xff)
					{
						printf("lg wireless %d 0\n", i);
						settings.setValue("wirelessdefense/defense" + QString::number(number), 0);
					}
					else
					{
						int tmp2;

						tmp2 = settings.value("wirelessdefense/defense" + QString::number(number)).toInt();
						settings.setValue("wirelessdefense/defense" + QString::number(number), 1);

						if(tmp2 == 0)
						{
							printf("lg wireless %d 1\n", i);
							alertRecord(defenseType, i, 1, dateTime);
						}
					}

					break;*/

					if (buf[3] == 0xff)
					{
						if (pSecurityMainDlg->m_delayAlarmClass[56 + i]->m_repeatAlarmLastDateTime.secsTo(dateTime) < pSecurityMainDlg->m_iRepeatAlarmFilterInterval)
						{
							printf("\twireless Repeat alarm filter\n");
							continue;
						}
						if (pSecurityMainDlg->m_delayAlarmClass[56 + i]->m_pDelayTimer)
						{
							if (m_pExitDelayTimer->isActive())
							{
								printf("Exit delay\n");
								continue;
							}

							int property = settings2.value("wirelessdefense" + QString::number(number) + "/property", "-1").toInt();

							if (pSecurityMainDlg->m_securityState == STATUS_DISARM)
							{
								if ((property == DEFENSE_PROPERTY_ATHOME) || (property == DEFENSE_PROPERTY_OUTSIDE))
								{
									continue;
								}
							}
							else if (pSecurityMainDlg->m_securityState == STATUS_ATHOME)
							{
								if (property == DEFENSE_PROPERTY_ATHOME)
								{
									continue;
								}
							}

							pSecurityMainDlg->m_delayAlarmClass[56 + i]->m_repeatAlarmLastDateTime = dateTime;
							pSecurityMainDlg->m_delayAlarmClass[56 + i]->m_alertDateTimeList.append(dateTime);

							if (!pSecurityMainDlg->m_delayAlarmClass[56 + i]->m_pDelayTimer->isActive())
							{
								printf("Delay alarm wait start\n");
								pSecurityMainDlg->m_delayAlarmClass[56 + i]->m_pDelayTimer->start(pSecurityMainDlg->m_iEnterDelay * 1000);
							}
						}
						else
						{
							if (alertRecord(defenseType, number - 1, bRecord == 0, dateTime))
							{
								pSecurityMainDlg->m_delayAlarmClass[56 + i]->m_repeatAlarmLastDateTime = dateTime;

								bRecord = 1;
							}
					   }

					}

					break;
				}
			}
		}

		if (bRecord)
		{
			beginRecord();
		}
	}

}

/************************************************************
描述：门铃回调函数
参数：无
返回：无
************************************************************/
void Serial1Thread::Recall_Ring()
{
	if (!g_pMainDlg->m_bInterFree && eXosipElement->state == IDLE)
	{
	    if(b_ring_play_run)
	    {
		    return;
	    }

	    my_ring_start((char *)"wav/doorbell.wav", 2000);
	    my_ring_set_level(g_pMainDlg->m_pVoipMainDlg->m_nRingVolume);//david 2014.4.8

	    m_pDoorRingTimer->start(20000);
	}
}

/************************************************************
描述：启动JPEG发送线程和启动报警录像
参数：无
返回：无
************************************************************/
void Serial1Thread::beginRecord()
{
	printf("david beginRecord\n");
    closeMessBox(g_pMainDlg);

	if (g_pMainDlg->m_pInputMethodWidget)
	{
		g_pMainDlg->m_pInputMethodWidget->hide();
	}    

        int b_m_number=m_number-1;
        openRecorderCnt[b_m_number]=m_openRecorderCnt++;
        strDateTime[b_m_number]=m_strDateTime;
        number[b_m_number]=m_number;
        property[b_m_number]=m_property;
        channel[b_m_number]=m_channel;
        name[b_m_number]=m_name;
        stype[b_m_number]=m_type;
        strLocation[b_m_number]=m_strLocation;
        strAViPath[b_m_number]=m_strAviPath;

        emit openRecorder(m_number,openRecorderCnt[b_m_number],
                          strDateTime[b_m_number],
                          number[b_m_number],
                          property[b_m_number],
                          channel[b_m_number],
                          name[b_m_number],
                          stype[b_m_number],
                          strLocation[b_m_number],
                          strAViPath[b_m_number]);

}

/************************************************************
描述：播放报警声，将此次报警存成报警信息文件，更新安防管理窗口的报警记
	 录列表
参数：defenseType - 0xa0:有线防区
				   0xa1:无线防区
	 defenseIdx - (防区号 - 1)
	 bIsRecord - 1:除了记录此次报警外，还要启动报警录像
				 0:只记录此次报警外，不启动报警录像
	 dateTime - 报警时间
返回：1 - 成功
	 0 - 失败
************************************************************/
int Serial1Thread::alertRecord(unsigned char defenseType, int defenseIdx, int bIsRecord, QDateTime dateTime)
{
	SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();

	QSettings settings("/mnt/disk/config/defense.conf", QSettings::IniFormat);

	settings.setIniCodec(QTextCodec::codecForName("UTF8"));

	int type = 0, number = 0, name = 0, channel = 0, property = 0;
	int bDelayAlarm = 0, bBypass = 0;
	QString strLocation, strDateTime, strAviPath;

	if (defenseType == (unsigned char)0xa0)
	{
		type = DEFENSE_TYPE_WIRE;//有线防区
		number = settings.value("wiredefense" + QString::number(defenseIdx + 1) + "/number", "-1").toInt();
		name = settings.value("wiredefense" + QString::number(defenseIdx + 1) + "/type", "-1").toInt();
		channel = settings.value("wiredefense" + QString::number(defenseIdx + 1) + "/channel", "-1").toInt();
		property = settings.value("wiredefense" + QString::number(defenseIdx + 1) + "/property", "-1").toInt();
		strLocation = settings.value("wiredefense" + QString::number(defenseIdx + 1) + "/location", "").toString();
		bDelayAlarm = settings.value("wiredefense" + QString::number(defenseIdx + 1) + "/delay_alarm", "0").toInt();
		bBypass = settings.value("wiredefense" + QString::number(defenseIdx + 1) + "/bypass", "0").toInt();
	}
	else if (defenseType == (unsigned char)0xa1)
	{
		type = DEFENSE_TYPE_WIRELESS;//无线防区
		number = settings.value("wirelessdefense" + QString::number(defenseIdx + 1) + "/number", "-1").toInt();
		name = settings.value("wirelessdefense" + QString::number(defenseIdx + 1) + "/type", "-1").toInt();
		channel = settings.value("wirelessdefense" + QString::number(defenseIdx + 1) + "/channel", "-1").toInt();
		property = settings.value("wirelessdefense" + QString::number(defenseIdx + 1) + "/property", "-1").toInt();
		strLocation = settings.value("wirelessdefense" + QString::number(defenseIdx + 1) + "/location", "").toString();
	}

	if (number == -1) return 0;

	if (bBypass)
	{
		printf("\tBypass\n");
		return 0;
	}

	if (pSecurityMainDlg->m_securityState == STATUS_DISARM)
	{
		if ((property == DEFENSE_PROPERTY_ATHOME) || (property == DEFENSE_PROPERTY_OUTSIDE))
        {
			return 0;
		}
	}
	else if (pSecurityMainDlg->m_securityState == STATUS_ATHOME)
	{
		if (property == DEFENSE_PROPERTY_ATHOME)
        {
			return 0;
		}
	}


	char filepath[PATH_MAX];
	FILE *fp;

	sprintf(filepath, "/mnt/disk/sec_info/%04d%02d%02d-%02d%02d%02d-%03d.txt", dateTime.date().year(), dateTime.date().month(),
			dateTime.date().day(), dateTime.time().hour(), dateTime.time().minute(), dateTime.time().second(),
            //david 2014.4.10
            number/*dateTime.time().msec()*/);

    	if (bIsRecord)
	{
		strAviPath.sprintf("/mnt/disk/sec_media/%04d%02d%02d-%02d%02d%02d-%03d.avi", dateTime.date().year(), dateTime.date().month(),
			dateTime.date().day(), dateTime.time().hour(), dateTime.time().minute(), dateTime.time().second(),
            //david 2014.4.10
             number/*dateTime.time().msec()*/);
	}

	fp = fopen(filepath, "wt");
	if (fp == NULL)	return 0;

	fprintf(fp, "DEFENSE_TYPE:%d\n", type);
	fprintf(fp, "DEFENSE_NUMBER:%d\n",	number);
	fprintf(fp, "DEFENSE_NAME:%d\n", name);
	fprintf(fp, "DEFENSE_LOCATION:%s\n", strLocation.toUtf8().data());
	fprintf(fp, "UNREAD:1\n");

	fclose(fp);

    	if ((g_bIstOrAlertBusy == 0) && (!((SecurityMainDialog *)parent())->m_pSecurityAlertDlg->isVisible()) && bIsRecord)//!m_bAlertDlgOpened)
	{
		MainDialog *pMainDlg = (MainDialog *)(parent()->parent());
		if (!pMainDlg->m_pVoipMainDlg->isVisible())
		{
			my_ring_stop();
		}

		if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
			usleep(50000);;
        	playWaveFile((char *)"wav/alert.wav", ((MainDialog *)(parent()->parent()))->m_pVoipMainDlg->m_nRingVolume, 1);
	}
											// 将list新信息的记录移动到sigRefrashList中进行，因为分线程不进行GUI操作
	emit sigRefrashList(dateTime,number,type,name,strLocation);			// 通知securitymaindialog类进行多余文件的清除以及list控件的更新

	if (!pSecurityMainDlg->m_bSecurityMainDlgAloneOpened)
	{
        pSecurityMainDlg->m_newDefenseCnt++;
        /* david 2014/7/9
		QSettings settings_config(CONFIG_FILE, QSettings::IniFormat);
		settings_config.setValue("info/new_defense_cnt", pSecurityMainDlg->m_newDefenseCnt);
        */
		g_pMainDlg->emitSigSetNotify(1);
	}

	m_bIsRecord = false;
	if (bIsRecord)
	{
		m_type = type;
		m_number = number;
		m_name = name;
		m_channel = channel;
		m_property = property;
		m_strLocation = strLocation;

		strDateTime.sprintf("%d-%d-%d %d:%02d", dateTime.date().year(), dateTime.date().month(), dateTime.date().day(),
			dateTime.time().hour(), dateTime.time().minute());//, dateTime.time().second());
		m_strDateTime = strDateTime;
		m_strAviPath = strAviPath;
		m_bIsRecord = bIsRecord;
	}

	return 1;
}

/************************************************************
描述：协议探测,管理机与物业软件是否支持使用udp协议接收数据
参数：
返回：
************************************************************/
void Serial1Thread::CheckUdpState()
{
    addr_info_t    addr_manager,addr_computer;
    char loc_ip[20] = {'\0'} ;
    char loc_usr[20] = {'\0'} ;
    char remmanager_ip[20] = {'\0'};
    char remmanager_usr[20] = {'\0'};
    char remcomputer_ip[20] = {'\0'};
    char remcomputer_usr[20] = {'\0'};
    char strIP[50];

    GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;
    strcpy(loc_ip, pGetPeerIPThread->m_nativeIP.toLatin1().data());
    strcpy(loc_usr, pGetPeerIPThread->m_nativeName.toLatin1().data());

    for (int i = 0; i < 2; i++)
    {
        if (pGetPeerIPThread->GetPeerIP(g_strLphoneName[i + 1], strIP) != 0)
        {
            continue;
        }

        char *ptr;
        int iModalType = BASIC_MODAL_TYPE;

        ptr = strstr(strIP, "?");
        if (ptr)
        {
            iModalType = atoi(ptr + strlen("?"));
            *ptr = '\0';
        }

        if (iModalType != MANCENTER_MODAL_TYPE)
        {
            continue ;
        }

        if(i==0)
        {
            strcpy(remmanager_ip,strIP);
            strcpy(remmanager_usr,g_strLphoneName[i + 1]);
        }
        else
        {
            strcpy(remcomputer_ip,strIP);
            strcpy(remcomputer_usr,g_strLphoneName[i + 1]);
        }
    }
    printf("remmanager_ip=%s........................................\n",remmanager_ip);
    printf("remmanager_usr=%s........................................\n",remmanager_usr);
    printf("remcopmuter_ip=%s........................................\n",remcomputer_ip);
    printf("remcomputer_usr=%s........................................\n",remcomputer_usr);
    addr_manager.loc_ip = (uint8_t *)loc_ip;
    addr_manager.loc_usr = (uint8_t *)loc_usr;
    addr_manager.rem_ip = (uint8_t *)remmanager_ip;
    addr_manager.rem_usr = (uint8_t *)remmanager_usr;
    if(*remmanager_ip != '\0')
    {
        int ret1 = start_system_0x0501(&addr_manager, manager_result_callback);
        printf("start_system_0x0501 ret1 = %d\n",ret1);

        int time_count = 0;
        while (1)
        {
            usleep(100 * 1000);
            if (g_check_managemachine_udp_state != RESULT_NULL || time_count++ >= 60)
            {
                break;
            }
        }

        stop_system_0x0501();
    }

    addr_computer.loc_ip = (uint8_t *)loc_ip;
    addr_computer.loc_usr = (uint8_t *)loc_usr;
    addr_computer.rem_ip = (uint8_t *)remcomputer_ip;
    addr_computer.rem_usr = (uint8_t *)remcomputer_usr;
    if(*remcomputer_ip != '\0')
    {
        int ret2 = start_system_0x0501(&addr_computer, computer_result_callback);
        printf("start_system_0x0501 ret2 = %d\n",ret2);

        int time_count = 0;
        while (1)
        {
            usleep(100 * 1000);
            if (g_check_computercenter_udp_state != RESULT_NULL || time_count++ >= 60)
            {
                break;
            }
        }

        stop_system_0x0501();
    }
}
/************************************************************
描述：使用udp协议发送报警信息
参数：num : 报警防区号
     destination : 为0表示向管理机发送，为1表示向物业软件发送
返回：无
************************************************************/
void Serial1Thread::SendByUdp(int num ,int destination)
{
    area_record_t area;
    addr_info_t addr;
    name_t picdata[6] = {'\0'};
    unsigned char buf[6][64] = {'\0'};
    int i;
    char pic_name[64] = {'\0'};
    char loc_ip[20] = {'\0'};
    char loc_usr[20] = {'\0'};
    char rem_ip[20] = {'\0'};
    char rem_usr[20] = {'\0'};
    char strIP[50] = {'\0'};

    GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;
    strcpy(loc_ip, pGetPeerIPThread->m_nativeIP.toLatin1().data());
    strcpy(loc_usr, pGetPeerIPThread->m_nativeName.toLatin1().data());

    if (pGetPeerIPThread->GetPeerIP(g_strLphoneName[destination + 1], strIP) != 0)
    {
        return ;
    }
    char *ptr;
    ptr = strstr(strIP, "?");
    if (ptr)
    {
       *ptr = '\0';
    }

    strcpy(rem_ip,strIP);
    strcpy(rem_usr,g_strLphoneName[destination + 1]);


    for(i=0;i<6;i++)
    {
        memset(pic_name,'\0',30);
        sprintf(pic_name, "%s_%s-%d-%d.jpg",pGetPeerIPThread->m_nativeName.toLocal8Bit().data(),m_AlarmDateTime.toString("yyyyMMddhhmmss").toLocal8Bit().data(),num,i);
        picdata[i].name = buf                                                                                                                                                                                     [i];
        memcpy(picdata[i].name,pic_name,strlen(pic_name));
        *(picdata[i].name+strlen(pic_name))='\0';
        picdata[i].name_len = (uint8_t)strlen(pic_name);
    }

    addr.loc_ip = (uint8_t *)loc_ip;
    addr.loc_usr = (uint8_t *)loc_usr;
    addr.rem_ip = (uint8_t *)rem_ip;
    addr.rem_usr = (uint8_t *)rem_usr;
    area.area_type = (uint8_t)stype[num-1];
    area.area_num = (uint8_t)number[num-1];
    area.area_name = (uint8_t)name[num-1];
    area.cam_channel = (uint8_t)channel[num-1];
    area.area_attrib = (uint8_t)property[num-1];
    area.area_len = (uint8_t)strlen(strLocation[num-1].toLocal8Bit().data());
    area.area_addr = (uint8_t*)strLocation[num-1].toLocal8Bit().data();
    area.alarm_time[0] = 0x14;
    area.alarm_time[1] = 0x0f;
    area.alarm_time[2] = 0x0a;
    area.alarm_time[3] = 0x0f;
    area.alarm_time[4] = 0x0e;
    area.alarm_time[5] = 0x0d;
    area.alarm_time[6] = 0x13;

    bool ret = start_security_0x0003(&addr, &area, 6, picdata, NULL);
    printf("start_security_0x0003 ret = %d\n",ret);

}
/************************************************************
描述：使用tcp协议发送报警信息
参数：num : 报警防区号
     i : 为0表示向管理机发送，为1表示向物业软件发送
返回：
************************************************************/
void Serial1Thread::SendByTcp(int num , int i)
{
    m_pJpegSendThread->ConnectToManCenter(i);

    int b_num=num-1;
    m_pJpegSendThread->SendAlertInfo(stype[b_num], number[b_num], name[b_num], channel[b_num], property[b_num], strLocation[b_num],  strDateTime[b_num],i);
}
/************************************************************
描述：启动上传报警信息线程
参数：num:报警防区号
返回：
************************************************************/
void Serial1Thread::SendAlarmInfo(int num)
{
    g_mainMonitorFlag = 1;
    if(m_pSendAlarmInfoThread->m_bRun)
    {
        m_pSendAlarmInfoThread->m_bRun = false;
        m_pSendAlarmInfoThread->quit();
    }

    m_pSendAlarmInfoThread->wait();

    if(!m_pSendAlarmInfoThread->m_bRun)
    {
        m_pSendAlarmInfoThread->m_num = num;
        m_pSendAlarmInfoThread->start();
    }
}

/************************************************************
描述：初始化串口1
参数：无
返回：true - 成功
	 false - 失败
************************************************************/
bool Serial1Thread::uartInit()
{
	struct termios Opt;

	if (m_serial1_fd != -1)
	{
		::close(m_serial1_fd);
	}

#ifdef ARM11
	m_serial1_fd = ::open("/dev/ttySAC1", O_RDWR | O_NOCTTY | O_NONBLOCK);
#endif
#ifdef CORTEXA7
	m_serial1_fd = ::open("/dev/tty7", O_RDWR | O_NOCTTY | O_NONBLOCK);
#endif
	if (m_serial1_fd < 0)
	{
		return false;
	}

	tcgetattr(m_serial1_fd, &Opt);      //初始化
	tcflush(m_serial1_fd, TCIFLUSH);
	cfsetispeed(&Opt, B9600); //设置波特率
	cfsetospeed(&Opt, B9600);

	Opt.c_cflag |= CS8;       //设置数据位
	Opt.c_cflag &= ~PARENB;
	Opt.c_oflag &= ~(OPOST);
	Opt.c_cflag &= ~CSTOPB;
	Opt.c_lflag &= ~(ICANON|ISIG|ECHO|IEXTEN);
	Opt.c_iflag &= ~(INPCK|BRKINT|ICRNL|ISTRIP|IXON);

	Opt.c_cc[VMIN] = 0;
	Opt.c_cc[VTIME] = 0;
	tcsetattr(m_serial1_fd, TCSANOW, &Opt); //装载初始化参数

	return true;
}

/*void Serial1Thread::getDefenseStatus()
{
	QSettings settings("/mnt/disk/config/defense", QSettings::IniFormat);
	unsigned char buf[14];
	char ch;
	int cnt;
	int i ;
	int sum;
	int repeat_cnt;

	for (i = 0; i < 8; i++)
	{
		settings.setValue("wirelessdefense/defense" + QString::number(i + 1), 0);
	}

	//uartInit();

	repeat_cnt = 0;

repeat_send:
	memset(buf,0,14);

	buf[0] = 0xf7;
	buf[1] = 0x01;
	buf[2] = 0xfa;
	buf[3] = 0xff;

	sum = 0;
	for (i = 0; i < 13; i++)
	{
		sum += buf[i];
	}
	buf[13] = (-sum) & 0xff;

	tcflush(m_serial1_fd, TCIOFLUSH);

	cnt = write(m_serial1_fd, buf, 14);

	usleep(200000);

	i = 0;
	cnt = 0;
	while (i < 600/200) //600 mseconds
	{
		cnt += read(m_serial1_fd, buf + cnt, 14 - cnt);
		if (cnt >= 14) break;
		i++;
		usleep(200000);
	}

	sum = 0;
	for(i = 0; i < 14; i++)
	{int
		sum += buf[i];
	}

	//确认收到数据，并释放串口资源
	ch = 0xfa;
	write(m_serial1_fd, &ch, 1);

	if ((cnt < 14) || ((sum & 0xff) != 0))
	{
		if (repeat_cnt++ < 3)
		{
			goto repeat_send;
		}

		return;
	}

	char para;//有线防区状态标志
	int status;
	para = buf[4];

	printf("wiredefense:%02x\n", para);

	for (i = 0; i < 8; i++)
	{
		status = ((para << i) & 0x80) != 0;
		settings.setValue("wiredefense/defense" + QString::number(i + 1), status);
	}
}
*/
