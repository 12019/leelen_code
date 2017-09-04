#ifndef LUXDOMOMANAGEHOMEDATA_H
#define LUXDOMOMANAGEHOMEDATA_H

#include <QObject>
#include<QVector>
class LuxDomoManageHomeData : public QObject
{
    Q_OBJECT
public:
    explicit LuxDomoManageHomeData(QObject *parent = 0);
signals:
public slots:
    void    slot_addRoom(QString,QString,QString);
    void    slot_deleteRoom(QString);
    void    slot_changRoomName(QString,QString);
    void    slot_addDevice(QByteArray);
    void    slot_deleteDevice(QByteArray);
    void    slot_changDeviceInforMation(QByteArray);
public:
    QVector<int>    uids;
    void      LoadAllUid();
    int         GetEmptyUid();
};

#endif // LUXDOMOMANAGEHOMEDATA_H
