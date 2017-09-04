#include "otherdialog.h"
#include "ui_otherdialog.h"
#include <QSettings>
#include <QTextCodec>
#include "maindialog.h"

#include "mylineedit.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
extern MainDialog *g_pMainDlg;

OtherDialog::OtherDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OtherDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}


	int res = g_pScaleSrc->get_screen_version();
	if 	( res == 1 )  // 800x480
	{
		resize(538, 450);
		move(34, 18);
	}
	else if ( res == 2 )  // 1024x600
	{
		resize(700, 580);
		move(44,12);
		ui->frameOtherSetting->setGeometry(QRect(0, 0, 700, 580));

		int x = 0, y = 0, w = 0, h = 0;
		//标题
		ui->label_title->setGeometry(QRect(95, 10, 510, 50));

		ui->label_other_1->setGeometry(QRect(0, 73, 280, 70));
		ui->label_other_2->setGeometry(QRect(0, 143, 280, 70));

		ui->pushButtonDoorBellType->setGeometry(QRect(280, 73, 420, 70));
		ui->lineEditDoorTimeout->setGeometry(QRect(295, 143, 420, 70));

		ui->label_other_1->setStyleSheet(QString::fromUtf8("font: 24px"));
		ui->label_other_2->setStyleSheet(QString::fromUtf8("font: 24px"));

            	ui->lineEditDoorTimeout->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                    "background-color: transparent;\n"
                                    "font: 24px"));
	}
	else if ( res == 3 )  // 1280x800
	{
		resize(858,745);
		move(59,30);
		ui->frameOtherSetting->setGeometry(QRect(0, 0, 858, 745));

		int x = 0, y = 0, w = 0, h = 0;
		//标题
		ui->label_title->setGeometry(QRect(179, 10, 510, 70));

		ui->label_other_1->setGeometry(QRect(0, 90, 330, 93));
		ui->label_other_2->setGeometry(QRect(0, 183, 330, 93));

		ui->pushButtonDoorBellType->setGeometry(QRect(330, 90, 528, 93));
		ui->lineEditDoorTimeout->setGeometry(QRect(350, 183, 528, 93));

		ui->label_other_1->setStyleSheet(QString::fromUtf8("font: 27px"));
		ui->label_other_2->setStyleSheet(QString::fromUtf8("font: 27px"));

        	ui->lineEditDoorTimeout->setStyleSheet(QString::fromUtf8("border: 0px;\n"
									"background-color: transparent;\n"
									"font: 26px"));
	}

	ui->lineEditDoorTimeout->m_CheckType = 3;			// 进行数量检测

	setAttribute(Qt::WA_TranslucentBackground, true);
}

OtherDialog::~OtherDialog()
{
	delete ui;
}

int OtherDialog::loadData()
{
	int iSpecialDefense;
	QSettings settings(CONFIG_FILE, QSettings::IniFormat);

    iSpecialDefense = settings.value("alert_param/doorbell_type","-1").toInt();

    if(iSpecialDefense == -1)
    {
        iSpecialDefense = settings.value("alert_param/special_defense","0").toInt();
    }

	((MyLineEdit *)ui->lineEditDoorTimeout)->m_type = 30;			// 西文状态

	switch ( iSpecialDefense )
	{
		case 0:
			ui->pushButtonDoorBellType->setText(tr("无"));		// 1
			break;

		case 1:
			ui->pushButtonDoorBellType->setText(tr("非可视门铃"));  // 2
			break;

		case 2:
			ui->pushButtonDoorBellType->setText(tr("可视门铃"));    // 3
			break;

		case 3:
			ui->pushButtonDoorBellType->setText(tr("可视带刷卡门铃")); // 4
			break;
	}

	QString strdoortimeout = settings.value("alert_param/door_timeout_time", "0").toString();

	ui->lineEditDoorTimeout->setText(strdoortimeout);

	return 0;
}

void OtherDialog::showEvent(QShowEvent *)
{
	loadData();
}

void OtherDialog::hideEvent(QHideEvent *)
{

}

void OtherDialog::pushButtonSave_pressed()
{
	int iVal;
	if 	( ui->pushButtonDoorBellType->text() == tr("无") )
		iVal = 0;
	else if ( ui->pushButtonDoorBellType->text() == tr("非可视门铃") )
		iVal = 1;
	else if ( ui->pushButtonDoorBellType->text() == tr("可视门铃") )
		iVal = 2;
	else if ( ui->pushButtonDoorBellType->text() == tr("可视带刷卡门铃") )
		iVal = 3;

    setAllButtonsEnabled(this, false);
    setAllButtonsEnabled((QDialog *)parent(), false);
    ui->frameOtherSetting->setFocus(Qt::TabFocusReason);

	QSettings settings(CONFIG_FILE, QSettings::IniFormat);
    //settings.setValue("alert_param/special_defense",iVal);
    settings.setValue("alert_param/doorbell_type",iVal);

	QString strdoortimeout = ui->lineEditDoorTimeout->text();
	bool bOK;
	iVal = strdoortimeout.toInt(&bOK);
	settings.setValue("alert_param/door_timeout_time", iVal);

	g_pScaleSrc->process_doorbell_functions();			// 进行二次门铃功能处理

	MyMessageBox messBox(this);
    CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
	okButton->setFocus(Qt::TabFocusReason);
	messBox.setText(tr("门铃参数修改成功"));			// 5
	messBox.show();
	QRect rc = messBox.geometry();
	QRect rc2 = geometry();
	rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
	messBox.setGeometry(rc);
	messBox.exec();

    setAllButtonsEnabled((QDialog *)parent(), true);
    setAllButtonsEnabled(this, true);

}

void OtherDialog::on_pushButtonDoorBellType_pressed()
{
	if 	(ui->pushButtonDoorBellType->text() == tr("无"))
		ui->pushButtonDoorBellType->setText(tr("非可视门铃"));
	else if (ui->pushButtonDoorBellType->text() == tr("非可视门铃"))
		ui->pushButtonDoorBellType->setText(tr("可视门铃"));
	else if (ui->pushButtonDoorBellType->text() == tr("可视门铃"))
		ui->pushButtonDoorBellType->setText(tr("可视带刷卡门铃"));
	else
		ui->pushButtonDoorBellType->setText(tr("无"));
}
