#include "hcroomselect.h"
#include<homecontrolmaindlg.h>

extern QDeclarativeItem *item;

HCRoomSelect::HCRoomSelect(QObject *parent) :
    QObject(parent)
{
    m_layerId=0;
    m_roomId=0;
    setCancel(tr("取消"));
}

HCRoomSelect::~HCRoomSelect()
{
    //delete m_pHCRoomSelectqml;
}

void HCRoomSelect::loadLayerlist()   //载入楼层
{
    HomeControlMainDlg *pHomeControlDlg=(HomeControlMainDlg*)parent();

    QDomElement houseElement;
    QDomElement layerElement;
    QList<int>   layerList;
    houseElement=pHomeControlDlg->m_domDoc.firstChildElement("house");
    layerElement=houseElement.firstChildElement("layer");
    m_layerId = layerElement.attribute("mark").toShort();
    while(!layerElement.isNull())
    {
        layerList.append(layerElement.attribute("mark").toInt());
        layerElement=layerElement.nextSiblingElement("layer");
    }
    qSort(layerList.begin(),layerList.end());
    QListIterator<int>  i(layerList);
   while (i.hasNext())
    {
       QString  layerQString=QString::number(i.next());
       layerQString=layerQString+"层";
        QMetaObject::invokeMethod(item,"roomselectEvent",
                                Q_ARG(QVariant, "layer"),
                                Q_ARG(QVariant, layerQString));
    }
}

/*******************房间列表点击事件**************************/
void HCRoomSelect::roomList(short index)
{
    /*QMetaObject::invokeMethod(item,"setLoading",
                              Q_ARG(QVariant, true));*/
    HomeControlMainDlg *pHomeControlDlg=(HomeControlMainDlg*)parent();
    qDebug()<<"layer:"<<QString::number(layerIndex);
    qDebug()<<"room:"<<roomIdList.at(index);
    m_roomId=index;

    pHomeControlDlg->m_phouseMapXmlThread->eventType = loadroom;
    pHomeControlDlg->m_phouseMapXmlThread->m_roomId=roomIdList.at(index);
    pHomeControlDlg->m_phouseMapXmlThread->start();

    pHomeControlDlg->m_pHCStatusBar->setHeigh();
}

/***********************楼层列表点击事件**************************/
void HCRoomSelect::layerList(QString name)
{
    MyMsgLog<<name;
    m_layerId=name.left(name.size() - 1).toShort();
    qDebug()<<"m_layerId:"<<m_layerId;
    HomeControlMainDlg *pHomeControlDlg=(HomeControlMainDlg*)parent();
    //layerIndex = index+1;
    QMetaObject::invokeMethod(item,"roomselectEvent",Q_ARG(QVariant, "rclear"),
                              Q_ARG(QVariant, ""));

    QDomElement houseElement;
    QDomElement layerElement;
    QDomElement roomElement;

    houseElement=pHomeControlDlg->m_domDoc.firstChildElement("house");//m_domDoc.firstChildElement("house");
    layerElement=houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        if(layerElement.attribute("mark","").toShort()==m_layerId)
        {
            roomElement=layerElement.firstChildElement("room");
            break;
        }
        layerElement=layerElement.nextSiblingElement("layer");
    }
    pHomeControlDlg->m_layerElement=layerElement;  //找到楼层
    roomIdList.clear();
    while(!roomElement.isNull())
    {
        /*QMetaObject::invokeMethod(item,"roomselectEvent",
                                  Q_ARG(QVariant, "room"),
                                  Q_ARG(QVariant, roomElement.attribute("name")));
        roomIdList.append(roomElement.attribute("id",""));
        roomElement=roomElement.nextSiblingElement("room");*/
        QString image = "home_control/m" + roomElement.attribute("image");
        QFileInfo file(image);

        image = "file://"+file.absoluteFilePath();

        QMetaObject::invokeMethod(item,"addRoomModel",
                                  Q_ARG(QVariant, roomElement.attribute("name")),
                                  Q_ARG(QVariant, image));

        roomIdList.append(roomElement.attribute("id",""));
        roomElement=roomElement.nextSiblingElement("room");
    }
}


void HCRoomSelect::close()
{
    HomeControlMainDlg *pHomeControlDlg=(HomeControlMainDlg*)parent();
    pHomeControlDlg->setMainState("");
    QMetaObject::invokeMethod(item,"setLoading",
                              Q_ARG(QVariant, false));
    //QMetaObject::invokeMethod(item,"setState",Q_ARG(QVariant, ""));
    //m_pMainQml->hide();
}

void HCRoomSelect::loadRoomMap()
{
    //HomeControlMainDlg *pHomeControlDlg=(HomeControlMainDlg*)parent();
    //pHomeControlDlg->getDeviceStat();
    close();
}

void HCRoomSelect::setroomId(const short &roomId)
{
    m_roomId=roomId;
    emit sendroomIdChange();
}

short HCRoomSelect::getroomId(void)const
{
    return m_roomId;
}

void HCRoomSelect::setlayerId(const short &layerId)
{
    m_layerId=layerId;
    emit layerIdChange();
}

short HCRoomSelect::getlayerId(void)const
{
    return m_layerId;
}

/************************取消**************************/
QString HCRoomSelect::cancel()const
{
    return m_Cancel;
}

void HCRoomSelect::setCancel(const QString&cancel)
{
    m_Cancel = cancel;
    emit sig_Cancel();
}

void HCRoomSelect::Translate()
{
    setCancel(tr("取消"));
}

void HCRoomSelect::hideAll()
{

}
