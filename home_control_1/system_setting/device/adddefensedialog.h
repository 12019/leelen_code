#ifndef ADDDEFENSEDIALOG_H
#define ADDDEFENSEDIALOG_H

#include <QDialog>
#include "mymessagebox.h"
#include "commonpushbutton.h"

namespace Ui {
    class AddDefenseDialog;
}

class AddDefenseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDefenseDialog(QWidget *parent = 0);
    ~AddDefenseDialog();

	int MessageBox(QDialog *parent, QString text, QString button_caption);

	void showCurrentItem(int defenseType, int defenseNumber);

	int m_defenseType; //防区类型(有线/无线)
	int m_channel; //防区通道(0-3)
	int m_property; //防区属性
	int m_type; //防区名称

	MyMessageBox *m_pAbnormalMsgBox; //防区修改时异常防区信息显示对话框
	CommonPushButton *m_pAbnormalMsgBoxOkBtn; //防区修改时异常防区信息显示对话框的"确定"按钮

	int getWirelessDefenseIdentity();

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

    Ui::AddDefenseDialog *ui;

public slots:
	void showAbnormalMsgBox(QString strText);

private slots:
	void on_pushButtonAddDefReset_pressed();
	void on_pushButtonAddDefCalibrate_pressed();
	void on_pushButtonAddDefType_pressed();
	void on_pushButtonAddDefChannel_pressed();
	void on_pushButtonAddDefProperty_pressed();
	void on_pushButtonAddDefSubType_pressed();
	void on_pushButtonAddDefDelayAlarm_pressed();
	void on_pushButtonAddDefBypass_pressed();
	void on_pushButtonAddDefSave_pressed();
	void on_pushButtonAddDefNext_pressed();
	void on_pushButtonAddDefPrev_pressed();
	void on_pushButtonAddDefDel_pressed();
	void on_pushButtonAddDefReboot_pressed();
	void on_pushButtonAddDefReturn_pressed();
};

#define DEFENSE_TYPE_WIRE		0
#define DEFENSE_TYPE_WIRELESS		1

#define DEFENSE_PROPERTY_NORMAL		0
#define DEFENSE_PROPERTY_OUTSIDE	1
#define DEFENSE_PROPERTY_ATHOME		2

extern const char *g_strDefenseType[2];
extern const char *g_strProperty[3];
extern const char *g_strType[6];

#endif // ADDDEFENSEDIALOG_H
