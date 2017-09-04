#include "inputinfodialog.h"
#include "ui_inputinfodialog.h"
#include "maindialog.h"
#include "digitpanel.h"
#include "ui_digitpanel.h"
#include "Big5pinyinpanel.h"
#include "ui_big5pinyinpanel.h"
#include "ui_addresssetdialog.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

extern MainDialog *g_pMainDlg;

static bool ConfirmIP(const char* cTrueIP);
static bool ConfirmDigit(char *str);

InputInfoDialog::InputInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputInfoDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23
    QFont textfont("fzltxhkm", 32);
    ui->InputInfoTextEdit->setFont(textfont);

	int res = g_pScaleSrc->get_screen_version();
	if ( res == 2 )  // 1024x600 
	{
		ui->DigitWidget->setGeometry(QRect(0, 179, 1024, 421));
		ui->frame->setGeometry(QRect(0, 0, 1024, 181));

        ui->InputInfoButtonCancel->setGeometry(QRect(609, 120, 166, 58));
        ui->InputInfoButtonConfirm->setGeometry(QRect(824, 120, 166, 58));
        ui->PicLabel->setGeometry(QRect(34, 10, 956, 94));
        ui->InputInfoTextEdit->setGeometry(QRect(34, 10, 956, 94));
        ui->InputInfoTextEdit->setStyleSheet(QString::fromUtf8("font:30px"));


        ui->InputInfoButtonCancel->m_offsetX = -5;
        ui->InputInfoButtonConfirm->m_offsetX = -5;
	}
	else if ( res == 3 )  // 1280x800
	{
		ui->DigitWidget->setGeometry(QRect(0, 230, 1280, 570));

        ui->frame->setGeometry(QRect(0, 0, 1281, 251));
        ui->InputInfoButtonCancel->setGeometry(QRect(761, 176, 206, 72));
        ui->InputInfoButtonConfirm->setGeometry(QRect(1030, 176, 206, 72));
        ui->PicLabel->setGeometry(QRect(40, 18, 1196, 138));
        ui->InputInfoTextEdit->setGeometry(QRect(40, 18, 1196, 138));

        ui->InputInfoTextEdit->setStyleSheet(QString::fromUtf8("font:40px"));
        ui->InputInfoButtonCancel->setStyleSheet(QString::fromUtf8("font:bold 34px;"));
        ui->InputInfoButtonConfirm->setStyleSheet(QString::fromUtf8("font:bold 34px;"));

        ui->InputInfoButtonCancel->m_offsetY = 4;
        ui->InputInfoButtonConfirm->m_offsetY = 4;
        //ui->InputInfoButtonCancel->m_offsetX = -10;
        //ui->InputInfoButtonConfirm->m_offsetX = -10;
	}
	m_Check_Type = 0;  // 不进行检查
}

InputInfoDialog::~InputInfoDialog()
{
	delete ui;
}

/************************************************************
描述：窗口显示g_pMainDlg
参数：无
返回：无
************************************************************/
void InputInfoDialog::showEvent(QShowEvent *)
{
    QString strCancelStyleSheet;
    QString strConfirmStyleSheet;
    QSettings Settings(CONFIG_FILE, QSettings::IniFormat);
    QString lang = Settings.value("language/lang").toString();

    ////g_pMainDlg->showRefreshTabFocus(this);

    ui->InputInfoTextEdit->setFocus(Qt::TabFocusReason);
    //光标设置在文本后
    QTextCursor textCursor = ui->InputInfoTextEdit->textCursor();
    textCursor.movePosition(QTextCursor::End);
    ui->InputInfoTextEdit->setTextCursor(textCursor);

    //设置取消确认按键按钮背景图片
        if (lang == "zh_CN")
        {
            strCancelStyleSheet = " QPushButton {"
                    "background-image: url(:/images/keyboard/cancel1-1.png);"
                    "  }";
            strCancelStyleSheet += " QPushButton:focus {"
                    "background-image: url(:/images/keyboard/cancel1-2.png);"
                    "	}";
            strCancelStyleSheet += " QPushButton:pressed {"
                    "background-image: url(:/images/keyboard/cancel1-2.png);"
                    "  }";

            strConfirmStyleSheet = " QPushButton {"
                    "background-image: url(:/images/keyboard/confirm1-1.png);"
                    "  }";
            strConfirmStyleSheet += " QPushButton:focus {"
                    "background-image: url(:/images/keyboard/confirm1-2.png);"
                    "	}";
            strConfirmStyleSheet += " QPushButton:pressed {"
                    "background-image: url(:/images/keyboard/confirm1-2.png);"
                    "  }";
        }
        else if (lang == "en")
        {
            strCancelStyleSheet = " QPushButton {"
                    "background-image: url(:/images/keyboard/cancel2-1.png);"
                    "  }";
            strCancelStyleSheet += " QPushButton:focus {"
                    "background-image: url(:/images/keyboard/cancel2-2.png);"
                    "	}";
            strCancelStyleSheet += " QPushButton:pressed {"
                    "background-image: url(:/images/keyboard/cancel2-2.png);"
                    "  }";

            strConfirmStyleSheet = " QPushButton {"
                    "background-image: url(:/images/keyboard/confirm2-1.png);"
                    "  }";
            strConfirmStyleSheet += " QPushButton:focus {"
                    "background-image: url(:/images/keyboard/confirm2-2.png);"
                    "	}";
            strConfirmStyleSheet += " QPushButton:pressed {"
                    "background-image: url(:/images/keyboard/confirm2-2.png);"
                    "  }";
        }
        else if (lang == "zh_TW")
        {
            strCancelStyleSheet = " QPushButton {"
                    "background-image: url(:/images/keyboard/cancel1-1.png);"
                    "  }";
            strCancelStyleSheet += " QPushButton:focus {"
                    "background-image: url(:/images/keyboard/cancel1-2.png);"
                    "	}";
            strCancelStyleSheet += " QPushButton:pressed {"
                    "background-image: url(:/images/keyboard/cancel1-2.png);"
                    "  }";

            strConfirmStyleSheet = " QPushButton {"
                    "background-image: url(:/images/keyboard/confirm3-1.png);"
                    "  }";
            strConfirmStyleSheet += " QPushButton:focus {"
                    "background-image: url(:/images/keyboard/confirm3-2.png);"
                    "	}";
            strConfirmStyleSheet += " QPushButton:pressed {"
                    "background-image: url(:/images/keyboard/confirm3-2.png);"
                    "  }";
        }

    ui->InputInfoButtonCancel->setStyleSheet(strCancelStyleSheet);
    ui->InputInfoButtonConfirm->setStyleSheet(strConfirmStyleSheet);
}

/************************************************************
描述：窗口隐藏
参数：无
返回：无
************************************************************/
void InputInfoDialog::hideEvent(QHideEvent *)
{

}

void InputInfoDialog::sigInputMethReceiveData(QString data)
{
    ui->InputInfoTextEdit->setText(data);
}

void InputInfoDialog::on_InputInfoButtonConfirm_pressed()
{
    if (CallInputInfo == 1)
        m_pMyTextEdit->setText(ui->InputInfoTextEdit->toPlainText());
    else if (CallInputInfo == 0)
    {
	    if ( m_Check_Type != 0 )
	    {
		    switch ( m_Check_Type ) 
		    {
			    case 1: 			// 门口机检验
				    {
					    QString t = ui->InputInfoTextEdit->toPlainText();
					    if 	     ( t.length() > 9 || t.at(4) != QChar('-') )
					    {
						MyMessageBox messBox(this);
						CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
						okButton->setFocus(Qt::TabFocusReason);
						messBox.setText(tr("门口机设置异常"));
						messBox.Start_Auto_Close();
						messBox.exec();
						return;
					    }
				    }
				    break;

			    case 2:			// IP地址确认
				    {
					    QString t = ui->InputInfoTextEdit->toPlainText();
					    if ( !ConfirmIP(t.toLatin1().data()))
					    {
						MyMessageBox messBox(this);
						CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
						okButton->setFocus(Qt::TabFocusReason);
						messBox.setText(tr("网络地址设置异常"));
						messBox.Start_Auto_Close();
						messBox.exec();
						return;
					    }
				    }
				    break;

			   case 3:			// door timeout 参数
				    {
					    QString t = ui->InputInfoTextEdit->toPlainText();
                        QByteArray array = t.toLocal8Bit();
                        char *ch = array.data();
					    bool ok;
					    int c = t.toInt(&ok,10);
                        int len = t.length();
                        int i;
                        for(i=0;i<len;i++)
                        {
                            if(!((ch[i]>='0')&&(ch[i]<='9')))
                            {
                                MyMessageBox messBox(this);
                                CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
                                okButton->setFocus(Qt::TabFocusReason);
                                messBox.setText(tr("包含数字以外字符"));
                                messBox.Start_Auto_Close();
                                messBox.exec();
                                return;
                            }
                        }

                        if ( c > 300 || c < 0 )
					    {
                            MyMessageBox messBox(this);
                            CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
                            okButton->setFocus(Qt::TabFocusReason);
                            messBox.setText(tr("输入的数值应在0-300之间"));
                            messBox.Start_Auto_Close();
                            messBox.exec();
                            return;
					    }
				    }
				    break;

			   case 4:
				    {
					    QString t = ui->InputInfoTextEdit->toPlainText();
					    QString t2 = ui->InputInfoTextEdit->toPlainText();
					    t2.replace(4,1,'0');      				// 将正常的中横杠进行替换

					    if 	     ( t.length() > 9 || t.at(4) != QChar('-') || t2.contains(QChar('-')) || t2.contains(QChar('*')) )
					    {
						MyMessageBox messBox(this);
						CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
						okButton->setFocus(Qt::TabFocusReason);
						messBox.setText(tr("名称设置包含异常字符"));
						messBox.Start_Auto_Close();
						messBox.exec();
						return;
					    }
				    }
				    break;

			    default:
				    break;
		    }				/* -----  end switch  ----- */
	    }
	    m_pMyLineEdit->setText(ui->InputInfoTextEdit->toPlainText());
    }

    hide();
}

void InputInfoDialog::on_InputInfoButtonCancel_pressed()
{
    ui->InputInfoTextEdit->clear();
    hide();
}

void InputInfoDialog::Set_Input_Mode(int type)
{
	ui->DigitWidget->m_type = type;
}

void InputInfoDialog::Set_Input_Check_Method(int type)
{
	m_Check_Type = type;
}

/************************************************************
描述：判断输入字符串是否为合法的IP地址
参数：cTrueIP - 输入字符串
返回：true - 输入字符串是合法的IP地址
	 false - 输入字符串不是合法的IP地址
************************************************************/
bool ConfirmIP(const char* cTrueIP)
{
	char *ptr;
	char  ip[16];

	if ( strlen(cTrueIP) == 0 ) return true;     			//  如果是没有字符串也可返回

	if (strlen(cTrueIP) < 7 || strlen(cTrueIP) > 15)
		return false;

	strcpy(ip, cTrueIP);

	if ((ptr = strtok(ip, ".")) == NULL) return false;
	if (atoi(ptr) < 0 || atoi(ptr) > 255) return false;
	if  (!ConfirmDigit(ptr)) return false;

	int i;
	for (i = 1; i < 4; i++)
	{
		if ((ptr = strtok(NULL, ".")) == NULL) return false;
		if (atoi(ptr) < 0 || atoi(ptr) > 255) return false;
		if (!ConfirmDigit(ptr)) return false;
	}

	if (cTrueIP[ptr + strlen(ptr) - ip] != '\0') return false;

	return true;
}

/************************************************************
描述：判断输入字符串是否每个字符都是数字字符
参数：str - 输入字符串
返回：true - 输入字符串每个字符都是数字字符
	 false - 输入字符串不是每个字符都是数字字符
************************************************************/
bool ConfirmDigit(char *str)
{
	int i;
	for (i = 0; i < (int)strlen(str); i++)
	{
		if (isdigit(str[i]) == 0)
		{
			return false;
		}
	}

	return true;
}
