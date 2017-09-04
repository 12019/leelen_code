#ifndef CLOCK_H
#define CLOCK_H

#include <QWidget>
#include <QTimer>

namespace Ui {
    class Clock;
}

class Clock : public QWidget
{
    Q_OBJECT

public:
    explicit Clock(QWidget *parent = 0);
    ~Clock();

	QTimer *m_pTimer; //定时器每秒钟触发一次模拟时钟显示更新
	QPixmap *m_pPixmap; //模拟时钟表盘背景图片

	QString m_picPath; //模拟时钟表盘背景图片路径

	QPoint m_pntHour[4]; //时针图案4个端点的坐标
	QPoint m_pntMin[4]; //分针图案4个端点的坐标
	QPoint m_pntSec[4]; //秒针图案4个端点的坐标

    QTimer *m_pUpdatetimetimer;

	void ClockRun();
	void ClockStop();

	int setBackgroundImage(QString pic_path);

protected:
	void paintEvent(QPaintEvent *event);

private:
    Ui::Clock *ui;

private slots:
	void callBack();
    void slotUpdateTimer();
};

#endif // CLOCK_H
