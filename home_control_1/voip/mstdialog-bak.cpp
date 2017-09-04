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
#include "callcenterdialog.h"

extern LinphoneCore *lphone;
extern eXosip_element_t *eXosipElement;

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

	setFixedSize(800,480);

	m_strRemoteIps.clear();


	ui->verticalSliderVolume->setRange(0, 60);
	ui->verticalSliderVolume->setValue(60);

	QStringListModel *model = new QStringListModel();
	ui->listViewMon->setModel(model);

	ui->listViewMon->setSelectionMode(QListView::SingleSelection);
	ui->listViewMon->setEditTriggers(QListView::NoEditTriggers);

	model->setStringList(m_strRemoteIps);
	//bcom ui->pushButtonExitMon->setText("");
}

MstDialog::~MstDialog()
{
	delete ui;
}

void MstDialog::ResetState()
{
	ui->verticalSliderVolume->setValue((int)(((VoipMainDialog *)parent())->m_nVolume * 60.0/1024.0));
}

void MstDialog::on_pushButtonExitMon_clicked()
{
	QStringList strs = m_strRemoteIps;
	int i;
	QString strIp;

	for (i = 0; i < strs.count(); i++)
	{
		int idx;
		strIp = strs.at(i);
		idx = strIp.indexOf(":");
		if (idx != -1)
		{
			strIp = strIp.left(idx);
		}

		lpc_cmd_terminate(lphone, strIp.toLatin1().data());
	}

	m_strRemoteIps.clear();

	QStringListModel *model = (QStringListModel *)ui->listViewMon->model();

	model->setStringList(m_strRemoteIps);

	done(0);
}

void MstDialog::on_pushButtonCloseMon_clicked()
{
	if (m_strRemoteIps.count() <= 0) return;

	int row = ui->listViewMon->currentIndex().row();
	QString strIp = m_strRemoteIps.at(row);
	QStringListModel *model = (QStringListModel *)ui->listViewMon->model();

	int idx;
	idx = strIp.indexOf(":");
	if (idx != -1)
	{
		strIp = strIp.left(idx);
	}

	lpc_cmd_terminate(lphone, strIp.toLatin1().data());
	ui->lineEditInfo->setText(QString::fromUtf8("发送取消...") + strIp);

	model->removeRow(row);
	m_strRemoteIps.removeAt(row);
}

bool MstDialog::event(QEvent *event)
{
	switch (event->type() - 5000)
	{
	case NID_MST_NONE:
		{
			ui->lineEditInfo->setText(QString::fromUtf8("无"));
			break;
		}

	case	NID_IDLE_RCVMON:
		{
			MainDialog *pMainDlg = (MainDialog *)(parent()->parent());

			if (pMainDlg->m_pCurActiveDlg)
			{
				if (pMainDlg->m_pMediaManDlg->m_pMediaPlayerDlg->m_pReadAVI)
				{
					pMainDlg->m_pMediaManDlg->m_pMediaPlayerDlg->stopPlayer();
					pMainDlg->m_pMediaManDlg->m_pMediaPlayerDlg->hide();
				}

				pMainDlg->m_pCurActiveDlg->hide();
			}

			if (eXosipElement->audiostream != NULL)
			{
				audio_stream_set_level(eXosipElement->audiostream, ((VoipMainDialog *)parent())->m_nVolume);
			}

			((QDialog *)parent())->show();
			show();

			ResetState();

			QString strRemoteIp = (dynamic_cast <Levent *> (event))->msg;

			if (m_strRemoteIps.indexOf(strRemoteIp) == -1)
			{
				extern int g_video_channel;
				m_strRemoteIps.append(strRemoteIp + ":" + QString::number(g_video_channel));

				QStringListModel *model = (QStringListModel *)ui->listViewMon->model();
				model->setStringList(m_strRemoteIps);

				QModelIndex index = model->index(m_strRemoteIps.count() - 1, 0);

				ui->listViewMon->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
			}

			ui->lineEditInfo->setText(QString::fromUtf8("来自:") + strRemoteIp);
			break;
		}
	case	NID_MST_TIMEOUT:
		{
			ui->lineEditInfo->setText(QString::fromUtf8("超时"));
			break;
		}
	case	NID_MST_RCVCANCEL:
		{
			printf("lg NID_MST_RCVCANCEL\n");
			extern int g_video_channel;
			QString strRemoteIp = (dynamic_cast <Levent *> (event))->msg;
			int index = m_strRemoteIps.indexOf(strRemoteIp + ":" + QString::number(g_video_channel));
			QStringListModel *model;

			if (index != -1)
			{
				model = (QStringListModel *)ui->listViewMon->model();
				model->removeRow(index);
				m_strRemoteIps.removeAt(index);
			}

			ui->lineEditInfo->setText(QString::fromUtf8("接收取消...") + strRemoteIp);

			if (m_strRemoteIps.count() == 0)
			{
				hide();
			}

			break;
		}
	case	NID_MST_RCVOPEN:
		{
			QString strRemoteIp = (dynamic_cast <Levent *> (event))->msg;
			/*int index = m_strRemoteIps.indexOf(strRemoteIp);
			QStringListModel *model;

			if (index != -1)
			{
				model = (QStringListModel *)ui->listViewMon->model();
				model->removeRow(index);
			}
			*/
			ui->lineEditInfo->setText(QString::fromUtf8("接收打开...") + strRemoteIp);
			break;
		}
	case	NID_MST_END:
		{
			hide();
			break;
		}
	default:
		{
			return QDialog::event(event);
		}
	}

	return true;
}


void MstDialog::on_verticalSliderVolume_valueChanged(int value)
{
	VoipMainDialog *pVMDlg = (VoipMainDialog *)parent();

	pVMDlg->m_nVolume = (unsigned int)(value * 1024.0/60.0);

	if (isVisible() && (eXosipElement->audiostream != NULL))
	{
		audio_stream_set_level(eXosipElement->audiostream, pVMDlg->m_nVolume);
	}
}

void MstDialog::on_pushButtonCallCenter_clicked()
{
	on_pushButtonExitMon_clicked();

	CallCenterDialog::showCallCenterDialog();
}
