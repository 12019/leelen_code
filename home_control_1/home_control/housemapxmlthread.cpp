#include "housemapxmlthread.h"
#include<homecontrolmaindlg.h>
#include<QPolygonF>
#include"MyBuffer.h"

short layerId,roomId;
extern QDeclarativeItem *item;
extern LMap_6410_Msg mLMap_6410_Msg;

houseMapXmlThread::houseMapXmlThread(QObject *parent) :
    QThread(parent)
{
    connect(this,SIGNAL(sig_Pixmap(QString)),this,SLOT(slot_Pixmap(QString)),Qt::BlockingQueuedConnection);
}

houseMapXmlThread::~houseMapXmlThread()
{

}

void houseMapXmlThread::run()
{
    if(eventType == loadroom)
    {
        loadRoomMapDev();
        eventType = 0;
    }
    else if(eventType == irlearn)
    {
        loadIrDevice();
        eventType = 0;
    }
    else if(eventType == creatscenexml)  //创建场景
    {
        creatSceneXml();
        //creatSceneAllOffXml();
        eventType = 0;
    }
    else if(eventType == scenedeviceload)
    {
        sceneDevLoad();
        eventType = 0;
    }
}

/**************************载入家居文件************************/
void houseMapXmlThread::loadRoomMapDev()
{
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg *)parent();
    int x,y;
    QString roomImage;
    if(pHomeControlDlg->m_layerElement.isNull())
    {
        printf("m_layerElement:isNull");
        return;
    }
    QDomElement roomElement;
    QDomElement deviceElement;
    QDomElement deviceShapeElement;
    roomElement=pHomeControlDlg->m_layerElement.firstChildElement("room");
    deviceElement=roomElement.firstChildElement("device");
    QString roomName;          //房间名

    layerId=pHomeControlDlg->m_layerElement.attribute("mark").toShort();
    roomId = m_roomId.toShort();

    while(!roomElement.isNull())  //找到房间
    {
        if(roomElement.attribute("id","")==m_roomId)
            break;
        roomElement=roomElement.nextSiblingElement("room");
    }

    roomName=roomElement.attribute("name");
    mLMap_6410_Msg.Layer_Room_Name=QString::number(layerId)+"层 "+roomName;
    pHomeControlDlg->m_pHCStatusBar->setlrName(mLMap_6410_Msg.Layer_Room_Name);
    roomImage="home_control/"+roomElement.attribute("image");

    emit sig_Pixmap(roomImage);

    mLMap_6410_Msg.ImgHeightRatio = mLMap_6410_Msg.roomImgH/roomImageHeight/1.00;
    mLMap_6410_Msg.ImgWidthRatio = mLMap_6410_Msg.roomImgW/roomImageWidth/1.00;
    QFileInfo info(currentImg);
    QMetaObject::invokeMethod(item,"setRoomImage",
                              Q_ARG(QVariant,"file://"+info.absoluteFilePath()));

    deviceElement=roomElement.firstChildElement("device");
    pHomeControlDlg->m_deviceElement=deviceElement;    //得到房间里所有设备
    QMetaObject::invokeMethod(item,"clearAllItem");
    while(!deviceElement.isNull())
    {
        QString subtype = deviceElement.attribute("subtype");
        QString type = deviceElement.attribute("type");
        if(subtype == "var")
            type = "var";
        QString deviceId = deviceElement.attribute("id");
        deviceShapeElement=deviceElement.firstChildElement("point");
        x = deviceShapeElement.attribute("x").toInt()*mLMap_6410_Msg.ImgWidthRatio;
        y = deviceShapeElement.attribute("y").toInt()*mLMap_6410_Msg.ImgHeightRatio;
        QMetaObject::invokeMethod(item,"addItem",
                                  Q_ARG(QVariant,x),
                                  Q_ARG(QVariant,y),
                                  Q_ARG(QVariant,deviceId),
                                  Q_ARG(QVariant,type));
        deviceElement=deviceElement.nextSiblingElement("device");
    }
    QMetaObject::invokeMethod(item,"setState",Q_ARG(QVariant,""));
    pHomeControlDlg->UpdateDevice();
    pHomeControlDlg->setloading(false);
    pHomeControlDlg->setMainState("");
}

/*****************************查找并载入要学习的红外设备************************/
void houseMapXmlThread::loadIrDevice()
{
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg *)parent();
    QDomElement deviceElement;
    deviceElement = pHomeControlDlg->m_deviceElement;
    while(!deviceElement.isNull())
    {
        QString type = deviceElement.attribute("type");
        if(type == "music" || type == "ac" || type == "tv")
        {
            QString deviceId = deviceElement.attribute("id");
            QString name = deviceElement.attribute("name");
            QMetaObject::invokeMethod(item,"irLearnEvent",
                                      Q_ARG(QVariant, type),
                                      Q_ARG(QVariant, name),
                                      Q_ARG(QVariant, deviceId));
        }
        deviceElement = deviceElement.nextSiblingElement("device");
    }
}

/***********************************************************************************
函数名 : creatSceneXml
描  述 : 第一次启动时创建配置文件（XML），添加红外设备等
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void houseMapXmlThread::creatSceneXml()
{
    int cmdCnt=0;
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg *)parent();
    QFile file("home_control/house-map-user-1.xml");
    if(file.exists())
    {
        for(int i=2;i<=6;i++)
        {
            char strFileName[256];
            sprintf(strFileName, "home_control/house-map-user-%d.xml", i);
            QFile file(strFileName);
            if(file.exists())
            {
                continue;
            }
            QFile::copy("home_control/house-map-user-1.xml",strFileName);
        }
        return;
    }
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QString string[8] = {"起床模式","就餐模式","影音模式","睡眠模式","自定义A","自定义B","居家","外出"};    //创建六个情景模式，居家和外出暂时没用

    QDomDocument document;
    QString strHeader( "version=\"1.0\" encoding=\"UTF-8\"" );      //创建xml的头
    document.appendChild(document.createProcessingInstruction("xml",strHeader));

    QDomElement root_house = document.createElement("house");
    document.appendChild(root_house);

    QDomElement houseElement;
    QDomElement infoElement;
    QDomElement layerElement;
    QDomElement roomElement;
    QDomElement deviceElement;
    houseElement = pHomeControlDlg->m_domDoc.firstChildElement("house");
    infoElement = houseElement.firstChildElement("info");

    QDomElement info = document.createElement("info");
    info.setAttribute("name","起床模式");
    info.setAttribute("id",infoElement.attribute("id"));
    root_house.appendChild(info);

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        QString mark = layerElement.attribute("mark");
        QDomElement root_layer = document.createElement("layer");
        root_layer.setAttribute("mark",mark);
        root_house.appendChild(root_layer);

        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            QDomElement root_room = document.createElement("room");
            QString id = roomElement.attribute("id");
            QString name = roomElement.attribute("name");
            root_room.setAttribute("id",id);
            root_room.setAttribute("name",name);
            root_layer.appendChild(root_room);

            deviceElement=roomElement.firstChildElement("device");

            while(!deviceElement.isNull())
            {
                QDomElement device = document.createElement("device");
                QString deviceId = deviceElement.attribute("id");
                QString name = deviceElement.attribute("name");
                QString type = deviceElement.attribute("type");
                QString subtype = deviceElement.attribute("subtype");
                QString mode = deviceElement.attribute("mode");
                QString temperature = deviceElement.attribute("temperature");
                QString wind = deviceElement.attribute("wind");
                QString swing = deviceElement.attribute("swing");
                QString value =deviceElement.attribute("value");
                QString open =deviceElement.attribute("open");
                QString actflag = "enable";

                device.setAttribute("id",deviceId);
                device.setAttribute("type",type);
                device.setAttribute("subtype",subtype);
                device.setAttribute("name",name);
                device.setAttribute("power","on");
                device.setAttribute("mode",mode);
                device.setAttribute("temperature",temperature);
                device.setAttribute("wind",wind);
                device.setAttribute("swing",swing);
                device.setAttribute("value",value);
                device.setAttribute("open",open);
                device.setAttribute("actflag",actflag);

                if(type == "music")
                {
                    QStringList IrLearnAction;
                    QDomElement action = pHomeControlDlg->m_domDoc.createElement("action");
                    QDomElement action2 = document.createElement("action");

                    deviceElement.setAttribute("source","mp3");
                    deviceElement.setAttribute("mute","on");
                    deviceElement.setAttribute("paly","off");
                    device.setAttribute("source","mp3");
                    device.setAttribute("mute","on");
                    device.setAttribute("paly","off");

                    QFile file(":/Ir/enMUSIC.txt");
                    if (file.open(QIODevice::ReadOnly) )
                    {
                        QTextStream stream(&file);
                        stream.setCodec(QTextCodec::codecForName("UTF-8"));
                        IrLearnAction=stream.readAll().split(" ");
                    }
                    file.close();
                    for(int i=0;i<IrLearnAction.count()-1;i++)
                    {
                        action.setAttribute(IrLearnAction.at(i),cmdCnt);
                        action2.setAttribute(IrLearnAction.at(i),cmdCnt);
                        cmdCnt += 1;
                        if(cmdCnt > 109)
                            cmdCnt = 0;
                    }
                    deviceElement.appendChild(action);
                    device.appendChild(action2);
                }
                else if(type == "tv")
                {
                    QStringList IrLearnAction;
                    QDomElement action = pHomeControlDlg->m_domDoc.createElement("action");
                    QDomElement action2 = document.createElement("action");
                    deviceElement.appendChild(action);
                    device.appendChild(action2);
                    deviceElement.setAttribute("mode","tv");
                    device.setAttribute("mode","tv");
                    mode = "tv";

                    QFile file(":/Ir/enTV.txt");
                    if (file.open(QIODevice::ReadOnly) )
                    {
                        QTextStream stream(&file);
                        stream.setCodec(QTextCodec::codecForName("GB2312"));
                        IrLearnAction=stream.readAll().split(" ");
                    }
                    file.close();
                    for(int i=0;i<IrLearnAction.count()-1;i++)
                    {
                        action.setAttribute(IrLearnAction.at(i),cmdCnt);
                        action2.setAttribute(IrLearnAction.at(i),cmdCnt);
                        cmdCnt += 1;
                        if(cmdCnt > 109)
                            cmdCnt = 0;
                    }
                }
                else if(type == "ac")
                {
                    QStringList IrLearnAction;
                    QDomElement action = pHomeControlDlg->m_domDoc.createElement("action");
                    QDomElement action2 = document.createElement("action");
                    deviceElement.appendChild(action);
                    device.appendChild(action2);
                    QFile file(":/Ir/enAIR.txt");
                    if (file.open(QIODevice::ReadOnly) )
                    {
                        QTextStream stream(&file);
                        stream.setCodec(QTextCodec::codecForName("GB2312"));
                        IrLearnAction=stream.readAll().split(" ");
                    }
                    file.close();
                    for(int i=0;i<IrLearnAction.count()-1;i++)
                    {
                        action.setAttribute(IrLearnAction.at(i),cmdCnt);
                        action2.setAttribute(IrLearnAction.at(i),cmdCnt);
                        cmdCnt += 1;
                        if(cmdCnt > 109)
                            cmdCnt = 0;
                    }
                }
                root_room.appendChild(device);
                deviceElement = deviceElement.nextSiblingElement("device");
            }
            cmdCnt = 0;
            roomElement = roomElement.nextSiblingElement("room");
        }
        layerElement = layerElement.nextSiblingElement("layer");
    }



    QTextStream out2( &file );
    document.save( out2, 4 );
    file.close();
    for(int i=2;i<=6;i++)
    {
        info.setAttribute("name",string[i-1]);
        char strFileName[256];
        sprintf(strFileName, "home_control/house-map-user-%d.xml", i);

        QFile file(strFileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            return;
        }
        QTextStream out2( &file );
        document.save( out2, 4 );
        file.close();
    }


    pHomeControlDlg->saveHouseMapXmlFile1();
    pHomeControlDlg->saveHouseMapXmlFile();
}


void houseMapXmlThread::creatSceneAllOffXml()
{
        QFile file1("home_control/house-map-user-8.xml");
        if(file1.exists())
            return;
        QString strFile = "home_control/house-map-user-1.xml";
        QDomDocument m_domDoc;
        QFile file(strFile);
        if (!file.open(QIODevice::ReadOnly))
        {
            return;
        }
        if (!m_domDoc.setContent(&file))
        {
            file.close();
            return;
        }
        file.close();
        QDomElement houseElement;
        //QDomElement infoElement;
        QDomElement roomElement;
        QDomElement layerElement;
        QDomElement deviceElement;

        houseElement = m_domDoc.firstChildElement("house");
        QDomElement info = houseElement.firstChildElement("info");
        info.setAttribute("name","外出");
        layerElement = houseElement.firstChildElement("layer");
        while(!layerElement.isNull())
        {
                roomElement = layerElement.firstChildElement("room");
                while (!roomElement.isNull())
                {
                    deviceElement = roomElement.firstChildElement("device");
                    while(!deviceElement.isNull())
                    {
                        deviceElement.setAttribute("power","off");
                        deviceElement = deviceElement.nextSiblingElement("device");
                    }
                    roomElement = roomElement.nextSiblingElement("room");
                }
                layerElement = layerElement.nextSiblingElement("layer");
        }

        QFile housemap("home_control/house-map-user-8.xml");
        if (!housemap.open(QIODevice::WriteOnly | QIODevice::Truncate)) return;
        QTextStream stream(&housemap);
        m_domDoc.save(stream, 4);
        housemap.close();
        sync();
}

/*********************************载入场景设备********************************/
void houseMapXmlThread::sceneDevLoad()
{
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg*)parent();
    pHomeControlDlg->setloading(true);
    QFile file(pHomeControlDlg->m_pHCSceneEdit->m_sceneXmlFileName);
    if (!file.open(QIODevice::ReadWrite))
    {
        return;
    }
    if (!pHomeControlDlg->m_pHCSceneEdit->doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement houseElement;
    QDomElement layerElement;
    QDomElement roomElement;
    QDomElement deviceElement;
    houseElement = pHomeControlDlg->m_pHCSceneEdit->doc.firstChildElement("house");
    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        if(layerElement.attribute("mark").toShort() == layerId)
        {
            roomElement = layerElement.firstChildElement("room");
            while (!roomElement.isNull())
            {
                if(roomElement.attribute("id").toShort() == roomId)
                {
                    pHomeControlDlg->m_pHCSceneEdit->m_deviceElement = roomElement.firstChildElement("device");
                    deviceElement = pHomeControlDlg->m_pHCSceneEdit->m_deviceElement;
                    while (!deviceElement.isNull())
                    {
                        QString Icon;
                        QString Id = deviceElement.attribute("id");
                        QString name = deviceElement.attribute("name");
                        QString power = deviceElement.attribute("power");
                        QString type = deviceElement.attribute("type");
                        QString actflag = deviceElement.attribute("actflag");
                        if(power == "on")
                            Icon = "images/home_control/list/on.png";
                        else
                            Icon = "images/home_control/list/off.png";
                        QMetaObject::invokeMethod(item,"hcsceneeditEvent",
                                                   Q_ARG(QVariant, "actionModel"),
                                                   Q_ARG(QVariant, name),
                                                   Q_ARG(QVariant, Icon),
                                                   Q_ARG(QVariant, Id),
                                                   Q_ARG(QVariant, type),
                                                   Q_ARG(QVariant,actflag));
                        deviceElement = deviceElement.nextSiblingElement("device");
                    }
                    pHomeControlDlg->setloading(false);
                    return;
                }
                roomElement = roomElement.nextSiblingElement("room");
            }
        }
        layerElement = layerElement.nextSiblingElement("layer");
    }
    pHomeControlDlg->setloading(false);
}

void houseMapXmlThread::slot_Pixmap(QString roomImage)
{
    QPixmap pixmap(roomImage);
    roomImageHeight=pixmap.height();
    roomImageWidth=pixmap.width();

    currentImg = roomImage.insert(13,"m");
    QPixmap pixmap1(currentImg);
    mLMap_6410_Msg.roomImgW = pixmap1.width();
    mLMap_6410_Msg.roomImgH = pixmap1.height();
}
