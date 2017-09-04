#ifndef INPUTININFORDIALOG_H
#define INPUTININFORDIALOG_H
#include <QDialog>
#include "digitpanel.h"
#include "mylineedit.h"
#include "mytextedit.h"
#include "infomanmaindialog.h"
#include <QByteArray>

namespace Ui {
    class InputInfoDialog;
}

class InputInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputInfoDialog(QWidget *parent = 0);
    ~InputInfoDialog();

    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
    void Set_Input_Mode(int);  	// 设置输入法显示状态
    void Set_Input_Check_Method(int);  // 检验字符串合法性
    int CallInputInfo;		//1:MyTextEdit 0:MyLineEdit

    Ui::InputInfoDialog *ui;

    MyTextEdit *m_pMyTextEdit;
    MyLineEdit *m_pMyLineEdit;

protected:
	int m_Check_Type;

private slots:
    void sigInputMethReceiveData(QString data);
    void on_InputInfoButtonConfirm_pressed();
    void on_InputInfoButtonCancel_pressed();
};
#endif // INPUTININFORDIALOG_H
