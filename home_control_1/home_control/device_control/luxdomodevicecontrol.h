#ifndef LUXDOMODEVICECONTROL_H
#define LUXDOMODEVICECONTROL_H

#include <QObject>

class LuxDomoDeviceControl : public QObject
{
    Q_OBJECT
public:
    explicit LuxDomoDeviceControl(QObject *parent = 0);

signals:

public slots:
};

#endif // LUXDOMODEVICECONTROL_H
