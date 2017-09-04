#include "mylabel.h"
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QMessageBox>
#include <stdio.h>
#include "maindialog.h"
#include "ui_maindialog.h"

extern MainDialog *g_pMainDlg;

MyLabel::MyLabel(QObject *parent) :
	QLabel((QWidget *)parent)
{
	m_bCalendar = false;
	m_glowRgb = qRgb(235, 235, 235);
	m_glowSize = 3;
}

void MyLabel::paintEvent(QPaintEvent *pe)
{
	Q_UNUSED(pe);

	QRect rc = geometry();
	rc = QRect(0, 0, rc.width() - 1, rc.height() - 1);

	if (m_bCalendar)
	{
		if (g_pMainDlg->m_pCurActiveDlg != g_pMainDlg) return;
		if ((pe->rect().x() == 0) && (pe->rect().width() < rc.width()/2)) return;
	}

	QString str = text();
	QFont fon = font();

	QFontMetrics fm(fon);
	int textflags = Qt::AlignCenter;
	QSize textsize = fm.size(textflags, str);

	int glowSize = m_glowSize;
	int offset = 4;
	QImage image(textsize.width() + glowSize * 2 + offset * 2, textsize.height() + glowSize * 2, QImage::Format_ARGB32);
	QSize sz = image.size();

	uchar *pImageBuf = image.bits();
	memset(pImageBuf, 0, image.numBytes());

	QPainter painterImage(&image);

	painterImage.setFont(fon);
	painterImage.setPen(QColor(128, 128, 128));//0xff, 0xff, 0xff));
	painterImage.drawText(glowSize + offset, fm.ascent() + glowSize, str);

	QColor color = palette().text().color();
	int value = (color.red() * 11 + color.green() * 16 + color.blue() * 5)/32;
	QRgb glowRgb;
	if (value >= 128)
	{
		glowRgb = qRgb(20, 20, 20);//64, 64, 64);
	}
	else
	{
		glowRgb = m_glowRgb;//qRgb(235, 235, 235);//192, 192, 192);
	}

#ifdef STYLE_YELLOW
        creatGlow(&image, glowSize, glowRgb); //改用白色粗体字，去掉描边 add by lidh 20150319
#endif

	int x = 0, y;

	if (alignment() & Qt::AlignLeft)
	{
		x = 0 + 1;
	}
	else if (alignment() & Qt::AlignHCenter)
	{
		x = rc.width()/2 - textsize.width()/2 - glowSize - offset + 1;
	}

	y = rc.height()/2 - textsize.height()/2 - glowSize + 1;

	QPainter painter(this);

	painter.drawImage(x, y, image);

	if (alignment() & Qt::AlignLeft)
	{
		x = glowSize + offset + 1;
	}
	else if (alignment() & Qt::AlignHCenter)
	{
		x = rc.width()/2 - textsize.width()/2 + 1;
	}

	y = rc.height()/2 - textsize.height()/2 + fm.ascent() + 1;

	painter.setFont(fon);
	painter.setPen(color);
	painter.drawText(x, y, str);
}

#define GetPixel(x, y) pImageBuf[(x) + (y) * (pitch)]
#define SetPixel(x, y, value) pImageBuf[(x) + (y) * (pitch)] = value

void creatGlow(QImage *pImage, int expandSize, QRgb rgb)
{
	QSize sz = pImage->size();
	int i, j, k, l;
	int squareExpandSize = (expandSize + 0.45) * (expandSize + 0.45);
	unsigned int *pImageBuf = (unsigned int *)pImage->bits();
	int pitch = pImage->bytesPerLine()/4;
	unsigned int srcARgb = 0x00000000;
	unsigned int dstARgb = 0xff000000 + rgb;

	for (j = expandSize; j < sz.height() - expandSize; j++)
	{
		for (i = expandSize; i < sz.width() - expandSize; i++)
		{
			unsigned int aRgb = GetPixel(i, j);
			if (aRgb == srcARgb) continue;
			if (aRgb == dstARgb) continue;

			unsigned int lARgb, rARgb, tARgb, bARgb;
			//unsigned int ltARgb, rtARgb, lbARgb, rbARgb;
			lARgb = GetPixel(i - 1, j);
			rARgb = GetPixel(i + 1, j);
			tARgb = GetPixel(i, j - 1);
			bARgb = GetPixel(i, j + 1);
			//ltARgb = GetPixel(i - 1, j - 1);
			//rtARgb = GetPixel(i + 1, j - 1);
			//lbARgb = GetPixel(i - 1, j + 1);
			//rbARgb = GetPixel(i + 1, j + 1);

			bool bLeft, bRight, bTop, bBottom;
			//bool bLeftTop, bRightTop, bLeftBottom, bRightBottom;
			bLeft = (lARgb != srcARgb) && (lARgb != dstARgb);
			bRight = (rARgb != srcARgb) && (rARgb != dstARgb);
			bTop = (tARgb != srcARgb) && (tARgb != dstARgb);
			bBottom = (bARgb != srcARgb) && (bARgb != dstARgb);
			//bLeftTop = (ltARgb != srcARgb) && (ltARgb != dstARgb);
			//bRightTop = (rtARgb != srcARgb) && (rtARgb != dstARgb);
			//bLeftBottom = (lbARgb != srcARgb) && (lbARgb != dstARgb);
			//bRightBottom = (rbARgb != srcARgb) && (rbARgb != dstARgb);

			if (bLeft && bRight && bTop && bBottom) continue;

			if (!bLeft)
			{
				for (k = 0; k < expandSize; k++)
				{
					if (GetPixel((i - 1) - k, j) == srcARgb)
					{
						SetPixel((i - 1) - k, j, dstARgb);
					}
				}
			}

			if (!bRight)
			{
				for (k = 0; k < expandSize; k++)
				{
					if (GetPixel((i + 1) + k, j) == srcARgb)
					{
						SetPixel((i + 1) + k, j, dstARgb);
					}
				}
			}

			if (!bTop)
			{
				for (k = 0; k < expandSize; k++)
				{
					if (GetPixel(i, (j - 1) - k) == srcARgb)
					{
						SetPixel(i, (j - 1) - k, dstARgb);
					}
				}
			}

			if (!bBottom)
			{
				for (k = 0; k < expandSize; k++)
				{
					if (GetPixel(i, (j + 1) + k) == srcARgb)
					{
						SetPixel(i, (j + 1) + k, dstARgb);
					}
				}
			}

			if ((!bLeft) && (!bTop))// && (!bLeftTop))
			{
				for (k = 1; k < expandSize + 1; k++)
				{
					for (l = 1; l < expandSize + 1; l++)
					{
						if ((k * k + l * l < squareExpandSize))
						{
							if (GetPixel(i - k, j - l) == srcARgb)
							{
								SetPixel(i - k, j - l, dstARgb);
							}
						}
					}
				}
			}

			if ((!bRight) && (!bTop))// && (!bRightTop))
			{
				for (k = 1; k < expandSize + 1; k++)
				{
					for (l = 1; l < expandSize + 1; l++)
					{
						if ((k * k + l * l < squareExpandSize))
						{
							if (GetPixel(i + k, j - l) == srcARgb)
							{
								SetPixel(i + k, j - l, dstARgb);
							}
						}
					}
				}
			}

			if ((!bLeft) && (!bBottom))// && (!bLeftBottom))
			{
				for (k = 1; k < expandSize + 1; k++)
				{
					for (l = 1; l < expandSize + 1; l++)
					{
						if ((k * k + l * l < squareExpandSize))
						{
							if (GetPixel(i - k, j + l) == srcARgb)
							{
								SetPixel(i - k, j + l, dstARgb);
							}
						}
					}
				}
			}

			if ((!bRight) && (!bBottom))
			{
				for (k = 1; k < expandSize + 1; k++)
				{
					for (l = 1; l < expandSize + 1; l++)
					{
						if ((k * k + l * l < squareExpandSize))
						{
							if (GetPixel(i + k, j + l) == srcARgb)
							{
								SetPixel(i + k, j + l, dstARgb);
							}
						}
					}
				}
			}
		}
	}
}
