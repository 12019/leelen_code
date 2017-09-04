#include "luxdomomanagehomedata.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;

LuxDomoManageHomeData::LuxDomoManageHomeData(QObject *parent) : QObject(parent)
{

}

void     LuxDomoManageHomeData::slot_addRoom(QString  objectName,QString  name,QString   type)
{
    QDomElement   houseElement;
    QDomElement   layerElement;
    houseElement = g_pMainDlg->m_pHomeControlDlg->m_domDoc.firstChildElement("house");
    layerElement = houseElement.firstChildElement("layer");

    QDomElement roomElement =g_pMainDlg->m_pHomeControlDlg->m_domDoc.createElement("room");
    roomElement.setAttribute("x",0);
    roomElement.setAttribute("y",0);
    roomElement.setAttribute("w",0);
    roomElement.setAttribute("h",0);
    roomElement.setAttribute("image","");
    roomElement.setAttribute("id",objectName);
    roomElement.setAttribute("uid",objectName);
    roomElement.setAttribute("name",name);
    roomElement.setAttribute("mark",tr("0"));
   if(!layerElement.isNull())
   {
       layerElement.appendChild(roomElement);
       g_pMainDlg->m_pHomeControlDlg->saveHomeXmlFile();
   }
}

void    LuxDomoManageHomeData::slot_deleteRoom(QString  objectName)
{
    QDomElement   houseElement;
    QDomElement   layerElement;
    QDomElement   roomElement;
    houseElement = g_pMainDlg->m_pHomeControlDlg->m_domDoc.firstChildElement("house");

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            if(roomElement.attribute("uid")==objectName)
            {
                layerElement.removeChild(roomElement);
                g_pMainDlg->m_pHomeControlDlg->saveHomeXmlFile();
                return;
            }
           roomElement=roomElement.nextSiblingElement("room");
       }
       layerElement=layerElement.nextSiblingElement("layer");
    }
}

void   LuxDomoManageHomeData::slot_changRoomName(QString  objectName, QString  name)
{
    QDomElement   houseElement;
    QDomElement   layerElement;
    QDomElement   roomElement;
    houseElement = g_pMainDlg->m_pHomeControlDlg->m_domDoc.firstChildElement("house");

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            if(roomElement.attribute("uid")==objectName)
            {
                roomElement.setAttribute("name",name);
                g_pMainDlg->m_pHomeControlDlg->saveHomeXmlFile();
                return;
            }
           roomElement=roomElement.nextSiblingElement("room");
         }
        layerElement=layerElement.nextSiblingElement("layer");
    }
}

void   LuxDomoManageHomeData::slot_addDevice(QByteArray data)
{
    QString  roomObjectName,deviceObjectName,type,id,name;
    QDataStream  out(&data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>roomObjectName>>deviceObjectName>>type>>id>>name;
    MyMsgLog<<roomObjectName<<deviceObjectName<<type<<id<<name;

    QDomElement deviceElement =g_pMainDlg->m_pHomeControlDlg->m_domDoc.createElement("device");
    deviceElement.setAttribute("show_in_floor",tr("0"));
    deviceElement.setAttribute("show_in_room",tr("0"));
    deviceElement.setAttribute("x_floor",tr("0"));
    deviceElement.setAttribute("y_room",tr("0"));
    deviceElement.setAttribute("x_room",tr("0"));
     if(type==tr("tv"))
     {
         deviceElement.setAttribute("mode","tv");
     }
     else
     {
         deviceElement.setAttribute("mode","auto");
     }
     deviceElement.setAttribute("temperature",tr("20"));
     if(tr("ac")==type)
     {
        deviceElement.setAttribute("swing","on");
     }
     else
     {
         deviceElement.setAttribute("swing","off");
     }
     deviceElement.setAttribute("open","off");
     deviceElement.setAttribute("wind","auto");
     deviceElement.setAttribute("value",tr("20"));
     deviceElement.setAttribute("power","off");
     deviceElement.setAttribute("wind","auto");
     deviceElement.setAttribute("id",id);
     deviceElement.setAttribute("uid",deviceObjectName);
     deviceElement.setAttribute("name",name);
     if(type==tr("var"))
     {
         deviceElement.setAttribute("subtype","var");
     }
     else
     {
         deviceElement.setAttribute("subtype","non-var");
     }
     deviceElement.setAttribute("type",type);
     deviceElement.setAttribute("play",tr("off"));
     deviceElement.setAttribute("source",tr("mp3"));
     deviceElement.setAttribute("mute",tr("on"));
     deviceElement.setAttribute("deviceIndex",tr("0"));
     deviceElement.setAttribute("folderName",tr("0"));
     deviceElement.setAttribute("folderIndex",tr("0"));
     deviceElement.setAttribute("pageIndex",tr("0"));
     deviceElement.setAttribute("channel",tr("0"));
     deviceElement.setAttribute("musicname",tr(""));
     deviceElement.setAttribute("channelSize",tr("4"));

     QDomElement   houseElement;
     QDomElement   layerElement;
     QDomElement   roomElement;
     houseElement = g_pMainDlg->m_pHomeControlDlg->m_domDoc.firstChildElement("house");

     layerElement = houseElement.firstChildElement("layer");
     while(!layerElement.isNull())
     {
         roomElement = layerElement.firstChildElement("room");
         while(!roomElement.isNull())
         {
             if(roomElement.attribute("uid")==roomObjectName)
             {
                 roomElement.appendChild(deviceElement);
                 g_pMainDlg->m_pHomeControlDlg->saveHomeXmlFile();
                 return;
             }
             roomElement=roomElement.nextSiblingElement("room");
         }
         layerElement=layerElement.nextSiblingElement("layer");
     }
}

void    LuxDomoManageHomeData::slot_deleteDevice(QByteArray  data)
{
    QString  roomObjectName,deviceObjectName,type;
    QDataStream  out(&data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>roomObjectName>>deviceObjectName>>type;
    MyMsgLog<<roomObjectName<<deviceObjectName<<type;

    QDomElement   houseElement;
    QDomElement   layerElement;
    QDomElement   roomElement;
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
                    if(deviceElement.attribute("uid")==deviceObjectName)
                    {
                        roomElement.removeChild(deviceElement);
                        g_pMainDlg->m_pHomeControlDlg->saveHomeXmlFile();
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

void    LuxDomoManageHomeData::slot_changDeviceInforMation(QByteArray  data)
{
    QString  roomObjectName,deviceObjectName,type,name;
    QDataStream  out(&data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>roomObjectName>>deviceObjectName>>type>>name;
    MyMsgLog<<roomObjectName<<deviceObjectName<<type<<name;

    QDomElement   houseElement;
    QDomElement   layerElement;
    QDomElement   roomElement;
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
                   if(deviceElement.attribute("uid")==deviceObjectName)
                   {
                       deviceElement.setAttribute("name",name);
                       g_pMainDlg->m_pHomeControlDlg->saveHomeXmlFile();
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

void   LuxDomoManageHomeData::LoadAllUid()
{
    uids.clear();
    QDomElement   houseElement;
    QDomElement   layerElement;
    QDomElement   roomElement;
    QDomElement   deviceElement;
    houseElement = g_pMainDlg->m_pHomeControlDlg->m_domDoc.firstChildElement("house");

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        uids.append(layerElement.attribute("uid").toInt());
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            uids.append(roomElement.attribute("uid").toInt());
            deviceElement=roomElement.firstChildElement("device");
            while(!deviceElement.isNull())
            {
                uids.append(deviceElement.attribute("uid").toInt());
                deviceElement=deviceElement.nextSiblingElement("device");
            }
            roomElement=roomElement.nextSiblingElement("room");
        }
        layerElement=layerElement.nextSiblingElement("layer");
    }
}

int    LuxDomoManageHomeData::GetEmptyUid()
{
    int  value=-1;
    for(int  i=20;i<9999999;i++)
    {
        if(!uids.contains(i))
        {
            value=i;
            break;
        }
    }
    uids.append(value);
    return   value;
}



