#include "infowritedialog.h"
#include "ui_infowritedialog.h"
#include "maindialog.h"
#include "data_types.h"
#include <QTcpSocket>
//#include "ui_maindialog.h"
#include <QTextCodec>
#include "helpdialog.h"
#include "ui_infomanmaindialog.h"
#include "ui_infomandetaildialog.h"

extern MainDialog *g_pMainDlg;
extern char g_strNativeName[50];

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

InfoWriteDialog::InfoWriteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoWriteDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

	int res = g_pScaleSrc->get_screen_version();
	if ( res == 2 )  // 1024x600
	{
        ui->pushButtonInfoWriteReturn->setGeometry(QRect(764, 500, 230, 80));
		ui->labelInfoTitle->setGeometry(QRect(100, 170, 101, 41));
        ui->pushButtonInfoWriteSend->setGeometry(QRect(764, 420, 230, 80));
		ui->lineEditReceiverName->setGeometry(QRect(300, 110, 215, 41));
		ui->lineEditInfoTitle->setGeometry(QRect(300, 170, 351, 41));
		ui->frameInfoBox->setGeometry(QRect(57, 97, 662, 471));
		ui->labelInfoReceiver->setGeometry(QRect(100, 110, 101, 41));
		ui->line_2->setGeometry(QRect(57, 150, 662, 20));
		ui->line_3->setGeometry(QRect(57, 203, 662, 16));
		ui->line->setGeometry(QRect(240, 100, 20, 111));
		ui->labelInfoBody->setGeometry(QRect(110, 220, 311, 41));
        ui->pushButtonInfoWriteAddress->setGeometry(QRect(764, 340, 230, 80));
		ui->textEditInfoBody->setGeometry(QRect(114, 273, 550, 260));
	}
	else if ( res == 3 )  // 1280x800
	{
        ui->pushButtonInfoWriteReturn->setGeometry(QRect(959, 677, 279, 99));
        ui->labelInfoTitle->setGeometry(QRect(80, 250, 121, 111));
        ui->pushButtonInfoWriteSend->setGeometry(QRect(959, 569, 279, 99));
        ui->frameInfoBox->setGeometry(QRect(80, 139, 811, 611));
        ui->labelInfoReceiver->setGeometry(QRect(81, 140, 121, 111));
        ui->line_2->setGeometry(QRect(80, 241, 811, 16));
        ui->line_3->setGeometry(QRect(80, 350, 811, 16));
        ui->line->setGeometry(QRect(190, 140, 20, 221));
        ui->labelInfoBody->setGeometry(QRect(100, 370, 321, 75));
        ui->pushButtonInfoWriteAddress->setGeometry(QRect(959, 461, 279, 99));
        ui->textEditInfoBody->setGeometry(QRect(100, 450, 771, 271));
        ui->lineEditInfoTitle->setGeometry(QRect(220, 250, 671, 111));
        ui->lineEditReceiverName->setGeometry(QRect(220, 140, 671, 111));
	}

    int x,y,w,h;
    g_pScaleSrc->get_info_framepanel_geometry(x,y,w,h);
    ui->framePanel->setGeometry(x,y,w,h);

    g_pScaleSrc->get_info_label_geometry(x,y,w,h);
    ui->label_title->setGeometry(x,y,w,h);

    m_pSendInfoThread = new SendInfoThread(this);

    connect(this,SIGNAL(sigTcpSendInfo()),this,SLOT(slotTcpSendInfo()));
    connect(this,SIGNAL(sigUdpSendInfo()),this,SLOT(slotUdpSendInfo()));
	//m_bSending = false;
}

InfoWriteDialog::~InfoWriteDialog()
{
	delete ui;
}

void InfoWriteDialog::ResetState(QString strReceiverName)
{
	ui->lineEditReceiverName->setText(strReceiverName);
	ui->lineEditInfoTitle->setText("");
	ui->textEditInfoBody->setText("");
}

/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void InfoWriteDialog::showEvent(QShowEvent *)
{
    g_pMainDlg->showRefreshTabFocus(this);
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void InfoWriteDialog::hideEvent(QHideEvent *)
{
    ui->pushButtonInfoWriteAddress->setEnabled(true);
	g_pMainDlg->m_pAddressBookDlg->hide();

	g_pMainDlg->hideRefreshTabFocus(this);

	if (!g_pMainDlg->m_pInfoManMainDlg->m_pInfoManDetailDlg->isVisible())
	{
        g_pMainDlg->m_pInfoManMainDlg->ui->pushButtonInfoManMainWrite->setFocus(Qt::TabFocusReason);
	}
	else
	{
        g_pMainDlg->m_pInfoManMainDlg->m_pInfoManDetailDlg->ui->pushButtonInfoManDetailReply->setFocus(Qt::TabFocusReason);
	}
}

void InfoWriteDialog::on_pushButtonInfoWriteReturn_pressed()
{
	hide();
}
void InfoWriteDialog::slotUdpSendInfo()
{
    QString strBody = ui->textEditInfoBody->toPlainText();
    QString strTitle =  ui->lineEditInfoTitle->text();
    QString strPeerName = ui->lineEditReceiverName->text();
    QByteArray tmpTitle, tmpBody;
    QTextCodec *gk_codec = QTextCodec::codecForName("UTF-8");
    addr_info_t addr;
    info_t info;
    char loc_ip[20] = {'\0'};
    char loc_usr[20] = {'\0'};
    char rem_ip[20] = {'\0'};
    char rem_usr[20] = {'\0'};
    char strIP[50] = {'\0'};
    if ( strBody == "" )
    {
        setAllButtonsEnabled(this, false);
        ui->framePanel->setFocus(Qt::TabFocusReason);
        MyMessageBox messBox(this);
        CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
        okButton->setFocus(Qt::TabFocusReason);
        messBox.setText(tr("信息不能为空"));
        messBox.exec();
        setAllButtonsEnabled(this, true);

        ui->pushButtonInfoWriteSend->setFocus(Qt::TabFocusReason);
        return;
    }

    if (strPeerName == "")
    {
        return;
    }

    strTitle = ui->lineEditInfoTitle->text();
    tmpTitle = gk_codec->fromUnicode(strTitle);

    strBody = ui->textEditInfoBody->toPlainText();
    tmpBody = gk_codec->fromUnicode(strBody);

    GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;
    strcpy(loc_ip, pGetPeerIPThread->m_nativeIP.toLatin1().data());
    strcpy(loc_usr, pGetPeerIPThread->m_nativeName.toLatin1().data());

    int len = strPeerName.length();
    QString strTemp;
    if ((strPeerName.at(len - 2) == '-') && (strPeerName.at(len - 1).isDigit()))
    {
        strTemp = strPeerName.left(len - 2);
    }
    else
    {
        strTemp = strPeerName;
    }

    if (strTemp.indexOf("-") == -1)
    {
        if (g_strNativeName[0] != '\0')
        {
            int pos;

            strTemp = g_strNativeName;
            pos = strTemp.indexOf("?");
            if (pos >= 0) strTemp = strTemp.left(pos);

            len = strTemp.length();
            if ((strTemp.at(len - 2) == '-') && (strTemp.at(len - 1).isDigit()))
            {
                strTemp = strTemp.left(len - 2);
            }

            pos = strTemp.indexOf("-");
            if (pos != -1)
            {
                strTemp = strTemp.left(pos);
                strPeerName = strTemp + "-" + strPeerName;
            }
        }
    }

    if (pGetPeerIPThread->GetPeerIP(strPeerName.toLocal8Bit().data(), strIP) != 0)
    {
        setAllButtonsEnabled(this, false);
        ui->framePanel->setFocus(Qt::TabFocusReason);
        MyMessageBox messBox(this);
        CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
        //Q_UNUSED(okButton);
        okButton->setFocus(Qt::TabFocusReason);
        messBox.setText(tr("获取对方IP地址失败"));
        messBox.exec();
        setAllButtonsEnabled(this, true);
        ui->pushButtonInfoWriteSend->setFocus(Qt::TabFocusReason);
        return ;
    }
    char *ptr;
    ptr = strstr(strIP, "?");
    if (ptr)
    {
       *ptr = '\0';
    }

    strcpy(rem_ip,strIP);
    strcpy(rem_usr,strPeerName.toLocal8Bit().data());

    addr.loc_ip = (uint8_t *)loc_ip;
    addr.loc_usr = (uint8_t *)loc_usr;
    addr.rem_ip = (uint8_t *)rem_ip;
    addr.rem_usr = (uint8_t *)rem_usr;

    info.title_len = tmpTitle.length();
    info.title = (uint8_t *)tmpTitle.data();
    info.content_len = tmpBody.length();
    info.content = (uint8_t *)tmpBody.data();

    bool ret = start_system_0x0504(&addr,&info,0,NULL,NULL);

    if (ret)
    {
        setAllButtonsEnabled(this, false);
        ui->framePanel->setFocus(Qt::TabFocusReason);
        MyMessageBox messBox(this);
        CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
        okButton->setFocus(Qt::TabFocusReason);
        messBox.setText(tr("信息发送成功"));
        messBox.exec();
        setAllButtonsEnabled(this, true);

        ui->pushButtonInfoWriteSend->setFocus(Qt::TabFocusReason);

        hide();
    }
    else
    {
        setAllButtonsEnabled(this, false);
        ui->framePanel->setFocus(Qt::TabFocusReason);
        MyMessageBox messBox(this);
        CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
        //Q_UNUSED(okButton);
        okButton->setFocus(Qt::TabFocusReason);
        messBox.setText(tr("信息发送失败"));
        messBox.exec();
        setAllButtonsEnabled(this, true);

        ui->pushButtonInfoWriteSend->setFocus(Qt::TabFocusReason);
    }


}
void InfoWriteDialog::emitSigTcpSendInfo()
{
    emit sigTcpSendInfo();
}
void InfoWriteDialog::emitSigUdpSendInfo()
{
    emit sigUdpSendInfo();
}
void InfoWriteDialog::slotTcpSendInfo()
{
    QString strBody = ui->textEditInfoBody->toPlainText();

    if ( strBody == "" )
    {
        setAllButtonsEnabled(this, false);
        ui->framePanel->setFocus(Qt::TabFocusReason);
        MyMessageBox messBox(this);
        CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
        okButton->setFocus(Qt::TabFocusReason);
        messBox.setText(tr("信息不能为空"));
        messBox.exec();
        setAllButtonsEnabled(this, true);

        ui->pushButtonInfoWriteSend->setFocus(Qt::TabFocusReason);
        return;
    }

    QString strPeerName = ui->lineEditReceiverName->text();
    if (strPeerName == "") return;

    setAllButtonsEnabled(this, false);
    ui->framePanel->setFocus(Qt::TabFocusReason);

    char strPeerIP[200] = "192.168.1.53,192.168.1.54?4";;
    int ret = 0;
    GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;

    int len = strPeerName.length();
    QString strTemp;
    if ((strPeerName.at(len - 2) == '-') && (strPeerName.at(len - 1).isDigit()))
    {
        strTemp = strPeerName.left(len - 2);
    }
    else
    {
        strTemp = strPeerName;
    }

    if (strTemp.indexOf("-") == -1)
    {
        if (g_strNativeName[0] != '\0')
        {
            int pos;

            strTemp = g_strNativeName;
            pos = strTemp.indexOf("?");
            if (pos >= 0) strTemp = strTemp.left(pos);

            len = strTemp.length();
            if ((strTemp.at(len - 2) == '-') && (strTemp.at(len - 1).isDigit()))
            {
                strTemp = strTemp.left(len - 2);
            }

            pos = strTemp.indexOf("-");
            if (pos != -1)
            {
                strTemp = strTemp.left(pos);
                strPeerName = strTemp + "-" + strPeerName;
            }
        }
    }

    printf("lg strPeerName:%s\n", strPeerName.toLocal8Bit().data());

    ret = pGetPeerIPThread->GetPeerIP(strPeerName.toLocal8Bit().data(), strPeerIP);

    char *ptr;

    if (ret == 0)
    {
        m_iModalType = BASIC_MODAL_TYPE;
        ptr = strstr(strPeerIP, "?");
        if (ptr)
        {
            m_iModalType = atoi(ptr + strlen("?"));
            *ptr = '\0';
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
        ui->framePanel->setFocus(Qt::TabFocusReason);
        MyMessageBox messBox(this);
        CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
        //Q_UNUSED(okButton);
        okButton->setFocus(Qt::TabFocusReason);
        messBox.setText(tr("获取对方IP地址失败"));
        messBox.exec();
        setAllButtonsEnabled(this, true);
        ui->pushButtonInfoWriteSend->setFocus(Qt::TabFocusReason);
        return;
    }

    ret = -1;
    char *ptr2;
    do
    {
        ptr2 = strstr(ptr, ",");

        if (ptr2)
        {
            *ptr2 = '\0';
        }

        if (sendInfo(ptr) != -1)
        {
            ret = 0;
        }

        if (ptr2)
        {
            ptr = ptr2 + 1;
        }
    }
    while (ptr2 != NULL);

    if (ret != -1)
    {
        setAllButtonsEnabled(this, false);
        ui->framePanel->setFocus(Qt::TabFocusReason);
        MyMessageBox messBox(this);
        CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
        okButton->setFocus(Qt::TabFocusReason);
        messBox.setText(tr("信息发送成功"));
        messBox.exec();
        setAllButtonsEnabled(this, true);

        ui->pushButtonInfoWriteSend->setFocus(Qt::TabFocusReason);

        hide();
    }
    else
    {
        setAllButtonsEnabled(this, false);
        ui->framePanel->setFocus(Qt::TabFocusReason);
        MyMessageBox messBox(this);
        CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
        //Q_UNUSED(okButton);
        okButton->setFocus(Qt::TabFocusReason);
        messBox.setText(tr("信息发送失败"));
        messBox.exec();
        setAllButtonsEnabled(this, true);

        ui->pushButtonInfoWriteSend->setFocus(Qt::TabFocusReason);
    }

    //m_bSending = false;
}
void InfoWriteDialog::on_pushButtonInfoWriteSend_pressed()
{
    if(m_pSendInfoThread->m_bRun)
    {
        m_pSendInfoThread->m_bRun = false;
        m_pSendInfoThread->quit();
        m_pSendInfoThread->wait();
    }
    m_pSendInfoThread->m_sendToName = ui->lineEditReceiverName->text();
    m_pSendInfoThread->start();

//	QString strBody = ui->textEditInfoBody->toPlainText();

//	if ( strBody == "" )
//	{
//		setAllButtonsEnabled(this, false);
//		ui->framePanel->setFocus(Qt::TabFocusReason);
//		MyMessageBox messBox(this);
//		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
//		okButton->setFocus(Qt::TabFocusReason);
//		messBox.setText(tr("信息不能为空"));
//		messBox.exec();
//		setAllButtonsEnabled(this, true);

//		ui->pushButtonInfoWriteSend->setFocus(Qt::TabFocusReason);
//		return;
//	}

//	QString strPeerName = ui->lineEditReceiverName->text();
//	if (strPeerName == "") return;

//	setAllButtonsEnabled(this, false);
//	ui->framePanel->setFocus(Qt::TabFocusReason);

//	char strPeerIP[200] = "192.168.1.53,192.168.1.54?4";;
//	int ret = 0;
//	GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;

//	int len = strPeerName.length();
//	QString strTemp;
//	if ((strPeerName.at(len - 2) == '-') && (strPeerName.at(len - 1).isDigit()))
//	{
//		strTemp = strPeerName.left(len - 2);
//	}
//	else
//	{
//		strTemp = strPeerName;
//	}

//	if (strTemp.indexOf("-") == -1)
//	{
//		if (g_strNativeName[0] != '\0')
//		{
//			int pos;

//			strTemp = g_strNativeName;
//			pos = strTemp.indexOf("?");
//			if (pos >= 0) strTemp = strTemp.left(pos);

//			len = strTemp.length();
//			if ((strTemp.at(len - 2) == '-') && (strTemp.at(len - 1).isDigit()))
//			{
//				strTemp = strTemp.left(len - 2);
//			}

//			pos = strTemp.indexOf("-");
//			if (pos != -1)
//			{
//				strTemp = strTemp.left(pos);
//				strPeerName = strTemp + "-" + strPeerName;
//			}
//		}
//	}

//	printf("lg strPeerName:%s\n", strPeerName.toLocal8Bit().data());

//	ret = pGetPeerIPThread->GetPeerIP(strPeerName.toLocal8Bit().data(), strPeerIP);

//	char *ptr;

//	if (ret == 0)
//	{
//		m_iModalType = BASIC_MODAL_TYPE;
//		ptr = strstr(strPeerIP, "?");
//		if (ptr)
//		{
//			m_iModalType = atoi(ptr + strlen("?"));
//			*ptr = '\0';
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
//		setAllButtonsEnabled(this, false);
//		ui->framePanel->setFocus(Qt::TabFocusReason);
//		MyMessageBox messBox(this);
//		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
//		//Q_UNUSED(okButton);
//		okButton->setFocus(Qt::TabFocusReason);
//		messBox.setText(tr("获取对方IP地址失败"));
//		messBox.exec();
//		setAllButtonsEnabled(this, true);
//		ui->pushButtonInfoWriteSend->setFocus(Qt::TabFocusReason);
//		return;
//	}

//	ret = -1;
//	char *ptr2;
//	do
//	{
//		ptr2 = strstr(ptr, ",");

//		if (ptr2)
//		{
//			*ptr2 = '\0';
//		}

//		if (sendInfo(ptr) != -1)
//		{
//			ret = 0;
//		}

//		if (ptr2)
//		{
//			ptr = ptr2 + 1;
//		}
//	}
//	while (ptr2 != NULL);

//	if (ret != -1)
//	{
//		setAllButtonsEnabled(this, false);
//		ui->framePanel->setFocus(Qt::TabFocusReason);
//		MyMessageBox messBox(this);
//		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
//		okButton->setFocus(Qt::TabFocusReason);
//		messBox.setText(tr("信息发送成功"));
//		messBox.exec();
//		setAllButtonsEnabled(this, true);

//		ui->pushButtonInfoWriteSend->setFocus(Qt::TabFocusReason);

//		hide();
//	}
//	else
//	{
//		setAllButtonsEnabled(this, false);
//		ui->framePanel->setFocus(Qt::TabFocusReason);
//		MyMessageBox messBox(this);
//		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
//		//Q_UNUSED(okButton);
//		okButton->setFocus(Qt::TabFocusReason);
//		messBox.setText(tr("信息发送失败"));
//		messBox.exec();
//		setAllButtonsEnabled(this, true);

//		ui->pushButtonInfoWriteSend->setFocus(Qt::TabFocusReason);
//	}

//	//m_bSending = false;
}

int InfoWriteDialog::sendInfo(char *pIpAddr)
{
	QTcpSocket socket;

	socket.connectToHost(pIpAddr, 554);

	int loop = 0;
	int cnt = 0;

	while (1)
	{
		qApp->processEvents();

		if (socket.waitForConnected(100))
		{
			cnt = 1;
			break;
		}

		if (++loop >= 800/100) break;
	}

	if (cnt == 0) return -1;

	REQUEST_FRAME_T requestFrame;
	unsigned short usSubType = 0;
	unsigned short usLen;
	char strNativeName[50];
	char *ptr;
	QByteArray baTitle, baBody;
	QString strTitle, strBody;
    QTextCodec *gk_codec = QTextCodec::codecForName("GBK");

	strcpy(strNativeName, g_strNativeName);
	ptr = strstr(strNativeName, "?");
	if (ptr) *ptr = '\0';

	strTitle = ui->lineEditInfoTitle->text();
	baTitle = gk_codec->fromUnicode(strTitle);

	strBody = ui->textEditInfoBody->toPlainText();
	baBody = gk_codec->fromUnicode(strBody);

	requestFrame.PackageFlag = PACKAGE_FLAG_UINT32;
	requestFrame.PackageLength.PackageLength32 = 1 + 4 + 1 + 2
		+ 2 + strlen(strNativeName) + 2 + baTitle.length()
		+ 2 + baBody.length() + 2;
	requestFrame.CmdType = CMD_TYPE_INFO_SEND;

	socket.write((char *)&requestFrame.PackageFlag, sizeof(unsigned char));
	socket.write((char *)&requestFrame.PackageLength.PackageLength32, sizeof(uint32_t));
	socket.write((char *)&requestFrame.CmdType, sizeof(uint8_t));

	socket.write((char *)&usSubType, sizeof(unsigned short));

	usLen = strlen(strNativeName);
	socket.write((char *)&usLen, sizeof(unsigned short));
	socket.write((char *)strNativeName, usLen);

	usLen = baTitle.length();
	socket.write((char *)&usLen, sizeof(unsigned short));
	socket.write((char *)baTitle.data(), usLen);

	usLen = baBody.length();
	socket.write((char *)&usLen, sizeof(unsigned short));
	socket.write((char *)baBody.data(), usLen);

	unsigned short usPicCnt = 0;
	socket.write((char *)&usPicCnt, sizeof(unsigned short));

	socket.flush();

	loop = 0;
	cnt = 0;
	char szBuf[3];
    /*
	while (1)
	{
		qApp->processEvents();

		if (socket.waitForReadyRead(200))
		{
			cnt += socket.read((char *)szBuf + cnt, 3 - cnt);
		}

		if (cnt >= 3) break;
		if (++loop >= 2000/200) break;
	}
    */
    while (socket.waitForReadyRead(1200))
    {
            qApp->processEvents();

            cnt += socket.read((char *)szBuf + cnt, 3 - cnt);

            if (cnt >= 3) break;

            if (++loop >= 2000/200) break;
    }
    //david 2014.4.8
	socket.close();

    printf("david senInfo %d:%02x %02x %02x\n", cnt, szBuf[0], szBuf[1], szBuf[2]);

	if ((cnt == 3) && (szBuf[0] == (unsigned char)PACKAGE_FLAG_UINT8)
		&& (szBuf[1] == 3) && (szBuf[2] == 1))
	{
		return 0;
	}

	return -1;
}

void InfoWriteDialog::on_pushButtonInfoWriteAddress_pressed()
{
	if (g_pMainDlg->m_pAddressBookDlg->isVisible()) return;

    ui->pushButtonInfoWriteAddress->setEnabled(false);

	g_pMainDlg->m_pAddressBookDlg->raise();
	g_pMainDlg->m_pAddressBookDlg->show();
}
