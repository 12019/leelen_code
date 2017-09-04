#include "monlistdialog.h"
#include "ui_monlistdialog.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include "maindialog.h"
#include "ui_mondialog.h"
#include <global.h>

#define MAX_ROWS		5
#define MAX_COLS		3

extern Kevin::Global  *g_pScaleSrc;
extern MainDialog *g_pMainDlg;
extern char g_strLphoneName[3][50];

MonListDialog::MonListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MonListDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	QStandardItemModel *model = new QStandardItemModel(MAX_ROWS, MAX_COLS);

	ui->tableViewMonList->setModel(model);
	ui->tableViewMonList->horizontalHeader()->hide();
	ui->tableViewMonList->verticalHeader()->hide();
	ui->tableViewMonList->setSelectionBehavior(QAbstractItemView::SelectItems);
	ui->tableViewMonList->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableViewMonList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableViewMonList->setSortingEnabled(false);//true); //lg lg
	ui->tableViewMonList->setShowGrid(false);
	ui->tableViewMonList->setFocusPolicy(Qt::NoFocus);


	if ( g_pScaleSrc->get_screen_version() == 2 )
	{
		resize(524,328);
		ui->frameMonList->setGeometry(QRect(20, 42, 484, 174));
		ui->pushButtonMonListCancel->setGeometry(QRect(186, 232, 150, 60));
		ui->tableViewMonList->setGeometry(QRect(50, 52, 454, 174));
	}
	else if ( g_pScaleSrc->get_screen_version() == 3 )
	{
		resize(700,500);
		ui->frameMonList->setGeometry(QRect(30, 60, 641, 311));
		ui->pushButtonMonListCancel->setGeometry(QRect(230, 400, 195, 70));
		ui->tableViewMonList->setGeometry(QRect(50, 60, 621, 311));
	}

	reloadList();
}
	
int MonListDialog::get_max_rows()
{
	return MAX_ROWS;
}

MonListDialog::~MonListDialog()
{
    	delete ui;
}

int MonListDialog::reloadList()
{
	m_monListName.clear();
	m_monListNetName.clear();

	int i;
	for (i = 0; i < 4; i++)
	{
//		m_monListName += tr("本地摄像头") + QString::number((i == 0) ? (i + 1) : (i + 2));
		m_monListName += tr("本地摄像头") + QString::number(i + 1);
	}

//	for (i = 0; i < 4; i++)
//	{
//		m_monListName += tr("门铃摄像头") + QString::number(i + 1);
//	}

	QSettings settings("/mnt/disk/config/mon_list", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	settings.beginGroup("door_machine");
	QStringList keys = settings.childKeys();

	//m_monListName比m_monListNetName多8项

    /*for (i = 0; i < 1; i++)
	{
		m_monListName += tr("网络摄像头") + QString::number(i + 1);
    }*/

	m_doorMachineCnt = keys.count();
	for (i = 0; i < keys.count(); i++)
	{
		QString key = keys.at(i);
		m_monListName += key.mid(2);
		m_monListNetName += settings.value(key, "").toString();
	}

	settings.endGroup();

	if (m_monListNetName.count() > 0)
	{
		strcpy(g_strLphoneName[0], m_monListNetName.at(0).toLatin1().data());
	}

	QSettings settings2("/mnt/disk/config/ip_camera", QSettings::IniFormat);
	settings2.setIniCodec(QTextCodec::codecForName("UTF-8"));

	settings2.beginGroup("ip_camera");
	keys = settings2.childKeys();

	m_ipCameraCnt = keys.count();
	for (i = 0; i < keys.count(); i++)
	{
		QString key = keys.at(i);
		m_monListName += key.mid(2);
		m_monListNetName += settings2.value(key, "").toString();
	}

	settings2.endGroup();

	/*m_doorMachineCnt = settings.value("monitor/door_machine_cnt", "0").toInt();
	m_wallMachineCnt = settings.value("monitor/wall_machine_cnt", "0").toInt();
	QString strMonList[3 + 4 + m_doorMachineCnt + m_wallMachineCnt];
	m_pMonListName = new QString[m_doorMachineCnt + m_wallMachineCnt];
	m_pMonListNetName = new QString[m_doorMachineCnt + m_wallMachineCnt];

	i = 0;
	for (j = 0; j < m_doorMachineCnt; j++, i++)
	{
		strMonList[i] = settings.value("monitor/door_machine_name" +  QString::number(j + 1), "").toString();
		m_pMonListName[j] = strMonList[i];
		m_pMonListNetName[j] = settings.value("monitor/door_machine_net_name" +  QString::number(j + 1), "").toString();
	}

	for (j = 0; j < m_wallMachineCnt; j++, i++)
	{
		strMonList[i] = settings.value("monitor/wall_machine_name" +  QString::number(j + 1), "").toString();
		m_pMonListName[j + m_doorMachineCnt] = strMonList[i];
		m_pMonListNetName[j + m_doorMachineCnt] = settings.value("monitor/wall_machine_net_name" +  QString::number(j + 1), "").toString();
	}
	*/

	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewMonList->model();
	QStandardItem *item;
	model->removeRows(0, model->rowCount());
	for (i = 0; i < m_monListName.count(); i++)
	{
		item = new QStandardItem(m_monListName.at(i));
		model->setItem(i % MAX_ROWS, i / MAX_ROWS, item);
	}

	for (i = 0; i < MAX_ROWS; i++)
	{
		ui->tableViewMonList->setRowHeight(i, g_pScaleSrc->get_public_tableviewlist_row_height());
	}

	for (i = 0; i < MAX_COLS; i++)
	{
		if ( g_pScaleSrc->get_screen_version() == 3 )
			ui->tableViewMonList->setColumnWidth(i, 165);
		else
			ui->tableViewMonList->setColumnWidth(i, 150);
	}

	return 0;
}


/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void MonListDialog::showEvent(QShowEvent *)
{
	int width = this->width();
	int tmp = width - 87/*50*/ * 2; //left + right
	tmp = tmp % 33/*22*/;
	if (tmp)
	{
		width += 33/*22*/ - tmp;
	}

	int height = (g_pScaleSrc->get_public_tableviewlist_row_height() * MAX_ROWS) + 4 + 20 + 27/*lg*/;

	ui->tableViewMonList->setEnabled(true);

	ui->tableViewMonList->setFixedHeight(height - 20);
	ui->tableViewMonList->move(width/2 - ui->frameMonList->width()/2 + 30, 44/*22*/ + 20 + 10/*lg*/);

	ui->frameMonList->setFixedHeight(height);
	ui->frameMonList->move(width/2 - ui->frameMonList->width()/2, 44/*22*/ + 20);

	ui->pushButtonMonListCancel->move(width/2 - ui->pushButtonMonListCancel->width()/2,
			44/*22*/ + 20 + height + 10);

	height += 44/*22*/ + 20 + /**/ 10 + 45 + 20 + 8 + 10/*lg*/;
	setFixedSize(width, height);

	QSize sz = QApplication::desktop()->size();
	move(sz.width()/2 - width/2, sz.height()/2 - height/2 - 6 /*handle_height*/);

	//lg 12.22
	//g_pMainDlg->showRefreshTabFocus(this);
	//lg

	ui->pushButtonMonListCancel->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void MonListDialog::hideEvent(QHideEvent *)
{
    //lg 12.22
    //g_pMainDlg->hideRefreshTabFocus(this);
    //lg
}

void MonListDialog::on_pushButtonMonListCancel_pressed()
{
	if (!isVisible()) return;

	hide();

	((MonDialog *)parent())->ui->pushButtonMonDial->setEnabled(((MonDialog *)parent())->m_strMonAddr != "");
	((MonDialog *)parent())->ui->pushButtonMonList->setFocus(Qt::TabFocusReason);
}

void MonListDialog::on_tableViewMonList_pressed(QModelIndex index)
{
	Q_UNUSED(index);

    //printf("lg MonListDialog::on_tableViewMonList_pressed:%d-%d %d\n",
    //	   index.row(), index.column(), m_monListName.count());

	int pos = index.row() + index.column() * MAX_ROWS;
	if (pos >= m_monListName.count()) return;

	ui->tableViewMonList->setEnabled(false);

	if (pos >= 4 + m_doorMachineCnt && pos < 4 + m_doorMachineCnt + m_ipCameraCnt)
		((MonDialog *)parent())->m_pIPCTimer->start(500);
	else
		((MonDialog *)parent())->m_pTimer->start(500);
}

/*void MonListDialog::slotMonListPressed()
{
	m_pTimer->stop();

	int row = ui->tableViewMonList->currentIndex().row();
	int col = ui->tableViewMonList->currentIndex().column();
	if ((row < 0) || (col < 0)) return;

	hide();
}
*/
