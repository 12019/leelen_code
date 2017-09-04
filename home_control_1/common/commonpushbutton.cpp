#include "commonpushbutton.h"
#include <QPainter>
#include <QStyle>
#include <QStylePainter>
#include <QStyleOptionButton>

CommonPushButton::CommonPushButton(QObject *parent) :
	QPushButton((QWidget *)parent)
{
	m_offsetX = 20;
	m_offsetY = 0;
	m_offsetXPressed = 0;
}

void CommonPushButton::paintEvent(QPaintEvent *pe)
{
	Q_UNUSED(pe);

	//printf("CommonPushButton::paintEvent 1\n");
	QStylePainter painter(this);
	QStyleOptionButton option;
	initStyleOption(&option);

	style()->drawControl(QStyle::CE_PushButtonBevel, &option, &painter, this);

	int offsetX = m_offsetX;
	if (option.state & QStyle::State_Sunken)
	{
		offsetX += m_offsetXPressed;
	}

	QRect rc = geometry();
	rc = QRect(offsetX, m_offsetY, rc.width() - 1, rc.height() - 1);
	painter.setViewport(rc);
	style()->drawControl(QStyle::CE_PushButtonLabel, &option, &painter, this);

	//QRect rc = geometry();
	//rc = QRect(0, 0, rc.width() - 1, rc.height() - 1);
	//painter.drawRect(rc);

	painter.end();

	//printf("CommonPushButton::paintEvent 2\n");
}
