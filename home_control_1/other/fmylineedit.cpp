#include "fmylineedit.h"

FMyLineEdit::FMyLineEdit(QObject *parent) :
    QLineEdit((QWidget*)parent)
{

}

void FMyLineEdit::focusOutEvent(QFocusEvent *e)
{
    QLineEdit::focusOutEvent(e);
    emit lostFocus();
}
