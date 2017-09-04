#ifndef RISCOALARMSYS_H
#define RISCOALARMSYS_H

#include <QObject>

class RiscoAlarmSys : public QObject
{
    Q_OBJECT
public:
    explicit RiscoAlarmSys(QObject *parent = 0);

signals:

public slots:
};

#endif // RISCOALARMSYS_H
