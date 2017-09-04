#ifndef LINVERSIONUPDATETHREAD_H
#define LINVERSIONUPDATETHREAD_H

#include <QThread>

class LinVersionUpdateThread : public QThread
{
    Q_OBJECT
public:
    explicit LinVersionUpdateThread(QObject *parent = 0);
     void   run();
signals:

public slots:
};

#endif // LINVERSIONUPDATETHREAD_H
