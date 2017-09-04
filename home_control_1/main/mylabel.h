#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QPainterPath>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QObject *parent = 0);

	bool m_bCalendar;

	QRgb m_glowRgb;

	int m_glowSize;

protected:
	void paintEvent(QPaintEvent *pe);

signals:

public slots:

};

void creatGlow(QImage *pImage, int expandSize, QRgb rgb);

#endif // MYLABEL_H
