#include "keypadreadthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include "maindialog.h"
#include "application.h"
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/mman.h>
#include "ui_mondialog.h"
#include "ui_istdialog.h"
#include "ui_maindialog.h"
#include "ui_doorbelldialog.h"

/************************************************************
  该文件为终端右边5个按键处理线程的主程序
************************************************************/

extern MainDialog *g_pMainDlg;
extern int g_bIstOrAlertBusy;

extern bool g_Digital_Channel_Flag;				// 标识数字通道的变量
/************************************************************
描述：按键处理线程构造函数
参数：parent - 该线程的父对象指针
返回：无
************************************************************/
KeypadReadThread::KeypadReadThread(QObject *parent) :
    QThread(parent)
{
	MainDialog *pMainDlg = (MainDialog *)parent;

	connect(this, SIGNAL(showMonDlg()), pMainDlg->m_pVoipMainDlg->m_pMonDlg, SLOT(showMonDlg()));

	connect(this, SIGNAL(sigSendTerminalHelp()), pMainDlg, SLOT(sendTerminalHelp()));

	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), pMainDlg, SLOT(openCalibration()));

	connect(this, SIGNAL(sigSetLCDAndTimerOn()), (Application *)qApp, SLOT(setLCDAndTimerOn()));

    connect(this, SIGNAL(sigCloseDoorBellDlg()), pMainDlg->m_pDoorBellDlg, SLOT(on_pushButtonDoorBellReturn_pressed()));
}

/************************************************************
描述：按键处理线程析构函数
参数：无
返回：无
************************************************************/
KeypadReadThread::~KeypadReadThread()
{
	delete m_pTimer;
}

/************************************************************
描述：按键处理线程主函数，5个按钮从上到下的功能为：1.终端求助，2.长按5
	 秒启动触摸屏校准，3.打开视频监控窗口，4.窗口按钮Tab转移，5.窗口按
	 钮回调函数触发
参数：无
返回：无
************************************************************/
void KeypadReadThread::run()
{
	int fd;
	int ret;
	unsigned char KeyValue, tmp;

	fd = open("/dev/CapKey_driver", O_RDONLY);

	if (fd < 0) return;

	m_bRun = true;
	while (m_bRun)
	{
		usleep(100000);

		struct pollfd fds;
		memset(&fds,0,sizeof(fds));
		fds.events = POLLIN;
		fds.fd = fd;
		ret = poll(&fds, 1, 100);

		if (ret <= 0) continue;

		ret = read(fd, &KeyValue, sizeof(unsigned char));
		tmp = KeyValue;
		tmp = tmp & 0x80;
		KeyValue = KeyValue & 0x7f;
        printf("keypatreadthread value:%d\n",KeyValue);
		processKey(tmp, KeyValue);
	}

	close(fd);
}

void KeypadReadThread::processKey(int tmp, int KeyValue)
{
	MainDialog *pMainDlg = (MainDialog *)parent();

	if (!pMainDlg->isVisible()) return;

	if (pMainDlg->m_pSysSetMainDlg->m_pLanguageSetDlg->m_bIsSettingLanguage)
	{
		m_pTimer->stop();
		return;
	}

	QEvent::Type keyType;
	int key = 0;

	if (!((Application *)qApp)->m_bTimerOff) //bug
	{
		emit sigSetLCDAndTimerOn();
	}

	printf("-------------keyvalue:%d %d\n", KeyValue, tmp);

	if (tmp == 0)
	{
		keyType = QEvent::KeyPress;

		int volume = 1024;
		volume = pMainDlg->m_pVoipMainDlg->m_nRingVolume;

		if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
			usleep(50000);;
		playWaveFile((char *)"wav/keytone.wav", volume, 1);
	}
	else
	{
		keyType = QEvent::KeyRelease;
	}

	if (g_pMainDlg->m_pCalibrationDlg->isVisible())// || (g_doorbell_on == 1))
	{
		return;
	}

	if (KeyValue == 3)
	{
		if (tmp == 0)
		{
#if 1
			if (!g_pMainDlg->ui->pushButtonTerminalHelp->isEnabled())
			{
				return;
			}

			emit sigSendTerminalHelp();

#else
			char strFileName[256];
			/*int cnt = 0;

			while (1)
			{
				sprintf(strFileName, "./print_screen/%d.bmp", cnt);

				if (access(strFileName, F_OK) == -1)
				{
					break;
				}

				cnt++;
			}*/

			int i;
			QDialog *pDlg;
			QObjectList q;

			pDlg = g_pMainDlg;
			do
			{
				q = pDlg->children();
				for (i = q.length() - 1; i >= 0; i--)
				{
					QObject* o = q.at(i);

					if (o->inherits("QDialog"))
					{
						QDialog *d = qobject_cast <QDialog *>(o);
						if (d->isVisible())
						{
							pDlg = d;
							break;
						}
					}
				}
			}
			while (i >= 0);

			QString strLang;
			switch (g_pMainDlg->m_inputMethodMode)
			{
			case 0:
				strLang = "en";
				break;
			case 1:
				strLang = "zh_CN";
				break;
			case 2:
				strLang = "zh_TW";
				break;
			}

			sprintf(strFileName, "./print_screen/%s-%s.bmp", pDlg->objectName().toLocal8Bit().data(),
					strLang.toLocal8Bit().data());

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

			QImage image((uchar *)fb_addr, 800, 480, QImage::Format_RGB16);
			if (image.save(strFileName, "bmp"))
			{
				printf("bmp saved OK: %s\n", strFileName);
			}
			else
			{
				printf("bmp saved failed: %s\n", strFileName);
			}

			munmap(fb_addr, fb_size);
			::close(fb_fd);
#endif
		}
	}
	else if (KeyValue == 4)
	{
		if (tmp == 0)
		{
            bool m = mainIsShow(); //david 2014.5.13
            if (m && !pMainDlg->m_pVoipMainDlg->m_pMonDlg->isVisible())
			{
				emit showMonDlg();
                g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->m_pFirstCamTimer->start(100);
			}
			else
            {
                QWidget *pWidgetTemp;
                key = Qt::Key_Backtab;
                QWidget *pWidget = QApplication::focusWidget();

                if(pWidget == NULL) return;

                if(keyType == QEvent::KeyPress)
                {
                    QWidget *pOldWidget = pWidget;

                    do
                    {
                        QWidget *pOldWidget2 = pWidget;

                        QKeyEvent modifiedEvent(QEvent::KeyPress, key, Qt::NoModifier);
                        QApplication::sendEvent((QObject *)pWidget, &modifiedEvent);

                        pWidget = QApplication::focusWidget();

                        if ((pWidget == pOldWidget) || (pWidget == pOldWidget2)) break;


                        pWidgetTemp = (QWidget *)(pWidget->parent());

                        if ((!pWidgetTemp) || (pWidgetTemp->objectName() != "MyMessageBox")
                            || (pWidgetTemp->parent() == g_pMainDlg->m_pCurActiveDlg)
                            || (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pSysSetMainDlg)
                            || (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pSysSetEngineerDlg))
                        {
                            break;
                        }

                        QKeyEvent modifiedEvent2(QEvent::KeyRelease, key, Qt::NoModifier);
                        QApplication::sendEvent((QObject *)pWidget, &modifiedEvent2);

                    } while (1);

                }
                else
                {
                    QKeyEvent modifiedEvent(keyType, key, Qt::NoModifier);
                    QApplication::sendEvent((QObject *)pWidget, &modifiedEvent);
                }
			}
		}

	}
	else if (KeyValue == 5)
	{
		if (tmp == 0)
		{
			m_pTimer->start(5000);
		}
		else
		{
			m_pTimer->stop();


            		callElevator();
		}
	}
	else if (KeyValue == 1)
	{
		QWidget *pWidgetTemp;

		if ((g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->m_pRecvHookDlg->m_pFrameFullshow->isVisible())
			|| (g_pMainDlg->m_pVoipMainDlg->m_pIstDlg->m_pFrameFullshow->isVisible())
			|| (g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->m_pFrameFullshow->isVisible())
			|| (g_pMainDlg->m_pCallCenterDlg->m_pFrameFullshow->isVisible()))
		{
			return;
		}

		if (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pVoipMainDlg->m_pIstDlg)
		{
			if (keyType == QEvent::KeyPress)
			{
				if (g_pMainDlg->m_pVoipMainDlg->m_pIstDlg->ui->pushButtonIstAnswer->isEnabled())
				{
					g_pMainDlg->m_pVoipMainDlg->m_pIstDlg->on_pushButtonIstAnswer_pressed();
				}
				else if (g_pMainDlg->m_pVoipMainDlg->m_pIstDlg->m_bTalking)
				{
					g_pMainDlg->m_pVoipMainDlg->m_pIstDlg->on_pushButtonIstHangUp_pressed();
				}
			}

			return;
		}
		else if (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pDoorBellDlg)
		{
			if (keyType == QEvent::KeyPress)
			{
                if (g_pMainDlg->m_pDoorBellDlg->ui->pushButtonDoorBellAnswer->isEnabled())
				{
                    g_pMainDlg->m_pDoorBellDlg->on_pushButtonDoorBellAnswer_pressed();
				}
				else
				{
                    emit sigCloseDoorBellDlg();
				}
			}

			return;
		}

		key = Qt::Key_Space; //Qt::Key_Enter

		QWidget *pWidget = QApplication::focusWidget();

		if (pWidget == NULL) return;

		//printf("pWidget:%p %s\n", pWidget, pWidget->objectName().toLocal8Bit().data());

		pWidgetTemp = pWidget;

		if (pWidgetTemp)
		{
			pWidgetTemp = (QWidget *)(pWidgetTemp->parent());

			if ((!pWidgetTemp) || (pWidgetTemp->objectName() != "MyMessageBox") //"mymsgbox")
				|| (pWidgetTemp->parent() == g_pMainDlg->m_pCurActiveDlg)
				|| (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pSysSetMainDlg)
				|| (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pSysSetEngineerDlg))
			{
				QKeyEvent *modifiedEvent = new QKeyEvent(keyType, key, Qt::NoModifier);
				QApplication::postEvent((QObject *)pWidget, modifiedEvent);
			}
		}
	}
	else if (KeyValue == 2)
	{
		QWidget *pWidgetTemp;

		if ((g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->m_pRecvHookDlg->m_pFrameFullshow->isVisible())
			|| (g_pMainDlg->m_pVoipMainDlg->m_pIstDlg->m_pFrameFullshow->isVisible())
			|| (g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->m_pFrameFullshow->isVisible())
			|| (g_pMainDlg->m_pCallCenterDlg->m_pFrameFullshow->isVisible()))
		{
			return;
		}

		if (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pVoipMainDlg->m_pIstDlg)
		{
            if (/*(keyType == QEvent::KeyPress) &&*/ g_pMainDlg->m_pVoipMainDlg->m_pIstDlg->ui->pushButtonIstOpen->isEnabled())
			{
				QKeyEvent *modifiedEvent = new QKeyEvent(keyType, Qt::Key_Space, Qt::NoModifier);
                		QApplication::postEvent((QObject *)g_pMainDlg->m_pVoipMainDlg->m_pIstDlg->ui->pushButtonIstOpen, modifiedEvent);
			}

			return;
		}
		else if (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pDoorBellDlg)
		{
			{
				QKeyEvent *modifiedEvent = new QKeyEvent(keyType, Qt::Key_Space, Qt::NoModifier);
				QApplication::postEvent((QObject *)g_pMainDlg->m_pDoorBellDlg->ui->pushButtonDoorBellOpen, modifiedEvent);
			}

			return;
		}

		key = Qt::Key_Tab;

		QWidget *pWidget = QApplication::focusWidget();

		if (pWidget == NULL) return;

		if (keyType == QEvent::KeyPress)
		{
			QWidget *pOldWidget = pWidget;

			do
			{
				QWidget *pOldWidget2 = pWidget;

				QKeyEvent modifiedEvent(QEvent::KeyPress, key, Qt::NoModifier);
				QApplication::sendEvent((QObject *)pWidget, &modifiedEvent);

				pWidget = QApplication::focusWidget();

				if ((pWidget == pOldWidget) || (pWidget == pOldWidget2)) break;

				pWidgetTemp = (QWidget *)(pWidget->parent());

				if ((!pWidgetTemp) || (pWidgetTemp->objectName() != "MyMessageBox") //"mymsgbox")
					|| (pWidgetTemp->parent() == g_pMainDlg->m_pCurActiveDlg)
					|| (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pSysSetMainDlg)
					|| (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pSysSetEngineerDlg))
				{
					break;
				}

				QKeyEvent modifiedEvent2(QEvent::KeyRelease, key, Qt::NoModifier);
				QApplication::sendEvent((QObject *)pWidget, &modifiedEvent2);
			} while (1);
		}
		else
		{
			QKeyEvent modifiedEvent(keyType, key, Qt::NoModifier);
			QApplication::sendEvent((QObject *)pWidget, &modifiedEvent);
		}
	}
}

void KeypadReadThread::callElevator()
{
	int loop_cnt = 0;
	while (g_bIstOrAlertBusy)
	{
		usleep(5000);

		if (loop_cnt++ >= 8) //40 ms
		{
			qApp->processEvents();
			loop_cnt = 0;
		}
	}

	g_bIstOrAlertBusy = 5;

	g_pMainDlg->m_pVoipMainDlg->closeVoipDialog();
	g_pMainDlg->m_pVoipMainDlg->closeOtherRelatedDialog();
	g_pMainDlg->m_pVoipMainDlg->hide();

	QObjectList q = g_pMainDlg->children();
	for (int i = q.length() - 1; i >= 0; i-- )
	{
		QObject* o = q.at(i);

		if (o->inherits("QDialog") && (o->objectName() != "MyMessageBox"))
		{
			QDialog* d = qobject_cast<QDialog*>(o);
			if (d->isVisible())
			{
				d->hide();
			}
		}
	}

	g_bIstOrAlertBusy = 0;

	g_pMainDlg->ui->pushButtonCallElevator->click();
}

bool KeypadReadThread::mainIsShow()
{
	QObjectList q = g_pMainDlg->children();
	for (int i = q.length() - 1; i >= 0; i-- )
	{
		QObject* o = q.at(i);

		if (o->inherits("QDialog") && (o->objectName() != "MyMessageBox"))
		{
			QDialog* d = qobject_cast<QDialog*>(o);
			if (d->isVisible())
			{
				return false;
			}
		}
	}

	return true;
}
