#include <Qt/qhostaddress.h>
#include <Qt/qtcpsocket.h>
#include <Qt/qdatastream.h>
#include <Qt/qbytearray.h>
#include <Qt/qstringlist.h>
#include <Qt/qstringlistmodel.h>
#include <Qt/qmessagebox.h>
#include <string.h>
#include "terminalhelpmaindialog.h"
#include "ui_terminalhelpmaindialog.h"
#include "data_types.h"
#include "callcenterdialog.h"
#include "maindialog.h"
#include <QSettings>
#include "helpdialog.h"
#include "ui_maindialog.h"

extern MainDialog *g_pMainDlg;
extern char g_strNativeName[50];
extern char g_strLphoneName[3][50];

TerminalHelpMainDialog *TerminalHelpMainDialog::m_pDlg = NULL;

TerminalHelpMainDialog::TerminalHelpMainDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TerminalHelpMainDialog)
{
    ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	TerminalHelpMainDialog::m_pDlg = this;

	/*ui->pushButtonCallCenter->setText("");
	ui->pushButtonEmergency->setText("");
	ui->pushButtonHelp->setText("");
	ui->pushButtonReturn->setText("");
	*/
	model = new QStringListModel(this);
	//第一行数据
	data << tr("连接管理中心计算机...");
	data += tr("连接管理中心机...");
	data += tr("获取管理中心机状态...");
	//data += "";

	//data +=QString::fromUtf8("等待管理中心机");
	ui->listView->setModel(model);
	//ui->listView->setFixedHeight(200);
	ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->listView->setUpdatesEnabled(true);

	model->setStringList(data);

	m_pThread = new TerminalHelpThread(this);

	connect(m_pThread, SIGNAL(sigSendHelp()), this, SLOT(sendHelp()));
}

TerminalHelpMainDialog::~TerminalHelpMainDialog()
{
	delete m_pThread;
    delete ui;
}

void TerminalHelpMainDialog::on_pushButtonEmergency_clicked()
{
	ui->pushButtonEmergency->setEnabled(false);
	ui->frame->setFocus(Qt::TabFocusReason);

	QString text0 = tr("连接管理中心计算机...");
	QModelIndex index0 = model->index(0);
	model->setData(index0, text0);

	QString text1 = tr("连接管理中心机...");
	QModelIndex index1 = model->index(1);
	model->setData(index1, text1);

	QString text2 = tr("获取管理中心机状态...");
	QModelIndex index2 = model->index(2);
	model->setData(index2, text2);

	if (model->rowCount() == 4)
	{
		model->removeRow(3);
	}

	QModelIndex index3 = model->index(3);
	model->setData(index3, "");

	m_pThread->quit();
	m_pThread->wait();
	m_pThread->start();
}

void TerminalHelpMainDialog::sendHelp()
{
	QModelIndex index0 = model->index(0);
	QModelIndex index1 = model->index(1);
	QModelIndex index2 = model->index(2);
	//QModelIndex index3 = model->index(3);

	int i;
	const char *str[2] ={QT_TRANSLATE_NOOP("TerminalHelpMainDialog", "管理中心计算机"), QT_TRANSLATE_NOOP("TerminalHelpMainDialog", "管理中心机")};
	GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;
	for (i = 0; i < 2; i++)
	{
		char strIP[200] = "192.168.1.139";

		QTcpSocket *pTcpSock = new QTcpSocket(this);

		if (pGetPeerIPThread->GetPeerIP(g_strLphoneName[i + 1], strIP) != 0)
		{
			delete pTcpSock;
			model->setData((i == 0) ? index0 : index1, tr("获取") + tr(str[i]) + tr("IP失败"));
			continue;
		}

		printf("TerminalHelpMainDialog::sendHelp strIP:%s\n", strIP);

		char *ptr = strstr(strIP, "->");
		if (ptr)
		{
			ptr[0] = '\0';
		}
		/*if (!strIP[0])
		{
			delete pTcpSock;
			model->setData((i == 0) ? index0 : index1, tr("获取") + tr(str[i]) + tr("IP失败"));
			continue;
		}*/

		pTcpSock->connectToHost(strIP, 17731);
		int ret = 0;

		if (pTcpSock->waitForConnected())
		{
			short len;

			len = strlen(g_strNativeName);
			pTcpSock->write((char *)&len, sizeof(short));
			pTcpSock->write((char *)g_strNativeName, len);
			pTcpSock->flush();

			char buf[2];
			int cnt = 0;
			int loop = 0;
			while (pTcpSock->waitForReadyRead(500))
			{
				cnt += pTcpSock->read(buf + cnt, 2 - cnt);
				if (++loop >= 2) break;
			}

			if (cnt == 2)
			{
				if (buf[0] == 1)
				{
					model->setData((i == 0) ? index0 : index1, tr("连接") + tr(str[i]) + tr("成功"));
				}
				else
				{
					model->setData((i == 0) ? index0 : index1, tr("连接") + tr(str[i]) + tr("失败"));
				}

				if (buf[1] == 1)
				{
					model->setData(index2, tr(str[i]) + tr("空闲"));
				}
				else
				{
					model->setData(index2, tr(str[i]) + tr("忙"));
				}

				if ((buf[0] == 1) && (buf[1] == 1))
				{
					if (model->rowCount() == 3)
					{
						model->insertRow(3);
					}

					model->setData(model->index(3), tr("等待") + tr(str[i]) + tr("回呼..."));

					ret = 1;
				}
			}
			else
			{
				model->setData((i == 0) ? index0 : index1, tr("连接") + tr(str[i]) + tr("失败"));
			}

			pTcpSock->close();
		}
		else
		{
			model->setData((i == 0) ? index0 : index1, tr("连接") + tr(str[i]) + tr("失败"));
		}

		delete pTcpSock;

		if (ret == 1) break;
	}

	ui->pushButtonEmergency->setEnabled(true);
	ui->pushButtonEmergency->setFocus(Qt::TabFocusReason);
}

void TerminalHelpMainDialog::on_pushButtonReturn_clicked()
{
	close();
}

void TerminalHelpMainDialog::on_pushButtonCallCenter_clicked()
{
	CallCenterDialog::showCallCenterDialog();
}

void TerminalHelpMainDialog::showEvent(QShowEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)parent();
	pMainDlg->showRefreshTabFocus(this);
}

void TerminalHelpMainDialog::hideEvent(QHideEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)parent();
	pMainDlg->hideRefreshTabFocus(this);

	if (pMainDlg->m_pCurActiveDlg == pMainDlg)
	{
		pMainDlg->ui->pushButtonTerminalHelp->setFocus(Qt::TabFocusReason);
	}
}

void TerminalHelpMainDialog::showTerminalHelpDialog()
{
	TerminalHelpMainDialog::m_pDlg->show();
	TerminalHelpMainDialog::m_pDlg->raise();
}

void TerminalHelpMainDialog::on_pushButtonHelp_clicked()
{
	HelpDialog::showHelpDialog(this);
}
