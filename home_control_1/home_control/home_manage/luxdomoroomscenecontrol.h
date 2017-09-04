#ifndef LUXDOMOROOMSCENECONTROL_H
#define LUXDOMOROOMSCENECONTROL_H

#include<QDeclarativeItem>

class LuxDomoRoomSceneControl : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoRoomSceneControl(QDeclarativeItem *parent = 0);
    ~LuxDomoRoomSceneControl();
    void  componentComplete();
    Q_INVOKABLE  void   init(QString,QString);
    Q_INVOKABLE  void   changSceneData(QString,QString,QString,int index=-1);
    Q_INVOKABLE  void   close();
    Q_INVOKABLE  void   editDevice(QString,QString);
    void      CreateElement(QString);
signals:
    void   sigChangData(QByteArray);
public slots:
    void     slot_addListData(QByteArray);
    void     slot_changListData(QString,QString);
private:
    QString    m_roomObjectName;
    QString    m_sceneObjectName;
    QDeclarativeItem       *editItem;
    QDeclarativeEngine    *editEngine;
    QMap<QString,int>   m_devices;
    int    m_deviceCount;
};
#endif // LUXDOMOROOMSCENECONTROL_H
