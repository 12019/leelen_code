#include "callelevatordialog.h"
#include "ui_callelevatordialog.h"
#include "maindialog.h"
#include <Qt/qtcpsocket.h>
#include "ui_maindialog.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

extern MainDialog *g_pMainDlg;
extern char g_strLphoneName[3][50];
extern char g_strNativeName[50];

CallElevatorDialog::CallElevatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CallElevatorDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

    RESCALESCREEN(ui->pushButtonCallElevatorNum0)
    RESCALESCREEN(ui->pushButtonCallElevatorNum1)
    RESCALESCREEN(ui->pushButtonCallElevatorNum2)
    RESCALESCREEN(ui->pushButtonCallElevatorNum3)
    RESCALESCREEN(ui->pushButtonCallElevatorNum4)
    RESCALESCREEN(ui->pushButtonCallElevatorNum5)
    RESCALESCREEN(ui->pushButtonCallElevatorNum6)
    RESCALESCREEN(ui->pushButtonCallElevatorNum7)
    RESCALESCREEN(ui->pushButtonCallElevatorNum8)
    RESCALESCREEN(ui->pushButtonCallElevatorNum9)
    RESCALESCREEN(ui->pushButtonCallElevatorDot)
    RESCALESCREEN(ui->pushButtonCallElevatorShop)
    RESCALESCREEN(ui->pushButtonCallElevatorElevator)
    RESCALESCREEN(ui->pushButtonCallElevatorClear)
    RESCALESCREEN(ui->pushButtonCallElevatorReturn)
	RESCALESCREEN(ui->label_floor_num)
	RESCALESCREEN(ui->lineEditDisplay)

    ui->pushButtonCallElevatorNum0->m_offsetX = 0;
    ui->pushButtonCallElevatorNum1->m_offsetX = 0;
    ui->pushButtonCallElevatorNum2->m_offsetX = 0;
    ui->pushButtonCallElevatorNum3->m_offsetX = 0;
    ui->pushButtonCallElevatorNum4->m_offsetX = 0;
    ui->pushButtonCallElevatorNum5->m_offsetX = 0;
    ui->pushButtonCallElevatorNum6->m_offsetX = 0;
    ui->pushButtonCallElevatorNum7->m_offsetX = 0;
    ui->pushButtonCallElevatorNum8->m_offsetX = 0;
    ui->pushButtonCallElevatorNum9->m_offsetX = 0;
    ui->pushButtonCallElevatorDot->m_offsetX = 0;
    ui->pushButtonCallElevatorShop->m_offsetX = 0;

	//ui->lineEditDisplay->setText("1");
}

CallElevatorDialog::~CallElevatorDialog()
{
    	delete ui;
}

/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void CallElevatorDialog::showEvent(QShowEvent *)
{
	g_pMainDlg->showRefreshTabFocus(this);
    ui->pushButtonCallElevatorNum1->setFocus(Qt::TabFocusReason);

	ui->lineEditDisplay->setText("");
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void CallElevatorDialog::hideEvent(QHideEvent *)
{
	g_pMainDlg->hideRefreshTabFocus(this);

	if (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg)
	{
		g_pMainDlg->ui->pushButtonCallElevator->setFocus(Qt::TabFocusReason);
	}
}

/************************************************************
描述：按了对话框的"0"按钮，在拨号号码显示控件的文本后面添加"0"
参数：无
返回：无
************************************************************/
void CallElevatorDialog::on_pushButtonCallElevatorNum0_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "0");
}

/************************************************************
描述：按了对话框的"1"按钮，在拨号号码显示控件的文本后面添加"1"
参数：无
返回：无
************************************************************/
void CallElevatorDialog::on_pushButtonCallElevatorNum1_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "1");
}

/************************************************************
描述：按了对话框的"2"按钮，在拨号号码显示控件的文本后面添加"2"
参数：无
返回：无
************************************************************/
void CallElevatorDialog::on_pushButtonCallElevatorNum2_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "2");
}

/************************************************************
描述：按了对话框的"3"按钮，在拨号号码显示控件的文本后面添加"3"
参数：无
返回：无
************************************************************/
void CallElevatorDialog::on_pushButtonCallElevatorNum3_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "3");
}

/************************************************************
描述：按了对话框的"4"按钮，在拨号号码显示控件的文本后面添加"4"
参数：无
返回：无
************************************************************/
void CallElevatorDialog::on_pushButtonCallElevatorNum4_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "4");
}

/************************************************************
描述：按了对话框的"5"按钮，在拨号号码显示控件的文本后面添加"5"
参数：无
返回：无
************************************************************/
void CallElevatorDialog::on_pushButtonCallElevatorNum5_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "5");
}

/************************************************************
描述：按了对话框的"6"按钮，在拨号号码显示控件的文本后面添加“6"
参数：无
返回：无
************************************************************/
void CallElevatorDialog::on_pushButtonCallElevatorNum6_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "6");
}

/************************************************************
描述：按了对话框的"7"按钮，在拨号号码显示控件的文本后面添加"7"
参数：无
返回：无
************************************************************/
void CallElevatorDialog::on_pushButtonCallElevatorNum7_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "7");
}

/************************************************************
描述：按了对话框的"8"按钮，在拨号号码显示控件的文本后面添加"8"
参数：无
返回：无
************************************************************/
void CallElevatorDialog::on_pushButtonCallElevatorNum8_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "8");
}

/************************************************************
描述：按了对话框的"9"按钮，在拨号号码显示控件的文本后面添加"9"
参数：无
返回：无
************************************************************/
void CallElevatorDialog::on_pushButtonCallElevatorNum9_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "9");
}

/************************************************************
描述：按了对话框的"."按钮，在拨号号码显示控件的文本后面添加"."
参数：无
返回：无
************************************************************/
void CallElevatorDialog::on_pushButtonCallElevatorDot_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + ".");
}

/************************************************************
描述：按了对话框的"#"按钮，在拨号号码显示控件的文本后面添加"-"
参数：无
返回：无
************************************************************/
void CallElevatorDialog::on_CallElevatorShop_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "-");
}

void CallElevatorDialog::on_pushButtonCallElevatorElevator_clicked()
{
	if (!isVisible()) return;

	if (ui->lineEditDisplay->text() == "") return;

	char strPeerIP[200] = "192.168.1.107?4";
	int ret = 0;
	GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;

	ret = pGetPeerIPThread->GetPeerIP(g_strLphoneName[0], strPeerIP);

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

		if (!(iModalType & DOORWAY_MODAL_TYPE))
		{
			setAllButtonsEnabled(this, false);
			//ui->frame->setFocus(Qt::TabFocusReason);
			MyMessageBox messBox(this);
			CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
			okButton->setFocus(Qt::TabFocusReason);
			messBox.setText(tr("门口机设置错误"));
			messBox.exec();
			setAllButtonsEnabled(this, true);

            ui->pushButtonCallElevatorElevator->setFocus(Qt::TabFocusReason);

			return;
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

	if (ret != 0)
	{
		setAllButtonsEnabled(this, false);
		//ui->frame->setFocus(Qt::TabFocusReason);
		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("获取门口机IP失败"));
		messBox.exec();
		setAllButtonsEnabled(this, true);

        ui->pushButtonCallElevatorElevator->setFocus(Qt::TabFocusReason);

		return;
	}

	QTcpSocket socket;

	socket.connectToHost(ptr, 1206);

	if (!socket.waitForConnected(750))
	{
		setAllButtonsEnabled(this, false);
		//ui->frame->setFocus(Qt::TabFocusReason);
		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("连接门口机失败"));
		messBox.exec();
		setAllButtonsEnabled(this, true);

        ui->pushButtonCallElevatorElevator->setFocus(Qt::TabFocusReason);

		return;
	}

	QString strNativeName(g_strNativeName);
	QString strDoorwayName(g_strLphoneName[0]);
	int pos1 = strNativeName.indexOf("-");
	int pos2 = strDoorwayName.indexOf("-");

	if ((pos1 >= 0) && (pos2 >= 0) && (strNativeName.left(pos1) == strDoorwayName.left(pos2)))
	{
		strNativeName = strNativeName.mid(pos1 + 1);
	}

	pos1 = strNativeName.indexOf("-");
	if (pos1 >= 0) strNativeName = strNativeName.left(pos1);

	int iFloor = strNativeName.left(2).toInt();
	int iRoom = strNativeName.mid(2).toInt();
	int iDestFloor = ui->lineEditDisplay->text().toInt();

	if (iDestFloor > 32767)
	{
		iDestFloor = 32767;
		ui->lineEditDisplay->setText(QString::number(iDestFloor));
	}
	else if (iDestFloor < -32768)
	{
		iDestFloor = -32768;
		ui->lineEditDisplay->setText(QString::number(iDestFloor));
	}

	printf("iFloor:%d,iRoom;%d\n",iFloor,iRoom);

	unsigned char szBuf[7];
	szBuf[0] = (unsigned char)0xf7;
	szBuf[1] = (unsigned char)(iFloor >> 8);
	szBuf[2] = (unsigned char)(iFloor & 0xff);
	szBuf[3] = (unsigned char)iRoom;
	szBuf[4] = (unsigned char)(iDestFloor >> 8);
	szBuf[5] = (unsigned char)(iDestFloor & 0xff);
	szBuf[6] = (unsigned char)0x01;

	socket.write((const char *)szBuf, 7);
	socket.flush();

	int loop = 0;
	int cnt = 0;
	while (socket.waitForReadyRead(1200))
	{
		cnt += socket.read((char *)szBuf + cnt, 2 - cnt);
		if (cnt >= 2) break;
		if (++loop >= 2) break;
	}

	socket.close();

	if ((cnt == 2) && (szBuf[0] == (unsigned char)0xf7) && szBuf[1] == (unsigned char)0x01)
	{
		setAllButtonsEnabled(this, false);
		//ui->frame->setFocus(Qt::TabFocusReason);
		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		//Q_UNUSED(okButton);
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("电梯已呼叫, 清等待..."));
		messBox.exec();
		setAllButtonsEnabled(this, true);

        ui->pushButtonCallElevatorElevator->setFocus(Qt::TabFocusReason);

		hide();
	}
	else
	{
		setAllButtonsEnabled(this, false);
		//ui->frame->setFocus(Qt::TabFocusReason);
		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		//Q_UNUSED(okButton);
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("呼叫电梯失败"));
		messBox.exec();
		setAllButtonsEnabled(this, true);

        ui->pushButtonCallElevatorElevator->setFocus(Qt::TabFocusReason);
	}
}

void CallElevatorDialog::on_pushButtonCallElevatorClear_clicked()
{
	QString str = ui->lineEditDisplay->text();
	str = str.left(str.length() - 1);
	ui->lineEditDisplay->setText(str);
}

void CallElevatorDialog::on_pushButtonCallElevatorReturn_clicked()
{
	if (!isVisible()) return;

	hide();
}

void CallElevatorDialog::on_pushButtonCallElevatorNum0_pressed()
{
    on_pushButtonCallElevatorNum0_clicked();
}

void CallElevatorDialog::on_pushButtonCallElevatorNum1_pressed()
{
    on_pushButtonCallElevatorNum1_clicked();
}

void CallElevatorDialog::on_pushButtonCallElevatorNum2_pressed()
{
    on_pushButtonCallElevatorNum2_clicked();
}

void CallElevatorDialog::on_pushButtonCallElevatorNum3_pressed()
{
    on_pushButtonCallElevatorNum3_clicked();
}

void CallElevatorDialog::on_pushButtonCallElevatorNum4_pressed()
{
    on_pushButtonCallElevatorNum4_clicked();
}

void CallElevatorDialog::on_pushButtonCallElevatorNum5_pressed()
{
    on_pushButtonCallElevatorNum5_clicked();
}

void CallElevatorDialog::on_pushButtonCallElevatorNum6_pressed()
{
    on_pushButtonCallElevatorNum6_clicked();
}

void CallElevatorDialog::on_pushButtonCallElevatorNum7_pressed()
{
    on_pushButtonCallElevatorNum7_clicked();
}

void CallElevatorDialog::on_pushButtonCallElevatorNum8_pressed()
{
    on_pushButtonCallElevatorNum8_clicked();
}

void CallElevatorDialog::on_pushButtonCallElevatorNum9_pressed()
{
    on_pushButtonCallElevatorNum9_clicked();
}

void CallElevatorDialog::on_pushButtonCallElevatorDot_pressed()
{
    on_pushButtonCallElevatorDot_clicked();
}

void CallElevatorDialog::on_pushButtonCallElevatorShop_pressed()
{
    on_CallElevatorShop_clicked();
}

void CallElevatorDialog::on_pushButtonCallElevatorElevator_pressed()
{
    on_pushButtonCallElevatorElevator_clicked();
}

void CallElevatorDialog::on_pushButtonCallElevatorClear_pressed()
{
    on_pushButtonCallElevatorClear_clicked();
}

void CallElevatorDialog::on_pushButtonCallElevatorReturn_pressed()
{
    on_pushButtonCallElevatorReturn_clicked();
}
