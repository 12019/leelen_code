#ifndef LUXDOMOTHEMEBAR_H
#define LUXDOMOTHEMEBAR_H

#include<QDeclarativeItem>
#include"MyBuffer.h"

class LuxDomoThemebar : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoThemebar(QDeclarativeItem *parent = 0);
    ~LuxDomoThemebar();
    void    componentComplete();
    void    SetPMValue(QString,QString,QString);
    void    SetHumitureValue(QString,QString,QString,QString);
    QStringList    GetSensorValue(QString);
signals:
    void   sigRefreshSensorValue(QString,QString);
public slots:
    void   slot_sensorValue(QString,QString,QString,QString,QString);
    void   slot_checkSensorValue();
    void   slot_deleteRoom(QString);
    void   slot_addRoom(QString,QString,QString);
    void   slot_changRoomName(QString,QString);
private:
    QTimer              *timer;
    QVector<SensorValue>   sensorValues;
};

#endif // LUXDOMOTHEMEBAR_H
