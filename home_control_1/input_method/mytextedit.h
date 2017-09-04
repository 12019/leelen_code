#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QtGui/QTextEdit>

class MyTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit MyTextEdit(QObject *parent = 0);

    void mousePressEvent(QMouseEvent *);

signals:

public slots:

};

#endif // MYTEXTEDIT_H
