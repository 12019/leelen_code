#include "luxdomohomescene.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   OperateType   operatingType;
LuxDomoHomeScene::LuxDomoHomeScene(QThread *parent) : QThread(parent)
{

}

void   LuxDomoHomeScene::run()
{
    if(operatingType==AddDevice)
    {
        AddRoomScene();
    }
    else  if(operatingType==DeleteDevice)
    {
        DeleteRoomScene();
    }
    else  if(operatingType==EditDevice)
    {
        ChangRoomSceneInforMation();
    }
    else  if(operatingType==AddRoom)
    {
        RoomSceneAddRoom();
        HomeSceneAddRoom();
    }
    else  if(operatingType==DeleteRoom)
    {
        RoomSceneDeleteRoom();
        HomeSceneDeleteRoom();
    }
   else   if(operatingType==EditRoom)
   {
        HomeSceneEditRoom();
   }
}

QStringList    LuxDomoHomeScene::GetSceneImage(QString  strFile)
{
    QStringList   data;
    data.clear();
    if(tr("home_control/house-map-user-2.xml")==strFile)
    {
         data<<tr("zaijia.png")<<tr("在家模式");
    }
    else  if(tr("home_control/house-map-user-6.xml")==strFile)
    {
         data<<tr("waichu.png")<<tr("外出模式");
    }
    else  if(tr("home_control/house-map-user-3.xml")==strFile)
    {
         data<<tr("qichuang.png")<<tr("起床模式");
    }
    else  if(tr("home_control/house-map-user-1.xml")==strFile)
    {
         data<<tr("shuimian.png")<<tr("睡眠模式");
    }
    else   if(tr("home_control/house-map-user-4.xml")==strFile)
    {
         data<<tr("yule.png")<<tr("娱乐模式");
    }
    else   if(tr("home_control/house-map-user-7.xml")==strFile)
    {
         data<<tr("huike.png")<<tr("会客模式");
    }
    else   if(tr("home_control/house-map-user-5.xml")==strFile)
    {
        data<<tr("yingyin.png")<<tr("影音模式");
    }
    else   if(tr("home_control/house-map-user-8.xml")==strFile)
    {
        data<<tr("jiucan.png")<<tr("就餐模式");
    }
    return  data;
}

void   LuxDomoHomeScene::InitRoomScene()
{
    QFile file("home_control/house-map-users.xml");
    if(file.exists())
    {
           return;
    }
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QDomDocument document;
    QString strHeader( "version=\"1.0\" encoding=\"UTF-8\"" );      //创建xml的头
    document.appendChild(document.createProcessingInstruction("xml",strHeader));

    QDomElement root_house = document.createElement("house");
    document.appendChild(root_house);
    QDomElement info = document.createElement("info");
    info.setAttribute("name",tr("房间场景"));
    info.setAttribute("id","");
    root_house.appendChild(info);

    QDomElement  houseElement;
    QDomElement  layerElement;
    QDomElement  roomElement;
    houseElement = g_pMainDlg->m_pHomeControlDlg->m_domDoc.firstChildElement("house");

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        QDomElement layer = document.createElement("layer");
        layer.setAttribute("uid",layerElement.attribute("uid"));
        layer.setAttribute("name",layerElement.attribute("name"));
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            QDomElement room  =  document.createElement("room");
            room.setAttribute("uid",roomElement.attribute("uid"));
            layer.appendChild(room);
            roomElement=roomElement.nextSiblingElement("room");
        }
        root_house.appendChild(layer);
        layerElement=layerElement.nextSiblingElement("layer");
    }

    QTextStream stream( &file );
    document.save( stream, 4 );
    file.close();
    RoomSceneAdd();
}

void   LuxDomoHomeScene::InitHomeScene()
{
    QString  strFile;
    for(int i=1;i<=8;i++)
    {
        strFile=tr("home_control/house-map-user-")+QString::number(i)+tr(".xml");
        QFile    file(strFile);
        if(file.exists())
        {
            continue;
        }
        MyMsgLog<<strFile;
        AddHomeScene(strFile);
    }
}

void   LuxDomoHomeScene::AddRoomScene()
{
    QString  roomObjectName,sceneObjectName,type,id,name;
    QDataStream  out(&m_data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>roomObjectName>>sceneObjectName>>type>>id>>name;
    //MyMsgLog<<roomObjectName<<sceneObjectName<<type<<id<<name;
    if(type!=tr("scene"))
    {
        RoomAddDevice();
        HomeAddDevice();
        return;
    }

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

    QDomElement sceneElement = scene.createElement("scene");
    sceneElement.setAttribute("uid",sceneObjectName);
    sceneElement.setAttribute("name",name);

    QDomElement   houseElement=g_pMainDlg->m_pHomeControlDlg->m_domDoc.firstChildElement("house");
    QDomElement   layerElement;
    QDomElement   roomElement;
    QDomElement   deviceElement;

    layerElement=houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement=layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            if(roomElement.attribute("uid")==roomObjectName)
            {
                deviceElement=roomElement.firstChildElement("device");
                while(!deviceElement.isNull())
                {
                    if(deviceElement.attribute("type")==tr("scene"))
                    {
                        deviceElement=deviceElement.nextSiblingElement("device");
                        continue;
                    }
                    QDomElement device = scene.createElement("device");
                    device.setAttribute("id",deviceElement.attribute("id"));
                    device.setAttribute("type",deviceElement.attribute("type"));
                    device.setAttribute("subtype",deviceElement.attribute("subtype"));
                    device.setAttribute("name",deviceElement.attribute("name"));
                    device.setAttribute("uid",deviceElement.attribute("uid"));
                    if(deviceElement.attribute("type")==tr("tv"))
                    {
                        device.setAttribute("mode","tv");
                    }
                    else
                    {
                        device.setAttribute("mode","auto");
                    }
                    device.setAttribute("power","off");
                    if(tr("ac")==deviceElement.attribute("type"))
                    {
                        device.setAttribute("swing","on");
                    }
                    else
                    {
                        device.setAttribute("swing","off");
                    }
                    device.setAttribute("temperature",tr("20"));
                    device.setAttribute("wind",tr("auto"));
                    device.setAttribute("value",tr("20"));
                    device.setAttribute("open",tr("on"));
                    device.setAttribute("actflag","enable");
                    device.setAttribute("play",tr("off"));
                    device.setAttribute("source",tr("mp3"));
                    device.setAttribute("mute",tr("on"));
                    device.setAttribute("channel",tr("0"));
                    device.setAttribute("musicname",tr(""));
                    device.setAttribute("channelSize",tr("4"));
                    sceneElement.appendChild(device);
                    deviceElement=deviceElement.nextSiblingElement("device");
                }
                break;
            }
            roomElement=roomElement.nextSiblingElement("room");
        }
        layerElement=layerElement.nextSiblingElement("layer");
   }

    QDomElement   house=scene.firstChildElement("house");
    QDomElement   layer;
    QDomElement   room;

    layer=house.firstChildElement("layer");
    while(!layer.isNull())
    {
        room=layer.firstChildElement("room");
        while(!room.isNull())
        {
            if(room.attribute("uid")==roomObjectName)
            {
                room.appendChild(sceneElement);
                QFile file1(tr("home_control/house-map-users.xml"));
                if (!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))  return;
                QTextStream stream(&file1);
                scene.save(stream, 4);
                file1.close();
                sync();
                return;
             }
             room=room.nextSiblingElement("room");
         }
         layer=layer.nextSiblingElement("layer");
    }
}

void    LuxDomoHomeScene::DeleteRoomScene()
{
    QString  roomObjectName,sceneObjectName,type;
    QDataStream  out(&m_data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>roomObjectName>>sceneObjectName>>type;
    MyMsgLog<<roomObjectName<<sceneObjectName<<type;

    if(type!=tr("scene"))
    {
          RoomDeleteDevice();
          HomeDeleteDevice();
          return;
    }

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

    QDomElement   house=scene.firstChildElement("house");
    QDomElement   layer;
    QDomElement   room;
    QDomElement   sceneElement;

    layer=house.firstChildElement("layer");
    while(!layer.isNull())
    {
        room=layer.firstChildElement("room");
        while(!room.isNull())
        {
            if(room.attribute("uid")==roomObjectName)
            {
                sceneElement=room.firstChildElement("scene");
                while(!scene.isNull())
                {
                    if(sceneElement.attribute("uid")==sceneObjectName)
                    {
                        room.removeChild(sceneElement);
                        QFile file1(tr("home_control/house-map-users.xml"));
                        if (!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))  return;
                        QTextStream stream(&file1);
                        scene.save(stream, 4);
                        file1.close();
                        sync();
                        return;
                     }
                     sceneElement=sceneElement.nextSiblingElement("scene");
                 }
             }
             room=room.nextSiblingElement("room");
         }
         layer=layer.nextSiblingElement("layer");
     }
}

void   LuxDomoHomeScene::ChangRoomSceneInforMation()
{
    QString  roomObjectName,sceneObjectName,type,name;
    QDataStream  out(&m_data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>roomObjectName>>sceneObjectName>>type>>name;

    if(type!=tr("scene"))
    {
        RoomChangDeviceInforMation();
        HomeChangDeviceInforMation();
        return;
    }

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

    QDomElement   house=scene.firstChildElement("house");
    QDomElement   layer;
    QDomElement   room;
    QDomElement   sceneElement;

    layer=house.firstChildElement("layer");
    while(!layer.isNull())
    {
        room=layer.firstChildElement("room");
        while(!room.isNull())
        {
            if(room.attribute("uid")==roomObjectName)
            {
                sceneElement=room.firstChildElement("scene");
                while(!sceneElement.isNull())
                {
                    if(sceneElement.attribute("uid")==sceneObjectName)
                    {
                        sceneElement.setAttribute("name",name);
                        break;
                    }
                    sceneElement=sceneElement.nextSiblingElement("scene");
                 }
                 QFile file1(tr("home_control/house-map-users.xml"));
                 if (!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))  return;
                 QTextStream stream(&file1);
                 scene.save(stream, 4);
                 file1.close();
                 sync();
                 return;
             }
             room=room.nextSiblingElement("room");
         }
        layer=layer.nextSiblingElement("layer");
    }
}

void   LuxDomoHomeScene::RoomAddDevice()
{
    QString  roomObjectName,deviceObjectName,type,id,name;
    QDataStream  out(&m_data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>roomObjectName>>deviceObjectName>>type>>id>>name;
    //MyMsgLog<<roomObjectName<<deviceObjectName<<type<<id<<name;
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
    houseElement = scene.firstChildElement("house");

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            if(roomElement.attribute("uid")==roomObjectName)
            {
                sceneElement=roomElement.firstChildElement("scene");
                while(!sceneElement.isNull())
                {
                    QDomElement deviceElement = scene.createElement("device");
                    deviceElement.setAttribute("id",id);
                    deviceElement.setAttribute("type",type);
                    if(type==tr("var"))
                    {
                        deviceElement.setAttribute("subtype","var");
                    }
                    else
                    {
                        deviceElement.setAttribute("subtype","non-var");
                    }
                    deviceElement.setAttribute("name",name);
                    deviceElement.setAttribute("power","off");
                    if(type==tr("tv"))
                    {
                        deviceElement.setAttribute("mode","tv");
                    }
                    else
                    {
                        deviceElement.setAttribute("mode","auto");
                    }
                    deviceElement.setAttribute("temperature",tr("20"));
                    deviceElement.setAttribute("wind","auto");
                    if(tr("ac")==type)
                    {
                          deviceElement.setAttribute("swing","on");
                    }
                    else
                    {
                        deviceElement.setAttribute("swing","off");
                    }
                    deviceElement.setAttribute("value",tr("20"));
                    deviceElement.setAttribute("open","off");
                    deviceElement.setAttribute("uid",deviceObjectName);
                    deviceElement.setAttribute("actflag","enable");
                    deviceElement.setAttribute("play",tr("off"));
                    deviceElement.setAttribute("source",tr("mp3"));
                    deviceElement.setAttribute("mute",tr("on"));
                    deviceElement.setAttribute("channel",tr("0"));
                    deviceElement.setAttribute("musicname",tr(""));
                    deviceElement.setAttribute("channelSize",tr("4"));
                    sceneElement.appendChild(deviceElement);
                    sceneElement=sceneElement.nextSiblingElement("scene");
                 }
                QFile file1(tr("home_control/house-map-users.xml"));
                if (!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))  return;
                QTextStream stream(&file1);
                scene.save(stream, 4);
                file1.close();
                sync();
                return;
            }
            roomElement=roomElement.nextSiblingElement("room");
        }
        layerElement=layerElement.nextSiblingElement("layer");
   }
}

void    LuxDomoHomeScene::RoomDeleteDevice()
{
    QString  roomObjectName,deviceObjectName,type;
    QDataStream  out(&m_data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>roomObjectName>>deviceObjectName>>type;

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
    QDomElement    sceneElement;
    houseElement = scene.firstChildElement("house");

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            if(roomElement.attribute("uid")==roomObjectName)
            {
                sceneElement=roomElement.firstChildElement("scene");
                while(!sceneElement.isNull())
                {
                    QDomElement  deviceElement=sceneElement.firstChildElement("device");
                    while(!deviceElement.isNull())
                    {
                        if(deviceElement.attribute("uid")==deviceObjectName)
                        {
                            sceneElement.removeChild(deviceElement);
                            break;
                        }
                        deviceElement=deviceElement.nextSiblingElement("device");
                    }
                    sceneElement=sceneElement.nextSiblingElement("scene");
                }
                QFile file1(tr("home_control/house-map-users.xml"));
                if (!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))  return;
                QTextStream stream(&file1);
                scene.save(stream, 4);
                file1.close();
                sync();
                return;
             }
             roomElement=roomElement.nextSiblingElement("room");
         }
         layerElement=layerElement.nextSiblingElement("layer");
    }
}

void    LuxDomoHomeScene::RoomChangDeviceInforMation()
{
    QString  roomObjectName,deviceObjectName,type,name;
    QDataStream  out(&m_data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>roomObjectName>>deviceObjectName>>type>>name;
    //MyMsgLog<<roomObjectName<<deviceObjectName<<type;
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
    QDomElement    sceneElement;
    houseElement = scene.firstChildElement("house");
    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            if(roomElement.attribute("uid")==roomObjectName)
            {
                sceneElement=roomElement.firstChildElement("scene");
                while(!sceneElement.isNull())
                {
                    QDomElement   deviceElement=sceneElement.firstChildElement("device");
                    while(!deviceElement.isNull())
                    {
                        if(deviceElement.attribute("uid")==deviceObjectName)
                        {
                            deviceElement.setAttribute("name",name);
                            break;
                        }
                        deviceElement=deviceElement.nextSiblingElement("device");
                    }
                    sceneElement=sceneElement.nextSiblingElement("scene");
                 }
                 QFile file1(tr("home_control/house-map-users.xml"));
                 if (!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))  return;
                 QTextStream stream(&file1);
                 scene.save(stream, 4);
                 file1.close();
                 sync();
                 return;
             }
             roomElement=roomElement.nextSiblingElement("room");
        }
        layerElement=layerElement.nextSiblingElement("layer");
    }
}

void   LuxDomoHomeScene::slot_loadingRoomSceneData(QString roomUid,QString sceneUid)
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
    if (!m_roomScene.setContent(&file))
    {
         file.close();
         return;
    }
    file.close();
    m_roomUid=roomUid;
    QDomElement   houseElement;
    QDomElement   layerElement;
    QDomElement   roomElement;
    QDomElement   sceneElement;
    QDomElement   deviceElement;
    houseElement = m_roomScene.firstChildElement("house");

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
                        m_operatingSceneElement=sceneElement;
                        while(!deviceElement.isNull())
                        {
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

void   LuxDomoHomeScene::slot_changRoomSceneData(QByteArray data)
{
    QString   type;
    QDataStream  out(&data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>type;

    if(tr("AKeyOperate")==type)
    {
        AKeyChangRoomScene(data);
    }
    else   if(tr("SingleOperate")==type)
    {
        SingleChangRoomScene(data);
    }
    else   if(tr("Memory")==type)
    {
        MemoryRoomScene();
    }
    else   if(tr("Save")==type)
    {
        QFile sceneMap(tr("home_control/house-map-users.xml"));
        if (!sceneMap.open(QIODevice::WriteOnly | QIODevice::Truncate))  return;
        QTextStream stream(&sceneMap);
        m_roomScene.save(stream, 4);
        sceneMap.close();
        sync();
    }
}

void  LuxDomoHomeScene::MemoryRoomScene()
{
    if(m_operatingSceneElement.isNull())
    {
        return;
    }
    QDomElement   deviceElement=m_operatingSceneElement.firstChildElement("device");
    while(!deviceElement.isNull())
    {
        QString   deviceUid=deviceElement.attribute("uid");
        QDomElement  device=g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->LoadingDeviceConditionData(m_roomUid,deviceUid);
        CopySceneCondition(deviceElement,device);
        emit  sigChangRoomSceneData(deviceUid,deviceElement.attribute("power"));
        deviceElement=deviceElement.nextSiblingElement("device");
    }
}

void   LuxDomoHomeScene::AKeyChangRoomScene(QByteArray  data)
{
    QString   type,attributeName,attributeValue;
    QDataStream  out(&data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>type>>attributeName>>attributeValue;
    if(m_operatingSceneElement.isNull())
    {
        return;
    }
    QDomElement   deviceElement=m_operatingSceneElement.firstChildElement("device");
    while(!deviceElement.isNull())
    {
        deviceElement.setAttribute(attributeName,attributeValue);
        deviceElement=deviceElement.nextSiblingElement("device");
    }
}

void    LuxDomoHomeScene::SingleChangRoomScene(QByteArray data)
{
    QString   type,uid,attributeName,attributeValue;
    QDataStream  out(&data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>type>>uid>>attributeName>>attributeValue;
    if(m_operatingSceneElement.isNull())
    {
        return;
    }
    QDomElement   deviceElement=m_operatingSceneElement.firstChildElement("device");
    while(!deviceElement.isNull())
    {
         if(deviceElement.attribute("uid")==uid)
         {
             deviceElement.setAttribute(attributeName,attributeValue);
             return;
         }
         deviceElement=deviceElement.nextSiblingElement("device");
    }
}

QDomElement   LuxDomoHomeScene::LoadingRoomSceneDeviceData(QString deviceUid)
{
    QDomElement   element;
    element.clear();
    if(m_operatingSceneElement.isNull())
    {
        return  element;
    }

    QDomElement   deviceElement=m_operatingSceneElement.firstChildElement("device");
    while(!deviceElement.isNull())
     {
          if(deviceElement.attribute("uid")==deviceUid)
          {
              return    deviceElement;
          }
          deviceElement=deviceElement.nextSiblingElement("device");
    }
    return  element;
}

void   LuxDomoHomeScene::RoomSceneAdd()
{
    QDomElement   houseElement=g_pMainDlg->m_pHomeControlDlg->m_domDoc.firstChildElement("house");
    QDomElement   roomElement;
    QDomElement   deviceElement;

    QDomElement   layerElement=houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement=layerElement.firstChildElement("room");
        while (!roomElement.isNull())
        {
            deviceElement=roomElement.firstChildElement("device");
            while(!deviceElement.isNull())
            {
                if(deviceElement.attribute("type")==tr("scene"))
                {
                    QByteArray  data;
                    QDataStream   in(&data,QIODevice::WriteOnly);
                    in.setVersion(QDataStream::Qt_4_7);
                    in<<roomElement.attribute("uid")<<deviceElement.attribute("uid")<<tr("scene");
                    in<<deviceElement.attribute("id")<<deviceElement.attribute("name");
                    m_data=data;
                    AddRoomScene();
                }
                deviceElement=deviceElement.nextSiblingElement("device");
            }
            roomElement=roomElement.nextSiblingElement("room");
        }
        layerElement=layerElement.nextSiblingElement("layer");
    }
}

void    LuxDomoHomeScene::AddHomeScene(QString  strFile)
{
    QStringList   data=GetSceneImage(strFile);
    if(data.size()!=2)
    {
        return;
    }
    QFile file(strFile);
    if(strFile==tr(""))
    {
        return;
    }
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QDomDocument document;
    QString strHeader( "version=\"1.0\" encoding=\"UTF-8\"" );      //创建xml的头
    document.appendChild(document.createProcessingInstruction("xml",strHeader));
    QDomElement root_house = document.createElement("house");
    document.appendChild(root_house);
    QDomElement info = document.createElement("info");
    info.setAttribute("image",data.at(0));
    info.setAttribute("name",data.at(1));
    info.setAttribute("id","");
    info.setAttribute("actflag",tr("enable"));
    root_house.appendChild(info);

    QDomElement  houseElement;
    QDomElement  layerElement;
    QDomElement  roomElement;
    QDomElement  deviceElement;
    houseElement = g_pMainDlg->m_pHomeControlDlg->m_domDoc.firstChildElement("house");

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        QDomElement layer = document.createElement("layer");
        layer.setAttribute("uid",layerElement.attribute("uid"));
        layer.setAttribute("name",layerElement.attribute("name"));
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            QDomElement room=document.createElement("room");
            room.setAttribute("uid",roomElement.attribute("uid"));
            room.setAttribute("name",roomElement.attribute("name"));
            deviceElement=roomElement.firstChildElement("device");
            while(!deviceElement.isNull())
            {
                QDomElement device=document.createElement("device");
                device.setAttribute("id",deviceElement.attribute("id"));
                device.setAttribute("type",deviceElement.attribute("type"));
                device.setAttribute("subtype",deviceElement.attribute("subtype"));
                device.setAttribute("name",deviceElement.attribute("name"));
                device.setAttribute("uid",deviceElement.attribute("uid"));
                if(deviceElement.attribute("type")==tr("tv"))
                {
                      device.setAttribute("mode","tv");
                }
                else
                {
                    device.setAttribute("mode","auto");
                }
                device.setAttribute("power","off");
                if(tr("ac")==deviceElement.attribute("type"))
                {
                     device.setAttribute("swing","off");
                }
                else
                {
                    device.setAttribute("swing","off");
                }
                device.setAttribute("temperature",tr("20"));
                device.setAttribute("wind",tr("auto"));
                device.setAttribute("value",tr("20"));
                device.setAttribute("open",tr("on"));
                device.setAttribute("actflag","enable");
                device.setAttribute("play",tr("off"));
                device.setAttribute("source",tr("mp3"));
                device.setAttribute("mute",tr("on"));
                device.setAttribute("channel",tr("0"));
                device.setAttribute("musicname",tr(""));
                device.setAttribute("channelSize",tr("4"));
                room.appendChild(device);
                deviceElement=deviceElement.nextSiblingElement("device");
            }
            layer.appendChild(room);
            roomElement=roomElement.nextSiblingElement("room");
         }
         root_house.appendChild(layer);
         layerElement=layerElement.nextSiblingElement("layer");
    }

    QTextStream stream( &file );
    document.save( stream, 4 );
    file.close();
}

void    LuxDomoHomeScene::slot_loadingHomeSceneData(QString  strFile)
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
    if (!m_homeScene.setContent(&file))
    {
         file.close();
         return;
    }
    file.close();
    m_homeSceneFile=strFile;
    QDomElement   houseElement;
    QDomElement   layerElement;
    QDomElement   roomElement;
    QDomElement   deviceElement;
    houseElement = m_homeScene.firstChildElement("house");

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

void   LuxDomoHomeScene::slot_changHomeSceneData(QByteArray  data)
{
    QString   type;
    QDataStream  out(&data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>type;

    if(tr("AKeyOperate")==type)
    {
        AKeyChangHomeScene(data);
     }
    else   if(tr("SingleOperate")==type)
    {
        SingleChangHomeScene(data);
    }
    else   if(tr("Memory")==type)
    {
        MemoryHomeScene();
    }
   else   if(tr("Save")==type)
    {
        QFile sceneMap(m_homeSceneFile);
        if (!sceneMap.open(QIODevice::WriteOnly | QIODevice::Truncate))  return;
        QTextStream stream(&sceneMap);
        m_homeScene.save(stream, 4);
        sceneMap.close();
        sync();
    }
}

void  LuxDomoHomeScene::MemoryHomeScene()
{
    QDomElement   houseElement;
    QDomElement   layerElement;
    QDomElement   roomElement;
    QDomElement   deviceElement;
    houseElement = m_homeScene.firstChildElement("house");

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            deviceElement=roomElement.firstChildElement("device");
            while(!deviceElement.isNull())
            {
                QString    deviceUid=deviceElement.attribute("uid");
                QString    roomUid=roomElement.attribute("uid");
                QDomElement  device=g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->LoadingDeviceConditionData(roomUid,deviceUid);
                CopySceneCondition(deviceElement,device);
                emit   sigChangHomeSceneData(deviceUid,deviceElement.attribute("power"));
                deviceElement=deviceElement.nextSiblingElement("device");
            }
            roomElement=roomElement.nextSiblingElement("room");
         }
         layerElement=layerElement.nextSiblingElement("layer");
    }
}

void   LuxDomoHomeScene::AKeyChangHomeScene(QByteArray  data)
{
    QString   type,attributeName,attributeValue;
    QDataStream  out(&data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>type>>attributeName>>attributeValue;

    QDomElement   houseElement;
    QDomElement   layerElement;
    QDomElement   roomElement;
    QDomElement   deviceElement;
     houseElement = m_homeScene.firstChildElement("house");

     layerElement = houseElement.firstChildElement("layer");
     while(!layerElement.isNull())
     {
         roomElement = layerElement.firstChildElement("room");
         while(!roomElement.isNull())
         {
             deviceElement=roomElement.firstChildElement("device");
              while(!deviceElement.isNull())
              {
                  deviceElement.setAttribute(attributeName,attributeValue);
                  deviceElement=deviceElement.nextSiblingElement("device");
              }
              roomElement=roomElement.nextSiblingElement("room");
          }
          layerElement=layerElement.nextSiblingElement("layer");
     }
}

void  LuxDomoHomeScene::SingleChangHomeScene(QByteArray  data)
{
    QString   type,roomUid,deviceUid,attributeName,attributeValue;
    QDataStream  out(&data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>type>>roomUid>>deviceUid>>attributeName>>attributeValue;

    QDomElement   houseElement;
    QDomElement   layerElement;
    QDomElement   roomElement;
    QDomElement   deviceElement;
     houseElement = m_homeScene.firstChildElement("house");

     layerElement = houseElement.firstChildElement("layer");
     while(!layerElement.isNull())
     {
         roomElement = layerElement.firstChildElement("room");
         while(!roomElement.isNull())
         {
             if(roomElement.attribute("uid")==roomUid)
             {
                 deviceElement=roomElement.firstChildElement("device");
                 while(!deviceElement.isNull())
                 {
                     if(deviceElement.attribute("uid")==deviceUid)
                     {
                         deviceElement.setAttribute(attributeName,attributeValue);
                         return;
                     }
                     deviceElement=deviceElement.nextSiblingElement("device");
                  }
              }
              roomElement=roomElement.nextSiblingElement("room");
         }
         layerElement=layerElement.nextSiblingElement("layer");
     }
}

QDomElement     LuxDomoHomeScene::LoadingHomeSceneDeviceData(QString  deviceUid)
{
    QDomElement   element;
    element.clear();
    QDomElement   houseElement;
    QDomElement   layerElement;
    QDomElement   roomElement;
    QDomElement   deviceElement;
    houseElement = m_homeScene.firstChildElement("house");

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            deviceElement=roomElement.firstChildElement("device");
            while(!deviceElement.isNull())
            {
                if(deviceElement.attribute("uid")==deviceUid)
                {
                    return  deviceElement;
                }
                deviceElement=deviceElement.nextSiblingElement("device");
            }
            roomElement=roomElement.nextSiblingElement("room");
        }
        layerElement=layerElement.nextSiblingElement("layer");
    }
    return   element;
}


void    LuxDomoHomeScene::slot_loadingHomeScene()
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

void  LuxDomoHomeScene::EnableHomeScene(QString  strFile,QString  value)
{
    QFile file(XmlFilePath+strFile);
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
        infoElement.setAttribute("actflag",value);
        emit   sigChangScene(strFile,value);
        QFile file1(XmlFilePath+strFile);
        if (!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))  return;
        QTextStream stream(&file1);
        scene.save(stream, 4);
        file1.close();
        sync();
    }
}

void    LuxDomoHomeScene::RoomSceneAddRoom()
{
    QString  objectName,name;
    QDataStream  out(&m_data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>objectName>>name;
    //MyMsgLog<<objectName<<name;
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

    QDomElement   house=scene.firstChildElement("house");
    QDomElement   layer=house.firstChildElement("layer");
    QDomElement   room=scene.createElement("room");
    room.setAttribute("uid",objectName);
    if(!layer.isNull())
    {
          layer.appendChild(room);
    }

    QFile file1(tr("home_control/house-map-users.xml"));
    if (!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))  return;
    QTextStream stream(&file1);
    scene.save(stream, 4);
    file1.close();
    sync();
}

void    LuxDomoHomeScene::RoomSceneDeleteRoom()
{
    QString  objectName;
    QDataStream  out(&m_data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>objectName;
    //MyMsgLog<<objectName;
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

    QDomElement   house=scene.firstChildElement("house");
    QDomElement   layer;
    QDomElement   room;

    layer=house.firstChildElement("layer");
    while(!layer.isNull())
    {
        room=layer.firstChildElement("room");
        while(!room.isNull())
        {
            if(room.attribute("uid")==objectName)
            {
                layer.removeChild(room);
                QFile file1(tr("home_control/house-map-users.xml"));
                if (!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))  return;
                QTextStream stream(&file1);
                scene.save(stream, 4);
                file1.close();
                sync();
                return;
            }
            room=room.nextSiblingElement("room");
         }
         layer=layer.nextSiblingElement("layer");
    }
}

void    LuxDomoHomeScene::HomeSceneAddRoom()
{
    QString  objectName,name;
    QDataStream  out(&m_data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>objectName>>name;
    //MyMsgLog<<objectName<<name;
    QDir  *tempDir = new QDir("home_control/");
    QStringList  filter = (QStringList() << "*.xml");
    tempDir->setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(tempDir->entryInfoList());

    for(int i=0;i<fileInfo->size();i++)
    {
        QString fileName = fileInfo->at(i).fileName();
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
         QDomElement   layerElement;
         houseElement = scene.firstChildElement("house");
         layerElement = houseElement.firstChildElement("layer");
         if(!layerElement.isNull())
         {
                QDomElement   roomElement=scene.createElement("room");
                roomElement.setAttribute("uid",objectName);
                roomElement.setAttribute("name",name);
                layerElement.appendChild(roomElement);
                QFile  file1(tr("home_control/")+fileName);
                if (!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))  return;
                QTextStream stream(&file1);
                scene.save(stream, 4);
                file1.close();
                sync();
         }
    }
     delete   tempDir;
     delete  fileInfo;
}

void    LuxDomoHomeScene::HomeSceneDeleteRoom()
{
    QString  objectName;
    QDataStream  out(&m_data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>objectName;
    //MyMsgLog<<objectName;
    QDir  *tempDir = new QDir("home_control/");
    QStringList  filter = (QStringList() << "*.xml");
    tempDir->setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(tempDir->entryInfoList());

    for(int i=0;i<fileInfo->size();i++)
    {
        QString fileName = fileInfo->at(i).fileName();
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
        QDomElement   layerElement;
        QDomElement   roomElement;
        houseElement = scene.firstChildElement("house");

        layerElement = houseElement.firstChildElement("layer");
        while(!layerElement.isNull())
        {
            roomElement=layerElement.firstChildElement("room");
            while(!roomElement.isNull())
            {
                if(roomElement.attribute("uid")==objectName)
                {
                    layerElement.removeChild(roomElement);
                    QFile  file1(tr("home_control/")+fileName);
                    if (!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))  return;
                    QTextStream stream(&file1);
                    scene.save(stream, 4);
                    file1.close();
                    sync();
                    break;
                }
                roomElement=roomElement.nextSiblingElement("room");
            }
            layerElement=layerElement.nextSiblingElement("layer");
        }
    }
    delete   tempDir;
    delete  fileInfo;
}

void     LuxDomoHomeScene::HomeSceneEditRoom()
{
    QString  objectName,name;
    QDataStream  out(&m_data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>objectName>>name;
    //MyMsgLog<<objectName<<name;
    QDir  *tempDir = new QDir("home_control/");
    QStringList  filter = (QStringList() << "*.xml");
    tempDir->setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(tempDir->entryInfoList());

    for(int i=0;i<fileInfo->size();i++)
    {
        QString fileName = fileInfo->at(i).fileName();
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
        QDomElement   layerElement;
        QDomElement   roomElement;
        houseElement = scene.firstChildElement("house");

        layerElement = houseElement.firstChildElement("layer");
        while(!layerElement.isNull())
        {
            roomElement=layerElement.firstChildElement("room");
            while(!roomElement.isNull())
            {
                if(roomElement.attribute("uid")==objectName)
                {
                    roomElement.setAttribute("name",name);
                    QFile  file1(tr("home_control/")+fileName);
                    if (!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))  return;
                    QTextStream stream(&file1);
                    scene.save(stream, 4);
                    file1.close();
                    sync();
                    break;
                }
                roomElement=roomElement.nextSiblingElement("room");
            }
            layerElement=layerElement.nextSiblingElement("layer");
        }
    }
    delete   tempDir;
    delete  fileInfo;
}

void  LuxDomoHomeScene::HomeAddDevice()
{
    QString  roomObjectName,deviceObjectName,type,id,name;
    QDataStream  out(&m_data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>roomObjectName>>deviceObjectName>>type>>id>>name;
    //MyMsgLog<<roomObjectName<<deviceObjectName<<type<<id<<name;
    QDir  *tempDir = new QDir("home_control/");
    QStringList  filter = (QStringList() << "*.xml");
    tempDir->setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(tempDir->entryInfoList());

   for(int i=0;i<fileInfo->size();i++)
   {
       QString fileName = fileInfo->at(i).fileName();
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
       QDomElement   layerElement;
       QDomElement   roomElement;
       houseElement = scene.firstChildElement("house");

       layerElement = houseElement.firstChildElement("layer");
       while(!layerElement.isNull())
       {
           roomElement=layerElement.firstChildElement("room");
           while(!roomElement.isNull())
           {
               if(roomElement.attribute("uid")==roomObjectName)
               {
                   QDomElement deviceElement = scene.createElement("device");
                   deviceElement.setAttribute("id",id);
                   deviceElement.setAttribute("type",type);
                   if(type==tr("var"))
                   {
                       deviceElement.setAttribute("subtype","var");
                   }
                   else
                   {
                       deviceElement.setAttribute("subtype","non-var");
                   }
                   deviceElement.setAttribute("name",name);
                   deviceElement.setAttribute("power","off");
                   if(type==tr("tv"))
                   {
                       deviceElement.setAttribute("mode","tv");
                   }
                   else
                   {
                       deviceElement.setAttribute("mode","auto");
                   }
                   deviceElement.setAttribute("temperature",tr("20"));
                   deviceElement.setAttribute("wind","auto");
                   if(tr("ac")==type)
                   {
                       deviceElement.setAttribute("swing","off");
                   }
                   else
                   {
                       deviceElement.setAttribute("swing","off");
                   }
                   deviceElement.setAttribute("value",tr("20"));
                   deviceElement.setAttribute("open","off");
                   deviceElement.setAttribute("uid",deviceObjectName);
                   deviceElement.setAttribute("actflag","enable");
                   deviceElement.setAttribute("play",tr("off"));
                   deviceElement.setAttribute("source",tr("mp3"));
                   deviceElement.setAttribute("mute",tr("on"));
                   deviceElement.setAttribute("channel",tr("0"));
                   deviceElement.setAttribute("musicname",tr(""));
                   deviceElement.setAttribute("channelSize",tr("4"));
                   roomElement.appendChild(deviceElement);
                   QFile  file1(tr("home_control/")+fileName);
                   if (!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))  return;
                   QTextStream stream(&file1);
                   scene.save(stream, 4);
                   file1.close();
                   sync();
                   break;
               }
               roomElement=roomElement.nextSiblingElement("room");
           }
           layerElement=layerElement.nextSiblingElement("layer");
       }
  }
  delete   tempDir;
  delete  fileInfo;
}

void  LuxDomoHomeScene::HomeDeleteDevice()
{
    QString  roomObjectName,deviceObjectName,type;
    QDataStream  out(&m_data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>roomObjectName>>deviceObjectName>>type;

    QDir  *tempDir = new QDir("home_control/");
    QStringList  filter = (QStringList() << "*.xml");
    tempDir->setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(tempDir->entryInfoList());

    for(int i=0;i<fileInfo->size();i++)
    {
        QString fileName = fileInfo->at(i).fileName();
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
        QDomElement   layerElement;
        QDomElement   roomElement;
        QDomElement deviceElement;
        houseElement = scene.firstChildElement("house");

        layerElement = houseElement.firstChildElement("layer");
        while(!layerElement.isNull())
        {
            roomElement=layerElement.firstChildElement("room");
            while(!roomElement.isNull())
            {
                if(roomElement.attribute("uid")==roomObjectName)
                {
                    deviceElement=roomElement.firstChildElement("device");
                    while(!deviceElement.isNull())
                    {
                        if(deviceElement.attribute("uid")==deviceObjectName)
                        {
                            roomElement.removeChild(deviceElement);
                            QFile  file1(tr("home_control/")+fileName);
                            if (!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))  return;
                            QTextStream stream(&file1);
                            scene.save(stream, 4);
                            file1.close();
                            sync();
                            break;
                        }
                        deviceElement=deviceElement.nextSiblingElement("device");
                     }
                     break;
                }
                roomElement=roomElement.nextSiblingElement("room");
            }
            layerElement=layerElement.nextSiblingElement("layer");
        }
    }
    delete   tempDir;
    delete  fileInfo;
}

void    LuxDomoHomeScene::HomeChangDeviceInforMation()
{
    QString  roomObjectName,deviceObjectName,type,name;
    QDataStream  out(&m_data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>roomObjectName>>deviceObjectName>>type>>name;

    QDir  *tempDir = new QDir("home_control/");
    QStringList  filter = (QStringList() << "*.xml");
    tempDir->setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(tempDir->entryInfoList());

    for(int i=0;i<fileInfo->size();i++)
    {
        QString fileName = fileInfo->at(i).fileName();
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
        QDomElement   layerElement;
        QDomElement   roomElement;
        QDomElement  deviceElement;
        houseElement = scene.firstChildElement("house");

        layerElement = houseElement.firstChildElement("layer");
        while(!layerElement.isNull())
        {
            roomElement=layerElement.firstChildElement("room");
            while(!roomElement.isNull())
            {
                if(roomElement.attribute("uid")==roomObjectName)
                {
                    deviceElement=roomElement.firstChildElement("device");
                    while(!deviceElement.isNull())
                    {
                        if(deviceElement.attribute("uid")==deviceObjectName)
                        {
                            deviceElement.setAttribute("name",name);
                            QFile  file1(tr("home_control/")+fileName);
                            if (!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))  return;
                            QTextStream stream(&file1);
                            scene.save(stream, 4);
                            file1.close();
                            sync();
                            break;
                        }
                        deviceElement=deviceElement.nextSiblingElement("device");
                    }
                    break;
                }
                roomElement=roomElement.nextSiblingElement("room");
            }
            layerElement=layerElement.nextSiblingElement("layer");
        }
    }
    delete   tempDir;
    delete  fileInfo;
}

void   LuxDomoHomeScene::CopySceneCondition(QDomElement  sceneDevice, QDomElement   homeDevice)
{
    if(homeDevice.isNull()||sceneDevice.isNull())
    {
        return;
    }
    sceneDevice.setAttribute("mode",homeDevice.attribute("mode"));
    sceneDevice.setAttribute("temperature",homeDevice.attribute("temperature"));
    sceneDevice.setAttribute("play",homeDevice.attribute("play"));
    sceneDevice.setAttribute("swing",homeDevice.attribute("swing"));
    sceneDevice.setAttribute("wind",homeDevice.attribute("wind"));
    sceneDevice.setAttribute("open",homeDevice.attribute("open"));
    sceneDevice.setAttribute("mute",homeDevice.attribute("mute"));
    sceneDevice.setAttribute("value",homeDevice.attribute("value"));
    sceneDevice.setAttribute("source",homeDevice.attribute("source"));
    sceneDevice.setAttribute("power",homeDevice.attribute("power"));
}





