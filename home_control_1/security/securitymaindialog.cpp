#include "securitymaindialog.h"
#include "ui_securitymaindialog.h"
#include "maindialog.h"
#include <dirent.h>
#include <unistd.h>
#include <sys/vfs.h>
#include "callcenterdialog.h"
#include "securitypassworddialog.h"
#include "securityabnormaldialog.h"
#include "securityplayerdialog.h"
#include "serial1thread.h"
//#include "helpdialog.h"
#include "mediastreamer2/mediastream.h"
#include "maindialog.h"
#include "ui_maindialog.h"
//#include "terminalhelpmaindialog.h"
#include "helpdialog.h"
#include "ui_maindialog.h"
#include "adddefensedialog.h"
#include "exosip_main.h"
#include <linux/watchdog.h>
#include <sys/ioctl.h>
#include <QApplication>
#include "digitpanel.h"
#include "ui_securityalertdialog.h"
#include "ui_securityabnormaldialog.h"
#include "command.h"
#include "exosip_main.h"
#include "core/leelen_comdef.h"
#include "leelen_protocol.h"
#include "leelen_system.h"
#include "core/leelen_utils.h"


#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
extern bool g_Digital_Channel_Flag;				// 标识数字通道的变量
extern int g_door_bell_type;                 // 二次门铃的类型的全局变量

/************************************************************
  该文件为安防管理对话框主程序
************************************************************/
extern int g_iIST_state;
extern int g_iDeviceType;
extern eXosip_t eXosip;
extern eXosip_element_t *eXosipElement;
extern int b_ring_play_run;
extern int g_bIstOrAlertBusy;
extern WriteAVI *g_pWriteAVI;
extern int g_doorbell_on;
extern int g_jpg_image_ready;
extern MainDialog *g_pMainDlg;
extern int g_iIctHungExceteUpEnd;
extern LinphoneCore *lphone;

extern int g_jpg_tvd_image_ready;
extern char g_strLphoneName[3][50];

extern RESULT_E g_check_computercenter_udp_state ;//协议探测结果
void secmaindlg_result_callback(RESULT_E result) //协议探测回调函数
{
    printf("=============[%s -- %d] result = %d\n", __func__, __LINE__, result);
    g_check_computercenter_udp_state = result;
}
/************************************************************
描述：防区延迟报警结构类构造函数
参数：parent - 父对象指针
	 defenseType - 防区类型(有线/无线)
	 isDelayAlarm - 1:是延迟防区
					0:不是延迟防区
返回：无
************************************************************/
DelayAlarmClass::DelayAlarmClass(QObject *parent, unsigned char defenseType, int isDelayAlarm) :
		QObject(parent)
{
	if (isDelayAlarm)
	{
		m_pDelayTimer = new QTimer(this);
		m_pDelayTimer->setSingleShot(true);
	}
	else
	{
		m_pDelayTimer = NULL;
	}

	m_repeatAlarmLastDateTime = QDateTime::currentDateTime().addSecs(-(255 + 1));

	m_defenseType = defenseType;
	m_isTimeOut = 0;
}

/************************************************************
描述：防区延迟报警结构类析构函数
参数：无
返回：无
************************************************************/
DelayAlarmClass::~DelayAlarmClass()
{
	if (m_pDelayTimer)
	{
		m_pDelayTimer->stop();
		delete m_pDelayTimer;
	}

	m_alertDateTimeList.clear();
	m_isTimeOut = 0;
}

/************************************************************
描述：安防管理对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
SecurityMainDialog::SecurityMainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecurityMainDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23
	// make the dialog full screen

	int res = g_pScaleSrc->get_screen_version();
	if 	( res == 2 )  // 1024x600
	{
        ui->pushButtonSecMainReturn->setGeometry(QRect(764, 500, 230, 80));
        ui->pushButtonSecMainNext->setGeometry(QRect(764, 100, 230, 80));
        ui->pushButtonSecMainDel->setGeometry(QRect(764, 340, 230, 80));
        ui->pushButtonSecMainPrev->setGeometry(QRect(764, 20, 230, 80));
        ui->pushButtonSecMainDetail->setGeometry(QRect(764, 260, 230, 80));
        ui->pushButtonSecMainDelAll->setGeometry(QRect(764, 420, 230, 80));
        ui->pushButtonSecMainSetArm->setGeometry(QRect(764, 180, 230, 80));
	}
	else if ( res == 3 )  // 1280x800
	{
        ui->pushButtonSecMainReturn->setGeometry(QRect(959, 677, 279, 99));
        ui->pushButtonSecMainNext->setGeometry(QRect(959, 137, 279, 99));
        ui->pushButtonSecMainDel->setGeometry(QRect(959, 461, 279, 99));
        ui->pushButtonSecMainPrev->setGeometry(QRect(959, 29, 279, 99));
        ui->pushButtonSecMainDetail->setGeometry(QRect(959, 353, 279, 99));
        ui->pushButtonSecMainDelAll->setGeometry(QRect(959, 569, 279, 99));
        ui->pushButtonSecMainSetArm->setGeometry(QRect(959, 245, 279, 99));
	}

	//ui->pushButton->hide();
	int x,y,w,h;
	g_pScaleSrc->get_info_framepanel_geometry(x,y,w,h);
	ui->framePanel->setGeometry(x,y,w,h);

	QStandardItemModel *model = new QStandardItemModel(0, 9);
	
	g_pScaleSrc->get_tabviewinfolist_geometry(x,y,w,h);
	ui->tableViewSecurityList->setGeometry(x,y,w,h);

	ui->tableViewSecurityList->setModel(model);
	ui->tableViewSecurityList->setShowGrid(true);
	ui->tableViewSecurityList->verticalHeader()->hide();
	ui->tableViewSecurityList->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableViewSecurityList->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableViewSecurityList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableViewSecurityList->setSortingEnabled(false);//lg lg true);
	ui->tableViewSecurityList->setFocusPolicy(Qt::NoFocus);
        ui->tableViewSecurityList->horizontalHeader()->setHighlightSections(false);

	/*
	model->setHeaderData(0, Qt::Horizontal, tr("时间"));
	model->setHeaderData(1, Qt::Horizontal, tr("防区类型"));
	model->setHeaderData(2, Qt::Horizontal, tr("防区号码"));
	model->setHeaderData(3, Qt::Horizontal, tr("防区名称"));
	model->setHeaderData(4, Qt::Horizontal, tr("安装位置"));
	model->setHeaderData(5, Qt::Horizontal, tr("视频"));
	model->setHeaderData(6, Qt::Horizontal, tr("毫秒"));
	*/

	g_pScaleSrc->get_info_tabviewSecurityList_width(x,y,w,h);
	ui->tableViewSecurityList->setColumnWidth(0, x);
	ui->tableViewSecurityList->setColumnWidth(1, y);
	ui->tableViewSecurityList->setColumnWidth(2, w);
	ui->tableViewSecurityList->setColumnWidth(3, h);

	ui->tableViewSecurityList->setColumnWidth(4, 50);
	ui->tableViewSecurityList->setColumnWidth(5, 50);
	ui->tableViewSecurityList->setColumnWidth(6, 50);
	ui->tableViewSecurityList->setColumnWidth(7, 50);
	ui->tableViewSecurityList->setColumnWidth(8, 50);

	ui->tableViewSecurityList->setColumnHidden(4, true);
	ui->tableViewSecurityList->setColumnHidden(5, true);
	ui->tableViewSecurityList->setColumnHidden(6, true);
	ui->tableViewSecurityList->setColumnHidden(7, true);
	ui->tableViewSecurityList->setColumnHidden(8, true);

	ui->tableViewSecurityList->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
	ui->tableViewSecurityList->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
	ui->tableViewSecurityList->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
	ui->tableViewSecurityList->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
	//ui->tableViewSecurityList->horizontalHeader()->setResizeMode(4, QHeaderView::Fixed);
	//ui->tableViewSecurityList->horizontalHeader()->setResizeMode(5, QHeaderView::Fixed);

	ui->tableViewSecurityList->setIconSize(QSize(g_pScaleSrc->get_w_value(45), g_pScaleSrc->get_h_value(26)));

	g_pScaleSrc->get_info_label_geometry(x,y,w,h);
	ui->label_title->setGeometry(x,y,w,h);

	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(slotSecurityListPressed()));

    connect(this,SIGNAL(sigPlayWav(int)),this,SLOT(slotPlayWav(int)));
    connect(this,SIGNAL(sigUploadDeviceInfo(int)),this,SLOT(slotUploadDeviceInfo(int)));

	//connect(ui->tableViewSecurityList->horizontalHeader(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
	//		this, SLOT(tvSecurityListSortIndicatorChanged(int, Qt::SortOrder)));

	int i;
	QString defense, strNumber;
	int bDelayAlarm, bBypass;
	QSettings settings("/mnt/disk/config/defense.conf", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF8"));
	m_wireDefenseCnt = 0;
	m_wirelessDefenseCnt = 0;

	memset(m_delayAlarmClass, 0, sizeof(m_delayAlarmClass));

	for (i = 0; i < 56; i++)
	{
		defense = QString("wiredefense") + QString::number(i + 1);
		strNumber = settings.value(defense + "/number", "-1").toString();
		if (strNumber != "-1")
		{
			bDelayAlarm = settings.value(defense + "/delay_alarm", "0").toInt();
			bBypass = settings.value(defense + "/bypass", "0").toInt();

			m_delayAlarmClass[m_wireDefenseCnt] = new DelayAlarmClass(this, (unsigned char)0xa0, (!bBypass) && bDelayAlarm);
			m_allDefenseNumber[m_wireDefenseCnt++] = strNumber.toInt();

			printf("wire %d %d\n", m_wireDefenseCnt, strNumber.toInt());
		}

		defense = QString("wirelessdefense") + QString::number(i + 1);
		strNumber = settings.value(defense + "/number", "-1").toString();
		if (strNumber != "-1")
		{
			bDelayAlarm = settings.value(defense + "/delay_alarm", "0").toInt();
			bBypass = settings.value(defense + "/bypass", "0").toInt();

			m_delayAlarmClass[56 + m_wirelessDefenseCnt] = new DelayAlarmClass(this, (unsigned char)0xa1, (!bBypass) && bDelayAlarm);
			m_allDefenseNumber[56 + (m_wirelessDefenseCnt++)] = strNumber.toInt();

			printf("wireless %d %d\n", m_wirelessDefenseCnt, strNumber.toInt());
		}
	}

	m_pSecurityAlertDlg = new SecurityAlertDialog(this);
	m_pSecurityPlayerDlg = new SecurityPlayerDialog(this);
	m_pSecurityAbnormalDlg = new SecurityAbnormalDialog(this);
	m_pSecurityPasswordDlg = new SecurityPasswordDialog(this);

	QSettings settings_config(CONFIG_FILE, QSettings::IniFormat);

	m_iEnterDelay = settings_config.value("alert_param/enter_delay", "40").toInt();
	m_iExitDelay = settings_config.value("alert_param/exit_delay", "60").toInt();
	m_iRepeatAlarmFilterInterval = settings_config.value("alert_param/repeat_alarm_filter_interval", "60").toInt();
	m_iSendPicToCenter = settings_config.value("alert_param/send_pic_to_center", "0").toInt();

	m_pSerial1Thread = new Serial1Thread(this);
	m_pSerial1Thread->uartInit();
	m_pSerial1Thread->start();

    m_pUploadDeviceInfoThread = new UploadDeviceInfoThread(this);
    m_pUploadSetSecurityStateThread = new UploadSetSecurityStateThread(this);

	m_securityState = settings_config.value("alert_param/security_state", STATUS_GOOUT).toInt();
	setSecurityState(m_securityState);//ATHOME);

	for (i = 0; i < m_wireDefenseCnt; i++)
	{
		if (m_delayAlarmClass[i]->m_pDelayTimer)
		{
			connect(m_delayAlarmClass[i]->m_pDelayTimer, SIGNAL(timeout()), m_pSerial1Thread, SLOT(enterDelayTimeOut()));
		}
	}

	for (i = 0; i < m_wirelessDefenseCnt; i++)
	{
		if (m_delayAlarmClass[56 + i]->m_pDelayTimer)
		{
			connect(m_delayAlarmClass[56 + i]->m_pDelayTimer, SIGNAL(timeout()), m_pSerial1Thread, SLOT(enterDelayTimeOut()));
		}
	}

	m_newDefenseCnt = settings_config.value("info/new_defense_cnt", 0).toInt();

	QStandardItemModel *model2 = (QStandardItemModel *)g_pMainDlg->ui->tableViewNotifier->model();
	if (m_newDefenseCnt == 0)
	{
        model2->item(4, 1)->setIcon(QIcon(":/images/main/alarm_none.png"));
        model2->item(4, 2)->setText(MainDialog::tr("无未接警报"));
	}
	else
	{
        model2->item(4, 1)->setIcon(QIcon(":/images/main/alarm.png"));
        model2->item(4, 2)->setText(MainDialog::tr("有") + QString::number(m_newDefenseCnt) + MainDialog::tr("条未接警报"));
	}
	//((MainDialog *)parent)->getUi()->labelIconDefenseCnt->setText("\n\n" + QString::number(m_newDefenseCnt));
    m_bRecording = false;
    m_bSecurityMainDlgAloneOpened = false; //false;
    m_DeviceInfoSocket = NULL;
	reloadSecurityList();

	createJpegSendThread();

	if(g_iDeviceType == TOUCHR)
	{
		m_pDefenseGPIOScanThread = new DefenseGPIOScanThread(this);

		m_pDefenseGPIOScanThread->start();

		m_pSerial1Thread->m_pExitDelayTimer->stop();

		m_pSerial1Thread->m_pExitDelayTimer->start(m_iExitDelay * 1000);
	}

	connect(g_pMainDlg,SIGNAL(sigSetRowHeight(int, int)),this,SLOT(slotSetSecRowHeight(int, int)));
    connect(this,SIGNAL(sigHandleReceiveUdpData(unsigned char)),this,SLOT(slotHandleReceiveUdpdata(unsigned char)));

#if 1

	m_pSecurityRecvDoorwayServer = new SecurityRecvDoorwayServer(this);

	m_pSecurityRecvDoorwayServer->listen(QHostAddress::Any,17777);
#endif
}


/************************************************************
描述：安防管理对话框析构函数
参数：无
返回：无
************************************************************/
SecurityMainDialog::~SecurityMainDialog()
{
	m_pSerial1Thread->m_bRun = 0;
	m_pSerial1Thread->quit();
	m_pSerial1Thread->wait();
	delete m_pSerial1Thread;

	int i;
	for (i = 0; i < m_wireDefenseCnt; i++)
	{
		delete m_delayAlarmClass[i];
	}

	for (i = 0; i < m_wirelessDefenseCnt; i++)
	{
		delete m_delayAlarmClass[56 + i];
	}

	delete m_pSecurityAlertDlg;
	delete m_pSecurityPlayerDlg;
	delete m_pSecurityAbnormalDlg;
	delete m_pSecurityPasswordDlg;

	delete m_pTimer;

	delete ui;
}

/************************************************************
描述：获取报警记录列表控件
参数：无
返回：报警记录列表控件的指针
************************************************************/
QTableView *SecurityMainDialog::getTableViewSecurityList()
{
	return ui->tableViewSecurityList;
}

/************************************************************
描述：设置当前设防状态，并将状态存入config文件
参数：state - STATUS_DISARM:撤防状态
			 STATUS_GOOUT:外出设防状态
			 STATUS_ATHOME:居家设防状态
返回：无
************************************************************/
void SecurityMainDialog::setSecurityState(int state)
{
	printf("setSecurityState\n");
	Ui::MainDialog *pMainDlgUi = ((MainDialog *)parent())->getUi();
	QStandardItemModel *model = (QStandardItemModel *)pMainDlgUi->tableViewNotifier->model();

	QString strTmp;

	switch (state)
	{
	case STATUS_DISARM:
		model->item(0, 1)->setIcon(QIcon(":/images/main/disarm.png"));
		model->item(0, 2)->setText(MainDialog::tr("撤防"));

		strTmp = SecurityAbnormalDialog::tr("撤防");

		break;
	case STATUS_GOOUT:
		m_pSerial1Thread->m_pExitDelayTimer->stop();
		if (m_iExitDelay != 0)
		{
			m_pSerial1Thread->m_pExitDelayTimer->start(m_iExitDelay * 1000);
		}

		model->item(0, 1)->setIcon(QIcon(":/images/main/goout.png"));
		model->item(0, 2)->setText(MainDialog::tr("外出"));

		strTmp = SecurityAbnormalDialog::tr("外出设防");

		break;
	case STATUS_ATHOME:
		m_pSerial1Thread->m_pExitDelayTimer->stop();
		if (m_iExitDelay != 0)
		{
			m_pSerial1Thread->m_pExitDelayTimer->start(m_iExitDelay * 1000);
		}

		model->item(0, 1)->setIcon(QIcon(":/images/main/athome.png"));
		model->item(0, 2)->setText(MainDialog::tr("居家"));

		strTmp = SecurityAbnormalDialog::tr("居家设防");

		break;
	}

	if (m_securityState != state)
	{
		QSettings settings(CONFIG_FILE, QSettings::IniFormat);
		settings.setValue("alert_param/security_state", QString::number(state));
	}

	QDate date = QDate::currentDate();
	QString strInfo;
	char strTmp2[20];
	sprintf(strTmp2, "%d", date.month());
	strInfo = strTmp2 + tr("月");
	sprintf(strTmp2, "%d", date.day());
	strInfo += strTmp2 + tr("日");
	strInfo += tr("设置为");
	strInfo += strTmp;
	strInfo += tr("状态");

    	m_pSecurityAbnormalDlg->ui->label_title->setText(strInfo);

	m_securityState = state;

	g_pScaleSrc->Backup_Config_File();
}

/************************************************************
描述：显示当前设防状态开始时的日期时间
参数：无
返回：无
************************************************************/
/*void SecurityMainDialog::showCurrentTime()
{
	////QDateTime dateTime = QDateTime::currentDateTime();

	////int y = dateTime.date().year();
	////int m = dateTime.date().month();
	////int d = dateTime.date().day();

	////QString strTime = dateTime.time().toString();
	////ui->labelDateTime->setText(QString::number(y) + tr("年")
	////	 + QString::number(m) + tr("月")
	////	 + QString::number(d) + tr("日") + " " + strTime);
}*/

/************************************************************
描述：在报警记录列表控件中加入一条报警信息
参数：security_file_name - 待加入的报警信息的文件名
返回：0:成功
	 -1:失败
************************************************************/
int SecurityMainDialog::securityListAddOne( char *security_file_name)
{
	FILE * fp;
	int i;
	char filepath[PATH_MAX];
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewSecurityList->model();
	QStandardItem *item;
	int rowNum;
	int year, month, day, hour, minute, second, /*msecond,*/defensenuber;
	char buf[300], *ptr;
	char s_type[100],s_number[100],s_name[100],s_location[100];
	char s_unread[2];
	char str[80];

	sprintf(filepath, "/mnt/disk/sec_info/%s", security_file_name);
	fp = fopen(filepath,"rt");
	if(!fp)
		return -1;

	QFont font("Century Gothic");
	//font.setPixelSize(18);
	rowNum = model->rowCount();
	sscanf(security_file_name, "%04d%02d%02d-%02d%02d%02d-%03d", &year, &month, &day,
           &hour, &minute, &second, &defensenuber/*&msecond*/);//david 2014.4.10
	sprintf(str, "%d-%d-%d %d:%02d", year, month, day, hour, minute);
	item = new QStandardItem(str);
	//font.setPixelSize(18);
	item->setTextAlignment(Qt::AlignCenter);
	//item->setFont(font);
	model->setItem(rowNum, 0, item);

	fgets(buf, sizeof(buf), fp);
	s_type[0] = 0;
	if (strncmp(buf,"DEFENSE_TYPE:",strlen("DEFENSE_TYPE:")) == 0)
	{
		ptr = buf + strlen("DEFENSE_TYPE:");
		i = 0;
		while (*ptr && (*ptr != '\n')&& (*ptr != '\r')) s_type[i++] = *ptr++;
		s_type[i] = 0;
	}

	fgets(buf, sizeof(buf), fp);
	s_number[0] = 0;
	if (strncmp(buf,"DEFENSE_NUMBER:", strlen("DEFENSE_NUNBER:")) == 0)
	{
		ptr = buf + strlen("DEFENSE_NUMBER:");
		i = 0;
		while (*ptr && (*ptr != '\n')&& (*ptr != '\r')) s_number[i++] = *ptr++;
		s_number[i] = 0;
	}

	fgets(buf, sizeof(buf), fp);
	s_name[0] = 0;
	if(strncmp(buf, "DEFENSE_NAME:", strlen("DEFENSE_NAME:")) == 0)
	{
		ptr = buf + strlen("DEFENSE_NAME:");
		i = 0;
		while (*ptr && (*ptr != '\n')&& (*ptr != '\r')) s_name[i++] = *ptr++;
		s_name[i] = 0;
	}

	fgets(buf, sizeof(buf), fp);
	s_location[0] = 0;
	if (strncmp(buf, "DEFENSE_LOCATION:", strlen("DEFENSE_LOCATION:")) == 0)
	{
		ptr = buf + strlen("DEFENSE_LOCATION:");
		i = 0;
		while (*ptr && (*ptr != '\n')&& (*ptr != '\r')) s_location[i++] = *ptr++;
		s_location[i] = 0;
	}

	fgets(buf, sizeof(buf), fp);
	if (strncmp(buf, "UNREAD:", strlen("UNREAD:")) == 0)
	{
		ptr = buf + strlen("UNREAD:");

		s_unread[0] = *ptr;
		s_unread[1] = 0;
	}
	else
	{
		s_unread[0] = '0';
		s_unread[1] = 0;
	}

	fclose(fp);

	item = new QStandardItem(QString::fromUtf8(s_number));// + QObject::tr(g_strDefenseType[atoi(s_type)]));
	if (strcmp(s_type, "0") == 0)
	{
		item->setIcon(QIcon(":/images/security/wire.png")); //s_type
	}
	else
	{
		item->setIcon(QIcon(":/images/security/wireless.png")); //s_type
	}
	font.setPixelSize(22);
	item->setFont(font);
	//item->setTextAlignment(Qt::AlignCenter);
	model->setItem(rowNum, 1, item);

	item = new QStandardItem(QObject::tr(g_strType[atoi(s_name)]));
	item->setTextAlignment(Qt::AlignCenter);
	model->setItem(rowNum, 2, item);

	item = new QStandardItem(QString::fromUtf8(s_location));
	item->setTextAlignment(Qt::AlignCenter);
	model->setItem(rowNum, 3, item);

	QString strHaveVideo;
	QString strFilePath;
	strFilePath.sprintf("/mnt/disk/sec_media/%04d%02d%02d-%02d%02d%02d-%03d.avi", year, month, day,
        hour, minute, second,defensenuber /*msecond*/);//david 2014.4.10
	QFile avi_file(strFilePath);
	if (avi_file.open(QIODevice::ReadOnly))
	{
		avi_file.close();
		strHaveVideo = "yes";
	}
	else
	{
		strHaveVideo = "no";
	}
	item = new QStandardItem(strHaveVideo);
	item->setTextAlignment(Qt::AlignCenter);
	model->setItem(rowNum, 4, item);

	sprintf(buf, "%02d-%03d", second, defensenuber/*msecond*/);//david 2014.4.10
	item = new QStandardItem(QString::fromUtf8(buf));
	item->setTextAlignment(Qt::AlignCenter);
	model->setItem(rowNum, 5, item);

	item = new QStandardItem(s_type);
	item->setTextAlignment(Qt::AlignCenter);
	model->setItem(rowNum, 6, item);

	item = new QStandardItem(security_file_name);
	item->setTextAlignment(Qt::AlignCenter);
	model->setItem(rowNum, 7, item);

	item = new QStandardItem(s_unread);
	item->setTextAlignment(Qt::AlignCenter);
	model->setItem(rowNum, 8, item);

	if (s_unread[0] == '1')
	{
		model->item(rowNum, 0)->setIcon(QIcon(":/images/infoman_dialog/unread.png"));
	}

	ui->tableViewSecurityList->setRowHeight(rowNum, g_pScaleSrc->get_public_tableviewlist_row_height());

	return 0;
}

/************************************************************
描述：搜索/mnt/disk/sec_info目录下的.txt的报警信息文件，把找到的报
	 警信息全部加入报警记录列表控件，将这些记录按从晚到早的顺序排序
参数：无
返回：
************************************************************/
void SecurityMainDialog::refresh_list_containt()
{
	DIR *dp;
	struct dirent *dirp;
	int len;
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewSecurityList->model();

	model->removeRows(0, model->rowCount());			//  清空数据

	if((dp = opendir("/mnt/disk/sec_info")) == NULL)
	{
		printf ( "erro open the sec_info error %s:%d\n",__FILE__,__LINE__ );
		return;
	}

	while((dirp = readdir(dp)) != NULL)
	{
		if(dirp->d_type == DT_DIR)
		{
			continue;
		}
		len = strlen(dirp->d_name);
		if (strcasecmp(&dirp->d_name[len - 4], ".txt") != 0)
		{
			continue;
		}

		securityListAddOne(dirp->d_name);				// 将读取到的记录数全部的导入到listview控件中
	}
	closedir(dp);

	ui->tableViewSecurityList->sortByColumn(7, Qt::DescendingOrder);  	//按照时间进行升序排列


}

/************************************************************
描述：搜索/mnt/disk/sec_info目录下的.txt的报警信息文件，把找到的报
	 警信息全部加入报警记录列表控件，将这些记录按从晚到早的顺序排序
参数：无
返回：0:成功
	 -1:失败
************************************************************/
int SecurityMainDialog::reloadSecurityList()
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewSecurityList->model();
	//QString strDateTime, strType, strNumber, strName, strLocation;
	int /*rowSel,*/ rowSelNew;

	model->setHeaderData(0, Qt::Horizontal, tr("时间"));
	model->setHeaderData(1, Qt::Horizontal, tr("防区号码"));
	model->setHeaderData(2, Qt::Horizontal, tr("防区名称"));
	model->setHeaderData(3, Qt::Horizontal, tr("安装位置"));
	model->setHeaderData(4, Qt::Horizontal, tr("视频"));
	model->setHeaderData(5, Qt::Horizontal, tr("秒"));
	model->setHeaderData(6, Qt::Horizontal, tr("防区类型"));
	model->setHeaderData(7, Qt::Horizontal, tr("时间排序"));
	model->setHeaderData(8, Qt::Horizontal, "未读");

	refresh_list_containt();						// 根据txt文件，导入list中

	if (model->rowCount() > SECURITY_RECORD_COUNT)				// 如果条目数超过预期
	{
		int rowCnt = model->rowCount();
		for (int i = SECURITY_RECORD_COUNT; i < rowCnt; i++)
		{
			delSecurityRecord(SECURITY_RECORD_COUNT);		// 删除最旧的多余的记录条目
		}
	}

	rowSelNew = 0;

	ui->tableViewSecurityList->selectRow(rowSelNew);

	return 0;
}

/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void SecurityMainDialog::showEvent(QShowEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)parent();
	//pMainDlg->m_nShowSubDlgCnt++;

    pMainDlg->showRefreshTabFocus(this);

    if (!m_bRecording) //lg 12.22 !m_pSecurityAlertDlg->isVisible())
	{
		if (m_newDefenseCnt > 0)
		{
			m_newDefenseCnt = 0;

			g_pMainDlg->emitSigSetNotify(1);
		}

		QSettings settings_config(CONFIG_FILE, QSettings::IniFormat);
		settings_config.setValue("info/new_defense_cnt", m_newDefenseCnt);

		//((MainDialog *)parent())->getUi()->labelIconDefenseCnt->setText("\n\n" + QString::number(m_newDefenseCnt));
    }
	else
	{
        m_bSecurityMainDlgAloneOpened = false;

		setAllButtonsFocusPolicy(this, Qt::NoFocus);
	}
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void SecurityMainDialog::hideEvent(QHideEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)parent();
	//david 2014/7/9
	QSettings settings_config(CONFIG_FILE, QSettings::IniFormat);
	settings_config.setValue("info/new_defense_cnt",m_newDefenseCnt);

    m_bRecording = false;
    m_bSecurityMainDlgAloneOpened = false;//false;

	//m_pSecurityAlertDlg->hide();
	m_pSecurityAbnormalDlg->hide();
	m_pSecurityPlayerDlg->hide();
	m_pSecurityPasswordDlg->hide();

	pMainDlg->hideRefreshTabFocus(this);

	if (pMainDlg->m_pCurActiveDlg == pMainDlg)
	{
		pMainDlg->ui->pushButtonSecurityMan->setFocus(Qt::TabFocusReason);
	}
}

/************************************************************
描述：关闭本窗口
参数：无
返回：无
************************************************************/
void SecurityMainDialog::on_pushButtonSecMainReturn_pressed()
{
	done(0);
}

/************************************************************
描述：查看报警记录的详细信息，如果该记录有录像则开始播放报警录像
参数：无
返回：无
************************************************************/
void SecurityMainDialog::on_pushButtonSecMainDetail_pressed()
{
	if (m_pSecurityPlayerDlg->isVisible()) return; //bug

	QStandardItemModel *model =(QStandardItemModel *)ui->tableViewSecurityList->model();
	int rowSel = ui->tableViewSecurityList->currentIndex().row();
	if (rowSel < 0) return;

	int type;
	QString strNumber;

	type = model->item(rowSel, 6)->text().toInt();
	strNumber = model->item(rowSel, 1)->text();

	char filepath[PATH_MAX];

	if (model->item(rowSel, 8)->text() == "1")
	{
		QString strTime, strTimeSec;
		int year, month, day, hour, minute, second,/* msecond,*/defensenumber;

		strTime = model->item(rowSel, 0)->text();
		strTimeSec = model->item(rowSel, 5)->text();
		sscanf(strTime.toUtf8().data(), "%d-%d-%d %d:%02d", &year, &month, &day,
				&hour, &minute);
		sscanf(strTimeSec.toUtf8().data(), "%02d-%03d", &second,&defensenumber/* &msecond*/);//david 2014.4.10
		sprintf(filepath, "/mnt/disk/sec_info/%04d%02d%02d-%02d%02d%02d-%03d.txt", year, month, day,
				hour, minute, second, defensenumber/*msecond*/);

		char buf[512];
		int len;
		FILE *fp;
		char *ptr;
		fp = fopen(filepath, "rb+");
		if (!fp) return;

		len = fread(buf, 1, sizeof(buf), fp);
		buf[len] = 0;
		ptr = strstr(buf, "\nUNREAD:");
		if (ptr)
		{
			ptr += strlen("\nUNREAD:");
			*ptr = '0';
		}

		fseek(fp, 0, SEEK_SET);
		fwrite(buf, 1, len, fp);
		fclose(fp);

		model->item(rowSel, 0)->setIcon(QIcon());

		model->item(rowSel, 8)->setText("0");
	}

	if (model->item(rowSel, 4)->text() != "yes")
	{
		filepath[0] = '\0';
	}
	else
	{
		QString strTime, strTimeSec;
		int year, month, day, hour, minute, second,/* msecond,*/defensenumber;

		strTime = model->item(rowSel, 0)->text();
		strTimeSec = model->item(rowSel, 5)->text();
		sscanf(strTime.toUtf8().data(), "%d-%d-%d %d:%02d", &year, &month, &day,
				&hour, &minute);
		sscanf(strTimeSec.toUtf8().data(), "%02d-%03d", &second, &defensenumber/*&msecond*/);
		//david 2014.4.10
		sprintf(filepath, "/mnt/disk/sec_media/%04d%02d%02d-%02d%02d%02d-%03d.avi", year, month, day,
				hour, minute, second, defensenumber/*msecond*/);//david 2014.4.10
	}

	m_pSecurityPlayerDlg->setDetail(filepath, strNumber + tr("号") + QObject::tr(g_strDefenseType[type]));
	m_pSecurityPlayerDlg->show();
	m_pSecurityPlayerDlg->repaint();
	m_pSecurityPlayerDlg->startPlayer();
	m_pSecurityPlayerDlg->show();
}

/************************************************************
描述：删除一条报警记录
参数：row:待删除的报警记录在列表控件中的行号
返回：无
注意：下标从0开始
************************************************************/
void SecurityMainDialog::delSecurityRecord(int row)
{
	QString strTime;
	QString strTimeSec;
	char filepath[PATH_MAX];
	int year, month, day, hour, minute, second,/*msecond,*/defensenumber;
	QStandardItemModel *model;
	model = (QStandardItemModel *)ui->tableViewSecurityList->model();

//	for (int i=0;i<model->rowCount();i++ )
//	{
//		printf ( "kevin  %d %s\n",i,model->item(i, 0)->text().toLatin1().data() );
//	}
//	printf ( "kevin  --- %d\n",row );

	strTime = model->item(row, 0)->text();
	strTimeSec = model->item(row, 5)->text();
	sscanf(strTime.toUtf8().data(), "%d-%d-%d %d:%02d", &year, &month, &day,
		   &hour, &minute);
	sscanf(strTimeSec.toUtf8().data(), "%02d-%03d", &second, &defensenumber/*&msecond*/);//david 2014.4.10
	sprintf(filepath, "/mnt/disk/sec_info/%04d%02d%02d-%02d%02d%02d-%03d.txt", year, month, day,
            hour, minute, second,defensenumber/* msecond*/);//david 2014.4.10
	remove(filepath);

	sprintf(filepath, "/mnt/disk/sec_media/%04d%02d%02d-%02d%02d%02d-%03d.avi", year, month, day,
            hour, minute, second, defensenumber/* msecond*/);//david 2014.4.10
	remove(filepath);

	model->removeRow(row);
}

/************************************************************
描述：删除最旧的AVI文件,但是报警记录保持
参数：无
返回：无
************************************************************/
void SecurityMainDialog::dellastsecurityavifile()
{
	QString strTime;
	QString strTimeSec;
	char filepath[PATH_MAX];
	int year, month, day, hour, minute, second,/*msecond,*/defensenumber;
	QStandardItemModel *model;
	model = (QStandardItemModel *)ui->tableViewSecurityList->model();
	int row =  model->rowCount();
	if ( row == 0 ) return;
	row = row - 1;

AGAIN:
	strTime = model->item(row, 0)->text();
	strTimeSec = model->item(row, 5)->text();
	sscanf(strTime.toUtf8().data(), "%d-%d-%d %d:%02d", &year, &month, &day,
		   &hour, &minute);
	sscanf(strTimeSec.toUtf8().data(), "%02d-%03d", &second, &defensenumber/*&msecond*/);//david 2014.4.10

	sprintf(filepath, "/mnt/disk/sec_media/%04d%02d%02d-%02d%02d%02d-%03d.avi", year, month, day,
            hour, minute, second, defensenumber/* msecond*/);				    //david 2014.4.10

    	if((access(filepath,F_OK)) == -1)						    // 没有这个AVI文件，就继续往后查找
	{
		if ( row == 0 )								    // 没有AVI数据了
			return;
		row --;
		goto AGAIN;
	}

	remove(filepath);

	QStandardItem *item = new QStandardItem("no");
	item->setTextAlignment(Qt::AlignCenter);
	model->setItem(row, 4, item);
}

/************************************************************
描述：删除当前选择的报警记录
参数：无
返回：无
************************************************************/
void SecurityMainDialog::on_pushButtonSecMainDel_pressed()
{
	int rowSel;

	rowSel = ui->tableViewSecurityList->currentIndex().row();
	if (rowSel < 0) return;

	setAllButtonsEnabled(this, false);
	ui->tableViewSecurityList->setFocus(Qt::TabFocusReason);
	MyMessageBox messBox(this);
	CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
	CommonPushButton *cancelButton = messBox.addButton(QObject::tr("取消"), "pushButtonMyMessageBoxCancel");
	Q_UNUSED(okButton);
	cancelButton->setFocus(Qt::TabFocusReason);
	messBox.setText(tr("确定删除?"));
	messBox.exec();
	setAllButtonsEnabled(this, true);
	ui->pushButtonSecMainDel->setFocus(Qt::TabFocusReason);
	if(messBox.clickedButton() == cancelButton)
	{
		return;
	}

	rowSel = ui->tableViewSecurityList->currentIndex().row();
	delSecurityRecord(rowSel);

	////setTableViewSecurityListColor(TABLE_VIEW_COLOR);
}

/************************************************************
描述：设置报警记录列表控件的背景色为按行号白-非白显示
参数：color - 非白行的背景色
返回：无
************************************************************/
/*void SecurityMainDialog::setTableViewSecurityListColor(QColor color)
{
	int i, j;
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewSecurityList->model();

	for (i = 0; i < model->rowCount(); i++)
	{
		//printf("\t%d", i);
		QColor col;
		if (i % 2 == 1)
		{
			col = color;
		}
		else
		{
			col = QColor(255, 255, 255);
		}

		for (j = 0; j < model->columnCount(); j++)
		{
			QStandardItem *item = model->item(i, j);
			if (item)
			{
				item->setBackground(QBrush(col));
			}
		}
	}
}*/

/************************************************************
描述：切换设防状态到外出设防状态，如果有异常防区则打开设防状态异常防区
	 忽略撤销窗口
参数：无
返回：无
************************************************************/
/*void SecurityMainDialog::on_pushButtonGoOut_pressed()
{
	if (m_isSecurityStateShifting) return;
	m_isSecurityStateShifting = 1;

	int i ;
	bool bAbnormal = false;

	showCurrentTime();

	if (m_securityState == STATUS_GOOUT)
	{
		m_isSecurityStateShifting = 0;
		return;
	}

	m_goingStatus = STATUS_GOOUT;
	memset(m_allDefenseStatus, 0, sizeof(m_allDefenseStatus));
	memset(m_regAbnormalDefense, 0, sizeof(m_regAbnormalDefense));
	getAllDefenseStatus();
	getRegisteredDefenseProperty();

	for (i = 0; i < m_wireDefenseCnt; i++)
	{
		if (m_allDefenseStatus[i] == 0)
		{
			if ((m_allDefenseProperty[i] == PROPERTY_NORMAL) || (m_allDefenseProperty[i] == PROPERTY_GOOUT)
				|| (m_allDefenseProperty[i] == PROPERTY_ATHOME))
			{
				bAbnormal = true;
				m_regAbnormalDefense[i] = 1;
			}
		}
	}

	//printf("\nlg m_wirelessDefenseCnt:%d\n", m_wirelessDefenseCnt);

	for (i = 0; i < m_wirelessDefenseCnt; i++)
	{
		if (m_allDefenseStatus[i + 56] == 1)
		{
			if ((m_allDefenseProperty[i + 56] == PROPERTY_NORMAL) || (m_allDefenseProperty[i + 56] == PROPERTY_GOOUT)
				|| (m_allDefenseProperty[i + 56] == PROPERTY_ATHOME))
			{
				bAbnormal = true;
				m_regAbnormalDefense[i + 56] = 1;
			}
		}
	}

	MainDialog *pMainDlg = (MainDialog *)parent();

	if (!bAbnormal)
	{
        //david
		m_pSecurityAlertDlg->setStateText();
        //david

		setSecurityState(STATUS_GOOUT);
		//添加进入外出设防模式的声音

		if (pMainDlg->m_inputMethodMode == 0)
		{
			playWaveFile((char *)"wav/goout-en.wav", pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
		}
		else
		{
			playWaveFile((char *)"wav/goout.wav", pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
		}
	}
	else
	{
		int ret = RET_CANCEL;

		SecurityAbnormalDialog secAbDlg(this);
		secAbDlg.raise();
		ret = secAbDlg.exec();

		if ((ret == RET_IGNORE) || (ret == RET_AUTOIGNORE)) //加上自动忽略
		{
            //david
			m_pSecurityAlertDlg->setStateText();
            //david

			setSecurityState(STATUS_GOOUT);

			if (pMainDlg->m_inputMethodMode == 0)
			{
				playWaveFile((char *)"wav/goout-en.wav", pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
			}
			else
			{
				playWaveFile((char *)"wav/goout.wav", pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
			}
		}
	}

	m_isSecurityStateShifting = 0;
}
*/
/************************************************************
描述：切换设防状态到居家设防状态，若原状态为外出设防状态则需要输入密码，
	 如果有异常防区则打开设防状态异常防区忽略撤销窗口
参数：无
返回：无
************************************************************/
/*void SecurityMainDialog::on_pushButtonSecAlertHome_pressed()
{
	if (m_isSecurityStateShifting) return;
	m_isSecurityStateShifting = 1;

	int i ;
	bool bAbnormal = false;

	showCurrentTime();

	if (m_securityState == STATUS_ATHOME)
	{
		m_isSecurityStateShifting = 0;
		return;
	}

	m_goingStatus = STATUS_ATHOME;
	memset(m_allDefenseStatus, 0, sizeof(m_allDefenseStatus));
	memset(m_regAbnormalDefense, 0, sizeof(m_regAbnormalDefense));
	getAllDefenseStatus();
	getRegisteredDefenseProperty(); //property[16]

	for (i = 0; i < m_wireDefenseCnt; i++)
	{
		if (m_allDefenseStatus[i] == 0)
		{
			if ((m_allDefenseProperty[i] == PROPERTY_NORMAL) || (m_allDefenseProperty[i] == PROPERTY_GOOUT))
			{
				bAbnormal = true;
				m_regAbnormalDefense[i] = 1;
			}
		}
	}

	for (i = 0; i < m_wirelessDefenseCnt; i++)
	{
		if (m_allDefenseStatus[i + 56] == 1)
		{
			if ((m_allDefenseProperty[i + 56] == PROPERTY_NORMAL) || (m_allDefenseProperty[i + 56] == PROPERTY_GOOUT))
			{
				bAbnormal = true;
				m_regAbnormalDefense[i + 56] = 1;
			}
		}
	}

	MainDialog *pMainDlg = (MainDialog *)parent();

	if (!bAbnormal)
	{
		if (m_securityState == STATUS_DISARM) //0 表示撤防状态 1 表示居家状态 2 表示外出状态
		{
			setSecurityState(STATUS_ATHOME);
			//添加进入居家设防模式的声音
			if (pMainDlg->m_inputMethodMode == 0)
			{
				playWaveFile((char *)"wav/athome-en.wav", pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
			}
			else
			{
				playWaveFile((char *)"wav/athome.wav", pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
			}
		}
		else if (m_securityState == STATUS_GOOUT)
		{
			int ret;

			SecurityPasswordDialog spwDlg(this);
			ret = spwDlg.exec();

			//printf("status_goout:ret %d\n", ret);

			if (ret == RET_PASSWORDOK)
			{
				setSecurityState(STATUS_ATHOME);
				//添加进入居家设防模式的声音
				if (pMainDlg->m_inputMethodMode == 0)
				{
					playWaveFile((char *)"wav/athome-en.wav", pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
				}
				else
				{
					playWaveFile((char *)"wav/athome.wav", pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
				}
			}
		}
	}
	else
	{
		if (m_securityState == STATUS_DISARM)
		{
			int ret = RET_CANCEL;

			SecurityAbnormalDialog secAbDlg(this);
			secAbDlg.raise();
			ret = secAbDlg.exec();

			if ((ret == RET_IGNORE) || (ret == RET_AUTOIGNORE)) //加上自动忽略
			{
				setSecurityState(STATUS_ATHOME);
				if (pMainDlg->m_inputMethodMode == 0)
				{
					playWaveFile((char *)"wav/athome-en.wav", pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
				}
				else
				{
					playWaveFile((char *)"wav/athome.wav", pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
				}
			}
		}
		else if (m_securityState == STATUS_GOOUT)//0 表示撤防状态 1 表示居家状态 2 表示外出状态
		{
			int ret;

			SecurityPasswordDialog spwDlg(this);
			ret = spwDlg.exec();

			if (ret == RET_PASSWORDOK)
			{
				ret = RET_CANCEL;
				SecurityAbnormalDialog secAbDlg(this);
				secAbDlg.raise();
				ret = secAbDlg.exec();

				if ((ret == RET_IGNORE) || (ret == RET_AUTOIGNORE)) //加上自动忽略
				{
					setSecurityState(STATUS_ATHOME);
					if (pMainDlg->m_inputMethodMode == 0)
					{
						playWaveFile((char *)"wav/athome-en.wav", pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
					}
					else
					{
						playWaveFile((char *)"wav/athome.wav", pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
					}
				}
			}
		}
	}

	m_isSecurityStateShifting = 0;
}
*/
/************************************************************
描述：切换设防状态到撤防状态，需要输入密码，如果有异常防区则打开设防状
	 态异常防区忽略撤销窗口
参数：无
返回：无
************************************************************/
/*void SecurityMainDialog::on_pushButtonSecAlertDisArm_pressed()
{
	if (m_isSecurityStateShifting) return;
	m_isSecurityStateShifting = 1;

	int i ;
	bool bAbnormal = false;

	showCurrentTime();

	if (m_securityState == STATUS_DISARM)
	{
		m_isSecurityStateShifting = 0;
		return;
	}

	m_goingStatus = STATUS_DISARM;
	memset(m_allDefenseStatus, 0, sizeof(m_allDefenseStatus));
	memset(m_regAbnormalDefense, 0, sizeof(m_regAbnormalDefense));
	getAllDefenseStatus();
	getRegisteredDefenseProperty(); //property[16]

	for (i = 0; i < m_wireDefenseCnt; i++)
	{
		printf("lg %d %d\n", i, m_allDefenseProperty[i]);
		if (m_allDefenseStatus[i] == 0)		int ret;

		SecurityPasswordDialog spwDlg(this);
		ret = spwDlg.exec();

		if (ret == RET_PASSWORDOK)
		{
			if (m_allDefenseProperty[i] == PROPERTY_NORMAL)
			{
				bAbnormal = true;
				m_regAbnormalDefense[i] = 1;
			}
		}
	}

	for (i = 0; i < m_wirelessDefenseCnt; i++)
	{
		if (m_allDefenseStatus[i + 56] == 1)
		{
			if (m_allDefenseProperty[i + 56] == PROPERTY_NORMAL)
			{
				bAbnormal = true;
				m_regAbnormalDefense[i + 56] = 1;
			}
		}
	}

	MainDialog *pMainDlg = (MainDialog *)parent();

	if (!bAbnormal)
	{
		int ret;

		SecurityPasswordDialog spwDlg(this);
		ret = spwDlg.exec();

		if (ret == RET_PASSWORDOK)
		{
			setSecurityState(STATUS_DISARM);
			//添加进入居家设防模式的声音
			if (pMainDlg->m_inputMethodMode == 0)
			{
				playWaveFile((char *)"wav/disarm-en.wav", pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
			}
			else
			{
				playWaveFile((char *)"wav/disarm.wav", pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
			}
		}
	}
	else
	{
		int ret;

		SecurityPasswordDialog spwDlg(this);
		ret = spwDlg.exec();

		if (ret == RET_PASSWORDOK)
		{
			ret = RET_CANCEL;

			SecurityAbnormalDialog secAbDlg(this);
			secAbDlg.raise();
			ret = secAbDlg.exec();

			if ((ret == RET_IGNORE) || (ret == RET_AUTOIGNORE)) //加上自动忽略
			{
				setSecurityState(STATUS_DISARM);
				if (pMainDlg->m_inputMethodMode == 0)
				{
					playWaveFile((char *)"wav/disarm-en.wav", pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
				}
				else
				{
					playWaveFile((char *)"wav/disarm.wav", pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
				}
			}
		}
	}

	m_isSecurityStateShifting = 0;
}
*/
/************************************************************
描述：删除全部报警记录、报警信息文件和所有AVI报警录像文件
参数：无
返回：无
************************************************************/
void SecurityMainDialog::on_pushButtonSecMainDelAll_pressed()
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewSecurityList->model();
	if (model->rowCount() == 0) return;

	setAllButtonsEnabled(this, false);
	ui->tableViewSecurityList->setFocus(Qt::TabFocusReason);
	MyMessageBox messBox(this);
	CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
	CommonPushButton *cancelButton = messBox.addButton(QObject::tr("取消"), "pushButtonMyMessageBoxCancel");
	Q_UNUSED(okButton);
	cancelButton->setFocus(Qt::TabFocusReason);
	messBox.setText(tr("确定删除所有记录?"));
	messBox.exec();
	setAllButtonsEnabled(this, true);
	ui->pushButtonSecMainDelAll->setFocus(Qt::TabFocusReason);
	if(messBox.clickedButton() == cancelButton)
	{
		return;
	}

	model->removeRows(0, model->rowCount());

	system("/bin/rm /mnt/disk/sec_info/*.txt");
	system("/bin/rm /mnt/disk/sec_media/*.avi");
}

/************************************************************
描述：获取所有已设防区的类型
参数：无
返回：无
************************************************************/
/*void SecurityMainDialog::getRegisteredDefenseProperty()
{
	int i ;
	memset(m_allDefenseProperty, 0, sizeof(m_allDefenseProperty));

	QSettings settings("/mnt/disk/config/defense.conf", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF8"));

	for (i = 0; i < m_wireDefenseCnt; i++)
	{
		m_allDefenseProperty[i] = settings.value("wiredefense" + QString::number(m_allDefenseNumber[i]) + "/property", QString::number(PROPERTY_GOOUT)).toInt();
	}

	for (i = 0; i < m_wirelessDefenseCnt; i++)
	{
		m_allDefenseProperty[i + 56] = settings.value("wirelessdefense" + QString::number(m_allDefenseNumber[i + 56]) + "/property", QString::number(PROPERTY_GOOUT)).toInt();
	}
}
*/

/************************************************************
描述：获取所有已设防区的状态
参数：无
返回：无
************************************************************/
/*void SecurityMainDialog::getAllDefenseStatus()
{
	int i ;
	memset(m_allDefenseStatus, 0, sizeof(m_allDefenseStatus));

	QSettings settings("/mnt/disk/config/defense", QSettings::IniFormat);
	for(i = 0; i < m_wireDefenseCnt; i++)
	{
		int tmp;
		tmp = settings.value("wiredefense/defense" + QString::number(m_allDefenseNumber[i]), "1").toInt();
		m_allDefenseStatus[i] = tmp;
	}

	for (i = 0; i < m_wirelessDefenseCnt; i++)
	{
		int tmp;
		tmp = settings.value("wirelessdefense/defense" + QString::number(m_allDefenseNumber[i + 56]), "0").toInt();
		m_allDefenseStatus[i + 56] = tmp;
	}
}
*/

/************************************************************
描述：按动报警记录列表控件排序指示符的回调函数
参数：logicalIndex - 排序的列号
	 order - Qt::DescendingOrder 按降序排序
			 Qt::AscendingOrder 按升序排序
返回：无
************************************************************/
/*void SecurityMainDialog::tvSecurityListSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
	Q_UNUSED(logicalIndex);
	Q_UNUSED(order);

	setTableViewSecurityListColor(TABLE_VIEW_COLOR);
}
*/
/*void SecurityMainDialog::on_pushButton_pressed()
{
	show();
	raise();
	m_pSerial1Thread->alertRecord(0xa0, 2, 1, QDateTime::currentDateTime());

	return;
	QTcpSocket *tcpSocket1 = new QTcpSocket;//pDlg->m_pTermHelpMainDlg);
	//求助信息包头
	tcpSocket1->connectToHost("192.168.1.238", 17701);//管理中心计算机
	if (tcpSocket1->waitForConnected())
	{
		tcpSocket1->write("abc", 4);//block);
		tcpSocket1->waitForBytesWritten();
		tcpSocket1->close();
	}

	delete tcpSocket1;
}
*/

/************************************************************
描述：关闭其它非对讲窗口并开始报警视频录像(对讲优先于报警录像)
参数：strDateTime - 报警时间
	 number - 防区号码
	 property - 防区属性
	 channel - 防区摄像头通道号(0-3)
	 name - 防区名称
	 type - 防区类型(有线/无线)
	 strLocation - 防区安装位置
	 strAviPath - 报警AVI录像文件路径
返回：无
************************************************************/
void SecurityMainDialog::startRecorder(int num,int idx, QString strDateTime, int number, int property, int channel, int name,
									   int type, QString strLocation, QString strAviPath)
{

	MainDialog *pMainDlg = (MainDialog *)parent();

	printf("startRecorder\n");
	printf("\tlg %d %s\n", number, strDateTime.toLocal8Bit().data());
    m_pSerial1Thread->SendAlarmInfo(num);

	QFont font("Century Gothic");
	font.setPixelSize(22);
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewSecurityList->model();
	int i;
	for (i = 0; i < model->rowCount(); i++)
	{
		QString str = model->item(i, 6)->text();
		QString str2;
		if (str.left(1) != "&")
		{
			break;
		}

		QStandardItem *item = model->item(i, 1);
		item->setText(str.mid(3));
		item->setFont(font);
		item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

		str2 = str.mid(1, 1);
		model->item(i, 6)->setText(str2);

		if (str2 == "0")
		{
            item->setIcon(QIcon(":/images/security/wire.png")); //s_type
		}
		else
		{
            item->setIcon(QIcon(":/images/security/wireless.png")); //s_type
		}
	}

	if (m_pSecurityPasswordDlg->isVisible()
			&& (m_pSecurityPasswordDlg->m_flag == 1))
	{
		return;
	}

	if (g_bIstOrAlertBusy == 2) return;

	bool bStopRecoder = false;

	//lg bug
	if ((g_bIstOrAlertBusy == 0) && m_pSecurityAlertDlg->isVisible())
    {
        bStopRecoder = true;

        if(m_pSecurityAlertDlg->doorbell_power_on_flag&&g_door_bell_type==2)
        {
            g_pScaleSrc->doorbell_power(false);
            m_pSecurityAlertDlg->doorbell_power_on_flag = false;
        }

        m_pSecurityAlertDlg->stopRecorder(false);
		if (!pMainDlg->m_pVoipMainDlg->isVisible())
        {
			my_ring_stop();
		}

		if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
			usleep(50000);;
		playWaveFile((char *)"wav/alert.wav", pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
	}
	//lg bug

	if((eXosipElement->state == ICT_CALLING)
			||(eXosipElement->state == ICT_RINGBACK) || (eXosipElement->state == ICT_PREREC))  // 如果是留言状态就退出
	{
        	g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->on_pushButtonIctReturn_pressed();
	}

	if((eXosipElement->state == ICT_TALKING)  )
	{
		g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->m_pRecvHookDlg->on_pushButtonIctRcvHookHangUp_pressed();
	}

	printf("startRecorder 2 state:%d\n",eXosipElement->state);

	if(eXosipElement->state == ICT_RINGBACK || eXosipElement->state == ICT_PREREC )  // 如果是呼叫中心时，有报警也退出
	{
		g_pMainDlg->m_pCallCenterDlg->on_pushButtonCallCenterReturn_pressed();
	}

	if((eXosipElement->state == IST_IDLE)
			|| (eXosipElement->state == IST_RING)
			|| (eXosipElement->state == IST_PRETALKING)
			|| (eXosipElement->state == IST_TALKING))
	{
		g_pMainDlg->m_pVoipMainDlg->m_pIstDlg->on_pushButtonIstHangUp_pressed();
        if(eXosipElement->state == IST_IDLE)
                   usleep(800000);
	}

	while(eXosipElement->state != IDLE)
	{
		qApp->processEvents();
		lpc_cmd_terminate(lphone,NULL);
		usleep(800000);
	}

	while(g_iIST_state)
	{
		qApp->processEvents();
		usleep(80000);
	}

	int loop_cnt = 0;
	while (g_bIstOrAlertBusy)
	{
		usleep(5000);

		if (g_bIstOrAlertBusy == 2) return;

		if (loop_cnt++ >= 8) //40 ms
		{
			qApp->processEvents();
			loop_cnt = 0;
		}
	}

	g_bIstOrAlertBusy = 2;

	if (eXosip.callflag == 0)
	{
		if ((eXosipElement->videostream != NULL) || (eXosipElement->audiostream != NULL))
		{
			g_bIstOrAlertBusy = 0;
			return;
		}
	}
	else
	{
		int i;
		for (i = 0; i < MAX_EXTENSION_MACHINE_CNT; i++) {
			if ((eXosip.remip[i].videostream != NULL)
					|| (eXosip.remip[i].audiostream != NULL))
			{
				g_bIstOrAlertBusy = 0;
				return;
			}
		}
	}

	if (pMainDlg->m_pCalibrationDlg->isVisible())
	{
		g_bIstOrAlertBusy = 0;
		return;
	}

	if (g_doorbell_on)//pMainDlg->m_pDoorBellDlg->isVisible())
	{
		g_bIstOrAlertBusy = 0;
        //return;
	}

	if ( pMainDlg->m_pSysSetMainDlg->m_pRingAndVolumeDlg->m_pDoorBellRingFileSetDlg->isVisible() )
		pMainDlg->m_pSysSetMainDlg->m_pRingAndVolumeDlg->m_pDoorBellRingFileSetDlg->Remote_Call_Return();

	if (!bStopRecoder && m_pSecurityAlertDlg->isVisible())
    {
        if(m_pSecurityAlertDlg->doorbell_power_on_flag&&g_door_bell_type==2)
        {
            g_pScaleSrc->doorbell_power(false);
            m_pSecurityAlertDlg->doorbell_power_on_flag = false;
        }
		m_pSecurityAlertDlg->stopRecorder(false);
	}

	if (pMainDlg->m_pHelpDlg->isVisible())
	{
		pMainDlg->m_pHelpDlg->hide();
	}
	if (pMainDlg->m_pDoorBellDlg->isVisible())
	{
		pMainDlg->m_pDoorBellDlg->stopViewer();
		pMainDlg->m_pDoorBellDlg->on_pushButtonDoorBellReturn_pressed();
		pMainDlg->m_pDoorBellDlg->hide();
	}

    //lg 12.22
    if (g_pMainDlg->m_pAddressBookDlg->isVisible())
    {
	    g_pMainDlg->m_pAddressBookDlg->on_pushButtonAddressBookCancel_pressed();
    }

    if (g_pMainDlg->m_pPhotoMainDlg->m_pFrameFullshow->isVisible())
    {
	    g_pMainDlg->m_pPhotoMainDlg->m_pTimer->stop();
	    g_pMainDlg->m_pPhotoMainDlg->m_pFrameFullshow->hide();
	    g_pMainDlg->m_pPhotoMainDlg->m_pFrameFullshow->lower();
    }

    if (pMainDlg->m_pPhotoMainDlg->m_pFrameFullshow->isVisible())
    {
	    g_pMainDlg->m_pPhotoMainDlg->m_pTimer->stop();
	    g_pMainDlg->m_pPhotoMainDlg->m_pFrameFullshow->hide();
	    g_pMainDlg->m_pPhotoMainDlg->m_pFrameFullshow->lower();
    }

    if (g_pMainDlg->m_pPhotoMainDlg->m_pPhotoUDiskDlg->isVisible())
    {
	    g_pMainDlg->m_pPhotoMainDlg->m_pPhotoUDiskDlg->on_pushButtonPhotoUDiskReturn_pressed();
    }

    if (g_pMainDlg->m_pSysSetMainDlg->isVisible())
    {
	    g_pMainDlg->m_pSysSetMainDlg->on_pushButtonSysSetMainVersion_pressed();
    }

    if (g_pMainDlg->m_pSysSetEngineerDlg->isVisible())
    {
	    g_pMainDlg->m_pSysSetEngineerDlg->on_pushButtonSysSetEngineerNetAddress_pressed();
    }
    //lg

	if (pMainDlg->m_pVoipMainDlg->m_pMonDlg->isVisible())
	{
		pMainDlg->m_pVoipMainDlg->m_pMonDlg->Exit_From_Full_Screen_Status();		// 先要从全屏状态退出，如果有的话
		pMainDlg->m_pVoipMainDlg->m_pMonDlg->on_pushButtonMonReturn_pressed();
	}

	if (pMainDlg->m_pVoipMainDlg->m_pMstDlg->isVisible())
	{
		g_bIstOrAlertBusy = 0;
		return;
	}

	if (pMainDlg->m_pMediaManDlg->m_pMediaPlayerDlg->isVisible())
	{
		//return;
		pMainDlg->m_pMediaManDlg->m_pMediaPlayerDlg->stopPlayer();
		pMainDlg->m_pMediaManDlg->m_pMediaPlayerDlg->done(0);
	}

	if (pMainDlg->m_pInfoManMainDlg->isVisible() && pMainDlg->m_pInfoManMainDlg->m_pInfoManDetailDlg->isVisible())
	{
		if (pMainDlg->m_pInfoManMainDlg->m_pInfoManDetailDlg->m_pViewAttachemntDlg->isVisible())
		{
			pMainDlg->m_pInfoManMainDlg->m_pInfoManDetailDlg->m_pViewAttachemntDlg->hide();//done(0);
		}
	}

    if(pMainDlg->m_pHomeControlDlg->isVisible())
    {
        pMainDlg->m_pHomeControlDlg->hide();
    }

	if (m_pSecurityPlayerDlg->isVisible())
	{
		m_pSecurityPlayerDlg->stopPlayer();
		m_pSecurityPlayerDlg->done(0);
	}

    if (m_pSerial1Thread->m_pJpegSendThread )
    {

        printf("m_iSendPicToCenter:%d\n",m_iSendPicToCenter);
        if (m_iSendPicToCenter)
        {
            g_jpg_tvd_image_ready = 3;//bug

        }
    }

	m_pSecurityAlertDlg->setDetail(strDateTime, number, property, channel, name,
			type, strLocation, strAviPath);

    //lg 12.22
    m_bRecording = true;
    //lg

    show();
    raise();

	m_pSecurityAlertDlg->show();
	m_pSecurityAlertDlg->raise();

	//m_pSecurityAlertDlg->repaint(); david /2014/7/4
	m_pSecurityAlertDlg->update();

	remove_oldest_avi_file();			// 在写入新的AVI文件前，删除最旧的AVI文件

	repaint();

	m_pSecurityAlertDlg->startRecorder();				// 真正写入AVI文件的地方

	//printf("\tmy startRecorder\n");
	{
		int i;
		int year, month, day, hour, minute, second,/*msec,*/defensenumber;
		QString strDateTime, strSec;

		sscanf(strAviPath.toLocal8Bit().data(), "/mnt/disk/sec_media/%04d%02d%02d-%02d%02d%02d-%03d.avi",
				&year, &month, &day, &hour, &minute, &second,&defensenumber/*&msec*/);//david 2014.4.10
		strDateTime.sprintf("%d-%d-%d %d:%02d", year, month, day, hour, minute);
		strSec.sprintf("%02d-%03d", second,defensenumber/* msec*/);

		printf("lg %s %s\n", strDateTime.toLocal8Bit().data(), strSec.toLocal8Bit().data());

		for (i = 0; i < model->rowCount(); i++)
		{
			//printf("%d lg %s %s\n", i, strDateTime.toLocal8Bit().data(), strMSec.toLocal8Bit().data());
			printf("lg 2 %d %s %s\n", i, model->item(i, 0)->text().toLocal8Bit().data(), model->item(i, 5)->text().toLocal8Bit().data());
			if ((model->item(i, 0)->text() == strDateTime) && (model->item(i, 5)->text() == strSec))
			{
				model->item(i, 4)->setText("yes");
				break;
			}
		}
	}

	g_bIstOrAlertBusy = 0;

    m_pSerial1Thread->m_pSendAlarmInfoThread->m_StarSendJpegThreadFlag = true;
//    if (m_pSerial1Thread->m_pJpegSendThread )
//    {

//        printf("m_iSendPicToCenter:%d\n",m_iSendPicToCenter);
//        if (m_iSendPicToCenter)
//        {
//            //g_jpg_tvd_image_ready = 3;//bug
//            m_pSerial1Thread->m_pJpegSendThread->m_bRun = true;
//            m_pSerial1Thread->m_pJpegSendThread-> m_alarm_num = number;
//            m_pSerial1Thread->m_pJpegSendThread->start();
//        }
//        else
//        {
//            int i;
//            for (i = 0; i < 2; i++)
//            {
//                if (m_pSerial1Thread->m_pJpegSendThread->m_pTcpSock[i])
//                {
//                    m_pSerial1Thread->m_pJpegSendThread->m_pTcpSock[i]->close();
//                    delete m_pSerial1Thread->m_pJpegSendThread->m_pTcpSock[i];
//                    m_pSerial1Thread->m_pJpegSendThread->m_pTcpSock[i] = NULL;
//                }
//            }
//        }
//    }
}

/************************************************************
描述：创建报警JPEG图像发送线程
参数：无
返回：无
************************************************************/
void SecurityMainDialog::createJpegSendThread()
{
	m_pSerial1Thread->m_pJpegSendThread = new JpegSendThread(this);
}

/************************************************************
描述：选择报警记录列表控件中的上一页记录
参数：无
返回：无
************************************************************/
void SecurityMainDialog::on_pushButtonSecMainPrev_pressed()
{
	int row = ui->tableViewSecurityList->verticalScrollBar()->value();

	if (row - TABLE_ROWS_PER_PAGE >= 0)
	{
		ui->tableViewSecurityList->selectRow(row - TABLE_ROWS_PER_PAGE);
		ui->tableViewSecurityList->verticalScrollBar()->setSliderPosition(row - TABLE_ROWS_PER_PAGE);
	}
	else if (row > 0)
	{
		ui->tableViewSecurityList->selectRow(0);
		ui->tableViewSecurityList->verticalScrollBar()->setSliderPosition(0);
	}

	slotSecurityListPressed();
}

/************************************************************
描述：选择报警记录列表控件中的下一页记录
参数：无
返回：无
************************************************************/
void SecurityMainDialog::on_pushButtonSecMainNext_pressed()
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewSecurityList->model();
	int row = ui->tableViewSecurityList->verticalScrollBar()->value();

	if (row + TABLE_ROWS_PER_PAGE < model->rowCount())
	{
		ui->tableViewSecurityList->selectRow(row + TABLE_ROWS_PER_PAGE);
		ui->tableViewSecurityList->verticalScrollBar()->setSliderPosition(row + TABLE_ROWS_PER_PAGE);
	}

	slotSecurityListPressed();
}

void SecurityMainDialog::on_tableViewSecurityList_pressed(QModelIndex index)
{
	Q_UNUSED(index);

	m_pTimer->start(200);
}

void SecurityMainDialog::slotSecurityListPressed()
{
	m_pTimer->stop();

	int row = ui->tableViewSecurityList->currentIndex().row();
	if (row < 0) return;

	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewSecurityList->model();
	QString strHaveAvi = model->item(row, 4)->text();
    ui->pushButtonSecMainDetail->setEnabled(strHaveAvi == "yes");
}

void SecurityMainDialog::on_pushButtonSecMainSetArm_pressed()
{
	if (m_pSecurityAbnormalDlg->isVisible()) return;

	m_pSecurityAbnormalDlg->resetState();
	m_pSecurityAbnormalDlg->show();
}

/************************************************************
描述：设置指定报警管理信息框高度
参数：无
返回：无
************************************************************/
void SecurityMainDialog::slotSetSecRowHeight(int ID, int row)
{
    if (ID == 1)
        ui->tableViewSecurityList->setRowHeight(row, g_pScaleSrc->get_public_tableviewlist_row_height());
}

/************************************************************
描述：删除最旧的报警AVI文件
参数：无
返回：无
************************************************************/
void SecurityMainDialog::remove_oldest_avi_file()
{
	int del_index = 0;
	int totalcount = 0;
	QString temp("yes");
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewSecurityList->model();
	for (int i = model->rowCount()-1; i >= 0 ; i--)			// 从历史上最旧的记录开始查起
	{
		if ( model->item(i, 4)->text()==temp )                  // 有AVI文件的记录数
		{
			totalcount ++;
			if ( totalcount == 1 )
				del_index = i;
		}
	}

	if ( totalcount >= SECURITY_AVI_RECORD_COUNT )			//  如果record.xml中记录的AVI文件刚好等于了限定数量，就删除一个
	{
		QString strTime;
		QString strTimeSec;
		char filepath[PATH_MAX];
		int year, month, day, hour, minute, second,/*msecond,*/defensenumber;


		strTime = model->item(del_index, 0)->text();
		strTimeSec = model->item(del_index, 5)->text();
		sscanf(strTime.toUtf8().data(), "%d-%d-%d %d:%02d", &year, &month, &day,
				&hour, &minute);
		sscanf(strTimeSec.toUtf8().data(), "%02d-%03d", &second, &defensenumber);

		sprintf(filepath, "/mnt/disk/sec_media/%04d%02d%02d-%02d%02d%02d-%03d.avi", year, month, day,
				hour, minute, second, defensenumber);
		remove(filepath);

		model->item(del_index, 4)->setText("no");
	}
}

void SecurityMainDialog::slotRefreshList(QDateTime dateTime,int number,int type,int name,QString strLocation)
{
	char filepath[PATH_MAX];

	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewSecurityList->model();

	QStandardItem *newItem[9];
	QString strTemp,strDateTime;

	sprintf(filepath, "/mnt/disk/sec_info/%04d%02d%02d-%02d%02d%02d-%03d.txt", dateTime.date().year(), dateTime.date().month(),
			dateTime.date().day(), dateTime.time().hour(), dateTime.time().minute(), dateTime.time().second(),number);

	strDateTime.sprintf("%d-%d-%d %d:%02d", dateTime.date().year(), dateTime.date().month(), dateTime.date().day(),
			dateTime.time().hour(), dateTime.time().minute());

	newItem[0] = new QStandardItem(strDateTime);
    newItem[0]->setIcon(QIcon(":/images/infoman_dialog/unread.png"));

	QFont font("Century Gothic");
	newItem[1] = new QStandardItem(" " + QString::number(number));
	newItem[1]->setTextAlignment(Qt::AlignCenter);

	font.setPixelSize(22);
	newItem[1]->setFont(font);
	newItem[2] = new QStandardItem(QObject::tr(g_strType[name]));
	newItem[3] = new QStandardItem(strLocation);
	newItem[4] = new QStandardItem("no");
    	strTemp.sprintf("%02d-%03d", dateTime.time().second(), number/*dateTime.time().msec()*/);//david 2014.4.10
	newItem[5] = new QStandardItem(strTemp);
	newItem[6] = new QStandardItem("&" + QString::number(type) + " " + QString::number(number));
	newItem[7] = new QStandardItem(filepath + strlen("/mnt/disk/sec_info/"));
	newItem[8] = new QStandardItem("1");

	model->insertRow(0);

   	// systemRunLog(5);

	for (int i = 0; i < 9; i++)
	{
		if (i != 1)
		{
			newItem[i]->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
		model->setItem(0, i, newItem[i]);
	}
    	g_pMainDlg->emitSigSetRowHeight(1, 0);//修复安防管理信息框比较大问题 lidh 20141018

    	if (ui->pushButtonSecMainDel->isEnabled())
	{
		ui->tableViewSecurityList->selectRow(0);
	}
/* 				以上将安防信息保存 			     */
/* ------------------------------------------------------------------------- */
/* 				以下进行多余文件的删除 			     */

	struct statfs diskInfo;
	statfs("/",&diskInfo);
	int freeDiskVolume = diskInfo.f_bfree * diskInfo.f_bsize >> 20;

    //david 2014.5.7
	if(freeDiskVolume < 20)
		dellastsecurityavifile();					    // 删除最旧的AVI文件

	if (model->rowCount() > SECURITY_RECORD_COUNT)			            // 如果超过N条记录，那么进行最后几跳多余记录的删除
	{
		int rowCnt = model->rowCount();
		for (int i = SECURITY_RECORD_COUNT; i < rowCnt; i++)
		{
			delSecurityRecord(SECURITY_RECORD_COUNT);
		}
	}
}
/************************************************************
描述：发射上传防拆防移信息信号
参数：infotype : 为0代表上传设备拆除信息 为1代表上传设备移离信息
返回：无
************************************************************/
void SecurityMainDialog::emitUploadDeviceInfo(int infotype)
{
    emit sigUploadDeviceInfo(infotype);
}

void SecurityMainDialog::emitSigPlayWav(int type)
{
    emit sigPlayWav(type);
}
/************************************************************
描述：发射信号将分机的布撤防状态同步给手机
参数：set_state - STATUS_DISARM:撤防状态
                 STATUS_GOOUT:外出设防状态
                 STATUS_ATHOME:居家设防状态
返回：无
************************************************************/
void SecurityMainDialog::emitsigSetSecurityStatueToPhone(int set_state)
{
    emit sigSetSecurityStatueToPhone(set_state);
}
/************************************************************
描述：启动上传防拆防移离信息线程
参数：infotype : 为0代表上传设备拆除信息 为1代表上传设备移离信息
返回：无
************************************************************/
void SecurityMainDialog::slotUploadDeviceInfo(int infotype)
{
    if(m_pUploadDeviceInfoThread->m_bRun == false)
    {
        m_pUploadDeviceInfoThread->m_infotype = infotype;
        m_pUploadDeviceInfoThread->start();
    }
}
/************************************************************
描述：协议探测
参数：无
返回：无
************************************************************/
void SecurityMainDialog::CheckUdpState()
{
    addr_info_t    addr_computer;
    char loc_ip[20] = {'\0'} ;
    char loc_usr[20] = {'\0'} ;
    char remcomputer_ip[20] = {'\0'};
    char remcomputer_usr[20] = {'\0'};
    char strIP[50] = {'\0'};

    GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;
    strcpy(loc_ip, pGetPeerIPThread->m_nativeIP.toLatin1().data());
    strcpy(loc_usr, pGetPeerIPThread->m_nativeName.toLatin1().data());

    if (pGetPeerIPThread->GetPeerIP(g_strLphoneName[2], strIP) != 0)
    {
        return;
    }

    char *ptr;
    int iModalType = BASIC_MODAL_TYPE;

    ptr = strstr(strIP, "?");
    if (ptr)
    {
        iModalType = atoi(ptr + strlen("?"));
        *ptr = '\0';
    }

    if (iModalType != MANCENTER_MODAL_TYPE)
    {
        return ;
    }

    strcpy(remcomputer_ip,strIP);
    strcpy(remcomputer_usr,g_strLphoneName[2]);

    addr_computer.loc_ip = (uint8_t *)loc_ip;
    addr_computer.loc_usr = (uint8_t *)loc_usr;
    addr_computer.rem_ip = (uint8_t *)remcomputer_ip;
    addr_computer.rem_usr = (uint8_t *)remcomputer_usr;

    int ret = start_system_0x0501(&addr_computer, secmaindlg_result_callback);
    printf("start_system_0x0501 ret = %d\n",ret);

}

/************************************************************
描述：使用udp上传设备异常信息
参数：infotype : 为0代表设备拆除 为1代表设备移离
返回：无
************************************************************/
void SecurityMainDialog::SendDeviceInfoByUdp(int infotype)
{
    addr_info_t    addr;
    device_record_t device;
    char loc_ip[20] = {'\0'};
    char loc_usr[20] = {'\0'};
    char rem_ip[20] = {'\0'};
    char rem_usr[20] = {'\0'};
    char strIP[50] = {'\0'};

    char device_type = 0x01;
    char device_move = 0x01;//设备移离
    char device_demolish = 0x02;//设备拆除
    char device_len = 11;

    QSettings settings(CONFIG_FILE, QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("UTF8"));

    GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;
    strcpy(loc_ip, pGetPeerIPThread->m_nativeIP.toLatin1().data());
    strcpy(loc_usr, pGetPeerIPThread->m_nativeName.toLatin1().data());

    if (pGetPeerIPThread->GetPeerIP(g_strLphoneName[2], strIP) != 0)
    {
        return ;
    }
    char *ptr;
    ptr = strstr(strIP, "?");
    if (ptr)
    {
       *ptr = '\0';
    }

    strcpy(rem_ip,strIP);
    strcpy(rem_usr,g_strLphoneName[2]);

    addr.loc_ip = (uint8_t *)loc_ip;
    addr.loc_usr = (uint8_t *)loc_usr;
    addr.rem_ip = (uint8_t *)rem_ip;
    addr.rem_usr = (uint8_t *)rem_usr;

    device.device_type = (uint8_t)device_type;
    addrtobyte(addr.loc_usr,device.device_num);
    if(infotype == 0 )
    {
        device.device_alarm = (uint8_t)device_demolish;
    }
    else
    {
        device.device_alarm = (uint8_t)device_move;
    }
    device.device_len = (uint8_t)device_len;
    device.device_name = (uint8_t*)loc_usr;

    int res = start_system_0x0503(&addr,&device, NULL);
    printf("start_system_0x0503 res = %d\n",res);
}
/************************************************************
描述：使用tcp上传设备异常信息
参数：infotype : 为0代表设备拆除 为1代表设备移离
返回：无
************************************************************/
void SecurityMainDialog::SendDeviceInfoByTcp(int infotype)
{
    connectToCenter();

    if(!m_DeviceInfoSocket)
    {
        printf("connect to center software error \n");
        return;
    }

    unsigned char tag = 0x05;
    unsigned char version = 0x00;
    int length = 13;
    char cmd = 0;
    char nativename[9] = {'\0'};
    char devicetype = 1;
    unsigned char alarm_type;
    char number = 0;

    memcpy(nativename,qPrintable(g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread->m_nativeName),9);
    if(infotype ==0)
    {
        alarm_type = 0x02;
    }
    else
    {
        alarm_type = 0x01;
    }

    if(m_DeviceInfoSocket)
    {
         m_DeviceInfoSocket->write((char *)&tag,sizeof(unsigned char));
         m_DeviceInfoSocket->write((char*)&version,sizeof(unsigned char));
         m_DeviceInfoSocket->write((char *)&length,sizeof(int));
         m_DeviceInfoSocket->write((char*)&cmd,sizeof(char));
         m_DeviceInfoSocket->write((char *)nativename,9);
         m_DeviceInfoSocket->write((char*)&devicetype,sizeof(char));
         m_DeviceInfoSocket->write((char*)&alarm_type,sizeof(unsigned char));
         m_DeviceInfoSocket->write((char*)&number,sizeof(char));
         m_DeviceInfoSocket->flush();

    }
    if(m_DeviceInfoSocket)
    {
        m_DeviceInfoSocket->close();
        delete m_DeviceInfoSocket;
        m_DeviceInfoSocket = NULL;
    }

}
void SecurityMainDialog::ReceiveUdpData(unsigned char receive_data)
{
    emit sigHandleReceiveUdpData(receive_data);
}
void SecurityMainDialog::slotHandleReceiveUdpdata(unsigned char receive_data)
{   
    int state = (int)receive_data;
    if(state == 1)
    {
        state = STATUS_DISARM;
    }
    else if(state == 2)
    {
        state = STATUS_ATHOME;
    }
    else if(state == 3)
    {
        state = STATUS_GOOUT;
    }

    QString strWaveFile;
    if(m_securityState == state)
    {
        return ;
    }

    switch(state)
    {
        case STATUS_DISARM:   //撤防模式
        {
            strWaveFile = "disarm";
            break;
        }
        case STATUS_ATHOME:   //居家模式
        {
            strWaveFile = "athome";
            break;
        }
        case STATUS_GOOUT:    //外出模式
        {
            strWaveFile = "goout";
            break;
        }
        default:
            break;
    }

    if (g_pMainDlg->m_inputMethodMode == 0)
    {
        strWaveFile += "-en";
    }

    strWaveFile = "wav/" + strWaveFile + ".wav";

    if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
    {
        usleep(50000);
    }
    playWaveFile(strWaveFile.toLocal8Bit().data(), g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);

    setSecurityState(state);
    if(m_pUploadSetSecurityStateThread->m_bRun)
    {
        m_pUploadSetSecurityStateThread->m_bRun = false;
        m_pUploadSetSecurityStateThread->quit();
        m_pUploadSetSecurityStateThread->wait();
    }
    m_pUploadSetSecurityStateThread->start();
    m_pSecurityAbnormalDlg->resetState();

}
int SecurityMainDialog::connectToCenter()
{
    if(m_DeviceInfoSocket)  delete m_DeviceInfoSocket;

    m_DeviceInfoSocket = new QTcpSocket;

    char strIP[80];

    GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;

    if(pGetPeerIPThread->GetPeerIP(g_strLphoneName[2],strIP) != 0)
    {
        printf("can not conenct to manage center software\n");
        return -1;
    }

    char *ptr;
    int iModalType = 0;

    ptr = strstr(strIP,"?");

    if(ptr)
    {
        iModalType = atoi(ptr + strlen("?"));
        *ptr = '\0';
    }

    if(iModalType != MANCENTER_MODAL_TYPE)  return -1;

    m_DeviceInfoSocket->connectToHost(strIP,17721);
    if(!m_DeviceInfoSocket->waitForConnected(500))
    {
        delete m_DeviceInfoSocket;
        m_DeviceInfoSocket = NULL;
    }

   return 0;
}
void SecurityMainDialog::slotPlayWav(int type)
{
    if (type == 0)
    {
        if (g_pMainDlg->m_inputMethodMode)
	{
		if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
			usleep(50000);;
		playWaveFile((char *)"wav/deviceopen.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
	}
        else
	{
		if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
			usleep(50000);;

            playWaveFile((char *)"wav/deviceopen-en.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
	}
    }
    else if (type == 1)
    {
        if (g_pMainDlg->m_inputMethodMode)
	{
		if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
			usleep(50000);;

            playWaveFile((char *)"wav/devicemove.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
	}
        else
	{
		if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
			usleep(50000);;

            playWaveFile((char *)"wav/devicemove-en.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
	}
    }
}
