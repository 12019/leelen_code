#include "ipcameradialog.h"
#include "ui_ipcameradialog.h"
#include <QSettings>
#include <QTextCodec>
#include "maindialog.h"

#include "mylineedit.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
extern MainDialog *g_pMainDlg;

IpCameraDialog::IpCameraDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IpCameraDialog)
{
    ui->setupUi(this);

    if (strstr(QT_BUILD_KEY, "arm") != 0) //arm-linux-gcc
    {
        setWindowFlags(Qt::FramelessWindowHint);
        hide();
    }

    int i = 0;

    m_pNameLineEdit[0] = ui->lineEditName1;
    m_pNameLineEdit[1] = ui->lineEditName2;
    m_pNameLineEdit[2] = ui->lineEditName3;
    m_pNameLineEdit[3] = ui->lineEditName4;
    m_pNameLineEdit[4] = ui->lineEditName5;
    m_pNameLineEdit[5] = ui->lineEditName6;
    m_pNameLineEdit[6] = ui->lineEditName7;
    m_pNameLineEdit[7] = ui->lineEditName8;

    m_pUserNameLineEdit[0] = ui->lineEditUserName1;
    m_pUserNameLineEdit[1] = ui->lineEditUserName2;
    m_pUserNameLineEdit[2] = ui->lineEditUserName3;
    m_pUserNameLineEdit[3] = ui->lineEditUserName4;
    m_pUserNameLineEdit[4] = ui->lineEditUserName5;
    m_pUserNameLineEdit[5] = ui->lineEditUserName6;
    m_pUserNameLineEdit[6] = ui->lineEditUserName7;
    m_pUserNameLineEdit[7] = ui->lineEditUserName8;

    m_pPwdLineEdit[0] = ui->lineEditPwd1;
    m_pPwdLineEdit[1] = ui->lineEditPwd2;
    m_pPwdLineEdit[2] = ui->lineEditPwd3;
    m_pPwdLineEdit[3] = ui->lineEditPwd4;
    m_pPwdLineEdit[4] = ui->lineEditPwd5;
    m_pPwdLineEdit[5] = ui->lineEditPwd6;
    m_pPwdLineEdit[6] = ui->lineEditPwd7;
    m_pPwdLineEdit[7] = ui->lineEditPwd8;

    m_pUrlLineEdit[0] = ui->lineEditUrl1;
    m_pUrlLineEdit[1] = ui->lineEditUrl2;
    m_pUrlLineEdit[2] = ui->lineEditUrl3;
    m_pUrlLineEdit[3] = ui->lineEditUrl4;
    m_pUrlLineEdit[4] = ui->lineEditUrl5;
    m_pUrlLineEdit[5] = ui->lineEditUrl6;
    m_pUrlLineEdit[6] = ui->lineEditUrl7;
    m_pUrlLineEdit[7] = ui->lineEditUrl8;


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
        ui->frameIpCameraSetting->setGeometry(QRect(0, 0, 700, 580));

        int x = 0, y = 0, w = 0, h = 0;
        //标题
        ui->label_title->setGeometry(QRect(95, 10, 510, 50));

        //第一列
        x = 8, y = 73, w = 188, h = 54;

        ui->label_IpCamera_1->setGeometry(QRect(x, y+0*54, w, h));
        ui->lineEditName1->setGeometry(QRect(x, y+1*54, w, h));
        ui->lineEditName2->setGeometry(QRect(x, y+2*54, w, h));
        ui->lineEditName3->setGeometry(QRect(x, y+3*54, w, h));
        ui->lineEditName4->setGeometry(QRect(x, y+4*54, w, h));
        ui->lineEditName5->setGeometry(QRect(x, y+5*54, w, h));
        ui->lineEditName6->setGeometry(QRect(x, y+6*54, w, h));
        ui->lineEditName7->setGeometry(QRect(x, y+7*54, w, h));
        ui->lineEditName8->setGeometry(QRect(x, y+8*54, w, h));

        //第二列
        x = 195, y = 73, w = 126, h = 54;
        ui->label_IpCamera_2->setGeometry(QRect(x, y+0*54, w, h));
        ui->lineEditUserName1->setGeometry(QRect(x, y+1*54, w, h));
        ui->lineEditUserName2->setGeometry(QRect(x, y+2*54, w, h));
        ui->lineEditUserName3->setGeometry(QRect(x, y+3*54, w, h));
        ui->lineEditUserName4->setGeometry(QRect(x, y+4*54, w, h));
        ui->lineEditUserName5->setGeometry(QRect(x, y+5*54, w, h));
        ui->lineEditUserName6->setGeometry(QRect(x, y+6*54, w, h));
        ui->lineEditUserName7->setGeometry(QRect(x, y+7*54, w, h));
        ui->lineEditUserName8->setGeometry(QRect(x, y+8*54, w, h));

        //第三列
        x = 322, y = 73, w = 126, h = 54;
        ui->label_IpCamera_3->setGeometry(QRect(x, y+0*54, w, h));
        ui->lineEditPwd1->setGeometry(QRect(x, y+1*54, w, h));
        ui->lineEditPwd2->setGeometry(QRect(x, y+2*54, w, h));
        ui->lineEditPwd3->setGeometry(QRect(x, y+3*54, w, h));
        ui->lineEditPwd4->setGeometry(QRect(x, y+4*54, w, h));
        ui->lineEditPwd5->setGeometry(QRect(x, y+5*54, w, h));
        ui->lineEditPwd6->setGeometry(QRect(x, y+6*54, w, h));
        ui->lineEditPwd7->setGeometry(QRect(x, y+7*54, w, h));
        ui->lineEditPwd8->setGeometry(QRect(x, y+8*54, w, h));

        //第四列
        x = 450, y = 73, w = 245, h = 54;
        ui->label_IpCamera_4->setGeometry(QRect(x, y+0*54, w, h));
        ui->lineEditUrl1->setGeometry(QRect(x, y+1*54, w, h));
        ui->lineEditUrl2->setGeometry(QRect(x, y+2*54, w, h));
        ui->lineEditUrl3->setGeometry(QRect(x, y+3*54, w, h));
        ui->lineEditUrl4->setGeometry(QRect(x, y+4*54, w, h));
        ui->lineEditUrl5->setGeometry(QRect(x, y+5*54, w, h));
        ui->lineEditUrl6->setGeometry(QRect(x, y+6*54, w, h));
        ui->lineEditUrl7->setGeometry(QRect(x, y+7*54, w, h));
        ui->lineEditUrl8->setGeometry(QRect(x, y+8*54, w, h));

        ui->label_IpCamera_1->setStyleSheet(QString::fromUtf8("font: 24px;"));
        ui->label_IpCamera_2->setStyleSheet(QString::fromUtf8("font: 24px;"));
        ui->label_IpCamera_3->setStyleSheet(QString::fromUtf8("font: 24px;"));
        ui->label_IpCamera_4->setStyleSheet(QString::fromUtf8("font: 24px;"));

        for(i = 0; i < 8; i++)
        {
            m_pNameLineEdit[i]->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                                                "background-color: transparent;\n"
                                                                "font: 25px"));
            m_pUserNameLineEdit[i]->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                                                "background-color: transparent;\n"
                                                                "font: 25px"));
            m_pPwdLineEdit[i]->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                                                "background-color: transparent;\n"
                                                                "font: 25px"));
            m_pUrlLineEdit[i]->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                                                "background-color: transparent;\n"
                                                                "font: 25px"));
        }
    }
    else if ( res == 3 )  // 1280x800
    {
        resize(858,745);
        move(59,30);

        ui->frameIpCameraSetting->setGeometry(QRect(0, 0, 858, 745));

        int x = 0, y = 0, w = 0, h = 0;
        //标题
        ui->label_title->setGeometry(QRect(179, 10, 510, 70));

        //第一列
        x = 0, y = 90, w = 230, h = 72;
        ui->label_IpCamera_1->setGeometry(QRect(x, y+0*72, w, h));
        ui->lineEditName1->setGeometry(QRect(x, y+1*72, w, h));
        ui->lineEditName2->setGeometry(QRect(x, y+2*72, w, h));
        ui->lineEditName3->setGeometry(QRect(x, y+3*72, w, h));
        ui->lineEditName4->setGeometry(QRect(x, y+4*72, w, h));
        ui->lineEditName5->setGeometry(QRect(x, y+5*72, w, h));
        ui->lineEditName6->setGeometry(QRect(x, y+6*72, w, h));
        ui->lineEditName7->setGeometry(QRect(x, y+7*72, w, h));
        ui->lineEditName8->setGeometry(QRect(x, y+8*72, w, h));

        //第二列
        x = 230, y = 90, w = 155, h = 72;
        ui->label_IpCamera_2->setGeometry(QRect(x, y+0*72, w, h));
        ui->lineEditUserName1->setGeometry(QRect(x, y+1*72, w, h));
        ui->lineEditUserName2->setGeometry(QRect(x, y+2*72, w, h));
        ui->lineEditUserName3->setGeometry(QRect(x, y+3*72, w, h));
        ui->lineEditUserName4->setGeometry(QRect(x, y+4*72, w, h));
        ui->lineEditUserName5->setGeometry(QRect(x, y+5*72, w, h));
        ui->lineEditUserName6->setGeometry(QRect(x, y+6*72, w, h));
        ui->lineEditUserName7->setGeometry(QRect(x, y+7*72, w, h));
        ui->lineEditUserName8->setGeometry(QRect(x, y+8*72, w, h));

        //第三列
        x = 385, y = 90, w = 155, h = 72;
        ui->label_IpCamera_3->setGeometry(QRect(x, y+0*72, w, h));
        ui->lineEditPwd1->setGeometry(QRect(x, y+1*72, w, h));
        ui->lineEditPwd2->setGeometry(QRect(x, y+2*72, w, h));
        ui->lineEditPwd3->setGeometry(QRect(x, y+3*72, w, h));
        ui->lineEditPwd4->setGeometry(QRect(x, y+4*72, w, h));
        ui->lineEditPwd5->setGeometry(QRect(x, y+5*72, w, h));
        ui->lineEditPwd6->setGeometry(QRect(x, y+6*72, w, h));
        ui->lineEditPwd7->setGeometry(QRect(x, y+7*72, w, h));
        ui->lineEditPwd8->setGeometry(QRect(x, y+8*72, w, h));

        //第四列
        x = 540, y = 90, w = 318, h = 72;
        ui->label_IpCamera_4->setGeometry(QRect(x, y+0*72, w, h));
        ui->lineEditUrl1->setGeometry(QRect(x, y+1*72, w, h));
        ui->lineEditUrl2->setGeometry(QRect(x, y+2*72, w, h));
        ui->lineEditUrl3->setGeometry(QRect(x, y+3*72, w, h));
        ui->lineEditUrl4->setGeometry(QRect(x, y+4*72, w, h));
        ui->lineEditUrl5->setGeometry(QRect(x, y+5*72, w, h));
        ui->lineEditUrl6->setGeometry(QRect(x, y+6*72, w, h));
        ui->lineEditUrl7->setGeometry(QRect(x, y+7*72, w, h));
        ui->lineEditUrl8->setGeometry(QRect(x, y+8*72, w, h));

        ui->label_IpCamera_1->setStyleSheet(QString::fromUtf8("font: 27px;"));
        ui->label_IpCamera_2->setStyleSheet(QString::fromUtf8("font: 27px;"));
        ui->label_IpCamera_3->setStyleSheet(QString::fromUtf8("font: 27px;"));
        ui->label_IpCamera_4->setStyleSheet(QString::fromUtf8("font: 27px;"));

        for(i = 0; i < 8; i++)
        {
            m_pNameLineEdit[i]->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                                                "background-color: transparent;\n"
                                                                "font: 26px"));
            m_pUserNameLineEdit[i]->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                                                "background-color: transparent;\n"
                                                                "font: 26px"));
            m_pPwdLineEdit[i]->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                                                "background-color: transparent;\n"
                                                                "font: 26px"));
            m_pUrlLineEdit[i]->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                                                "background-color: transparent;\n"
                                                                "font: 26px"));
        }
    }

    setAttribute(Qt::WA_TranslucentBackground, true);
}

IpCameraDialog::~IpCameraDialog()
{
    delete ui;
}

int IpCameraDialog::loadData()
{
    QSettings settings("/mnt/disk/config/ip_camera", QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

    settings.beginGroup("ip_camera");
    QStringList keys = settings.childKeys();
    int i;

    for (i = 0; i < 8; i++)
    {
        m_pNameLineEdit[i]->setText("");
        m_pUserNameLineEdit[i]->setText("");
        m_pPwdLineEdit[i]->setText("");
        m_pUrlLineEdit[i]->setText("");

	((MyLineEdit *)m_pPwdLineEdit[i])->m_type = 20;
	((MyLineEdit *)m_pUserNameLineEdit[i])->m_type = 20;
	((MyLineEdit *)m_pUrlLineEdit[i])->m_type = 40;

	((MyLineEdit *)m_pUrlLineEdit[i])->m_CheckType = 2;
    }

    for (i = 0; i < keys.count(); i++)
    {
        QString key = keys.at(i);
        int indx = key.left(key.indexOf("&")).toInt() - 1;

        if (indx >= 8)
        {
            continue;
        }

        m_pNameLineEdit[indx]->setText(key.mid(2));

	QString tmp = settings.value(key, "").toString();

	m_pUserNameLineEdit[indx]->setText(tmp.mid(0,tmp.indexOf(":")));
        m_pPwdLineEdit[indx]->setText(tmp.mid(tmp.indexOf(":")+1,tmp.indexOf("@")-tmp.indexOf(":")-1));
        m_pUrlLineEdit[indx]->setText(tmp.mid(tmp.indexOf("@")+1));
    }

    settings.endGroup();

    return 0;
}

void IpCameraDialog::showEvent(QShowEvent *)
{
    //MainDialog *pMainDlg = (MainDialog *)(parent()->parent());
    //pMainDlg->refreshTabFocus();
    loadData();
}

void IpCameraDialog::hideEvent(QHideEvent *)
{

}

void IpCameraDialog::pushButtonSave_pressed()
{
	QString strName, strUrl, strUserName, strPwd, tmp;
    for (int i = 0; i < 8; i++)
	{
		strName = m_pNameLineEdit[i]->text();
		strUrl = m_pUrlLineEdit[i]->text();
		strUserName = m_pUserNameLineEdit[i]->text();
		strPwd = m_pPwdLineEdit[i]->text();

		if ((strName == "") && (strUrl == "") && (strUserName == "") && (strPwd == ""))
		{
			continue;
		}
		if (strName == "")
		{
			MessageBox(tr("第") + QString::number(i + 1) + tr("个名称为空"), QObject::tr("确定"));
			return;
		}
		else if (strUrl == "")
		{
			MessageBox(tr("第") + QString::number(i + 1) + tr("个URL地址为空"), QObject::tr("确定"));
			return;
		}
		else if (strUserName == "")
		{
			MessageBox(tr("第") + QString::number(i + 1) + tr("个用户名为空"), QObject::tr("确定"));
			return;
		}
		else if (strPwd == "")
		{
			MessageBox(tr("第") + QString::number(i + 1) + tr("个密码为空"), QObject::tr("确定"));
			return;
		}
	}

	{
		QSettings settings("/mnt/disk/config/ip_camera", QSettings::IniFormat);
		settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

		settings.beginGroup("ip_camera");
		QStringList keys = settings.childKeys();

		for (int i = 0; i < keys.count(); i++)
		{
			settings.remove(keys.at(i));
		}

        for (int i = 0; i < 8; i++)
		{
			strName = m_pNameLineEdit[i]->text();
			strUrl = m_pUrlLineEdit[i]->text();
			strUserName =m_pUserNameLineEdit[i]->text();
			strPwd = m_pPwdLineEdit[i]->text();

			if ((strName == "") && (strUrl == ""))
			{
				continue;
			}
			tmp = strUserName+":"+strPwd +"@"+strUrl;
			settings.setValue(QString::number(i + 1) + "&" + strName,tmp);
		}

		settings.endGroup();
	}

	g_pMainDlg->m_pVoipMainDlg->m_pMonDlg->m_pMonListDlg->reloadList();

	MessageBox(tr("网路摄像头列表保存成功!"), QObject::tr("确定"));
}

int IpCameraDialog::MessageBox(QString text, QString button_caption)
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
