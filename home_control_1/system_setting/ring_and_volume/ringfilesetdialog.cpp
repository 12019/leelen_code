#include "ringfilesetdialog.h"
#include "ui_ringfilesetdialog.h"
#include <QStandardItemModel>
#include <dirent.h>
#include <unistd.h>
#include <QMessageBox>
#include "data_types.h"
#include "syssetmaindialog.h"
#include <stdio.h>
#include "mediastreamer2/mediastream.h"
#include "maindialog.h"
#include "ui_syssetmaindialog.h"
#include "ui_ringandvolumedialog.h"
#include "ringandvolumedialog.h"

#include <global.h>

bool g_Digital_Channel_Flag = true;				// 标识数字通道的变量

extern Kevin::Global  *g_pScaleSrc;
extern MainDialog *g_pMainDlg;

extern char g_strLphoneRingFile[5][50];
static char strTempDoorBellRingFile[50]; 			// 用来临时保存二次门铃的变量
/************************************************************
  该文件为铃声选择对话框主程序
************************************************************/

/************************************************************
描述：铃声选择对话框构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
RingFileSetDialog::RingFileSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RingFileSetDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

	if 	( g_pScaleSrc->get_screen_version() == 1 )  // 800x480
	{
		// need restore the old ui from lilin-tech-6410
	}
	else if ( g_pScaleSrc->get_screen_version() == 2 ) // 1024x600
	{
        ui->framePanel->setGeometry(QRect(44, 12, 700, 570));

        //标题
        ui->label_title->setGeometry(QRect(139, 22, 510, 50));

        //右侧按键
        ui->pushButtonRingFileSetSave->setGeometry(QRect(790, 380, 200, 100));
        ui->pushButtonRingFileSetCancel->setGeometry(QRect(790, 480, 200, 100));

        ui->tableViewRingFile->setGeometry(QRect(64, 92, 660, 480));
	}
	else if ( g_pScaleSrc->get_screen_version() == 3 )  // 1280x800
	{
        ui->framePanel->setGeometry(QRect(59, 30, 858, 745));

        //标题
        ui->label_title->setGeometry(QRect(219, 40, 510, 70));

        //右侧按键
		ui->pushButtonRingFileSetCancel->setGeometry(QRect(980, 660, 246, 112));
        ui->pushButtonRingFileSetSave->setGeometry(QRect(980, 530, 246, 112));

		ui->tableViewRingFile->setGeometry(QRect(90, 130, 800, 617));

	}

	ui->tableViewRingFile->horizontalHeader()->setObjectName("tvRingFileHeader");
	setAttribute(Qt::WA_TranslucentBackground, false);

	QStandardItemModel *model = new QStandardItemModel(0, 1);
	model->setHeaderData(0, Qt::Horizontal, tr("铃声"));
	ui->tableViewRingFile->setModel(model);
	ui->tableViewRingFile->verticalHeader()->hide();
	ui->tableViewRingFile->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableViewRingFile->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableViewRingFile->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableViewRingFile->setSortingEnabled(false);
	ui->tableViewRingFile->setShowGrid(false);
	ui->tableViewRingFile->setFocusPolicy(Qt::NoFocus);
        ui->tableViewRingFile->horizontalHeader()->setHighlightSections(false);

	ui->tableViewRingFile->verticalHeader()->hide();
	ui->tableViewRingFile->horizontalHeader()->hide();

	int column_width = ui->tableViewRingFile->geometry().width();
	ui->tableViewRingFile->setColumnWidth(0, column_width);

	ui->tableViewRingFile->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);

	m_pPushButton = NULL;

	m_pBackFromSave = false;
}

/************************************************************
描述：铃声选择对话框析构函数
参数：无
返回：无
************************************************************/
RingFileSetDialog::~RingFileSetDialog()
{
	if ( !doorbell_flag )
		stopWaveFile();
	else
	{
		Switch_Channel_Sound(false);
		g_pScaleSrc->doorbell_power(false);
		g_pMainDlg->stop_pwm_ring();
	}
	delete ui;
}

/************************************************************
描述： 数字模拟通道的切换
参数： v false 为数字 true 为模拟
返回：无
************************************************************/
void RingFileSetDialog::Switch_Channel_Sound(bool v)
{
	char *cmd = new char[100];

	if ( v == true )
	{
		sprintf(cmd,"1");
		g_pScaleSrc->Write_fm1188(cmd);
		g_Digital_Channel_Flag = false; 
	}
	else
	{
		sprintf(cmd,"0");
		g_pScaleSrc->Write_fm1188(cmd);
		g_Digital_Channel_Flag = true;
	}

	delete[] cmd;
	cmd = NULL;
}

/************************************************************
描述：铃声选择对话框构造函数
参数：搜索/mnt/disk/rings下的wav文件，将搜到的文件名加到铃声列表控
	 件中
返回：无
************************************************************/
void RingFileSetDialog::loadRingFile()
{
	DIR *dp;
	struct dirent *dirp;
	int len;
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewRingFile->model();
	QStandardItem *item;
	int i;

	model->removeRows(0, model->rowCount());

	if((dp = opendir("/mnt/disk/rings")) == NULL)
	{
		setAllButtonsEnabled((QDialog *)parent(), false);

		MyMessageBox messBox(((SysSetMainDialog *)parent())->m_curDlg);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		//Q_UNUSED(okButton);
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("打开铃声目录失败!"));
		messBox.show();
		QRect rc = messBox.geometry();
		QRect rc2 = geometry();
		rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
		messBox.setGeometry(rc);
		messBox.exec();

		setAllButtonsEnabled((QDialog *)parent(), true);

		return;
	}

	i = 0;
	while((dirp = readdir(dp)) != NULL)
	{
		if(dirp->d_type == DT_DIR)
		{
			continue;
		}

		len = strlen(dirp->d_name);
		if(strcasecmp(&dirp->d_name[len - 4],".wav") !=0)
		{
			continue;
		}

		item = new QStandardItem(QString::fromUtf8(dirp->d_name));//QString::fromUtf8(txtpath.toLatin1().data()));
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		if (i % 2 == 0)
			item->setBackground(QBrush(TABLE_VIEW_COLOR));
		else
			item->setBackground(QBrush(QColor(255, 255, 255)));

		model->setItem(i, 0, item);

		i++;
	}

	ui->tableViewRingFile->selectRow(0);

	disconnect(ui->tableViewRingFile->selectionModel(),
			   SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
			   this, SLOT(playSelectedWavFile(const QModelIndex &, const QModelIndex &)));
	connect(ui->tableViewRingFile->selectionModel(),
		SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
		this, SLOT(playSelectedWavFile(const QModelIndex &, const QModelIndex &)));
	
	doorbell_flag = false;
}

/************************************************************
描述：Door Bell 铃声选择
参数：
返回：
************************************************************/
void RingFileSetDialog::loadDoorBellRingFile()
{
	struct dirent *dirp;
	int len;
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewRingFile->model();
	QStandardItem *item;

	model->removeRows(0, model->rowCount());

	char *buf = new char[10];
	for ( int i = 0; i < 2; i += 1 ) 
	{
		sprintf(buf,"ring%d",i+1);
		item = new QStandardItem(QString::fromUtf8(buf));
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		if (i % 2 == 0)
			item->setBackground(QBrush(TABLE_VIEW_COLOR));
		else
			item->setBackground(QBrush(QColor(255, 255, 255)));

		model->setItem(i, 0, item);
	}
	delete[] buf;
	buf = NULL;

	ui->tableViewRingFile->selectRow(0);
	doorbell_flag = true;

	disconnect(ui->tableViewRingFile->selectionModel(),
			   SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
			   this, SLOT(playDoorBellFile(const QModelIndex &, const QModelIndex &)));
	connect(ui->tableViewRingFile->selectionModel(),
		SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
		this, SLOT(playDoorBellFile(const QModelIndex &, const QModelIndex &)));
}

/************************************************************
描述：在铃声列表控件中选中指定文件名的铃声，并播放该铃声
参数：strRingFile - 待选中的指定文件名
返回：0 - 成功
	 -1 - 失败
************************************************************/
int RingFileSetDialog::selectRingFile(QString strRingFile)
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewRingFile->model();
	int i;
	int row = ui->tableViewRingFile->currentIndex().row();

	for (i = 0; i < model->rowCount(); i++)
	{
		if (model->item(i, 0)->text() == strRingFile)
		{
			if (i != row)
			{
				ui->tableViewRingFile->selectRow(i);
			}
			else
			{
				QString strWaveFile = model->item(i, 0)->text();

				repaint();
				strWaveFile = "rings/" + strWaveFile;

				//stopWaveFile();

				if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
					usleep(50000);;
				playWaveFile(strWaveFile.toLatin1().data(), ((MainDialog *)(parent()->parent()))->m_pVoipMainDlg->m_nRingVolume, 0);
			}

            return 0;
		}
	}

	return -1;
}

/************************************************************
描述：选择门铃声音文件
参数：无
返回：无
************************************************************/
int RingFileSetDialog::selectDoorBellFile(QString strRingFile)
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewRingFile->model();
	int i;
	int row = ui->tableViewRingFile->currentIndex().row();
	for (i = 0; i < model->rowCount(); i++)
	{
		if (model->item(i, 0)->text() == strRingFile)
		{
			if (i != row)
			{
				ui->tableViewRingFile->selectRow(i);
			}
			else
			{
				repaint();
			}

			strcpy(strTempDoorBellRingFile,g_strLphoneRingFile[2]); 		   // 进行原铃声备份
			strcpy(g_strLphoneRingFile[2],("rings/" + strRingFile).toLatin1().data()); // 进行原铃声保存

			Switch_Channel_Sound(true);
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
			g_pMainDlg->start_pwm_ring();

			return 0;
		}
	}

	return -1;
}

/************************************************************
描述：保存选中的铃声文件，关闭本窗口
参数：无
返回：无
************************************************************/
void RingFileSetDialog::on_pushButtonRingFileSetSave_pressed ()
{
	if (!isVisible()) return;

	hide();
	((SysSetMainDialog *)parent())->m_curDlg->show();

	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewRingFile->model();
	int row = ui->tableViewRingFile->currentIndex().row();
	if ((row != -1) && m_pPushButton)
	{
		QString str = model->item(row, 0)->text();
		if ( !doorbell_flag )
			str = str.left(str.length() - 4);
		m_pPushButton->setText(str);
	}
	if (m_pPushButton) m_pPushButton->setFocus(Qt::TabFocusReason);

	m_pBackFromSave = true;

	if ( g_pScaleSrc->get_screen_version() == 2 || g_pScaleSrc->get_screen_version() == 3  )
	{
		SysSetMainDialog *pMainDlg = (SysSetMainDialog *)(parent());
	}
}

/************************************************************
描述：取消铃声选择，关闭本窗口
参数：无
返回：无
************************************************************/
void RingFileSetDialog::on_pushButtonRingFileSetCancel_pressed()
{
	if (!isVisible()) return;

	strcpy(g_strLphoneRingFile[2],strTempDoorBellRingFile); // 进行原铃声还原

	hide();
	((SysSetMainDialog *)parent())->m_curDlg->show();

    	if ( g_pScaleSrc->get_screen_version() == 2 || g_pScaleSrc->get_screen_version() == 3  )
	{
		SysSetMainDialog *pMainDlg = (SysSetMainDialog *)(parent());
	}
}

/************************************************************
描述：对话框返回
参数：无
返回：无
************************************************************/
void RingFileSetDialog::Remote_Call_Return()
{
	on_pushButtonRingFileSetCancel_pressed();
}

/************************************************************
描述：在铃声列表控件中选择某个铃声后，开始播放该铃声
参数：current - 列表控件当前选择项序号
	 previous - 列表控件以前选择项序号
返回：无
************************************************************/
void RingFileSetDialog::playSelectedWavFile(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(previous);

	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewRingFile->model();
	QString strWaveFile = model->item(current.row(), 0)->text();

	repaint();
	strWaveFile = "rings/" + strWaveFile;

	if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
		usleep(50000);;
    stopWaveFile();
	playWaveFile(strWaveFile.toLatin1().data(), ((MainDialog *)(parent()->parent()))->m_pVoipMainDlg->m_nRingVolume, 0);
}

/************************************************************
描述：在铃声列表控件中选择某个铃声后，开始播放该铃声
参数：current - 列表控件当前选择项序号
	 previous - 列表控件以前选择项序号
返回：无
************************************************************/
void RingFileSetDialog::playDoorBellFile(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(previous);

	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewRingFile->model();
	QString strWaveFile = model->item(current.row(), 0)->text();

	strcpy(g_strLphoneRingFile[2],("rings/" + strWaveFile).toLatin1().data()); // 进行铃声设置

	repaint();

    //g_pScaleSrc->doorbell_power(true);
    //g_pMainDlg->start_pwm_ring();
}

/************************************************************
描述：窗口显示回调函数
参数：无
返回：无
************************************************************/
void RingFileSetDialog::showEvent(QShowEvent *)
{
	//MainDialog *pMainDlg = (MainDialog *)(parent()->parent());
	//pMainDlg->refreshTabFocus();
	//((SysSetMainDialog *)parent())->ui->pushButtonRingSet->setFocus(Qt::TabFocusReason);
	//ui->pushButtonCancel->setFocus(Qt::TabFocusReason);

	g_pMainDlg->showRefreshTabFocus(this);
}

/************************************************************
描述：窗口隐藏时，停止铃声播放
参数：无
返回：无
************************************************************/
void RingFileSetDialog::hideEvent(QHideEvent *)
{
	g_pMainDlg->hideRefreshTabFocus(this);

	if ( !doorbell_flag )
		stopWaveFile();
	else
	{
		Switch_Channel_Sound(false);
		g_pScaleSrc->doorbell_power(false);
		g_pMainDlg->stop_pwm_ring();
	}

	if (QApplication::focusWidget()->parent() != this) return;

	if (m_pPushButton) m_pPushButton->setFocus(Qt::TabFocusReason);
}
