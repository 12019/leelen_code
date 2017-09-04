#include "addresssetdialog.h"
#include "ui_addresssetdialog.h"
#include "ui_inputinfodialog.h"
#include "lpcore.h"
#include <QSettings>
#include "maindialog.h"
#include "syssetmaindialog.h"
#include "ui_syssetmaindialog.h"
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include "checkip.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
extern MainDialog *g_pMainDlg;
/************************************************************
  该文件为网络地址设置对话框主程序
************************************************************/
extern int g_vFlagmessage;

extern LinphoneCore *lphone;
extern LinphoneCoreVTable linphonec_vtable;

extern char g_strNativeName[50];
extern char g_strLphoneName[3][50];
extern char g_strNativeIP[50];
extern bool g_network_mask;
#define ENGLISH_INPUT 20
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
描述：判断输入字符串是否为合法的Mac地址
参数：mac_addr_string - 输入字符串
返回：true - 输入字符串是合法的Mac地址
	 false - 输入字符串不是合法的Mac地址
************************************************************/
static bool CheckMacAddr(char *mac_addr_string)
{
	if (strlen(mac_addr_string) != 17) return false;

	char tmpBuf[20];
	strcpy(tmpBuf, mac_addr_string);

	char *strtmp = strtok(tmpBuf, ":");
	int hexnum = 0;
	while (strtmp != NULL)
	{
		if (strlen(strtmp) != 2) return false;
		if ((strtmp[0] >= '0' && strtmp[0] <= '9') || (strtmp[0] >= 'A' && strtmp[0] <= 'F') || (strtmp[0] >= 'a' && strtmp[0] <= 'f'))
		{
			if ((strtmp[1] >= '0' && strtmp[1] <= '9') || (strtmp[1] >= 'A' && strtmp[1] <= 'F') || (strtmp[1] >= 'a' && strtmp[1] <= 'f'))
			{
				hexnum ++;
				strtmp = strtok(NULL, ":");
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	if (hexnum != 6) return false;

	return true;
}

/************************************************************
描述：读取本机Mac地址
参数：无
返回：本机Mac地址字符串
************************************************************/
QString GetLocalMac()
{
	int sock_mac;
	struct ifreq ifr_mac;
	char mac_addr[30];
	sock_mac = socket( AF_INET, SOCK_STREAM, 0 );
	if( sock_mac == -1)
	{
		perror("create socket falise...mac\n");
		return "";
	}
	memset(&ifr_mac,0,sizeof(ifr_mac));
	strncpy(ifr_mac.ifr_name, "eth0", sizeof(ifr_mac.ifr_name)-1);
	if( (ioctl( sock_mac, SIOCGIFHWADDR, &ifr_mac)) < 0)
	{
		close(sock_mac);
		printf("mac ioctl error\n");
		return "";
	}
	sprintf(mac_addr, "%02x:%02x:%02x:%02x:%02x:%02x",
			(unsigned char)ifr_mac.ifr_hwaddr.sa_data[0],
			(unsigned char)ifr_mac.ifr_hwaddr.sa_data[1],
			(unsigned char)ifr_mac.ifr_hwaddr.sa_data[2],
			(unsigned char)ifr_mac.ifr_hwaddr.sa_data[3],
			(unsigned char)ifr_mac.ifr_hwaddr.sa_data[4],
			(unsigned char)ifr_mac.ifr_hwaddr.sa_data[5]);
	printf("local mac:%s \n",mac_addr);
	close( sock_mac );
	return QString( mac_addr );
}

/************************************************************
描述：网络地址设置对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
AddressSetDialog::AddressSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddressSetDialog)
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

        ui->frameAddressSetting->setGeometry(QRect(0, 0, 700, 580));

        //标题
        ui->label_title->setGeometry(QRect(95, 10, 510, 50));

        //第一列
        ui->label_address_1->setGeometry(QRect(0, 73, 280, 70));
        ui->label_address_2->setGeometry(QRect(0, 143, 280, 70));
        ui->label_address_3->setGeometry(QRect(0, 213, 280, 70));
        ui->label_address_4->setGeometry(QRect(0, 283, 280, 70));
        ui->label_address_5->setGeometry(QRect(0, 353, 280, 70));
        ui->label_address_6->setGeometry(QRect(0, 423, 280, 70));
        ui->label_address_7->setGeometry(QRect(0, 493, 280, 70));

        //第二列
        ui->lineEditNativeIP->setGeometry(QRect(280, 73, 420, 70));
        ui->lineEditNetmask->setGeometry(QRect(280, 143, 420, 70));
        ui->lineEditGateway->setGeometry(QRect(280, 213, 420, 70));
        ui->lineEditMacAddr->setGeometry(QRect(280, 283, 420, 70));
        ui->lineEditNativeName->setGeometry(QRect(280, 353, 420, 70));
        ui->lineEditManageCenter1Name->setGeometry(QRect(280, 423, 420, 70));
        ui->lineEditManageCenter2Name->setGeometry(QRect(280, 493, 420, 70));

            ui->label_address_1->setStyleSheet(QString::fromUtf8("font: 24px"));
            ui->label_address_2->setStyleSheet(QString::fromUtf8("font: 24px"));
            ui->label_address_3->setStyleSheet(QString::fromUtf8("font: 24px"));
            ui->label_address_4->setStyleSheet(QString::fromUtf8("font: 24px"));
            ui->label_address_5->setStyleSheet(QString::fromUtf8("font: 24px"));
            ui->label_address_6->setStyleSheet(QString::fromUtf8("font: 24px"));
            ui->label_address_7->setStyleSheet(QString::fromUtf8("font: 24px"));

            ui->lineEditNativeIP->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                    "background-color: transparent;\n"
                                    "font: 24px"));
            ui->lineEditNetmask->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                    "background-color: transparent;\n"
                                    "font: 24px"));
            ui->lineEditManageCenter2Name->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                    "background-color: transparent;\n"
                                    "font: 24px"));
            ui->lineEditGateway->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                    "background-color: transparent;\n"
                                    "font: 24px"));
            ui->lineEditMacAddr->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                    "background-color: transparent;\n"
                                    "font: 24px"));
            ui->lineEditManageCenter1Name->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                    "background-color: transparent;\n"
                                    "font: 24px"));
            ui->lineEditNativeName->setStyleSheet(QString::fromUtf8("border: 0px;\n"
                                    "background-color: transparent;\n"
                                    "font: 24px"));
	}
	else if ( res == 3 )
	{
        resize(858,745);
        move(59,30);


        ui->frameAddressSetting->setGeometry(QRect(0, 0, 858, 745));

        //标题
        ui->label_title->setGeometry(QRect(160, 10, 510, 70));

        //第一列
        ui->label_address_1->setGeometry(QRect(0, 90, 330, 93));
        ui->label_address_2->setGeometry(QRect(0, 183, 330, 93));
        ui->label_address_3->setGeometry(QRect(0, 276, 330, 93));
        ui->label_address_4->setGeometry(QRect(0, 369, 330, 93));
        ui->label_address_5->setGeometry(QRect(0, 462, 330, 93));
        ui->label_address_6->setGeometry(QRect(0, 555, 330, 93));
        ui->label_address_7->setGeometry(QRect(0, 648, 330, 93));

        //第二列
        ui->lineEditNativeIP->setGeometry(QRect(330, 90, 528, 93));
        ui->lineEditNetmask->setGeometry(QRect(330, 183, 528, 93));
        ui->lineEditGateway->setGeometry(QRect(330, 276, 528, 93));
        ui->lineEditMacAddr->setGeometry(QRect(330, 369, 528, 93));
        ui->lineEditNativeName->setGeometry(QRect(330, 462, 528, 93));
        ui->lineEditManageCenter1Name->setGeometry(QRect(330, 555, 528, 93));
        ui->lineEditManageCenter2Name->setGeometry(QRect(330, 648, 528, 70));

        	ui->label_address_1->setStyleSheet(QString::fromUtf8("font: 27px"));
        	ui->label_address_2->setStyleSheet(QString::fromUtf8("font: 27px"));
        	ui->label_address_3->setStyleSheet(QString::fromUtf8("font: 27px"));
        	ui->label_address_4->setStyleSheet(QString::fromUtf8("font: 27px"));
        	ui->label_address_5->setStyleSheet(QString::fromUtf8("font: 27px"));
        	ui->label_address_6->setStyleSheet(QString::fromUtf8("font: 27px"));
        	ui->label_address_7->setStyleSheet(QString::fromUtf8("font: 27px"));

        	ui->lineEditNativeIP->setStyleSheet(QString::fromUtf8("border: 0px;\n"
									"background-color: transparent;\n"
									"font: 26px"));
        	ui->lineEditNetmask->setStyleSheet(QString::fromUtf8("border: 0px;\n"
									"background-color: transparent;\n"
									"font: 26px"));
        	ui->lineEditManageCenter2Name->setStyleSheet(QString::fromUtf8("border: 0px;\n"
									"background-color: transparent;\n"
									"font: 26px"));
        	ui->lineEditGateway->setStyleSheet(QString::fromUtf8("border: 0px;\n"
									"background-color: transparent;\n"
									"font: 26px"));
        	ui->lineEditMacAddr->setStyleSheet(QString::fromUtf8("border: 0px;\n"
									"background-color: transparent;\n"
									"font: 26px"));
        	ui->lineEditManageCenter1Name->setStyleSheet(QString::fromUtf8("border: 0px;\n"
									"background-color: transparent;\n"
									"font: 26px"));
        	ui->lineEditNativeName->setStyleSheet(QString::fromUtf8("border: 0px;\n"
									"background-color: transparent;\n"
									"font: 26px"));
	}

	setAttribute(Qt::WA_TranslucentBackground, true);

    	ui->lineEditMacAddr->setReadOnly(true);
	ui->lineEditNativeName->setMaxLength(11);
	ui->lineEditManageCenter1Name->setMaxLength(9);
	ui->lineEditManageCenter2Name->setMaxLength(9);
	
	ui->lineEditNativeIP->m_CheckType = 2;		// 进行IP地址检查
	ui->lineEditGateway->m_CheckType = 2;
	
    //ui->lineEditNativeName->m_CheckType = 4;
	ui->lineEditManageCenter1Name->m_CheckType = 4;
	ui->lineEditManageCenter2Name->m_CheckType = 4;
}                                                

/************************************************************
描述：网络地址设置对话框析构函数
参数：无
返回：无
************************************************************/
AddressSetDialog::~AddressSetDialog()
{
	delete ui;
}

/************************************************************
描述：从config文件中读取本机网络地址信息，并将信息更新到行编辑控件上
参数：无
返回：0 - 成功
	 -1 - 失败
************************************************************/
int AddressSetDialog::loadData()
{
	QSettings settings(CONFIG_FILE, QSettings::IniFormat);
	QString nativeIP;
	QString netMask;
	QString gateWay;
	QString macAddr;
	QString nativeName;
	QString doorwayName;
	QString manageCenter1Name;
	QString manageCenter2Name;

	nativeIP = settings.value("system/native_ip", "192.168.1.100").toString();
	netMask = settings.value("system/net_mask", "255.255.0.0").toString();
	gateWay = settings.value("system/gate_way", "192.168.1.1").toString();
	//lg
	//macAddr = settings.value("system/mac_addr", "00:01:02:03:04:05").toString();
	macAddr = GetLocalMac();
	//lg
	nativeName = settings.value("system/native_name", "1111-2222").toString();
	//doorwayName = settings.value("system/doorway_name", "1111-0001").toString();
	manageCenter1Name = settings.value("system/manage_center1_name", "9999-0001").toString();
	manageCenter2Name = settings.value("system/manage_center2_name", "9999-0002").toString();

	ui->lineEditNativeIP->setText(nativeIP);
	ui->lineEditNativeIP->m_type = ENGLISH_INPUT;
	ui->lineEditNetmask->setText(netMask);
	ui->lineEditNetmask->m_type = ENGLISH_INPUT;
	ui->lineEditGateway->setText(gateWay);
	ui->lineEditGateway->m_type = ENGLISH_INPUT;
	ui->lineEditMacAddr->setText(macAddr);
	ui->lineEditNativeName->setText(nativeName);
	ui->lineEditManageCenter1Name->setText(manageCenter1Name);
	ui->lineEditManageCenter2Name->setText(manageCenter2Name);

	return 0;
}

/************************************************************
描述：根据给定的字符串创建消息框并显示消息框
参数：text - 消息框显示的文本信息
	 button_caption - 消息框按钮中的文本信息
返回：0 - 成功
	 -1 - 失败
************************************************************/
int AddressSetDialog::MessageBox(QString text, QString button_caption)
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

	return 0;
}

/************************************************************
描述：将行编辑控件上修改后的本机网络地址信息存入config文件中
参数：无
返回：无
************************************************************/
void AddressSetDialog::pushButtonSave_pressed()
{
	g_network_mask = true;

	QString nativeIP;
	QString netMask;
	QString gateWay;
	QString macAddr;
	QString nativeName;
	QString manageCenter1Name;
	QString manageCenter2Name;

	nativeIP = ui->lineEditNativeIP->text();

//	char *a = new char[100];
//	nativeIP.sprintf(a);
//	printf ( "%s\n",a );

	if (!ConfirmIP(nativeIP.toLocal8Bit().data()))
	{
		MessageBox(tr("本地IP地址格式错误!"), QObject::tr("确定"));
		g_network_mask = false;
		return;
	}
    if(strcmp(g_strNativeIP,nativeIP.toLocal8Bit().data())!=0)
    {
        strcpy(g_strNativeIP,nativeIP.toLocal8Bit().data());
        GetPeerIPThread *pGetPeerIPThread = ((MainDialog *)(parent()->parent()))->m_pVoipMainDlg->m_pGetPeerIPThread;
        pGetPeerIPThread ->m_bRun = false;
        pGetPeerIPThread->quit();
        pGetPeerIPThread->wait();
    }
	netMask = ui->lineEditNetmask->text();
	if (!ConfirmIP(netMask.toLocal8Bit().data()))
	{
		MessageBox(tr("子网掩码格式错误!"), QObject::tr("确定"));
		g_network_mask = false;
		return;
	}
	gateWay = ui->lineEditGateway->text();
	printf("gateWay:%s\n",qPrintable(gateWay));
	if (/*(gateWay != "") &&*//*2014.6.10*/ (!ConfirmIP(gateWay.toLocal8Bit().data())))
	{
		MessageBox(tr("网关地址格式错误!"), QObject::tr("确定"));
		g_network_mask = false;
		return;
	}

	macAddr = ui->lineEditMacAddr->text();
	if (!CheckMacAddr(macAddr.toLocal8Bit().data()))
	{
		MessageBox(tr("MAC地址格式错误!"), QObject::tr("确定"));
		g_network_mask = false;
		return;
	}

	nativeName = ui->lineEditNativeName->text();
	if (nativeName == "")
	{
		MessageBox(tr("本机名称不能为空!"), QObject::tr("确定"));
		g_network_mask = false;
		return;
	}

	Del_Space(nativeName);
    QRegExp regExp("^([0-9]{4})-([0-9]{4})$");
    QRegExp regExp2("^([0-9]{4})-([0-9]{4})-([0-9]{1})$");
    if(!regExp.exactMatch(nativeName)&&!regExp2.exactMatch(nativeName))
    {
        MessageBox(tr("本机名称格式错误\n或包含特殊字符!"), QObject::tr("确定"));
        g_network_mask = false;
        return ;
    }

	//doorWayName = ui->lineEditDoorwayName->text();
	manageCenter1Name = ui->lineEditManageCenter1Name->text();
	manageCenter2Name = ui->lineEditManageCenter2Name->text();

	Del_Space(manageCenter1Name);
    if(!regExp.exactMatch(manageCenter1Name))
    {
        MessageBox(tr("管理中心名称格式错误\n或包含特殊字符!"), QObject::tr("确定"));
        g_network_mask = false;
        return ;
    }

	Del_Space(manageCenter2Name);
    if(!regExp.exactMatch(manageCenter2Name))
    {
        MessageBox(tr("中心计算机名称格式错误\n或包含特殊字符!"), QObject::tr("确定"));
        g_network_mask = false;
        return ;
    }


	//return;

	if (ip_check(nativeIP.toLatin1().data()) == -1)
	{
		setAllButtonsEnabled(this, false);
		setAllButtonsEnabled((QDialog *)parent(), false);
		ui->frameAddressSetting->setFocus(Qt::TabFocusReason);

		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		//Q_UNUSED(okButton);
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("本机IP地址和网络中其它某台设备相重"));
		messBox.show();
		QRect rc = messBox.geometry();
		QRect rc2 = geometry();
		rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
		messBox.setGeometry(rc);
		((MainDialog *)(parent()->parent()))->m_pInputMethodWidget->hide();
		messBox.exec();

		setAllButtonsEnabled((QDialog *)parent(), true);
		setAllButtonsEnabled(this, true);

		g_network_mask = false;
		return;
	}

	setAllButtonsEnabled(this, false);
	setAllButtonsEnabled((QDialog *)parent(), false);
	ui->frameAddressSetting->setFocus(Qt::TabFocusReason);

    QSettings settings(CONFIG_FILE, QSettings::IniFormat);
    settings.setValue("system/native_ip", nativeIP);
    settings.setValue("system/net_mask", netMask);
    settings.setValue("system/gate_way", gateWay);
    //settings.setValue("system/mac_addr", macAddr);
    settings.setValue("system/native_name", nativeName);
    //settings.setValue("system/doorway_name", doorWayName);
    settings.setValue("system/manage_center1_name", manageCenter1Name);
    settings.setValue("system/manage_center2_name", manageCenter2Name);

    g_pScaleSrc->Backup_Config_File();

	GetPeerIPThread *pGetPeerIPThread = ((MainDialog *)(parent()->parent()))->m_pVoipMainDlg->m_pGetPeerIPThread;
	pGetPeerIPThread->m_nativeIP = nativeIP;
	pGetPeerIPThread->m_nativeName = nativeName;

	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());
	pMainDlg->m_pCallCenterDlg->setCallCenterName(manageCenter1Name, manageCenter2Name);

	sprintf(g_strNativeName, "%s?%d", nativeName.toLatin1().data(), pMainDlg->m_iCurModalType);
	//strcpy(g_strLphoneName[0], doorWayName.toLatin1().data());
	strcpy(g_strLphoneName[1], manageCenter1Name.toLatin1().data());
	strcpy(g_strLphoneName[2], manageCenter2Name.toLatin1().data());

	char cmd[256];

	system("/sbin/ifconfig eth0 down");

	system("/sbin/ifconfig eth0 up");

	sprintf(cmd,"/sbin/ifconfig eth0 %s netmask %s", nativeIP.toLatin1().data(),
            netMask.toLatin1().data());
	system(cmd);

    system("/sbin/route del default eth0");

	sprintf(cmd, "/sbin/route add default gw %s dev eth0", gateWay.toLatin1().data());
	system(cmd);

	stopWaveThread();

    if(!pGetPeerIPThread->m_bRun)
    {   printf("restart pGetPeerIPThread->start();\n");
        pGetPeerIPThread->start();
    }

	if (lphone)
	{
		linphone_core_uninit(lphone);
	}

	usleep(100000);

	g_vFlagmessage = 1;

	lphone = linphone_core_init(&linphonec_vtable, "linphonerc");

	startWaveThread();

	usleep(1500000);			// 为了让网络状态的改变跳过
	MyMessageBox messBox(this);
	CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
	//Q_UNUSED(okButton);
	okButton->setFocus(Qt::TabFocusReason);
	messBox.setText(tr("网络参数修改成功"));
	messBox.show();
	QRect rc = messBox.geometry();
	QRect rc2 = geometry();
	rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
	messBox.setGeometry(rc);
	((MainDialog *)(parent()->parent()))->m_pInputMethodWidget->hide();
	messBox.exec();

    setAllButtonsEnabled((QDialog *)parent(), true);
    setAllButtonsEnabled(this, true);

    g_network_mask = false;
    emit sigIpHasChange();
}

/************************************************************
描述：窗口显示时，从config文件重新读入本机网络地址信息并更新到行编辑控
	 件上
参数：无
返回：无
************************************************************/
void AddressSetDialog::showEvent(QShowEvent *)
{
	//MainDialog *pMainDlg = (MainDialog *)(parent()->parent());
	//pMainDlg->refreshTabFocus();
    //网络设置的输入法状态设置为数字输入
    	g_pMainDlg->m_pInputMethodWidget->ui->DigitWidget->m_inputMethodStatus = 1;
	loadData();
}

/************************************************************
描述：窗口隐藏回调函数
参数：无
返回：无
************************************************************/
void AddressSetDialog::hideEvent(QHideEvent *)
{
	//MainDialog *pMainDlg = (MainDialog *)(parent()->parent());
	//lg
	//SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();
	//pSysSetMainDlg->ui->pushButtonAddressSet->setFocus(Qt::TabFocusReason);
	//lg
	//pMainDlg->refreshTabFocus();

    //网络设置的输入法状态设置为字母输入
    	g_pMainDlg->m_pInputMethodWidget->ui->DigitWidget->m_inputMethodStatus = 0;
}

/************************************************************
描述：删除字符串中的特殊字符，例如回车，空格
参数：无
返回：无
************************************************************/
void AddressSetDialog::Del_Space(QString& str)
{
	str = str.simplified();
}
