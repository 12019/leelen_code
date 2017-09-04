#include "timemode3dialog.h"
#include "ui_timemode3dialog.h"
#include <QTime>
#include "maindialog.h"
//#include "ui_datetimesetdialog.h"

/************************************************************
  该文件为24小时制数字时钟设置对话框主程序
************************************************************/

/************************************************************
描述：24小时制数字时钟设置对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
TimeMode3Dialog::TimeMode3Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimeMode3Dialog)
{
    ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	QRect rc = geometry();
	QRect rc2 = ((QDialog *)parent)->geometry();
	rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
	setGeometry(rc);

	ui->lineEditTime->setText(QTime::currentTime().toString("hh:mm:ss"));
}

/************************************************************
描述：24小时制数字时钟显示设置对话框析构函数
参数：无
返回：无
************************************************************/
TimeMode3Dialog::~TimeMode3Dialog()
{
    delete ui;
}

/************************************************************
描述：主窗口时间设置为使用24小时制数字时钟，并关闭本窗口
参数：无
返回：无
************************************************************/
void TimeMode3Dialog::on_pushButtonSave_clicked()
{
	done(1);
}

/************************************************************
描述：主窗口时间显示方式不变，同时关闭本窗口
参数：无
返回：无
************************************************************/
void TimeMode3Dialog::on_pushButtonCancel_clicked()
{
	done(0);
}

/************************************************************
描述：本窗口显示时，"保存"按钮设置为焦点控件
参数：无
返回：无
************************************************************/
void TimeMode3Dialog::showEvent(QShowEvent *)
{
	//MainDialog *pMainDlg = (MainDialog *)(parent()->parent()->parent());

	//pMainDlg->refreshTabFocus();

	ui->pushButtonSave->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：本窗口显示时，"保存"按钮设置为焦点控件
参数：无
返回：无
************************************************************/
void TimeMode3Dialog::hideEvent(QHideEvent *)
{
	//MainDialog *pMainDlg = (MainDialog *)(parent()->parent()->parent());

	//pMainDlg->refreshTabFocus();
	//((DatetimeSetDialog *)pMainDlg->m_pSysSetMainDlg->m_curDlg)->ui->pushButtonMode3->setFocus(Qt::TabFocusReason);
}

void TimeMode3Dialog::on_pushButtonCancel_pressed()
{
	on_pushButtonCancel_clicked();
}

void TimeMode3Dialog::on_pushButtonSave_pressed()
{
	on_pushButtonSave_clicked();
}
