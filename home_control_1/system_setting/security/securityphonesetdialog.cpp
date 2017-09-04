#include "securityphonesetdialog.h"
#include "ui_securityphonesetdialog.h"
#include "syssetmaindialog.h"
#include "securityparamsetdialog.h"
#include <QSettings>
#include <QMessageBox>
#include "ui_syssetmaindialog.h"
#include <QBitmap>
#include "maindialog.h"

SecurityPhoneSetDialog::SecurityPhoneSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecurityPhoneSetDialog)
{
    ui->setupUi(this);

	if (strstr(QT_BUILD_KEY, "arm") != 0) //not arm-linux-gcc
	{
		setWindowFlags(Qt::FramelessWindowHint);
		hide();
	}

	move(150, 0);

    /*QPixmap pixmap_add(":/images/voip_dialog/volumeadd-1.bmp");
	QRect rc = ui->pushButtonNumberAdd->geometry();
	pixmap_add = pixmap_add.scaled(rc.width(), rc.height());
	QBitmap bitmap_add = pixmap_add.createMaskFromColor(QColor(255, 255,255));
	ui->pushButtonNumberAdd->setMask(bitmap_add);
	ui->pushButtonNumberSub->setMask(bitmap_add);
	*/
	setAttribute(Qt::WA_TranslucentBackground, true);

	//setAttribute(Qt::WA_DeleteOnClose);
}

SecurityPhoneSetDialog::~SecurityPhoneSetDialog()
{
    delete ui;
}

void SecurityPhoneSetDialog::on_pushButtonSecurityParam_clicked()
{
	/*SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();

	pSysSetMainDlg->m_curDlg = (QDialog *)new SecurityParamSetDialog(pSysSetMainDlg);
	pSysSetMainDlg->m_curDlg->show();

	close();*/
	hide();

	SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();
	pSysSetMainDlg->m_curDlg->show();
}

void SecurityPhoneSetDialog::on_pushButtonNumberSub_clicked()
{
	QSettings settings(CONFIG_FILE, QSettings::IniFormat);
	int number = ui->labelNumber->text().toInt();

	if (number > 1)
	{
		number--;

		ui->labelNumber->setText(QString::number(number));
		ui->lineEditPhoneNumber->setText(
			settings.value("alert_param/phonenumber" + QString::number(number), "").toString());
	}
}

void SecurityPhoneSetDialog::on_pushButtonNumberAdd_clicked()
{
	QSettings settings(CONFIG_FILE, QSettings::IniFormat);
	int number = ui->labelNumber->text().toInt();

	if (number < 3)
	{
		number++;

		ui->labelNumber->setText(QString::number(number));
		ui->lineEditPhoneNumber->setText(
			settings.value("alert_param/phonenumber" + QString::number(number), "").toString());
	}
}

void SecurityPhoneSetDialog::on_pushButtonSet_clicked()
{
	QSettings settings(CONFIG_FILE, QSettings::IniFormat);
	settings.setValue("alert_param/phonenumber" + ui->labelNumber->text(),
				  ui->lineEditPhoneNumber->text());

	setAllButtonsEnabled(this, false);
	setAllButtonsEnabled((QDialog *)parent(), false);
	ui->frame->setFocus(Qt::TabFocusReason);
	MyMessageBox messBox(this);
	CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
	//Q_UNUSED(okButton);
	okButton->setFocus(Qt::TabFocusReason);
	messBox.setText(tr("报警电话号码") + ui->labelNumber->text() + tr("设置成功"));
	messBox.show();
	QRect rc = messBox.geometry();
	QRect rc2 = geometry();
	rc.setRect((rc2.width() - rc.width())/2, (rc2.height() - rc.height())/2, rc.width(), rc.height());
	messBox.setGeometry(rc);
	messBox.exec();
	setAllButtonsEnabled((QDialog *)parent(), true);
	setAllButtonsEnabled(this, true);
	ui->pushButtonSet->setFocus(Qt::TabFocusReason);
}

void SecurityPhoneSetDialog::showEvent(QShowEvent *)
{
	QString strPhoneNumber;
	QSettings settings(CONFIG_FILE, QSettings::IniFormat);
	ui->labelNumber->setText("1");
	strPhoneNumber = settings.value("alert_param/phonenumber1").toString();
	ui->lineEditPhoneNumber->setText(strPhoneNumber);
}

void SecurityPhoneSetDialog::hideEvent(QHideEvent *)
{
	//SysSetMainDialog *pSysSetMainDlg = (SysSetMainDialog *)parent();
	//pSysSetMainDlg->ui->pushButtonSecuritySet->setFocus(Qt::TabFocusReason);
}

