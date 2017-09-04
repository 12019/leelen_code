#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QtGui/QLineEdit>

class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MyLineEdit(QObject *parent = 0);

    void mousePressEvent(QMouseEvent *);

    int m_type;
    int m_CheckType;
signals:

public slots:

};

#endif // MYLINEEDIT_H
