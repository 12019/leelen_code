#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include <QPainterPath>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyPushButton(QObject *parent = 0);

protected:
	void paintEvent(QPaintEvent *pe);

	bool event(QEvent *e);

signals:

public slots:

};

#endif // MYPUSHBUTTON_H
