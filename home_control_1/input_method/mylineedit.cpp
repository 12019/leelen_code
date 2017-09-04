#include "mylineedit.h"
#include "main/maindialog.h"
#include "Big5frame.h"
#include "digitpanel.h"
#include "ui_digitpanel.h"
#include "Big5pinyinpanel.h"
#include "ui_big5pinyinpanel.h"
#include "maindialog.h"
#include "ui_inputinfodialog.h"

extern MainDialog *g_pMainDlg;

MyLineEdit::MyLineEdit(QObject *parent) :
    QLineEdit((QWidget *)parent)
{
	m_type 	    = 0;		// 正常状态
	m_CheckType = 0;		// 用于检验的类型
}

void MyLineEdit::mousePressEvent(QMouseEvent *e)
{

    g_pMainDlg->m_pInputMethodWidget->CallInputInfo = 0;
    g_pMainDlg->m_pInputMethodWidget->ui->InputInfoTextEdit->setText(this->text());
    g_pMainDlg->m_pInputMethodWidget->m_pMyLineEdit = this;

    g_pMainDlg->m_pInputMethodWidget->Set_Input_Check_Method(m_CheckType);
    g_pMainDlg->m_pInputMethodWidget->Set_Input_Mode(m_type);
    g_pMainDlg->m_pInputMethodWidget->show();

    if(e) QLineEdit::mousePressEvent(e);
}
