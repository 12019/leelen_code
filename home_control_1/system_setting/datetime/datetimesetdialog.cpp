#include "datetimesetdialog.h"
#include "ui_datetimesetdialog.h"
#include <QDateTime>
//#include <QMessageBox>
#include <time.h>
#include <sys/time.h>
#include <linux/rtc.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "clock.h"
#include "timemode1dialog.h"
#include "timemode2dialog.h"
#include "timemode3dialog.h"
#include "maindialog.h"
#include "helpdialog.h"
#include "callcenterdialog.h"
#include "syssetmaindialog.h"
#include <QSettings>
#include "ui_syssetmaindialog.h"
#include "ui_maindialog.h"
#include "mediastreamer2/mediastream.h"
#include "fmylineedit.h"
#include "core/leelen_comdef.h"
#include "leelen_protocol.h"
#include "leelen_system.h"
#include <signal.h>


#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
/************************************************************
  该文件为修改日期时间对话框主程序
************************************************************/
extern char g_strLphoneName[3][50];
extern MainDialog *g_pMainDlg;

extern RESULT_E g_check_managemachine_udp_state ;
extern RESULT_E g_check_computercenter_udp_state ;

void set_manager_time_result_callback(RESULT_E result)
{
    printf("=============[%s -- %d] result = %d\n", __func__, __LINE__, result);
    g_check_managemachine_udp_state = result;
}
void set_computer_time_result_callback(RESULT_E result)
{
    printf("=============[%s -- %d] result = %d\n", __func__, __LINE__, result);
    g_check_computercenter_udp_state = result;
}
static void sigusr2_handler(int signo, siginfo_t *info,void *ctx)
{
    g_pMainDlg->m_pSysSetMainDlg->m_pDatetimeSetDlg->ReceiveUdpTimeDate((char*)info->si_value.sival_ptr);
}

/************************************************************
描述：修改日期时间对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
DatetimeSetDialog::DatetimeSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatetimeSetDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	int res = g_pScaleSrc->get_screen_version();
    if ( res == 1 )
    {
        ui->labelDateTImeSetYear->setStyleSheet(QString::fromUtf8("font: 22px"));
        ui->labelDateTImeSetMonth->setStyleSheet(QString::fromUtf8("font: 22px"));
        ui->labelDateTImeSetDay->setStyleSheet(QString::fromUtf8("font: 22px"));
        ui->labelDateTImeSetHour->setStyleSheet(QString::fromUtf8("font: 22px"));
        ui->labelDateTImeSetMinute->setStyleSheet(QString::fromUtf8("font: 22px"));
        ui->labelDateTImeSetSecond->setStyleSheet(QString::fromUtf8("font: 22px"));
    }
    else if ( res == 2 )  // 1024x600
	{
		resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23
        ui->frameDateTimeSetting->setGeometry(QRect(32, 10, 940, 140));

        int w=0, h=0, w1=0, w2=0;

        //年月日时分秒(lineEdit)
        w1 = 210, w2 = 100, h = 65;
        ui->lineEditYear->setGeometry(QRect(38, 15, w1, h));
        ui->lineEditMonth->setGeometry(QRect(38+w1+w2, 15, w1, h));
        ui->lineEditDay->setGeometry(QRect(38+2*w1+2*w2, 15, w1, h));

        ui->lineEditHour->setGeometry(QRect(38, 80, w1, h));
        ui->lineEditMinute->setGeometry(QRect(38+w1+w2, 80, w1, h));
        ui->lineEditSecond->setGeometry(QRect(38+2*w1+2*w2, 80, w1, h));

        //年月日时分秒(label)
        ui->labelDateTImeSetYear->setGeometry(QRect(38+w1, 15, w2, h));
        ui->labelDateTImeSetMonth->setGeometry(QRect(38+2*w1+w2, 15, w2, h));
        ui->labelDateTImeSetDay->setGeometry(QRect(38+3*w1+2*w2, 15, w2, h));

        ui->labelDateTImeSetHour->setGeometry(QRect(38+w1, 80, w2, h));
        ui->labelDateTImeSetMinute->setGeometry(QRect(38+2*w1+w2, 80, w2, h));
        ui->labelDateTImeSetSecond->setGeometry(QRect(38+3*w1+2*w2, 80, w2, h));

        ui->labelDateTImeSetYear->setStyleSheet(QString::fromUtf8("font: 27px"));
        ui->labelDateTImeSetMonth->setStyleSheet(QString::fromUtf8("font: 27px"));
        ui->labelDateTImeSetDay->setStyleSheet(QString::fromUtf8("font: 27px"));
        ui->labelDateTImeSetHour->setStyleSheet(QString::fromUtf8("font: 27px"));
        ui->labelDateTImeSetMinute->setStyleSheet(QString::fromUtf8("font: 27px"));
        ui->labelDateTImeSetSecond->setStyleSheet(QString::fromUtf8("font: 27px"));

        //第一列
        w = 220, h = 100;
        ui->pushButtonDateTimeSetNum1->setGeometry(QRect(30, 160, w, h));
        ui->pushButtonDateTimeSetNum4->setGeometry(QRect(30, 270, w, h));
        ui->pushButtonDateTimeSetNum7->setGeometry(QRect(30, 380, w, h));
        ui->pushButtonDateTimeSetDot->setGeometry(QRect(30, 490, w, h));

        //第二列
        w = 220, h = 100;
        ui->pushButtonDateTimeSetNum2->setGeometry(QRect(270, 160, w, h));
        ui->pushButtonDateTimeSetNum5->setGeometry(QRect(270, 270, w, h));
        ui->pushButtonDateTimeSetNum8->setGeometry(QRect(270, 380, w, h));
        ui->pushButtonDateTimeSetNum0->setGeometry(QRect(270, 490, w, h));

        //第三列
        w = 220, h = 100;
        ui->pushButtonDateTimeSetNum3->setGeometry(QRect(510, 160, w, h));
        ui->pushButtonDateTimeSetNum6->setGeometry(QRect(510, 270, w, h));
        ui->pushButtonDateTimeSetNum9->setGeometry(QRect(510, 380, w, h));
        ui->pushButtonDateTimeSetShop->setGeometry(QRect(510, 490, w, h));

        //第四列
        w = 200, h = 100;
        ui->pushButtonDateTimeSetBack->hide();
        ui->pushButtonDateTimeSetClear->setGeometry(QRect(770, 160, w, h));
        ui->pushButtonDateTimeSetTime->setGeometry(QRect(770, 270, w, h));
        ui->pushButtonDateTimeSetSave->setGeometry(QRect(770, 380, w, h));
        ui->pushButtonDateTimeSetReturn->setGeometry(QRect(770, 490, w, h));
	}
	else if ( res == 3 ) // 1280x800
	{
		resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23
        ui->frameDateTimeSetting->setGeometry(QRect(58, 45, 1163, 172));

        int w=0, h=0, w1=0, w2=0;

        //年月日时分秒(lineEdit)
        w1 = 265, w2 = 125, h = 86;
        ui->lineEditYear->setGeometry(QRect(58, 45, w1, h));
        ui->lineEditMonth->setGeometry(QRect(58+w1+w2, 45, w1, h));
        ui->lineEditDay->setGeometry(QRect(58+2*w1+2*w2, 45, w1, h));

        ui->lineEditHour->setGeometry(QRect(58, 131, w1, h));
        ui->lineEditMinute->setGeometry(QRect(58+w1+w2, 131, w1, h));
        ui->lineEditSecond->setGeometry(QRect(58+2*w1+2*w2, 131, w1, h));

        //年月日时分秒(label)
        ui->labelDateTImeSetYear->setGeometry(QRect(58+w1, 45, w2, h));
        ui->labelDateTImeSetMonth->setGeometry(QRect(58+2*w1+w2, 45, w2, h));
        ui->labelDateTImeSetDay->setGeometry(QRect(58+3*w1+2*w2, 45, w2, h));

        ui->labelDateTImeSetHour->setGeometry(QRect(58+w1, 131, w2, h));
        ui->labelDateTImeSetMinute->setGeometry(QRect(58+2*w1+w2, 131, w2, h));
        ui->labelDateTImeSetSecond->setGeometry(QRect(58+3*w1+2*w2, 131, w2, h));

        ui->labelDateTImeSetYear->setStyleSheet(QString::fromUtf8("font: 35px"));
        ui->labelDateTImeSetMonth->setStyleSheet(QString::fromUtf8("font: 35px"));
        ui->labelDateTImeSetDay->setStyleSheet(QString::fromUtf8("font: 35px"));
        ui->labelDateTImeSetHour->setStyleSheet(QString::fromUtf8("font: 35px"));
        ui->labelDateTImeSetMinute->setStyleSheet(QString::fromUtf8("font: 35px"));
        ui->labelDateTImeSetSecond->setStyleSheet(QString::fromUtf8("font: 35px"));

        //第一列
        w = 272, h = 123;
        ui->pushButtonDateTimeSetNum1->setGeometry(QRect(58, 237, w, h));
        ui->pushButtonDateTimeSetNum4->setGeometry(QRect(58, 373, w, h));
        ui->pushButtonDateTimeSetNum7->setGeometry(QRect(58, 507, w, h));
        ui->pushButtonDateTimeSetDot->setGeometry(QRect(58, 641, w, h));

        //第二列
        w = 272, h = 123;
        ui->pushButtonDateTimeSetNum2->setGeometry(QRect(353, 237, w, h));
        ui->pushButtonDateTimeSetNum5->setGeometry(QRect(353, 373, w, h));
        ui->pushButtonDateTimeSetNum8->setGeometry(QRect(353, 507, w, h));
        ui->pushButtonDateTimeSetNum0->setGeometry(QRect(353, 641, w, h));

        //第三列
        w = 272, h = 123;
        ui->pushButtonDateTimeSetNum3->setGeometry(QRect(649, 237, w, h));
        ui->pushButtonDateTimeSetNum6->setGeometry(QRect(649, 373, w, h));
        ui->pushButtonDateTimeSetNum9->setGeometry(QRect(649, 507, w, h));
        ui->pushButtonDateTimeSetShop->setGeometry(QRect(649, 641, w, h));

        //第四列
        w = 246, h = 112;
        ui->pushButtonDateTimeSetBack->hide();
        ui->pushButtonDateTimeSetClear->setGeometry(QRect(976, 231, w, h));
        ui->pushButtonDateTimeSetTime->setGeometry(QRect(976, 372, w, h));
        ui->pushButtonDateTimeSetSave->setGeometry(QRect(976, 513, w, h));
        ui->pushButtonDateTimeSetReturn->setGeometry(QRect(976, 641, w, h));
	}

    setAttribute(Qt::WA_TranslucentBackground, false);

    ui->pushButtonDateTimeSetNum0->m_offsetX = 0;
    ui->pushButtonDateTimeSetNum1->m_offsetX = 0;
    ui->pushButtonDateTimeSetNum2->m_offsetX = 0;
    ui->pushButtonDateTimeSetNum3->m_offsetX = 0;
    ui->pushButtonDateTimeSetNum4->m_offsetX = 0;
    ui->pushButtonDateTimeSetNum5->m_offsetX = 0;
    ui->pushButtonDateTimeSetNum6->m_offsetX = 0;
    ui->pushButtonDateTimeSetNum7->m_offsetX = 0;
    ui->pushButtonDateTimeSetNum8->m_offsetX = 0;
    ui->pushButtonDateTimeSetNum9->m_offsetX = 0;
    ui->pushButtonDateTimeSetDot->m_offsetX  = 0;
    ui->pushButtonDateTimeSetShop->m_offsetX = 0;

	m_lineEditSel = ui->lineEditYear;

	m_pAutoUpdateTimer = new QTimer(this);

	connect(m_pAutoUpdateTimer,SIGNAL(timeout()),this,SLOT(slot_update_timeout()));

	srand(int(time(0)));

	m_pAutoUpdateTimer->start((1 + rand()%6000) * 50);

	m_nGetTimeCnt = 0;

    m_pDateTimeSetThread = new DateTimeSetThread(this);

	//david 2014/9/17
	connect(ui->lineEditYear,SIGNAL(lostFocus()),this,SLOT(on_lineEditYear_lostFocus()));
	connect(ui->lineEditMonth,SIGNAL(lostFocus()),this,SLOT(on_lineEditMonth_lostFocus()));
	connect(ui->lineEditDay,SIGNAL(lostFocus()),this,SLOT(on_lineEditDay_lostFocus()));
	connect(ui->lineEditHour,SIGNAL(lostFocus()),this,SLOT(on_lineEditHour_lostFocus()));
	connect(ui->lineEditMinute,SIGNAL(lostFocus()),this,SLOT(on_lineEditMinute_lostFocus()));
	connect(ui->lineEditSecond,SIGNAL(lostFocus()),this,SLOT(on_lineEditSecond_lostFocus()));
    connect(this,SIGNAL(sigTcpSetTime(int)),this,SLOT(slotTcpSetTime(int)));

    struct sigaction act;
    act.sa_sigaction = sigusr2_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;  // 信息传递开关

    if (sigaction(SIGUSR2,&act,NULL) == -1)
    {
        printf("sigaction SIGUSR2 error!\n");
    }

}

/************************************************************
描述：修改日期时间对话框析构函数
参数：无
返回：无
************************************************************/
DatetimeSetDialog::~DatetimeSetDialog()
{
    	delete ui;
}

/************************************************************
描述：读取系统日期时间，将读到的结果更新在行编辑控件上
参数：无
返回：无
************************************************************/
void DatetimeSetDialog::loadDateTime()
{
	//获取时间
	QDateTime datetime = QDateTime::currentDateTime();

	int y = datetime.date().year();
	int m = datetime.date().month();
	int d = datetime.date().day();

	ui->lineEditYear->setText(QString::number(y));
	ui->lineEditMonth->setText(QString::number(m));
	ui->lineEditDay->setText(QString::number(d));

	int h = datetime.time().hour();
	int mi = datetime.time().minute();
	int s = datetime.time().second();

	ui->lineEditHour->setText(QString::number(h));
	ui->lineEditMinute->setText(QString::number(mi));
	ui->lineEditSecond->setText(QString::number(s));
}

/************************************************************
描述：年行编辑控件失去焦点时，m_lineEditSel设为年行编辑控件
参数：无
返回：无
************************************************************/
void DatetimeSetDialog::on_lineEditYear_lostFocus()
{
	m_lineEditSel = ui->lineEditYear;
}

/************************************************************
描述：月行编辑控件失去焦点时，m_lineEditSel设为月行编辑控件
参数：无
返回：无
************************************************************/
void DatetimeSetDialog::on_lineEditMonth_lostFocus()
{
	m_lineEditSel = ui->lineEditMonth;
}

/************************************************************
描述：日行编辑控件失去焦点时，m_lineEditSel设为日行编辑控件
参数：无
返回：无
************************************************************/
void DatetimeSetDialog::on_lineEditDay_lostFocus()
{
	m_lineEditSel = ui->lineEditDay;
}

/************************************************************
描述：小时行编辑控件失去焦点时，m_lineEditSel设为小时行编辑控件
参数：无
返回：无
************************************************************/
void DatetimeSetDialog::on_lineEditHour_lostFocus()
{
	m_lineEditSel = ui->lineEditHour;
}

/************************************************************
描述：分钟行编辑控件失去焦点时，m_lineEditSel设为分钟行编辑控件
参数：无
返回：无
************************************************************/
void DatetimeSetDialog::on_lineEditMinute_lostFocus()
{
	m_lineEditSel = ui->lineEditMinute;
}

/************************************************************
描述：秒钟行编辑控件失去焦点时，m_lineEditSel设为秒钟行编辑控件
参数：无
返回：无
************************************************************/
void DatetimeSetDialog::on_lineEditSecond_lostFocus()
{
	m_lineEditSel = ui->lineEditSecond;
}

/************************************************************
描述：在m_lineEditSel指向的行编辑控件的文本后面加入新的数字
参数：strnum - 待加入的新的数字
返回：无
************************************************************/
void DatetimeSetDialog::appendNum(QString strnum)
{
	int days[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};//平年的日期

	if (m_lineEditSel)
	{
		int num = m_lineEditSel->text().toInt() * 10  + strnum.toInt();
		QString str = QString::number(num);
		bool is_cancel = false;

		if (m_lineEditSel == ui->lineEditYear)
		{
			if (str.toInt() > 3000 ) is_cancel = true;
		}
		else if (m_lineEditSel == ui->lineEditMonth)
		{
			if (str.toInt() > 12) is_cancel = true;

			if (!is_cancel)
			{
				int month_day_num;
				int year, month;

				year = ui->lineEditYear->text().toInt();
				month = str.toInt();

				month_day_num = days[month];
				//闰年
				if (((year%4) == 0 && ((year%100 ) != 0 || (year%400) == 0)) && (month == 2))
				{
					month_day_num++;
				}
				if (ui->lineEditDay->text().toInt() > month_day_num)
				{
					ui->lineEditDay->setText(QString::number(month_day_num));
				}
			}
		}
		else if (m_lineEditSel == ui->lineEditDay)
		{
			int month_day_num;
			int year, month;

			year = ui->lineEditYear->text().toInt();
			month = ui->lineEditMonth->text().toInt();

			month_day_num = days[month];
			//闰年
			if (((year%4) == 0 && ((year%100 ) != 0 || (year%400) == 0)) && (month == 2))
			{
				month_day_num++;
			}
			if (str.toInt() > month_day_num) is_cancel = true;
		}
		else if (m_lineEditSel == ui->lineEditHour)
		{
			if (str.toInt() > 23) is_cancel = true;
		}
		else if (m_lineEditSel == ui->lineEditMinute)
		{
			if (str.toInt() > 59) is_cancel = true;
		}
		else if (m_lineEditSel == ui->lineEditSecond)
		{
			if (str.toInt() > 59) is_cancel = true;
		}

		if (!is_cancel)
		{
			m_lineEditSel->setText(str);
		}
	}
}

/************************************************************
描述：返回调用者
参数：无
返回：无
************************************************************/
void DatetimeSetDialog::on_pushButtonDateTimeSetReturn_pressed()
{
    hide();
    SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();
    pSysSetMainDlg->on_pushButtonSysSetMainVersion_pressed();
}

/************************************************************
  描述：按了对话框的"0"按钮，在m_lineEditSel的文本后面添加"0"
  参数：无
  返回：无
 ************************************************************/
void DatetimeSetDialog::on_pushButtonDateTimeSetNum0_pressed()
{
    ui->pushButtonDateTimeSetNum0->setFocus(Qt::TabFocusReason);

	appendNum("0");
}

/************************************************************
  描述：按了对话框的"1"按钮，在m_lineEditSel的文本后面添加"1"
  参数：无
  返回：无
 ************************************************************/
void DatetimeSetDialog::on_pushButtonDateTimeSetNum1_pressed()
{
    ui->pushButtonDateTimeSetNum1->setFocus(Qt::TabFocusReason);

	appendNum("1");
}

/************************************************************
  描述：按了对话框的"2"按钮，在m_lineEditSel的文本后面添加"2"
  参数：无
  返回：无
 ************************************************************/
void DatetimeSetDialog::on_pushButtonDateTimeSetNum2_pressed()
{
    ui->pushButtonDateTimeSetNum2->setFocus(Qt::TabFocusReason);

	appendNum("2");
}

/************************************************************
  描述：按了对话框的"3"按钮，在m_lineEditSel的文本后面添加"3"
  参数：无
  返回：无
 ************************************************************/
void DatetimeSetDialog::on_pushButtonDateTimeSetNum3_pressed()
{
    ui->pushButtonDateTimeSetNum3->setFocus(Qt::TabFocusReason);

	appendNum("3");
}

/************************************************************
  描述：按了对话框的"4"按钮，在m_lineEditSel的文本后面添加"4"
  参数：无
  返回：无
 ************************************************************/
void DatetimeSetDialog::on_pushButtonDateTimeSetNum4_pressed()
{
    ui->pushButtonDateTimeSetNum4->setFocus(Qt::TabFocusReason);

	appendNum("4");
}

/************************************************************
  描述：按了对话框的"5"按钮，在m_lineEditSel的文本后面添加"5"
  参数：无
  返回：无
 ************************************************************/
void DatetimeSetDialog::on_pushButtonDateTimeSetNum5_pressed()
{
    ui->pushButtonDateTimeSetNum5->setFocus(Qt::TabFocusReason);

	appendNum("5");
}

/************************************************************
  描述：按了对话框的"6"按钮，在m_lineEditSel的文本后面添加"6"
  参数：无
  返回：无
 ************************************************************/
void DatetimeSetDialog::on_pushButtonDateTimeSetNum6_pressed()
{
    ui->pushButtonDateTimeSetNum6->setFocus(Qt::TabFocusReason);

	appendNum("6");
}

/************************************************************
描述：按了对话框的"7"按钮，在m_lineEditSel的文本后面添加"7"
参数：无
返回：无
************************************************************/
void DatetimeSetDialog::on_pushButtonDateTimeSetNum7_pressed()
{
    ui->pushButtonDateTimeSetNum7->setFocus(Qt::TabFocusReason);

	appendNum("7");
}

/************************************************************
描述：按了对话框的"8"按钮，在m_lineEditSel的文本后面添加"8"
参数：无
返回：无
************************************************************/
void DatetimeSetDialog::on_pushButtonDateTimeSetNum8_pressed()
{
    ui->pushButtonDateTimeSetNum8->setFocus(Qt::TabFocusReason);

	appendNum("8");
}

/************************************************************
  描述：按了对话框的"9"按钮，在m_lineEditSel的文本后面添加"9"
  参数：无
  返回：无
 ************************************************************/
void DatetimeSetDialog::on_pushButtonDateTimeSetNum9_pressed()
{
    ui->pushButtonDateTimeSetNum9->setFocus(Qt::TabFocusReason);

	appendNum("9");
}

/************************************************************
描述：按了对话框的"退格"按钮，删除m_lineEditSel的文本最后面的数字
参数：无
返回：无
************************************************************/
void DatetimeSetDialog::on_pushButtonDateTimeSetBack_pressed()
{
    ui->pushButtonDateTimeSetBack->setFocus(Qt::TabFocusReason);//david 2014.6.12

	if (m_lineEditSel)
	{
		QString str = m_lineEditSel->text();
		str = str.left(str.length() - 1);

		if (str == "")
		{
			m_lineEditSel->setText(QString::number(0));
		}
		else
		{
			m_lineEditSel->setText(str);
		}
	}
}

/************************************************************
  描述：将m_lineEditSel的文本数字清空为"0"
  参数：无
  返回：无
 ************************************************************/
void DatetimeSetDialog::on_pushButtonDateTimeSetClear_pressed()
{
    ui->pushButtonDateTimeSetClear->setFocus(Qt::TabFocusReason);//david 2014.6.12

	if (m_lineEditSel)
	{
		m_lineEditSel->setText("0");
	}
}

/************************************************************
  描述：从行编辑控件读取修改后的日期时间，保存进config文件，并更新到系统
  时间和RTC时间，还删除了正在计时的延迟报警
  参数：无
  返回：无
 ************************************************************/
void DatetimeSetDialog::on_pushButtonDateTimeSetSave_pressed()
{
//    if(!isVisible()) //由于添加开机五分钟内自动同步时间所以将这两行代码屏蔽掉
//        return ;
	int year = ui->lineEditYear->text().toInt();

    if(!m_pDateTimeSetThread->m_autoUpdateTimeFlag)
    {
        if ( (year < 1900) || (year > 2037))
        {
            MyMessageBox messBox(this);
            CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
            okButton->setFocus(Qt::TabFocusReason);
            messBox.setText(tr("年份输入超出范围"));
            messBox.show();
            QRect rc = messBox.geometry();
            QRect rc2 = geometry();
            rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
            messBox.setGeometry(rc);
            messBox.exec();
            return ;
        }
    }

	QDateTime datetime = QDateTime::currentDateTime();
	QDateTime datetime_utc(QDate(datetime.toUTC().date().year(), datetime.toUTC().date().month(), datetime.toUTC().date().day()),
			QTime(datetime.toUTC().time().hour(), datetime.toUTC().time().minute(), datetime.toUTC().time().second()));
	int time_zone_sec_off = datetime_utc.secsTo(datetime);

    if(!m_pDateTimeSetThread->m_autoUpdateTimeFlag)
    {
        setAllButtonsEnabled(this, false);
        setAllButtonsEnabled((QDialog *)parent(), false);
        ui->frameDateTimeSetting->setFocus(Qt::TabFocusReason);
    }

	int i;
	SecurityMainDialog *pSecMainDlg = ((MainDialog *)(parent()->parent()))->m_pSecurityMainDlg;
	{
		for (i = 0; i < pSecMainDlg->m_wireDefenseCnt; i++)
		{
			if (pSecMainDlg->m_delayAlarmClass[i]->m_pDelayTimer)
			{
				pSecMainDlg->m_delayAlarmClass[i]->m_pDelayTimer->stop();
				pSecMainDlg->m_delayAlarmClass[i]->m_alertDateTimeList.clear();
				pSecMainDlg->m_delayAlarmClass[i]->m_isTimeOut = 0;
			}
		}

		for (i = 0; i < pSecMainDlg->m_wirelessDefenseCnt; i++)
		{
			if (pSecMainDlg->m_delayAlarmClass[56 + i]->m_pDelayTimer)
			{
				pSecMainDlg->m_delayAlarmClass[56 + i]->m_pDelayTimer->stop();
				pSecMainDlg->m_delayAlarmClass[56 + i]->m_alertDateTimeList.clear();
				pSecMainDlg->m_delayAlarmClass[56 + i]->m_isTimeOut = 0;
			}
		}
	}

	if (ui->lineEditMonth->text() == "0")
	{
		ui->lineEditMonth->setText("1");
	}

	if (ui->lineEditDay->text() == "0")
	{
		ui->lineEditDay->setText("1");
	}

	int y, m, d;
	int h, mi, s;

	y = ui->lineEditYear->text().toInt();
	m = ui->lineEditMonth->text().toInt();
	d = ui->lineEditDay->text().toInt();

	h = ui->lineEditHour->text().toInt();
	mi = ui->lineEditMinute->text().toInt();
	s = ui->lineEditSecond->text().toInt();

	stopWaveFile();

	struct tm tm_time;
	struct timeval tv_time;

	memset(&tm_time, 0, sizeof(struct tm));

	tm_time.tm_year = y - 1900;
	tm_time.tm_mon = m - 1;
	tm_time.tm_mday =d;

	tm_time.tm_hour = h;
	tm_time.tm_min = mi;
	tm_time.tm_sec = s;

	int rtc;
	if ((rtc = ::open("/dev/rtc0", O_WRONLY)) < 0)
	{
		printf("RTC open error\n");
	}

	tv_time.tv_sec = (time_t)(timegm(&tm_time) - time_zone_sec_off);
	tv_time.tv_usec = 0;

	settimeofday(&tv_time, NULL);

	if (rtc >= 0)
	{
		if (ioctl(rtc, RTC_SET_TIME, &tm_time) < 0)
		{
			printf("RTC write error\n");
		}

		::close(rtc);
	}

	g_pMainDlg->ui->widgetCalendar->Generate(true);

	{
		QDateTime dateTime = QDateTime::currentDateTime().addSecs(-(255 + 1));

		for (i = 0; i < pSecMainDlg->m_wireDefenseCnt; i++)
		{
			pSecMainDlg->m_delayAlarmClass[i]->m_repeatAlarmLastDateTime = dateTime;
		}

		for (i = 0; i < pSecMainDlg->m_wirelessDefenseCnt; i++)
		{
			pSecMainDlg->m_delayAlarmClass[56 + i]->m_repeatAlarmLastDateTime = dateTime;
		}
	}

    if(!m_pDateTimeSetThread->m_autoUpdateTimeFlag)
    {
        MyMessageBox messBox(this);
        CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
        //Q_UNUSED(okButton);
        okButton->setFocus(Qt::TabFocusReason);
        messBox.setText(tr("时间保存成功"));
        messBox.show();
        QRect rc = messBox.geometry();
        QRect rc2 = geometry();
        rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
        messBox.setGeometry(rc);
        messBox.exec();

        setAllButtonsEnabled((QDialog *)parent(), true);
        setAllButtonsEnabled(this, true);
        ui->pushButtonDateTimeSetSave->setFocus(Qt::TabFocusReason);
    }

    if(m_pDateTimeSetThread->m_autoUpdateTimeFlag)
    {
        m_pDateTimeSetThread->m_autoUpdateTimeFlag = false;
    }
}

/************************************************************
  描述：窗口显示时，重新读入系统时间并更新到行编辑控件上
  参数：无
  返回：无
 ************************************************************/
void DatetimeSetDialog::showEvent(QShowEvent *)
{
    g_pMainDlg->showRefreshTabFocus(this);

    m_lineEditSel = ui->lineEditYear;//david 2014.6.11

	loadDateTime();
}

/************************************************************
  描述：窗口隐藏回调函数
  参数：无
  返回：无
 ************************************************************/
void DatetimeSetDialog::hideEvent(QHideEvent *)
{
	//SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();
	//pSysSetMainDlg->ui->pushButtonDatetimeSet->setFocus(Qt::TabFocusReason);

    g_pMainDlg->hideRefreshTabFocus(this);
}

void DatetimeSetDialog::on_pushButtonDateTimeSetTime_pressed()
{
    if(m_pDateTimeSetThread->m_bRun)
    {
        m_pDateTimeSetThread->m_bRun = false;
        m_pDateTimeSetThread->quit();
        m_pDateTimeSetThread->wait();
    }
    m_pDateTimeSetThread->m_autoUpdateTimeFlag = false;
    m_pDateTimeSetThread->start();

}
/************************************************************
描述：协议探测，物业中心和管理机是否支持udp同步时间
参数：无
返回：无
************************************************************/
void DatetimeSetDialog::CheckUdpState()
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
        int ret1 = start_system_0x0501(&addr_manager, set_manager_time_result_callback);
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
        int ret2 = start_system_0x0501(&addr_computer, set_computer_time_result_callback);
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
描述： 接收udp库收到的时间数据并设置系统时间
参数： buf - 存储时间数据指针
返回：无
************************************************************/
void DatetimeSetDialog::ReceiveUdpTimeDate(char *buf)
{
    ui->lineEditYear->setText(QString::number(buf[0]) + QString::number(buf[1]));
    ui->lineEditMonth->setText(QString::number(buf[2]));
    ui->lineEditDay->setText(QString::number(buf[3]));
    ui->lineEditHour->setText(QString::number(buf[4]));
    ui->lineEditMinute->setText(QString::number(buf[5]));
    ui->lineEditSecond->setText(QString::number(buf[6]));

    on_pushButtonDateTimeSetSave_pressed();

}
/************************************************************
描述： 请求用udp同步时间
参数： dest:为1同步物业软件时间 为0同步管理机时间
返回： 0成功 -1失败
************************************************************/
int DatetimeSetDialog::SetTimeByUdp(int destination)
{
    addr_info_t    addr;
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
        return -1;
    }
    char *ptr;
    ptr = strstr(strIP, "?");
    if (ptr)
    {
       *ptr = '\0';
    }

    strcpy(rem_ip,strIP);
    strcpy(rem_usr,g_strLphoneName[destination + 1]);

    addr.loc_ip = (uint8_t *)loc_ip;
    addr.loc_usr = (uint8_t *)loc_usr;
    addr.rem_ip = (uint8_t *)rem_ip;
    addr.rem_usr = (uint8_t *)rem_usr;

    int res = start_system_0x0502(&addr, NULL);
    printf("start_system_0x0502 res = %d\n",res);
    return 0;
}
/************************************************************
描述：用tcp同步时间
参数：dest:为1同步物业软件时间 为0同步管理机时间
返回：0成功  -1失败
************************************************************/
int DatetimeSetDialog::SetTimeByTcp(int destination)
{
    char strPeerIP[200] = "192.168.1.107?4";
    int ret = 0;

    GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;

    ret = pGetPeerIPThread->GetPeerIP(g_strLphoneName[destination+1], strPeerIP);

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

        if (!(iModalType & MANCENTER_MODAL_TYPE) && !m_pDateTimeSetThread->m_autoUpdateTimeFlag)
        {
            ui->frameDateTimeSetting->setFocus(Qt::TabFocusReason);
            MyMessageBox messBox(this);
            CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
            //Q_UNUSED(okButton);
            okButton->setFocus(Qt::TabFocusReason);
            if(destination == 0)
            {
                messBox.setText(tr("对方不是管理机"));
            }
            else
            {
                messBox.setText(tr("对方不是中心机"));
            }
            messBox.show();
            QRect rc = messBox.geometry();
            QRect rc2 = geometry();
            rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
            messBox.setGeometry(rc);
            messBox.exec();
            ui->pushButtonDateTimeSetTime->setFocus(Qt::TabFocusReason);
            return -1;

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

    if (ret != 0 && !m_pDateTimeSetThread->m_autoUpdateTimeFlag)
    {
        ui->frameDateTimeSetting->setFocus(Qt::TabFocusReason);
        MyMessageBox messBox(this);
        CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
        //Q_UNUSED(okButton);
        okButton->setFocus(Qt::TabFocusReason);
        if(destination == 0)
        {
            messBox.setText(tr("获取管理机ip地址失败"));
        }
        else
        {
            messBox.setText(tr("获取中心机ip地址失败"));
        }
        messBox.show();
        QRect rc = messBox.geometry();
        QRect rc2 = geometry();
        rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
        messBox.setGeometry(rc);
        messBox.exec();
        ui->pushButtonDateTimeSetTime->setFocus(Qt::TabFocusReason);
        return -1;

    }

    QTcpSocket socket;

    socket.connectToHost(ptr, 1300);

    if (!socket.waitForConnected(750) && !m_pDateTimeSetThread->m_autoUpdateTimeFlag)
    {
        ui->frameDateTimeSetting->setFocus(Qt::TabFocusReason);
        MyMessageBox messBox(this);
        CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
        //Q_UNUSED(okButton);
        okButton->setFocus(Qt::TabFocusReason);
        if(destination == 0)
        {
            messBox.setText(tr("连接管理机失败"));
        }
        else
        {
            messBox.setText(tr("连接中心机失败"));
        }
        messBox.show();
        QRect rc = messBox.geometry();
        QRect rc2 = geometry();
        rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
        messBox.setGeometry(rc);
        messBox.exec();
        ui->pushButtonDateTimeSetTime->setFocus(Qt::TabFocusReason);
        return -1;

    }

    unsigned char szBuf[4];
    unsigned char sBuf[10];

    szBuf[0] = (unsigned char)0xf8;
    szBuf[1] = (unsigned char)0x01;
    szBuf[2] = (unsigned char)0x01;
    szBuf[3] = (unsigned char)0xfe;


    socket.write((const char *)szBuf, 4);
    socket.flush();
    int loop = 0;
    int cnt = 0;
    while (socket.waitForReadyRead(1200))
    {
        cnt += socket.read((char *)sBuf, 10);

        if (cnt >= 10) break;

        if (++loop >= 10) break;
    }

    socket.close();

    if(loop >= 10)
    {
        return -1;
    }

    if(sBuf[1] == 0x01 && sBuf[2] == 0x02)
    {
        ui->lineEditYear->setText(QString::number(sBuf[3]) + QString::number(sBuf[4]));

        ui->lineEditMonth->setText(QString::number(sBuf[5]));

        ui->lineEditDay->setText(QString::number(sBuf[6]));

        ui->lineEditHour->setText(QString::number(sBuf[7]));

        ui->lineEditMinute->setText(QString::number(sBuf[8]));

        on_pushButtonDateTimeSetSave_pressed();
    }
    return 0;
}

void DatetimeSetDialog::slot_update_timeout()
{
    if(m_pAutoUpdateTimer->isActive())
    {
        m_pAutoUpdateTimer->stop();
    }
	if(this->isVisible()) return;

    if(m_pDateTimeSetThread->m_bRun)
    {
        m_pDateTimeSetThread->m_bRun = false;
        m_pDateTimeSetThread->quit();
        m_pDateTimeSetThread->wait();
    }
    m_pDateTimeSetThread->m_autoUpdateTimeFlag = true;
    m_pDateTimeSetThread->start();
//	m_nGetTimeCnt++;

//	if(m_nGetTimeCnt > 2)
//	{
//		m_nGetTimeCnt = 0;

//		m_pAutoUpdateTimer->stop();

//		return;
//	}

//	m_pAutoUpdateTimer->stop();

//	char strPeerIP[200] = "";

//	int ret = 0;

//	GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;

//	ret = pGetPeerIPThread->GetPeerIP(g_strLphoneName[1], strPeerIP);

//	char *ptr;
//	if (ret == 0)
//	{
//		int iModalType = BASIC_MODAL_TYPE;
//		ptr = strstr(strPeerIP, "?");
//		if (ptr)
//		{
//			iModalType = atoi(ptr + strlen("?"));
//			*ptr = '\0';
//		}

//		if (!(iModalType & MANCENTER_MODAL_TYPE))
//		{
//			if(m_pAutoUpdateTimer->isActive())
//			{
//				m_pAutoUpdateTimer->stop();
//			}

//			m_pAutoUpdateTimer->start(5 * 60 * 1000);

//			return;
//		}

//		ptr = strstr(strPeerIP, ":");
//		if (ptr)
//		{
//			ptr += strlen(":");
//		}
//		else
//		{
//			ptr = strPeerIP;
//		}

//		if (!ptr[0]) ret = -1;
//	}

//	if (ret != 0)
//	{
//		if(m_pAutoUpdateTimer->isActive())
//		{
//			m_pAutoUpdateTimer->stop();
//		}

//		m_pAutoUpdateTimer->start(5 * 60 * 1000);

//		return;
//	}

//	QTcpSocket socket;

//	socket.connectToHost(ptr, 1300);

//	if (!socket.waitForConnected(750))
//	{
//		if(m_pAutoUpdateTimer->isActive())
//		{
//			m_pAutoUpdateTimer->stop();
//		}

//		m_pAutoUpdateTimer->start(5 * 60 * 1000);

//		return;
//	}

//	unsigned char szBuf[4];
//	unsigned char sBuf[10];

//	szBuf[0] = (unsigned char)0xf8;
//	szBuf[1] = (unsigned char)0x01;
//	szBuf[2] = (unsigned char)0x01;
//	szBuf[3] = (unsigned char)0xfe;


//	socket.write((const char *)szBuf, 4);
//	socket.flush();
//	int loop = 0;
//	int cnt = 0;
//	while (socket.waitForReadyRead(1200))
//	{
//		cnt += socket.read((char *)sBuf, 10);
//		if (cnt >= 10) break;
//		if (++loop >= 10) break;

//	}
//	socket.close();

//	if(loop >= 10)
//	{
//		if(m_pAutoUpdateTimer->isActive())
//		{
//			m_pAutoUpdateTimer->stop();
//		}

//		m_pAutoUpdateTimer->start(5 * 60 * 1000);

//		return;
//	}

//	if(sBuf[1] == 0x01 && sBuf[2] == 0x02)
//	{
//		ui->lineEditYear->setText(QString::number(sBuf[3])+QString::number(sBuf[4]));
//		ui->lineEditMonth->setText(QString::number(sBuf[5]));
//		ui->lineEditDay->setText(QString::number(sBuf[6]));
//		ui->lineEditHour->setText(QString::number(sBuf[7]));
//		ui->lineEditMinute->setText(QString::number(sBuf[8]));

//		if (ui->lineEditMonth->text() == "0")
//		{
//			ui->lineEditMonth->setText("1");
//		}

//		if (ui->lineEditDay->text() == "0")
//		{
//			ui->lineEditDay->setText("1");
//		}

//		QDateTime datetime = QDateTime::currentDateTime();
//		QDateTime datetime_utc(QDate(datetime.toUTC().date().year(), datetime.toUTC().date().month(), datetime.toUTC().date().day()),
//				QTime(datetime.toUTC().time().hour(), datetime.toUTC().time().minute(), datetime.toUTC().time().second()));
//		int time_zone_sec_off = datetime_utc.secsTo(datetime);

//		int y, m, d;
//		int h, mi, s;

//		y = ui->lineEditYear->text().toInt();
//		m = ui->lineEditMonth->text().toInt();
//		d = ui->lineEditDay->text().toInt();

//		h = ui->lineEditHour->text().toInt();
//		mi = ui->lineEditMinute->text().toInt();
//		s = ui->lineEditSecond->text().toInt();

//		struct tm tm_time;
//		struct timeval tv_time;

//		memset(&tm_time, 0, sizeof(struct tm));

//		tm_time.tm_year = y - 1900;
//		tm_time.tm_mon = m - 1;
//		tm_time.tm_mday =d;

//		tm_time.tm_hour = h;
//		tm_time.tm_min = mi;
//		tm_time.tm_sec = s;

//		int rtc;
//		if ((rtc = ::open("/dev/rtc0", O_WRONLY)) < 0)
//		{
//			printf("RTC open error\n");
//		}

//		tv_time.tv_sec = (time_t)(timegm(&tm_time) - time_zone_sec_off);
//		tv_time.tv_usec = 0;

//		settimeofday(&tv_time, NULL);

//		if (rtc >= 0)
//		{
//			if (ioctl(rtc, RTC_SET_TIME, &tm_time) < 0)
//			{
//				printf("RTC write error\n");
//			}

//			::close(rtc);
//		}

//		g_pMainDlg->emitSigResetTimeVaraiable();		// 网络授时结束要变量重设置
//	}
}

void DatetimeSetDialog::emitSigTcpSetTime(int destination)
{
    emit sigTcpSetTime(destination);
}

void DatetimeSetDialog::slotTcpSetTime(int destination)
{
    m_pDateTimeSetThread->m_TcpSetCmmputerTimeResult = SetTimeByTcp(destination);
}

