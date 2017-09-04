#include "mypushbutton.h"
#include <QPainter>
#include "maindialog.h"
#include "mylabel.h"
#include <QStyle>

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

extern MainDialog     *g_pMainDlg;


MyPushButton::MyPushButton(QObject *parent) :
	QPushButton((QWidget *)parent)
{
}

void MyPushButton::paintEvent(QPaintEvent *pe)
{
	Q_UNUSED(pe);

	/*if ((g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pSecurityMainDlg->m_pSecurityPlayerDlg)
		|| (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pSecurityMainDlg->m_pSecurityAlertDlg)
		|| (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pMediaManDlg->m_pMediaPlayerDlg))
	{
		printf("lg MyPushButton::paintEvent:%d %d %d %d\n", pe->rect().x(), pe->rect().y(),
			pe->rect().width(), pe->rect().height());

		//return;
	}*/

	//可能二次门铃响后不能正确显示按钮
	if (g_pMainDlg->m_pCurActiveDlg != parent()) return;
	//printf("lg MyPushButton::paintEvent:%s %d %d %d %d\n", text().toLocal8Bit().data(), pe->rect().x(), pe->rect().y(),
	//	pe->rect().width(), pe->rect().height());

	QStylePainter painter(this);
	QStyleOptionButton option;
	initStyleOption(&option);

	style()->drawControl(QStyle::CE_PushButtonBevel, &option, &painter, this);
	if (1)//!(option.state & (/*QStyle::State_HasFocus |*/ QStyle::State_Sunken)))
	{
		QRect rc = geometry();
		rc = QRect(0, 0, rc.width() - 1, rc.height() - 1);

		QColor color = option.palette.foreground().color();
		QString str = text();
		QFont fon = font();

		QFontMetrics fm(fon);
		int textflags = Qt::AlignCenter;
		QSize textsize = fm.size(textflags, str);

        int glowSize = 3 ;
		int offset = 4;

		QImage image(textsize.width() + glowSize * 2 + offset * 2, textsize.height() + glowSize * 2, QImage::Format_ARGB32);
		QSize sz = image.size();

		uchar *pImageBuf = image.bits();
		memset(pImageBuf, 0, image.numBytes());

		QPainter painterImage(&image);

		painterImage.setFont(fon);
		painterImage.setPen(QColor(128, 128, 128));//0xff, 0xff, 0xff));

		int a = -7; 
        painterImage.drawText(-a, fm.ascent() + glowSize, str);

		int value = (color.red() * 11 + color.green() * 16 + color.blue() * 5)/32;
		QRgb glowRgb;
		if (value >= 128)
		{
            glowRgb = qRgb(20, 20, 20);//64, 64, 64);
		}
		else
		{
			glowRgb = qRgb(235, 235, 235);//192, 192, 192);
		}

#ifdef STYLE_YELLOW
        	creatGlow(&image, glowSize, glowRgb); //改用白色粗体字，去掉描边 add by lidh 20150319
#endif

		int x, y;
		x = rc.width()/2 - textsize.width()/2 - glowSize - offset;
		y = rc.height()/2 - textsize.height()/2 - glowSize;

		if (option.state & QStyle::State_Sunken)
		{
			x += style()->proxy()->pixelMetric(QStyle::PM_ButtonShiftHorizontal, &option, this);
			y += style()->proxy()->pixelMetric(QStyle::PM_ButtonShiftVertical, &option, this);
		}

		int i = 0;
		while (str.at(i) == '\n')
		{
			i++;
			y += fm.lineSpacing();
		}

        painter.drawImage(x, y, image);
	}
	style()->drawControl(QStyle::CE_PushButtonLabel, &option, &painter, this);

	//选中画虚框
	/*if (option.state & QStyle::State_HasFocus) {
		QStyleOptionFocusRect fropt;
		fropt.QStyleOption::operator=(option);
		fropt.rect = style()->subElementRect(QStyle::SE_PushButtonFocusRect, &option, this);
		style()->drawPrimitive(QStyle::PE_FrameFocusRect, &fropt, &painter, this);
	}*/

	painter.end();
	//printf("MyPushButton::paintEvent 2\n");
}

bool MyPushButton::event(QEvent *e)
{
	//printf("lg event:%d\n", e->type());
	//printf("lg g_pMainDlg:%p\n", g_pMainDlg);
	/*if (g_pMainDlg && g_pMainDlg->m_pCurActiveDlg)
	{
		if ((g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pSecurityMainDlg->m_pSecurityPlayerDlg)
			|| (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pSecurityMainDlg->m_pSecurityAlertDlg)
			|| (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg->m_pMediaManDlg->m_pMediaPlayerDlg))
		{
			printf("lg event 1:%d\n", e->type());
			return true;
		}
	}
	*/
	//printf("lg event 2:%d\n", e->type());
	return QPushButton::event(e);
}
