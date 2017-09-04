#ifndef GETDATATIMETHREAD_H
#define GETDATATIMETHREAD_H

#include <QThread>

class GetDataTimeThread : public QThread
{
    Q_OBJECT
public:
    explicit GetDataTimeThread(QObject *parent = 0);
    ~GetDataTimeThread();
    void run();
signals:

public slots:

};

#endif // GETDATATIMETHREAD_H
