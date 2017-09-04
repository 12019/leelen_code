#ifndef COMMONPUSHBUTTON_H
#define COMMONPUSHBUTTON_H

#include <QPushButton>

class CommonPushButton : public QPushButton
{
Q_OBJECT
public:
	explicit CommonPushButton(QObject *parent = 0);

	int m_offsetX;
	int m_offsetY;
	int m_offsetXPressed;

protected:
	void paintEvent(QPaintEvent *pe);

signals:

public slots:

};

#endif // COMMONPUSHBUTTON_H
