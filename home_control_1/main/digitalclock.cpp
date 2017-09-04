#include "digitalclock.h"
#include <QTime>
#include "maindialog.h"
//#include "exosip_main.h"

/************************************************************
  该文件为主窗口数字时钟绘画的主程序
************************************************************/

//extern eXosip_element_t *eXosipElement;
extern MainDialog *g_pMainDlg;

/************************************************************
描述：数字时钟部件构造函数
参数：parent - 该部件的父窗口指针
返回：无
************************************************************/
DigitalClock::DigitalClock(QObject *parent) :
	QLCDNumber((QWidget *)parent)
{
	QPalette p = palette();
	p.setColor(QPalette::Normal, QPalette::WindowText,Qt::red);
	setPalette(p);

	setSegmentStyle(QLCDNumber::Filled);

	m_pTimer = NULL;
	m_bShowColon = true;
	m_bMode24 = true;
}

/************************************************************
描述：数字时钟部件析构函数
参数：无
返回：无
************************************************************/
DigitalClock::~DigitalClock()
{
	if (m_pTimer)
	{
		m_pTimer->stop();
		delete m_pTimer;
	}
}

/************************************************************
描述：启动数字时钟定时器，每500毫秒执行一次回调函数
参数：bMode24 - true: 24小时模式
			   false: 12小时模式
返回：无
************************************************************/
void DigitalClock::ClockRun(bool bMode24)
{
	if (m_pTimer != NULL)
	{
		m_pTimer->stop();
		delete m_pTimer;
	}

	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(showDigTime()));
	m_bShowColon = true;
	m_bMode24 = bMode24;

	setNumDigits(bMode24 ? 8 : 10);//显示格式为7位,如：2:12:56

	m_pTimer->start(500);
	showDigTime();
}

/************************************************************
描述：停止并删除数字时钟定时器
参数：无
返回：无
************************************************************/
void DigitalClock::ClockStop()
{
	if (m_pTimer)
	{
		m_pTimer->stop();
		delete m_pTimer;
		m_pTimer = NULL;
	}
}

/************************************************************
描述：数字时钟定时器超时回调函数，每500毫秒钟调用一次，获取系统时间后
	 更新数字时钟
参数：无
返回：无
************************************************************/
void DigitalClock::showDigTime()
{
	/*MainDialog *pMainDlg = (MainDialog *)parent();

	while (pMainDlg->objectName() != "MainDialog")
	{
		pMainDlg = (MainDialog *)(pMainDlg->parent());
	}*/

	//if (pMainDlg->m_nShowSubDlgCnt > 0) return;

	if ((g_pMainDlg->m_pCurActiveDlg != g_pMainDlg)
		&& ((g_pMainDlg->m_pCurActiveDlg != g_pMainDlg->m_pSysSetMainDlg)
		|| ((parent()->objectName() != "TimeMode2Dialog")
		&& (parent()->objectName() != "TimeMode3Dialog"))))
	{
		return;
	}
	/*if (pMainDlg->m_pVoipMainDlg->m_pIctDlg->m_pRecvHookDlg->isVisible()) return;
	if (pMainDlg->m_pVoipMainDlg->m_pIstDlg->isVisible()) return;
	if (pMainDlg->m_pVoipMainDlg->m_pMonDlg->isVisible()) return;
	if (pMainDlg->m_pMediaManDlg->m_pMediaPlayerDlg->isVisible()) return;	if (pMainDlg->m_pCallCenterDlg->isVisible()) return;
	if (pMainDlg->m_pSecurityMainDlg->m_pSecurityPlayerDlg->isVisible()) return;
	if (pMainDlg->m_pSecurityMainDlg->m_pSecurityAlertDlg->isVisible()) return;
	if (pMainDlg->m_pCallCenterDlg->isVisible()) return;
	*/

	m_bShowColon = !m_bShowColon;

	if (!m_bShowColon)
	{
		QTime time = QTime::currentTime();

		if (m_bMode24)
		{
			m_Text = time.toString("hh:mm:ss");
		}
		else
		{
			m_Text = time.toString("A hh:mm:ss");
			m_Text = m_Text.left(1) + m_Text.mid(2);
		}
	}

	if (m_bShowColon)
	{
		if (m_bMode24)
		{
			m_Text[2] = ':';
			m_Text[5] = ':';
		}
		else
		{
			m_Text[4] = ':';
			m_Text[7] = ':';
		}
	}
	else
	{
		if (m_bMode24)
		{
			m_Text[2] = ' ';
			m_Text[5] = ' ';
		}
		else
		{
			m_Text[4] = ' ';
			m_Text[7] = ' ';
		}
	}

	display(m_Text);
}
