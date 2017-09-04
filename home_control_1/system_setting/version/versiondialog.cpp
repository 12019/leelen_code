#include "versiondialog.h"
#include "ui_versiondialog.h"
#include "maindialog.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

extern QString strSoftVersion;

extern MainDialog *g_pMainDlg;

/************************************************************
  该文件为版本设置对话框主程序
************************************************************/

/************************************************************
描述：版本设置对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
VersionDialog::VersionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionDialog)
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
        ui->lineEdit_HardwareVersion->setText("1.0_20120101");
	}
	else if ( res == 2 )  // 1024x600 
	{
        resize(700, 580);
        move(44,12);

        ui->frameVersionSetting->setGeometry(QRect(0, 0, 700, 580));

        //标题
        ui->label_title->setGeometry(QRect(95, 10, 510, 50));

        //第一列
        ui->label_2->setGeometry(QRect(0, 73, 205, 70));
        ui->label_3->setGeometry(QRect(0, 143, 205, 70));
        ui->label_4->setGeometry(QRect(0, 213, 205, 70));

        //第二列
        ui->lineEdit_ModalType->setGeometry(QRect(205, 73, 495, 70));
        ui->lineEdit_HardwareVersion->setGeometry(QRect(205, 143, 495, 70));
        ui->lineEdit_SoftwareVersion->setGeometry(QRect(205, 213, 495, 70));

        ui->label_2->setStyleSheet(QString::fromUtf8("font: 24px"));
        ui->label_3->setStyleSheet(QString::fromUtf8("font: 24px"));
        ui->label_4->setStyleSheet(QString::fromUtf8("font: 24px"));

        ui->lineEdit_ModalType->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                    "background-color: transparent;\n"
                    "font: 24px"));

        ui->lineEdit_HardwareVersion->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                    "background-color: transparent;\n"
                    "font: 24px"));

        ui->lineEdit_SoftwareVersion->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                    "background-color: transparent;\n"
                    "font: 24px"));
        ui->lineEdit_HardwareVersion->setText("1.0_20120101_6");
	}
	else if ( res == 3 )
	{
		resize(858, 745);
        move(59,30);

		ui->frameVersionSetting->setGeometry(QRect(0, 0, 858, 745));

        //标题
        ui->label_title->setGeometry(QRect(179, 20, 500, 40));

        //第一列
        ui->label_2->setGeometry(QRect(0, 90, 260, 93));
        ui->label_3->setGeometry(QRect(0, 183, 260, 93));
        ui->label_4->setGeometry(QRect(0, 276, 260, 93));

        //第二列
        ui->lineEdit_ModalType->setGeometry(QRect(260, 90, 598, 93));
        ui->lineEdit_HardwareVersion->setGeometry(QRect(260, 183, 598, 93));
        ui->lineEdit_SoftwareVersion->setGeometry(QRect(260, 276, 598, 93));

        ui->label_2->setStyleSheet(QString::fromUtf8("font: 27px"));
        ui->label_3->setStyleSheet(QString::fromUtf8("font: 27px"));
        ui->label_4->setStyleSheet(QString::fromUtf8("font: 27px"));

		ui->lineEdit_ModalType->setStyleSheet(QString::fromUtf8("border: 0px;\n"
						      "background-color: transparent;\n"
						      "font: 26px"));

		ui->lineEdit_HardwareVersion->setStyleSheet(QString::fromUtf8("border: 0px;\n"
						      "background-color: transparent;\n"
						      "font: 26px"));

		ui->lineEdit_SoftwareVersion->setStyleSheet(QString::fromUtf8("border: 0px;\n"
						      "background-color: transparent;\n"
						      "font: 26px"));
        ui->lineEdit_HardwareVersion->setText("1.0_20120101_8");
	}

        ui->lineEdit_ModalType->setEnabled(false);
        ui->lineEdit_ModalType->clearFocus();

        ui->lineEdit_HardwareVersion->setEnabled(false);
        ui->lineEdit_HardwareVersion->clearFocus();

        ui->lineEdit_SoftwareVersion->setEnabled(false);
        ui->lineEdit_SoftwareVersion->clearFocus();

	setAttribute(Qt::WA_TranslucentBackground, true);
}

/************************************************************
描述：版本设置对话框析构函数
参数：无
返回：无
************************************************************/
VersionDialog::~VersionDialog()
{
    delete ui;
}

void VersionDialog::showEvent(QShowEvent *)
{
    ui->lineEdit_SoftwareVersion->setText(strSoftVersion);

    //g_pMainDlg->showRefreshTabFocus(this);
}

void VersionDialog::hideEvent(QHideEvent *)
{
    //g_pMainDlg->hideRefreshTabFocus(this);
}
