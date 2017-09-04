#ifndef CALENDAR_H
#define CALENDAR_H

#include <QWidget>
#include <QTimer>

namespace Ui {
    class Calendar;
}

class Calendar : public QWidget
{
    Q_OBJECT

public:
    explicit Calendar(QWidget *parent = 0);
    ~Calendar();

	QTimer *m_pTimer; //该定时器每秒钟触发一次日历显示更新

	int m_yearSolar; //当前年
	int m_monthSolar; //当前月
	int m_daySolar; //当前天

	void LunarCalendar(int year_solar, int month_solar, int day_solar,
		int *p_year_lunar, int *p_month_lunar, int *p_day_lunar);

	int GetJieqi(int year_solar, int month_solar, int day_solar);

	void GetJieri(int year_solar, int month_solar, int day_solar, int year_lunar, int month_lunar, int day_lunar,
	   int weekday, int *p_yangli_idx, int *p_yinli_idx, int *p_week_idx, int *p_fuhuojie_idx);

//private:
    Ui::Calendar *ui;

private:
	unsigned char CalendarCalculateSolarTerm1(unsigned char ucSolarCalendarYear,
		unsigned char ucSolarCalendarMonth);
	unsigned char CalendarCalculateSolarTerm2(unsigned char ucSolarCalendarYear,
		unsigned char ucSolarCalendarMonth);

	int yangli_jieri(int month_solar, int day_solar);
	int yinli_jieri(int year_lunar, int month_lunar, int day_lunar);
	int week_jieri(int year_solar, int month_solar, int day_solar, int weekday);
	int fuhuojie_jieri(int year_solar, int month_solar, int day_solar);

public slots:
	void Generate(bool bForceUpdate = false);
};

#endif // CALENDAR_H
