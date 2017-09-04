#ifndef LUXDOMOROOMCONTROL_H
#define LUXDOMOROOMCONTROL_H

#include<QDeclarativeItem>
#include"luxdomoroomcontrolpage.h"
class LuxDomoRoomControl : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoRoomControl(QDeclarativeItem *parent = 0);
     ~LuxDomoRoomControl();
    void    componentComplete();
    void     init(QString);
    void    Back();
    void    ShowMenu();
    void     CreatePage();
    void     AddHeader();
    bool     FindChild(QString);
    void     ShowPage(QString);
    void     rightMovePage();
    void     leftMovePage();
    void     EditFolderName(QString);
signals:
public slots:
    void     slot_addDevice(QString,QString,QString,QString,QString,QString,QString,QString,QString);
    void     slot_refreshControlInterface(QByteArray);
    void     slot_refreshSensorValue(QString,QString);
private:
      QVector<QString>    roomControlPages;
      QDeclarativeItem     *childItem;
      QString    roomObjectName;
public:
      QDeclarativeItem    *controlDeviceItem;
      QVector<QDeclarativeItem*>   deviceControls;
      QString     runningPageName;
      QDeclarativeItem     *folderItem;
};

#endif // LUXDOMOROOMCONTROL_H
