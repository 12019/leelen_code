#include <QtGui>
#include "scrolllineedit.h"
#include <time.h>

#define BORDER_SIZE		5
#define BORDER_RADIUS	7

#define BORDER_COLOR	QColor(0xE0, 0xE0, 0xE0)
#define BK_COLOR		QColor(0xFF, 0xFF, 0xFF)
#define FG_COLOR		QColor(0x00, 0x00, 0x00)

#define SCROLL_STEP		2
#define TIMER_INTERVAL	35

#define TEXT_SPACE		50

ScrollLineEdit::ScrollLineEdit(QWidget *parent)
	:QLineEdit(parent)
{
	//textColor = QColor();
	//borderColor = QColor(202,239,209,150);
	offset = 0;
	myTimerId = 0;
}

void ScrollLineEdit::paintEvent(QPaintEvent *event)
{          
	Q_UNUSED(event);

	QPainter painter(this);

	//int i;
	int x, y, w, h;
	QRect rc = rect();
	x = rc.x();
	y = rc.y();
	w = rc.width();
	h = rc.height();

	/*painter.setPen(BORDER_COLOR);
	for (i = 0; i < BORDER_SIZE; i++)
	{
		painter.drawRoundedRect(x + i, y + i, w - i * 2, h - i * 2, BORDER_RADIUS - i, BORDER_RADIUS - i);
	}
	painter.setPen(BK_COLOR);
	for (; i < BORDER_RADIUS; i++)
	{
		painter.drawRoundedRect(x + i, y + i, w - i * 2, h - i * 2, BORDER_RADIUS - i, BORDER_RADIUS - i);
	}
	painter.fillRect(x + BORDER_RADIUS, y + BORDER_RADIUS, w - BORDER_RADIUS * 2 + 1, h - BORDER_RADIUS * 2 + 1, BK_COLOR);
	*/
	painter.setClipRect(x + BORDER_SIZE, y + BORDER_SIZE, w - BORDER_SIZE * 2 + 1, h - BORDER_SIZE * 2 + 1);

	int textWidth = fontMetrics().width(text());
    if (textWidth < 1)
		return;
	
	painter.setPen(palette().text().color());
	if (textWidth <= width() - BORDER_SIZE * 2)
	{
		int x = (width() - BORDER_SIZE * 2 - textWidth)/2;
		painter.drawText(x, 0, textWidth, height(),
						alignment(), text());
	}
	else 
	{
		int x = -offset;
		while (x < width() - BORDER_SIZE * 2)
		{
			painter.drawText(x, 0, textWidth, height(),
                        Qt::AlignLeft | Qt::AlignVCenter, text());
			x += textWidth + TEXT_SPACE;
		}   
	}
}

void ScrollLineEdit::timerEvent(QTimerEvent *event)
{   
	if (event->timerId() == myTimerId)
	{
		if (m_text != text())
		{
			offset = BORDER_SIZE;
			m_text = text();
		}

		if (width() - BORDER_SIZE * 2 < fontMetrics().width(text()))
		{
			update();

			offset += SCROLL_STEP;
			if (offset >= TEXT_SPACE + fontMetrics().width(text()))
				offset = BORDER_SIZE;
			//scroll(-SCROLL_STEP, 0);
		}
	}
	else
	{
		QWidget::timerEvent(event);
	}
}

void ScrollLineEdit::showEvent(QShowEvent *event)
{
	Q_UNUSED(event);
	offset = BORDER_SIZE;
	myTimerId = startTimer(TIMER_INTERVAL);
}

void ScrollLineEdit::hideEvent(QHideEvent *event)
{
	Q_UNUSED(event);
	killTimer(myTimerId);
	myTimerId = 0;
}

/*void ScrollLineEdit::setTextColor(const QColor &newColor)
{
	textColor = newColor;
	update();
	updateGeometry();
}

void ScrollLineEdit::setBorderColor(const QColor &newColor)
{
	borderColor = newColor;
	update();
	updateGeometry();
}
*/
