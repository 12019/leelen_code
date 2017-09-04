#include "luxdomodevicecontainer.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   EditDataType     dataType;
extern   OperateType   operatingType;
extern   QDeclarativeItem    *smartHomeItem;
extern   QMultiMap<QString,QDeclarativeEngine*>   engines;
LuxDomoDeviceContainer::LuxDomoDeviceContainer(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
         devices.clear();
         childItem=NULL;     
         deviceOptionDialog=NULL;
  connect( g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI,SIGNAL(sigRoomManageData(QString,QString,QString,QString,QString)),
                  this,SLOT(slot_addDevice(QString,QString,QString,QString,QString)),Qt::BlockingQueuedConnection);
  connect(this,SIGNAL(sigAddDevice(QByteArray)),g_pMainDlg->m_pHomeControlDlg->m_pManageHomeData,SLOT(slot_addDevice(QByteArray)));
  connect(this,SIGNAL(sigDeleteDevice(QByteArray)),g_pMainDlg->m_pHomeControlDlg->m_pManageHomeData,SLOT(slot_deleteDevice(QByteArray)));
  connect(this,SIGNAL(sigChangDeviceInforMation(QByteArray)),g_pMainDlg->m_pHomeControlDlg->m_pManageHomeData,SLOT(slot_changDeviceInforMation(QByteArray)));
}

LuxDomoDeviceContainer::~LuxDomoDeviceContainer()
{

}

void   LuxDomoDeviceContainer::componentComplete()
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

void   LuxDomoDeviceContainer::init()
{
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingLabel->setParentItem(parentItem());
    operatingType=LoadRoomManageData;
    QString  name=parentItem()->property("roomObjectName").toString();
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->roomObjectName=name;
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->start();
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingLabel->setParentItem(smartHomeItem);
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingLabel->setVisible(true);
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingLabel->show();
}

bool   LuxDomoDeviceContainer::FindChild(QString childName)
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

void  LuxDomoDeviceContainer::setAddButtonLocation()
{
           int number=devices.size()+1;
           int  x=-1,y=-1;

           x=number%5;
           y=number/5;
          if(0==x)  {  x=4;    y-=1;}
          else   {  x-=1;  }

          parentItem()->setProperty("addDeviceX",x*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin1);
          parentItem()->setProperty("addDeviceY",y*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin);
}

void  LuxDomoDeviceContainer::setDeviceLocation(QString deviceName)
{
         int  number=devices.value(deviceName);
         int  x=-1,y=-1;

         x=number%5;
         y=number/5;
         if(0==x)  {  x=4;   y-=1; }
         else   {  x-=1;  }
         if(FindChild(deviceName)==false)  {   return; }
         childItem->setX(x*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin1);
         childItem->setY(y*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin);
}

void   LuxDomoDeviceContainer::AddDeviceContainerHeight()
{
        int size=devices.size()+1;
        if(size%5!=0)  { return;}
        if(size/5<4)     { return;}
        parentItem()->setHeight(parentItem()->height()+mLMap_6410_Msg.pieceH);
        parentItem()->setY(parentItem()->y()-mLMap_6410_Msg.pieceH);
}

void  LuxDomoDeviceContainer::deleteDeviceContainerHeight()
{
       int size=devices.size();
       if(size%5!=0)  { return;}
       if(size/5<4)     { return;}
       parentItem()->setHeight(parentItem()->height()-mLMap_6410_Msg.pieceH);
       parentItem()->setY(parentItem()->y()+mLMap_6410_Msg.pieceH);
}

void   LuxDomoDeviceContainer::CreateDevice(QString objectName,QString type,QString id,QString on_off,QString name)
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/home_manage_dialog/LuxDomoDeviceDialog.qml"));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setObjectName(objectName);
    item->setWidth(mLMap_6410_Msg.controlW);
    item->setHeight(mLMap_6410_Msg.controlH);
    item->setParentItem(parentItem());
    item->setProperty("deviceType",type);
    item->setProperty("deviceName",name);
    int   deviceId=id.toInt();
    item->setProperty("deviceAddress",QString::number(deviceId&0xff));
    item->setProperty("deviceAttribute",QString::number(deviceId>>8));
    item->setProperty("deviceImage",g_pMainDlg->m_pHomeControlDlg->GetControlImage(type,on_off));
    item->setProperty("on_off",on_off);
    item->setProperty("roomObjectName",parentItem()->property("roomObjectName").toString());
    engines.insert(tr("roomManage"),engine);
}

void  LuxDomoDeviceContainer::addDevice(QString type,QString address,QString attribute,QString name)
{         
         AddDeviceContainerHeight();
         int     _attr = attribute.toInt();
         int    _addr = address.toInt();
         int    id = (_attr << 8)+_addr;
         QString    deviceObjectName=QString::number(g_pMainDlg->m_pHomeControlDlg->m_pManageHomeData->GetEmptyUid());
         QString    roomObjectName=parentItem()->property("roomObjectName").toString();

         QByteArray  data;
         QDataStream   in(&data,QIODevice::WriteOnly);
         in.setVersion(QDataStream::Qt_4_7);
         in<<roomObjectName<<deviceObjectName<<type<<QString::number(id)<<name;
         g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->m_data=data;
         operatingType=AddDevice;
         g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->start();
         emit  sigAddDevice(data);

         CreateDevice(deviceObjectName,type,QString::number(id),tr("off"),name);
         devices.insert(deviceObjectName,devices.size()+1);
         setDeviceLocation(deviceObjectName);
         setAddButtonLocation();
}

void   LuxDomoDeviceContainer::setDeviceInforMation(QString  data)
{
          if( dataType==DeviceName)
          {
                  if(deviceOptionDialog!=NULL)
                 {
                      deviceOptionDialog->setProperty("optionName",data);
                  }
          }
          else  if( dataType==DeviceAttribute)
          {
                  if(deviceOptionDialog!=NULL)
                 {
                      deviceOptionDialog->setProperty("optionAttribute",data);
                 }
          }
          else  if( dataType==DeviceAdress)
          {
                   if(deviceOptionDialog!=NULL)
                   {
                         deviceOptionDialog->setProperty("optionAddress",data);
                   }
          }
          else  if(dataType==DeviceOtherName)
          {
                    if(FindChild(operateDevice)==false)  { return; }
                    childItem->setProperty("deviceName",data);
                    QString   roomObjectName=parentItem()->property("roomObjectName").toString();
                    QString   type=childItem->property("deviceType").toString();

                    QByteArray  data1;
                    QDataStream   in(&data1,QIODevice::WriteOnly);
                     in.setVersion(QDataStream::Qt_4_7);
                     in<<roomObjectName<<operateDevice<<type<<data;
                     g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->m_data=data1;
                     operatingType=EditDevice;
                     g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->start();
                     emit   sigChangDeviceInforMation(data1);
          }
}

void   LuxDomoDeviceContainer::showDeviceOption()
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem()->parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/home_manage_dialog/LuxDomoDeviceOptionDialog.qml"));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem()->parentItem());
    item->setWidth(mLMap_6410_Msg.screenW*7/8);
    item->setHeight(mLMap_6410_Msg.screenH);
    item->setX(0);
    item->setY(0);
    deviceOptionDialog=item;
    engines.insert(tr("roomManage"),engine);
}

void   LuxDomoDeviceContainer::deleteDevice(QString  deviceUid)
{
          deleteDeviceContainerHeight(); 
          if(FindChild(deviceUid)==false) { return; }
          QString   type=childItem->property("deviceType").toString();
          QString   roomObjectName=parentItem()->property("roomObjectName").toString();

          QByteArray  data;
          QDataStream   in(&data,QIODevice::WriteOnly);
          in.setVersion(QDataStream::Qt_4_7);
          in<<roomObjectName<<deviceUid<<type;
          g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->m_data=data;
          operatingType=DeleteDevice;
          g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->start();
          emit   sigDeleteDevice(data);

         childItem->deleteLater();
         int  value=devices.value(deviceUid);
         devices.remove(deviceUid);
         QMap<QString,int>::iterator  it=devices.begin();
         for(;it!=devices.end();it++)
         {
                if(it.value()>value)
               {
                     devices.insert(it.key(),it.value()-1);
                     setDeviceLocation(it.key());
               }
          }
          setAddButtonLocation();
}


void   LuxDomoDeviceContainer::setOperateDevice(QString  deviceUid)
{
          operateDevice=deviceUid;
}

void    LuxDomoDeviceContainer::slot_addDevice(QString objectName,QString type,QString id,QString on_off,QString name)
{
          AddDeviceContainerHeight();
          CreateDevice(objectName,type,id,on_off,name);
          devices.insert(objectName,devices.size()+1);
          setDeviceLocation(objectName);
          setAddButtonLocation();
}


