#include "securityabnormaldialog.h"
#include "ui_securityabnormaldialog.h"
#include <QStandardItemModel>
#include "securitymaindialog.h"
#include <QSettings>
#include <QTextCodec>
#include <QTimer>
//#include "callcenterdialog.h"
//#include "helpdialog.h"
//#include "terminalhelpmaindialog.h"
#include "maindialog.h"
#include "adddefensedialog.h"
#include "mediastreamer2/mediastream.h"
#include "securitypassworddialog.h"

extern MainDialog *g_pMainDlg;

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
extern bool g_Digital_Channel_Flag;				// 标识数字通道的变量
/************************************************************
  该文件为修改设防状态时防区异常显示对话框主程序
************************************************************/

/************************************************************
描述：防区异常显示对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
SecurityAbnormalDialog::SecurityAbnormalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecurityAbnormalDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

	int x,y,w,h;
	g_pScaleSrc->get_info_framepanel_geometry(x,y,w,h);
	ui->framePanel->setGeometry(x,y,w,h);

	QStandardItemModel *model = new QStandardItemModel(0, 5);

	g_pScaleSrc->get_tabviewinfolist_geometry(x,y,w,h);
	ui->tableViewAbnormalList->setGeometry(x,y,w,h);

	ui->tableViewAbnormalList->setModel(model);
	ui->tableViewAbnormalList->verticalHeader()->hide();
	ui->tableViewAbnormalList->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableViewAbnormalList->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableViewAbnormalList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableViewAbnormalList->setSortingEnabled(false);
	ui->tableViewAbnormalList->setShowGrid(false);
    ui->tableViewAbnormalList->horizontalHeader()->setHighlightSections(false);// add by lidh 20141119 点击表时不对表头行光亮

	g_pScaleSrc->get_info_tableViewAbnormalList_width(x,y,w,h);
	ui->tableViewAbnormalList->setColumnWidth(0, x);
	ui->tableViewAbnormalList->setColumnWidth(1, y);
	ui->tableViewAbnormalList->setColumnWidth(2, w);
	ui->tableViewAbnormalList->setColumnWidth(3, h);

	ui->tableViewAbnormalList->setColumnWidth(4, 50);
	ui->tableViewAbnormalList->setColumnHidden(4, true);

	ui->tableViewAbnormalList->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
	ui->tableViewAbnormalList->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
	ui->tableViewAbnormalList->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
	ui->tableViewAbnormalList->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);

	ui->tableViewAbnormalList->setIconSize(QSize(g_pScaleSrc->get_w_value(45), g_pScaleSrc->get_h_value(26)));

	g_pScaleSrc->get_info_label_geometry(x,y,w,h);
    ui->label_title->setGeometry(x,y,w,h);

    int res = g_pScaleSrc->get_screen_version();
    if 	( res == 2 )  // 1024x600
    {
        ui->pushButtonSecAbnormalOut->setGeometry(QRect(764, 100, 230, 80));
        ui->pushButtonSecAbnormalHome->setGeometry(QRect(764, 180, 230, 80));
        ui->pushButtonSecAbnormalDisArm->setGeometry(QRect(764, 260, 230, 80));
        ui->pushButtonSecAbnormalPrev->setGeometry(QRect(764, 340, 230, 80));
        ui->pushButtonSecAbnormalNext->setGeometry(QRect(764, 420, 230, 80));
        ui->pushButtonSecAbnormalReturn->setGeometry(QRect(764, 500, 230, 80));
    }
    else if ( res == 3 )  // 1280x800
    {
        ui->pushButtonSecAbnormalOut->setGeometry(QRect(959, 137, 279, 99));
        ui->pushButtonSecAbnormalHome->setGeometry(QRect(959, 245, 279, 99));
        ui->pushButtonSecAbnormalDisArm->setGeometry(QRect(959, 353, 279, 99));
        ui->pushButtonSecAbnormalPrev->setGeometry(QRect(959, 461, 279, 99));
        ui->pushButtonSecAbnormalNext->setGeometry(QRect(959, 569, 279, 99));
        ui->pushButtonSecAbnormalReturn->setGeometry(QRect(959, 677, 279, 99));
    }
}

/************************************************************
描述：防区异常显示对话框析构函数
参数：无
返回：无
************************************************************/
SecurityAbnormalDialog::~SecurityAbnormalDialog()
{
	delete ui;
}

void SecurityAbnormalDialog::resetState()
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewAbnormalList->model();
	model->setHeaderData(0, Qt::Horizontal, tr("号码"));
	model->setHeaderData(1, Qt::Horizontal, tr("防区名称"));
	model->setHeaderData(2, Qt::Horizontal, tr("安装位置"));
	model->setHeaderData(3, Qt::Horizontal, tr("备注"));
	model->setHeaderData(4, Qt::Horizontal, tr("防区类型"));

	SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();
	int allDefenseStatus[56 + 56]; //进入各个状态时，对应防区的异常状态
	int allDefenseProperty[56 + 56]; //各个防区的防区属性
	int regAbnormalDefense[56 + 56];

	memset(regAbnormalDefense, 0, sizeof(regAbnormalDefense));
	getAllDefenseStatus(allDefenseStatus);
	getRegisteredDefenseProperty(allDefenseProperty);

	int allProperty[3];
	int propertyCnt = 3;
	allProperty[0] = PROPERTY_NORMAL;
	allProperty[1] = PROPERTY_GOOUT;
	allProperty[2] = PROPERTY_ATHOME;

	//ui->pushButtonSecAbnormalOut->setEnabled(true);
	//ui->pushButtonSecAbnormalHome->setEnabled(true);
	//ui->pushButtonSecAbnormalDisArm->setEnabled(true);

	switch (pSecurityMainDlg->m_securityState)
	{
	case STATUS_DISARM:
		propertyCnt = 1;
		ui->pushButtonSecAbnormalDisArm->setFocus(Qt::TabFocusReason);
		break;
	case STATUS_ATHOME:
		propertyCnt = 2;
		ui->pushButtonSecAbnormalHome->setFocus(Qt::TabFocusReason);
		break;
	case STATUS_GOOUT:
		ui->pushButtonSecAbnormalOut->setFocus(Qt::TabFocusReason);
		propertyCnt = 3;
		break;
	}

	int i, j;
	for (i = 0; i < pSecurityMainDlg->m_wireDefenseCnt; i++)
	{
		if (allDefenseStatus[i] == 0)
		{
			for (j = 0; j < propertyCnt; j++)
			{
				if (allDefenseProperty[i] == allProperty[j])
				{
					regAbnormalDefense[i] = 1;
					break;
				}
			}
		}
	}

	for (i = 0; i < pSecurityMainDlg->m_wirelessDefenseCnt; i++)
	{
		if (allDefenseStatus[i + 56] == 1)
		{
			for (j = 0; j < propertyCnt; j++)
			{
				if (allDefenseProperty[i + 56] == allProperty[j])
				{
					regAbnormalDefense[i + 56] = 1;
					break;
				}
			}
		}
	}

	model->removeRows(0, model->rowCount());

	QSettings settings("/mnt/disk/config/defense.conf", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF8"));

	QFont font("Century Gothic");
	font.setPixelSize(24);

	int k;
	for (j = 0; j < 2; j++)
	{
		int off, cnt;
		if (j == 0)
		{
			off = 0;
			cnt = pSecurityMainDlg->m_wireDefenseCnt;
		}
		else
		{
			off = 56;
			cnt = pSecurityMainDlg->m_wirelessDefenseCnt;
		}

		for (i = 0; i < cnt; i++)
		{
			int row;
			QString strNumber;
			QString strName;
			QString strLocation;
			QString strProperty;

			int number;
			int property;
			int type;

			if (j == 0)
			{
				number = pSecurityMainDlg->m_allDefenseNumber[i];
				strNumber = settings.value("wiredefense" + QString::number(number) + "/number").toString();
				type = settings.value("wiredefense" + QString::number(number) + "/type").toInt();
				strName = QObject::tr(g_strType[type]);
				strLocation = settings.value("wiredefense" + QString::number(number) + "/location").toString();

				property = settings.value("wiredefense" + QString::number(number) + "/property", "-1").toInt();
				if (property != -1) strProperty = QObject::tr(g_strProperty[property]);
			}
			else
			{
				number = pSecurityMainDlg->m_allDefenseNumber[i + 56];
				strNumber = settings.value("wirelessdefense" + QString::number(number) + "/number").toString();
				type = settings.value("wirelessdefense" + QString::number(number) + "/type").toInt();
				strName = QObject::tr(g_strType[type]);
				strLocation = settings.value("wirelessdefense" + QString::number(number) + "/location").toString();

				property = settings.value("wirelessdefense" + QString::number(number) + "/property", "-1").toInt();
				if (property != -1) strProperty = QObject::tr(g_strProperty[property]);
			}

			QStandardItem *item[5];
			QString strTmp;
			//要改成Icon
			switch (j * 2 + regAbnormalDefense[i + off])
			{
			case 0:
				strTmp = ":/images/security/wire.png";
				break;
			case 1:
				strTmp = ":/images/security/wire_abnormal.png";
				break;
			case 2:
				strTmp = ":/images/security/wireless.png";
				break;
			case 3:
				strTmp = ":/images/security/wireless_abnormal.png";
				break;
			}

			item[0] = new QStandardItem(" " + strNumber);
			item[0]->setFont(font);
			item[0]->setIcon(QIcon(strTmp));
			item[1] = new QStandardItem(strName);
			item[2] = new QStandardItem(strLocation);
			item[3]=  new QStandardItem(strProperty + tr("防区"));
			item[4]=  new QStandardItem(QString::number(j));

			row = model->rowCount();
			for (k = 0; k < 5; k++)
			{
				if (k > 0)
				{
					item[k]->setTextAlignment(Qt::AlignCenter);
				}

				model->setItem(row, k, item[k]);
			}

			ui->tableViewAbnormalList->setRowHeight(row, g_pScaleSrc->get_public_tableviewlist_row_height());
		}
	}

	model->sort(0, Qt::AscendingOrder);
	model->sort(4, Qt::AscendingOrder);

	ui->tableViewAbnormalList->selectRow(0);
}

/************************************************************
描述：选择异常列表控件中的上一页异常信息
参数：无
返回：无
************************************************************/
void SecurityAbnormalDialog::on_pushButtonSecAbnormalPrev_pressed()
{
	//QStandardItemModel *model = (QStandardItemModel *)ui->tableViewAbnormalList->model();
	int row = ui->tableViewAbnormalList->verticalScrollBar()->value();

	if (row - TABLE_ROWS_PER_PAGE >= 0)
	{
		ui->tableViewAbnormalList->selectRow(row - TABLE_ROWS_PER_PAGE);
		ui->tableViewAbnormalList->verticalScrollBar()->setSliderPosition(row - TABLE_ROWS_PER_PAGE);
	}
	else if (row > 0)
	{
		ui->tableViewAbnormalList->selectRow(0);
		ui->tableViewAbnormalList->verticalScrollBar()->setSliderPosition(0);
	}
	/*else if (model->rowCount() > TABLE_ROWS_PER_PAGE)
	{
		ui->tableViewAbnormalList->selectRow(model->rowCount() - TABLE_ROWS_PER_PAGE);
		ui->tableViewAbnormalList->verticalScrollBar()->setSliderPosition(model->rowCount() - TABLE_ROWS_PER_PAGE);
	}*/
}

/************************************************************
描述：选择异常列表控件中的下一页异常信息
参数：无
返回：无
************************************************************/
void SecurityAbnormalDialog::on_pushButtonSecAbnormalNext_pressed()
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewAbnormalList->model();
	int row = ui->tableViewAbnormalList->verticalScrollBar()->value();

	if (row + TABLE_ROWS_PER_PAGE < model->rowCount())
	{
		ui->tableViewAbnormalList->selectRow(row + TABLE_ROWS_PER_PAGE);
		ui->tableViewAbnormalList->verticalScrollBar()->setSliderPosition(row + TABLE_ROWS_PER_PAGE);
	}
	/*else
	{
		ui->tableViewAbnormalList->selectRow(0);
		ui->tableViewAbnormalList->verticalScrollBar()->setSliderPosition(0);
	}*/
}

/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void SecurityAbnormalDialog::showEvent(QShowEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());

	pMainDlg->showRefreshTabFocus(this);

    //ui->pushButtonSecAbnormalReturn->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void SecurityAbnormalDialog::hideEvent(QHideEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());

	pMainDlg->hideRefreshTabFocus(this);
}

/************************************************************
描述：获取所有已设防区的类型
参数：无
返回：无
************************************************************/
void SecurityAbnormalDialog::getRegisteredDefenseProperty(int *pAllDefenseProperty)
{
	int i ;
	memset(pAllDefenseProperty, 0, (56 + 56) * sizeof(int));

	QSettings settings("/mnt/disk/config/defense.conf", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF8"));

	SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();

	for (i = 0; i < pSecurityMainDlg->m_wireDefenseCnt; i++)
	{
		pAllDefenseProperty[i] = settings.value("wiredefense" + QString::number(pSecurityMainDlg->m_allDefenseNumber[i]) + "/property", QString::number(PROPERTY_GOOUT)).toInt();
	}

	for (i = 0; i < pSecurityMainDlg->m_wirelessDefenseCnt; i++)
	{
		pAllDefenseProperty[i + 56] = settings.value("wirelessdefense" + QString::number(pSecurityMainDlg->m_allDefenseNumber[i + 56]) + "/property", QString::number(PROPERTY_GOOUT)).toInt();
	}
}

/************************************************************
描述：获取所有已设防区的状态
参数：无
返回：无
************************************************************/
void SecurityAbnormalDialog::getAllDefenseStatus(int *pAllDefenseStatus)
{
	int i ;
	memset(pAllDefenseStatus, 0, (56 + 56) * sizeof(int));

	QSettings settings("/mnt/disk/config/defense", QSettings::IniFormat);
	SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();
	for(i = 0; i < pSecurityMainDlg->m_wireDefenseCnt; i++)
	{
		int tmp;
		tmp = settings.value("wiredefense/defense" + QString::number(pSecurityMainDlg->m_allDefenseNumber[i]), "1").toInt();
		pAllDefenseStatus[i] = tmp;
	}

	for (i = 0; i < pSecurityMainDlg->m_wirelessDefenseCnt; i++)
	{
		int tmp;
		tmp = settings.value("wirelessdefense/defense" + QString::number(pSecurityMainDlg->m_allDefenseNumber[i + 56]), "0").toInt();
		pAllDefenseStatus[i + 56] = tmp;
	}
}

void SecurityAbnormalDialog::on_pushButtonSecAbnormalReturn_pressed()
{
	hide();
}

void SecurityAbnormalDialog::on_pushButtonSecAbnormalOut_pressed()
{
	printf("on_pushButtonSecAbnormalOut_pressed\n");
	SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();

	if (pSecurityMainDlg->m_securityState == STATUS_GOOUT) return;

	if (g_pMainDlg->m_inputMethodMode == 0)
	{
		if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
			usleep(50000);;
		playWaveFile((char *)"wav/goout-en.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
	}
	else
	{
		if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
			usleep(50000);;
		playWaveFile((char *)"wav/goout.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
	}

	pSecurityMainDlg->setSecurityState(STATUS_GOOUT);
    pSecurityMainDlg->emitsigSetSecurityStatueToPhone(STATUS_GOOUT);
    if(pSecurityMainDlg->m_pUploadSetSecurityStateThread->m_bRun)
    {
        pSecurityMainDlg->m_pUploadSetSecurityStateThread->m_bRun = false;
        pSecurityMainDlg->m_pUploadSetSecurityStateThread->quit();
        pSecurityMainDlg->m_pUploadSetSecurityStateThread->wait();
    }
    pSecurityMainDlg->m_pUploadSetSecurityStateThread->start();
    resetState();
}

void SecurityAbnormalDialog::on_pushButtonSecAbnormalHome_pressed()
{
	SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();

	if (pSecurityMainDlg->m_securityState == STATUS_ATHOME) return;

	printf("lg pSecurityMainDlg->m_securityState:%d\n", pSecurityMainDlg->m_securityState);
	if (pSecurityMainDlg->m_securityState == STATUS_GOOUT)
	{
		//pSecurityMainDlg->m_pSecurityPasswordDlg->m_goingSecurityState = STATUS_ATHOME;
		pSecurityMainDlg->m_pSecurityPasswordDlg->resetState(STATUS_ATHOME, 0);
		pSecurityMainDlg->m_pSecurityPasswordDlg->show();
		pSecurityMainDlg->m_pSecurityPasswordDlg->raise();

		return;

		/*int ret;

		m_pSubDlg = (QDialog*)new SecurityPasswordDialog(this);
		ret = m_pSubDlg->exec();
		delete m_pSubDlg;
		m_pSubDlg = NULL;

		if (ret != RET_PASSWORDOK) return;
		*/
	}

	if (g_pMainDlg->m_inputMethodMode == 0)
	{
		if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
			usleep(50000);;
		playWaveFile((char *)"wav/athome-en.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
	}
	else
	{
		if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
			usleep(50000);;
		playWaveFile((char *)"wav/athome.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
	}

	pSecurityMainDlg->setSecurityState(STATUS_ATHOME);
    pSecurityMainDlg->emitsigSetSecurityStatueToPhone(STATUS_ATHOME);
    if(pSecurityMainDlg->m_pUploadSetSecurityStateThread->m_bRun)
    {
        pSecurityMainDlg->m_pUploadSetSecurityStateThread->m_bRun = false;
        pSecurityMainDlg->m_pUploadSetSecurityStateThread->quit();
        pSecurityMainDlg->m_pUploadSetSecurityStateThread->wait();
    }
    pSecurityMainDlg->m_pUploadSetSecurityStateThread->start();
	resetState();
}

void SecurityAbnormalDialog::on_pushButtonSecAbnormalDisArm_pressed()
{
	SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();
	//pSecurityMainDlg->m_pSecurityPasswordDlg->m_goingSecurityState = STATUS_DISARM;

	if (pSecurityMainDlg->m_securityState == STATUS_DISARM) return;

	pSecurityMainDlg->m_pSecurityPasswordDlg->resetState(STATUS_DISARM, 0);
	pSecurityMainDlg->m_pSecurityPasswordDlg->show();
	pSecurityMainDlg->m_pSecurityPasswordDlg->raise();

	return;

	/*SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();

	int ret;
	m_pSubDlg = (QDialog*)new SecurityPasswordDialog(this);
	ret = m_pSubDlg->exec();
	delete m_pSubDlg;
	m_pSubDlg = NULL;

	if (ret != RET_PASSWORDOK) return;

	if (g_pMainDlg->m_inputMethodMode == 0)
	{
		playWaveFile((char *)"wav/disarm-en.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
	}
	else
	{
		playWaveFile((char *)"wav/disarm.wav", g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
	}

	pSecurityMainDlg->setSecurityState(STATUS_DISARM);
	resetState();*/
}
