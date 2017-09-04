#include "mymessagebox.h"
#include <QApplication>
#include <QDesktopWidget>
#include "commonpushbutton.h"
#include "maindialog.h"
#include "exosip_main.h"
#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

extern int b_ring_play_run;
extern int b_ring_play_passwd;
extern MainDialog *g_pMainDlg;
extern eXosip_element_t *eXosipElement;
extern char g_strLphoneRingFile[5][50];

MyMessageBox::MyMessageBox(QWidget *parent) :
	QDialog(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setObjectName("MyMessageBox");

	m_pFrame = new QFrame(this);
	m_pFrame->setStyleSheet("border: 2px solid #C0C0C0");

	m_pClickedButton = NULL;

	m_pLabel = NULL;

	m_pAutoCloseTimer = new QTimer(this);
	connect(m_pAutoCloseTimer, SIGNAL(timeout()), this, SLOT(AutoClose()));
}

MyMessageBox::~MyMessageBox()
{
	if ( m_pAutoCloseTimer->isActive() )
		m_pAutoCloseTimer->stop();
	delete m_pAutoCloseTimer;

	if (m_pLabel) delete m_pLabel;
	delete m_pFrame;

	int i;
	for (i = 0; i < m_pushButtonList.count(); i++)
	{
		delete m_pushButtonList.at(i);
	}

	m_pushButtonList.clear();
}

void MyMessageBox::AutoClose()
{
	m_pAutoCloseTimer->stop();
	on_buttonClicked();
}

void MyMessageBox::Start_Auto_Close()
{
	if ( m_pAutoCloseTimer->isActive() )
		m_pAutoCloseTimer->stop();
	m_pAutoCloseTimer->start(2*1000);
}

void MyMessageBox::setText(const QString &text)
{
	if (m_pLabel) delete m_pLabel;

	m_pLabel = new QLabel(this);
    m_pLabel->setObjectName("labelMyMessageBox");
	m_pLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	m_pLabel->setContentsMargins(20, 10, 20, 10);

	m_pLabel->setText(text);
	m_pLabel->adjustSize();

	//updateSize();
}

CommonPushButton *MyMessageBox::addButton(const QString &text, const QString objectName)
{
	CommonPushButton *pushButton = new CommonPushButton(this);
	if (objectName != "")
	{
		pushButton->setObjectName(objectName);
	}
	else
	{
        pushButton->setObjectName("pushButtonMyMessageBoxOK");
	}

	pushButton->setText(text);
	pushButton->adjustSize();

	int re = g_pScaleSrc->get_screen_version();

	switch ( re ) 
	{
		case 1:  // 800x480 
			pushButton->setFixedSize(153, 60); //pushButton->width() + 45 + 10 * 2, 45);
			break;

		case 2: // 1024x600 
			pushButton->setFixedSize(153, 60); //pushButton->width() + 45 + 10 * 2, 45);
			break;

		case 3: // 1280x800
			pushButton->setFixedSize(220, 70);   //pushButton->width() + 45 + 10 * 2, 45);
			break;

		default:	
			pushButton->setFixedSize(153, 60); //pushButton->width() + 45 + 10 * 2, 45);
			break;
	}				/* -----  end switch  ----- */

	//pushButton->m_offsetX = 20;

	connect(pushButton, SIGNAL(clicked()),
			this, SLOT(on_buttonClicked()));

	m_pushButtonList.append(pushButton);

	//updateSize();

	return pushButton;
}

void MyMessageBox::on_buttonClicked()
{
	m_pClickedButton = (QPushButton *)sender();

    if(g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_pAutoQuitTimer->isActive())
    {
        g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread->m_pAutoQuitTimer->stop();
    }
    //david

    if(b_ring_play_run && (!b_ring_play_passwd))
	{
		my_ring_stop();       
        if(eXosipElement->state == ICT_RINGBACK)
        {
           my_ring_start(g_strLphoneRingFile[0], 2000);
        }

	}

    b_ring_play_passwd = 0;
	done(0);
}

QPushButton *MyMessageBox::clickedButton()
{
	return m_pClickedButton;
}

void MyMessageBox::updateSize()
{
	int width1 = 0;
	int width2 = 0;
	int height1 = 80;
	int height2 = 0;

	if (m_pLabel)
	{
		if (m_pLabel->height() < 80)
		{
			m_pLabel->setFixedHeight(80);
		}

		width1 = m_pLabel->width();
		height1 = m_pLabel->height() + 10;////lg bug
	}

	int middleSpan = 10;
	int borderSpan = 15;

	int i;
	for (i = 0; i < m_pushButtonList.count(); i++)
	{
		QPushButton *pButton = m_pushButtonList.at(i);
		if (i > 0)
		{
			width2 += middleSpan; //span
		}

		width2 += pButton->width();

		if (height2 < pButton->height())
		{
			height2 = pButton->height();
		}
	}

	int width3 = (width1 >= width2) ? width1 : width2;
	int height = height1 + height2;

	//left_border_span + right_border_span
	int width = width3 + (3 + borderSpan) * 2;
	//top_border + top_span + middle_span + bottom_span + bottom_border
	height += 44/*22*/ + borderSpan + middleSpan + borderSpan + 8;

	if (width < 207/*122*/) width = 207/*122*/;
	if (height < 87/*60*/) height = 87/*60*/;

	int tmp = width - 87/*50*/ * 2; //left + right
	tmp = tmp % 33/*22*/;
	if (tmp)
	{
		width += 33/*22*/ - tmp;
	}

	if (m_pLabel)
	{
		m_pLabel->setFixedWidth(width3);
		m_pLabel->move(width/2 - /*width1*/ width3/2, 44/*22*/ + borderSpan + 5); ////lg bug
	}

	m_pFrame->setFixedSize(width3, height1);
	m_pFrame->move(width/2 - width3/2, 44/*22*/ + borderSpan);

	int offsetX = width/2 - width2/2;
	int offsetY = 44/*22*/  + borderSpan + height1 + middleSpan + height2/2;

	/*if (width1 >= width2)
	{
		offsetX = width/2 + width1/2 - width2;
	}
	else
	{
		offsetX = width/2 - width2/2;
	}
	*/

	for (i = 0; i < m_pushButtonList.count(); i++)
	{
		QPushButton *pButton = m_pushButtonList[i];

		pButton->move(offsetX, offsetY - pButton->height()/2);
		offsetX += pButton->width() + middleSpan;//span
	}

	setFixedSize(width, height)	;

	//printf("%d %d %d\n", width1, width2, width);

	QSize sz = QApplication::desktop()->size();
	move(sz.width()/2 - width/2, sz.height()/2 - height/2 - 6 /*handle_height*/);
}

void MyMessageBox::showEvent(QShowEvent *)
{
	updateSize();

	//g_pMainDlg->repaint();
}

//lg 12.22
void MyMessageBox::hideEvent(QHideEvent *)
{
    done(0);
}
//lg
