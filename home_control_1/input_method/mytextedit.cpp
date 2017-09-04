#include "mytextedit.h"
#include "main/maindialog.h"
#include "Big5frame.h"
#include "digitpanel.h"
#include "ui_digitpanel.h"
#include "Big5pinyinpanel.h"
#include "ui_big5pinyinpanel.h"
#include "maindialog.h"
#include "ui_inputinfodialog.h"

extern MainDialog *g_pMainDlg;

MyTextEdit::MyTextEdit(QObject *parent) :
    QTextEdit((QWidget *)parent)
{
}

void MyTextEdit::mousePressEvent(QMouseEvent *e)
{
    g_pMainDlg->m_pInputMethodWidget->CallInputInfo = 1;
    g_pMainDlg->m_pInputMethodWidget->ui->InputInfoTextEdit->setText(this->toPlainText());
    g_pMainDlg->m_pInputMethodWidget->show();

    g_pMainDlg->m_pInputMethodWidget->m_pMyTextEdit = this;

    if(e) QTextEdit::mousePressEvent(e);
}


