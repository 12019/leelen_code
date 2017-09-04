#include "luxdomoroomcontainer.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;
extern  OperateType   operatingType;
extern  QDeclarativeItem    *smartHomeItem;
extern  QMultiMap<QString,QDeclarativeEngine*>   engines;
LuxDomoRoomContainer::LuxDomoRoomContainer(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    childItem=NULL;
    rooms.clear();
    operateRoomItem=NULL;
    connect(g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI,SIGNAL(sigHomeManageData(QString,QString,QString)),
                   this,SLOT(slot_addRoom(QString,QString)),Qt::BlockingQueuedConnection);

    connect(this,SIGNAL(sigAddRoom(QString,QString,QString)),g_pMainDlg->m_pHomeControlDlg->m_pManageHomeData,SLOT(slot_addRoom(QString,QString,QString)));
    connect(this,SIGNAL(sigAddRoom(QString,QString,QString)),g_pMainDlg->m_pHomeControlDlg->mainIterface,SLOT(slot_addElement(QString,QString,QString)));
    connect(this,SIGNAL(sigDeleteRoom(QString)),g_pMainDlg->m_pHomeControlDlg->m_pManageHomeData,SLOT(slot_deleteRoom(QString)));
    connect(this,SIGNAL(sigDeleteRoom(QString)),g_pMainDlg->m_pHomeControlDlg->mainIterface,SLOT(slot_deleteElement(QString)));
    connect(this,SIGNAL(sigChangName(QString,QString)),g_pMainDlg->m_pHomeControlDlg->m_pManageHomeData,SLOT(slot_changRoomName(QString,QString)));
    connect(this,SIGNAL(sigChangName(QString,QString)),g_pMainDlg->m_pHomeControlDlg->mainIterface,SLOT(slot_changName(QString,QString)));
    connect(this,SIGNAL(sigAddRoom(QString,QString,QString)),g_pMainDlg->m_pHomeControlDlg->m_pThemebar,SLOT(slot_addRoom(QString,QString,QString)));
    connect(this,SIGNAL(sigDeleteRoom(QString)),g_pMainDlg->m_pHomeControlDlg->m_pThemebar,SLOT(slot_deleteRoom(QString)));
    connect(this,SIGNAL(sigChangName(QString,QString)),g_pMainDlg->m_pHomeControlDlg->m_pThemebar,SLOT(slot_changRoomName(QString,QString)));
}

LuxDomoRoomContainer::~LuxDomoRoomContainer()
{

}

void  LuxDomoRoomContainer::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW*7/8);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(0);
    parentItem()->setY(0);
    parentItem()->setProperty("screenH",mLMap_6410_Msg.screenH);
    parentItem()->setProperty("addDeviceH",mLMap_6410_Msg.controlH);
    parentItem()->setProperty("addDeviceW",mLMap_6410_Msg.controlW);
    parentItem()->setProperty("nameSize",mLMap_6410_Msg.controlFont);
    setAddButtonLocation();
}

void   LuxDomoRoomContainer::init()
{
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingLabel->setParentItem(parentItem());
    operatingType=LoadHomeManageData;
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->start();
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingLabel->setParentItem(smartHomeItem);
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingLabel->setVisible(true);
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingLabel->show();
}

bool   LuxDomoRoomContainer::FindChild(QString childName)
{
    int size = parentItem()->childItems().size();
    for(int i=0;i<size;i++)
   {
           QDeclarativeItem *item = (QDeclarativeItem*)parentItem()->childItems().at(i);
           if(item->objectName()==childName)
           {
                  childItem=item;
                  return   true;
           }
    }
   return  false;
}

void   LuxDomoRoomContainer::AddRoomContainerHeight()
{
    int size=rooms.size()+1;
    if(size%5!=0)  { return;}
    if(size/5<4)     { return;}
    parentItem()->setHeight(parentItem()->height()+mLMap_6410_Msg.pieceH);
    parentItem()->setY(parentItem()->y()-mLMap_6410_Msg.pieceH);
}

void  LuxDomoRoomContainer::deleteRoomContainerHeight()
{
    int   size=rooms.size();
    if(size%5!=0){ return;}
    if(size/5<4)   { return;}
    parentItem()->setHeight(parentItem()->height()-mLMap_6410_Msg.pieceH);
    parentItem()->setY(parentItem()->y()+mLMap_6410_Msg.pieceH);
}

void  LuxDomoRoomContainer::setAddButtonLocation()
{
    int number=rooms.size()+1;
    int  x=-1,y=-1;

    x=number%5;
    y=number/5;
    if(0==x)  {  x=4;    y-=1;}
    else   {  x-=1;  }

    parentItem()->setProperty("addRoomX",x*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin1);
    parentItem()->setProperty("addRoomY",y*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin);
}

void   LuxDomoRoomContainer::setRoomLocation(QString  roomName)
{
    int  number=rooms.value(roomName);
    int  x=-1,y=-1;

    x=number%5;
    y=number/5;
    if(0==x)  {  x=4;   y-=1; }
    else   {  x-=1;  }

    if(FindChild(roomName)==false)  {   return; }
    childItem->setX(x*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin1);
    childItem->setY(y*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin);
}

void   LuxDomoRoomContainer::CreateRoom(QString  objectName,QString  name)
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/home_manage_dialog/LuxDomoRoomDialog.qml"));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setObjectName(objectName);
    item->setProperty("roomName",name);
    item->setProperty("roomImage",g_pMainDlg->m_pHomeControlDlg->GetControlImage(tr("room")));
    item->setWidth(mLMap_6410_Msg.controlW);
    item->setHeight(mLMap_6410_Msg.controlH);
    item->setParentItem(parentItem());
    engines.insert(tr("homeManage"),engine);
}

void   LuxDomoRoomContainer::addRoom()
{
    if(g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->isRunning())
    {
        return;
    }
    AddRoomContainerHeight();
    QString    objectName=QString::number(g_pMainDlg->m_pHomeControlDlg->m_pManageHomeData->GetEmptyUid());

    QByteArray  data;
    QDataStream   in(&data,QIODevice::WriteOnly);
    in.setVersion(QDataStream::Qt_4_7);
    in<<objectName<<tr("新房间");
    g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->m_data=data;
    operatingType=AddRoom;
    g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->start();
    emit   sigAddRoom(objectName,tr("新房间"),tr("room"));

    CreateRoom(objectName,tr("新房间"));
    rooms.insert(objectName,rooms.size()+1);
    setRoomLocation(objectName);
    setAddButtonLocation();
}

void   LuxDomoRoomContainer::slot_addRoom(QString  objectName, QString  name)
{
    AddRoomContainerHeight();
    CreateRoom(objectName,name);
    rooms.insert(objectName,rooms.size()+1);
    setRoomLocation(objectName);
    setAddButtonLocation();
}

void   LuxDomoRoomContainer::showFunctionOption(QString roomName)
{
    if(FindChild(roomName)==false) { return; }
    operateRoomItem=childItem;
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/home_manage_dialog/LuxDomoRoomFunctionOptionDialog.qml"));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
     item->setParentItem(parentItem());
     item->setWidth(mLMap_6410_Msg.screenW*7/8);
     item->setHeight(mLMap_6410_Msg.screenH);
     item->setX(0);
     item->setY(-parentItem()->y());
     item->setProperty("roomName",operateRoomItem->property("roomName").toString());
     engines.insert(tr("homeManage"),engine);
}

void  LuxDomoRoomContainer::deleteRoom()
{
    deleteRoomContainerHeight();
    if(operateRoomItem==NULL) { return; }
    QString  roomName=operateRoomItem->objectName();

    QByteArray  data;
    QDataStream   in(&data,QIODevice::WriteOnly);
    in.setVersion(QDataStream::Qt_4_7);
    in<<roomName;
    g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->m_data=data;
    operatingType=DeleteRoom;
    g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->start();
    emit   sigDeleteRoom(roomName);

    operateRoomItem->deleteLater();
    operateRoomItem=NULL;
    int   value=rooms.value(roomName);
    rooms.remove(roomName);
    QMap<QString,int>::iterator  it=rooms.begin();
    for(;it!=rooms.end();it++)
    {
            if(it.value()>value)
            {
                  rooms.insert(it.key(),it.value()-1);
                  setRoomLocation(it.key());
            }
    }
    setAddButtonLocation();
}

void LuxDomoRoomContainer::setRoomName(QString  roomName)
{
    if(operateRoomItem==NULL){ return; }
    operateRoomItem->setProperty("roomName",roomName);

    QByteArray  data;
    QDataStream   in(&data,QIODevice::WriteOnly);
    in.setVersion(QDataStream::Qt_4_7);
    in<<operateRoomItem->objectName()<<roomName;
    g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->m_data=data;
    operatingType=EditRoom;
    g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->start();
    emit  sigChangName(operateRoomItem->objectName(),roomName);
}



