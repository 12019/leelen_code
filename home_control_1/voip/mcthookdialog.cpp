#include "mcthookdialog.h"
#include "ui_mcthookdialog.h"
#include "lpcore.h"
#include "command.h"
#include "qconfig.h"
#include "mctdialog.h"
#include "voipmaindialog.h"
#include "exosip_main.h"
#include "callcenterdialog.h"
#include <QStandardItemModel>
#include "terminalhelpmaindialog.h"
#include "helpdialog.h"
#include "application.h"

extern LinphoneCore *lphone;
extern eXosip_element_t *eXosipElement;

MctHookDialog::MctHookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MctHookDialog)
{
    ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	setFixedSize(800,480);

	//ui->pushButtonHangUp->setText("");
	//ui->pushButtonReturn->setText("");

	ui->horizontalSliderVolume->setRange(0, 60);
	//ui->horizontalSliderVolume->setValue(60);
	ui->horizontalSliderVolume->setFocusPolicy(Qt::NoFocus);

	m_pFrameFullshow = new QFrame(this);
	m_pFrameFullshow->setObjectName(QString::fromUtf8("frameFullshow"));
	m_pFrameFullshow->setGeometry(QRect(0, 0, 800, 480));
	m_pFrameFullshow->setFrameShadow(QFrame::Plain);
	m_pFrameFullshow->setFrameShape(QFrame::Panel);

	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	m_pFrameFullshow->installEventFilter(this);
	ui->frameVideoShow->installEventFilter(this);

	//QStandardItemModel *model = new QStandardItemModel(0, 4);
	//ui->tableWidgetVideoChannel->setModel(model);
	ui->tableWidgetVideoChannel->setRowCount(2);
	ui->tableWidgetVideoChannel->setColumnCount(2);
	ui->tableWidgetVideoChannel->horizontalHeader()->hide();
	ui->tableWidgetVideoChannel->verticalHeader()->hide();
	ui->tableWidgetVideoChannel->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidgetVideoChannel->setSelectionBehavior(QAbstractItemView::SelectItems);
	ui->tableWidgetVideoChannel->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableWidgetVideoChannel->setShowGrid(false);
	ui->tableWidgetVideoChannel->setFocusPolicy(Qt::NoFocus);
	QRect rc = ui->tableWidgetVideoChannel->geometry();
	rc.setWidth(rc.width() - 4);
	rc.setHeight(rc.height() - 4);
	ui->tableWidgetVideoChannel->setColumnWidth(0, rc.width()/2);
	ui->tableWidgetVideoChannel->setColumnWidth(1, rc.width()/2);
	ui->tableWidgetVideoChannel->setRowHeight(0, rc.height()/2);
	ui->tableWidgetVideoChannel->setRowHeight(1, rc.height()/2);
	int i;
	for (i = 0; i < 4; i++)
	{
		QTableWidgetItem *item = new QTableWidgetItem(QString::number(i + 1));
		item->setTextAlignment(Qt::AlignCenter);
		ui->tableWidgetVideoChannel->setItem(i/2, i%2, item);
	}

	m_videoChannel = -1;

	QModelIndex index;
	index = ui->tableWidgetVideoChannel->model()->index(0, 0);
	ui->tableWidgetVideoChannel->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
}

MctHookDialog::~MctHookDialog()
{
	delete m_pFrameFullshow;
    delete ui;
}

void MctHookDialog::ResetState(QString text, QString strName, int vchannel, bool bInitVolume)
{
	ui->lineEditInfo->setText(text);
	if (bInitVolume)
	{
		ui->horizontalSliderVolume->setValue((int)(((VoipMainDialog *)(parent()->parent()))->m_nVolume * 60.0/1024.0));
	}

	if (vchannel != -1)
	{
		QModelIndex index;
		m_videoChannel = -1;
		index = ui->tableWidgetVideoChannel->model()->index(vchannel/2, vchannel%2);
		ui->tableWidgetVideoChannel->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);

		m_videoChannel = vchannel;
	}

	if (strName != "")
	{
		m_strName = strName;
	}
}

void MctHookDialog::on_pushButtonHangUp_clicked()
{
	//((MctDialog *)parent())->m_bHookCloseMctDlg = true;

	setDisplayWindow(false);
	usleep(120000);

	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	lpc_cmd_terminate(lphone, NULL);

	((VoipMainDialog *)(parent()->parent()))->hide();
	((MctDialog *)parent())->hide();
	hide();

	((Application *)qApp)->setLCDAndTimerOn();
}

void MctHookDialog::on_pushButtonReturn_clicked()
{
	//((MctDialog *)parent())->m_bHookCloseMctDlg  = false;

	setDisplayWindow(false);
	usleep(120000);

	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	//printf("lg 2\n");
	lpc_cmd_terminate(lphone, NULL);

	((VoipMainDialog *)(parent()->parent()))->hide();
	((MctDialog *)parent())->hide();
	hide();

	((Application *)qApp)->setLCDAndTimerOn();
}

bool MctHookDialog::eventFilter(QObject *o, QEvent *e)
{
	if (e->type() == QEvent::MouseButtonPress)
	{
		setDisplayWindow(false);
		usleep(120000);

		if (o == ui->frameVideoShow)
		{
			m_pFrameFullshow->show();
			m_pFrameFullshow->raise();
		}
		else if (o == m_pFrameFullshow)
		{
			m_pFrameFullshow->hide();
			m_pFrameFullshow->lower();
		}

		repaint();
		setDisplayWindow(true);
	}

	return false;
}

extern Lvirdd *lvirdd;

int MctHookDialog::setDisplayWindow(bool bShow)
{
	int vrect[4];
	QRect rect;

	if (!lvirdd) return -1;
	if (!lvirdd->branch.display) return -1;

	if (!bShow)
	{
		vrect[0] = vrect[1] = 1;
		vrect[2] = vrect[3] = 1;
	}
	else
	{
		if (!m_pFrameFullshow->isVisible())
		{
			rect = ui->frameVideoShow->geometry();
			rect = QRect(rect.x(), rect.y(), rect.width() - 1, rect.height() - 1);
		}
		else
		{
			rect = m_pFrameFullshow->geometry();
		}

		vrect[0] = (rect.x()) & 0x7ffffffe;
		vrect[1] = rect.y();

		vrect[2] = rect.width() & 0x7ffffffe;
		vrect[3] = rect.height();
	}

	ms_filter_call_method(lvirdd->branch.display, MS_FILTER_SET_VIDEO_RECT, vrect);

	return 0;
}

void MctHookDialog::on_horizontalSliderVolume_valueChanged(int value)
{
	VoipMainDialog *pVMDlg = (VoipMainDialog *)(parent()->parent());

	pVMDlg->m_nVolume = (unsigned int)(value * 1024.0/60.0);

	if (isVisible() && (eXosipElement->audiostream != NULL))
	{
		audio_stream_set_level(eXosipElement->audiostream, pVMDlg->m_nVolume);
	}
}

void MctHookDialog::on_pushButtonCallCenter_clicked()
{
	on_pushButtonReturn_clicked();

	CallCenterDialog::showCallCenterDialog();
}

/*void MctHookDialog::on_tableViewVideoChannel_clicked(QModelIndex index)
{
	Q_UNUSED(index);
	QStandardItemModel *model;
	int vchannel;

	model = (QStandardItemModel *)ui->tableViewVideoChannel->model();

	vchannel = model->data(ui->tableViewVideoChannel->selectionModel()->currentIndex()).toInt();

	if (vchannel == m_videoChannel)
	{
		return;
	}

	printf("lg vchannel:%d\n", vchannel);

	((MctDialog *)parent())->m_bHookCloseMctDlg = false;

	lpc_cmd_terminate(lphone, NULL);

	usleep(400000);
	lpc_cmd_mon(lphone, (m_strName + ":" + QString::number(vchannel)).toLatin1().data());

	m_videoChannel = vchannel;
}
*/
void MctHookDialog::on_tableWidgetVideoChannel_itemSelectionChanged()
{
	int vchannel;
	QModelIndex index;
	extern bool g_bIsMainDlgDeleting;

	if (g_bIsMainDlgDeleting) return;

	index = ui->tableWidgetVideoChannel->selectionModel()->currentIndex();
	vchannel = ui->tableWidgetVideoChannel->item(index.row(), index.column())->text().toInt() - 1;

	if (m_videoChannel == -1)//修正bug, 当调用ResetState()时不应调用该函数
	{
		return;
	}

	if (vchannel == m_videoChannel)
	{
		return;
	}

	printf("lg vchannel:%d m_videoChannel:%d\n", vchannel, m_videoChannel);

	((MctDialog *)parent())->m_bHookCloseMctDlg = false;

	//printf("lg 3\n");
	lpc_cmd_terminate(lphone, NULL);

	m_videoChannel = vchannel;
	usleep(400000);
	//printf("lg %s\n", (m_strName + ":" + QString::number(vchannel)).toLatin1().data());

	printf("lg mct hook lpc_cmd_mon\n");
	lpc_cmd_mon(lphone, (m_strName + ":" + QString::number(vchannel)).toLatin1().data());

	((Application *)qApp)->setLCDOn();
}

void MctHookDialog::on_pushButtonVolumeSub_clicked()
{
	ui->horizontalSliderVolume->triggerAction(QAbstractSlider::SliderPageStepSub);
}

void MctHookDialog::on_pushButtonVolumeAdd_clicked()
{
	ui->horizontalSliderVolume->triggerAction(QAbstractSlider::SliderPageStepAdd);
}

void MctHookDialog::on_pushButtonEmergency_clicked()
{
	on_pushButtonReturn_clicked();

	TerminalHelpMainDialog::showTerminalHelpDialog();
}

void MctHookDialog::on_pushButtonHelp_clicked()
{
	on_pushButtonReturn_clicked();

	HelpDialog::showHelpDialog("help1.txt");
}
