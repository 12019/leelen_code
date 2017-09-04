#include "helpdialog.h"
#include "ui_helpdialog.h"
#include <stdio.h>
#include "maindialog.h"
#include <QSettings>

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
/************************************************************
  该文件为联机帮助对话框主程序
************************************************************/

/************************************************************
  用该静态变量存储帮助对话框指针
************************************************************/
HelpDialog *HelpDialog::m_pDlg = NULL;

/************************************************************
描述：帮助对话框构造函数
参数：parent - 该对话框的父窗口指针
返回：无
************************************************************/
HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
	ui->setupUi(this);

	HelpDialog::m_pDlg = this;

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	resize(qApp->desktop()->size().width(),qApp->desktop()->size().height()); // modify kevin 2014.07.23

	RESCALESCREEN(ui->label)
	RESCALESCREEN(ui->pushButtonReturn)
	RESCALESCREEN(ui->lineEditHelpTitle)
	RESCALESCREEN(ui->textEditHelp)

	ui->lineEditHelpTitle->setReadOnly(true);
	ui->textEditHelp->setReadOnly(true);
}

/************************************************************
描述：帮助对话框析构函数
参数：无
返回：无
************************************************************/
HelpDialog::~HelpDialog()
{
    delete ui;
}

/************************************************************
描述：获取帮助对话框帮助标题控件
参数：无
返回：帮助标题控件指针
************************************************************/
QLineEdit *HelpDialog::getLineEditHelpTile()
{
	return ui->lineEditHelpTitle;
}

/************************************************************
描述：获取帮助对话框帮助内容控件
参数：无
返回：帮助内容控件指针
************************************************************/
QTextEdit *HelpDialog::getTextEditHelp()
{
	return ui->textEditHelp;
}

/************************************************************
描述：显示帮助对话框
参数：pCurDlg - 点击联机帮助的当前对话框
返回：无
************************************************************/
void HelpDialog::showHelpDialog(QDialog *pCurDlg)//QString help_file_name)
{
	QString help_file_name;
	char help_file_path[512];
	char buf[512];
	int len;
	FILE *fp;
	char *ptr, *buf_ptr;
	int size;

	if (m_pDlg->isVisible()) return;

	QSettings Settings(CONFIG_FILE, QSettings::IniFormat);
	QString sLang = Settings.value("language/lang").toString();
	help_file_name = "help/" + pCurDlg->objectName().toLower() + "_" + sLang + ".txt";
	//help_file_name = "help/" + help_file_name;

	//printf("help_file:%s\n", help_file_name.toLocal8Bit().data());

	if (strstr(QT_BUILD_KEY, "arm") == 0) //not arm-linux-gcc
	{
		help_file_name = QString::fromUtf8(help_file_name.toLatin1().data());
	}

	strcpy(help_file_path, help_file_name.toLatin1().data());

	fp = fopen(help_file_path, "rt");
	if (!fp)
	{
		m_pDlg->getLineEditHelpTile()->setText("");
		m_pDlg->getTextEditHelp()->setText("");

		m_pDlg->show();
		m_pDlg->raise();

		return;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	QTextCodec *gk_codec = QTextCodec::codecForName("GBK");

	//Get title
	if (fgets(buf, sizeof(buf) , fp) == NULL) goto jump;
	len = strlen("TITLE:");
	if (strncmp(buf, "TITLE:", len) != 0) goto jump;
	ptr = buf + len;
	while (*ptr && (*ptr != '\r') && (*ptr != '\n')) ptr++;
	*ptr = 0;


	m_pDlg->getLineEditHelpTile()->setText(gk_codec->toUnicode(buf + len));//QString::fromUtf8(buf + len));

	//Get body
	if (fgets(buf, sizeof(buf) , fp) == NULL) goto jump;
	len = strlen("BODY:");
	if (strncmp(buf, "BODY:", len) != 0) goto jump;

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

	m_pDlg->getTextEditHelp()->setText(gk_codec->toUnicode(buf_ptr));//QString::fromUtf8(buf_ptr));

	//printf("bcom free 7\n");
	free(buf_ptr);

jump:
	fclose(fp);
	//HelpDialog::m_pDlg->exec();
	//delete HelpDialog::m_pDlg;

	m_pDlg->raise();
	m_pDlg->show();

	m_pDlg->ui->pushButtonReturn->setFocus(Qt::TabFocusReason);
}

/************************************************************
描述：窗口显示时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void HelpDialog::showEvent(QShowEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)parent();
	pMainDlg->showRefreshTabFocus(this);
}

/************************************************************
描述：窗口隐藏时，刷新Tab焦点
参数：无
返回：无
************************************************************/
void HelpDialog::hideEvent(QHideEvent *)
{
	MainDialog *pMainDlg = (MainDialog *)parent();
	pMainDlg->hideRefreshTabFocus(this);
}

/************************************************************
描述：关闭帮助对话框
参数：无
返回：无
************************************************************/
void HelpDialog::on_pushButtonReturn_pressed()
{
	close();
}
