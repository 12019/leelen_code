#ifndef FMYLINEEDIT_H
#define FMYLINEEDIT_H

#include <QLineEdit>

class FMyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit FMyLineEdit(QObject *parent = 0);
    

    void focusOutEvent(QFocusEvent *);

signals:
    void lostFocus();

public slots:
    
};

#endif // FMYLINEEDIT_H
