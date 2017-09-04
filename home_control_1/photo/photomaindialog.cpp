#include "photomaindialog.h"
#include "ui_photomaindialog.h"
#include "maindialog.h"
#include "ui_maindialog.h"
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/mount.h>
#include <QStandardItemModel>
#include "application.h"
#include <linux/watchdog.h>

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

#define PHOTO_PLAY_INTERVAL		4000 //ms

extern MainDialog *g_pMainDlg;

PhotoMainDialog::PhotoMainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PhotoMainDialog)
{
	ui->setupUi(this);
	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}
	m_pPhotoUDiskDlg = new PhotoUDiskDialog(this);

	m_pFrameFullshow = new QFrame(this);
	m_pFrameFullshow->setGeometry(QRect(0, 0, qApp->desktop()->size().width(), qApp->desktop()->size().height()));

	int x,y,w,h;
	g_pScaleSrc->get_info_framepanel_geometry(x,y,w,h);
	ui->framePanel->setGeometry(x,y,w,h);

	m_pFrameFullshow->setFrameShadow(QFrame::Plain);
	m_pFrameFullshow->setFrameShape(QFrame::Panel);

	m_pFrameFullshow->setStyleSheet("background-color: #000000;");

	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	m_pFrameFullshow->installEventFilter(this);
	m_pPicLabel = new QLabel(m_pFrameFullshow);

	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(slotPlayTimeout()));

	m_nextPlayRow = 0;

	QStandardItemModel *model = new QStandardItemModel(0, 4);
	ui->tableViewPhotoList->setModel(model);

	g_pScaleSrc->get_tabviewinfolist_geometry(x,y,w,h);
	ui->tableViewPhotoList->setGeometry(x,y,w,h);

	ui->tableViewPhotoList->verticalHeader()->hide();
	ui->tableViewPhotoList->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableViewPhotoList->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableViewPhotoList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableViewPhotoList->setSortingEnabled(false);
	ui->tableViewPhotoList->setShowGrid(false);
	ui->tableViewPhotoList->setFocusPolicy(Qt::NoFocus);
        ui->tableViewPhotoList->horizontalHeader()->setHighlightSections(false);

	ui->tableViewPhotoList->setIconSize(QSize(g_pScaleSrc->get_w_value(32), g_pScaleSrc->get_h_value(32)));

	g_pScaleSrc->get_photo_tabviewinfolist_width(x,y,w);

	ui->tableViewPhotoList->setColumnWidth(0, x);
	ui->tableViewPhotoList->setColumnWidth(1, y);
	ui->tableViewPhotoList->setColumnWidth(2, w);
	ui->tableViewPhotoList->setColumnWidth(3, 50);

	ui->tableViewPhotoList->setColumnHidden(3, true);;

	ui->tableViewPhotoList->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
	ui->tableViewPhotoList->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
	ui->tableViewPhotoList->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
	ui->tableViewPhotoList->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);

	translateTableViewPhotoList();

	g_pScaleSrc->get_info_label_geometry(x,y,w,h);
	ui->label_title->setGeometry(x,y,w,h);

    int res = g_pScaleSrc->get_screen_version();
    if ( res == 3 )  // 1280x800
    {
        ui->pushButtonPhotoMainPrev->setGeometry(QRect(959, 29, 279, 99));
        ui->pushButtonPhotoMainNext->setGeometry(QRect(959, 137, 279, 99));
        ui->pushButtonPhotoMainCheck->setGeometry(QRect(959, 245, 279, 99));
        ui->pushButtonPhotoMainImport->setGeometry(QRect(959, 353, 279, 99));
        ui->pushButtonPhotoMainExport->setGeometry(QRect(959, 461, 279, 99));
        ui->pushButtonPhotoMainDel->setGeometry(QRect(959, 569, 279, 99));
        ui->pushButtonPhotoMainReturn->setGeometry(QRect(959, 677, 279, 99));
    }
    else if (res == 2) // 1024x600
    {
        ui->pushButtonPhotoMainPrev->setGeometry(QRect(764, 20, 230, 80));
        ui->pushButtonPhotoMainNext->setGeometry(QRect(764, 100, 230, 80));
        ui->pushButtonPhotoMainCheck->setGeometry(QRect(764, 180, 230, 80));
        ui->pushButtonPhotoMainImport->setGeometry(QRect(764, 260, 230, 80));
        ui->pushButtonPhotoMainExport->setGeometry(QRect(764, 340, 230, 80));
        ui->pushButtonPhotoMainDel->setGeometry(QRect(764, 420, 230, 80));
        ui->pushButtonPhotoMainReturn->setGeometry(QRect(764, 500, 230, 80));
    }
}

PhotoMainDialog::~PhotoMainDialog()
{
	delete m_pPicLabel;
	delete m_pFrameFullshow;
	delete m_pTimer;

	delete m_pPhotoUDiskDlg;

	delete ui;
}

void PhotoMainDialog::translateTableViewPhotoList()
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewPhotoList->model();

	model->setHeaderData(0, Qt::Horizontal, tr("名称"));
	model->setHeaderData(1, Qt::Horizontal, tr("大小"));
	model->setHeaderData(2, Qt::Horizontal, tr("修改时间"));
	model->setHeaderData(3, Qt::Horizontal, tr("目录/文件"));
}

bool PhotoMainDialog::load(QString strDir)
{
#ifdef ARM11
	umount("/udisk");
	if (access("/udisk", F_OK) != 0)
	{
		mkdir("/udisk", 0777);
	}

	if (mount("/dev/udisk", "/udisk", "vfat", 0, 0) < 0)
	{
		if (mount("/dev/sda1", "/udisk", "vfat", 0, 0) < 0)
		{
			mount("/dev/sdb1", "/udisk", "vfat", 0, 0);
		}
	}
#endif

	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewPhotoList->model();
	model->removeRows(0, model->rowCount());

	DIR *dp;
	struct dirent *dirp;
	char path[512];
	char pathPrefix[512];

	strcpy(pathPrefix, strDir.toLocal8Bit().data());

	if ((dp = opendir(pathPrefix)) == NULL)
	{
		setAllButtonsEnabled(this, false);
		ui->tableViewPhotoList->setEnabled(false);
		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("打开系统图片目录失败"));
		messBox.exec();
		setAllButtonsEnabled(this, true);
		ui->tableViewPhotoList->setEnabled(true);

		return false;
	}

	m_curDir = strDir;

	while ((dirp = readdir(dp)) != NULL)
	{
		sprintf(path, "%s/%s", pathPrefix, dirp->d_name);

		struct stat buf;
		if (stat(path, &buf) >= 0)
		{
			if (strcmp(dirp->d_name, ".") == 0) // | fileInfo.fileName()=="..")
			{
			   continue;
			}

			if ((strDir == "/mnt/disk/photo") && (strcmp(dirp->d_name, "..") == 0))
			{
				continue;
			}

			QStandardItem *item;
			int row = model->rowCount();
			QString strTemp;

			if (S_ISDIR(buf.st_mode))
            {
				item = new QStandardItem(QIcon(":/images/photo/folder.ico"), dirp->d_name);
				item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
				model->setItem(row, 0, item);

				if (strcmp(dirp->d_name, "..") == 0)
				{
					item = new QStandardItem("0");
					model->setItem(row, 3, item);
				}
				else
				{
					struct tm *lmodifytime;
					lmodifytime = gmtime(&(buf.st_mtime));

					strTemp.sprintf("%d/%d/%d %d:%02d", lmodifytime->tm_year + 1900, lmodifytime->tm_mon + 1,
								lmodifytime->tm_mday, lmodifytime->tm_hour, lmodifytime->tm_min);
					item = new QStandardItem(strTemp);
					item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
					model->setItem(row, 2, item);

					item = new QStandardItem("1");
					model->setItem(row, 3, item);
				}

                ui->tableViewPhotoList->setRowHeight(row, g_pScaleSrc->get_public_tableviewlist_row_height());
			}
			else
			{
				int len = strlen(dirp->d_name);

				if ((strcasecmp(&dirp->d_name[len - 4], ".bmp") == 0)
					|| (strcasecmp(&dirp->d_name[len - 4], ".jpg") == 0)
					|| (strcasecmp(&dirp->d_name[len - 4], ".gif") == 0)
					|| (strcasecmp(&dirp->d_name[len - 4], ".png") == 0)
					|| (strcasecmp(&dirp->d_name[len - 4], ".tif") == 0)
					|| (strcasecmp(&dirp->d_name[len - 5], ".tiff") == 0))
				{
					item = new QStandardItem(QIcon(":/images/photo/file.ico"), dirp->d_name);
					item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
					model->setItem(row, 0, item);

					int size = buf.st_size;
					if (size < 1024)
					{
						strTemp = "1KB";
					}
					else
					{
						strTemp.sprintf("%dKB", size / 1024);
					}
					item = new QStandardItem(strTemp);
					item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
					model->setItem(row, 1, item);

					struct tm *lmodifytime;
					lmodifytime = gmtime(&(buf.st_mtime));

					strTemp.sprintf("%d/%d/%d %d:%02d", lmodifytime->tm_year + 1900, lmodifytime->tm_mon + 1,
								lmodifytime->tm_mday, lmodifytime->tm_hour, lmodifytime->tm_min);
					item = new QStandardItem(strTemp);
					item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
					model->setItem(row, 2, item);

					item = new QStandardItem("2");
					model->setItem(row, 3, item);

					ui->tableViewPhotoList->setRowHeight(row, g_pScaleSrc->get_public_tableviewlist_row_height());
				}
			}
		}
	}

	closedir(dp);

	if (model->rowCount() > 0)
	{
		ui->tableViewPhotoList->sortByColumn(0, Qt::AscendingOrder);
		ui->tableViewPhotoList->sortByColumn(3, Qt::AscendingOrder);

		if ((model->item(0, 0)->text() == "..") && (model->rowCount() > 1))
		{
			ui->tableViewPhotoList->setCurrentIndex(model->index(1, 0));
		}
		else
		{
			ui->tableViewPhotoList->setCurrentIndex(model->index(0, 0));
		}

		ui->tableViewPhotoList->verticalScrollBar()->setSliderPosition(0);
	}

	return true;
}

void PhotoMainDialog::on_pushButtonPhotoMainImport_pressed()
{
	if (m_pPhotoUDiskDlg->isVisible()) return;

	m_pPhotoUDiskDlg->show();
	m_pPhotoUDiskDlg->load("/udisk");
}

void PhotoMainDialog::on_pushButtonPhotoMainReturn_pressed()
{
	hide();
}

void PhotoMainDialog::showEvent(QShowEvent *)
{
	g_pMainDlg->showRefreshTabFocus(this);
}

void PhotoMainDialog::hideEvent(QHideEvent *)
{
	m_pTimer->stop();

	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	m_pPhotoUDiskDlg->hide();

	g_pMainDlg->hideRefreshTabFocus(this);

	if (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg)
	{
		g_pMainDlg->ui->pushButtonPhoto->setFocus(Qt::TabFocusReason);
	}
}

void PhotoMainDialog::on_pushButtonPhotoMainCheck_pressed()
{
	if (((Application *)qApp)->m_bTimerOff) return;
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewPhotoList->model();
	int row = ui->tableViewPhotoList->currentIndex().row();
	if (row < 0) return;

	QString strMode = model->item(row, 3)->text();
	if (strMode == "0")
	{
		load(m_curDir.left(m_curDir.lastIndexOf("/")));
	}
	else if (strMode == "1")
	{
		QString strSubDir = model->item(row, 0)->text();
		load(m_curDir + "/" + strSubDir);
	}
	else
	{
		((Application *)qApp)->setLCDOn();

		m_nextPlayRow = row;

		slotPlayTimeout();

		if (((Application *)qApp)->m_bTimerOff)
		{
			m_pFrameFullshow->show();
			m_pFrameFullshow->raise();
		}
	}
}

void PhotoMainDialog::on_pushButtonPhotoMainPrev_pressed()
{
	//QStandardItemModel *model = (QStandardItemModel *)ui->tableViewPhotoList->model();
	int row = ui->tableViewPhotoList->verticalScrollBar()->value();

	if (row - TABLE_ROWS_PER_PAGE >= 0)
	{
		ui->tableViewPhotoList->selectRow(row - TABLE_ROWS_PER_PAGE);
		ui->tableViewPhotoList->verticalScrollBar()->setSliderPosition(row - TABLE_ROWS_PER_PAGE);
	}
	else if (row > 0)
	{
		ui->tableViewPhotoList->selectRow(0);
		ui->tableViewPhotoList->verticalScrollBar()->setSliderPosition(0);
	}
	/*else if (model->rowCount() > TABLE_ROWS_PER_PAGE)
	{
		ui->tableViewPhotoList->selectRow(model->rowCount() - TABLE_ROWS_PER_PAGE);
		ui->tableViewPhotoList->verticalScrollBar()->setSliderPosition(model->rowCount() - TABLE_ROWS_PER_PAGE);
	}*/
}

void PhotoMainDialog::on_pushButtonPhotoMainNext_pressed()
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewPhotoList->model();
	int row = ui->tableViewPhotoList->verticalScrollBar()->value(); //ui->tableViewInfoList->currentIndex().row();

	if (row + TABLE_ROWS_PER_PAGE < model->rowCount())
	{
		ui->tableViewPhotoList->selectRow(row + TABLE_ROWS_PER_PAGE);
		ui->tableViewPhotoList->verticalScrollBar()->setSliderPosition(row + TABLE_ROWS_PER_PAGE);
	}
	/*else
	{
		ui->tableViewPhotoList->selectRow(0);
		ui->tableViewPhotoList->verticalScrollBar()->setSliderPosition(0);
	}*/
}

bool PhotoMainDialog::deleteDir(const QString dirName)
{
	QDir directory(dirName);

	if (!directory.exists())
	{
		return true;
	}

	QStringList files = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);

	QList <QString>::iterator f = files.begin();

	bool error = false;

	for (; f != files.end(); ++f)
	{
		QString filePath = QDir::convertSeparators(directory.path() + '/' + (*f));
		QFileInfo fi(filePath);
		if (fi.isFile() || fi.isSymLink())
		{
			QFile::setPermissions(filePath, QFile::WriteOwner);
			if (!QFile::remove(filePath))
			{
				//DBG(qDebug() << "Global::deleteDir 1" << filePath << "faild");
				error = true;
			}
		}
		else if (fi.isDir())
		{
			if (!deleteDir(filePath))
			{
				error = true;
			}
		}
	}

	if(!directory.rmdir(QDir::convertSeparators(directory.path())))
	{
		//DBG(qDebug() << "Global::deleteDir 3" << directory.path()  << "faild");
		error = true;
	}

	return !error;
}

void PhotoMainDialog::on_pushButtonPhotoMainDel_pressed()
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewPhotoList->model();
	int row = ui->tableViewPhotoList->currentIndex().row();
	if (row < 0) return;

	QString strMode = model->item(row, 3)->text();

	if (strMode == "0") return;

	setAllButtonsEnabled(this, false);
	ui->tableViewPhotoList->setEnabled(false);
	MyMessageBox messBox(this);
	CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
	CommonPushButton *cancelButton = messBox.addButton(QObject::tr("取消"), "pushButtonMyMessageBoxCancel");
	Q_UNUSED(okButton);
	cancelButton->setFocus(Qt::TabFocusReason);
	if (strMode == "1")
	{
		messBox.setText(tr("确认删除该目录及里面的全部内容?"));
	}
	else
	{
		messBox.setText(tr("确认删除该文件?"));
	}
	messBox.exec();
	if(messBox.clickedButton() == cancelButton)
	{
		setAllButtonsEnabled(this, true);
		ui->tableViewPhotoList->setEnabled(true);
        ui->pushButtonPhotoMainDel->setFocus(Qt::TabFocusReason);

		return;
	}

	//QString strCmd = "/bin/rm -rf " + m_curDir + "/" + model->item(row, 0)->text();
	//printf("lg %s\n", strCmd.toLocal8Bit().data());
	//system(strCmd.toLocal8Bit().data());

	bool bTmp;
	QString strPath = m_curDir + "/" + model->item(row, 0)->text();
	if (strMode == "1")
	{
		bTmp = deleteDir(strPath);
	}
	else
	{
		bTmp = QFile::remove(strPath);
	}

	load(m_curDir);

	//setAllButtonsEnabled(this, false);
	//ui->tableViewPhotoList->setEnabled(false);
	MyMessageBox messBox2(this);
	CommonPushButton *okButton2 = messBox2.addButton(QObject::tr("确定"));
	okButton2->setFocus(Qt::TabFocusReason);
	QString strTmp;
	if (strMode == "1")
	{
		if (bTmp)
		{
			strTmp = tr("目录已删除");
		}
		else
		{
			strTmp = tr("目录删除失败");
		}
	}
	else
	{
		if (bTmp)
		{
			strTmp = tr("文件已删除");
		}
		else
		{
			strTmp = tr("文件删除失败");
		}
	}
	messBox2.setText(strTmp);
	messBox2.exec();
	setAllButtonsEnabled(this, true);
	ui->tableViewPhotoList->setEnabled(true);
	ui->pushButtonPhotoMainDel->setFocus(Qt::TabFocusReason);
}

void PhotoMainDialog::on_pushButtonPhotoMainExport_pressed()
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewPhotoList->model();
	int row = ui->tableViewPhotoList->currentIndex().row();
	if (row < 0) return;

	QString strMode = model->item(row, 3)->text();
	if (strMode == "0") return;

#ifdef ARM11
	if (umount("/udisk") != EBUSY)
	{
		if (access("/udisk", F_OK) != 0)
		{
			mkdir("/udisk", 0777);
		}

		if ((mount("/dev/udisk", "/udisk", "vfat", 0, 0) < 0)
			&& (mount("/dev/sdb1", "/udisk", "vfat", 0, 0) < 0))
#endif

#ifdef CORTEXA7
		if (((opendir("/udisk")) == NULL))
#endif
		{
			setAllButtonsEnabled(this, false);
			ui->tableViewPhotoList->setEnabled(false);
			MyMessageBox messBox(this);
			CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
			okButton->setFocus(Qt::TabFocusReason);
			messBox.setText(tr("U盘挂载失败"));
			messBox.exec();
			setAllButtonsEnabled(this, true);
			ui->tableViewPhotoList->setEnabled(true);
			ui->pushButtonPhotoMainExport->setFocus(Qt::TabFocusReason);

			return;
		}
#ifdef ARM11
	}
#endif

	QString strFileName = model->item(row, 0)->text();
	QStringList strDirs = m_curDir.mid(1).split("/");
	QString strDir = "/udisk";

	if (strMode == "1")
	{
		strDirs.append(strFileName);
	}

	int i;
	for (i = 3; i < strDirs.count(); i++)
	{
		strDir += "/" + strDirs.at(i);

		struct stat st;
		if (stat(strDir.toLocal8Bit().data(), &st) >= 0)
		{
			if (!S_ISDIR(st.st_mode))
			{
				break;
			}
		}
		else
		{
			mkdir(strDir.toLocal8Bit().data(), 0777);
		}
	}

	if (i < strDirs.count())
	{
		setAllButtonsEnabled(this, false);
		ui->tableViewPhotoList->setEnabled(false);
		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("U盘中有文件和待导出的\n目录名相同, 导出失败"));
		messBox.exec();
		setAllButtonsEnabled(this, true);
		ui->tableViewPhotoList->setEnabled(true);
		ui->pushButtonPhotoMainExport->setFocus(Qt::TabFocusReason);

		return;
	}

	if (strMode == "1")
	{
		setAllButtonsEnabled(this, false);
		ui->tableViewPhotoList->setEnabled(false);

		copy_overwrite = OVERWRITE_NORMAL;
		bool bRet = copyDir(this, (m_curDir + "/" + strFileName).toLocal8Bit().data(),
							strDir.toLocal8Bit().data(), (char *)"/udisk");

		if (bRet)
		{
			MyMessageBox messBox(this);
			CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
			okButton->setFocus(Qt::TabFocusReason);
			messBox.setText(tr("文件夹导出完毕"));
			messBox.exec();
		}

		setAllButtonsEnabled(this, true);
		ui->tableViewPhotoList->setEnabled(true);
		ui->pushButtonPhotoMainImport->setFocus(Qt::TabFocusReason);
	}
	else
	{
		strDir += "/" + strFileName;

		struct stat st;
		if (stat(strDir.toLocal8Bit().data(), &st) >= 0)
		{
			if (S_ISDIR(st.st_mode))
			{
				setAllButtonsEnabled(this, false);
				ui->tableViewPhotoList->setEnabled(false);
				MyMessageBox messBox(this);
				CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
				okButton->setFocus(Qt::TabFocusReason);
				messBox.setText(tr("U盘中有目录和待导出的\n文件名相同，导出失败"));
				messBox.exec();
				setAllButtonsEnabled(this, true);
				ui->tableViewPhotoList->setEnabled(true);
				ui->pushButtonPhotoMainImport->setFocus(Qt::TabFocusReason);

				return;
			}
			else
			{
				setAllButtonsEnabled(this, false);
				ui->tableViewPhotoList->setEnabled(false);
				MyMessageBox messBox(this);
				CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
				CommonPushButton *cancelButton = messBox.addButton(QObject::tr("取消"), "pushButtonMyMessageBoxCancel");
				Q_UNUSED(okButton);
				cancelButton->setFocus(Qt::TabFocusReason);
				messBox.setText(tr("U盘中有文件和待导出的\n文件名相同，覆盖该文件?"));
				messBox.exec();
				setAllButtonsEnabled(this, true);
				ui->tableViewPhotoList->setEnabled(true);
				ui->pushButtonPhotoMainImport->setFocus(Qt::TabFocusReason);
				if(messBox.clickedButton() == cancelButton)
				{
					return;
				}

				QFile::remove(strDir);
			}
		}

		setAllButtonsEnabled(this, false);
		ui->tableViewPhotoList->setEnabled(false);
		bool bRet = QFile::copy(m_curDir + "/" + strFileName, strDir);
		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		okButton->setFocus(Qt::TabFocusReason);
		if (bRet)
		{
			messBox.setText(tr("文件导出完毕"));
		}
		else
		{
			messBox.setText(tr("文件导出失败"));
		}
		messBox.exec();
		setAllButtonsEnabled(this, true);
		ui->tableViewPhotoList->setEnabled(true);
		ui->pushButtonPhotoMainImport->setFocus(Qt::TabFocusReason);
	}
}

bool PhotoMainDialog::eventFilter(QObject *o, QEvent *e)
{
	if (e->type() == QEvent::MouseButtonPress)
	{
		if (o == m_pFrameFullshow)
		{
			m_pTimer->stop();

			m_pFrameFullshow->hide();
			m_pFrameFullshow->lower();

			((Application *)qApp)->setLCDAndTimerOn();
		}
	}

	return false;
}

void PhotoMainDialog::slotPlayTimeout()
{
	m_pTimer->stop();

	int timeout = 30;
	if (g_pMainDlg->m_watchdog_fd >= 0)
	{
		ioctl(g_pMainDlg->m_watchdog_fd, WDIOC_SETTIMEOUT, &timeout);
	}

	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewPhotoList->model();
	ui->tableViewPhotoList->setCurrentIndex(model->index(m_nextPlayRow, 0));

	QString strPicFileName = model->item(m_nextPlayRow, 0)->text();
	QString strPicPath = m_curDir + "/" + strPicFileName;
	QPixmap pixmap(strPicPath);

	timeout = 8;
	if (g_pMainDlg->m_watchdog_fd >= 0)
	{
		ioctl(g_pMainDlg->m_watchdog_fd, WDIOC_SETTIMEOUT, &timeout);
	}

	if ((pixmap.width() == 0) || (pixmap.height() == 0))
	{
		m_pFrameFullshow->hide();
		m_pFrameFullshow->lower();

		setAllButtonsEnabled(this, false);
		ui->tableViewPhotoList->setEnabled(false);
		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("图像打开失败"));
		messBox.exec();
		setAllButtonsEnabled(this, true);
		ui->tableViewPhotoList->setEnabled(true);

		((Application *)qApp)->setLCDAndTimerOn();

		return;
	}

	if ( g_pScaleSrc->get_screen_version() == 1 )
	{
		if (((pixmap.width() > 1024) && (pixmap.height() > 1024))
			|| (pixmap.width() * pixmap.height() > 1024 * 1024))
		{
			m_pFrameFullshow->hide();
			m_pFrameFullshow->lower();

			setAllButtonsEnabled(this, false);
			ui->tableViewPhotoList->setEnabled(false);
			MyMessageBox messBox(this);
			CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
			okButton->setFocus(Qt::TabFocusReason);
			messBox.setText(tr("图像大小或长宽乘积大于1024x1024, \n打开失败"));
			messBox.exec();
			setAllButtonsEnabled(this, true);
			ui->tableViewPhotoList->setEnabled(true);

			((Application *)qApp)->setLCDAndTimerOn();

			return;
		}
	}
	else if ( g_pScaleSrc->get_screen_version() == 2 || g_pScaleSrc->get_screen_version() == 3 )
	{
		if (((pixmap.width() > 1440) && (pixmap.height() > 1440))
			|| (pixmap.width() * pixmap.height() > 1440 * 1440))
		{
			m_pFrameFullshow->hide();
			m_pFrameFullshow->lower();

			setAllButtonsEnabled(this, false);
			ui->tableViewPhotoList->setEnabled(false);
			MyMessageBox messBox(this);
			CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
			okButton->setFocus(Qt::TabFocusReason);
			messBox.setText(tr("图像大小或长宽乘积大于1440x1440, \n打开失败"));
			messBox.exec();
			setAllButtonsEnabled(this, true);
			ui->tableViewPhotoList->setEnabled(true);

			((Application *)qApp)->setLCDAndTimerOn();

			return;
		}
	}

	if ((pixmap.width() > qApp->desktop()->size().width()) || (pixmap.height() > qApp->desktop()->size().height()))
	{
		if (pixmap.width() * qApp->desktop()->size().height() >= pixmap.height() * qApp->desktop()->size().width())
		{
			pixmap = pixmap.scaledToWidth(qApp->desktop()->size().width());
		}
		else
		{
			pixmap = pixmap.scaledToHeight(qApp->desktop()->size().height());
		}
	}

	m_pPicLabel->setFixedSize(pixmap.width(), pixmap.height());
	m_pPicLabel->move((qApp->desktop()->size().width() - pixmap.width() + 1) / 2, (qApp->desktop()->size().height() - pixmap.height() + 1) / 2);

	m_pPicLabel->setPixmap(pixmap);

	if (m_nextPlayRow < model->rowCount() - 1)
	{
		m_nextPlayRow++;
	}
	else
	{
		m_nextPlayRow = 0;
		while (model->item(m_nextPlayRow, 3)->text() != "2")
		{
			m_nextPlayRow++;
		}
	}

	m_pTimer->start(PHOTO_PLAY_INTERVAL);
}
