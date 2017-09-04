#include "photoudiskdialog.h"
#include "ui_photoudiskdialog.h"
#include "maindialog.h"
#include <QStandardItemModel>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/mount.h>
#include "ui_photomaindialog.h"
#include <linux/watchdog.h>

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

#define PHOTO_SIZE_LIMIT_MB		18

extern MainDialog *g_pMainDlg;

PhotoUDiskDialog::PhotoUDiskDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PhotoUDiskDialog)
{
	ui->setupUi(this);
	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	m_pFrameFullshow = new QFrame(this);
	//m_pFrameFullshow->setObjectName("frameFullshow");
	m_pFrameFullshow->setGeometry(QRect(0, 0, qApp->desktop()->size().width(),qApp->desktop()->size().height()));
	m_pFrameFullshow->setFrameShadow(QFrame::Plain);
	m_pFrameFullshow->setFrameShape(QFrame::Panel);

	m_pFrameFullshow->setStyleSheet("background-color: #000000;");

	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	m_pFrameFullshow->installEventFilter(this);
	m_pPicLabel = new QLabel(m_pFrameFullshow);

	int x,y,w,h;
	g_pScaleSrc->get_info_framepanel_geometry(x,y,w,h);
	ui->framePanel->setGeometry(x,y,w,h);

	QStandardItemModel *model = new QStandardItemModel(0, 4);

	g_pScaleSrc->get_tabviewinfolist_geometry(x,y,w,h);
	ui->tableViewPhotoList->setGeometry(x,y,w,h);

	ui->tableViewPhotoList->setModel(model);

	ui->tableViewPhotoList->verticalHeader()->hide();
	ui->tableViewPhotoList->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableViewPhotoList->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableViewPhotoList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableViewPhotoList->setSortingEnabled(false);
	ui->tableViewPhotoList->setShowGrid(false);
	ui->tableViewPhotoList->setFocusPolicy(Qt::NoFocus);

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
        ui->pushButtonPhotoUDiskPrev->setGeometry(QRect(959, 245, 279, 99));
        ui->pushButtonPhotoUDiskNext->setGeometry(QRect(959, 353, 279, 99));
        ui->pushButtonPhotoUDiskCheck->setGeometry(QRect(959, 461, 279, 99));
        ui->pushButtonPhotoUDiskImport->setGeometry(QRect(959, 569, 279, 99));
        ui->pushButtonPhotoUDiskReturn->setGeometry(QRect(959, 677, 279, 99));
    }
    else if (res == 2) // 1024x600
    {
        ui->pushButtonPhotoUDiskPrev->setGeometry(QRect(764, 180, 230, 80));
        ui->pushButtonPhotoUDiskNext->setGeometry(QRect(764, 260, 230, 80));
        ui->pushButtonPhotoUDiskCheck->setGeometry(QRect(764, 340, 230, 80));
        ui->pushButtonPhotoUDiskImport->setGeometry(QRect(764, 420, 230, 80));
        ui->pushButtonPhotoUDiskReturn->setGeometry(QRect(764, 500, 230, 80));
    }
}

PhotoUDiskDialog::~PhotoUDiskDialog()
{
	delete m_pPicLabel;
	delete m_pFrameFullshow;

	delete ui;
}

void PhotoUDiskDialog::translateTableViewPhotoList()
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewPhotoList->model();

	model->setHeaderData(0, Qt::Horizontal, tr("名称"));
	model->setHeaderData(1, Qt::Horizontal, tr("大小"));
	model->setHeaderData(2, Qt::Horizontal, tr("修改时间"));
	model->setHeaderData(3, Qt::Horizontal, tr("目录/文件"));
}

bool PhotoUDiskDialog::load(QString strDir)
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewPhotoList->model();
	model->removeRows(0, model->rowCount());

	DIR *dp;
	struct dirent *dirp;
	char path[512];
	char pathPrefix[512];

	strcpy(pathPrefix, strDir.toLocal8Bit().data());

#ifdef ARM11
	umount("/udisk");
	if (access("/udisk", F_OK) != 0)
	{
		mkdir("/udisk", 0777);
	}

	if (((mount("/dev/udisk", "/udisk", "vfat", 0, 0) < 0)
		&& (mount("/dev/sdb1", "/udisk", "vfat", 0, 0) < 0))
		|| ((dp = opendir(pathPrefix)) == NULL))
#endif

#ifdef CORTEXA7
	if (((dp = opendir(pathPrefix)) == NULL))
#endif
	{
		setAllButtonsEnabled(this, false);
		ui->tableViewPhotoList->setEnabled(false);
		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		okButton->setFocus(Qt::TabFocusReason);
		messBox.setText(tr("打开U盘失败"));
		messBox.exec();
		setAllButtonsEnabled(this, true);
		ui->tableViewPhotoList->setEnabled(true);

		hide();

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

			if ((strDir == "/udisk") && (strcmp(dirp->d_name, "..") == 0))
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

void PhotoUDiskDialog::showEvent(QShowEvent *)
{
	//load("/udisk/photo");

	g_pMainDlg->showRefreshTabFocus(this);

	ui->pushButtonPhotoUDiskCheck->setFocus(Qt::TabFocusReason);
}

void PhotoUDiskDialog::hideEvent(QHideEvent *)
{
	m_pFrameFullshow->hide();
	m_pFrameFullshow->lower();

	g_pMainDlg->hideRefreshTabFocus(this);

	PhotoMainDialog *pPhotoMainDlg = (PhotoMainDialog *)parent();
	if (g_pMainDlg->m_pCurActiveDlg == pPhotoMainDlg)
	{
        	pPhotoMainDlg->ui->pushButtonPhotoMainImport->setFocus(Qt::TabFocusReason);
	}
}

void PhotoUDiskDialog::on_pushButtonPhotoUDiskReturn_pressed()
{
	hide();
}

void PhotoUDiskDialog::on_pushButtonPhotoUDiskCheck_pressed()
{
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
		int timeout = 30;
		if (g_pMainDlg->m_watchdog_fd >= 0)
		{
			ioctl(g_pMainDlg->m_watchdog_fd, WDIOC_SETTIMEOUT, &timeout);
		}

		QString strPicPath = m_curDir + "/" + model->item(row, 0)->text();
		QPixmap pixmap(strPicPath);

		timeout = 8;
		if (g_pMainDlg->m_watchdog_fd >= 0)
		{
			ioctl(g_pMainDlg->m_watchdog_fd, WDIOC_SETTIMEOUT, &timeout);
		}

		if ((pixmap.width() == 0) || (pixmap.height() == 0))
		{
			setAllButtonsEnabled(this, false);
			ui->tableViewPhotoList->setEnabled(false);
			MyMessageBox messBox(this);
			CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
			okButton->setFocus(Qt::TabFocusReason);
			messBox.setText(PhotoMainDialog::tr("图像打开失败"));
			messBox.exec();
			setAllButtonsEnabled(this, true);
			ui->tableViewPhotoList->setEnabled(true);

			return;
		}

		if (((pixmap.width() > 1024) && (pixmap.height() > 1024))
			|| (pixmap.width() * pixmap.height() > 1024 * 1024))
		{
			setAllButtonsEnabled(this, false);
			ui->tableViewPhotoList->setEnabled(false);
			MyMessageBox messBox(this);
			CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
			okButton->setFocus(Qt::TabFocusReason);
			messBox.setText(PhotoMainDialog::tr("图像大小或长宽乘积大于1024x1024, \n打开失败"));
			messBox.exec();
			setAllButtonsEnabled(this, true);
			ui->tableViewPhotoList->setEnabled(true);

			return;
		}

		if ((pixmap.width() > 800) || (pixmap.height() > 480))
		{
			if (pixmap.width() * 480 >= pixmap.height() * 800)
			{
				pixmap = pixmap.scaledToWidth(800);
			}
			else
			{
				pixmap = pixmap.scaledToHeight(480);
			}
		}

		m_pPicLabel->setFixedSize(pixmap.width(), pixmap.height());
		m_pPicLabel->move((800 - pixmap.width() + 1) / 2, (480 - pixmap.height() + 1) / 2);

		m_pPicLabel->setPixmap(pixmap);

		m_pFrameFullshow->show();
		m_pFrameFullshow->raise();
	}
}

bool PhotoUDiskDialog::eventFilter(QObject *o, QEvent *e)
{
	if (e->type() == QEvent::MouseButtonPress)
	{
		if (o == m_pFrameFullshow)
		{
			m_pFrameFullshow->hide();
			m_pFrameFullshow->lower();

			//repaint();
		}
	}

	return false;
}

void PhotoUDiskDialog::on_pushButtonPhotoUDiskPrev_pressed()
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

void PhotoUDiskDialog::on_pushButtonPhotoUDiskNext_pressed()
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

int copy_overwrite = OVERWRITE_NORMAL;

bool copyDir(QDialog *pDlg, char *source, char *destination, char *dest_base)
{
	DIR *dp;
	struct dirent *dirp;
	char path[512];
	char path2[512];

	if ((dp = opendir(source)) == NULL)
	{
		return false;
	}

	while ((dirp = readdir(dp)) != NULL)
	{
		if (g_pMainDlg->m_watchdog_fd >= 0)
		{
			ioctl(g_pMainDlg->m_watchdog_fd, WDIOC_KEEPALIVE, 0);
		}

		if ((strcmp(dirp->d_name, ".") == 0) || (strcmp(dirp->d_name, "..") == 0))
		{
			continue;
		}

		sprintf(path, "%s/%s", source, dirp->d_name);

		struct stat buf;
		if (stat(path, &buf) >= 0)
		{
			sprintf(path2, "%s/%s", destination, dirp->d_name);

			if (!S_ISDIR(buf.st_mode)) //子目录项是图片文件
			{
				int len = strlen(dirp->d_name);

				if ((strcasecmp(&dirp->d_name[len - 4], ".bmp") != 0)
					&& (strcasecmp(&dirp->d_name[len - 4], ".jpg") != 0)
					&& (strcasecmp(&dirp->d_name[len - 4], ".gif") != 0)
					&& (strcasecmp(&dirp->d_name[len - 4], ".png") != 0)
					&& (strcasecmp(&dirp->d_name[len - 4], ".tif") != 0)
					&& (strcasecmp(&dirp->d_name[len - 5], ".tiff") != 0))
				{
					continue;
				}

				struct stat buf2;
				if (stat(path2, &buf2) >= 0)
				{
					if (!S_ISDIR(buf2.st_mode)) //目标为文件已存在
					{
						if (copy_overwrite == OVERWRITE_NORMAL)
						{
							setAllButtonsEnabled(pDlg, false);
							//ui->tableViewPhotoList->setEnabled(false);
							MyMessageBox messBox(pDlg);
							CommonPushButton *okButton = messBox.addButton(PhotoUDiskDialog::tr("确定"));
							CommonPushButton *cancelButton = messBox.addButton(PhotoUDiskDialog::tr("取消"), "pushButtonMyMessageBoxCancel");
							CommonPushButton *okAllButton = messBox.addButton(PhotoUDiskDialog::tr("全是"));
							CommonPushButton *cancelAllButton = messBox.addButton(PhotoUDiskDialog::tr("全否"), "pushButtonMyMessageBoxCancel");
							Q_UNUSED(cancelButton);
							Q_UNUSED(okAllButton);
							Q_UNUSED(cancelAllButton);
							okButton->setFocus(Qt::TabFocusReason);
							char *ptr = path2 + strlen(dest_base);//"/mnt/disk/photo/");
							QString strInfo;
							strInfo = PhotoUDiskDialog::tr("文件") + ptr + PhotoUDiskDialog::tr("已存在, 覆盖该文件?");
							messBox.setText(strInfo);
							messBox.exec();
							setAllButtonsEnabled(pDlg, true);
							//ui->tableViewPhotoList->setEnabled(true);
							//ui->pushButtonPhotoUDiskImport->setFocus(Qt::TabFocusReason);

							if (messBox.clickedButton() == cancelButton)
							{
								continue;
							}
							else if (messBox.clickedButton() == cancelAllButton)
							{
								copy_overwrite = OVERWRITE_NO;
								continue;
							}
							else if (messBox.clickedButton() == okAllButton)
							{
								copy_overwrite = OVERWRITE_YES;
							}
						}
						else if (copy_overwrite == OVERWRITE_NO)
						{
							continue;
						}

						QFile::remove(path2);
					}
					else //目标为目录已存在
					{
						setAllButtonsEnabled(pDlg, false);
						//ui->tableViewPhotoList->setEnabled(false);
						MyMessageBox messBox(pDlg);
						CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
						okButton->setFocus(Qt::TabFocusReason);
						messBox.setText(PhotoUDiskDialog::tr("本地系统中有目录和待导入\n的文件名相同，导入失败"));
						messBox.exec();
						setAllButtonsEnabled(pDlg, true);
						//ui->tableViewPhotoList->setEnabled(true);
						//ui->pushButtonPhotoUDiskImport->setFocus(Qt::TabFocusReason);

						return false;
					}
				}

				//printf("lg %s\n", path);

				QFile::copy(path, path2);
			}
			else //子目录项是目录
			{
				struct stat buf2;
				if (stat(path2, &buf2) >= 0)
				{
					if (!S_ISDIR(buf2.st_mode)) //目标为文件已存在
					{
						setAllButtonsEnabled(pDlg, false);
						//ui->tableViewPhotoList->setEnabled(false);
						MyMessageBox messBox(pDlg);
						CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
						okButton->setFocus(Qt::TabFocusReason);
						messBox.setText(PhotoUDiskDialog::tr("本地系统中有文件和待导入\n的目录名相同，导入失败"));
						messBox.exec();
						setAllButtonsEnabled(pDlg, true);
						//ui->tableViewPhotoList->setEnabled(true);
						//ui->pushButtonPhotoUDiskImport->setFocus(Qt::TabFocusReason);

						return false;
					}
				}
				else
				{
					mkdir(path2, 0777);
				}

				if (!copyDir(pDlg, path, path2, dest_base))
				{
					return false;
				}
			}
		}
	}

	closedir(dp);

	return true;
}

qint64 calcSpace(const QString &path)
{
	QDir dir(path);
	qint64 size = 0;

	foreach (QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
	{
		size += fileInfo.size();
	}

	foreach (QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
	{
		size += calcSpace(path + QDir::separator() + subDir);
	}

	return size;
}

void PhotoUDiskDialog::on_pushButtonPhotoUDiskImport_pressed()
{
	QStandardItemModel *model = (QStandardItemModel *)ui->tableViewPhotoList->model();
	int row = ui->tableViewPhotoList->currentIndex().row();
	if (row < 0) return;

	QString strMode = model->item(row, 3)->text();
    if (strMode == "0")
    {
        return;
    }
    else if(strMode == "1")
    {
        setAllButtonsEnabled(this, false);
        ui->tableViewPhotoList->setEnabled(false);
        MyMessageBox messBox(this);
        CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
        okButton->setFocus(Qt::TabFocusReason);
        messBox.setText(tr("您选择的是一个目录\n请选择一张图片进行导入。"));
        messBox.exec();
        setAllButtonsEnabled(this, true);
        ui->tableViewPhotoList->setEnabled(true);
        ui->pushButtonPhotoUDiskImport->setFocus(Qt::TabFocusReason);
        return ;
    }
    else if(strMode == "2")
    {
        QString strFileName = model->item(row, 0)->text();
        QString strDir = "/mnt/disk/photo/" + strFileName;
        if(access(strDir.toLocal8Bit().data(),F_OK) == 0)
        {
            setAllButtonsEnabled(this, false);
            ui->tableViewPhotoList->setEnabled(false);
            MyMessageBox messBox(this);
            CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
            CommonPushButton *cancelButton = messBox.addButton(QObject::tr("取消"), "pushButtonMyMessageBoxCancel");
            Q_UNUSED(okButton);
            cancelButton->setFocus(Qt::TabFocusReason);
            messBox.setText(tr("本地系统中有图片和待导入\n的图片名相同，覆盖该文件?"));
            messBox.exec();
            setAllButtonsEnabled(this, true);
            ui->tableViewPhotoList->setEnabled(true);
            ui->pushButtonPhotoUDiskImport->setFocus(Qt::TabFocusReason);
            if(messBox.clickedButton() == cancelButton)
            {
                return;
            }

            QFile::remove(strDir);
        }

        setAllButtonsEnabled(this, false);
        ui->tableViewPhotoList->setEnabled(false);
        bool bRet = QFile::copy(m_curDir + "/" + strFileName, strDir);
        MyMessageBox messBox(this);
        CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
        okButton->setFocus(Qt::TabFocusReason);
        if (bRet)
        {
            messBox.setText(tr("图片导入完毕"));
        }
        else
        {
            messBox.setText(tr("图片导入失败"));
        }
        messBox.exec();
        setAllButtonsEnabled(this, true);
        ui->tableViewPhotoList->setEnabled(true);
        ui->pushButtonPhotoUDiskImport->setFocus(Qt::TabFocusReason);
    }
//	{
//		QString strFileName = model->item(row, 0)->text();
//		QString strDir = "/mnt/disk/photo";
//		QStringList strDirs = m_curDir.mid(1).split("/");
//        printf("m_curDir.......%s..................\n",m_curDir.toLocal8Bit().data());
//		if (strMode == "1")
//		{
//			strDirs.append(strFileName);
//		}

//		int i;
//		for (i = 1; i < strDirs.count(); i++)
//		{
//			strDir += "/" + strDirs.at(i);
//            printf("strDirs.at(i)............%s....\n",strDirs.at(i).toLocal8Bit().data());
//			struct stat st;
//			if (stat(strDir.toLocal8Bit().data(), &st) >= 0)
//			{
//				if (!S_ISDIR(st.st_mode))
//				{
//					break;
//				}
//			}
//			else
//			{
//				mkdir(strDir.toLocal8Bit().data(), 0777);
//			}
//		}

//		if (i < strDirs.count())
//		{
//			setAllButtonsEnabled(this, false);
//			ui->tableViewPhotoList->setEnabled(false);
//			MyMessageBox messBox(this);
//			CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
//			okButton->setFocus(Qt::TabFocusReason);
//			messBox.setText(tr("本地系统中有文件和待导入\n的目录名相同，导入失败"));
//			messBox.exec();
//			setAllButtonsEnabled(this, true);
//			ui->tableViewPhotoList->setEnabled(true);
//			ui->pushButtonPhotoUDiskImport->setFocus(Qt::TabFocusReason);

//			return;
//		}

//		if (strMode == "1")
//		{
//			setAllButtonsEnabled(this, false);
//			ui->tableViewPhotoList->setEnabled(false);

//			copy_overwrite = OVERWRITE_NORMAL;
//			bool bRet = copyDir(this, (m_curDir + "/" + strFileName).toLocal8Bit().data(), strDir.toLocal8Bit().data(), (char *)"/mnt/disk/photo");

//			if (bRet)
//			{
//				MyMessageBox messBox(this);
//				CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
//				okButton->setFocus(Qt::TabFocusReason);
//				messBox.setText(tr("文件夹导入完毕"));
//				messBox.exec();
//			}

//			setAllButtonsEnabled(this, true);
//			ui->tableViewPhotoList->setEnabled(true);
//			ui->pushButtonPhotoUDiskImport->setFocus(Qt::TabFocusReason);
//		}
//		else
//		{
//			strDir += "/" + strFileName;

//			struct stat st;
//			if (stat(strDir.toLocal8Bit().data(), &st) >= 0)
//			{
//				if (S_ISDIR(st.st_mode))
//				{
//					setAllButtonsEnabled(this, false);
//					ui->tableViewPhotoList->setEnabled(false);
//					MyMessageBox messBox(this);
//					CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
//					okButton->setFocus(Qt::TabFocusReason);
//					messBox.setText(tr("本地系统中有目录和待导入\n的文件名相同，导入失败"));
//					messBox.exec();
//					setAllButtonsEnabled(this, true);
//					ui->tableViewPhotoList->setEnabled(true);
//					ui->pushButtonPhotoUDiskImport->setFocus(Qt::TabFocusReason);
//					return;
//				}
//				else
//				{
//					setAllButtonsEnabled(this, false);
//					ui->tableViewPhotoList->setEnabled(false);
//					MyMessageBox messBox(this);
//					CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
//					CommonPushButton *cancelButton = messBox.addButton(QObject::tr("取消"), "pushButtonMyMessageBoxCancel");
//					Q_UNUSED(okButton);
//					cancelButton->setFocus(Qt::TabFocusReason);
//					messBox.setText(tr("本地系统中有文件和待导入\n的文件名相同，覆盖该文件?"));
//					messBox.exec();
//					setAllButtonsEnabled(this, true);
//					ui->tableViewPhotoList->setEnabled(true);
//					ui->pushButtonPhotoUDiskImport->setFocus(Qt::TabFocusReason);
//					if(messBox.clickedButton() == cancelButton)
//					{
//						return;
//					}

//					QFile::remove(strDir);
//				}
//			}

//			setAllButtonsEnabled(this, false);
//			ui->tableViewPhotoList->setEnabled(false);
//			bool bRet = QFile::copy(m_curDir + "/" + strFileName, strDir);
//			MyMessageBox messBox(this);
//			CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
//			okButton->setFocus(Qt::TabFocusReason);
//			if (bRet)
//			{
//				messBox.setText(tr("文件导入完毕"));
//			}
//			else
//			{
//				messBox.setText(tr("文件导入失败"));
//			}
//			messBox.exec();
//			setAllButtonsEnabled(this, true);
//			ui->tableViewPhotoList->setEnabled(true);
//			ui->pushButtonPhotoUDiskImport->setFocus(Qt::TabFocusReason);
//		}
//	}

	PhotoMainDialog *pPhotoMainDlg = (PhotoMainDialog *)parent();
	pPhotoMainDlg->load(pPhotoMainDlg->m_curDir);

	int size = calcSpace("/mnt/disk/photo");
	if (size >= PHOTO_SIZE_LIMIT_MB * 1024 * 1024)
	{
		setAllButtonsEnabled(this, false);
		ui->tableViewPhotoList->setEnabled(false);
		MyMessageBox messBox(this);
		CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
		okButton->setFocus(Qt::TabFocusReason);
		QString strInfo;
		strInfo = tr("系统中的图片文件总大小为");
		strInfo += QString::number(size / (1024.0 * 1024.0), 'f', 2);
		strInfo += "MB\n" + tr("超过了最大允许的") + QString::number(PHOTO_SIZE_LIMIT_MB) + "MB\n";
		strInfo += tr("请即时清理");
		messBox.setText(strInfo);
		messBox.exec();
		setAllButtonsEnabled(this, true);
		ui->tableViewPhotoList->setEnabled(true);
		ui->pushButtonPhotoUDiskImport->setFocus(Qt::TabFocusReason);
	}
}
