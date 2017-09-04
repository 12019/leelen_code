#include "remotecheckdialog.h"
#include "ui_remotecheckdialog.h"
#include <qsettings.h>
#include <qtextcodec.h>
#include "common/mymessagebox.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

#define CONFIG_FILE "/mnt/disk/config/config"

RemoteCheckDialog::RemoteCheckDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RemoteCheckDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0)
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	int res = g_pScaleSrc->get_screen_version();
	if 	( res == 1 )  // 800x480
	{
		move(147, 32);
	}
	else if ( res == 2 )  // 1024x600 
	{
		resize(884, 600);
		move(139,10);
		ui->pushButtonSave->setGeometry(QRect(700, 360, 180, 100));
		ui->framePasswordSetting->setGeometry(QRect(10, 160, 680, 140));
		ui->label_3->setGeometry(QRect(10, 70, 154, 70));
		ui->lineEditPassword->setGeometry(QRect(220, 70, 391, 60));
		ui->label->setGeometry(QRect(10, 10, 154, 70));
		ui->lineEditUseName->setGeometry(QRect(220, 10, 381, 60));
	}
	else if ( res == 3 )  // 1280x800
	{
		resize(1089, 800);
		move(185,0);
		ui->pushButtonSave->setGeometry(QRect(852, 540, 246, 112));
		ui->framePasswordSetting->setGeometry(QRect(80, 220, 651, 200));
		ui->label_3->setGeometry(QRect(40, 100, 154, 100));
		ui->lineEditPassword->setGeometry(QRect(230, 100, 421, 100));
		ui->lineEditUseName->setGeometry(QRect(230, 0, 421, 100));
		ui->label->setGeometry(QRect(40, 0, 154, 100));
		ui->label_3->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->lineEditPassword->setStyleSheet(QString::fromUtf8("font: 26px;\n"
							"border: 0px;\n"
							"background-color: transparent;"));

		ui->lineEditUseName->setStyleSheet(QString::fromUtf8("font: 26px;\n"
							"border: 0px;\n"
							"background-color: transparent;"));

		ui->label->setStyleSheet(QString::fromUtf8("font: 27px;"));
	}

	setAttribute(Qt::WA_TranslucentBackground, true);
	ui->lineEditPassword->setText("");
	ui->lineEditUseName->setText("");
}

RemoteCheckDialog::~RemoteCheckDialog()
{
	delete ui;
}

void RemoteCheckDialog::on_lineEditPassword_textChanged(const QString &arg1)
{
	if (arg1.length() > 6)
    {
        ui->lineEditPassword->setText(arg1.left(6));
    }

	if (!check_character_for_number(ui->lineEditPassword->text()))
    {
		ui->lineEditPassword->setText(arg1.left(arg1.length() - 1));
    }
}

void RemoteCheckDialog::on_lineEditUseName_textChanged(const QString &arg1)
{
	if(arg1.length() > 9)
    {
        ui->lineEditUseName->setText(arg1.left(9));
    }
}

bool RemoteCheckDialog::check_character_for_number(QString str_num)
{
	int i;
	for (i = 0; i < str_num.length(); i++)
	{
		if (!str_num.at(i).isDigit())
		{
			return false;
		}
	}

    return true;
}

void RemoteCheckDialog::on_pushButtonSave_pressed()
{
    QSettings Settings(CONFIG_FILE, QSettings::IniFormat);

    Settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

    Settings.setValue("system/remotecheckpassword", ui->lineEditPassword->text());

    MyMessageBox messBox(this);

    CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));


    okButton->setFocus(Qt::TabFocusReason);

    g_pScaleSrc->Backup_Config_File();

    messBox.setText(tr("保存成功!"));

    messBox.show();

    QRect rc = messBox.geometry();

    QRect rc2 = geometry();

    rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());

    messBox.setGeometry(rc);

    messBox.exec();
}

/************************************************************
描述：窗口显示时，行编辑控件文本清空
参数：无
返回：无
************************************************************/
void RemoteCheckDialog::showEvent(QShowEvent *)
{
    QSettings Settings(CONFIG_FILE, QSettings::IniFormat);

    Settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

    QString name = Settings.value("system/native_name","1111-2222").toString();

    QString ps = Settings.value("system/remotecheckpassword", "666666").toString();

    ui->lineEditUseName->setText(name);

    ui->lineEditPassword->setText(ps);
}

/************************************************************
描述：窗口隐藏回调函数
参数：无
返回：无
************************************************************/
void RemoteCheckDialog::hideEvent(QHideEvent *)
{
    //SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();
    //pSysSetMainDlg->ui->pushButtonPasswordSet->setFocus(Qt::TabFocusReason);
}
