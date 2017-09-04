#include "mstdialog.h"
#include "ui_mstdialog.h"
#include "lpcore.h"
#include "command.h"
#include "qconfig.h"
#include "mediastreamer2/mediastream.h"
#include "mediastreamer2/msvideo.h"
#include "data_types.h"
#include <QStringListModel>
#include "voipmaindialog.h"
#include "exosip_main.h"
#include "maindialog.h"
#include "application.h"
#include "maindialog.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

extern LinphoneCore *lphone;
extern eXosip_element_t *eXosipElement;
extern MainDialog *g_pMainDlg;

extern int g_bIstOrAlertBusy;

MstDialog::MstDialog(QWidget *parent) :
	QDialog(parent),
    ui(new Ui::MstDialog)
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

	g_pScaleSrc->get_tabviewinfolist_geometry(x,y,w,h);
	ui->tableViewMstList->setGeometry(x,y,w,h);

	g_pScaleSrc->get_info_label_geometry(x,y,w,h);
	ui->label_2->setGeometry(x,y,w,h);

    RESCALESCREEN(ui->pushButtonMstPrev)
    RESCALESCREEN(ui->pushButtonMstNext)
    RESCALESCREEN(ui->pushButtonMstHangUp)
    RESCALESCREEN(ui->pushButtonMstReturn)

	QStandardItemModel *model = new QStandardItemModel(0, 4);////7);

	ui->tableViewMstList->setModel(model);
	ui->tableViewMstList->verticalHeader()->hide();
	ui->tableViewMstList->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableViewMstList->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableViewMstList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableViewMstList->setSortingEnabled(false);
	ui->tableViewMstList->setShowGrid(false);
	ui->tableViewMstList->setFocusPolicy(Qt::NoFocus);

	g_pScaleSrc->get_voip_tableViewMst_Listwidth(x,y,w,h);

	ui->tableViewMstList->setColumnWidth(0, x);
	ui->tableViewMstList->setColumnWidth(1, y);
	ui->tableViewMstList->setColumnWidth(2, w);
	ui->tableViewMstList->setColumnWidth(3, h);

	ui->tableViewMstList->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
	ui->tableViewMstList->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
	ui->tableViewMstList->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
	ui->tableViewMstList->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);

	SetTableHeader();
}

MstDialog::~MstDialog()
{
	delete ui;
}

void MstDialog::ResetState()
{
    ui->pushButtonMstPrev->setFocus(Qt::TabFocusReason);
}

void MstDialog::SetTableHeader()
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewMstList->model();

	model->setHeaderData(0, Qt::Horizontal, tr("序号"));
	model->setHeaderData(1, Qt::Horizontal, tr("号码信息"));
	model->setHeaderData(2, Qt::Horizontal, tr("端口"));
	model->setHeaderData(3, Qt::Horizontal, tr("IP地址"));
}

void MstDialog::on_pushButtonMstReturn_pressed() //Exit monitor state
{
	int i;
	char strIP[20];

	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewMstList->model();
	for (i = 0; i < model->rowCount(); i++)
	{
		strcpy(strIP, model->item(i, 3)->text().toLocal8Bit().data());
		lpc_cmd_terminate(lphone, strIP);
	}

	model->removeRows(0, model->rowCount());

    hide();
    ((QDialog *)parent())->hide();

	((Application *)qApp)->setLCDAndTimerOn();
}

void MstDialog::on_pushButtonMstHangUp_clicked()
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewMstList->model();

	if (model->rowCount() <= 0) return;

	int rowSel = ui->tableViewMstList->currentIndex().row();
	if (rowSel < 0) return;

	char strIP[20];
	strcpy(strIP, model->item(rowSel, 3)->text().toLocal8Bit().data());

	printf("str:%s\n",strIP);
	lpc_cmd_terminate(lphone, strIP);

	model->removeRow(rowSel);

	int i;
	for (i = rowSel; i < model->rowCount(); i++)
	{
		model->item(i, 0)->setText(QString::number(i + 1));
	}

	if (model->rowCount() == 0)
	{
        hide();
        ((QDialog *)parent())->hide();

		((Application *)qApp)->setLCDAndTimerOn();
	}
}

bool MstDialog::event(QEvent *event)
{
	switch (event->type() - 5000)
	{
	case NID_MST_NONE:
		{
			break;
		}
	case	NID_IDLE_RCVMON:
		{
			printf("\nNID_IDLE_RCVMON\n\n");

			((Application *)qApp)->setLCDOn();
			((Application *)qApp)->setOnCamera(true);

			video_stream_unmute_send(eXosipElement->videostream);

            ((QDialog *)parent())->raise();
            ((QDialog *)parent())->show();
            raise();
			show();

			ResetState();

			QString strRemoteNameAndIP = (dynamic_cast <Levent *> (event))->msg;
			QString strRemoteName = strRemoteNameAndIP.left(strRemoteNameAndIP.indexOf(' '));
			QString strRemoteIP = strRemoteNameAndIP.mid(strRemoteNameAndIP.indexOf(' ') + 1);

			int pos = strRemoteName.indexOf("?");
			if (pos)
			{
				strRemoteName = strRemoteName.left(pos);
			}

			extern int g_video_channel;
			/*if (m_strRemoteNames.indexOf(strRemoteName + ":" + QString::number(g_video_channel + 1)) == -1)
			{
				m_strRemoteNames.append(strRemoteName + ":" + QString::number(g_video_channel + 1));
				m_strRemoteIPs.append(strRemoteIP);

				QStringListModel *model = (QStringListModel *)ui->listViewMon->model();
				model->setStringList(m_strRemoteNames);

				QModelIndex index = model->index(m_strRemoteNames.count() - 1, 0);

				ui->listViewMon->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
			}

			ui->lineEditInfo->setText(tr("来自:") + strRemoteName);*/

			QStandardItemModel *model = (QStandardItemModel *)ui->tableViewMstList->model();
			int row = model->rowCount();
			QStandardItem *indxItem;
			QStandardItem *netNameItem;
			QStandardItem *portItem;
			QStandardItem *ipItem;
			indxItem = new QStandardItem(QString::number(row + 1));
			indxItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			model->setItem(row, 0, indxItem);
			netNameItem = new QStandardItem(strRemoteName);
			netNameItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			model->setItem(row, 1, netNameItem);
			portItem = new QStandardItem(QString::number(g_video_channel + 1));
			portItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			model->setItem(row, 2, portItem);
			ipItem = new QStandardItem(strRemoteIP);
			ipItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			model->setItem(row, 3, ipItem);

			if (model->rowCount() == 1)
			{
				ui->tableViewMstList->selectRow(0);
			}

			break;
		}
	case	NID_MST_TIMEOUT:
		{
			//ui->lineEditInfo->setText(tr("超时"));
			break;
		}
	case	NID_MST_RCVCANCEL:
		{
			printf("lg NID_MST_RCVCANCEL\n");

			//extern int g_video_channel;
			QString strRemoteIP = (dynamic_cast <Levent *> (event))->msg;

			QStandardItemModel *model = (QStandardItemModel *)ui->tableViewMstList->model();
			int i;
			for (i = 0; i < model->rowCount(); i++)
			{
				if (model->item(i, 3)->text() == strRemoteIP)
				{
					break;
				}
			}

			if (i < model->rowCount())
			{
				model->removeRow(i);

				for (; i < model->rowCount(); i++)
				{
					model->item(i, 0)->setText(QString::number(i + 1));
				}
			}

			/*int index = m_strRemoteIPs.indexOf(strRemoteIP);
			QStringListModel *model;

			model = (QStringListModel *)ui->listViewMon->model();

			printf("\nstrRemoteName:%s index:%d\n\n", strRemoteIP.toLatin1().data(), index);

			if (index != -1)
			{
				model->removeRow(index);
				m_strRemoteNames.removeAt(index);
				m_strRemoteIPs.removeAt(index);
			}

			ui->lineEditInfo->setText(tr("接收取消...") + strRemoteIP);
			*/
			if (model->rowCount() == 0)
			{
				if (g_bIstOrAlertBusy == 3)
				{
					g_bIstOrAlertBusy = 0;
				}

                hide();
                ((QDialog *)parent())->hide();

				((Application *)qApp)->setLCDAndTimerOn();
				((Application *)qApp)->setOnCamera(false);

				//lower();
				//((QDialog *)parent())->lower();
            }
			break;
		}
	case	NID_MST_RCVOPEN:
		{
			printf("lg NID_MST_RCVOPEN\n");
			//QString strRemoteIP = (dynamic_cast <Levent *> (event))->msg;
			//int index = m_strRemoteIPs.indexOf(strRemoteIP);
			/*QStringListModel *model;

			if (index != -1)
			{
				model = (QStringListModel *)ui->listViewMon->model();
				model->removeRow(index);
			}
			*/
			//ui->lineEditInfo->setText(tr("接收打开...") + m_strRemoteNames.at(index));
			break;
		}
	case	NID_MST_END:
		{
			//printf("\tlg lg NID_MST_END\n");
			QStandardItemModel *model = (QStandardItemModel *)ui->tableViewMstList->model();

			if (model->rowCount() == 0)
			{
				if (g_bIstOrAlertBusy == 3)
				{
					g_bIstOrAlertBusy = 0;
				}

                hide();
                ((QDialog *)parent())->hide();

				((Application *)qApp)->setLCDAndTimerOn();
				((Application *)qApp)->setOnCamera(false);
			}

			break;
		}
	default:
		{
			return QDialog::event(event);
		}
	}

	return true;
}

void MstDialog::showEvent(QShowEvent *)
{
	g_pMainDlg->showRefreshTabFocus(this);
}

void MstDialog::hideEvent(QHideEvent *)
{
	g_pMainDlg->hideRefreshTabFocus(this);
}

void MstDialog::on_pushButtonMstPrev_clicked()
{
	int row = ui->tableViewMstList->verticalScrollBar()->value(); //ui->tableViewInfoList->currentIndex().row();

	if (row - TABLE_ROWS_PER_PAGE >= 0)
	{
		ui->tableViewMstList->selectRow(row - TABLE_ROWS_PER_PAGE);
		ui->tableViewMstList->verticalScrollBar()->setSliderPosition(row - TABLE_ROWS_PER_PAGE);
	}
	else if (row > 0)
	{
		ui->tableViewMstList->selectRow(0);
		ui->tableViewMstList->verticalScrollBar()->setSliderPosition(0);
	}
}

#if 0
void MstDialog::on_pushButtonMon_clicked()
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewMstList->model();
	int row = ui->tableViewMstList->verticalScrollBar()->value(); //ui->tableViewInfoList->currentIndex().row();

	if (row + TABLE_ROWS_PER_PAGE < model->rowCount())
	{
		ui->tableViewMstList->selectRow(row + TABLE_ROWS_PER_PAGE);
		ui->tableViewMstList->verticalScrollBar()->setSliderPosition(row + TABLE_ROWS_PER_PAGE);
	}
}
#endif

