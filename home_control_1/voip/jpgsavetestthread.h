#ifndef JPGSAVETESTTHREAD_H
#define JPGSAVETESTTHREAD_H

#include <QThread>

class JpgSaveTestThread : public QThread
{
    Q_OBJECT
public:
    explicit JpgSaveTestThread(QObject *parent = 0);

	void run();

	int m_bRun;

signals:

public slots:

};

#endif // JPGSAVETESTTHREAD_H
