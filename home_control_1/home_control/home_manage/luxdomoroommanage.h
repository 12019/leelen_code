#ifndef LUXDOMOROOMMANAGE_H
#define LUXDOMOROOMMANAGE_H

#include<QDeclarativeItem>

class LuxDomoRoomManage : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoRoomManage(QDeclarativeItem *parent = 0);
     ~LuxDomoRoomManage();
    void  componentComplete();

    bool      FindChild(QString);
    void      CreateElement(QString,bool  type=true);
    void      SetRoomInforMation(QString,QString,QString);
    void      setDeviceInforMation(QString);

    Q_INVOKABLE    void  addDevice(QString,QString,QString,QString);

signals:

public slots:
       void     slot_refreshInfraredLearnInterface(QByteArray);
public:
     QDeclarativeItem    *infraredLearnItem;

private:
     QDeclarativeItem   *childItem;
     QString     roomName;
     QString     roomObjectName;
     QString     strImage;
};

#endif // LUXDOMOROOMMANAGE_H
