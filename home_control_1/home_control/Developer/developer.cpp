#include "developer.h"
#include<QFile>
#include<maindialog.h>
#include<homecontrolmaindlg.h>
#include<MyBuffer.h>

extern LIN_ExceptionType mLIN_ExceptionSvr;
extern MainDialog *g_pMainDlg;
extern QDeclarativeItem *item;

/***********************************************************************************
函数名 :LoadDevThread
描  述 : 用线程来载入某个房间内的所有设备
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void *LoadDevThread(void *arg)
{
    QString _name;
    QString _id;
    QString _type;
    g_pMainDlg->m_pHomeControlDlg->setloading(true);
    QDomElement deviceElement;
    QDomElement roomElement = g_pMainDlg->m_pHomeControlDlg->m_pDeveloper->mroomElement;
    deviceElement = roomElement.firstChildElement("device");
    while (!deviceElement.isNull())
    {

        _name = deviceElement.attribute("name");
        _id = deviceElement.attribute("id");
        _type = deviceElement.attribute("type");
        QMetaObject::invokeMethod(item,"setDevListView",
                                  Q_ARG(QVariant,_name),
                                  Q_ARG(QVariant,_id),
                                  Q_ARG(QVariant,_type));
        deviceElement = deviceElement.nextSiblingElement("device");
    }
    g_pMainDlg->m_pHomeControlDlg->setloading(false);
}


Developer::Developer(QObject *parent) : QObject(parent)
{
    house_map_user_x = "/mnt/disk/home_control/house-map-user-";
}

void Developer::setState(QString stat)
{
    MyMsgLog<<"TTTTTTTTTTTTT"<<stat;
    QMetaObject::invokeMethod(item,"settingsetState",Q_ARG(QVariant,stat));
}

/***********************************************************************
 函数名称：layerList
 说明：载入楼层
 ***********************************************************************/
QString Developer::layerList()
{
    QDomElement houseElement;
    QDomElement layerElement;

    QString layerName = "";
    houseElement=g_pMainDlg->m_pHomeControlDlg->m_domDoc.firstChildElement("house");
    layerElement=houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        layerName += ";" + layerElement.attribute("mark")+"层";
        layerElement=layerElement.nextSiblingElement("layer");
    }
    return layerName;
}

/***********************************************************************
 函数名称roomList
 说明：载入房间
 ***********************************************************************/
QString Developer::roomList()
{
    QString roomId = "";
    QString roomName = "";
    QString roomId_Name = "";
    QDomElement roomElement;
    roomElement = mlayerElement.firstChildElement("room");
    while(!roomElement.isNull())
    {
        roomId += ";" + roomElement.attribute("id");
        roomName += ";" + roomElement.attribute("name");
       //layerName += ";" + layerElement.attribute("mark")+"层";
        roomElement = roomElement.nextSiblingElement("room");
    }
    roomId_Name = roomName + "&" + roomId;
    return roomId_Name;
}

/***********************************************************************************
函数名 :setLayer
描  述 : 点击楼层的处理函数，主要用来记住当前的楼层并找到相应的配置文件
输入参数说明 :
    attr：属性
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void Developer::setLayer(QString attr)
{

    int length = attr.size();
    QString mark = attr.left(length-1);

    QDomElement houseElement;
    QDomElement layerElement;
    houseElement=g_pMainDlg->m_pHomeControlDlg->m_domDoc.firstChildElement("house");
    layerElement=houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
       if(layerElement.attribute("mark") == mark)
       {
           mlayerElement = layerElement;
           break;
       }
        layerElement=layerElement.nextSiblingElement("layer");
    }

}

/***********************************************************************************
函数名 :setRoom
描  述 : 点击房间的处理函数，主要用来记住当前的房间并找到相应的配置文件
输入参数说明 :
    attr：属性
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void Developer::setRoom(QString attr)
{
    QDomElement roomElement;
    roomElement= mlayerElement.firstChildElement("room");
    while (!roomElement.isNull())
    {
        if(roomElement.attribute("id") == attr)
        {
            mroomElement = roomElement;
            break;
        }
        roomElement=roomElement.nextSiblingElement("room");
    }
    MyMsgLog<<mroomElement.attribute("name");
    pthread_create(&LoadDevFD,NULL,LoadDevThread,NULL);
}

/***********************************************************************************
函数名 :deviceInfo
描  述 : 点击设备的处理函数，主要用来记住当前的设备并找到相应的配置文件
输入参数说明 :
    id：包括地址和属性
    type：类型
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
QString Developer::deviceInfo(QString id, QString type)
{
    QString Info;

    QDomElement deviceElement;
    QDomElement roomElement = mroomElement;
    deviceElement = roomElement.firstChildElement("device");
    while (!deviceElement.isNull())
    {
        if(deviceElement.attribute("id") == id && deviceElement.attribute("type") == type)
        {
            int attr = deviceElement.attribute("id").toInt()>>8 & 0xff;
            int addr = deviceElement.attribute("id").toInt() & 0xff;
            if(deviceElement.attribute("subtype") == "var")
                type = "var";
            Info = type + ";" + QString::number(attr) + ";" + deviceElement.attribute("name") + ";" + QString::number(addr);
            mdeviceElement = deviceElement;
            break;
        }
        deviceElement = deviceElement.nextSiblingElement("device");
    }
    return Info;
}

/***********************************************************************************
函数名 :saveconfigFile
描  述 : 保存配置文件包括场景配置文件
输入参数说明 :
    type：类型
    attr：属性
    name：名称
    addr：地址
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void Developer::saveconfigFile(QString type, QString attr, QString name, QString addr)
{
    int id = (attr.toInt()<<8) + addr.toInt();
    MyMsgLog<<type<<attr<<name<<addr<<QString::number(id);
    saveHouseMapUser(type,QString::number(id),name);
    saveHouseMapDefault(type,QString::number(id),name);

    //mdeviceElement.setAttribute("type",type);
    //mdeviceElement.setAttribute("name",name);
    //mdeviceElement.setAttribute("id",QString::number(id));
    //AppEngine->saveHouseMapXmlFile();
}
void Developer::saveHouseMapUser(QString type, QString id, QString name)
{
    MyMsgLog<<type<<id;
    for(int i=1;i<=6;i++)
    {
        doc.clear();
        QString map = house_map_user_x + QString::number(i) + ".xml";
        QFile file(map);
        if(!file.exists())
        {
            MyMsgLog<<map<<"is not eixts";
        }
        if (!file.open(QIODevice::ReadOnly))
        {
            MyMsgLog<<map<<"open failed";
            return;
        }
        if (!doc.setContent(&file))
        {
            MyMsgLog<<map<<"open failed";
            file.close();
            return;
        }
        file.close();
        QDomElement houseElement;
        QDomElement layerElement;
        QDomElement roomElement;
        QDomElement deviceElement;
        houseElement = doc.firstChildElement("house");
        layerElement = houseElement.firstChildElement("layer");
        while (!layerElement.isNull())
        {
            if(layerElement.attribute("mark") == mlayerElement.attribute("mark"))
            {
                break;
            }
            layerElement = layerElement.nextSiblingElement("layer");
        }
        roomElement = layerElement.firstChildElement("room");
        while (!roomElement.isNull())
        {
            if(roomElement.attribute("id") == mroomElement.attribute("id"))
            {
                break;
            }
            roomElement = roomElement.nextSiblingElement("room");
        }
        deviceElement = roomElement.firstChildElement("device");
        while (!deviceElement.isNull())
        {
            //MyMsgLog<<deviceElement.attribute("id")<<mdeviceElement.attribute("id");
            if(deviceElement.attribute("id") == mdeviceElement.attribute("id") &&
                    deviceElement.attribute("type") == mdeviceElement.attribute("type") &&
                    deviceElement.attribute("subtype") == mdeviceElement.attribute("subtype"))
            {
                MyMsgLog<<deviceElement.attribute("name")<<name;
                if(type == "var")    //表示可调灯光
                {
                    deviceElement.setAttribute("subtype",type);
                }
                else
                {
                    deviceElement.setAttribute("type",type);
                    deviceElement.setAttribute("subtype","non-var");
                }
                deviceElement.setAttribute("id",id);
                deviceElement.setAttribute("name",name);
                break;
            }
            deviceElement = deviceElement.nextSiblingElement("device");
        }

        QFile file1(map);
        if (!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return;
        QTextStream stream(&file1);
        doc.save(stream, 4);
        file1.close();
    }
}
void Developer::saveHouseMapDefault(QString type, QString id, QString name)
{
    QString oldDevType;
    if(mdeviceElement.attribute("type") == "light" && mdeviceElement.attribute("subtype") == "var")
    {
        oldDevType = "var";
    }
    else
    {
        oldDevType = mdeviceElement.attribute("type");
    }
    QMetaObject::invokeMethod(item,"setDevInfo",
                              Q_ARG(QVariant,mdeviceElement.attribute("id")),
                              Q_ARG(QVariant,oldDevType),
                              Q_ARG(QVariant,id),
                              Q_ARG(QVariant,type)
                              );
    if(type == "var")
    {
        mdeviceElement.setAttribute("subtype",type);
    }
    else
    {
        mdeviceElement.setAttribute("type",type);
        mdeviceElement.setAttribute("subtype","non-var");
    }
    
    mdeviceElement.setAttribute("name",name);
    mdeviceElement.setAttribute("id",id);
    g_pMainDlg->m_pHomeControlDlg->saveHouseMapXmlFile();
}

void Developer::setDevAddr(QString name)
{
    mLIN_ExceptionSvr = LIN_DeveloperDevAddr;
    g_pMainDlg->m_pHomeControlDlg->setHCLineText(name);
}

void Developer::setDevName(QString name)
{
    mLIN_ExceptionSvr = LIN_DeveloperDevName;
    g_pMainDlg->m_pHomeControlDlg->setHCLineText(name);
}
