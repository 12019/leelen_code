#ifndef LUXDOMOLOADINGDATATOUI_H
#define LUXDOMOLOADINGDATATOUI_H

#include <QThread>
#include<QDomElement>

class LuxDomoLoadingDataToUI : public QThread
{
    Q_OBJECT
public:
    explicit LuxDomoLoadingDataToUI(QThread *parent = 0);
    ~LuxDomoLoadingDataToUI();
    void     run();
    void     LoadingMainInterfaceData();
    void     LoadingRoomManageData();
    void     LoadingHomeManageData();
    void     LoadingRoomControlData();
    void     LoadingHomeScene();
    QDomElement    LoadingDeviceConditionData(QString,QString);
    void     LoadingRoomSceneData();
    void     LoadingHomescene();
    void     LoadingHomeSceneData();
signals:
    void   sigHideLoadingLabel();
    void   sigMainInterfaceData(QString,QString,QString);
    void   sigHomeManageData(QString,QString,QString);
    void   sigRoomManageData(QString,QString,QString,QString,QString);
    void   sigRoomControlData(QString,QString,QString,QString,QString,QString,QString,QString,QString);
    void   sigRoomSceneData(QByteArray);
    void   sigHomeSceneData(QByteArray);
    void   sigHomeScene(QByteArray);
public slots:
public:
    QString    roomObjectName;
    QString    roomUid;
    QString    sceneUid;
    QString    strFile;
    bool         m_isRunning;
};

#endif // LUXDOMOLOADINGDATATOUI_H
