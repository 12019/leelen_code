#include "luxdomoapplyscene.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   OperateType   operatingType;
LuxDomoApplyScene::LuxDomoApplyScene(QThread *parent) : QThread(parent)
{
    m_roomObjectName=tr("");
    m_sceneObjectName=tr("");
    m_strFile=tr("");
}

void  LuxDomoApplyScene::run()
{
    if(operatingType==RoomScenePlaying)
    {
          UsingRoomScene();
    }
    else   if(operatingType==HomeScenePlaying)
    {
          UsingHomeScene();
    }
}

void  LuxDomoApplyScene::DealSceneData(QDomElement deviceElement, QString  roomUid)
{
    QString   type=deviceElement.attribute("type");
    QString   deviceUid=deviceElement.attribute("uid");
    int      deviceId=deviceElement.attribute("id").toInt();
    int      channel=deviceElement.attribute("channel").toInt();
    if(tr("ac")==type)
    {
        if(deviceId>>8==Central_AC)
        {
            g_pMainDlg->m_pHomeControlDlg->m_pSendCommand->AcPowerControl(deviceElement.attribute("power"),deviceId,roomUid,deviceUid);
            if(deviceElement.attribute("power")==tr("on"))
            {
                g_pMainDlg->m_pHomeControlDlg->m_pSendCommand->AcModeControl(deviceElement.attribute("mode"),deviceId,roomUid,deviceUid);
                g_pMainDlg->m_pHomeControlDlg->m_pSendCommand->AcWindControl(deviceElement.attribute("swing"),deviceId,roomUid,deviceUid);
                g_pMainDlg->m_pHomeControlDlg->m_pSendCommand->AcSpeedControl(deviceElement.attribute("wind"),deviceId,roomUid,deviceUid);
                g_pMainDlg->m_pHomeControlDlg->m_pSendCommand->AcTempControl(deviceElement.attribute("temperature"),deviceId,roomUid,deviceUid);
            }
        }
        else
        {
            if(deviceElement.attribute("power")==tr("on"))
            {
                g_pMainDlg->m_pHomeControlDlg->m_pSendCommand->AcInfraredControl(deviceElement.attribute("power"),
                                                                                 deviceElement.attribute("mode"),
                                                                                 deviceElement.attribute("swing"),
                                                                                 deviceElement.attribute("wind"),
                                                                                 deviceElement.attribute("temperature"),
                                                                                 deviceId,roomUid,deviceUid);
            }
        }
    }
    else   if(tr("tv")==type)
    {
        g_pMainDlg->m_pHomeControlDlg->m_pSendCommand->TvPowerControl(deviceElement.attribute("power"),deviceId,roomUid,deviceUid);
        if(deviceElement.attribute("power")==tr("on"))
        {
            g_pMainDlg->m_pHomeControlDlg->m_pSendCommand->TvModeControl(deviceElement.attribute("mode"),deviceId,roomUid,deviceUid);
        }
    }
    else    if(tr("music")==type)
    {
        g_pMainDlg->m_pHomeControlDlg->m_pSendCommand->MusicPowerControl(deviceElement.attribute("power"),deviceId,channel,roomUid,deviceUid);
        if(deviceElement.attribute("power")==tr("on"))
        {
            g_pMainDlg->m_pHomeControlDlg->m_pSendCommand->MusicModeControl(deviceElement.attribute("source"),deviceId,channel,roomUid,deviceUid);
            g_pMainDlg->m_pHomeControlDlg->m_pSendCommand->MusicPauseControl(tr("off"),deviceId,channel,roomUid,deviceUid);
            g_pMainDlg->m_pHomeControlDlg->m_pSendCommand->MusicVolumeControl(deviceElement.attribute("value"),deviceId,channel,roomUid,deviceUid);
        }
    }
    else    if(tr("light")==type)
    {
        g_pMainDlg->m_pHomeControlDlg->m_pSendCommand->LightPowerControl(deviceElement.attribute("power"),deviceId,roomUid,deviceUid);
    }
    else    if(tr("var")==type)
    {
        g_pMainDlg->m_pHomeControlDlg->m_pSendCommand->LightPowerControl(deviceElement.attribute("power"),deviceId,roomUid,deviceUid);
        if(deviceElement.attribute("power")==tr("on"))
        {
            g_pMainDlg->m_pHomeControlDlg->m_pSendCommand->LightBrightnessControl(deviceElement.attribute("value"),deviceId,roomUid,deviceUid);
        }
    }
    else    if(tr("curtain")==type)
    {
        g_pMainDlg->m_pHomeControlDlg->m_pSendCommand->CurtainPowerControl(deviceElement.attribute("power"),deviceId,roomUid,deviceUid);
    }
}

void   LuxDomoApplyScene::RefreshControlInterface(QDomElement  deviceElement, QString  roomUid)
{
    QByteArray  data;
    QDataStream   in(&data,QIODevice::WriteOnly);
    in.setVersion(QDataStream::Qt_4_7);
    in<<roomUid<<deviceElement.attribute("uid")<<tr("power")<<deviceElement.attribute("power");
    if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL&&deviceElement.attribute("id").toInt()>>8!=Ac_Infrared)
    {
       emit  sigRefreshControlInterface(data);
    }
}

void   LuxDomoApplyScene::AllOnAndOff(QString value)
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
            deviceElement=roomElement.firstChildElement("device");
            while(!deviceElement.isNull())
            {
                deviceElement.setAttribute("power",value);
                deviceElement=deviceElement.nextSiblingElement("device");
            }
            roomElement=roomElement.nextSiblingElement("room");
        }
        layerElement=layerElement.nextSiblingElement("layer");
    }
}

void  LuxDomoApplyScene::UsingHomeScene()
{
    QFile file(m_strFile);
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
    QDomElement   layerElement;
    QDomElement   roomElement;
    QDomElement   deviceElement;
    houseElement = scene.firstChildElement("house");

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            deviceElement=roomElement.firstChildElement("device");
            while(!deviceElement.isNull())
            {
                if(deviceElement.attribute("actflag")==tr("enable"))
                {
                    DealSceneData(deviceElement,roomElement.attribute("uid"));
                    RefreshControlInterface(deviceElement,roomElement.attribute("uid"));
                }
                deviceElement=deviceElement.nextSiblingElement("device");
            }
            roomElement=roomElement.nextSiblingElement("room");
        }
        layerElement=layerElement.nextSiblingElement("layer");
    }
}

void  LuxDomoApplyScene::UsingRoomScene()
{
    QFile file(tr("home_control/house-map-users.xml"));
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
    QDomElement   layerElement;
    QDomElement   roomElement;
    QDomElement   sceneElement;
    QDomElement   deviceElement;
    houseElement = scene.firstChildElement("house");

   layerElement = houseElement.firstChildElement("layer");
   while(!layerElement.isNull())
   {
       roomElement = layerElement.firstChildElement("room");
       while(!roomElement.isNull())
       {
           if(roomElement.attribute("uid")==m_roomObjectName)
           {
               sceneElement=roomElement.firstChildElement("scene");
               while(!sceneElement.isNull())
               {
                   if(sceneElement.attribute("uid")==m_sceneObjectName)
                   {
                       deviceElement=sceneElement.firstChildElement("device");
                       while(!deviceElement.isNull())
                       {
                           if(deviceElement.attribute("actflag")==tr("enable"))
                           {
                               DealSceneData(deviceElement,m_roomObjectName);
                               RefreshControlInterface(deviceElement,m_roomObjectName);
                           }
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
