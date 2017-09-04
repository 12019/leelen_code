#include "doormachinedialog.h"
#include "ui_doormachinedialog.h"
#include <QSettings>
#include <QTextCodec>
#include "syssetengineerdialog.h"
#include "maindialog.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

extern MainDialog *g_pMainDlg;
extern char g_strLphoneName[3][50];

DoorMachineDialog::DoorMachineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DoorMachineDialog)
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
        ui->frameDoorMachineSetting->setGeometry(QRect(0, 0, 700, 580));

        //标题
        ui->label_title->setGeometry(QRect(95, 10, 510, 50));

        //第一列
        ui->label_DoorMachine_1->setGeometry(QRect(0, 127, 164, 54));
        ui->label_DoorMachine_4->setGeometry(QRect(0, 181, 164, 54));
        ui->label_DoorMachine_5->setGeometry(QRect(0, 235, 164, 54));
        ui->label_DoorMachine_6->setGeometry(QRect(0, 289, 164, 54));
        ui->label_DoorMachine_7->setGeometry(QRect(0, 343, 164, 54));
        ui->label_DoorMachine_8->setGeometry(QRect(0, 397, 164, 54));

        //第二列
        ui->label_DoorMachine_2->setGeometry(QRect(164, 73, 268, 54));
        ui->lineEditName1->setGeometry(QRect(164, 127, 268, 54));
        ui->lineEditName2->setGeometry(QRect(164, 181, 268, 54));
        ui->lineEditName3->setGeometry(QRect(164, 235, 268, 54));
        ui->lineEditName4->setGeometry(QRect(164, 289, 268, 54));
        ui->lineEditName5->setGeometry(QRect(164, 343, 268, 54));
        ui->lineEditName6->setGeometry(QRect(164, 397, 268, 54));

        //第三列
        ui->label_DoorMachine_3->setGeometry(QRect(432, 73, 268, 54));
        ui->lineEditNetName1->setGeometry(QRect(432, 127, 268, 54));
        ui->lineEditNetName2->setGeometry(QRect(432, 181, 268, 54));
        ui->lineEditNetName3->setGeometry(QRect(432, 235, 268, 54));
        ui->lineEditNetName4->setGeometry(QRect(432, 289, 268, 54));
        ui->lineEditNetName5->setGeometry(QRect(432, 343, 268, 54));
        ui->lineEditNetName6->setGeometry(QRect(432, 397, 268, 54));
	}
	else if ( res == 3 )  // 1280x800 
	{
		resize(858,745);
        move(59,30);

        ui->frameDoorMachineSetting->setGeometry(QRect(0, 0, 858, 745));

        //标题
        ui->label_title->setGeometry(QRect(179, 10, 510, 70));

        //第一列
        ui->label_DoorMachine_1->setGeometry(QRect(0, 162, 210, 72));
        ui->label_DoorMachine_4->setGeometry(QRect(0, 234, 210, 72));
        ui->label_DoorMachine_5->setGeometry(QRect(0, 306, 210, 72));
        ui->label_DoorMachine_6->setGeometry(QRect(0, 378, 210, 72));
        ui->label_DoorMachine_7->setGeometry(QRect(0, 450, 210, 72));
        ui->label_DoorMachine_8->setGeometry(QRect(0, 522, 210, 72));

        //第二列
        ui->label_DoorMachine_2->setGeometry(QRect(210, 90, 324, 72));
        ui->lineEditName1->setGeometry(QRect(210, 162, 324, 72));
        ui->lineEditName2->setGeometry(QRect(210, 234, 324, 72));
        ui->lineEditName3->setGeometry(QRect(210, 306, 324, 72));
        ui->lineEditName4->setGeometry(QRect(210, 378, 324, 72));
        ui->lineEditName5->setGeometry(QRect(210, 450, 324, 72));
        ui->lineEditName6->setGeometry(QRect(210, 522, 324, 72));

        //第三列
        ui->label_DoorMachine_3->setGeometry(QRect(534, 90, 324, 72));
        ui->lineEditNetName1->setGeometry(QRect(534, 162, 324, 72));
        ui->lineEditNetName2->setGeometry(QRect(534, 234, 324, 72));
        ui->lineEditNetName3->setGeometry(QRect(534, 306, 324, 72));
        ui->lineEditNetName4->setGeometry(QRect(534, 378, 324, 72));
        ui->lineEditNetName5->setGeometry(QRect(534, 450, 324, 72));
        ui->lineEditNetName6->setGeometry(QRect(534, 522, 324, 72));

		ui->label_DoorMachine_1->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->label_DoorMachine_2->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->label_DoorMachine_3->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->label_DoorMachine_4->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->label_DoorMachine_5->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->label_DoorMachine_6->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->label_DoorMachine_7->setStyleSheet(QString::fromUtf8("font: 27px;"));
		ui->label_DoorMachine_8->setStyleSheet(QString::fromUtf8("font: 27px;"));

		ui->lineEditName1->setStyleSheet(QString::fromUtf8("border: 0px;\n"
					"background-color: transparent;\n"
					"font: 26px"));
		ui->lineEditNetName1->setStyleSheet(QString::fromUtf8("border: 0px;\n"
					"background-color: transparent;\n"
					"font: 26px"));
		ui->lineEditName2->setStyleSheet(QString::fromUtf8("border: 0px;\n"
					"background-color: transparent;\n"
					"font: 26px"));
		ui->lineEditNetName2->setStyleSheet(QString::fromUtf8("border: 0px;\n"
					"background-color: transparent;\n"
					"font: 26px"));
		ui->lineEditName3->setStyleSheet(QString::fromUtf8("border: 0px;\n"
					"background-color: transparent;\n"
					"font: 26px"));
		ui->lineEditNetName3->setStyleSheet(QString::fromUtf8("border: 0px;\n"
					"background-color: transparent;\n"
					"font: 26px"));
		ui->lineEditName4->setStyleSheet(QString::fromUtf8("border: 0px;\n"
					"background-color: transparent;\n"
					"font: 26px"));
		ui->lineEditNetName4->setStyleSheet(QString::fromUtf8("border: 0px;\n"
					"background-color: transparent;\n"
					"font: 26px"));
		ui->lineEditName5->setStyleSheet(QString::fromUtf8("border: 0px;\n"
					"background-color: transparent;\n"
					"font: 26px"));
		ui->lineEditNetName5->setStyleSheet(QString::fromUtf8("border: 0px;\n"
					"background-color: transparent;\n"
					"font: 26px"));
		ui->lineEditName6->setStyleSheet(QString::fromUtf8("border: 0px;\n"
					"background-color: transparent;\n"
					"font: 26px"));
		ui->lineEditNetName6->setStyleSheet(QString::fromUtf8("border: 0px;\n"
					"background-color: transparent;\n"
					"font: 26px"));
	}

	setAttribute(Qt::WA_TranslucentBackground, true);

	m_pNameLineEdit[0] = ui->lineEditName1;
	m_pNameLineEdit[1] = ui->lineEditName2;
	m_pNameLineEdit[2] = ui->lineEditName3;
	m_pNameLineEdit[3] = ui->lineEditName4;
	m_pNameLineEdit[4] = ui->lineEditName5;
	m_pNameLineEdit[5] = ui->lineEditName6;

	ui->lineEditNetName1->m_type = 30;
	ui->lineEditNetName2->m_type = 30;
	ui->lineEditNetName3->m_type = 30;
	ui->lineEditNetName4->m_type = 30;
	ui->lineEditNetName5->m_type = 30;
	ui->lineEditNetName6->m_type = 30;

	ui->lineEditNetName1->m_CheckType = 1;
	ui->lineEditNetName2->m_CheckType = 1;
	ui->lineEditNetName3->m_CheckType = 1;
	ui->lineEditNetName4->m_CheckType = 1;
	ui->lineEditNetName5->m_CheckType = 1;
	ui->lineEditNetName6->m_CheckType = 1;

	m_pNetNameLineEdit[0] = ui->lineEditNetName1;
	m_pNetNameLineEdit[1] = ui->lineEditNetName2;
	m_pNetNameLineEdit[2] = ui->lineEditNetName3;
	m_pNetNameLineEdit[3] = ui->lineEditNetName4;
	m_pNetNameLineEdit[4] = ui->lineEditNetName5;
	m_pNetNameLineEdit[5] = ui->lineEditNetName6;
}

DoorMachineDialog::~DoorMachineDialog()
{
    delete ui;
}

int DoorMachineDialog::loadData()
{
	QSettings settings("/mnt/disk/config/mon_list", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	settings.beginGroup("door_machine");
	QStringList keys = settings.childKeys();
	int i;

	for (i = 0; i < 6; i++)
	{
		m_pNameLineEdit[i]->setText("");
		m_pNetNameLineEdit[i]->setText("");
	}

	for (i = 0; i < keys.count(); i++)
	{
		QString key = keys.at(i);
		int indx = key.left(key.indexOf("&")).toInt() - 1;

		if (indx >= 6)
		{
			continue;
		}

		m_pNameLineEdit[indx]->setText(key.mid(2));
		m_pNetNameLineEdit[indx]->setText(settings.value(key, "").toString());
	}

	settings.endGroup();

	return 0;
}

/************************************************************
描述：窗口显示时，从config文件重新读入本机网络地址信息并更新到行编辑控
	 件上
参数：无
返回：无
************************************************************/
void DoorMachineDialog::showEvent(QShowEvent *)
{
	//MainDialog *pMainDlg = (MainDialog *)(parent()->parent());
	//pMainDlg->refreshTabFocus();
	loadData();
}

/************************************************************
描述：窗口隐藏回调函擿参数：敿返回：敿************************************************************/
void DoorMachineDialog::hideEvent(QHideEvent *)
{

}

void DoorMachineDialog::pushButtonSave_pressed()
{
	QString strName, strNetName;
	int i;
	for (i = 0; i < 6; i++)
	{
		strName = m_pNameLineEdit[i]->text();
		strNetName = m_pNetNameLineEdit[i]->text();

		if ((strName == "") && (strNetName == ""))
		{
			continue;
		}

		if (strName == "")
		{
			MessageBox(tr("第") + QString::number(i + 1) + tr("个地址名称为空"), QObject::tr("确定"));
			return;
		}
		else if (strNetName == "")
		{
			MessageBox(tr("第") + QString::number(i + 1) + tr("个地址网络名为空"), QObject::tr("确定"));
			return;
		}
	}

	QSettings settings("/mnt/disk/config/mon_list", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	settings.beginGroup("door_machine");
	QStringList keys = settings.childKeys();

	for (i = 0; i < keys.count(); i++)
	{
		settings.remove(keys.at(i));
	}

	bool bCopiedToLphone = false;

	for (i = 0; i < 6; i++)
	{
		strName = m_pNameLineEdit[i]->text();
		strNetName = m_pNetNameLineEdit[i]->text();

		if ((strName == "") && (strNetName == ""))
		{
			continue;
		}

		if (!bCopiedToLphone)
		{
			strcpy(g_strLphoneName[0], strNetName.toLatin1().data());
			bCopiedToLphone = true;
		}

		settings.setValue(QString::number(i + 1) + "&" + strName, strNetName);
	}

	settings.endGroup();
	settings.sync();

	g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->m_pMonListDlg->reloadList();

	MessageBox(tr("门口机列表保存成功!"), QObject::tr("确定"));
}

#if 0
void DoorMachineDialog::on_pushButtonDoorMachineSave_pressed()
{
	int i;
	for (i = 0; i < 6; i++)
    {
		QString strName, strNetName;
		strName = m_pNameLineEdit[i]->text();
        strNetName = m_pNetNameLineEdit[i]->text();

		if ((strName == "") && (strNetName == ""))
		{
			continue;
		}

		if (strName == "")
		{
			MessageBox(tr("第") + QString::number(i + 1) + tr("个地址名称为空"), QObject::tr("确定"));
			return;
		}
		else if (strNetName == "")
		{
			MessageBox(tr("第") + QString::number(i + 1) + tr("个地址网络名为空"), QObject::tr("确定"));
            return;
		}
	}

	QSettings settings("/mnt/disk/config/mon_list", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	settings.beginGroup("door_machine");
	QStringList keys = settings.childKeys();

	for (i = 0; i < keys.count(); i++)
	{
		settings.remove(keys.at(i));
	}

	bool bCopiedToLphone = false;

	for (i = 0; i < 6; i++)
	{
		QString strName, strNetName;
		strName = m_pNameLineEdit[i]->text();
		strNetName = m_pNetNameLineEdit[i]->text();

		if ((strName == "") && (strNetName == ""))
		{
			continue;
		}

		if (!bCopiedToLphone)
		{
			strcpy(g_strLphoneName[0], strNetName.toLatin1().data());
			bCopiedToLphone = true;
		}

		settings.setValue(QString::number(i + 1) + "&" + strName, strNetName);
	}

	settings.endGroup();
	settings.sync();

	g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->m_pMonListDlg->reloadList();

	MessageBox(tr("门口机列表保存成功!"), QObject::tr("确定"));
}
#endif

int DoorMachineDialog::MessageBox(QString text, QString button_caption)
{
	setAllButtonsEnabled(this, false);
	setAllButtonsEnabled((QDialog *)parent(), false);

	MyMessageBox messBox(this);
	CommonPushButton *pButton = messBox.addButton(button_caption);
	//Q_UNUSED(pButton);
	pButton->setFocus(Qt::TabFocusReason);
	messBox.setText(text);
	messBox.show();
	QRect rc = messBox.geometry();
	QRect rc2 = geometry();
	rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
	messBox.setGeometry(rc);
	messBox.exec();

	setAllButtonsEnabled((QDialog *)parent(), true);
	setAllButtonsEnabled(this, true);
	//ui->pushButtonDoorMachineSave->setFocus(Qt::TabFocusReason);

	return 0;
}
