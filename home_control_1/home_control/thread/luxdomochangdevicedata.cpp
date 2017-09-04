#include "luxdomochangdevicedata.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;
extern  MusicInfo   musicInfo;
extern  AcInfo     acInfo;
LuxDomoChangDeviceData::LuxDomoChangDeviceData(QObject *parent) : QObject(parent)
{

}

void   LuxDomoChangDeviceData::slot_changDeviceLocation(QString  roomObjectName, QString  deviceObjectName, QString  pageIndex, QString deviceIndex, QString folderIndex, QString folderName)
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
                    if(deviceElement.attribute("uid")==deviceObjectName)
                    {
                        deviceElement.setAttribute("pageIndex",pageIndex);
                        deviceElement.setAttribute("deviceIndex",deviceIndex);
                        deviceElement.setAttribute("folderIndex",folderIndex);
                        deviceElement.setAttribute("folderName",folderName);
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

void   LuxDomoChangDeviceData::slot_changFolderLocation(QString roomObjectName, QString  newPage,QString  newLocation, QMap<QString, int>  devices)
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
                     if(devices.contains(deviceElement.attribute("uid")))
                     {
                         deviceElement.setAttribute("folderIndex",newLocation);
                         deviceElement.setAttribute("pageIndex",newPage);
                     }
                     deviceElement=deviceElement.nextSiblingElement("device");
                 }
                 return;
              }
              roomElement=roomElement.nextSiblingElement("room");
         }
         layerElement=layerElement.nextSiblingElement("layer");
    }
}

void    LuxDomoChangDeviceData::slot_changPartDeviceLocation(QString  roomObjectName, int  value, QMap<QString, int> devices)
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
                    if(devices.contains(deviceElement.attribute("uid")))
                    {
                        int   deviceIndex=deviceElement.attribute("deviceIndex").toInt();
                        if(value<deviceIndex)
                        {
                             deviceElement.setAttribute("deviceIndex",QString::number(deviceIndex-1));
                        }
                    }
                    deviceElement=deviceElement.nextSiblingElement("device");
                 }
                 return;
             }
             roomElement=roomElement.nextSiblingElement("room");
         }
         layerElement=layerElement.nextSiblingElement("layer");
    }
}

void   LuxDomoChangDeviceData::slot_changDeviceAttribute(QString roomObjectName, QString deviceObjectName, QString  attributeName, QString attributeValue)
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
                    if(deviceElement.attribute("uid")==deviceObjectName)
                    {
                        deviceElement.setAttribute(attributeName,attributeValue);
                        MyMsgLog<<attributeName<<attributeValue;
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

QString   LuxDomoChangDeviceData::TranslateGBKChar(char* buf)
{
    QString m_FinalDecode;
    QByteArray m_HexArray;
    int   size=(int)buf[0];
    int   value=(int)buf[5];
    for(int i=6;i < size-1;i++)
    {
        m_HexArray.append((uchar)buf[i]);
    }
    QTextStream stream(&m_HexArray);
    if(value==0)
    {
        stream.setCodec(QTextCodec::codecForName("gbk"));
    }
    else  if(value==1)
    {
         stream.setCodec(QTextCodec::codecForName("utf-8"));
    }
    m_FinalDecode = stream.readAll();
    MyMsgLog<<"KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKkk"<<m_FinalDecode;
    return m_FinalDecode;
}

void   LuxDomoChangDeviceData::TranslateDeviceCmd(char *data)
{
    int   attribute = data[3];        //属性
    int   address = data[2];        //地址
    int   deviceId = (attribute<<8) + address;
    MyMsgLog<<address<<attribute<<deviceId;

    if(attribute==0x12)
    {
        TranslateSecurityCmd(data[4]);
        return;
    }

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
                int   id=deviceElement.attribute("id").toInt();
                if((id==deviceId)||(attribute==BackAudioMusic&&id>>8==BackAudioMusic&&deviceElement.attribute("channel").toInt()==address))
                {
                    QString  type=deviceElement.attribute("type");
                    if(tr("ac")==type)
                    {
                        if(attribute==13)
                        {
                            TranslateInfraredAcCmd(deviceElement,data,roomElement.attribute("uid"));
                        }
                        else
                        {
                            TranslateAcCmd(deviceElement,data,roomElement.attribute("uid"));
                        }
                    }
                    else  if(tr("music")==type)
                    {
                        if(attribute==13)
                        {
                            TranslateInfraredMusicCmd(deviceElement,data,roomElement.attribute("uid"));
                        }
                        else
                        {
                            TranslateMusicCmd(deviceElement,data,roomElement.attribute("uid"));
                        }
                    }
                    else  if(tr("tv")==type)
                    {
                        TranslateInfraredTvCmd(deviceElement,data,roomElement.attribute("uid"));
                    }
                    else   if(tr("curtain")==type)
                    {
                        TranslateCurtainCmd(deviceElement,data,roomElement.attribute("uid"));
                    }
                    else   if(tr("var")==type)
                    {
                        TranslateVar_LightCmd(deviceElement,data,roomElement.attribute("uid"));
                    }
                    else  if(tr("light")==type)
                    {
                        TranslateLightCmd(deviceElement,data,roomElement.attribute("uid"));
                    }
                    else  if(tr("pm")==type)
                    {
                        TranslatePMCmd(deviceElement,data,roomElement.attribute("uid"));
                    }
                    else  if(tr("humiture")==type)
                    {
                        TranslateHumitureCmd(deviceElement,data,roomElement.attribute("uid"));
                    }
                }
                deviceElement=deviceElement.nextSiblingElement("device");
            }
            roomElement=roomElement.nextSiblingElement("room");
        }
        layerElement=layerElement.nextSiblingElement("layer");
    }
}

void   LuxDomoChangDeviceData::TranslateInfraredAcCmd(QDomElement  device,char *data,QString  roomUid)
{
    QString   deviceUid=device.attribute("uid");
    int   cmd=data[5];
    QSqlQuery   query;
    QString         sql = "select   *   from ac" + deviceUid+ " where  code="+"'"+QString::number(cmd)+"'";
    int   mOrderIsValid = query.exec(sql);
    if(!mOrderIsValid || !query.next())
    {
         return;
    }

    if(query.value(1).toString()==tr("开"))
    {
        device.setAttribute("power",tr("on"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("power"),tr("on"));
        }
    }
   else
   {
        device.setAttribute("power",tr("off"));
        return;
   }

    if(query.value(2).toString()==tr("固定"))
    {
        device.setAttribute("swing",tr("off"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("swing"),tr("off"));
        }
    }
    else
    {
        device.setAttribute("swing",tr("on"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("swing"),tr("on"));
        }
    }

    QString  mode=g_pMainDlg->m_pHomeControlDlg->GetAcModeName(query.value(3).toString());
    device.setAttribute("mode",mode);
    if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
    {
        RefreshRoomControlInterface(roomUid,deviceUid,tr("mode"),mode);
    }

    QString speed=g_pMainDlg->m_pHomeControlDlg->GetAcInfraredSpeedName(query.value(4).toString());
    device.setAttribute("wind",speed);
    if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
    {
        RefreshRoomControlInterface(roomUid,deviceUid,tr("wind"),speed);
    }

    QString   temp=query.value(5).toString();
    temp=temp.left(2);
    device.setAttribute("temperature",temp);
    if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
    {
        RefreshRoomControlInterface(roomUid,deviceUid,tr("temperature"),temp);
    }
}

void    LuxDomoChangDeviceData::TranslateInfraredMusicCmd(QDomElement  device,char *data,QString  roomUid)
{
    QString   deviceUid=device.attribute("uid");
    int   deviceValue=device.attribute("value").toInt();
    int   cmd=data[5];

    switch (cmd) {
    case 1:
        device.setAttribute("power",tr("off"));
        break;
    case 2:
        device.setAttribute("power",tr("on"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("power"),tr("on"));
        }
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        device.setAttribute("source",tr("mp3"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("source"),tr("mp3"));
        }
        break;
    case 6:
        device.setAttribute("source",tr("cd"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("source"),tr("cd"));
        }
        break;
    case 7:
        device.setAttribute("source",tr("aux"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("source"),tr("aux"));
        }
        break;
    case 8:
        device.setAttribute("source",tr("radio"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("source"),tr("radio"));
        }
        break;
    case 9:
        break;
    case 10:
        break;
    case 11:
        device.setAttribute("play",tr("on"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("play"),tr("on"));
        }
        break;
    case 12:
        device.setAttribute("play",tr("off"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("play"),tr("off"));
        }
        break;
    case 13:
        device.setAttribute("value",QString::number(deviceValue-1));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("value"),QString::number(deviceValue-1));
        }
        break;
    case 14:
        device.setAttribute("value",QString::number(deviceValue+1));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("value"),QString::number(deviceValue+1));
        }
        break;
    default:
        break;
    }
}

void  LuxDomoChangDeviceData::TranslateInfraredTvCmd(QDomElement  device,char *data,QString  roomUid)
{
    QString   deviceUid=device.attribute("uid");
    int   deviceValue=device.attribute("value").toInt();
    int   cmd=data[5];

    switch (cmd) {
    case 21:
        device.setAttribute("power",tr("on"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("power"),tr("on"));
        }
        break;
    case 22:
        device.setAttribute("power",tr("off"));
        break;
    case 23:
        device.setAttribute("value",QString::number(deviceValue+1));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("value"),QString::number(deviceValue+1));
        }
        break;
    case 24:
        device.setAttribute("value",QString::number(deviceValue-1));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("value"),QString::number(deviceValue-1));
        }
        break;
    case 25:
         break;
     case 26:
         break;
     case 27:
        device.setAttribute("mode",tr("av"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("mode"),tr("av"));
        }
        break;
     case 28:
        device.setAttribute("mode",tr("tv"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("mode"),tr("tv"));
        }
        break;
     default:
         break;
     }
}

void   LuxDomoChangDeviceData::TranslateAcCmd(QDomElement  device,char *data,QString  roomUid)
{
    QString   deviceUid=device.attribute("uid");
    QStringList  acModeEn=acInfo.acModeEn;
    QStringList  acCentralSpeedEn=acInfo.acCentralSpeedEn;
    QStringList  acCentralWindEn=acInfo.acCentralWindEn;
    int   cmd=data[4];
    int   value=data[5];

    switch (cmd) {
    case 0:
        device.setAttribute("power",tr("off"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("power"),tr("off"));
        }
        break;
    case 1:
        device.setAttribute("power",tr("on"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("power"),tr("on"));
        }
        break;
    case 2:
        device.setAttribute("temperature",QString::number(value));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("temperature"),QString::number(value));
        }
        break;
    case 3:
        if(value<0||value>=acModeEn.size())
        {
            break;
        }
        device.setAttribute("mode",acModeEn.at(value));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("mode"),acModeEn.at(value));
        }
        break;
    case 4:
        if(value<0||value>=acCentralSpeedEn.size())
        {
            break;
        }
        device.setAttribute("wind",acCentralSpeedEn.at(value));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("wind"),acCentralSpeedEn.at(value));
        }
        break;
    case 5:
        if(value<0||value>=acCentralWindEn.size())
        {
            break;
        }
        device.setAttribute("swing",acCentralWindEn.at(value));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("swing"),acCentralWindEn.at(value));
        }
        break;
    default:
        break;
    }
}

void  LuxDomoChangDeviceData::TranslateCurtainCmd(QDomElement  device,char *data,QString  roomUid)
{
    QString   deviceUid=device.attribute("uid");
    int   cmd=data[4];

    switch (cmd) {
    case 0:
        device.setAttribute("power",tr("off"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("power"),tr("off"));
        }
        break;
    case 1:
        device.setAttribute("power",tr("on"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("power"),tr("on"));
        }
        break;
    case 2:
        device.setAttribute("power",tr("stop"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("power"),tr("stop"));
        }
        break;
    default:
        break;
    }
}

void  LuxDomoChangDeviceData::TranslateLightCmd(QDomElement  device,char *data,QString  roomUid)
{
    QString   deviceUid=device.attribute("uid");
    int   cmd=data[4];

    switch (cmd) {
    case 0:
        device.setAttribute("power",tr("off"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("power"),tr("off"));
        }
        break;
    case 1:
        device.setAttribute("power",tr("on"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("power"),tr("on"));
        }
        break;
    default:
        break;
    }
}

void   LuxDomoChangDeviceData::TranslateMusicCmd(QDomElement  device,char *data,QString  roomUid)
{
    QString   deviceUid=device.attribute("uid");
    QString   musicName;
    int   cmd=data[4];
    int   value=data[5];

    switch (cmd) {
    case 0:
        device.setAttribute("power",tr("off"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("power"),tr("off"));
        }
        break;
    case 1:
        device.setAttribute("power",tr("on"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("power"),tr("on"));
        }
        break;
    case 2:
        device.setAttribute("play",tr("on"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("play"),tr("on"));
        }
        break;
    case 3:
        device.setAttribute("play",tr("off"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("play"),tr("off"));
        }
        break;
    case 4:
        if(value>=musicInfo.musicBackMode.size()||value<0)
        {
            break;
        }
        device.setAttribute("source",musicInfo.musicBackMode.at(value));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("source"),musicInfo.musicBackMode.at(value));
        }
        break;
    case 9:
        if(!g_pMainDlg->m_pHomeControlDlg->m_musicVolume)
        {
            device.setAttribute("value",QString::number(value));
            if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
            {
                RefreshRoomControlInterface(roomUid,deviceUid,tr("value"),QString::number(value));
            }
        }
        break;
    case 10:
        device.setAttribute("mute",tr("on"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("mute"),tr("on"));
        }
        break;
    case 16:
        device.setAttribute("mute",tr("off"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("mute"),tr("off"));
        }
        break;
    case 17:
        musicName=TranslateGBKChar(data);
        device.setAttribute("musicname",musicName);
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("musicname"),musicName);
        }
        break;
    case 21:
        device.setAttribute("channelSize",QString::number(value));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("channelSize"),QString::number(value));
        }
        break;
    default:
        break;
    }
}

void   LuxDomoChangDeviceData::TranslateVar_LightCmd(QDomElement  device,char *data,QString  roomUid)
{
    QString   deviceUid=device.attribute("uid");
    int   cmd=data[4];
    int   value=data[5];

    switch (cmd) {
    case 0:
        device.setAttribute("power",tr("off"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("power"),tr("off"));
        }
        break;
    case 1:
        device.setAttribute("power",tr("on"));
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            RefreshRoomControlInterface(roomUid,deviceUid,tr("power"),tr("on"));
        }
        break;
    case 0x11:
        if(!g_pMainDlg->m_pHomeControlDlg->m_varValue)
        {
            device.setAttribute("value",QString::number(value));
            if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
            {
                RefreshRoomControlInterface(roomUid,deviceUid,tr("value"),QString::number(value));
            }
        }
        break;
    default:
        break;
    }
}

void  LuxDomoChangDeviceData::TranslateHumitureCmd(QDomElement  device,char * buf, QString  roomUid)
{
    QString  temp,humidness;
    int     data1=(int)buf[4];
    int     data2=(int)buf[5];
    int     data3=(int)buf[6];
    int     data4=(int)buf[7];
    int     data=data2*16*16+data1;
    if(data>32768)
    {
        data=65536-data;
        temp=tr("-")+QString::number(data/10)+tr(".")+QString::number(data%10)+tr("℃");
    }
    else
    {
        temp=QString::number(data/10)+tr(".")+QString::number(data%10)+tr("℃");
    }
    data=data4*16*16+data3;
    humidness=QString::number(data/10)+tr(".")+QString::number(data%10)+tr("%");
    emit  sigRefreshSensorValueFromCmd(roomUid,device.attribute("uid"),tr("Humiture"),temp,humidness);
}

void  LuxDomoChangDeviceData::TranslatePMCmd(QDomElement  device,char *buf, QString  roomUid)
{
    QString  air;
    int          data1=(int)buf[4];
    int          data2=(int)buf[5];
    int          data=data2*16*16+data1;
    air=QString::number(data/10)+tr(".")+QString::number(data%10);
    emit  sigRefreshSensorValueFromCmd(roomUid,device.attribute("uid"),tr("PM"),air,air);
}

void  LuxDomoChangDeviceData::TranslateSecurityCmd(char cmd)
{
    int  data=(int)cmd;
    if(data==0)
    {
        data=1;
    }
    else  if(data==1)
    {
        data=3;
    }
    else  if(data==2)
    {
        data=2;
    }
    g_pMainDlg->m_pSecurityMainDlg->ReceiveUdpData((unsigned char)data);
}

void   LuxDomoChangDeviceData::slot_changFolderName(QString  roomUid, QString name, QMap<QString, int> devices)
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
            if(roomElement.attribute("uid")==roomUid)
            {
                deviceElement=roomElement.firstChildElement("device");
                while(!deviceElement.isNull())
                {
                    if(devices.contains(deviceElement.attribute("uid")))
                    {
                         deviceElement.setAttribute("folderName",name);
                    }
                    deviceElement=deviceElement.nextSiblingElement("device");
                }
                return;
            }
            roomElement=roomElement.nextSiblingElement("room");
        }
        layerElement=layerElement.nextSiblingElement("layer");
    }
}

void  LuxDomoChangDeviceData::RefreshRoomControlInterface(QString roomUId,QString deviceUid,QString  attribute,QString value)
{
    QByteArray  data;
    QDataStream   in(&data,QIODevice::WriteOnly);
    in.setVersion(QDataStream::Qt_4_7);
    in<<roomUId<<deviceUid<<attribute<<value;
    MyMsgLog<<roomUId<<deviceUid<<attribute<<value;
    emit   sigRefreshRoomControlInterfaceFromCmd(data);
}
