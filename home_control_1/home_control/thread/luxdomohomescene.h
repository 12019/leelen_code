#ifndef LUXDOMOHOMESCENE_H
#define LUXDOMOHOMESCENE_H

#include <QThread>
#include<QDomDocument>

class LuxDomoHomeScene : public QThread
{
    Q_OBJECT
public:
    explicit LuxDomoHomeScene(QThread *parent = 0);
    void    run();
    QStringList    GetSceneImage(QString);
    void    AddHomeScene(QString);
    void    RoomSceneAdd();
    void    InitRoomScene();
    void    InitHomeScene();

     void    AKeyChangRoomScene(QByteArray);
     void    SingleChangRoomScene(QByteArray);
     void    MemoryRoomScene();
     QDomElement   LoadingRoomSceneDeviceData(QString);
     void    AKeyChangHomeScene(QByteArray);
     void    SingleChangHomeScene(QByteArray);
     void    MemoryHomeScene();
    QDomElement   LoadingHomeSceneDeviceData(QString);
     void    CopySceneCondition(QDomElement,QDomElement);

     void     AddRoomScene();         //添加设备或场景
     void     RoomAddDevice();
     void     HomeAddDevice();
     void     DeleteRoomScene();   //删除设备或场景
     void     RoomDeleteDevice();
     void     HomeDeleteDevice();
     void     ChangRoomSceneInforMation();   //编辑设备或场景
     void     RoomChangDeviceInforMation();
     void     HomeChangDeviceInforMation();
     void     RoomSceneAddRoom();       //添加房间
     void     HomeSceneAddRoom();
     void     RoomSceneDeleteRoom();   //删除房间
     void     HomeSceneDeleteRoom();
     void     HomeSceneEditRoom();       //编辑设房间

     void     EnableHomeScene(QString,QString);   //使能全局场景
signals:
     void    sigRoomSceneData(QByteArray);
     void    sigHomeSceneData(QByteArray);
     void    sigHomeScene(QByteArray);
     void    sigRefreshControlInterface(QByteArray);
     void    sigChangRoomSceneData(QString,QString);
     void    sigChangHomeSceneData(QString,QString);
     void    sigChangScene(QString,QString);
public slots:
     void    slot_changRoomSceneData(QByteArray);
     void    slot_changHomeSceneData(QByteArray);

     void    slot_loadingRoomSceneData(QString,QString);
     void    slot_loadingHomeScene();
     void    slot_loadingHomeSceneData(QString);
public:         //场景变量
     QDomElement       m_operatingSceneElement;
     QDomDocument   m_roomScene;
     QString    m_roomUid;
     QDomDocument   m_homeScene;
     QString   m_homeSceneFile;
public:
     QByteArray     m_data;
};

#endif // LUXDOMOHOMESCENE_H
