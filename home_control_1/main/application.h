#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QTimer>

#include <QTcpServer>
#include <QTcpSocket> 

#define SCREEN_SAVER_TIME 45000 //msec

void setGM7113Channel(int channel);

class Application : public QApplication
{
	Q_OBJECT

public:
	explicit Application(int &argc, char **argv);
	~Application();

	bool notify(QObject *, QEvent *);

	int gpio_fd;

	void setGpioDirection(int iGpioNum, bool bIsOut);
	void setGpioValue(int iGpioNum, bool bHigh);
	void setGpioValue(char GPIO_char, int GPIO_idx, int GPIO_value);

	void setLCDOn();
	//void setLCDAndTimerOn();
	void setLCDBacklight(bool bLCDBacklightOn);

    	void setLCDLumi(int lumi);
	void setOnLCD(bool bOn);
	void setUnmuteSpeaker(bool bOn);
	void setUnmuteMic(bool bOn);
	void setOnCamera(bool bOn);	
	void setGPH7(bool bHigh);
	void setGPH8(bool bHigh);
	void setGPE0(bool bHigh);

	bool m_bLCDBacklightOn; //LCD背光是否处于亮的状态
	bool m_bTimerOff; //是否不启动屏保定时器

	QTimer *m_pTimer; //屏保定时器

	QTimer *m_pKeyTimer; //该变量未用到

    	QTimer *m_pTouchEnableTimer;

	void Re_Enable_Screen_Save();

	QTcpServer *m_ptcpserver;    // 开启一个服务端，用于应答当前状态的查询
	QTcpSocket *m_tcpsocket;
public slots:
	void setLCDAndTimerOn();
    	void timerTimeout();
    	void Read_data_process();
	void NewConnect();
};

#endif // APPLICATION_H
