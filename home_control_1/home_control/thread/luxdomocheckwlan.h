#ifndef LUXDOMOCHECKWLAN_H
#define LUXDOMOCHECKWLAN_H

#include <QThread>

class LuxDomoCheckWlan : public QThread
{
    Q_OBJECT
public:
    explicit LuxDomoCheckWlan(QThread *parent = 0);
    void  run();
signals:
public slots:
};

#endif // LUXDOMOCHECKWLAN_H
