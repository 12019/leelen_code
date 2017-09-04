#include "mondialdialog.h"
#include "ui_mondialdialog.h"
#include "maindialog.h"
#include "ui_mondialog.h"

extern MainDialog *g_pMainDlg;

MonDialDialog::MonDialDialog(QWidget *parent) :
    QDialog(parent),
	ui(new Ui::MonDialDialog)
{
    ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	ui->pushButton_dot->setText("");
	ui->pushButton_num_0->setText("");
	ui->pushButton_num_1->setText("");
	ui->pushButton_num_2->setText("");
	ui->pushButton_num_3->setText("");
	ui->pushButton_num_4->setText("");
	ui->pushButton_num_5->setText("");
	ui->pushButton_num_6->setText("");
	ui->pushButton_num_7->setText("");
	ui->pushButton_num_8->setText("");
	ui->pushButton_num_9->setText("");
	ui->pushButton_shop->setText("");
}

MonDialDialog::~MonDialDialog()
{
    delete ui;
}

/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void MonDialDialog::showEvent(QShowEvent *)
{
	//MonDialog *pMonDlg = (MonDialog *)parent();

	//ui->lineEditDisplay->setText("");
	g_pMainDlg->showRefreshTabFocus(this);

	ui->pushButton_num_1->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void MonDialDialog::hideEvent(QHideEvent *)
{
	//MonDialog *pMonDlg = (MonDialog *)parent();

	g_pMainDlg->hideRefreshTabFocus(this);
}

/************************************************************
描述：按了对话框的"0"按钮，在拨号号码显示控件的文本后面添加"0"
参数：无
返回：无
************************************************************/
void MonDialDialog::on_pushButton_num_0_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "0");
}

/************************************************************
描述：按了对话框的"1"按钮，在拨号号码显示控件的文本后面添加"1"
参数：无
返回：无
************************************************************/
void MonDialDialog::on_pushButton_num_1_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "1");
}

/************************************************************
描述：按了对话框的"2"按钮，在拨号号码显示控件的文本后面添加"2"
参数：无
返回：无
************************************************************/
void MonDialDialog::on_pushButton_num_2_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "2");
}

/************************************************************
描述：按了对话框的"3"按钮，在拨号号码显示控件的文本后面添加"3"
参数：无
返回：无
************************************************************/
void MonDialDialog::on_pushButton_num_3_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "3");
}

/************************************************************
描述：按了对话框的"4"按钮，在拨号号码显示控件的文本后面添加"4"
参数：无
返回：无
************************************************************/
void MonDialDialog::on_pushButton_num_4_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "4");
}

/************************************************************
描述：按了对话框的"5"按钮，在拨号号码显示控件的文本后面添加"5"
参数：无
返回：无
************************************************************/
void MonDialDialog::on_pushButton_num_5_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "5");
}

/************************************************************
描述：按了对话框的"6"按钮，在拨号号码显示控件的文本后面添加“6"
参数：无
返回：无
************************************************************/
void MonDialDialog::on_pushButton_num_6_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "6");
}

/************************************************************
描述：按了对话框的"7"按钮，在拨号号码显示控件的文本后面添加"7"
参数：无
返回：无
************************************************************/
void MonDialDialog::on_pushButton_num_7_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "7");
}

/************************************************************
描述：按了对话框的"8"按钮，在拨号号码显示控件的文本后面添加"8"
参数：无
返回：无
************************************************************/
void MonDialDialog::on_pushButton_num_8_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "8");
}

/************************************************************
描述：按了对话框的"9"按钮，在拨号号码显示控件的文本后面添加"9"
参数：无
返回：无
************************************************************/
void MonDialDialog::on_pushButton_num_9_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "9");
}

/************************************************************
描述：按了对话框的"."按钮，在拨号号码显示控件的文本后面添加"."
参数：无
返回：无
************************************************************/
void MonDialDialog::on_pushButton_dot_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + ".");
}

/************************************************************
描述：按了对话框的"#"按钮，在拨号号码显示控件的文本后面添加"-"
参数：无
返回：无
************************************************************/
void MonDialDialog::on_pushButton_shop_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "-");
}

void MonDialDialog::on_pushButtonDial_clicked()
{
	if (!isVisible()) return;

	if (ui->lineEditDisplay->text() != "")
	{
		hide();

		MonDialog *pMonDlg = (MonDialog *)parent();

		pMonDlg->timerCamListTimeout(ui->lineEditDisplay->text());
	}
}

void MonDialDialog::on_pushButtonClear_clicked()
{
	QString str = ui->lineEditDisplay->text();
	str = str.left(str.length() - 1);
	ui->lineEditDisplay->setText(str);
}

void MonDialDialog::on_pushButtonReturn_clicked()
{
	if (!isVisible()) return;

	hide();
}

void MonDialDialog::on_pushButton_num_0_pressed()
{
	on_pushButton_num_0_clicked();
}

void MonDialDialog::on_pushButton_num_1_pressed()
{
	on_pushButton_num_1_clicked();
}

void MonDialDialog::on_pushButton_num_2_pressed()
{
	on_pushButton_num_2_clicked();
}

void MonDialDialog::on_pushButton_num_3_pressed()
{
	on_pushButton_num_3_clicked();
}

void MonDialDialog::on_pushButton_num_4_pressed()
{
	on_pushButton_num_4_clicked();
}

void MonDialDialog::on_pushButton_num_5_pressed()
{
	on_pushButton_num_5_clicked();
}

void MonDialDialog::on_pushButton_num_6_pressed()
{
	on_pushButton_num_6_clicked();
}

void MonDialDialog::on_pushButton_num_7_pressed()
{
	on_pushButton_num_7_clicked();
}

void MonDialDialog::on_pushButton_num_8_pressed()
{
	on_pushButton_num_8_clicked();
}

void MonDialDialog::on_pushButton_num_9_pressed()
{
	on_pushButton_num_9_clicked();
}

void MonDialDialog::on_pushButton_dot_pressed()
{
	on_pushButton_dot_clicked();
}

void MonDialDialog::on_pushButton_shop_pressed()
{
	on_pushButton_shop_clicked();
}

void MonDialDialog::on_pushButtonDial_pressed()
{
	on_pushButtonDial_clicked();
}

void MonDialDialog::on_pushButtonClear_pressed()
{
	on_pushButtonClear_clicked();
}

void MonDialDialog::on_pushButtonReturn_pressed()
{
	on_pushButtonReturn_clicked();
}
