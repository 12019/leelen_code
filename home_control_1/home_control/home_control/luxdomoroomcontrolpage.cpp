#include "luxdomoroomcontrolpage.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern MainDialog *g_pMainDlg;
extern LMap_6410_Msg mLMap_6410_Msg;
extern  QMultiMap<QString,QDeclarativeEngine*>   engines;
LuxDomoRoomControlPage::LuxDomoRoomControlPage(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    for(int i=0;i<mLMap_6410_Msg.xPiece;i++)
         for(int j=0;j<mLMap_6410_Msg.yPiece;j++)
                deviceMatrix[i][j]=0;
      devices.clear();
      folders.clear();
      childItem=NULL;
      connect(this,SIGNAL(sigDeviceLocation(QString,QString,QString,QString,QString,QString)),
                     g_pMainDlg->m_pHomeControlDlg->m_pChangDeviceData,
                     SLOT(slot_changDeviceLocation(QString,QString,QString,QString,QString,QString)));
}

LuxDomoRoomControlPage::~LuxDomoRoomControlPage()
{

}

void  LuxDomoRoomControlPage::componentComplete()
{     MyMsgLog<<parentItem()->objectName()<<objectName();
      g_pMainDlg->m_pHomeControlDlg->roomControlPages.append(this);
      parentItem()->setWidth(mLMap_6410_Msg.screenW);
      parentItem()->setHeight(mLMap_6410_Msg.screenH);
      parentItem()->setX(0);
      parentItem()->setY(0);
      parentItem()->hide();
      Printf();
}

void  LuxDomoRoomControlPage::leftMove()
{
     if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
     {
          g_pMainDlg->m_pHomeControlDlg->roomControl->leftMovePage();
     }
}

void  LuxDomoRoomControlPage::rightMove()
{
       if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
      {
            g_pMainDlg->m_pHomeControlDlg->roomControl->rightMovePage();
       }
}

void   LuxDomoRoomControlPage::back()
{
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
              g_pMainDlg->m_pHomeControlDlg->roomControl->Back();
        }
}

QString   LuxDomoRoomControlPage::GetEmptyDeviceLocation()
{
      int i=0;
      int j=0;
      QString   location(tr("NULL"));

      for(i=0;i<mLMap_6410_Msg.xPiece;++i)
      {       for(j=0;j<mLMap_6410_Msg.yPiece;++j)
              {
                      if(deviceMatrix[i][j]==0)
                          {
                                        location=QString::number(i)+QString::number(j);
                                        return   location;
                          }
              }
      }

      return   location;
}

bool   LuxDomoRoomControlPage::FindChild(QString childName)
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

void   LuxDomoRoomControlPage::RemoveDevice(QString deviceName)
{
         int  location=devices.value(deviceName).toInt();
         int   y=location%10;
         int   x=(location-y)/10;
         deviceMatrix[x][y]=0;
         devices.remove(deviceName);
}

void  LuxDomoRoomControlPage::RemoveFolder(QString folderName)
{
       int  location=folders.value(folderName).toInt();
       int   y=location%10;
       int   x=(location-y)/10;
       deviceMatrix[x][y]=0;
       folders.remove(folderName);
}

void  LuxDomoRoomControlPage::AddDeviceControl(QDeclarativeItem *item, int x, int  y)
{
        item->setParentItem(parentItem());
        item->setX(x*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin);
        item->setY(y*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin);
        item->setProperty("inwhich",tr("page"));
        deviceMatrix[x][y]=1;
        devices.insert(item->objectName(),QString::number(x)+QString::number(y));
}

void  LuxDomoRoomControlPage::AddFolderControl(QDeclarativeItem *item, int x, int y)
{
       item->setParentItem(parentItem());
       item->setX(x*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin);
       item->setY(y*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin);
       item->setProperty("inwhich",tr("page"));
       deviceMatrix[x][y]=2;
       folders.insert(item->objectName(),QString::number(x)+QString::number(y));
}

void   LuxDomoRoomControlPage::CreateDeviceControl(QString objectName, QString name, QString type, QString id, QString on_off)
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/home_control_dialog/LuxDomoRoomDeviceDialog.qml"));
    QDeclarativeItem *deviceControlItem = qobject_cast<QDeclarativeItem*>(component.create());
    deviceControlItem->setObjectName(objectName);
    deviceControlItem->setProperty("inwhich",tr("page"));
    deviceControlItem->setProperty("deviceType",type);
    deviceControlItem->setProperty("deviceName",name);
    deviceControlItem->setProperty("on_off",on_off);
    int   deviceId=id.toInt();
    deviceControlItem->setProperty("deviceAddress",QString::number(deviceId&0xff));
    deviceControlItem->setProperty("deviceAttribute",QString::number(deviceId>>8));
    if(deviceId>>8==Ac_Infrared)
    {
        deviceControlItem->setProperty("deviceImage",g_pMainDlg->m_pHomeControlDlg->GetControlImage(type,tr("on")));
    }
    else
    {
        deviceControlItem->setProperty("deviceImage",g_pMainDlg->m_pHomeControlDlg->GetControlImage(type,on_off));
    }
    deviceControlItem->setProperty("roomObjectName",parentItem()->parentItem()->objectName());
    deviceControlItem->setProperty("screenW",mLMap_6410_Msg.screenW);
    deviceControlItem->setProperty("screenH",mLMap_6410_Msg.screenH);
    deviceControlItem->setWidth(mLMap_6410_Msg.controlW);
    deviceControlItem->setHeight(mLMap_6410_Msg.controlH);
    int   location=this->GetEmptyDeviceLocation().toInt();
    int   y=location%10;
    int   x=(location-y)/10;
    deviceControlItem->setX(x*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin);
    deviceControlItem->setY(y*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin);
    deviceControlItem->setParentItem(parentItem());
    deviceMatrix[x][y]=1;
    devices.insert(objectName,QString::number(x)+QString::number(y));
    engines.insert(tr("roomControl"),engine);
    emit    sigDeviceLocation(parentItem()->parentItem()->objectName(),objectName,parentItem()->objectName(),QString::number(5*y+1+x),tr("0"),tr("0"));
}

void   LuxDomoRoomControlPage::CreateDeviceControl(QString objectName, QString name, QString type, QString id, QString on_off,int  number)
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/home_control_dialog/LuxDomoRoomDeviceDialog.qml"));
    QDeclarativeItem *deviceControlItem = qobject_cast<QDeclarativeItem*>(component.create());
    deviceControlItem->setObjectName(objectName);
    deviceControlItem->setProperty("inwhich",tr("page"));
    deviceControlItem->setProperty("deviceType",type);
    deviceControlItem->setProperty("deviceName",name);
    deviceControlItem->setProperty("on_off",on_off);
    int   deviceId=id.toInt();
    deviceControlItem->setProperty("deviceAddress",QString::number(deviceId&0xff));
    deviceControlItem->setProperty("deviceAttribute",QString::number(deviceId>>8));
    if(deviceId>>8==Ac_Infrared)
    {
        deviceControlItem->setProperty("deviceImage",g_pMainDlg->m_pHomeControlDlg->GetControlImage(type,tr("on")));
    }
    else
    {
        deviceControlItem->setProperty("deviceImage",g_pMainDlg->m_pHomeControlDlg->GetControlImage(type,on_off));
    }
    deviceControlItem->setProperty("roomObjectName",parentItem()->parentItem()->objectName());
    deviceControlItem->setProperty("screenW",mLMap_6410_Msg.screenW);
    deviceControlItem->setProperty("screenH",mLMap_6410_Msg.screenH);
    deviceControlItem->setWidth(mLMap_6410_Msg.controlW);
    deviceControlItem->setHeight(mLMap_6410_Msg.controlH);
    int  x=-1;
    int  y=-1;
    x=number%5;
    y=number/5;
    if(0==x)  {  x=4;   y-=1; }
    else   {  x-=1;  }            MyMsgLog<<x<<y<<"OOOOOOOOOOOOOOO";
    deviceControlItem->setX(x*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin);
    deviceControlItem->setY(y*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin);
    deviceControlItem->setParentItem(parentItem());
    deviceMatrix[x][y]=1;
    devices.insert(objectName,QString::number(x)+QString::number(y));
    engines.insert(tr("roomControl"),engine);
}

void     LuxDomoRoomControlPage::CreateFolder(QString objectName,QString  folderName, int x, int y)
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/home_control_dialog/LuxDomoDeviceFolderDialog.qml"));
    QDeclarativeItem *folderItem = qobject_cast<QDeclarativeItem*>(component.create());
    folderItem->setObjectName(objectName);
    folderItem->setProperty("inwhich",tr("page"));
    folderItem->setProperty("folderName",folderName);
    folderItem->setProperty("screenW",mLMap_6410_Msg.screenW);
    folderItem->setProperty("screenH",mLMap_6410_Msg.screenH);
    folderItem->setProperty("locationX",x);
    folderItem->setProperty("locationY",y);
    folderItem->setWidth(mLMap_6410_Msg.controlW);
    folderItem->setHeight(mLMap_6410_Msg.controlH);
    folderItem->setX(x*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin);
    folderItem->setY(y*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin);
    folderItem->setParentItem(parentItem());
    deviceMatrix[x][y]=2;
    folders.insert(objectName,QString::number(x)+QString::number(y));
    engines.insert(tr("roomControl"),engine);
}

bool    LuxDomoRoomControlPage::DealDeviceMove(QDeclarativeItem *deviceControl,int x,int y)
{
          if(deviceMatrix[x][y]==0)
          {     return   this->DeviceMoveNewLocation(deviceControl,x,y);     }
          else    if(deviceMatrix[x][y]==1)
                      {     return   this->DeviceMoveToNewFolder(deviceControl,x,y);    }
          else
          {    return    this->DeviceMoveToFolder(deviceControl,x,y);   }
     return  false;
}

bool    LuxDomoRoomControlPage::DeviceMoveNewLocation(QDeclarativeItem *deviceControl,int x,int y)
{
            QString   objectName=deviceControl->objectName();
            deviceControl->setParentItem(parentItem());
            deviceControl->setProperty("inwhich",tr("page"));
            deviceControl->setX(x*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin);
            deviceControl->setY(y*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin);
            deviceMatrix[x][y]=1;
            devices.insert(objectName,QString::number(x)+QString::number(y));
            emit   sigDeviceLocation(parentItem()->parentItem()->objectName(),deviceControl->objectName(),parentItem()->objectName(),QString::number(5*y+x+1),tr("0"),tr("0"));
            return  true;
}

bool   LuxDomoRoomControlPage::DeviceMoveToNewFolder(QDeclarativeItem *deviceControl,int x,int y)
{
        QString   objectName=deviceControl->objectName();
        QString   location1=QString::number(x)+QString::number(y);
        QString   objectName1=devices.key(location1);

        QDeclarativeItem  *item1=NULL;
        if(FindChild(objectName1)==false)  {  return   false;  }
        item1=childItem;

         int  uid=g_pMainDlg->m_pHomeControlDlg->m_pManageHomeData->GetEmptyUid();
         QString   folderName=QString::number(uid);
         this->CreateFolder(folderName,tr("文件夹"),x,y);
         if(FindChild(folderName)==false)  {  return  false; }

         item1->hide();
         item1->setParentItem(childItem);
         devices.remove(objectName1);
    QMetaObject::invokeMethod(childItem,"setDeviceControlLocaton", Qt::DirectConnection,Q_ARG(QVariant,objectName1));
         deviceControl->hide();
         deviceControl->setParentItem(childItem);
     QMetaObject::invokeMethod(childItem,"setDeviceControlLocaton", Qt::QueuedConnection,Q_ARG(QVariant,objectName));
     return  true;
}

bool   LuxDomoRoomControlPage::DeviceMoveToFolder(QDeclarativeItem *deviceControl,int x,int y)
{
       QString   objectName=deviceControl->objectName();
       QString   folderLocation =QString::number(x)+QString::number(y);
       QString   folderObjectName=folders.key(folderLocation);

        if(this->FindChild(folderObjectName)==false){      return  false;    }
     deviceControl->setParentItem(childItem);
     QMetaObject::invokeMethod(childItem,"setDeviceControlLocaton", Qt::DirectConnection,Q_ARG(QVariant,objectName));
     return  true;
}

bool    LuxDomoRoomControlPage::DealFolderMove(QDeclarativeItem *folderControl,int x,int y)
{
           if(deviceMatrix[x][y]==0)
           {     return   this->FolderMoveNewLocation(folderControl,x,y);    }
           else    if(deviceMatrix[x][y]==1)
                       {      return   this->FolderMoveToDevice(folderControl,x,y);   }
           else
            {    return    this->FolderMoveToFolder(folderControl,x,y);   }
          return  false;
}

bool    LuxDomoRoomControlPage::FolderMoveNewLocation(QDeclarativeItem *folderControl,int x,int y)
{
            QString   objectName=folderControl->objectName();
             folderControl->setParentItem(parentItem());
             folderControl->setX(x*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin);
             folderControl->setY(y*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin);
             folderControl->setProperty("inwhich",tr("page"));
             folderControl->setProperty("locationX",x);
             folderControl->setProperty("locationY",y);
             deviceMatrix[x][y]=2;
             folders.insert(objectName,QString::number(x)+QString::number(y));
             QMetaObject::invokeMethod(folderControl,"changAllDeviceLocation", Qt::QueuedConnection);
             return  true;
}


bool     LuxDomoRoomControlPage::FolderMoveToDevice(QDeclarativeItem *folderControl,int x,int y)
{
          QString   objectName=folderControl->objectName();
          QString   deviceLocation =QString::number(x)+QString::number(y);
          QString   deviceName=devices.key(deviceLocation);

          QString   location=this->GetEmptyDeviceLocation();
          if(location==tr("NULL")) {  return  false; }
          int   y1=location.toInt()%10;
          int   x1=(location.toInt()-y1)/10;

         if(FindChild(deviceName)==false)  {  return  false;   }
         childItem->setX(x1*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin);
         childItem->setY(y1*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin);
         deviceMatrix[x1][y1]=1;
         devices.insert(deviceName,QString::number(x1)+QString::number(y1));
         emit  sigDeviceLocation(parentItem()->parentItem()->objectName(),deviceName,parentItem()->objectName(),QString::number(5*y1+1+x1),tr("0"),tr("0"));

         folderControl->setParentItem(parentItem());
         folderControl->setX(x*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin);
         folderControl->setY(y*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin);
         folderControl->setProperty("inwhich",tr("page"));
         folderControl->setProperty("locationX",x);
         folderControl->setProperty("locationY",y);
         deviceMatrix[x][y]=2;
         folders.insert(objectName,QString::number(x)+QString::number(y));
         QMetaObject::invokeMethod(folderControl,"changAllDeviceLocation", Qt::QueuedConnection);
         return  true;
}

bool   LuxDomoRoomControlPage::FolderMoveToFolder(QDeclarativeItem *folderControl,int x,int y)
{
         QString    objectName=folderControl->objectName();
         QString   folderLocation =QString::number(x)+QString::number(y);
         QString   folderName=folders.key(folderLocation);

         QString   location=this->GetEmptyDeviceLocation();
         if(location==tr("NULL")) {  return  false; }
         int   y1=location.toInt()%10;
         int   x1=(location.toInt()-y1)/10;

          if(FindChild(folderName)==false) {   return  false;  }
         childItem->setX(x1*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin);
         childItem->setY(y1*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin);
         childItem->setProperty("locationX",x1);
         childItem->setProperty("locationY",y1);
         deviceMatrix[x1][y1]=2;
         folders.insert(folderName,QString::number(x1)+QString::number(y1));
         QMetaObject::invokeMethod(childItem,"changAllDeviceLocation", Qt::QueuedConnection);

         folderControl->setParentItem(parentItem());
         folderControl->setX(x*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin);
         folderControl->setY(y*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin);
         folderControl->setProperty("inwhich",tr("page"));
         folderControl->setProperty("locationX",x);
         folderControl->setProperty("locationY",y);
         deviceMatrix[x][y]=2;
         folders.insert(objectName,QString::number(x)+QString::number(y));
         QMetaObject::invokeMethod(folderControl,"changAllDeviceLocation", Qt::QueuedConnection);
         return  true;
}

bool    LuxDomoRoomControlPage::DeleteFolder(QDeclarativeItem *  devItem, QString folderName)
{
         int   folderLocation=folders.value(folderName).toInt();
         int   y=folderLocation%10;
         int   x=(folderLocation-y)/10;
         devItem->setParentItem(parentItem());
         devItem->setX(x*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin);
         devItem->setY(y*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin);
         devItem->setWidth(mLMap_6410_Msg.controlW);
         devItem->setHeight(mLMap_6410_Msg.controlH);
         devItem->setEnabled(true);
         devItem->hide();
         devItem->setProperty("inwhich",tr("page"));
         deviceMatrix[x][y]=1;
         devices.insert(devItem->objectName(),QString::number(x)+QString::number(y));
         emit  sigDeviceLocation(parentItem()->parentItem()->objectName(),devItem->objectName(),parentItem()->objectName(),QString::number(y*5+x+1),tr("0"),tr("0"));
         folders.remove(folderName);
         return  true;
}

void   LuxDomoRoomControlPage::HideAllItem()
{
          int size = parentItem()->childItems().size();
          for(int i=0;i<size;i++)
         {
               QDeclarativeItem *item = (QDeclarativeItem*)parentItem()->childItems().at(i);
               if(devices.contains(item->objectName())||folders.contains(item->objectName()))
                {     item->hide();    }
         }
}

void   LuxDomoRoomControlPage::ShowAllItem()
{
         int size = parentItem()->childItems().size();
         for(int i=0;i<size;i++)
         {
               QDeclarativeItem *item = (QDeclarativeItem*)parentItem()->childItems().at(i);
              if(devices.contains(item->objectName())||folders.contains(item->objectName()))
               {     item->show();    }
         }
}



void    LuxDomoRoomControlPage::Printf()
{
    for(int i=0;i<mLMap_6410_Msg.yPiece;i++)
    MyMsgLog<<deviceMatrix[0][i]<<deviceMatrix[1][i]<<deviceMatrix[2][i]<<deviceMatrix[3][i]<<deviceMatrix[4][i];

    QMap<QString,QString>::iterator    it=devices.begin();
    while(it!=devices.end())
    {
        MyMsgLog<<"DDDDDDDDDDDDDDDDDDDDDDDDDD"<<it.key()<<it.value();
        it++;
    }
   QMap<QString,QString>::iterator    it1=folders.begin();
   while(it1!=folders.end())
   {
         MyMsgLog<<"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"<<it1.key()<<it1.value();
         it1++;
   }

   MyMsgLog<<folders.size()<<devices.size();
}

void   LuxDomoRoomControlPage::AddDevice(QString objectName, QString name, QString type, QString id, QString on_off, QString deviceIndex, QString folderIndex, QString folderName)
{
          if(folderIndex.toInt()>20||folderIndex.toInt()<1)
          {
                      if(deviceIndex.toInt()>20||deviceIndex.toInt()<1)
                      {   CreateDeviceControl(objectName,name,type,id,on_off);  }
                     else
                      {   CreateDeviceControl(objectName,name,type,id,on_off,deviceIndex.toInt());  }
          }
         else
          {
                  int  x=-1,  y=-1;
                  int  number=folderIndex.toInt();
                  x=number%5;
                  y=number/5;
                  if(0==x)  {  x=4;   y-=1; }
                  else   {  x-=1;  }

                  if(deviceMatrix[x][y]==2)
                  {
                                 QString   folder=folders.key(QString::number(x)+QString::number(y));
                                 if(FindChild(folder)==false) {  return; }
                                 QMetaObject::invokeMethod(childItem,"createDeviceControl", Qt::DirectConnection,
                                                           Q_ARG(QVariant,objectName),
                                                           Q_ARG(QVariant,name),
                                                           Q_ARG(QVariant,type),
                                                           Q_ARG(QVariant,id),
                                                           Q_ARG(QVariant,on_off),
                                                           Q_ARG(QVariant,deviceIndex));

                  }
                  else  if(deviceMatrix[x][y]==0)
                  {
                        int  uid=g_pMainDlg->m_pHomeControlDlg->m_pManageHomeData->GetEmptyUid();
                        CreateFolder(QString::number(uid),folderName,x,y);
                        if(FindChild(QString::number(uid))==false)  { return ; }
                        childItem->setProperty("folderName",folderName);
                        QMetaObject::invokeMethod(childItem,"createDeviceControl", Qt::DirectConnection,
                                                  Q_ARG(QVariant,objectName),
                                                  Q_ARG(QVariant,name),
                                                  Q_ARG(QVariant,type),
                                                  Q_ARG(QVariant,id),
                                                  Q_ARG(QVariant,on_off),
                                                  Q_ARG(QVariant,deviceIndex));
                  }
          }
}

void     LuxDomoRoomControlPage::on_offDevice(QString  deviceUid,bool  value)
{
    if(FindChild(deviceUid)==false)
    {
        return;
    }
    QString   deviceType=childItem->property("deviceType").toString();
    QString   deviceImage;
    if(value)
    {
         deviceImage = g_pMainDlg->m_pHomeControlDlg->GetControlImage(deviceType,tr("on"));
    }
    else
    {
        deviceImage = g_pMainDlg->m_pHomeControlDlg->GetControlImage(deviceType,tr("off"));
    }
    childItem->setProperty("deviceImage",deviceImage);
}
