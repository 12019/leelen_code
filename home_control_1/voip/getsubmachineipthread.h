#ifndef GETSUBMACHINEIPTHREAD_H
#define GETSUBMACHINEIPTHREAD_H

#include <QThread>

//#define AVOID_NONE		0
//#define AVOID_SET		1
//#define AVOID_SENDING	2

class GetSubMachineIPThread : public QThread
{
    Q_OBJECT
public:
    explicit GetSubMachineIPThread(QObject *parent = 0);

	void run();

	bool m_bRun;
	bool m_bAvoid;
	bool m_bBusy;

signals:

public slots:

};

#endif // GETSUBMACHINEIPTHREAD_H
