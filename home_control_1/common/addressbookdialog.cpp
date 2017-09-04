#include "addressbookdialog.h"
#include "ui_addressbookdialog.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include "maindialog.h"
#include "ui_ictdialog.h"
#include "ui_infowritedialog.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

#define MAX_ROWS		5
#define MAX_COLS		3

extern MainDialog *g_pMainDlg;
extern char g_strLphoneName[3][50];

AddressBookDialog::AddressBookDialog(QWidget *parent) :
    QDialog(parent),
	ui(new Ui::AddressBookDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
    }

	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(slotAddressListPressed()));

	QStandardItemModel *model = new QStandardItemModel(MAX_ROWS, MAX_COLS);

	ui->tableViewAddressList->setModel(model);
	ui->tableViewAddressList->horizontalHeader()->hide();
	ui->tableViewAddressList->verticalHeader()->hide();
	ui->tableViewAddressList->setSelectionBehavior(QAbstractItemView::SelectItems);
	ui->tableViewAddressList->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableViewAddressList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableViewAddressList->setSortingEnabled(false);//true); //lg lg
    ui->tableViewAddressList->setShowGrid(false);
	ui->tableViewAddressList->setFocusPolicy(Qt::NoFocus);

    if ( g_pScaleSrc->get_screen_version() == 2 )
    {
        resize(524,328);
        ui->frameAddressList->setGeometry(QRect(20, 42, 484, 174));
        ui->pushButtonAddressBookCancel->setGeometry(QRect(186, 232, 150, 60));
        ui->tableViewAddressList->setGeometry(QRect(50, 52, 454, 174));
    }
    else if ( g_pScaleSrc->get_screen_version() == 3 )
    {
        resize(700,500);
        ui->frameAddressList->setGeometry(QRect(30, 60, 641, 311));
        ui->pushButtonAddressBookCancel->setGeometry(QRect(230, 400, 195, 70));
        ui->tableViewAddressList->setGeometry(QRect(50, 60, 621, 311));
    }

	reloadList();
}

AddressBookDialog::~AddressBookDialog()
{
	delete m_pTimer;

	delete ui;
}

int AddressBookDialog::reloadList()
{
	int i;

	QSettings settings("/mnt/disk/config/address_book", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	settings.beginGroup("address");
	QStringList keys = settings.childKeys();

	m_addressListName.clear();
	m_addressListNetName.clear();

	for (i = 0; i < keys.count(); i++)
	{
		QString key = keys.at(i);
		m_addressListName += key.mid(2);
		m_addressListNetName += settings.value(key, "").toString();
	}

	settings.endGroup();

	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewAddressList->model();
	QStandardItem *item;
	model->removeRows(0, model->rowCount());
	for (i = 0; i < m_addressListName.count(); i++)
	{
		item = new QStandardItem(m_addressListName.at(i));
		model->setItem(i % MAX_ROWS, i / MAX_ROWS, item);
	}

	for (i = 0; i < MAX_ROWS; i++)
	{
        ui->tableViewAddressList->setRowHeight(i, (g_pScaleSrc->get_public_tableviewlist_row_height()));
	}

	for (i = 0; i < MAX_COLS; i++)
	{
        if ( g_pScaleSrc->get_screen_version() == 3 )
            ui->tableViewAddressList->setColumnWidth(i, 165);
        else
            ui->tableViewAddressList->setColumnWidth(i, 150);
	}

	return 0;
}


/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void AddressBookDialog::showEvent(QShowEvent *)
{
	int width = this->width();
	int tmp = width - 87/*50*/ * 2; //left + right
	tmp = tmp % 33/*22*/;
	if (tmp)
	{
		width += 33/*22*/ - tmp;
	}

    int height = (g_pScaleSrc->get_public_tableviewlist_row_height()) * MAX_ROWS + 4 + 20/*lg*/;

	ui->tableViewAddressList->setFixedHeight(height - 20);
	ui->tableViewAddressList->move(width/2 - ui->frameAddressList->width()/2 + 30, 44/*22*/ + 20 + 10/*lg*/);

	ui->frameAddressList->setFixedHeight(height);
	ui->frameAddressList->move(width/2 - ui->frameAddressList->width()/2, 44/*22*/ + 20);

    ui->pushButtonAddressBookCancel->move(width/2 - ui->pushButtonAddressBookCancel->width()/2,
							   44/*22*/ + 20 + height + 10);

    height += 44/*22*/ + 20 + /**/ 10 + 45 + 20 + 8 + 10/*lg*/;

	setFixedSize(width, height);

	QSize sz = QApplication::desktop()->size();
	move(sz.width()/2 - width/2, sz.height()/2 - height/2 - 6 /*handle_height*/);

    //lg 12.22
    //g_pMainDlg->showRefreshTabFocus(this);
    //lg

    ui->pushButtonAddressBookCancel->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void AddressBookDialog::hideEvent(QHideEvent *)
{
    //lg 12.22
    //g_pMainDlg->hideRefreshTabFocus(this);
    //lg

	if (g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->isVisible())
	{
		g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->Enable_Buttons();
		g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->ui->pushButtonIctAddressBook->setFocus(Qt::TabFocusReason);
	}
	else
	{
		g_pMainDlg->m_pInfoManMainDlg->m_pInfoWriteDlg->ui->pushButtonInfoWriteAddress->setEnabled(true);
		g_pMainDlg->m_pInfoManMainDlg->m_pInfoWriteDlg->ui->pushButtonInfoWriteAddress->setFocus(Qt::TabFocusReason);
	}
}

void AddressBookDialog::on_pushButtonAddressBookCancel_pressed()
{
	if (!isVisible()) return;

	hide();

	/*if (g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->isVisible())
	{
		g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->ui->pushButtonIctAddressBook->setEnabled(true);
	}
	else if (g_pMainDlg->m_pInfoManMainDlg->m_pInfoWriteDlg->isVisible())
	{
        g_pMainDlg->m_pInfoManMainDlg->m_pInfoWriteDlg->ui->pushButtonInfoWriteAddress->setEnabled(true);
	}*/
}

void AddressBookDialog::on_tableViewAddressList_pressed(QModelIndex index)
{
	Q_UNUSED(index);

	m_pTimer->start(500);
}

void AddressBookDialog::slotAddressListPressed()
{
	m_pTimer->stop();

	int row = ui->tableViewAddressList->currentIndex().row();
	int col = ui->tableViewAddressList->currentIndex().column();
	if ((row < 0) || (col < 0)) return;

	int indx = row + col * MAX_ROWS;
	if (indx >= m_addressListNetName.count()) return;

	hide();

	if (g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->isVisible())
	{
		//g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->ui->pushButtonIctAddressBook->setEnabled(true);

		g_pMainDlg->m_pVoipMainDlg->m_pIctDlg->ui->lineEditDisplay->setText(m_addressListNetName.at(indx));
	}
	else if (g_pMainDlg->m_pInfoManMainDlg->m_pInfoWriteDlg->isVisible())
	{
        	//g_pMainDlg->m_pInfoManMainDlg->m_pInfoWriteDlg->ui->pushButtonInfoWriteAddress->setEnabled(true);

		g_pMainDlg->m_pInfoManMainDlg->m_pInfoWriteDlg->ui->lineEditReceiverName->setText(m_addressListNetName.at(indx));
	}
}
