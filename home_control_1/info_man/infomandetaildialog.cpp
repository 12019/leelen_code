#include "infomandetaildialog.h"
#include "ui_infomandetaildialog.h"
#include "infomanmaindialog.h"
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <qconfig.h>
//#include "callcenterdialog.h"
//#include "terminalhelpmaindialog.h"
//#include "helpdialog.h"
#include "ui_infomanmaindialog.h"
#include "maindialog.h"
//#include "ui_infowritedialog.h"

/************************************************************
  该文件为接收到的信息内容查看对话框主程序
************************************************************/
extern char g_strNativeName[50];
extern MainDialog *g_pMainDlg;

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
/************************************************************
描述：信息详细内容显示对话框构造函数
参数：parent - 该对话框的父窗口指针
返回：无
************************************************************/
InfoManDetailDialog::InfoManDetailDialog(QWidget *parent) :
    QDialog(parent),
	ui(new Ui::InfoManDetailDialog)
{
	ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

	int res = g_pScaleSrc->get_screen_version();
	if ( res == 2 )  // 1024x600
	{
		ui->lineEditInfoTitle->setGeometry(QRect(498, 97, 220, 41));
		ui->textEditInfoBody->setGeometry(QRect(99, 180, 571, 161));
		ui->listViewInfoAttachment->setGeometry(QRect(100, 399, 571, 161));
        ui->pushButtonInfoManDetailAttach->setGeometry(QRect(764, 260, 230, 80));
        ui->pushButtonInfoManDetailPrev->setGeometry(QRect(764, 340, 230, 80));
        ui->pushButtonInfoManDetailNext->setGeometry(QRect(764, 420, 230, 80));
        ui->pushButtonInfoManDetailReturn->setGeometry(QRect(764, 500, 230, 80));
		ui->labeInfoTime->setGeometry(QRect(58, 97, 220, 41));
        ui->pushButtonInfoManDetailReply->setGeometry(QRect(764, 180, 230, 80));
		ui->framePanel->setGeometry(QRect(38, 17, 700, 580));
		ui->labelInfoFrom->setGeometry(QRect(278, 97, 220, 41));
		ui->frame->setGeometry(QRect(498, 97, 220, 41));
		ui->labelInfoBody->setGeometry(QRect(69, 140, 161, 41));
		ui->frameInfoBody->setGeometry(QRect(58, 138, 660, 221));
		ui->labelInfoAttachment->setGeometry(QRect(69, 355, 131, 41));
		ui->frameInfoAttachment->setGeometry(QRect(58, 359, 660, 218));
		ui->frameInfoBox->setGeometry(QRect(58, 97, 660, 480));
		ui->label_title->setGeometry(QRect(140, 25, 511, 51));
	}
	else if ( res == 3 )  // 1280x800
	{
		ui->lineEditInfoTitle->setGeometry(QRect(620, 140, 281, 61));
		ui->textEditInfoBody->setGeometry(QRect(130, 281, 731, 201));
		ui->listViewInfoAttachment->setGeometry(QRect(131, 571, 731, 141));
        ui->pushButtonInfoManDetailAttach->setGeometry(QRect(959, 353, 279, 99));
        ui->pushButtonInfoManDetailNext->setGeometry(QRect(959, 569, 279, 99));
        ui->pushButtonInfoManDetailReturn->setGeometry(QRect(959, 677, 279, 99));
        ui->pushButtonInfoManDetailPrev->setGeometry(QRect(959, 461, 279, 99));
        ui->pushButtonInfoManDetailReply->setGeometry(QRect(959, 245, 279, 99));
        ui->framePanel->setGeometry(QRect(56, 30, 858, 745));
		ui->frame->setGeometry(QRect(621, 140, 280, 61));
		ui->labelInfoBody->setGeometry(QRect(115, 220, 211, 51));
		ui->frameInfoBody->setGeometry(QRect(91, 200, 810, 301));
		ui->labelInfoAttachment->setGeometry(QRect(121, 510, 161, 51));
		ui->frameInfoAttachment->setGeometry(QRect(90, 500, 811, 251));
		ui->label_title->setGeometry(QRect(230, 40, 511, 71));
		ui->frameInfoBox->setGeometry(QRect(90, 140, 811, 611));
		ui->labelInfoFrom->setGeometry(QRect(381, 140, 241, 61));
		ui->labeInfoTime->setGeometry(QRect(90, 140, 292, 61));
	}

	//QFont font("Century Gothic");
	//font.setPixelSize(10);
	//ui->labeInfoTime->setFont(font);
	////ui->labeInfoTime->setStyleSheet("font: 19px; font-family: \"Century Gothic\";");
	//font.setPixelSize(10);
	//ui->labelInfoFrom->setFont(font);
	////ui->labelInfoFrom->setStyleSheet("font: 22px; font-family: \"Century Gothic\";");

	/*ui->pushButtonCallCenter->setText("");
	ui->pushButtonEmergency->setText("");
	ui->pushButtonHelp->setText("");
	ui->pushButtonInfoManDetailReturn->setText("");
	ui->pushButtonInfoManDetailPrev->setText("");
	ui->pushButtonInfoManDetailReturn->setText("");
	ui->pushButtonInfoManDetailAttach->setText("");
	*/
	QStringListModel *model = new QStringListModel();
	ui->listViewInfoAttachment->setModel(model);

	ui->listViewInfoAttachment->setViewMode(QListView::IconMode);
	ui->listViewInfoAttachment->setEditTriggers(QListView::NoEditTriggers);
	ui->listViewInfoAttachment->setMovement(QListView::Static);

	//m_pViewAttachemntDlg = NULL;
	m_pViewAttachemntDlg = new InfoManViewAttachmentDialog(this);
	/*int count, i;
	count = ((InfoManMainDialog *)parent)->GetTableViewInfoList()->model()->rowCount();
	m_AttachmentIndexArray = new int[count];
	for (i = 0; i < count; i++)
	{
		m_AttachmentIndexArray[i] = 0;
	}*/
}

/************************************************************
描述：信息详细内容显示对话框析构函数
参数：无
返回：无
************************************************************/
InfoManDetailDialog::~InfoManDetailDialog()
{
	//if (m_AttachmentIndexArray) delete m_AttachmentIndexArray;

	delete m_pViewAttachemntDlg;
	delete ui;
}

/************************************************************
描述：获取信息附件列表控件
参数：无
返回：信息附件列表控件的指针
************************************************************/
QListView *InfoManDetailDialog::GetListViewInfoAttachment()
{
	return ui->listViewInfoAttachment;
}

/************************************************************
描述：显示当前信息的详细内容
参数：无
返回：0 - 显示成功
	 -1 - 显示失败
************************************************************/
int InfoManDetailDialog::ShowCurrentInfoDetail()
{
	QTableView *pTabelViewInfoList = ((InfoManMainDialog *)parent())->GetTableViewInfoList();
	QStandardItemModel *model;
	int rowSel;
	QString strTime, strTimeSec;
	QString strTemp;
	int year, month, day, hour, minute, second, msecond;
	char filepath[PATH_MAX];
	char attchmnt_filename[PATH_MAX];
	FILE *fp;
	char str[80];
	char buf[512];
	char *ptr;
	char *buf_ptr;
	int len, i;
	bool is_body = false;
	int size;
	QStringList strings;
	QStringListModel *modelAttachment = (QStringListModel *)ui->listViewInfoAttachment->model();
	QModelIndex index;
	int pos;
	QStringList &addressList = g_pMainDlg->m_pAddressBookDlg->m_addressListNetName;
	QString strTemp2;

	rowSel = pTabelViewInfoList->currentIndex().row();
	if (rowSel < 0) return -1;

	model = (QStandardItemModel *)pTabelViewInfoList->model();

	strTime = model->data(model->index(rowSel, 0)).toString();
	strTimeSec = model->data(model->index(rowSel, 3)).toString();
	sscanf(strTime.toUtf8().data(), "%d-%d-%d %d:%02d", &year, &month, &day,
		&hour, &minute);
	sscanf(strTimeSec.toUtf8().data(), "%02d-%03d", &second, &msecond);
	sprintf(filepath, "/mnt/disk/info/%04d%02d%02d-%02d%02d%02d-%03d.txt", year, month, day,
		hour, minute, second, msecond);

	sprintf(str, "%d-%d-%d %d:%02d", year, month, day, hour, minute);////, second);
	sprintf(m_InfoTime, "%04d%02d%02d-%02d%02d%02d-%03d", year, month, day,
			hour, minute, second, msecond);
	ui->labeInfoTime->setText(str);

	if (model->item(rowSel, 5)->text() == "1")
	{
		fp = fopen(filepath, "rb+");
		if (!fp) return -1;

		len = fread(buf, 1, sizeof(buf), fp);
		buf[len] = 0;
		ptr = strstr(buf, "\nUNREAD:");
		if (ptr)
		{
			ptr += strlen("\nUNREAD:");
			*ptr = '0';
		}

		fseek(fp, 0, SEEK_SET);
		fwrite(buf, 1, len, fp);
		fclose(fp);

		model->item(rowSel, 0)->setIcon(QIcon());
		model->item(rowSel, 5)->setText("0");
	}

	fp = fopen(filepath, "rt");
	if (!fp) return -1;

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	//发送者信息
	if (fgets(buf, sizeof(buf) , fp) == NULL) goto jump;
	len = strlen("FROM:");
	if (strncmp(buf, "FROM:", len) != 0) goto jump;
	ptr = buf + len;
	while (*ptr && (*ptr != '\r') && (*ptr != '\n')) ptr++;
	*ptr = 0;

	strTemp = QString::fromUtf8(buf + len);
	pos = strTemp.indexOf("?");
	if (pos >= 0)
	{
		strTemp = strTemp.left(pos);
	}

	for (i = 0; i < addressList.count(); i++)
	{
		QString str = addressList.at(i);

		if (strTemp.length() > str.length())
		{
			if ((strTemp.left(str.length()) == str) && (strTemp.at(str.length()) == QChar('-')))
			{
				strTemp2 = g_pMainDlg->m_pAddressBookDlg->m_addressListName.at(i) + strTemp.mid(str.length());
				break;
			}
		}
		else if (strTemp == str)
		{
			strTemp2 = g_pMainDlg->m_pAddressBookDlg->m_addressListName.at(i);
			break;
		}
	}

	pos = strTemp.indexOf("-");
	if ((pos != -1) && ((pos != strTemp.length() - 2) || (strTemp.at(strTemp.length() - 1).isDigit())))
	{
		QString strTemp2 = g_strNativeName;//g_strLphoneName[0];
		int pos2;
		pos2 = strTemp2.indexOf("?");
		if (pos2 >= 0) strTemp2 = strTemp2.left(pos2);

		pos2 = strTemp2.indexOf("-");
		if ((pos2 != -1) && ((pos2 != strTemp2.length() - 2) || (strTemp2.at(strTemp2.length() - 1).isDigit())))
		{
			strTemp2 = strTemp2.left(pos2);
			if (strTemp.left(pos) == strTemp2)
			{
				strTemp = strTemp.mid(pos + strlen("-"));
			}
		}
	}

	m_senderNetName = strTemp;

	if (i >= addressList.count())
	{
		strTemp2 = strTemp;
	}

	ui->labelInfoFrom->setText(strTemp2);

	//发送者IP信息
	if (fgets(buf, sizeof(buf) , fp) == NULL) goto jump;
	len = strlen("FROM_IP:");
	if (strncmp(buf, "FROM_IP:", len) != 0) goto jump;
	ptr = buf + len;
	while (*ptr && (*ptr != '\r') && (*ptr != '\n')) ptr++;
	*ptr = 0;
	strcpy(m_SenderIPAddr, buf + len);

	//标题信息
	if (fgets(buf, sizeof(buf) , fp) == NULL) goto jump;
	len = strlen("TITLE:");
	if (strncmp(buf, "TITLE:", len) != 0) goto jump;
	ptr = buf + len;
	while (*ptr && (*ptr != '\r') && (*ptr != '\n')) ptr++;
	*ptr = 0;
	ui->lineEditInfoTitle->setText(QString::fromUtf8(buf + len));

	//UnRead
	if (fgets(buf, sizeof(buf) , fp) == NULL) goto jump;

	//消息序号
	////strTemp = tr("第") + QString::number(rowSel + 1) + tr("条 共") + QString::number(model->rowCount()) + tr("条");
	//sprintf(str, "第%2d条  共%2d条", rowSel + 1, model->rowCount());
	////ui->labelInfoIndex->setText(strTemp);

	//附件
	if (fgets(buf, sizeof(buf) , fp) == NULL) goto jump;
	if (strncmp(buf, "ATTACHMENT:", strlen("ATTACHMENT:")) == 0)
	{
		while (!feof(fp))
		{
			if (fgets(buf, sizeof(buf) , fp) == NULL) break;
			if (strncmp(buf, "BODY:", strlen("BODY:")) == 0)
			{
				is_body = true;
				break;
			}
			if (strncmp(buf, "\t", strlen("\t")) != 0) break;

			ptr = buf + strlen("\t");
			i = 0;
			while (*ptr && (*ptr != '\n') && (*ptr != '\r')) attchmnt_filename[i++] = *ptr++;
			attchmnt_filename[i] = 0;

			strings.append(QString::fromUtf8(attchmnt_filename));
		}
	}
	else if (strncmp(buf, "BODY:", strlen("BODY:")) == 0)
	{
		is_body = true;
	}

	modelAttachment->setStringList(strings);
	if (strings.count() > 0)
	{
		/*int attachment_idx = model->data(model->index(rowSel, 4)).toInt();
		if (attachment_idx == -1)
		{
			attachment_idx = 0;
			model->setData(model->index(rowSel, 4), 0);
		}
		*/
		index = modelAttachment->index(0, 0);

		ui->listViewInfoAttachment->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
	}

	//正文
	if (is_body)
	{
		buf_ptr = (char *)malloc(size);

		ptr = buf_ptr;

		len = strlen(buf + strlen("BODY:"));
		memcpy(ptr, buf + strlen("BODY:"), len);
		ptr += len;

		while (!feof(fp))
		{
			if (fgets(buf, sizeof(buf) , fp) == NULL) break;

			len = strlen(buf);
			memcpy(ptr, buf, len);
			ptr += len;
		}

		*ptr = 0;

		ui->textEditInfoBody->setText(QString::fromUtf8(buf_ptr));

		//printf("bcom free 1\n");
		free(buf_ptr);
	}

jump:
	fclose(fp);

	{
		int count;
		int i;

		count = ui->listViewInfoAttachment->model()->rowCount();

		if (m_pViewAttachemntDlg->m_CurScaleArray) delete [] m_pViewAttachemntDlg->m_CurScaleArray;
		m_pViewAttachemntDlg->m_CurScaleArray = new float[count];

		for (i = 0; i < count; i++)
		{
			m_pViewAttachemntDlg->m_CurScaleArray[i] = -1.0;
		}

		m_pViewAttachemntDlg->m_MaxScale = 1.0;
		m_pViewAttachemntDlg->m_MinScale = 1.0;

		if (m_pViewAttachemntDlg->m_Movie) delete m_pViewAttachemntDlg->m_Movie;
		m_pViewAttachemntDlg->m_Movie = NULL;
	}

	return 0;
}

/************************************************************
描述：关闭信息详细内容显示对话框
参数：无
返回：无
************************************************************/
void InfoManDetailDialog::on_pushButtonInfoManDetailReturn_pressed()
{
	done(0);
    //hide();
}

/************************************************************
描述：显示上一条信息详细内容
参数：无
返回：无
************************************************************/
void InfoManDetailDialog::on_pushButtonInfoManDetailPrev_pressed()
{
	QTableView *pTabelViewInfoList = ((InfoManMainDialog *)parent())->GetTableViewInfoList();
	int row = pTabelViewInfoList->currentIndex().row();
	QStandardItemModel *model = (QStandardItemModel *)pTabelViewInfoList->model();

	////model->setData(model->index(row, 4), ui->listViewInfoAttachment->currentIndex().row());

	if (row >= 1)
	{
		pTabelViewInfoList->selectRow(row - 1);
	}
	else
	{
		pTabelViewInfoList->selectRow(model->rowCount() - 1);
	}

	ShowCurrentInfoDetail();
}

/************************************************************
描述：显示下一条信息详细内容
参数：无
返回：无
************************************************************/
void InfoManDetailDialog::on_pushButtonInfoManDetailNext_pressed()
{
	QTableView *pTabelViewInfoList = ((InfoManMainDialog *)parent())->GetTableViewInfoList();
	QStandardItemModel *model = (QStandardItemModel *)pTabelViewInfoList->model();
	int row = pTabelViewInfoList->currentIndex().row();

	////model->setData(model->index(row, 4), ui->listViewInfoAttachment->currentIndex().row());

	if (row < model->rowCount() - 1)
	{
		pTabelViewInfoList->selectRow(row + 1);
	}
	else
	{
		pTabelViewInfoList->selectRow(0);
	}

	ShowCurrentInfoDetail();
}

/************************************************************
描述：打开当前信息图片附件显示窗口
参数：无
返回：无
************************************************************/
void InfoManDetailDialog::on_pushButtonInfoManDetailAttach_pressed()
{
	if (m_pViewAttachemntDlg->isVisible()) return;

	if (m_pViewAttachemntDlg->ShowCurrentAttachment() == -1)
	{
		return;
	}
    m_pViewAttachemntDlg->show();
}

/************************************************************
描述：信息附件列表控件双击回调函数
参数：index - 信息附件列表控件当前项的序号
返回：无
************************************************************/
void InfoManDetailDialog::on_listViewInfoAttachment_doubleClicked(QModelIndex index)
{
	Q_UNUSED(index);

    	on_pushButtonInfoManDetailAttach_pressed();
}

/************************************************************
描述：信息详细内容显示窗口结束函数
参数：ret_code - 窗口结束返回码
返回：无
************************************************************/
void InfoManDetailDialog::done(int ret_code)
{
	////QTableView *pTabelViewInfoList = ((InfoManMainDialog *)parent())->GetTableViewInfoList();
	////QStandardItemModel *model = (QStandardItemModel *)pTabelViewInfoList->model();
	////int row = pTabelViewInfoList->currentIndex().row();
	//////m_AttachmentIndexArray[row] = ui->listViewInfoAttachment->currentIndex().row();
	////model->setData(model->index(row, 4), ui->listViewInfoAttachment->currentIndex().row());

	QDialog::done(ret_code);
}

void InfoManDetailDialog::on_pushButtonInfoManDetailReply_pressed()
{
	InfoManMainDialog *pInfoManMainDlg = (InfoManMainDialog *)parent();

	if (pInfoManMainDlg->m_pInfoWriteDlg->isVisible()) return;

	pInfoManMainDlg->m_pInfoWriteDlg->ResetState(m_senderNetName);
	pInfoManMainDlg->m_pInfoWriteDlg->show();
	pInfoManMainDlg->m_pInfoWriteDlg->raise();
}

/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void InfoManDetailDialog::showEvent(QShowEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());

	pMainDlg->showRefreshTabFocus(this);
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void InfoManDetailDialog::hideEvent(QHideEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());

	m_pViewAttachemntDlg->hide();

	pMainDlg->hideRefreshTabFocus(this);

	((InfoManMainDialog *)parent())->ui->pushButtonInfoManMainDetail->setFocus(Qt::TabFocusReason);
}
