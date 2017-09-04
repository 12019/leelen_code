#include "infomanviewattachmentdialog.h"
#include "ui_infomanviewattachmentdialog.h"
#include "infomandetaildialog.h"
#include <qconfig.h>
//#include "callcenterdialog.h"
//#include "terminalhelpmaindialog.h"
//#include "helpdialog.h"
#include "ui_infomandetaildialog.h"
#include "maindialog.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
/************************************************************
  该文件为查看信息附件图片对话框主程序
************************************************************/

/************************************************************
描述：查看信息附件图片对话框构造函数
参数：parent - 该对话框的父窗口指针
返回：无
************************************************************/
InfoManViewAttachmentDialog::InfoManViewAttachmentDialog(QWidget *parent) :
        QDialog(parent),
		ui(new Ui::InfoManViewAttachmentDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

    if ( g_pScaleSrc->get_screen_version() == 3 ) // 1280x800
    {
        ui->label_info_title->setGeometry(QRect(140, 190, 711, 61));
        ui->pushButtonInfoManViewZoomIn->setGeometry(QRect(959, 245, 279, 99));
        ui->pushButtonInfoManViewZoomOut->setGeometry(QRect(959, 353, 279, 99));
        ui->pushButtonInfoManViewPrev->setGeometry(QRect(959, 461, 279, 99));
        ui->pushButtonInfoManViewNext->setGeometry(QRect(959, 569, 279, 99));
        ui->pushButtonInfoManViewReturn->setGeometry(QRect(959, 677, 279, 99));
        ui->graphicsViewAttachment->setGeometry(QRect(130, 260, 731, 471));
        ui->labelAttachmentFileName->setGeometry(QRect(100, 140, 381, 61));
        ui->labelAttachmentIndex->setGeometry(QRect(480, 140, 421, 61));
        ui->framePanel->setGeometry(QRect(65, 30, 858, 745));
        ui->label_3->setGeometry(QRect(230, 40, 511, 61));
        ui->frameBox->setGeometry(QRect(90, 139, 811, 611));
    }
    else if ( g_pScaleSrc->get_screen_version() == 2 )  // 1024x600
    {
        ui->label_info_title->setGeometry(QRect(100, 150, 561, 35));
        ui->pushButtonInfoManViewZoomIn->setGeometry(QRect(764, 180, 230, 80));
        ui->pushButtonInfoManViewZoomOut->setGeometry(QRect(764, 260, 230, 80));
        ui->pushButtonInfoManViewPrev->setGeometry(QRect(764, 340, 230, 80));
        ui->pushButtonInfoManViewNext->setGeometry(QRect(764, 420, 230, 80));
        ui->pushButtonInfoManViewReturn->setGeometry(QRect(764, 500, 230, 80));
        ui->graphicsViewAttachment->setGeometry(QRect(80, 190, 611, 351));
        ui->labelAttachmentFileName->setGeometry(QRect(57, 97, 331, 51));
        ui->labelAttachmentIndex->setGeometry(QRect(388, 97, 331, 51));
        ui->framePanel->setGeometry(QRect(38, 17, 700, 570));
        ui->label_3->setGeometry(QRect(140, 25, 511, 51));
        ui->frameBox->setGeometry(QRect(57, 97, 662, 471));
    }
	/*InfoManDetailDialog *pInfoManDetailDlg = (InfoManDetailDialog *)parent;
	  QListView *pListViewInfoAttchment = pInfoManDetailDlg->GetListViewInfoAttachment();
	  int count;
	  int i;

	  count = pListViewInfoAttchment->model()->rowCount();
	  m_CurScaleArray = new float[count];
	  for (i = 0; i < count; i++)
	  {
	  m_CurScaleArray[i] = -1.0;
	  }
	  */

	m_CurScaleArray = NULL;

	m_MaxScale = 1.0;
	m_MinScale = 1.0;

	m_Movie = NULL;

	ui->graphicsViewAttachment->setDragMode(QGraphicsView::ScrollHandDrag);
}

/************************************************************
描述：查看信息附件图片对话框析构函数
参数：无
返回：无
************************************************************/
InfoManViewAttachmentDialog::~InfoManViewAttachmentDialog()
{
	if (m_CurScaleArray) delete [] m_CurScaleArray;
	if (m_Movie) delete m_Movie;

	delete ui;
}

/************************************************************
描述：显示信息附件当前图片，图片的缩放因子存于m_CurScaleArray数组中
参数：无
返回：0 - 成功
	 -1 - 失败
************************************************************/
int InfoManViewAttachmentDialog::ShowCurrentAttachment()
{
	InfoManDetailDialog *pInfoManDetailDlg = (InfoManDetailDialog *)parent();
	QListView *pListViewInfoAttchment = pInfoManDetailDlg->GetListViewInfoAttachment();
	int rowSel;
	QStringListModel *model = (QStringListModel *)pListViewInfoAttchment->model();
	char attachFilePath[PATH_MAX];
	QString attachFileName;
	QString strTemp;
	////char str[30], str2[30];
	////FILE *fp;
	////int size;
	QSize image_sz, viewport_sz;
	QString attachFile;

	rowSel = pListViewInfoAttchment->currentIndex().row();
	if (rowSel == -1) return -1;

	strTemp = tr("第") + QString::number(rowSel + 1) + tr("张  共") + QString::number(model->rowCount()) + tr("张");
	//sprintf(str, "第%2d张  共%2d张", rowSel + 1, model->rowCount());
	ui->labelAttachmentIndex->setText(strTemp);

	attachFileName = model->stringList().value(rowSel);
	ui->labelAttachmentFileName->setText(tr("文件名:") + " " + attachFileName);
	sprintf(attachFilePath, "/mnt/disk/info/%s-%s", pInfoManDetailDlg->m_InfoTime, attachFileName.toUtf8().data());

	/*fp = fopen(attachFilePath, "rb");
	  if (fp)
	  {
	  fseek(fp, 0, SEEK_END);
	  size = ftell(fp);
	  fclose(fp);
	  }
	  else
	  {
	  size = 0;
	  }

	  if (size > 1024 * 1024)
	  {
	  sprintf(str2, "%6.2fMB", size/(1024.0 * 1024.0));
	  }
	  else if (size > 1024)
	  {
	  sprintf(str2, "%6.2fKB", size/1024.0);
	  }
	  else if (size > 0)
	  {
	  sprintf(str2, "%dB", size);
	  }
	  else
	  {
	  sprintf(str2, "0KB");
	  }*/

	QGraphicsScene *scene = ui->graphicsViewAttachment->scene();
	if (scene)	delete scene;
	if (m_Movie) delete m_Movie;
	m_Movie = NULL;

	scene = new QGraphicsScene(ui->graphicsViewAttachment);
	ui->graphicsViewAttachment->setScene(scene);

	if (strstr(QT_BUILD_KEY, "arm") == 0) //not arm-linux-gcc
	{
		attachFile = QString::fromUtf8(attachFilePath);
	}
	else
	{
		attachFile = QString(attachFilePath);
	}

	if (attachFileName.right(4) == ".gif")
	{
		m_Movie = new QMovie(attachFile);
		m_Movie->jumpToFrame(0);
		image_sz = m_Movie->currentPixmap().size();

		if ((image_sz.width() != 0) && (image_sz.height() != 0))
		{
			QLabel *label = new QLabel();

			//QPalette palette;
			//palette.setColor(QPalette::Active, QPalette::Background, QColor(255,255,255));
			//label->setPalette(palette);

			label->setMovie(m_Movie);
			m_Movie->start();
			scene->addWidget(label);
		}
		else
		{
			delete m_Movie;
			m_Movie = NULL;
		}
	}
	else
	{
		QPixmap pixmap(attachFile);
		scene->addPixmap(pixmap);
		image_sz = QSize(pixmap.width(), pixmap.height());
	}

	//sprintf(str, "%s %dx%d", str2, image_sz.width(), image_sz.height());
	//ui->labelAttachmentFileSize->setText(str);

	viewport_sz = ui->graphicsViewAttachment->maximumViewportSize();

	if ((image_sz.width() == 0) || (image_sz.height() == 0))
	{
		m_MinScale = 1.0;
		m_MaxScale = 1.0;
	}
	else if ((image_sz.width() <= viewport_sz.width()) && (image_sz.height() <= viewport_sz.height()))
	{
		m_MinScale = 1.0;

		if (((float)image_sz.width())/viewport_sz.width() <= ((float)image_sz.height())/viewport_sz.height())
		{
			m_MaxScale = ((float)viewport_sz.height())/image_sz.height();
		}
		else
		{
			m_MaxScale = ((float)viewport_sz.width())/image_sz.width();
		}
	}
	else
	{
		m_MaxScale = 1.0;

		if (((float)image_sz.width())/viewport_sz.width() <= ((float)image_sz.height())/viewport_sz.height())
		{
			m_MinScale = ((float)viewport_sz.height())/image_sz.height();
		}
		else
		{
			m_MinScale = ((float)viewport_sz.width())/image_sz.width();
		}
	}

	if (m_CurScaleArray[rowSel] < 0.0)
	{
		m_CurScaleArray[rowSel] = m_MinScale;
	}

	////sprintf(str, "%d%%", (int)(m_CurScaleArray[rowSel] * 100 + 0.5));
	////ui->lineEditZoomScale->setText(str);

	ui->graphicsViewAttachment->resetTransform();
	ui->graphicsViewAttachment->scale(m_CurScaleArray[rowSel], m_CurScaleArray[rowSel]);

	return 0;
}

/************************************************************
描述：关闭信息附件图片显示对话框
参数：无
返回：无
************************************************************/
void InfoManViewAttachmentDialog::on_pushButtonInfoManViewReturn_pressed()
{
    	done(0);
}

/************************************************************
描述：显示当前信息上一个附件图片
参数：无
返回：无
************************************************************/
void InfoManViewAttachmentDialog::on_pushButtonInfoManViewPrev_pressed()
{
	InfoManDetailDialog *pInfoManDetailDlg = (InfoManDetailDialog *)parent();
	QListView *pListViewInfoAttchment = pInfoManDetailDlg->GetListViewInfoAttachment();
	QStringListModel *model = (QStringListModel *)pListViewInfoAttchment->model();
	int row;
	QModelIndex index;

	row = pListViewInfoAttchment->currentIndex().row();
	if (row >= 1)
	{
		index = model->index(row - 1, 0);
	}
	else
	{
		index = model->index(model->rowCount() - 1, 0);
	}

	pListViewInfoAttchment->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
	ShowCurrentAttachment();
}

/************************************************************
描述：显示当前信息下一个附件图片
参数：无
返回：无
************************************************************/
void InfoManViewAttachmentDialog::on_pushButtonInfoManViewNext_pressed()
{
	InfoManDetailDialog *pInfoManDetailDlg = (InfoManDetailDialog *)parent();
	QListView *pListViewInfoAttchment = pInfoManDetailDlg->GetListViewInfoAttachment();
	QStringListModel *model = (QStringListModel *)pListViewInfoAttchment->model();
	int row;
	QModelIndex index;

	row = pListViewInfoAttchment->currentIndex().row();
	if (row < model->rowCount() - 1)
	{
		index = model->index(row + 1, 0);
	}
	else
	{
		index = model->index(0, 0);
	}

	pListViewInfoAttchment->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
	ShowCurrentAttachment();
}

/************************************************************
描述：显示的图片放大1.2倍
参数：无
返回：无
************************************************************/
void InfoManViewAttachmentDialog::on_pushButtonInfoManViewZoomIn_pressed()
{
	float scale, cur_scale;
	InfoManDetailDialog *pInfoManDetailDlg = (InfoManDetailDialog *)parent();
	QListView *pListViewInfoAttchment = pInfoManDetailDlg->GetListViewInfoAttachment();
	int row;
	////char str[20];

	row = pListViewInfoAttchment->currentIndex().row();

	cur_scale = m_CurScaleArray[row];
	if (cur_scale >= m_MaxScale) return;

	scale = m_MaxScale/cur_scale;

	if (scale > 1.2)
	{
		scale = 1.2;
		cur_scale *= 1.2;
	}
	else
	{
		cur_scale = m_MaxScale;
	}

	////sprintf(str, "%d%%", (int)(cur_scale * 100 + 0.5));
	////ui->lineEditZoomScale->setText(str);

	m_CurScaleArray[row] = cur_scale;
	ui->graphicsViewAttachment->scale(scale, scale);
}

/************************************************************
描述：显示的图片缩小1.2倍
参数：无
返回：无
************************************************************/
void InfoManViewAttachmentDialog::on_pushButtonInfoManViewZoomOut_pressed()
{
    float scale, cur_scale;
	InfoManDetailDialog *pInfoManDetailDlg = (InfoManDetailDialog *)parent();
	QListView *pListViewInfoAttchment = pInfoManDetailDlg->GetListViewInfoAttachment();
    int row;
	////char str[20];

    row = pListViewInfoAttchment->currentIndex().row();
    cur_scale = m_CurScaleArray[row];

    if (m_CurScaleArray[row] <= m_MinScale) return;

    scale = m_MinScale/cur_scale;

    if (scale < 1/1.2)
    {
        scale = 1/1.2;
        cur_scale *= 1/1.2;
    }
	else
    {
        cur_scale = m_MinScale;
    }

	////sprintf(str, "%d%%", (int)(cur_scale * 100 + 0.5));
	////ui->lineEditZoomScale->setText(str);

    m_CurScaleArray[row] = cur_scale;
    ui->graphicsViewAttachment->scale(scale, scale);
}

/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void InfoManViewAttachmentDialog::showEvent(QShowEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)(parent()->parent()->parent());

	pMainDlg->showRefreshTabFocus(this);

    ui->pushButtonInfoManViewZoomIn->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void InfoManViewAttachmentDialog::hideEvent(QHideEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)(parent()->parent()->parent());

	pMainDlg->hideRefreshTabFocus(this);

    ((InfoManDetailDialog *)parent())->ui->pushButtonInfoManDetailAttach->setFocus(Qt::TabFocusReason);
}

