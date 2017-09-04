#include "calendar.h"
#include "ui_calendar.h"
#include <QDateTime>
#include <QLabel>
//#include <QMessageBox>
#include <stdio.h>
#include "maindialog.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
/************************************************************
  该文件为主窗口界面日历部分的主程序
************************************************************/

extern MainDialog *g_pMainDlg;

/************************************************************
 每个月1日之前的天数，2月只算28天
************************************************************/
static int month_add[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};//每个月1日之前的天数，2月只算28天

/************************************************************
 每个月第一个节气出现的规律
************************************************************/
static unsigned char CalendarSolarTermTable01[12][33] = {
	{7, 6, 6, 6, 6, 6, 6, 6, 6, 5, 6, 6, 6, 5, 5, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 4, 5, 5}, //month 1
	{5, 4, 5, 5, 5, 4, 4, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 3, 3, 4, 4, 3, 3, 3}, //2
	{6, 6, 6, 7, 6, 6, 6, 6, 5, 6, 6, 6, 5, 5, 6, 6, 5, 5, 5, 6, 5, 5, 5, 5, 4, 5, 5, 5, 5}, //3
	{5, 5, 6, 6, 5, 5, 5, 6, 5, 5, 5, 5, 4, 5, 5, 5, 4, 4, 5, 5, 4, 4, 4, 5, 4, 4, 4, 4, 5}, //4
	{6, 6, 6, 7, 6, 6, 6, 6, 5, 6, 6, 6, 5, 5, 6, 6, 5, 5, 5, 6, 5, 5, 5, 5, 4, 5, 5, 5, 5}, //5
	{6, 6, 7, 7, 6, 6, 6, 7, 6, 6, 6, 6, 5, 6, 6, 6, 5, 5, 6, 6, 5, 5, 5, 6, 5, 5, 5, 5, 4, 5, 5, 5, 5}, //6
	{7, 8, 8, 8, 7, 7, 8, 8, 7, 7, 7, 8, 7, 7, 7, 7, 6, 7, 7, 7, 6, 6, 7, 7, 6, 6, 6, 7, 7}, //7
	{8, 8, 8, 9, 8, 8, 8, 8, 7, 8, 8, 8, 7, 7, 8, 8, 7, 7, 7, 8, 7, 7, 7, 7, 6, 7, 7, 7, 6, 6, 7, 7, 7}, //8
	{8, 8, 8, 9, 8, 8, 8, 8, 7, 8, 8, 8, 7, 7, 8, 8, 7, 7, 7, 8, 7, 7, 7, 7, 6, 7, 7, 7, 7}, //9
	{9, 9, 9, 9, 8, 9, 9, 9, 8, 8, 9, 9, 8, 8, 8, 9, 8, 8, 8, 8, 7, 8, 8, 8, 7, 7, 8, 8, 8}, //10
	{8, 8, 8, 8, 7, 8, 8, 8, 7, 7, 8, 8, 7, 7, 7, 8, 7, 7, 7, 7, 6, 7, 7, 7, 6, 6, 7, 7, 7}, //11
	{7, 8, 8, 8, 7, 7, 8, 8, 7, 7, 7, 8, 7, 7, 7, 7, 6, 7, 7, 7, 6, 6, 7, 7, 6, 6, 6, 7, 7}, //12
}; //这个数据表表示了每个月第一个节气出现的规律

/************************************************************
 每个月第一个节气出现规律对应的阳历年份范围
************************************************************/
static unsigned char CalendarSolarTermYear01[12][9] = {
	{13, 49, 85, 117, 149, 185, 201, 250, 250}, //month 1
	{13, 45, 81, 117, 149, 185, 201, 250, 250}, //2
	{13, 48, 84, 112, 148, 184, 200, 201, 250}, //3
	{13, 45, 76, 108, 140, 172, 200, 201, 250}, //4
	{13, 44, 72, 104, 132, 168, 200, 201, 250}, //5
	{ 5, 33, 68,  96, 124, 152, 188, 200, 201}, //6
	{29, 57, 85, 120, 148, 176, 200, 201, 250}, //7
	{13, 48, 76, 104, 132, 168, 196, 200, 201}, //8
	{25, 60, 88, 120, 148, 184, 200, 201, 250}, //9
	{16, 44, 76, 108, 144, 172, 200, 201, 250}, //10
	{28, 60, 92, 124, 160, 192, 200, 201, 250}, //11
	{17, 53, 85, 124, 156, 188, 200, 201, 250}, //12
}; //这个数据表表示了每个月第一个节气出现规律对应的阳历年份范围

/************************************************************
 每个月第二个节气出现的规律
************************************************************/
static unsigned char CalendarSolarTermTable02[12][29] = {
	{21,21,21,21,21,20,21,21,21,20,20,21,21,20,20,20,20,20,20,20,20,19,20,20,20,19,19,20},
	{20,19,19,20,20,19,19,19,19,19,19,19,19,18,19,19,19,18,18,19,19,18,18,18,18,18,18,18},
	{21,21,21,22,21,21,21,21,20,21,21,21,20,20,21,21,20,20,20,21,20,20,20,20,19,20,20,20,20},
	{20,21,21,21,20,20,21,21,20,20,20,21,20,20,20,20,19,20,20,20,19,19,20,20,19,19,19,20,20},
	{21,22,22,22,21,21,22,22,21,21,21,22,21,21,21,21,20,21,21,21,20,20,21,21,20,20,20,21,21},
	{22,22,22,22,21,22,22,22,21,21,22,22,21,21,21,22,21,21,21,21,20,21,21,21,20,20,21,21,21},
	{23,23,24,24,23,23,23,24,23,23,23,23,22,23,23,23,22,22,23,23,22,22,22,23,22,22,22,22,23},
	{23,24,24,24,23,23,24,24,23,23,23,24,23,23,23,23,22,23,23,23,22,22,23,23,22,22,22,23,23},
	{23,24,24,24,23,23,24,24,23,23,23,24,23,23,23,23,22,23,23,23,22,22,23,23,22,22,22,23,23},
	{24,24,24,24,23,24,24,24,23,23,24,24,23,23,23,24,23,23,23,23,22,23,23,23,22,22,23,23,23},
	{23,23,23,23,22,23,23,23,22,22,23,23,22,22,22,23,22,22,22,22,21,22,22,22,21,21,22,22,22},
	{22,22,23,23,22,22,22,23,22,22,22,22,21,22,22,22,21,21,22,22,21,21,21,22,21,21,21,21,22},
}; //这个数据表表示了每个月第二个节气出现的规律

/************************************************************
 每个月第二个节气出现规律对应的阳历年份范围
************************************************************/
static unsigned char CalendarSolarTermYear02[12][8] = {
	{13,45,81,113,149,185,201},{21,57,93,125,161,193,201},
	{21,56,88,120,152,188,200,201},
	{21,49,81,116,144,176,200,201},{17,49,77,112,140,168,200,201},
	{28,60,88,116,148,180,200,201},{25,53,84,112,144,172,200,201},
	{29,57,89,120,148,180,200,201},{17,45,73,108,140,168,200,201},
	{28,60,92,124,160,192,200,201},{16,44,80,112,148,180,200,201},
	{17,53,88,120,156,188,200,201},};
//这个数据表表示了每个月第二个节气出现规律对应的阳历年份范围

static const char *enMonth[] =
{
	"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
};

/************************************************************
 新历月份
************************************************************/
static const char *newChMonth[] =
{
    "", "一月", "二月", "三月", "四月", "五月", "六月", "七月", "八月", "九月", "十月", "十一月", "十二月",
};

/************************************************************
 农历日期
************************************************************/
static const char *ChDay[] =
{
	QT_TRANSLATE_NOOP("Calendar", ""), QT_TRANSLATE_NOOP("Calendar", "初一"), QT_TRANSLATE_NOOP("Calendar", "初二"), QT_TRANSLATE_NOOP("Calendar", "初三"), QT_TRANSLATE_NOOP("Calendar", "初四"), QT_TRANSLATE_NOOP("Calendar", "初五"),//第一个字节为空字符，即没有字符
	QT_TRANSLATE_NOOP("Calendar", "初六"), QT_TRANSLATE_NOOP("Calendar", "初七"), QT_TRANSLATE_NOOP("Calendar", "初八"), QT_TRANSLATE_NOOP("Calendar", "初九"), QT_TRANSLATE_NOOP("Calendar", "初十"),
	QT_TRANSLATE_NOOP("Calendar", "十一"), QT_TRANSLATE_NOOP("Calendar", "十二"), QT_TRANSLATE_NOOP("Calendar", "十三"), QT_TRANSLATE_NOOP("Calendar", "十四"), QT_TRANSLATE_NOOP("Calendar", "十五"),
	QT_TRANSLATE_NOOP("Calendar", "十六"), QT_TRANSLATE_NOOP("Calendar", "十七"), QT_TRANSLATE_NOOP("Calendar", "十八"), QT_TRANSLATE_NOOP("Calendar", "十九"), QT_TRANSLATE_NOOP("Calendar", "二十"),
	QT_TRANSLATE_NOOP("Calendar", "廿一"), QT_TRANSLATE_NOOP("Calendar", "廿二"), QT_TRANSLATE_NOOP("Calendar", "廿三"), QT_TRANSLATE_NOOP("Calendar", "廿四"), QT_TRANSLATE_NOOP("Calendar", "廿五"),
	QT_TRANSLATE_NOOP("Calendar", "廿六"), QT_TRANSLATE_NOOP("Calendar", "廿七"), QT_TRANSLATE_NOOP("Calendar", "廿八"), QT_TRANSLATE_NOOP("Calendar", "廿九"), QT_TRANSLATE_NOOP("Calendar", "三十")
};
/************************************************************
 农历月份
************************************************************/
static const char *ChMonth[] = {QT_TRANSLATE_NOOP("Calendar", ""), QT_TRANSLATE_NOOP("Calendar", "正月"), QT_TRANSLATE_NOOP("Calendar", "二月"), QT_TRANSLATE_NOOP("Calendar", "三月"), QT_TRANSLATE_NOOP("Calendar", "四月"), QT_TRANSLATE_NOOP("Calendar", "五月"),
								QT_TRANSLATE_NOOP("Calendar", "六月"), QT_TRANSLATE_NOOP("Calendar", "七月"), QT_TRANSLATE_NOOP("Calendar", "八月"), QT_TRANSLATE_NOOP("Calendar", "九月"), QT_TRANSLATE_NOOP("Calendar", "十月"), QT_TRANSLATE_NOOP("Calendar", "十一月"), QT_TRANSLATE_NOOP("Calendar", "腊月")};
/************************************************************
 星期字符串
************************************************************/
static const char *Week[] = {QT_TRANSLATE_NOOP("Calendar", "星期日"), QT_TRANSLATE_NOOP("Calendar", "星期一"), QT_TRANSLATE_NOOP("Calendar", "星期二"), QT_TRANSLATE_NOOP("Calendar", "星期三"), QT_TRANSLATE_NOOP("Calendar", "星期四"), QT_TRANSLATE_NOOP("Calendar", "星期五"), QT_TRANSLATE_NOOP("Calendar", "星期六")};
/************************************************************
 24节气字符串
************************************************************/
static const char *jieqi[] =
{	QT_TRANSLATE_NOOP("Calendar", ""),
	QT_TRANSLATE_NOOP("Calendar", "小寒"), QT_TRANSLATE_NOOP("Calendar", "大寒"), QT_TRANSLATE_NOOP("Calendar", "立春"), QT_TRANSLATE_NOOP("Calendar", "雨水"), QT_TRANSLATE_NOOP("Calendar", "惊蛰"), QT_TRANSLATE_NOOP("Calendar", "春分"), QT_TRANSLATE_NOOP("Calendar", "清明"), QT_TRANSLATE_NOOP("Calendar", "谷雨"), QT_TRANSLATE_NOOP("Calendar", "立夏"), QT_TRANSLATE_NOOP("Calendar", "小满"), QT_TRANSLATE_NOOP("Calendar", "芒种"), QT_TRANSLATE_NOOP("Calendar", "夏至"),
	QT_TRANSLATE_NOOP("Calendar", "小暑"), QT_TRANSLATE_NOOP("Calendar", "大暑"), QT_TRANSLATE_NOOP("Calendar", "立秋"), QT_TRANSLATE_NOOP("Calendar", "处暑"), QT_TRANSLATE_NOOP("Calendar", "白露"), QT_TRANSLATE_NOOP("Calendar", "秋分"), QT_TRANSLATE_NOOP("Calendar", "寒露"), QT_TRANSLATE_NOOP("Calendar", "霜降"), QT_TRANSLATE_NOOP("Calendar", "立冬"), QT_TRANSLATE_NOOP("Calendar", "小雪"), QT_TRANSLATE_NOOP("Calendar", "大雪"), QT_TRANSLATE_NOOP("Calendar", "冬至")
};
/************************************************************
 节日字符串
************************************************************/
static const char *jieri[] =
{	QT_TRANSLATE_NOOP("Calendar", ""),//0
	QT_TRANSLATE_NOOP("Calendar", "元旦"), QT_TRANSLATE_NOOP("Calendar", "情人节"), QT_TRANSLATE_NOOP("Calendar", "妇女节"), QT_TRANSLATE_NOOP("Calendar", "植树节"), QT_TRANSLATE_NOOP("Calendar", "愚人节"),//1到5
	QT_TRANSLATE_NOOP("Calendar", "红十字节"), QT_TRANSLATE_NOOP("Calendar", "劳动节"), QT_TRANSLATE_NOOP("Calendar", "青年节"), QT_TRANSLATE_NOOP("Calendar", "儿童节"), QT_TRANSLATE_NOOP("Calendar", "教师节"),//6到10
	QT_TRANSLATE_NOOP("Calendar", "国庆节"), QT_TRANSLATE_NOOP("Calendar", "啤酒节"), QT_TRANSLATE_NOOP("Calendar", "南瓜节"), QT_TRANSLATE_NOOP("Calendar", "万圣节"), QT_TRANSLATE_NOOP("Calendar", "感恩节"),//11到15
	QT_TRANSLATE_NOOP("Calendar", "护士节"), QT_TRANSLATE_NOOP("Calendar", "圣诞节"), QT_TRANSLATE_NOOP("Calendar", "艾滋病日"), QT_TRANSLATE_NOOP("Calendar", "春节"), QT_TRANSLATE_NOOP("Calendar", "元宵节"),//16到20
	QT_TRANSLATE_NOOP("Calendar", "端午节"), QT_TRANSLATE_NOOP("Calendar", "七夕"), QT_TRANSLATE_NOOP("Calendar", "中秋节"), QT_TRANSLATE_NOOP("Calendar", "重阳节"), QT_TRANSLATE_NOOP("Calendar", "除夕"),	//21到25
	QT_TRANSLATE_NOOP("Calendar", "母亲节"), QT_TRANSLATE_NOOP("Calendar", "父亲节"), QT_TRANSLATE_NOOP("Calendar", "感恩节"), QT_TRANSLATE_NOOP("Calendar", "复活节")				//26到30
};

/************************************************************
 农历日历表
************************************************************/
static int LunarCalendarTable[200] ={
				0x04AE,0x0A57,0xAA4D,0x0D26,0x0D95,0x8D55,0x056A,0x09AD,0x495D,0x04AE,/*1901-1910*/
				0xD49B,0x0A4D,0x0D25,0xBAA5,0x0B54,0x0D6A,0x52DA,0x095B,0xE937,0x0497,/*1911-1920*/
				0x0A4B,0xB64B,0x06A5,0x06D4,0x95B5,0x02B6,0x0957,0x492F,0x0497,0xCC96,/*1921-1930*/
				0x0D4A,0x0EA5,0xADA9,0x05AD,0x02B6,0x726E,0x092E,0xF92D,0x0C95,0x0D4A,/*1931-1940*/
				0xDB4A,0x0B55,0x056A,0x955B,0x025D,0x092D,0x592B,0x0A95,0xF695,0x06CA,/*1941-1950*/
				0x0B55,0xAAB5,0x04DA,0x0A5B,0x6A57,0x052B,0x1152A,0x0E95,0x06AA,0xD5AA,/*1951-1960*/
				0x0AB5,0x04B6,0x94AE,0x0A57,0x0526,0x7D26,0x0D95,0xEB55,0x056A,0x096D,/*1961-1970*/
				0xA95D,0x04AD,0x0A4D,0x9A4D,0x0D25,0x11AA5,0x0B54,0x0B6A,0xD2DA,0x095B,/*1971-1980*/
				0x049B,0x9497,0x0A4B,0x1564B,0x06A5,0x06D4,0xD5B4,0x0AB6,0x0937,0xA92F,/*1981-1990*/
				0x0497,0x064B,0x6D4A,0x0EA5,0x10D65,0x05AC,0x0AB6,0xB26D,0x092E,0x0C96,/*1991-2000*/
				0x9A95,0x0D4A,0x0DA5,0x4B55,0x056A,0xF55B,0x025D,0x092D,0xB92B,0x0A95,/*2001-2010*/
				0x0B4A,0x96AA,0x0AD5,0x12AB5,0x04BA,0x0A5B,0xCA57,0x052B,0x0A93,0x8E95,/*2011-2020*/
				0x06AA,0x0AD5,0x49B5,0x04B6,0xD2AE,0x0A4E,0x0D26,0xBD26,0x0D53,0x05AA,/*2021-2030*/
				0x6D6A,0x096D,0x1695D,0x04AD,0x0A4D,0xDA4B,0x0D25,0x0D52,0xBB54,0x0B5A,/*2031-2040*/
				0x056D,0x495B,0x049B,0xF497,0x0A4B,0x0AA5,0xB6A5,0x06D2,0x0ADA,0x6AB6,/*2041-2050*/
				0x0937,0x1092F,0x0497,0x064B,0xCD4A,0x0EA5,0x06B2,0x956C,0x0AAE,0x092E,/*2051-2060*/
				0x792E,0x0C96,0xFA95,0x0D4A,0x0DA5,0xAB55,0x056A,0x0A6D,0x8A5D,0x052D,/*2061-2070*/
				0x1152B,0x0A95,0x0B4A,0xD6AA,0x0AD5,0x055A,0x94BA,0x0A5B,0x052B,0x7527,/*2071-2080*/
				0x0693,0xEE53,0x06AA,0x0AD5,0xA9B5,0x04B6,0x0A57,0x8A4E,0x0D26,0x11D26,/*2081-2090*/
				0x0D52,0x0DAA,0xCD6A,0x056D,0x04AE,0x949D,0x0A4D,0x0D15,0x5B25};       /*2091-2099*/

/************************************************************
 天干名
************************************************************/
static const char *tiangan[] =
{
	QT_TRANSLATE_NOOP("Calendar", "甲"), QT_TRANSLATE_NOOP("Calendar", "乙"), QT_TRANSLATE_NOOP("Calendar", "丙"), QT_TRANSLATE_NOOP("Calendar", "丁"),
	QT_TRANSLATE_NOOP("Calendar", "戊"), QT_TRANSLATE_NOOP("Calendar", "己"), QT_TRANSLATE_NOOP("Calendar", "庚"), QT_TRANSLATE_NOOP("Calendar", "辛"),
	QT_TRANSLATE_NOOP("Calendar", "壬"), QT_TRANSLATE_NOOP("Calendar", "癸"),
};

/************************************************************
 地址名
************************************************************/
static const char *dizhi[] =
{
	QT_TRANSLATE_NOOP("Calendar", "子"), QT_TRANSLATE_NOOP("Calendar", "丑"), QT_TRANSLATE_NOOP("Calendar", "寅"), QT_TRANSLATE_NOOP("Calendar", "卯"),
	QT_TRANSLATE_NOOP("Calendar", "辰"), QT_TRANSLATE_NOOP("Calendar", "巳"), QT_TRANSLATE_NOOP("Calendar", "午"), QT_TRANSLATE_NOOP("Calendar", "未"),
	QT_TRANSLATE_NOOP("Calendar", "申"), QT_TRANSLATE_NOOP("Calendar", "酉"), QT_TRANSLATE_NOOP("Calendar", "戌"), QT_TRANSLATE_NOOP("Calendar", "亥"),
};

/************************************************************
描述：日历部件构造函数
参数：parent - 该部件父窗口指针
返回：无
************************************************************/
Calendar::Calendar(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Calendar)
{
	ui->setupUi(this);

	ui->labelYearMonth->m_bCalendar = true;
	ui->labelDay->m_bCalendar = true;
	ui->labelWeek->m_bCalendar = true;
	ui->labelJieriJieqi->m_bCalendar = true;
	ui->labelNongli->m_bCalendar = true;

	RESCALESCREEN(ui->labelYearMonth)
	RESCALESCREEN(ui->labelDay)
	RESCALESCREEN(ui->labelWeek)
	RESCALESCREEN(ui->labelJieriJieqi)
	RESCALESCREEN(ui->labelNongli)

	//实现半透明效果，采用重画
	setStyleSheet("background: transparent;border:0px");//实现背景全透明
	setWindowFlags(Qt::FramelessWindowHint);//实现背景全透明
#ifdef STYLE_BLUE
	ui->labelDay->setStyleSheet(QString::fromUtf8("font:bold;"));
	ui->labelJieriJieqi->setStyleSheet(QString::fromUtf8("font:bold;"));
	ui->labelNongli->setStyleSheet(QString::fromUtf8("font:bold;"));
	ui->labelWeek->setStyleSheet(QString::fromUtf8("font:bold;"));
	ui->labelYearMonth->setStyleSheet(QString::fromUtf8("font:bold;"));
#endif
	m_yearSolar = 0;
	m_monthSolar = 0;
	m_daySolar = 0;

	m_pTimer = new QTimer(this);
	//Generate();
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(Generate()));
	m_pTimer->start(1000);
}

/************************************************************
描述：日历部件析构函数
参数：无
返回：无
************************************************************/
Calendar::~Calendar()
{
	delete m_pTimer;
	delete ui;
}

/************************************************************
描述：日历显示的更新。每秒钟读取一次系统日期，如果日期有改变则更新日历
	 的显示
参数：无
返回：无
************************************************************/
void Calendar::Generate(bool bForceUpdate)
{
	QDateTime datatime = QDateTime::currentDateTime();
	int year_solar, month_solar, day_solar, weekday;

	year_solar = datatime.date().year();
	month_solar = datatime.date().month();
	day_solar = datatime.date().day();
	weekday = datatime.date().dayOfWeek() % 7;

	if (!bForceUpdate)
	{
		if ((year_solar == m_yearSolar) && (month_solar == m_monthSolar) &&
			(day_solar == m_daySolar))
		{
			return;
		}
	}

	/*if (g_pMainDlg->m_pCurActiveDlg != g_pMainDlg)
	{
		if ((g_pMainDlg->m_pCurActiveDlg != g_pMainDlg->m_pSysSetMainDlg)
			|| ((g_pMainDlg->m_pSysSetMainDlg->m_curDlg != g_pMainDlg->m_pSysSetMainDlg->m_pLanguageSetDlg)
			&& (g_pMainDlg->m_pSysSetMainDlg->m_curDlg != g_pMainDlg->m_pSysSetMainDlg->m_pDatetimeSetDlg)))
		{
			return;
		}
	}
	*/

	m_yearSolar = year_solar;
	m_monthSolar = month_solar;
	m_daySolar = day_solar;

	/*if (pMainDlg->m_pVoipMainDlg->m_pIctDlg->m_pRecvHookDlg->isVisible()) return;
	if (pMainDlg->m_pVoipMainDlg->m_pIstDlg->isVisible()) return;
	if (pMainDlg->m_pVoipMainDlg->m_pMonDlg->isVisible()) return;
	//if (pMainDlg->m_pVoipMainDlg->m_pMctDlg->m_pHookDlg->isVisible()) return;
	if (pMainDlg->m_pMediaManDlg->m_pMediaPlayerDlg->isVisible()) return;
	if (pMainDlg->m_pSecurityMainDlg->m_pSecurityPlayerDlg->isVisible()) return;
	if (pMainDlg->m_pSecurityMainDlg->m_pSecurityAlertDlg->isVisible()) return;
	if (pMainDlg->m_pCallCenterDlg->isVisible()) return;*/

	QString str;

	{
		str = QString::number(year_solar, 10);
		str += " ";
        if (g_pMainDlg->m_inputMethodMode != 0)
            str += newChMonth[month_solar];
        else
            str += enMonth[month_solar];
		ui->labelYearMonth->setText(str);
	}

	{
		str.sprintf("%02d", day_solar);
		ui->labelDay->setText(str);
	}

	{
		str = tr(Week[weekday]);//星期
		ui->labelWeek->setText(str);
	}

	int year_lunar, month_lunar, day_lunar;

	{
		if (g_pMainDlg->m_inputMethodMode != 0)
		{
			QRect rc = ui->labelNongli->geometry();
			rc.moveTop(rc.top() + rc.height());
			ui->labelJieriJieqi->setGeometry(rc);
			ui->labelNongli->show();

			LunarCalendar(year_solar, month_solar, day_solar, &year_lunar, &month_lunar, &day_lunar);

			str = tr(tiangan[(year_lunar - 4)%10]);
			str += tr(dizhi[(year_lunar - 4)%12]);
			str += tr("年");
			if (month_lunar < 0)
			{
				str += tr("闰");
				//month_lunar = -month_lunar;
				str += tr(ChMonth[-month_lunar]);
			}
			else
			{
				str += tr(ChMonth[month_lunar]);
			}

			str += tr(ChDay[day_lunar]);

			ui->labelNongli->setText(str);
		}
		else
		{
			ui->labelNongli->hide();
			ui->labelJieriJieqi->setGeometry(ui->labelNongli->geometry());
		}
	}

	{
		int jieqi_idx;
		int yangli_idx, yinli_idx, week_idx, fuhuojie_idx;

		jieqi_idx = GetJieqi(year_solar, month_solar, day_solar);

		GetJieri(year_solar, month_solar, day_solar, year_lunar, month_lunar, day_lunar,
			weekday, &yangli_idx, &yinli_idx, &week_idx, &fuhuojie_idx);

		QString strjieqi;
		strjieqi = tr(jieqi[jieqi_idx]);

		QString strjieri;
		strjieri = tr(jieri[yangli_idx]);
		if (strjieri == "")
		{
			strjieri = tr(jieri[yinli_idx]);
		}
		if (strjieri == "")
		{
			strjieri = tr(jieri[week_idx]);
		}
		if (strjieri == "")
		{
			strjieri = tr(jieri[fuhuojie_idx]);
		}

		str = "";

		//strjieri = QString::fromUtf8("青年节");

		if ((g_pMainDlg->m_inputMethodMode != 0) && (strjieqi != ""))
		{
			str = strjieqi;
		}

		if (strjieri != "")
		{
			if (str != "") str += " ";
			str += strjieri;
		}

		ui->labelJieriJieqi->setText(str);
	}
}

/************************************************************
描述：根据阳历日期求农历日期
参数：year_solar - 阳历年
	 month_solar - 阳历月
	 day_solar - 阳历日
	 p_year_lunar - 农历年指针
	 p_month_lunar - 农历月指针
	 p_day_lunar - 农历日指针
返回：无
************************************************************/
void Calendar::LunarCalendar(int year_solar, int month_solar, int day_solar,
							int *p_year_lunar, int *p_month_lunar, int *p_day_lunar)
{
	int DateCount, loop;
	int i, j, n, Bit;
	int year_lunar, month_lunar, day_lunar;
	// //   int month,day;//阳历数据
	//  //  month = *pmonth;
	//  //  day = *pday;
	//计算从1901年2月19日(正月初一)到现在所经历的天数。
	DateCount = (year_solar - 1901) * 365 //本年到1901年之间的年数 * 每年365天
			  + (year_solar - 1901) / 4   //本年到1901年之间的润年数，一个一天
			  + month_add[month_solar - 1] //本月之前的天数
			  - 49                  //2月19日之前有31天
			  + day_solar;                //本日天数

	//如今年阳历是闰年（2月有29天），而且当前月份大于2月，经历的总天数加1。
	if((!(year_solar % 4)) && (month_solar > 2))
		DateCount = DateCount + 1;

	//下面是查表的算法。
	loop = 1;
	j = 0;

	//从1921年开始，把DateCount一个月一个月的递减，减到天数小于当月天数为止
	while (loop)
	{
		if(LunarCalendarTable[j] <= 4095) /*4095=0xFFF*/ /*低12/13位表示每个月的大小，闰月顺序排列，高4位表示闰月序数*/
			i = 11;//没闰月
		else
			i = 12;//有闰月
		n = i;
		while(n>=0)
		{
			Bit = LunarCalendarTable[j];
			Bit = (Bit >> n) & 1; //1是指大月30天 0是指小月29天
			if (DateCount <= (29 + Bit))
			{
				loop = 0;
				break;
			}
			DateCount -= 29 + Bit;
			n--;
		}
		if(!loop)
			break;
		j++;//下一年
	}

	year_lunar = 1901 + j; //年
	month_lunar = i - n + 1; //月
	day_lunar = DateCount;//农历天数
	if (i == 12)//有润月
	{
		if (month_lunar == LunarCalendarTable[j] / 8192 + 1) //8192=13个1
		{
			month_lunar = 1 - month_lunar;//正好是闰月，月份是负数
		}
		else if (month_lunar > LunarCalendarTable[j] / 8192 + 1)
			month_lunar--; //在闰月后面，月份要调整
	}

	*p_year_lunar = year_lunar;
	*p_month_lunar = month_lunar;
	*p_day_lunar = day_lunar;
}


/****************************************************************
 *函数性质：共用
 *入口：公历年unsigned char ucSolarCalendarYear,公历月unsigned char ucSolarCalendarMonth
 *出口：日期
 *功能：计算输入的公历年月中的第一个24节气日在哪一天
 *调用方式：unsigned char CalendarCalculateSolarTerm1(void)
*****************************************************************/
unsigned char Calendar::CalendarCalculateSolarTerm1(unsigned char ucSolarCalendarYear,
		unsigned char ucSolarCalendarMonth)
{
	unsigned char ucDoneIndex;
	unsigned char ucSolarTerm;
	ucDoneIndex = 0;
	while (ucSolarCalendarYear >= CalendarSolarTermYear01[ucSolarCalendarMonth - 1][ucDoneIndex])
		ucDoneIndex++;
	ucSolarTerm = CalendarSolarTermTable01[ucSolarCalendarMonth - 1][4 * ucDoneIndex + ucSolarCalendarYear%4];

	//校正不符合规律的数据
	if ((ucSolarCalendarYear == 121) && (ucSolarCalendarMonth == 4))
		ucSolarTerm = 5;
	if ((ucSolarCalendarYear == 122) && (ucSolarCalendarMonth == 4))
		ucSolarTerm = 5;
	if ((ucSolarCalendarYear == 132) && (ucSolarCalendarMonth == 4))
		ucSolarTerm = 5;
	if ((ucSolarCalendarYear == 194) && (ucSolarCalendarMonth == 6))
		ucSolarTerm = 6;
	if ((ucSolarCalendarYear == 114) && (ucSolarCalendarMonth == 3))
		ucSolarTerm = 6;
	if ((ucSolarCalendarYear == 121) && (ucSolarCalendarMonth == 4))
		ucSolarTerm = 4;
	if ((ucSolarCalendarYear == 123) && (ucSolarCalendarMonth == 4))
		ucSolarTerm = 4;
	if ((ucSolarCalendarYear == 151) && (ucSolarCalendarMonth == 9))
		ucSolarTerm = 7;
	if ((ucSolarCalendarYear == 183) && (ucSolarCalendarMonth == 2))
		ucSolarTerm = 3;
	if ((ucSolarCalendarYear == 194) && (ucSolarCalendarMonth == 6))
		ucSolarTerm = 5;

	return(ucSolarTerm);
}

/****************************************************************
 *函数性质：共用
 *入口：公历年unsigned char ucSolarCalendarYear,公历月unsigned char ucSolarCalendarMonth
 *出口：日期
 *功能：计算输入的公历年月中的第二个24节气日在哪一天
 *调用方式：unsigned char CalendarCalculateSolarTerm1(void)
*****************************************************************/
unsigned char Calendar::CalendarCalculateSolarTerm2(unsigned char ucSolarCalendarYear,
												   unsigned char ucSolarCalendarMonth)
{
	unsigned char ucDoneIndex;
	unsigned char ucSolarTerm;
	ucDoneIndex = 0;
	while (ucSolarCalendarYear >= CalendarSolarTermYear02[ucSolarCalendarMonth - 1][ucDoneIndex])
		ucDoneIndex++;
	ucSolarTerm = CalendarSolarTermTable02[ucSolarCalendarMonth - 1][4*ucDoneIndex + ucSolarCalendarYear%4];

	//校正不符合规律的数据
	if ((ucSolarCalendarYear == 171) && (ucSolarCalendarMonth == 3))
		ucSolarTerm = 21;
	if ((ucSolarCalendarYear == 181) && (ucSolarCalendarMonth == 5))
		ucSolarTerm = 21;
	if ((ucSolarCalendarYear == 151) && (ucSolarCalendarMonth == 3))
		ucSolarTerm = 20;
	if ((ucSolarCalendarYear == 171) && (ucSolarCalendarMonth == 3))
		ucSolarTerm = 20;
	if ((ucSolarCalendarYear == 184) && (ucSolarCalendarMonth == 3))
		ucSolarTerm = 19;
	return (ucSolarTerm);
}

/************************************************************
描述：将求节气的两个程序合成一个程序,并将数据类型转换成合适的类型
参数：year_solar - 阳历年
	 month_solar - 阳历月
	 day_solar - 阳历日
返回：节气的序号
************************************************************/
int Calendar::GetJieqi(int year_solar, int month_solar, int day_solar)
{
	int idx_jieqi;

	year_solar = year_solar - 1900;
	idx_jieqi = 0;
	if (day_solar < 15)//日期小于15则运行节气程序
	{
		if (day_solar == CalendarCalculateSolarTerm1(year_solar, month_solar))
			idx_jieqi = (month_solar - 1) * 2 + 1;
	}
	else			//否则运行中气程序
		if (day_solar == CalendarCalculateSolarTerm2(year_solar, month_solar))
			idx_jieqi = (month_solar - 1) * 2 + 2;

	return idx_jieqi;
}

/************************************************************
描述：求阳历日期固定的节日
参数：month_solar - 阳历月
	 day_solar - 阳历日
返回：阳历节日的序号
************************************************************/
int Calendar::yangli_jieri(int month_solar, int day_solar)
{
	int temp, idx;

	temp = month_solar * 100 + day_solar;// 把month_solar和day_solar连成一个数字 如2月4号：temp=204，12月8号temp=1208

	switch (temp)
	{
	case 101: idx = 1;break;//元旦
	case 214: idx = 2;break;//情人节
	case 308: idx = 3;break;//国际妇女节
	case 312: idx = 4;break;//植树节
	case 401: idx = 5;break;//愚人节
	case 508: idx = 6;break;//国际红十字节
	case 501: idx = 7;break;//国际劳动节
	case 504: idx = 8;break;//国际青年节
	case 601: idx = 9;break;//国际儿童节
	case 910: idx = 10;break;//教师节
	case 1001: idx = 11;break;//国庆节
	case 1010: idx = 12;break;//啤酒节（德国）
	case 1031: idx = 13;break;//南瓜节（北美）
	case 1101: idx = 14;break;//万圣节
	case 1124: idx = 15;break;//感恩节
	case 1212: idx = 16;break;//护士节
	case 1225: idx = 17;break;//圣诞节
	case 1201: idx = 18;break;//世界爱滋病日
	default: idx = 0;
	}
	return idx;
}//求阳历日期固定的节日


/************************************************************
描述：求农历日期固定的节日
参数：year_lunar - 农历年
	 month_lunar - 农历月
	 day_lunar - 农历日
返回：农历节日的序号
************************************************************/
int Calendar::yinli_jieri(int year_lunar, int month_lunar, int day_lunar)
{
	int temp, idx;

	temp = month_lunar * 100 + day_lunar;//把month和day连成一个数字

	//传统节日如遇农历闰月,法定假日将定于首月
	switch (temp)
	{
	case 101: idx = 19;break;//春节
	case 115: idx = 20;break;//元宵节
	case 505: idx = 21;break;//端午节
	case 707: idx = 22;break;//七夕
	case 815: idx = 23;break;//中秋节
	case 909: idx = 24;break;//重阳节
	default: idx = 0;
	}

	//判断是否为除夕
	//除夕是正月初一的前一天，然而12月可能是29天，也可能是30天
	//除夕和春节肯定在阳历1月或2月
	//	printf("%d	%d	%d	%d",year_lunar,month_lunar,day_lunar,temp);
	if (month_lunar == 12 && (day_lunar == 29 || day_lunar == 30))
	{														//1900年到2100年没有闰十二月
		if (((LunarCalendarTable[year_lunar - 1901] & 1) == 0) && (day_lunar == 29))
			idx = 25;
		if(((LunarCalendarTable[year_lunar - 1901] & 1) == 1) && (day_lunar == 30))
			idx = 25;
	}//判断是否为除夕

	return idx;
}//求阴历日期固定的节日

/************************************************************
描述：求阳历某月某个星期的星期几的节日
参数：year_solar - 阳历年
	 month_solar - 阳历月
	 day_solar - 阳历日
	 weekday - 星期几
返回：节日数组中数的位置
************************************************************/
//**调用判断星期几的函数:cal_weekday(int year,int month,int day)
int Calendar::week_jieri(int year_solar, int month_solar, int day_solar, int weekday)
{
	Q_UNUSED(year_solar);

	int weekday_idx;
	int idx, temp;

	//weekday = getweekday(year_solar, month_solar, day_solar);

	weekday_idx = day_solar/7;
	if ((day_solar%7) >= 1)
		weekday_idx ++;

	temp = month_solar * 100 + weekday_idx * 10 + weekday;//把几月份，第几个星期的星期几 ，这三个几连在一起
	switch (temp)
	{							//若是星期日，运行getweekday()，得到0
	case 520: idx = 26;break;//母亲节 5月的第二个星期日
	case 630: idx = 27;break;//父亲节 6月的第三个星期日
	case 1144: idx = 28;break;//感恩节 美国，11月第四个星期四
	default: idx = 0;
	}

	return idx;
}//求阳历某月某个星期的星期几的节日

/************************************************************
描述：计算复活节
参数：year_solar - 阳历年
	 month_solar - 阳历月
	 day_solar - 阳历日
返回：复活节节日序号
************************************************************/
/*算法来自百度百科
　复活节是西方传统的节日，公元325年尼西亚宗教会议规定，每年过春分月圆后的第一个星期天为复活节。其日期是不固定的，通常是要查看日历才能知道。
　　现在有一个简便的算法供大家参考！
　　年份只限于1900年到2099年
　　NO.1 设要求的那一年是Y年，从Y减去1900，其差记为N。
　　NO.2 用19作除数去除N，余数记为A。
　　NO.3 用4作除数去除N，不管余数，把商记为Q。
　　NO.4 用19去除7A+1，把商记为B，不管余数。
　　NO.5 用29去除11A+4-B，余数记为M。
　　NO.6 用7去除N+Q+31-M，余数记为W。
　　NO.7 计算25-M-W，结果计为day。
　　得出答数即可定出复活节的日期。若为正数，月份为4月，是该月的第（day）天;如不为正数，月份为3月，是该月的第（31+day）天
　　现在我们选两个年份来实例计算一遍。
　　例1. 1983年的复活节日期。
　　1. 1983-1900=83， N=83
　　2. N/19=4余7， A=7
　　3. N/4=20余3， Q=20
　　4. 7A+1=50，50/19=2余12， B=2
　　5. 11A+4-B=79，79/29=2余21， M=21
　　6. N+Q+31-M=113，113/7=16余1， W=1
　　7. 25-M-W=3
　　由此可知，1983年的复活节日期是4月3日。
　　例2. 2008年，中国将在北京举办奥运会，那一年复活节是哪一天呢？
　　1. 2008-1900=108， N=108
　　2. N/19=5余13， A=13
　　3. N/4=27， Q=27
　　4. 7A+1=92，92/19=4余16， B=4
　　5. 11A+4-B=143，143/29=4余27， M=27
　　6. N+Q+31-M=139，139/7=19余6， W=6
　　7. 25-M-W=-8， 31-8=23
　　所以该年复活节日期是3月23日。
　　按照复杂的天文历法分析与计算，最早的复活节日期是3月22日，上一次出现是在1818年，而且要到2285年才会再度出现。最晚的日期是在4月25日，上一次出现是在1943年，要到2038年才会出现。
*/
int Calendar::fuhuojie_jieri(int year_solar, int month_solar, int day_solar)
{
	int y,n,a,q,b,m,w;
	int day,month;
	int idx;

	idx = 0;
	y = year_solar;
	n = y - 1900;
	a = n%19;
	q = n/4;
	b = (7 * a + 1)/19;
	m = (11 * a + 4 - b)%29;
	w = (n + q + 31 - m)%7;
	day = 25 - m - w;

	if (day > 0)
	{
		month = 4;
	}
	else
	{
		month = 3;
		day = 31 + day;
	}

	if (month == month_solar && day == day_solar)
		idx = 29;

	return idx;
} //计算复活节

/************************************************************
描述：主函数。将阳历节日，阴历节日，某月第几个星期星期几的节日和复活节
	 的程序组合成一个主程序
参数：year_solar - 阳历年
	 month_solar - 阳历月
	 day_solar - 阳历日
	 year_lunar - 农历年
	 month_lunar - 农历月
	 day_lunar - 农历日
	 weekday - 星期几
	 p_yangli_idx - 阳历节日序号
	 p_yinli_idx - 农历节日序号
	 p_week_idx - 星期节日序号
	 p_fuhuojie_idx - 复活节节日序号
返回：无
************************************************************/
//
//
void Calendar::GetJieri(int year_solar, int month_solar, int day_solar, int year_lunar, int month_lunar, int day_lunar,
					   int weekday, int *p_yangli_idx, int *p_yinli_idx, int *p_week_idx, int *p_fuhuojie_idx)
{
	*p_yangli_idx = yangli_jieri(month_solar, day_solar);
	*p_yinli_idx = yinli_jieri(year_lunar, month_lunar, day_lunar);
	*p_week_idx = week_jieri(year_solar, month_solar, day_solar, weekday);
	if (month_solar == 3 || month_solar == 4)
		*p_fuhuojie_idx = fuhuojie_jieri(year_solar, month_solar, day_solar);
	else
		*p_fuhuojie_idx = 0;
}//将阳历节日，阴历节日，某月第几个星期星期几的节日和复活节的程序组合成一个主程序
