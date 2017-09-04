#include "clock.h"
#include "ui_clock.h"
#include <QPainter>
#include <QTime>
#include <stdio.h>
#include "maindialog.h"
//#include "exosip_main.h"
#include "datetimesetthread.h"

/************************************************************
  该文件为主窗口模拟时钟绘画的主程序
************************************************************/

//extern eXosip_element_t *eXosipElement;

extern MainDialog *g_pMainDlg;

/************************************************************
  时针、分针、秒针的四边形端点坐标
************************************************************/
static QPoint sec[4] = {QPoint(0, -94), QPoint(2, 0), QPoint(0, 40), QPoint(-2, 0)};
static QPoint min[4] = {QPoint(0, -77), QPoint(6, 0), QPoint(0, 12), QPoint(-6, 0)};
static QPoint hour[4] = {QPoint(0, -50), QPoint(8, 0), QPoint(0, 13), QPoint(-8, 0)};

//QPoint sed2[4], min2[4], hour2[4];

/************************************************************
描述：模拟时钟部件构造函数
参数：parent - 该部件的父窗口指针
返回：无
************************************************************/
Clock::Clock(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Clock)
{
	ui->setupUi(this);

	//将clock类设置为圆形控件,直径为250
	//QRect rc = geometry();
	//QRegion maskedRegion(3, 3, rc.width() - 6, rc.height() - 6, QRegion::Ellipse);
	//setMask(maskedRegion);

	m_pPixmap = NULL;
	m_pTimer = NULL;

	memcpy(m_pntHour, hour, sizeof(m_pntHour));
	memcpy(m_pntMin, min, sizeof(m_pntMin));
	memcpy(m_pntSec, sec, sizeof(m_pntSec));
}

/************************************************************
描述：模拟时钟部件析构函数
参数：无
返回：无
************************************************************/
Clock::~Clock()
{
	if (m_pTimer)
	{
		m_pTimer->stop();
		delete m_pTimer;
	}

	if (m_pPixmap)
	{
		delete m_pPixmap;
	}

	delete ui;
}

/************************************************************
描述：启动模拟时钟定时器，每秒钟执行一次回调函数
参数：无
返回：无
************************************************************/
void Clock::ClockRun()
{
	QRect rc = geometry();
	QRegion maskedRegion(-2, -2, rc.width() + 4, rc.height() + 4, QRegion::Ellipse);
	setMask(maskedRegion);

	m_pTimer = new QTimer(this); //定时器
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(callBack()));
	m_pTimer->start(1000);

    m_pUpdatetimetimer = new QTimer(this);
    connect(m_pUpdatetimetimer,SIGNAL(timeout()),this,SLOT(slotUpdateTimer()));
}

/************************************************************
描述：停止并删除模拟时钟定时器
参数：无
返回：无
************************************************************/
void Clock::ClockStop()
{
	if (m_pTimer)
	{
		m_pTimer->stop();
		//printf("bcom double free 8\n");
		delete m_pTimer;
		m_pTimer = NULL;
	}
}

/************************************************************
+描述：十二点后五分钟内同步物业时间
+参数：无
+返回：无
+************************************************************/
void Clock::slotUpdateTimer()
{
    if(m_pUpdatetimetimer->isActive())
    {
        m_pUpdatetimetimer->stop();
    }
    DateTimeSetThread *pDateTimeSetThread = g_pMainDlg->m_pSysSetMainDlg->m_pDatetimeSetDlg->m_pDateTimeSetThread;
    if(pDateTimeSetThread->m_bRun)
    {
        pDateTimeSetThread->m_bRun = false;
        pDateTimeSetThread->quit();
        pDateTimeSetThread->wait();
    }
    pDateTimeSetThread->m_autoUpdateTimeFlag = true;
    pDateTimeSetThread->start();
}
/************************************************************
描述：模拟时钟定时器超时回调函数，每秒钟调用一次，获取系统时间后重绘模
	 拟时钟表盘和指针
参数：无
返回：无
************************************************************/
void Clock::callBack()
{       
    QDateTime datetime = QDateTime::currentDateTime();
    int h = datetime.time().hour();
    int mi = datetime.time().minute();
    int s = datetime.time().second();
    if(h == 12 && mi == 0 && s == 0)
    {
        srand(int(time(0)));
        m_pUpdatetimetimer->start((1 + rand()%6000) * 50);
    }
	//printf("lg %s\n", g_pMainDlg->m_pCurActiveDlg->objectName().toLocal8Bit().data());
	if ((g_pMainDlg->m_pCurActiveDlg != g_pMainDlg)
		&& ((g_pMainDlg->m_pCurActiveDlg != g_pMainDlg->m_pSysSetMainDlg)
		|| (parent()->objectName() != "TimeMode1Dialog")))
	{
		return;
	}

	//printf("lg clock::callback()\n");

	/*MainDialog *pMainDlg = (MainDialog *)parent();

	while (pMainDlg->objectName() != "MainDialog")
	{
		pMainDlg = (MainDialog *)(pMainDlg->parent());
	}

	//if (pMainDlg->m_nShowSubDlgCnt > 0) return;

	if (pMainDlg->m_pVoipMainDlg->m_pIctDlg->m_pRecvHookDlg->isVisible()) return;
	if (pMainDlg->m_pVoipMainDlg->m_pIstDlg->isVisible()) return;
	if (pMainDlg->m_pVoipMainDlg->m_pMonDlg->isVisible()) return;
	//if (pMainDlg->m_pVoipMainDlg->m_pMctDlg->m_pHookDlg->isVisible()) return;
	if (pMainDlg->m_pMediaManDlg->m_pMediaPlayerDlg->isVisible()) return;
	if (pMainDlg->m_pSecurityMainDlg->m_pSecurityPlayerDlg->isVisible()) return;
	if (pMainDlg->m_pSecurityMainDlg->m_pSecurityAlertDlg->isVisible()) return;
	if (pMainDlg->m_pCallCenterDlg->isVisible()) return;
	*/
	update();
}

/************************************************************
描述：设置模拟时钟表盘背景图，根据模拟时钟部件大小重新生成时分秒指针四
	 边形端点坐标
参数：pic_path - 模拟时钟表盘背景图路径
返回：1 - 成功
	 0 - 失败
************************************************************/
int Clock::setBackgroundImage(QString pic_path)
{
	int i;
	QRect rc = geometry();
	int w, h;

	w = rc.width();
	h = rc.height();

	for (i = 0; i < 4; i++)
	{
		m_pntHour[i].setX(hour[i].x()/250.0 * w);
		m_pntHour[i].setY(hour[i].y()/250.0 * h);
	}

	for (i = 0; i < 4; i++)
	{
		m_pntMin[i].setX(min[i].x()/250.0 * w);
		m_pntMin[i].setY(min[i].y()/250.0 * h);
	}

	for (i = 0; i < 4; i++)
	{
		m_pntSec[i].setX(sec[i].x()/250.0 * w);
		m_pntSec[i].setY(sec[i].y()/250.0 * h);
	}

	m_picPath = pic_path;

	//QRegion maskedRegion(3, 3, w - 6, h - 6, QRegion::Ellipse);
	//setMask(maskedRegion);

	QString pix_path;
	if (strstr(QT_BUILD_KEY, "arm") == 0) //not arm-linux-gcc
	{
		pix_path = QString::fromUtf8(pic_path.toLatin1().data());
	}
	else
	{
		pix_path = pic_path;
	}
	if (m_pPixmap) delete m_pPixmap;
	m_pPixmap = new QPixmap(pix_path);

	return ((m_pPixmap->width() != 0) && (m_pPixmap->height() != 0)) ? 0 : -1;
}

/************************************************************
描述：模拟时钟的自绘函数
参数：pe - 绘画事件
返回：无
************************************************************/
void Clock::paintEvent(QPaintEvent *pe)//QPaintEvent *
{
	Q_UNUSED(pe);

	QPainter painter(this);
	QTime time = QTime::currentTime();
	QRect rc = geometry();

	if (m_pPixmap)
	{
		painter.drawPixmap(0, 0, rc.width(), rc.height(), *m_pPixmap);
	}

	painter.translate(rc.width()/2, rc.height()/2);//改变钟表的中心位置
	painter.setRenderHint(QPainter::Antialiasing);        //消除锯齿
	//时指针
	painter.setBrush(Qt::black);//blue);
	painter.setPen(Qt::darkGray);
	painter.save();
	painter.rotate(30.0 * time.hour()+ time.minute()/2.0);
	painter.drawConvexPolygon(m_pntHour, 4);
	painter.restore();
	//分指针
	painter.setBrush(Qt::black);
	painter.setPen(Qt::darkGray);
	painter.save();
	painter.rotate(6.0 * time.minute() + time.second()/10.0);
	painter.drawConvexPolygon(m_pntMin, 4);
	painter.restore();
	//秒指针

	painter.setBrush(Qt::black);
	painter.setPen(Qt::darkGray);
	painter.save();
	painter.rotate(6.0 * time.second());
	painter.drawConvexPolygon(m_pntSec , 4);
	painter.restore();
}
