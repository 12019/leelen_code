#ifndef DIGITALCLOCK_H
#define DIGITALCLOCK_H

#include <QLCDNumber>
#include <QTimer>

class DigitalClock : public QLCDNumber
{
    Q_OBJECT
public:
    explicit DigitalClock(QObject *parent = 0);
	~DigitalClock();

	QTimer *m_pTimer; //定时器每0.5秒触发一次数字时钟更新
	bool m_bShowColon; //是否显示时钟数字间的冒号
	bool m_bMode24; //是否是24小时制
	QString m_Text; //数字时钟当前时间显示文本

	void ClockRun(bool bMode24);
	void ClockStop();

signals:

public slots:
	void showDigTime();
};

#endif // DIGITALCLOCK_H
