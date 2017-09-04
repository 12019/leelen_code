#include "luxdomoloadingdatatoui.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;
extern  OperateType   operatingType;

LuxDomoLoadingDataToUI::LuxDomoLoadingDataToUI(QThread *parent) : QThread(parent)
{
    roomObjectName=tr("NULL");
    m_isRunning=false;
}

LuxDomoLoadingDataToUI::~LuxDomoLoadingDataToUI()
{

}

void  LuxDomoLoadingDataToUI::run()
{
    m_isRunning=true;
    if(operatingType==LoadMainInterfaceData)
    {
        LoadingMainInterfaceData();
    }
    else  if(operatingType==LoadHomeManageData)
    {
        LoadingHomeManageData();
    }
    else  if(operatingType==LoadRoomManageData)
    {
        LoadingRoomManageData();
    }
    else  if(operatingType==LoadRoomControlData)
    {
        LoadingRoomControlData();
    }
    else  if(operatingType==LoadRoomSceneData)
    {
         LoadingRoomSceneData();
    }
    else  if(operatingType==LoadHomeSceneData)
    {
        LoadingHomeSceneData();
    }
    else  if(operatingType==LoadHomeScene)
    {
        LoadingHomescene();
    }
    m_isRunning=false;
    emit   sigHideLoadingLabel();
}

void   LuxDomoLoadingDataToUI::LoadingMainInterfaceData()
{
    QDomElement   houseElement;
    QDomElement   roomElement;
    QDomElement   layerElement;
    houseElement = g_pMainDlg->m_pHomeControlDlg->m_domDoc.firstChildElement("house");

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            if(false==m_isRunning)
            {
                return;
            }
            QString   uid=roomElement.attribute("uid");
            QString   name=roomElement.attribute("name");
            QString   type=tr("room");
            emit   sigMainInterfaceData(uid,name,type);
            roomElement=roomElement.nextSiblingElement("room");
        }
        layerElement=layerElement.nextSiblingElement("layer");
    }
    LoadingHomeScene();
}

void  LuxDomoLoadingDataToUI::LoadingHomeScene()
{
    emit  sigMainInterfaceData(tr("house-map-user-off.xml"),tr(""),tr("scene"));
    emit  sigMainInterfaceData(tr("house-map-user-on.xml"),tr(""),tr("scene"));

    QDir  *tempDir = new QDir("home_control/");
    QStringList  filter = (QStringList() << "*.xml");
    tempDir->setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(tempDir->entryInfoList());

    for(int i=0;i<fileInfo->size();i++)
    {
        QString fileName = fileInfo->at(i).fileName();     MyMsgLog<<fileInfo->at(i).fileName();
        if(fileName ==tr("house-map-users.xml") || fileName == tr("house-map-default1.xml")||fileName==tr("house-map-default.xml")||fileName==tr("risco-security-default.xml"))
        {    continue;    }

        QFile file(tr("home_control/")+fileName);
        QDomDocument   scene;
        if(!file.exists()) {   return;    }
        if (!file.open(QIODevice::ReadOnly))  {  return; }
        if (!scene.setContent(&file))
        {
              file.close();
              return;
         }
        QDomElement   houseElement;
        QDomElement   infoElement;
        houseElement = scene.firstChildElement("house");
        infoElement = houseElement.firstChildElement("info");
        if(!infoElement.isNull())
        {
            if(infoElement.attribute("actflag")==tr("enable"))
            {
                if(false==m_isRunning)
                {
                    return;
                }
                emit  sigMainInterfaceData(fileName,tr(""),tr("scene"));
            }
        }
     }
    delete   tempDir;
    delete   fileInfo;
}

void   LuxDomoLoadingDataToUI::LoadingHomeManageData()
{
    QDomElement   houseElement;
    QDomElement   roomElement;
    QDomElement   layerElement;
    houseElement = g_pMainDlg->m_pHomeControlDlg->m_domDoc.firstChildElement("house");

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            if(false==m_isRunning)
            {
                return;
            }
            QString   uid=roomElement.attribute("uid");
            QString   name=roomElement.attribute("name");
            QString   type=tr("room");
            emit  sigHomeManageData(uid,name,type);
            roomElement=roomElement.nextSiblingElement("room");
        }
        layerElement=layerElement.nextSiblingElement("layer");
    }
}

void   LuxDomoLoadingDataToUI::LoadingRoomManageData()
{
    QDomElement   houseElement;
    QDomElement   roomElement;
    QDomElement   layerElement;
    QDomElement   deviceElement;
    houseElement = g_pMainDlg->m_pHomeControlDlg->m_domDoc.firstChildElement("house");

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            if(roomElement.attribute("uid")==roomObjectName)
            {
                deviceElement=roomElement.firstChildElement("device");
                while(!deviceElement.isNull())
                {
                    if(false==m_isRunning)
                    {
                        return;
                    }
                    QString   uid=deviceElement.attribute("uid");
                    QString   type=deviceElement.attribute("type");
                    QString   name=deviceElement.attribute("name");
                    QString   id=deviceElement.attribute("id");
                    QString   on_off=deviceElement.attribute("power");
                    sigRoomManageData(uid,type,id,on_off,name);
                    deviceElement=deviceElement.nextSiblingElement("device");
                  }
                  return;
              }
              roomElement=roomElement.nextSiblingElement("room");
        }
        layerElement=layerElement.nextSiblingElement("layer");
    }
}

void     LuxDomoLoadingDataToUI::LoadingRoomControlData()
{
    QDomElement   houseElement;
    QDomElement   roomElement;
    QDomElement   layerElement;
    QDomElement   deviceElement;
    houseElement = g_pMainDlg->m_pHomeControlDlg->m_domDoc.firstChildElement("house");

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            if(roomElement.attribute("uid")==roomObjectName)
            {
                deviceElement=roomElement.firstChildElement("device");
                while(!deviceElement.isNull())
                {
                    if(false==m_isRunning)
                    {
                        return;
                    }
                    QString   uid=deviceElement.attribute("uid");
                    QString   type=deviceElement.attribute("type");
                    QString    name=deviceElement.attribute("name");
                    QString    id=deviceElement.attribute("id");
                    QString   on_off=deviceElement.attribute("power");
                    QString   pageIndex=deviceElement.attribute("pageIndex");
                    QString   deviceIndex=deviceElement.attribute("deviceIndex");
                    QString   folderIndex=deviceElement.attribute("folderIndex");
                    QString   folderName=deviceElement.attribute("folderName");
                    if(pageIndex==tr("")||deviceIndex==tr("")||folderIndex==tr("")||folderName==tr(""))
                    {
                        pageIndex=tr("0");
                        deviceIndex=tr("0");
                        folderIndex=tr("0");
                        folderName=tr("0");
                    }
                    MyMsgLog<<uid<<name<<type<<id<<on_off<<pageIndex<<deviceIndex<<folderIndex<<folderName;
                    emit  sigRoomControlData(uid,name,type,id,on_off,pageIndex,deviceIndex,folderIndex,folderName);
                    deviceElement=deviceElement.nextSiblingElement("device");
                 }
                 return;
             }
             roomElement=roomElement.nextSiblingElement("room");
        }
        layerElement=layerElement.nextSiblingElement("layer");
    }
}

QDomElement    LuxDomoLoadingDataToUI::LoadingDeviceConditionData(QString  roomUid, QString  deviceUid)
{
    QDomElement    data;
    QDomElement    houseElement;
    QDomElement    roomElement;
    QDomElement    layerElement;
    QDomElement    deviceElement;
    houseElement = g_pMainDlg->m_pHomeControlDlg->m_domDoc.firstChildElement("house");

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement=layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            if(roomElement.attribute("uid")==roomUid)
            {
                deviceElement=roomElement.firstChildElement("device");
                while (!deviceElement.isNull())
                {
                    if(deviceElement.attribute("uid")==deviceUid)
                    {
                        return  deviceElement;
                    }
                    deviceElement=deviceElement.nextSiblingElement("device");
                }
            }
            roomElement=roomElement.nextSiblingElement("room");
        }
        layerElement=layerElement.nextSiblingElement("layer");
    }
    return  data;
}

void   LuxDomoLoadingDataToUI::LoadingRoomSceneData()
{
    QFile file(tr("home_control/house-map-users.xml"));
    if(!file.exists())
    {
          return;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
         return;
     }
    if (!g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->m_roomScene.setContent(&file))
    {
         file.close();
         return;
    }
    file.close();
    g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->m_roomUid=roomUid;
    QDomElement   houseElement;
    QDomElement   layerElement;
    QDomElement   roomElement;
    QDomElement   sceneElement;
    QDomElement   deviceElement;
    houseElement=g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->m_roomScene.firstChildElement("house");

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            if(roomElement.attribute("uid")==roomUid)
            {
                sceneElement=roomElement.firstChildElement("scene");
                while(!sceneElement.isNull())
                {
                    if(sceneElement.attribute("uid")==sceneUid)
                    {
                        deviceElement=sceneElement.firstChildElement("device");
                        g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->m_operatingSceneElement=sceneElement;
                        while(!deviceElement.isNull())
                        {
                            if(false==m_isRunning)
                            {
                                return;
                            }
                            QByteArray  data;
                            QDataStream   in(&data,QIODevice::WriteOnly);
                             in.setVersion(QDataStream::Qt_4_7);
                             in<<deviceElement.attribute("uid")<<deviceElement.attribute("name")<<deviceElement.attribute("power")<<deviceElement.attribute("actflag")<<deviceElement.attribute("type");
                             emit  sigRoomSceneData(data);
                             deviceElement=deviceElement.nextSiblingElement("device");
                         }
                         return;
                      }
                      sceneElement=sceneElement.nextSiblingElement("scene");
                  }
             }
             roomElement=roomElement.nextSiblingElement("room");
         }
         layerElement=layerElement.nextSiblingElement("layer");
    }
}

void   LuxDomoLoadingDataToUI::LoadingHomeSceneData()
{
    QFile file(strFile);
    if(!file.exists())
    {
          return;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
         return;
     }
    if (!g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->m_homeScene.setContent(&file))
    {
         file.close();
         return;
    }
    file.close();
    g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->m_homeSceneFile=strFile;
    QDomElement   houseElement;
    QDomElement   layerElement;
    QDomElement   roomElement;
    QDomElement   deviceElement;
    houseElement=g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->m_homeScene.firstChildElement("house");

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            QString   roomName=roomElement.attribute("name");
            deviceElement=roomElement.firstChildElement("device");
             while(!deviceElement.isNull())
             {
                 if(false==m_isRunning)
                 {
                     return;
                 }
                 QByteArray  data;
                 QDataStream   in(&data,QIODevice::WriteOnly);
                  in.setVersion(QDataStream::Qt_4_7);
                  in<<roomElement.attribute("uid")<<deviceElement.attribute("uid")<<(roomName+deviceElement.attribute("name"))<<deviceElement.attribute("power");
                  in<<deviceElement.attribute("actflag")<<deviceElement.attribute("type");
                  emit    sigHomeSceneData(data);
                  deviceElement=deviceElement.nextSiblingElement("device");
             }
             roomElement=roomElement.nextSiblingElement("room");
        }
        layerElement=layerElement.nextSiblingElement("layer");
    }
}

void   LuxDomoLoadingDataToUI::LoadingHomescene()
{
    QDir  *tempDir = new QDir("home_control/");
    QStringList  filter = (QStringList() << "*.xml");
    tempDir->setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(tempDir->entryInfoList());

    for(int i=0;i<fileInfo->size();i++)
    {
        QString fileName = fileInfo->at(i).fileName();     MyMsgLog<<fileInfo->at(i).fileName();
        if(fileName ==tr("house-map-users.xml") || fileName == tr("house-map-default1.xml")||fileName==tr("house-map-default.xml"))
        {    continue;    }

        QFile file(tr("home_control/")+fileName);
        QDomDocument   scene;
        if(!file.exists()) {   return;    }
        if (!file.open(QIODevice::ReadOnly))  {  return; }
        if (!scene.setContent(&file))
        {
              file.close();
              return;
        }
        file.close();
        QDomElement   houseElement;
        QDomElement   infoElement;
        houseElement = scene.firstChildElement("house");
        infoElement = houseElement.firstChildElement("info");
        if(!infoElement.isNull())
        {
            if(false==m_isRunning)
            {
                return;
            }
            QByteArray  data;
            QDataStream   in(&data,QIODevice::WriteOnly);
            in.setVersion(QDataStream::Qt_4_7);
            in<<infoElement.attribute("actflag")<<fileName;  MyMsgLog<<fileName<<infoElement.attribute("actflag");
            emit   sigHomeScene(data);
        }
   }

    delete   tempDir;
    delete  fileInfo;
}

