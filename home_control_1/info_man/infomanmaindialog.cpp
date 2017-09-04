#include "infomanmaindialog.h"
#include "ui_infomanmaindialog.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "data_types.h"
#include <QTextCodec>
#include <unistd.h>
#include <qconfig.h>
#include "maindialog.h"
#include "infomanmaindialog.h"
//#include "callcenterdialog.h"
#include "ui_maindialog.h"
//#include "terminalhelpmaindialog.h"
//#include "helpdialog.h"
#include "application.h"
#include "ui_maindialog.h"
#include <QSettings>
#include <sys/vfs.h>

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
/************************************************************
  该文件为接收到的信息内容列表对话框主程序
************************************************************/

extern char g_strNativeName[50];
extern MainDialog *g_pMainDlg;

/************************************************************
描述：信息详细内容列表对话框构造函数
参数：parent - 该对话框的父窗口指针
返回：无
************************************************************/
InfoManMainDialog::InfoManMainDialog(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::InfoManMainDialog)
{
	ui->setupUi(this);

	//去掉 Dialog 标题栏
	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23
                                          // make the dialog full screen
    /*ui->pushButtonCallCenter->setText("");
    ui->pushButtonInfoManMainDel->setText("");
    ui->pushButtonEmergency->setText("");
    ui->pushButtonHelp->setText("");
    ui->pushButtonInfoManMainDetail->setText("");
    ui->pushButtonInfoManMainNext->setText("");
    ui->pushButtonInfoManMainPrev->setText("");
    ui->pushButtonInfoManMainReturn->setText("");
    */

    //m_pInfoManDetailDlg = NULL;

    int res = g_pScaleSrc->get_screen_version();
    if ( res == 2 )  // 1024x600
    {
        //ui->tableViewInfoList->setGeometry(QRect(58, 97, 660, 468));
        ui->pushButtonInfoManMainReturn->setGeometry(QRect(764, 500, 230, 80));
        ui->pushButtonInfoManMainPrev->setGeometry(QRect(764, 20, 230, 80));
        ui->pushButtonInfoManMainNext->setGeometry(QRect(764, 100, 230, 80));
        ui->pushButtonInfoManMainDetail->setGeometry(QRect(764, 260, 230, 80));
        ui->pushButtonInfoManMainDel->setGeometry(QRect(764, 340, 230, 80));
        ui->pushButtonInfoManMainWrite->setGeometry(QRect(764, 180, 230, 80));
        ui->pushButtonInfoManMainDelAll->setGeometry(QRect(764, 420, 230, 80));
        //ui->framePanel->setGeometry(QRect(38, 17, 700, 570));
    }
    else if (res == 3) // 1280*800
    {
        //ui->tableViewInfoList->setGeometry(QRect(80, 139, 811, 611));
        ui->pushButtonInfoManMainReturn->setGeometry(QRect(959, 677, 279, 99));
        ui->pushButtonInfoManMainPrev->setGeometry(QRect(959, 29, 279, 99));
        ui->pushButtonInfoManMainNext->setGeometry(QRect(959, 137, 279, 99));
        ui->pushButtonInfoManMainDetail->setGeometry(QRect(959, 353, 279, 99));
        ui->pushButtonInfoManMainDel->setGeometry(QRect(959, 461, 279, 99));
        ui->pushButtonInfoManMainWrite->setGeometry(QRect(959, 245, 279, 99));
        ui->pushButtonInfoManMainDelAll->setGeometry(QRect(959, 569, 279, 99));
        //ui->framePanel->setGeometry(QRect(56, 30, 858, 745));
    }

	int x,y,w,h;
	g_pScaleSrc->get_info_framepanel_geometry(x,y,w,h);
	ui->framePanel->setGeometry(x,y,w,h);

	QStandardItemModel *model = new QStandardItemModel(0, 6);////5);

	g_pScaleSrc->get_tabviewinfolist_geometry(x,y,w,h);
	ui->tableViewInfoList->setGeometry(x,y,w,h);

	ui->tableViewInfoList->setModel(model);
	ui->tableViewInfoList->verticalHeader()->hide();
	ui->tableViewInfoList->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableViewInfoList->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableViewInfoList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableViewInfoList->setSortingEnabled(false);//true); //lg lg
	ui->tableViewInfoList->setShowGrid(true);
	ui->tableViewInfoList->setFocusPolicy(Qt::NoFocus);
    ui->tableViewInfoList->horizontalHeader()->setHighlightSections(false);// add by lidh 20141119 点击表时不对表头行光亮

	/*
	model->setHeaderData(0, Qt::Horizontal, tr("日期"));
	model->setHeaderData(1, Qt::Horizontal, tr("发送者"));
	model->setHeaderData(2, Qt::Horizontal, tr("主题"));
	model->setHeaderData(3, Qt::Horizontal, tr("毫秒"));
	model->setHeaderData(4, Qt::Horizontal, tr("当前附件序号"));
	*/

	g_pScaleSrc->get_info_tabviewinfolist_width(x,y,w);
	ui->tableViewInfoList->setColumnWidth(0, x);
	ui->tableViewInfoList->setColumnWidth(1, y);
	ui->tableViewInfoList->setColumnWidth(2, w);

	ui->tableViewInfoList->setColumnWidth(3, g_pScaleSrc->get_w_value(50));
	ui->tableViewInfoList->setColumnWidth(4, g_pScaleSrc->get_w_value(50));
	ui->tableViewInfoList->setColumnWidth(5, g_pScaleSrc->get_w_value(50));

	ui->tableViewInfoList->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
	ui->tableViewInfoList->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
	ui->tableViewInfoList->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
	ui->tableViewInfoList->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
	ui->tableViewInfoList->horizontalHeader()->setResizeMode(4, QHeaderView::Fixed);
	ui->tableViewInfoList->horizontalHeader()->setResizeMode(5, QHeaderView::Fixed);
	ui->tableViewInfoList->setColumnHidden(3, true);
	ui->tableViewInfoList->setColumnHidden(4, true);
	ui->tableViewInfoList->setColumnHidden(5, true);

	ui->tableViewInfoList->setIconSize(QSize(g_pScaleSrc->get_w_value(20), g_pScaleSrc->get_h_value(16)));

	g_pScaleSrc->get_info_label_geometry(x,y,w,h);
	ui->label_title->setGeometry(x,y,w,h);

	////ui->tableViewInfoList->setColumnHidden(4, true);
	//ui->tableViewInfoList->horizontalHeader()->setStyleSheet("QHeaderView::section{ background-color:rgba(255, 255, 255, 128);}");

	////connect(ui->tableViewInfoList->horizontalHeader(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
	////		this, SLOT(tvInfoListSortIndicatorChanged(int, Qt::SortOrder)));

	QSettings settings_config(CONFIG_FILE, QSettings::IniFormat);
	m_newInfoCnt = settings_config.value("info/new_info_cnt", 0).toInt();
	//m_newInfoCnt = 0;
	//emit g_pMainDlg->emitSigSetNotify(2);
	QStandardItemModel *model2 = (QStandardItemModel *)g_pMainDlg->ui->tableViewNotifier->model();
	if (m_newInfoCnt == 0)
	{
        model2->item(5, 1)->setIcon(QIcon(":/images/main/information_none.png"));
        model2->item(5, 2)->setText(MainDialog::tr("无未查看信息"));
	}
	else
	{
        model2->item(5, 1)->setIcon(QIcon(":/images/main/information.png"));
        model2->item(5, 2)->setText(MainDialog::tr("有") + QString::number(m_newInfoCnt) + MainDialog::tr("条未查看信息"));
	}
	//((MainDialog *)parent)->getUi()->labelIconInfoCnt->setText("\n\n" + QString::number(m_newInfoCnt));

	//ReloadInfoList();
	m_pInfoManDetailDlg = new InfoManDetailDialog(this);
	m_pInfoWriteDlg = new InfoWriteDialog(this);

	ReloadInfoList();
    connect(this,SIGNAL(sigHandlUdpInfoFile(char*)),this,SLOT(slotHandlUdpInfoFile(char*)));
}

/************************************************************
描述：信息详细内容列表对话框析构函数
参数：无
返回：无
************************************************************/
InfoManMainDialog::~InfoManMainDialog()
{
	delete m_pInfoManDetailDlg;
	delete m_pInfoWriteDlg;

	delete ui;
}

/************************************************************
描述：获取信息列表控件
参数：无
返回：信息列表控件的指针
************************************************************/
QTableView *InfoManMainDialog::GetTableViewInfoList()
{
	return ui->tableViewInfoList;
}

/************************************************************
描述：接收远程发布的信息，存盘后更新信息列表控件记录
参数：p_sock - 接收远程信息的QSocket
	 buf - 接收到的数据的缓存区
	 len - 接收到的数据的字节数
返回：0 - 接收信息成功
	 -1 - 接收信息失败
************************************************************/
int InfoManMainDialog::InfoReceive(NetRecvClientSocket *p_sock, char *buf, int len)
{
	int i;
	//struct tm		tm_time;
	//struct timeval	tv_time;
	FILE		   *fpInfo;
	char			filepath[PATH_MAX];
	uint16_t		len16;
	char*			pData;
	QTextCodec	   *gk_codec = QTextCodec::codecForName("GBK");
	uint16_t		msg_len;
	char		   *msg_buf;
	uint16_t		sub_type;
	QDateTime		datetime;

	memcpy(&sub_type, buf, sizeof(uint16_t));
	buf += 2;
	len -= 2;

	if (sub_type != 0)
	{
		return -1;
	}

	i = 0;
	do
	{
		datetime = QDateTime::currentDateTime();

		sprintf(filepath, "/mnt/disk/info/%04d%02d%02d-%02d%02d%02d-%03d.txt", datetime.date().year(), datetime.date().month(),
				datetime.date().day(), datetime.time().hour(), datetime.time().minute(), datetime.time().second(),
				datetime.time().msec());

		if ((access(filepath, F_OK)) == -1) break;

		usleep(10000);
	}
	while (++i <= 5);

	if (i >= 5) return -1;

	fpInfo = fopen(filepath, "wb");

	if (!fpInfo) return -1;

	//SenderNameLen
	memcpy(&len16, buf, sizeof(int16_t));
	buf += 2;
	pData = (char *)malloc(len16 + 1);
	memcpy(pData, buf, len16);
	pData[len16] = 0;
	buf += len16;
	fprintf(fpInfo, "FROM:%s\n", gk_codec->toUnicode(pData).toUtf8().data());
	//printf("bcom free 2\n");
	free(pData);
	len -= 2 + len16;

	//SenderIP
	fprintf(fpInfo, "FROM_IP:%s\n", p_sock->peerAddress().toString().toUtf8().data());

	//TitleLen
	memcpy(&len16, buf, sizeof(int16_t));
	buf += 2;
	pData = (char *)malloc(len16 + 1);
	memcpy(pData, buf, len16);
	pData[len16] = 0;
	buf += len16;
	fprintf(fpInfo, "TITLE:%s\n", gk_codec->toUnicode(pData).toUtf8().data());
	//printf("bcom free 3\n");
	free(pData);
	len -= 2 + len16;

	fprintf(fpInfo, "UNREAD:1\n");

	//Msg body len
	memcpy(&msg_len, buf, sizeof(int16_t));
	buf += 2;
	msg_buf = (char *)malloc(msg_len + 1);
	memcpy(msg_buf, buf, msg_len);
	msg_buf[msg_len] = 0;
	buf += msg_len;
	len -= 2 + msg_len;

	//JPEG Receive
	unsigned short pic_cnt;
	memcpy(&pic_cnt, buf, sizeof(uint16_t));
	buf += 2;
	len -= 2;
	fprintf(fpInfo, "ATTACHMENT:\n");

	for (i = 0; i < pic_cnt; i++)
	{
		int len32;
		FILE *fpPic;

		//JPEG file name
		memcpy(&len16, buf, sizeof(uint16_t));
		buf += 2;
		pData = (char *)malloc(len16 + 1);
		memcpy(pData, buf, len16);
		pData[len16] = 0;
		buf += len16;
		len -= 2 + len16;
		fprintf(fpInfo, "\t%s\n", gk_codec->toUnicode(pData).toUtf8().data());

		sprintf(filepath, "/mnt/disk/info/%04d%02d%02d-%02d%02d%02d-%03d-%s", datetime.date().year(),
			datetime.date().month(), datetime.date().day(), datetime.time().hour(), datetime.time().minute(),
			datetime.time().second(), datetime.time().msec(), pData);

		//printf("bcom free 4\n");
		free(pData);

		//JPEG data
		memcpy(&len32, buf, sizeof(uint32_t));
		buf += 4;
		fpPic = fopen(gk_codec->toUnicode(filepath).toUtf8().data(), "wb");
		if (fpPic)
		{
			fwrite(buf, 1, len32, fpPic);
			fclose(fpPic);
		}
		buf += len32;
		len -= 4 + len32;
	}

	//write msg body
	fprintf(fpInfo, "BODY:%s", gk_codec->toUnicode(msg_buf).toUtf8().data());
	//printf("bcom free 5\n");
	free(msg_buf);
	fclose(fpInfo);

	if (!isVisible())
	{
		m_newInfoCnt++;

		QSettings settings_config(CONFIG_FILE, QSettings::IniFormat);
		settings_config.setValue("info/new_info_cnt", m_newInfoCnt);

		g_pMainDlg->emitSigSetNotify(2);
	}

	sprintf(filepath, "%04d%02d%02d-%02d%02d%02d-%03d.txt", datetime.date().year(),
			datetime.date().month(), datetime.date().day(), datetime.time().hour(),
			datetime.time().minute(), datetime.time().second(), datetime.time().msec());

    struct statfs diskInfo;

    statfs("/",&diskInfo);

    int freeDiskVolume = diskInfo.f_bfree * diskInfo.f_bsize >> 20;

    //david 2014.5.7
    if(freeDiskVolume < 20)
    {
        int rowCnt = ui->tableViewInfoList->model()->rowCount();
        delInfoRecord(rowCnt - 1);
    }

    if (ui->tableViewInfoList->model()->rowCount() >= 45/* || freeDiskVolume < 20*/)
	{
		int rowCnt = ui->tableViewInfoList->model()->rowCount();
		for (i = 45 - 1; i < rowCnt; i++)
		{
			delInfoRecord(45 - 1);
		}
	}

	((Application *)qApp)->setLCDAndTimerOn();

	InfoListAddOne(filepath);

    if ((!m_pInfoManDetailDlg->isVisible()/* == NULL*/) && (ui->pushButtonInfoManMainDel->isEnabled()))
	{
		ui->tableViewInfoList->selectRow(0);//lg lg ui->tableViewInfoList->model()->rowCount() - 1);
	}

	//ui->tableViewInfoList->horizontalHeader()->setSortIndicator(sortIdx, sortOrder);
	////setTableViewInfoListColor(TABLE_VIEW_COLOR);

	if (len == 0)
		return 0;
	else
		return -1;
}

/************************************************************
描述：接收udp库传上来的包含信息数据的文件
参数：str_file_name:文件名字符串
返回：无
************************************************************/
void InfoManMainDialog::ReceiveUdpInfo(char *str_file_name)
{
    static char tmp_file_name[128] = {'\0'};
    memcpy(tmp_file_name,str_file_name,sizeof(tmp_file_name));
    emit sigHandlUdpInfoFile(tmp_file_name);
}
/************************************************************
描述：处理信息更新列表
参数：str_file_name:文件名字符串
返回：无
************************************************************/
void InfoManMainDialog::slotHandlUdpInfoFile(char *info_file_name)
{
    int i;
    struct statfs diskInfo;

    statfs("/",&diskInfo);

    int freeDiskVolume = diskInfo.f_bfree * diskInfo.f_bsize >> 20;

    if(freeDiskVolume < 20)
    {
        int rowCnt = ui->tableViewInfoList->model()->rowCount();
        delInfoRecord(rowCnt - 1);
    }

    if (ui->tableViewInfoList->model()->rowCount() >= 45/* || freeDiskVolume < 20*/)
    {
        int rowCnt = ui->tableViewInfoList->model()->rowCount();
        for (i = 45 - 1; i < rowCnt; i++)
        {
            delInfoRecord(45 - 1);
        }
    }
    if (!isVisible())
    {
        m_newInfoCnt++;

        QSettings settings_config(CONFIG_FILE, QSettings::IniFormat);
        settings_config.setValue("info/new_info_cnt", m_newInfoCnt);

        g_pMainDlg->emitSigSetNotify(2);
    }

    ((Application *)qApp)->setLCDAndTimerOn();
    InfoListAddOne(info_file_name);

    if ((!m_pInfoManDetailDlg->isVisible()/* == NULL*/) && (ui->pushButtonInfoManMainDel->isEnabled()))
    {
        ui->tableViewInfoList->selectRow(0);//lg lg ui->tableViewInfoList->model()->rowCount() - 1);
    }
}
int InfoManMainDialog::read_data_from_file(FILE *fp,const char *type,char *type_name)
{
    char buf[300];
    char *ptr = NULL;
    int i=0;
    if((fp != NULL) && (type != NULL) && (type_name != NULL))
    {
        fgets(buf,sizeof(buf),fp);
        ptr = buf + strlen(type);
        type_name[0]=0;
        while(*ptr && (*ptr != '\n') && (*ptr != '\r'))
        {
            type_name[i] = *ptr++;
            i++;

        }
        type_name[i]=0;
    }
    else
    {
        return -1;
    }
    return 0;
}
/************************************************************
描述：向信息列表控件加入一条记录，加入后该记录为列表控件中的第一条记录
参数：info_file_name - 存储待加入信息的文件的文件名
返回：0 - 成功
	 -1 - 失败
************************************************************/
//接收到新信息可用该函数将该条新信息加入tableViewInfoList, 不用调用ReloadInfoList来重载整个InfoList
int InfoManMainDialog::InfoListAddOne(char *info_file_name)
{
	FILE *fp;
	char filepath[PATH_MAX];
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewInfoList->model();
    QStandardItem *item;
	int rowNum;
	int year, month, day, hour, minute, second, msecond;
	char buf[300], *ptr;
	char from[200], title[200];
	char unread[2];
	char str[80];
	int i;
	//QTextCodec *gk_codec = QTextCodec::codecForName("GBK");

	sprintf(filepath, "/mnt/disk/info/%s", info_file_name);

	fp = fopen(filepath, "rt");
	if (!fp)
	{
		return -1;
	}

	model->insertRow(0);
	rowNum = 0;//model->rowCount();//lg lg

	//QFont font("Century Gothic");
	//font.setPixelSize(18);
	sscanf(info_file_name, "%04d%02d%02d-%02d%02d%02d-%03d", &year, &month, &day,
		   &hour, &minute, &second, &msecond);
	sprintf(str, "%d-%d-%d %d:%02d", year, month, day, hour, minute);
    item = new QStandardItem(str);
	item->setTextAlignment(Qt::AlignCenter);
	//item->setFont(font);
	model->setItem(rowNum, 0, item);

	fgets(buf, sizeof(buf), fp);
	from[0] = 0;
	if (strncmp(buf, "FROM:", strlen("FROM:")) == 0)
	{
		ptr = buf + strlen("FROM:");
		i = 0;
		while (*ptr && (*ptr != '\n') && (*ptr != '\r')) from[i++] = *ptr++;
		from[i] = 0;
	}

	fgets(buf, sizeof(buf), fp);
	if (strncmp(buf, "FROM_IP:", strlen("FROM_IP:")) == 0)
	{
	}

	fgets(buf, sizeof(buf), fp);
	title[0] = 0;
	if (strncmp(buf, "TITLE:", strlen("TITLE:")) == 0)
	{
		ptr = buf + strlen("TITLE:");
		i = 0;
		while (*ptr && (*ptr != '\n') && (*ptr != '\r')) title[i++] = *ptr++;
		title[i] = 0;
	}

	fgets(buf, sizeof(buf), fp);
	if (strncmp(buf, "UNREAD:", strlen("UNREAD:")) == 0)
	{
		ptr = buf + strlen("UNREAD:");
		unread[0] = *ptr;
		unread[1] = 0;
	}
	else
	{
		unread[0] = '0';
		unread[1] = 0;
	}

	fclose(fp);

	QString strTemp = QString::fromUtf8(from);
	int pos = strTemp.indexOf("?");
	if (pos >= 0)
	{
		strTemp = strTemp.left(pos);
	}

	QStringList &addressList = g_pMainDlg->m_pAddressBookDlg->m_addressListNetName;

	for (i = 0; i < addressList.count(); i++)
	{
		QString str = addressList.at(i);

		if (strTemp.length() > str.length())
		{
			if ((strTemp.left(str.length()) == str) && (strTemp.at(str.length()) == QChar('-')))
			{
				strTemp = g_pMainDlg->m_pAddressBookDlg->m_addressListName.at(i) + strTemp.mid(str.length());
				break;
			}
		}
		else if (strTemp == str)
		{
			strTemp = g_pMainDlg->m_pAddressBookDlg->m_addressListName.at(i);
			break;
		}
	}

	if (i >= addressList.count())
	{
		pos = strTemp.indexOf("-");
		//printf("lg pos:%d\n", pos);
		if ((pos != -1) && ((pos != strTemp.length() - 2) || (strTemp.at(strTemp.length() - 1).isDigit())))
		{
			QString strTemp2 = g_strNativeName;//g_strLphoneName[0];

			int pos2;
			pos2 = strTemp2.indexOf("?");
			if (pos2 >= 0) strTemp2 = strTemp2.left(pos2);

			pos2 = strTemp2.indexOf("-");
			//printf("lg pos2:%d\n", pos2);
			if ((pos2 != -1) && ((pos2 != strTemp2.length() - 2) || (strTemp2.at(strTemp2.length() - 1).isDigit())))
			{
				strTemp2 = strTemp2.left(pos2);
				if (strTemp.left(pos) == strTemp2)
				{
					strTemp = strTemp.mid(pos + strlen("-"));
				}
			}
		}
	}

	//font.setPixelSize(20);
	item = new QStandardItem(strTemp);
	item->setTextAlignment(Qt::AlignCenter);
	//item->setFont(font);
	model->setItem(rowNum, 1, item);

    item = new QStandardItem(QString::fromUtf8(title));
	item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	model->setItem(rowNum, 2, item);

	sprintf(buf, "%02d-%03d", second, msecond);
	item = new QStandardItem(QString::fromUtf8(buf));
	item->setTextAlignment(Qt::AlignCenter);
	model->setItem(rowNum, 3, item);

	item = new QStandardItem(info_file_name);
	item->setTextAlignment(Qt::AlignCenter);
	model->setItem(rowNum, 4, item);

	item = new QStandardItem(unread);
	item->setTextAlignment(Qt::AlignCenter);
	model->setItem(rowNum, 5, item);

	if (unread[0] == '1')
	{
		model->item(rowNum, 0)->setIcon(QIcon(":/images/infoman_dialog/unread.png"));
	}
	/*else
	{
		model->item(rowNum, 0)->setIcon(QIcon(":/images/infoman_dialog/read.png"));
	}*/

	////item = new QStandardItem("-1");
	////item->setTextAlignment(Qt::AlignCenter);
	////model->setItem(rowNum, 4, item);

    ui->tableViewInfoList->setRowHeight(rowNum, g_pScaleSrc->get_public_tableviewlist_row_height());

	return 0;
}

/************************************************************
描述：遍历信息文件存储目录下的所有信息文件，将读到的信息加入信息列表控件
参数：无
返回：0 - 成功
	 -1 - 失败
************************************************************/
int InfoManMainDialog::ReloadInfoList()
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewInfoList->model();
	QString strSrc, strTime, strTimeSec;
	int rowSel, rowSelNew;
	DIR *dp;
	struct dirent *dirp;
	int len, i;
	model->setHeaderData(0, Qt::Horizontal, tr("时间"));
	model->setHeaderData(1, Qt::Horizontal, tr("发送者"));
	model->setHeaderData(2, Qt::Horizontal, tr("主题"));
	model->setHeaderData(3, Qt::Horizontal, tr("秒"));
	model->setHeaderData(4, Qt::Horizontal, tr("时间排序"));
	model->setHeaderData(5, Qt::Horizontal, "未读");
	////model->setHeaderData(4, Qt::Horizontal, tr("当前附件序号"));

	rowSel = ui->tableViewInfoList->currentIndex().row();

	if (rowSel >= 0)
	{
		strSrc = model->data(model->index(rowSel, 0)).toString();
		strTime = model->data(model->index(rowSel, 1)).toString();
		strTimeSec = model->data(model->index(rowSel, 2)).toString();
	}

	model->removeRows(0, model->rowCount());

	if ((dp = opendir("/mnt/disk/info")) == NULL)
	{
		setAllButtonsEnabled(this, false);
		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		//Q_UNUSED(okButton);
        okButton->setObjectName("pushButtonMyMessageBoxOK");
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("打开信息存储目录失败"));
		messBox.exec();
		setAllButtonsEnabled(this, true);
		return -1;
	}

	while ((dirp = readdir(dp)) != NULL)
	{
		if (dirp->d_type == DT_DIR)
		{
			continue;
		}

		len = strlen(dirp->d_name);

		/*if (dirp->d_name[len - 1] == '~')
		{
			continue;
		}*/

		if (strcasecmp(&dirp->d_name[len - 4], ".txt") != 0)
		{
			continue;
		}

		InfoListAddOne(dirp->d_name);
	}

	closedir(dp);
	//ui->tableViewInfoList->horizontalHeader()->setSortIndicator(0, Qt::DescendingOrder);
	ui->tableViewInfoList->sortByColumn(4, Qt::DescendingOrder);
	////ui->tableViewInfoList->sortByColumn(0, Qt::DescendingOrder);

	if (model->rowCount() > 45)
	{
		int rowCnt = model->rowCount();
		for (i = 45; i < rowCnt; i++)
		{
			delInfoRecord(45);
		}
	}

	rowSelNew = 0;
	if (rowSel >= 0)
	{
		int i;
		for (i = 0; i < model->rowCount(); i++)
		{
			if ((strSrc == model->data(model->index(i, 0)).toString()) &&
				(strTime == model->data(model->index(i, 1)).toString()) &&
				(strTimeSec == model->data(model->index(i, 2)).toString()))
			{
				rowSelNew = i;
				break;
			}
		}
	}

	ui->tableViewInfoList->selectRow(rowSelNew);

	////setTableViewInfoListColor(TABLE_VIEW_COLOR);

	return 0;
}

/************************************************************
描述：关闭信息列表对话框
参数：无
返回：无
************************************************************/
void InfoManMainDialog::on_pushButtonInfoManMainReturn_pressed()
{
	done(0);
}

/************************************************************
描述：选择上一条信息
参数：无
返回：无
************************************************************/
void InfoManMainDialog::on_pushButtonInfoManMainPrev_pressed()
{
	//QStandardItemModel *model = (QStandardItemModel *)ui->tableViewInfoList->model();
	int row = ui->tableViewInfoList->verticalScrollBar()->value(); //ui->tableViewInfoList->currentIndex().row();

	if (row - TABLE_ROWS_PER_PAGE >= 0)
	{
		ui->tableViewInfoList->selectRow(row - TABLE_ROWS_PER_PAGE);
		ui->tableViewInfoList->verticalScrollBar()->setSliderPosition(row - TABLE_ROWS_PER_PAGE);
	}
	else if (row > 0)
	{
		ui->tableViewInfoList->selectRow(0);
		ui->tableViewInfoList->verticalScrollBar()->setSliderPosition(0);
	}
	/*else if (model->rowCount() > TABLE_ROWS_PER_PAGE)
	{
		ui->tableViewInfoList->selectRow(model->rowCount() - TABLE_ROWS_PER_PAGE);
		ui->tableViewInfoList->verticalScrollBar()->setSliderPosition(model->rowCount() - TABLE_ROWS_PER_PAGE);
	}*/
}

/************************************************************
描述：选择下一条信息
参数：无
返回：无
************************************************************/
void InfoManMainDialog::on_pushButtonInfoManMainNext_pressed()
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewInfoList->model();
	int row = ui->tableViewInfoList->verticalScrollBar()->value(); //ui->tableViewInfoList->currentIndex().row();

	if (row + TABLE_ROWS_PER_PAGE < model->rowCount())
	{
		ui->tableViewInfoList->selectRow(row + TABLE_ROWS_PER_PAGE);
		ui->tableViewInfoList->verticalScrollBar()->setSliderPosition(row + TABLE_ROWS_PER_PAGE);
	}
	/*else
	{
		ui->tableViewInfoList->selectRow(0);
		ui->tableViewInfoList->verticalScrollBar()->setSliderPosition(0);
	}*/
}

/************************************************************
描述：打开当前信息详细内容显示窗口
参数：无
返回：无
************************************************************/
void InfoManMainDialog::on_pushButtonInfoManMainDetail_pressed()
{
	if (m_pInfoManDetailDlg->isVisible()) return;

	if (m_pInfoManDetailDlg->ShowCurrentInfoDetail() != -1)
	{
		m_pInfoManDetailDlg->show();
	}
}

/************************************************************
描述：删除一条信息，包括删除列表控件的记录和相关信息和图片文件
参数：row - 待删除的信息在列表控件中的行号
返回：无
************************************************************/
void InfoManMainDialog::delInfoRecord(int row)
{
	QString strTime;
	QString strTimeSec;
	QStandardItemModel *model;
	char filepath[PATH_MAX];
	char buf[512];
	char attchmnt_filepath[PATH_MAX];
	char attchmnt_filename[PATH_MAX];
	FILE *fp;
	char *ptr;
	int year, month, day, hour, minute, second, msecond;
	int i;

	model = (QStandardItemModel *)ui->tableViewInfoList->model();

	strTime = model->data(model->index(row, 0)).toString();
	strTimeSec = model->data(model->index(row, 3)).toString();
	sscanf(strTime.toUtf8().data(), "%d-%d-%d %d:%02d", &year, &month, &day,
		   &hour, &minute);
	sscanf(strTimeSec.toUtf8().data(), "%02d-%03d", &second, &msecond);
	sprintf(filepath, "/mnt/disk/info/%04d%02d%02d-%02d%02d%02d-%03d.txt", year, month, day,
			hour, minute, second, msecond);

	fp = fopen(filepath, "rt");
	if (!fp) return;

	if (fgets(buf, sizeof(buf) , fp) == NULL) goto jump;
	if (strncmp(buf, "FROM:", strlen("FROM:")) != 0) goto jump;
	if (fgets(buf, sizeof(buf) , fp) == NULL) goto jump;
	if (strncmp(buf, "FROM_IP:", strlen("FROM_IP:")) != 0) goto jump;
    if (fgets(buf, sizeof(buf) , fp) == NULL) goto jump;
    if (strncmp(buf, "TITLE:", strlen("TITLE:")) != 0) goto jump;
    if (fgets(buf, sizeof(buf) , fp) == NULL) goto jump;
    if (strncmp(buf, "UNREAD:", strlen("UNREAD:")) != 0) goto jump;
    if (fgets(buf, sizeof(buf) , fp) == NULL) goto jump;
    if (strncmp(buf, "ATTACHMENT:", strlen("ATTACHMENT:")) != 0)  goto jump;



	while (!feof(fp))
	{
		if (fgets(buf, sizeof(buf), fp) == NULL) break;
		if (strncmp(buf, "BODY:", strlen("BODY:")) == 0) break;
		if (strncmp(buf, "\t", strlen("\t")) != 0) break;

		ptr = buf + strlen("\t");
		i = 0;
		while (*ptr && (*ptr != '\n') && (*ptr != '\r')) attchmnt_filename[i++] = *ptr++;
		attchmnt_filename[i] = 0;

		sprintf(attchmnt_filepath, "/mnt/disk/info/%04d%02d%02d-%02d%02d%02d-%03d-%s", year, month, day,
				hour, minute, second, msecond, attchmnt_filename);

		remove(attchmnt_filepath);
	}

jump:
	fclose(fp);

	remove(filepath);

	model->removeRow(row);
}

/************************************************************
描述：删除当前信息记录和相关信息文件
参数：无
返回：无
************************************************************/
void InfoManMainDialog::on_pushButtonInfoManMainDel_pressed()
{
	int rowSel;

	rowSel = ui->tableViewInfoList->currentIndex().row();
	if (rowSel < 0) return;

	setAllButtonsEnabled(this, false);
	ui->framePanel->setFocus(Qt::TabFocusReason);
	MyMessageBox messBox(this);
	CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
    CommonPushButton *cancelButton = messBox.addButton(QObject::tr("取消"), "pushButtonMyMessageBoxCancel");
	Q_UNUSED(okButton);
	cancelButton->setFocus(Qt::TabFocusReason);
	messBox.setText(tr("确定删除?"));
	messBox.exec();
	setAllButtonsEnabled(this, true);
    ui->pushButtonInfoManMainDel->setFocus(Qt::TabFocusReason);
	if(messBox.clickedButton() == cancelButton)
	{
		return;
	}

	rowSel = ui->tableViewInfoList->currentIndex().row();
	delInfoRecord(rowSel);

	////setTableViewInfoListColor(TABLE_VIEW_COLOR);
}

/************************************************************
描述：信息列表控件双击回调函数
参数：index - 信息列表控件当前项的序号
返回：无
************************************************************/
void InfoManMainDialog::on_tableViewInfoList_doubleClicked(QModelIndex index)
{
	Q_UNUSED(index);

    on_pushButtonInfoManMainDetail_pressed();
}

/************************************************************
描述：设置信息列表控件的背景色为按行号白-非白显示
参数：color - 非白行的背景色
返回：无
************************************************************/
/*void InfoManMainDialog::setTableViewInfoListColor(QColor color)
{
	int i, j;
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewInfoList->model();

	for (i = 0; i < model->rowCount(); i++)
	{
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
描述：窗口显示时，刷新Tab焦点，将主窗口新信息数设为0
参数：无
返回：无
************************************************************/
void InfoManMainDialog::showEvent(QShowEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)parent();

	pMainDlg->showRefreshTabFocus(this);

	if (m_newInfoCnt > 0)
	{
		m_newInfoCnt = 0;

		g_pMainDlg->emitSigSetNotify(2);
		/*QStandardItemModel *model = (QStandardItemModel *)g_pMainDlg->ui->tableViewNotifier->model();
        model->item(5, 1)->setIcon(QIcon(":/images/main/information_none.png"));
        model->item(5, 2)->setText(MainDialog::tr("无未查看信息"));
		*/
	}


	QSettings settings_config(CONFIG_FILE, QSettings::IniFormat);
	settings_config.setValue("info/new_info_cnt", m_newInfoCnt);

	g_pScaleSrc->Backup_Config_File();
	//pMainDlg->getUi()->labelIconInfoCnt->setText("\n\n" + QString::number(m_newInfoCnt));
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void InfoManMainDialog::hideEvent(QHideEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)parent();

	m_pInfoManDetailDlg->hide();
	m_pInfoWriteDlg->hide();

	pMainDlg->hideRefreshTabFocus(this);

	if (pMainDlg->m_pCurActiveDlg == pMainDlg)
	{
		pMainDlg->ui->pushButtonInfoMan->setFocus(Qt::TabFocusReason);
	}
}

/************************************************************
描述：按动信息列表控件排序指示符的回调函数
参数：logicalIndex - 排序的列号
	 order - Qt::DescendingOrder 按降序排序
			 Qt::AscendingOrder 按升序排序
返回：无
************************************************************/
/*void InfoManMainDialog::tvInfoListSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
	Q_UNUSED(logicalIndex);
	Q_UNUSED(order);

	setTableViewInfoListColor(TABLE_VIEW_COLOR);
}
*/

void InfoManMainDialog::on_pushButtonInfoManMainWrite_pressed()
{
	if (m_pInfoWriteDlg->isVisible()) return;

	m_pInfoWriteDlg->ResetState("");
	m_pInfoWriteDlg->show();
	m_pInfoWriteDlg->raise();
}

/************************************************************
描述：删除信息列表控件所有信息记录和所有相关信息文件
参数：无
返回：无
************************************************************/
void InfoManMainDialog::on_pushButtonInfoManMainDelAll_pressed()
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewInfoList->model();

	if (model->rowCount() == 0) return;

	setAllButtonsEnabled(this, false);
	ui->framePanel->setFocus(Qt::TabFocusReason);
	MyMessageBox messBox(this);
	CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
    CommonPushButton *cancelButton = messBox.addButton(QObject::tr("取消"), "pushButtonMyMessageBoxCancel");
	Q_UNUSED(okButton);
	cancelButton->setFocus(Qt::TabFocusReason);
	messBox.setText(tr("确定删除所有记录?"));
	messBox.exec();
	setAllButtonsEnabled(this, true);
    ui->pushButtonInfoManMainDelAll->setFocus(Qt::TabFocusReason);
	if(messBox.clickedButton() == cancelButton)
	{
		return;
	}

	model->removeRows(0, model->rowCount());

	system("/bin/rm /mnt/disk/info/*.*");
}
