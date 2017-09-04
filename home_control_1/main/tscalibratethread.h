#ifndef TSCALIBRATETHREAD_H
#define TSCALIBRATETHREAD_H

#include <QThread>

class TSCalibrateThread : public QThread
{
    Q_OBJECT
public:
    explicit TSCalibrateThread(QObject *parent = 0);

	void run();

	bool m_bRun; //线程是否正在运行

signals:
	void sigOpenCalibration();

public slots:

};

#endif // TSCALIBRATETHREAD_H
