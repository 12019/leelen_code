#include "calibrationdialog.h"
#include <QWSPointerCalibrationData>
#include <QPainter>
#include <QFile>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QScreen>
#include <QWSServer>
#include "maindialog.h"
#include "application.h"

/************************************************************
  该文件为触摸屏校准窗口主程序
************************************************************/

extern MainDialog *g_pMainDlg;

/************************************************************
  根据屏幕上的TopLeft，TopRight，BottomRight，BottomLeft，Center
  五个点进行触摸屏校准
************************************************************/
static int location[5] = {QWSPointerCalibrationData::TopLeft,
			  QWSPointerCalibrationData::TopRight,
			  QWSPointerCalibrationData::BottomRight,
			  QWSPointerCalibrationData::BottomLeft,
			  QWSPointerCalibrationData::Center};

/************************************************************
描述：触摸屏校准窗口构造函数
参数：parent - 父窗口指针
返回：无
************************************************************/
CalibrationDialog::CalibrationDialog(QWidget *parent) :
	QDialog(parent)
{
	setObjectName("CalibrationDialog");

	QRect desktop = QApplication::desktop()->geometry();
	desktop.moveTo(QPoint(0, 0));
	setGeometry(desktop);

	setFocusPolicy(Qt::StrongFocus);
	setFocus();
	setModal(true);

	int width = qt_screen->deviceWidth();
	int height = qt_screen->deviceHeight();

	int dx = width / 10;
	int dy = height / 10;

	QPoint *points = data.screenPoints;
	points[QWSPointerCalibrationData::TopLeft] = QPoint(dx, dy);
	points[QWSPointerCalibrationData::BottomLeft] = QPoint(dx, height - dy);
	points[QWSPointerCalibrationData::BottomRight] = QPoint(width - dx, height - dy);
	points[QWSPointerCalibrationData::TopRight] = QPoint(width - dx, dy);
	points[QWSPointerCalibrationData::Center] = QPoint(width / 2, height / 2);

	pressCount = 0;
}

/************************************************************
描述：触摸屏校准窗口析构函数
参数：无
返回：无
************************************************************/
CalibrationDialog::~CalibrationDialog()
{
}

/************************************************************
描述：运行触摸屏校准窗口
参数：无
返回：无
************************************************************/
int CalibrationDialog::exec()
{
	((Application *)qApp)->setLCDOn();

	pressCount = 0;

	QWSServer::mouseHandler()->clearCalibration();
	grabMouse();

	//g_pMainDlg->m_pAbnormalMsgBox->hide();

	activateWindow();
	int ret = QDialog::exec();
	//show();
	//raise();
	releaseMouse();
	return ret;
}

/************************************************************
描述：触摸屏校准窗口自绘函数
参数：无
返回：无
************************************************************/
void CalibrationDialog::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.fillRect(rect(), Qt::white);

	QPoint point = data.screenPoints[location[pressCount]];

	// Map to logical coordinates in case the screen is transformed
	QSize screenSize(qt_screen->deviceWidth(), qt_screen->deviceHeight());
	point = qt_screen->mapFromDevice(point, screenSize);

	p.fillRect(point.x() - 6, point.y() - 1, 13, 3, Qt::black);
	p.fillRect(point.x() - 1, point.y() - 6, 3, 13, Qt::black);
}

/************************************************************
描述：触摸屏校准窗口触摸屏/鼠标放开回调函数，记录下5个点的触摸数据，然
	 后进行触摸屏校准计算
参数：event - 鼠标事件
返回：无
************************************************************/
void CalibrationDialog::mouseReleaseEvent(QMouseEvent *event)
{
    // Map from device coordinates in case the screen is transformed
	QSize screenSize(qt_screen->width(), qt_screen->height());
	QPoint p = qt_screen->mapToDevice(event->pos(), screenSize);

	data.devPoints[location[pressCount]] = p;

	if (++pressCount < 5)
		repaint();
	else
		accept();
}

/************************************************************
描述：根据记录下的5个点的触摸数据进行触摸屏校准计算
参数：event - 鼠标事件
返回：无
************************************************************/
void CalibrationDialog::accept()
{
	Q_ASSERT(pressCount == 5);
	QWSServer::mouseHandler()->calibrate(&data);
	QDialog::accept();

	((Application *)qApp)->setLCDAndTimerOn();
}

