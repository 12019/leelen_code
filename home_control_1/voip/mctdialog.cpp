#include "mctdialog.h"
#include "ui_mctdialog.h"
#include "lpcore.h"
#include "command.h"
#include <QtGui>
#include "qconfig.h"
#include "mediastreamer2/mediastream.h"
#include "exosip_main.h"
#include "maindialog.h"
#include "callcenterdialog.h"
#include "terminalhelpmaindialog.h"
#include "helpdialog.h"
#include "application.h"

extern LinphoneCore *lphone;
extern eXosip_element_t *eXosipElement;

MctDialog::MctDialog(QWidget *parent) :
	QDialog(parent),
    ui(new Ui::MctDialog)
{
    ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	setFixedSize(800,480);

	/*ui->pushButtonClear->setText("");
	ui->pushButtonDial->setText("");
	ui->pushButtonReturn->setText("");
	*/
	ui->pushButton_dot->setText("");
	ui->pushButton_num_0->setText("");
	ui->pushButton_num_1->setText("");
	ui->pushButton_num_2->setText("");
	ui->pushButton_num_3->setText("");
	ui->pushButton_num_4->setText("");
	ui->pushButton_num_5->setText("");
	ui->pushButton_num_6->setText("");
	ui->pushButton_num_7->setText("");
	ui->pushButton_num_8->setText("");
	ui->pushButton_num_9->setText("");
	ui->pushButton_shop->setText("");

	m_strDialIpAddress = "";
	m_bHookCloseMctDlg = true;

	m_pHookDlg = new MctHookDialog(this);
	m_pHookDlg->hide();
	m_pHookDlg->setDisplayWindow(false);

	QStandardItemModel *model = new QStandardItemModel(2, 2);
	ui->tableViewVideoChannel->setModel(model);
	ui->tableViewVideoChannel->horizontalHeader()->hide();
	ui->tableViewVideoChannel->verticalHeader()->hide();
	ui->tableViewVideoChannel->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableViewVideoChannel->setSelectionBehavior(QAbstractItemView::SelectItems);
	ui->tableViewVideoChannel->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableViewVideoChannel->setShowGrid(false);
	ui->tableViewVideoChannel->setFocusPolicy(Qt::NoFocus);

	QRect rc = ui->tableViewVideoChannel->geometry();
	rc.setWidth(rc.width() - 4);
	rc.setHeight(rc.height() - 4);
	ui->tableViewVideoChannel->setColumnWidth(0, rc.width()/2);
	ui->tableViewVideoChannel->setColumnWidth(1, rc.width()/2);

	ui->tableViewVideoChannel->setRowHeight(0, rc.height()/2);
	ui->tableViewVideoChannel->setRowHeight(1, rc.height()/2);

	int i;
	QStandardItem *item;
	for (i = 0; i < 4; i++)
	{
		item = new QStandardItem(QString::number(i + 1));
		item->setTextAlignment(Qt::AlignCenter);
		model->setItem(i/2, i%2, item);
	}

	QModelIndex index;
	index = model->index(0, 0);
	ui->tableViewVideoChannel->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);

	m_videoChannel = 0;
}

MctDialog::~MctDialog()
{
	delete m_pHookDlg;
    delete ui;
}

void MctDialog::ResetState()
{
	//ui->lineEditDisplay->setText("");//192.168.1.236");
}

void MctDialog::on_pushButton_num_0_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "0");
}

void MctDialog::on_pushButton_num_1_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "1");
}

void MctDialog::on_pushButton_num_2_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "2");
}

void MctDialog::on_pushButton_num_3_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "3");
}

void MctDialog::on_pushButton_num_4_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "4");
}

void MctDialog::on_pushButton_num_5_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "5");
}

void MctDialog::on_pushButton_num_6_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "6");
}

void MctDialog::on_pushButton_num_7_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "7");
}

void MctDialog::on_pushButton_num_8_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "8");
}

void MctDialog::on_pushButton_num_9_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "9");
}

void MctDialog::on_pushButton_dot_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + ".");
}

void MctDialog::on_pushButton_shop_clicked()
{
	ui->lineEditDisplay->setText(ui->lineEditDisplay->text() + "#");
}

void MctDialog::on_pushButtonDial_clicked()
{
	QStandardItemModel *model;

	if (ui->lineEditDisplay->text() != "")
	{
		char strPeerIP[16];// = "192.168.1.238";
		int ret;
		GetPeerIPThread *pGetPeerIPThread = ((MainDialog *)(parent()->parent()))->m_pVoipMainDlg->m_pGetPeerIPThread;

		ret = pGetPeerIPThread->GetPeerIP(ui->lineEditDisplay->text().toLatin1().data(), strPeerIP);

		printf("lg mct lpc_cmd_mon:%s\n", strPeerIP);

		if (ret == 0)
		{
			m_strDialIpAddress = QString(strPeerIP);

			model = (QStandardItemModel *)ui->tableViewVideoChannel->model();
			//strChannel = model->data(ui->tableViewVideoChannel->selectionModel()->currentIndex()).toString();
			m_videoChannel = model->data(ui->tableViewVideoChannel->selectionModel()->currentIndex()).toInt() - 1;

			((Application *)qApp)->setLCDOn();

			printf("lg mct lpc_cmd_mon\n");
			lpc_cmd_mon(lphone, (QString(strPeerIP) + ":" + QString::number(m_videoChannel)).toLatin1().data());
		}
		else
		{
			ui->pushButtonDial->setEnabled(false);
			ui->pushButtonReturn->setEnabled(false);
			QMessageBox messBox(this);
			messBox.setObjectName("mymsgbox");
			messBox.setWindowFlags(Qt::FramelessWindowHint) ;
			QPushButton *okButton = messBox.addButton(QString::fromUtf8("\n　确定　\n"), QMessageBox::ActionRole);
			Q_UNUSED(okButton);
			messBox.setText(QString::fromUtf8("\n获取对方IP地址失败！\n"));
			messBox.exec();
			ui->pushButtonDial->setEnabled(true);
			ui->pushButtonReturn->setEnabled(true);
		}
	}
}

void MctDialog::on_pushButtonClear_clicked()
{
	QString str = ui->lineEditDisplay->text();
	str = str.left(str.length() - 1);
	ui->lineEditDisplay->setText(str);
}

void MctDialog::on_pushButtonReturn_clicked()
{
	lpc_cmd_terminate(lphone, NULL);//ui->lineEditDisplay->text().toLatin1().data());

	((VoipMainDialog *)parent())->hide();
	hide();

	((Application *)qApp)->setLCDAndTimerOn();
}

bool MctDialog::event(QEvent *event)
{
	switch (event->type() - 5000)
	{
	case NID_MCT_NONE:
		{
			ui->lineEditInfo->setText(QString::fromUtf8("无"));
			break;
		}
	case NID_MCT_TIMEOUT:
		{
			ui->lineEditInfo->setText(QString::fromUtf8("超时"));
			break;
		}
	case	NID_MCT_RCVBUSY:
		{
			ui->lineEditInfo->setText(QString::fromUtf8("现在正忙"));
			break;
		}
	case	NID_MCT_CONNECTED:
		{
			((Application *)qApp)->setLCDOn();

			if (eXosipElement->audiostream != NULL)
			{
				audio_stream_set_level(eXosipElement->audiostream, ((VoipMainDialog *)parent())->m_nVolume);
			}

			QString str = QString::fromUtf8("已连接:") + ui->lineEditDisplay->text();
			QStandardItemModel *model;
			model = (QStandardItemModel *)ui->tableViewVideoChannel->model();

			if (!m_pHookDlg->isVisible())
			{
				int vchannel;
				vchannel = model->data(ui->tableViewVideoChannel->selectionModel()->currentIndex()).toInt() - 1;
				m_pHookDlg->show();
				m_pHookDlg->ResetState(str, m_strDialIpAddress, m_videoChannel);
			}
			else
			{
				QModelIndex index;
				m_videoChannel = m_pHookDlg->m_videoChannel;
				index = model->index(m_videoChannel/2, m_videoChannel%2);
				ui->tableViewVideoChannel->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
				m_pHookDlg->ResetState(str, m_strDialIpAddress, m_videoChannel);
			}

			m_pHookDlg->repaint();
			m_pHookDlg->setDisplayWindow(true);
			ui->lineEditInfo->setText(QString::fromUtf8("已连接"));
			break;
		}
	case	NID_MCT_RCVCANCEL:
		{
			ui->lineEditInfo->setText(QString::fromUtf8("接收取消"));
			break;
		}
	case	NID_MCT_END:
		{
			ui->lineEditInfo->setText(QString::fromUtf8("结束"));

			//if (m_bHookCloseMctDlg)
			//{
			//	hide();
			//}

			m_pHookDlg->m_pFrameFullshow->hide();
			m_pHookDlg->m_pFrameFullshow->lower();

			m_pHookDlg->ResetState(QString::fromUtf8("结束"), m_strDialIpAddress, -1, false);

			if (m_bHookCloseMctDlg)
			{
				((VoipMainDialog *)parent())->hide();
				m_pHookDlg->hide();
			}

			m_bHookCloseMctDlg = true;

			((Application *)qApp)->setLCDAndTimerOn();

			break;
		}
	default:
		{
			return QDialog::event(event);
		}
	}

	return true;
}

void MctDialog::on_pushButtonCallCenter_clicked()
{
	lpc_cmd_terminate(lphone, NULL);

	((Application *)qApp)->setLCDAndTimerOn();

	CallCenterDialog::showCallCenterDialog();
}

void MctDialog::on_pushButtonEmergency_clicked()
{
	lpc_cmd_terminate(lphone, NULL);

	((Application *)qApp)->setLCDAndTimerOn();

	TerminalHelpMainDialog::showTerminalHelpDialog();
}

void MctDialog::on_pushButtonHelp_clicked()
{
	lpc_cmd_terminate(lphone, NULL);

	((Application *)qApp)->setLCDAndTimerOn();

	HelpDialog::showHelpDialog("help1.txt");
}
