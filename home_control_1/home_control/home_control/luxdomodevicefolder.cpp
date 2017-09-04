#include "luxdomodevicefolder.h"
#include"maindialog.h"
#include"MyBuffer.h"
#include"luxdomoroomcontrolpage.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;
extern  QMultiMap<QString,QDeclarativeEngine*>   engines;
LuxDomoDeviceFolder::LuxDomoDeviceFolder(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
     childItem=NULL;
     pointX=0;
     pointY=0;
     oldPageName=tr("NULL");
     deviceControls.clear();
     folderPages.clear();
     connect(this,SIGNAL(sigDeviceLocation(QString,QString,QString,QString,QString,QString)),
                    g_pMainDlg->m_pHomeControlDlg->m_pChangDeviceData,
                    SLOT(slot_changDeviceLocation(QString,QString,QString,QString,QString,QString)));
     connect(this,SIGNAL(sigFolderLoaction(QString,QString,QString,QMap<QString,int>)),
                    g_pMainDlg->m_pHomeControlDlg->m_pChangDeviceData,
                    SLOT(slot_changFolderLocation(QString,QString,QString,QMap<QString,int>)));
     connect(this,SIGNAL(sigPartDeviceLocation(QString,int,QMap<QString,int>)),
                   g_pMainDlg->m_pHomeControlDlg->m_pChangDeviceData,
                   SLOT(slot_changPartDeviceLocation(QString,int,QMap<QString,int>)));
     connect(this,SIGNAL(sigchangFolderName(QString,QString,QMap<QString,int>)),
                    g_pMainDlg->m_pHomeControlDlg->m_pChangDeviceData,
                    SLOT(slot_changFolderName(QString,QString,QMap<QString,int>)));
}

LuxDomoDeviceFolder::~LuxDomoDeviceFolder()
{

}
void  LuxDomoDeviceFolder::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setProperty("nameSize",mLMap_6410_Msg.controlFont);
}

QString   LuxDomoDeviceFolder::GetEmptyDeviceLocation(int  number)
{
      QString   location(tr("NULL"));

      switch (number) {
             case 1: {  location=tr("00");    break;  }
             case 2: {  location=tr("10");    break;  }
             case 3: {  location=tr("20");    break;  }
             case 4: {  location=tr("01");    break;  }
             case 5: {  location=tr("11");    break;  }
             case 6: {  location=tr("21");    break;  }
             case 7: {  location=tr("02");    break;  }
             case 8: {  location=tr("12");    break;  }
             case 9: {  location=tr("22");    break;  }
             default:  {  break;   }
      }
      MyMsgLog<<location;
      return   location;
}

bool   LuxDomoDeviceFolder::FindChild(QString childName)
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

int    LuxDomoDeviceFolder::TransformPointX(int  pointX)
{
       int    x=-1;
       pointX=pointX+mLMap_6410_Msg.controlW/2;

       for(int  i=0;i<mLMap_6410_Msg.xPiece;i++)
       {
               if(pointX>=(i*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin)&&pointX<((i+1)*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin))
               {        x=i;       }
       }

       return  x;
}

int   LuxDomoDeviceFolder::TransformPointY(int  pointY)
{
        int   y=-1;
        pointY=pointY+mLMap_6410_Msg.controlH/2;

        for(int  j=0;j<mLMap_6410_Msg.yPiece;j++)
        {
               if(pointY>=(j*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin)&&pointY<((j+1)*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin))
               {         y=j;      }
        }

       return  y;
}

void   LuxDomoDeviceFolder::moveStart()
{
           pointX=parentItem()->x();
           pointY=parentItem()->y();
           QString    pageName=parentItem()->parentItem()->objectName();
           oldPageName=pageName;
           QVectorIterator<LuxDomoRoomControlPage*>   it(g_pMainDlg->m_pHomeControlDlg->roomControlPages);
           while(it.hasNext())
           {
                   LuxDomoRoomControlPage   *temp=it.next();
                  if(QString::compare(temp->parentItem()->objectName(),pageName)==0)
                  {
                                   temp->RemoveFolder(parentItem()->objectName());
                                   temp->Printf();
                                   break;
                   }
           }
          parentItem()->setParentItem(g_pMainDlg->m_pHomeControlDlg->roomControl->parentItem());
          parentItem()->setProperty("inwhich",tr("room"));
}

void    LuxDomoDeviceFolder::moveStop(int  pointX, int  PointY)
{
         int  x=TransformPointX(pointX);
         int  y=TransformPointY(PointY);

         if(-1==x||-1==y) {  this->DealError();   return;  }

        QString    pageName= g_pMainDlg->m_pHomeControlDlg->roomControl->runningPageName;
        QVectorIterator<LuxDomoRoomControlPage*>   it(g_pMainDlg->m_pHomeControlDlg->roomControlPages);

         while(it.hasNext())
         {
                LuxDomoRoomControlPage   *temp=it.next();
                if(QString::compare(temp->parentItem()->objectName(),pageName)==0)
                {
                          if(temp->DealFolderMove(parentItem(),x,y)==false)
                          {
                                this->DealError();
                          }
                          temp->Printf();
                          return;
                 }
          }
}

void    LuxDomoDeviceFolder::isChangPage(int pointX)
{
             if(pointX<0)
             {
                   g_pMainDlg->m_pHomeControlDlg->roomControl->rightMovePage();
             }
            else  if(pointX>mLMap_6410_Msg.screenW-mLMap_6410_Msg.controlW)
                      {
                                 g_pMainDlg->m_pHomeControlDlg->roomControl->leftMovePage();
                      }
}

void  LuxDomoDeviceFolder::DealError()
{
        int  x=TransformPointX(pointX);
        int  y=TransformPointY(pointY);
        g_pMainDlg->m_pHomeControlDlg->roomControl->ShowPage(oldPageName);
        QVectorIterator<LuxDomoRoomControlPage*>   it(g_pMainDlg->m_pHomeControlDlg->roomControlPages);

        while(it.hasNext())
       {
            LuxDomoRoomControlPage   *temp=it.next();
            if(QString::compare(temp->parentItem()->objectName(),oldPageName)==0)
            {
                      temp->AddFolderControl(parentItem(),x,y);
                      temp->Printf();
                      return;
            }
        }
}


void   LuxDomoDeviceFolder::setDeviceControlLocation(QString objectName)
{       
        if(FindChild(objectName)!=true)  {  return;  }
        childItem->setProperty("inwhich",tr("folder"));
        if(deviceControls.size()>=9)
       {
           childItem->hide();
           deviceControls.insert(objectName,(deviceControls.size()+1));
           SaveDeviceLocation(objectName,QString::number(deviceControls.size()));
           return;
       }

       childItem->setWidth(mLMap_6410_Msg.controlW/3-mLMap_6410_Msg.interval);
       childItem->setHeight(mLMap_6410_Msg.controlW/3);
       int   location=GetEmptyDeviceLocation(deviceControls.size()+1).toInt();
       int   y=location%10;
       int   x=(location-y)/10;
      childItem->setX((x*mLMap_6410_Msg.controlW)/3+mLMap_6410_Msg.interval/2);
      childItem->setY((y*mLMap_6410_Msg.controlW)/3+mLMap_6410_Msg.interval/2);
      childItem->setProperty("nameShow",false);
      childItem->show();
      deviceControls.insert(objectName,(deviceControls.size()+1));
      SaveDeviceLocation(objectName,QString::number(deviceControls.size()));
}

void  LuxDomoDeviceFolder::CreateFolderPage(QString objectName)
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/home_control_dialog/LuxDomoDeviceFolderPageDialog.qml"));
    QDeclarativeItem *folderPageItem = qobject_cast<QDeclarativeItem*>(component.create());
    folderPageItem->setObjectName(objectName);
    int   showAreaW=3*(mLMap_6410_Msg.controlW+3*mLMap_6410_Msg.interval)+3*mLMap_6410_Msg.interval;
    int   showAreaH=3*(mLMap_6410_Msg.controlH+3*mLMap_6410_Msg.interval)+3*mLMap_6410_Msg.interval;
    int   showAreaX=(mLMap_6410_Msg.screenW-showAreaW)/2;
    int   showAreaY=(mLMap_6410_Msg.screenH-showAreaH)/2;
    folderPageItem->setWidth(mLMap_6410_Msg.screenW);
    folderPageItem->setHeight(mLMap_6410_Msg.screenH);
    folderPageItem->setX(0);
    folderPageItem->setY(0);
    folderPageItem->setProperty("showAreaW",showAreaW);
    folderPageItem->setProperty("showAreaH",showAreaH);
    folderPageItem->setProperty("showAreaX",showAreaX);
    folderPageItem->setProperty("showAreaY",showAreaY);
    folderPageItem->setParentItem(parentItem());
    folderPages.append(objectName);
    engines.insert(tr("roomControl"),engine);
}

void    LuxDomoDeviceFolder::ShowPage(QString  objectName)
{
        QDeclarativeItem *pageItem=NULL;
        QVectorIterator<QString>   it(folderPages);
         while(it.hasNext())
         {
                  QString  pageName=it.next();
                  if(FindChild(pageName)==false) {  continue;  }
                  MyMsgLog<<pageName<<objectName;
                  if(pageName==objectName)
                   {
                          pageItem=childItem;
                   }
                   childItem->setX(0);
                   childItem->setY(0);
                   childItem->hide();
         }
       if(pageItem!=NULL)   {   pageItem->show();  }
}

void     LuxDomoDeviceFolder::rightMove(QString pageName)
{
         int  size=folderPages.size();
         QString   newPageName(tr("0"));
         if(pageName.toInt()<=1)
         {
               newPageName=QString::number(size);
         }
         else
         {
               newPageName=QString::number(pageName.toInt()-1);
         }
         ShowPage(newPageName);
}

void   LuxDomoDeviceFolder::leftMove(QString pageName)
{
         int  size=folderPages.size();
         QString   newPageName(tr("0"));
         if(pageName.toInt()>=size)
         {
               newPageName=QString::number(1);
         }
       else
        {
             newPageName=QString::number(pageName.toInt()+1);
         }
        ShowPage(newPageName);
}

bool    LuxDomoDeviceFolder::CheckFolderItem()
{
            if(deviceControls.size()>1){  return  false; }

            QMap<QString,int>::iterator  iterator=deviceControls.begin();
            if(FindChild(iterator.key())==false)  { return  true;  }

            QString   pageName=parentItem()->parentItem()->objectName();
            QVectorIterator<LuxDomoRoomControlPage*>   it(g_pMainDlg->m_pHomeControlDlg->roomControlPages);
            while(it.hasNext())
             {
                       LuxDomoRoomControlPage   *temp=it.next();
                       if(QString::compare(temp->parentItem()->objectName(),pageName)==0)
                        {
                                temp->DeleteFolder(childItem,parentItem()->objectName());
                                temp->ShowAllItem();
                                parentItem()->hide();
                                //parentItem()->deleteLater();
                                break;
                        }
             }
           return  true;
}

void     LuxDomoDeviceFolder::showFolderControl()
{
          parentItem()->parentItem()->parentItem()->setProperty("isShowTitle",true);
         if(CheckFolderItem())  {  return;  }
         parentItem()->setWidth(mLMap_6410_Msg.controlW);
         parentItem()->setHeight(mLMap_6410_Msg.controlH);
         parentItem()->setX(pointX);
         parentItem()->setY(pointY);
         parentItem()->setProperty("state",false);

         int  number=0;
         if(deviceControls.size()>9)  { number=9;  }
         else  {  number=deviceControls.size();   }

          for(int i=1;i<=number;i++)
          {
                  QString   name=deviceControls.key(i);
                  if(FindChild(name)==false) {   continue;  }
                  childItem->setWidth(mLMap_6410_Msg.controlW/3-mLMap_6410_Msg.interval);
                  childItem->setHeight(mLMap_6410_Msg.controlW/3);
                  int   location=GetEmptyDeviceLocation(i).toInt();
                  int   y=location%10;
                  int   x=(location-y)/10;
                  childItem->setX((x*mLMap_6410_Msg.controlW)/3+mLMap_6410_Msg.interval/2);
                  childItem->setY((y*mLMap_6410_Msg.controlW)/3+mLMap_6410_Msg.interval/2);
                  childItem->setProperty("nameShow",false);
                  childItem->show();
          }

        QString   pageName=parentItem()->parentItem()->objectName();
        QVectorIterator<LuxDomoRoomControlPage*>   it(g_pMainDlg->m_pHomeControlDlg->roomControlPages);
         while(it.hasNext())
          {
                   LuxDomoRoomControlPage   *temp=it.next();
                   if(QString::compare(temp->parentItem()->objectName(),pageName)==0)
                   {
                          temp->ShowAllItem();
                          break;
                   }
           }
}


void    LuxDomoDeviceFolder::showFolderPage()
{
         parentItem()->parentItem()->parentItem()->setProperty("isShowTitle",false);
         pointX=parentItem()->x();
         pointY=parentItem()->y();
         int   showAreaW=3*(mLMap_6410_Msg.controlW+3*mLMap_6410_Msg.interval)+3*mLMap_6410_Msg.interval;
         int   showAreaH=3*(mLMap_6410_Msg.controlH+3*mLMap_6410_Msg.interval)+3*mLMap_6410_Msg.interval;
         int   showAreaX=(mLMap_6410_Msg.screenW-showAreaW)/2;
         int   showAreaY=(mLMap_6410_Msg.screenH-showAreaH)/2;
         parentItem()->setWidth(mLMap_6410_Msg.screenW);
         parentItem()->setHeight(mLMap_6410_Msg.screenH);
         parentItem()->setX(0);
         parentItem()->setY(0);
         parentItem()->setProperty("state",true);
         parentItem()->setProperty("showAreaW",showAreaW);
         parentItem()->setProperty("showAreaH",showAreaH);
         parentItem()->setProperty("showAreaX",showAreaX);
         parentItem()->setProperty("showAreaY",showAreaY);

         int  data1=deviceControls.size()%9;
         int  data2=deviceControls.size()/9;
         int  countPage=0;
         if(0==data1) {   countPage=data2;    }
         else {    countPage=data2+1;   }
        for(int i=1;i<=countPage;i++)
        {
                CreateFolderPage(QString::number(i));
        }

        QDeclarativeItem   *folderPageItem=NULL;
        QMap<QString,int>::iterator   iterator=deviceControls.begin();
        MyMsgLog<<deviceControls.size()<<"BBBBBBBBBBBBBBBBB";
        for(;iterator!=deviceControls.end();iterator++)
        {
                int  location=iterator.value()%9;
                int  page=0;
                if(0==location) {  page = iterator.value()/9;    location=9;  }
                else {   page=iterator.value()/9+1;  }
               QString  pageName=QString::number(page);

               if(FindChild(pageName)==false)     {       continue;     }
               folderPageItem=childItem;
               if(FindChild(iterator.key())==false)  {      continue;     }
               childItem->setParentItem(folderPageItem);
               QMetaObject::invokeMethod(folderPageItem,"setDeviceControl",Qt::DirectConnection,Q_ARG(QVariant,iterator.key()),Q_ARG(QVariant,location));
        }

       QString   pageName=parentItem()->parentItem()->objectName();
       QVectorIterator<LuxDomoRoomControlPage*>   it(g_pMainDlg->m_pHomeControlDlg->roomControlPages);
        while(it.hasNext())
        {
                   LuxDomoRoomControlPage   *temp=it.next();
                   if(QString::compare(temp->parentItem()->objectName(),pageName)==0)
                    {
                           temp->HideAllItem();
                           break;
                    }
        }
       parentItem()->show();
       ShowPage(tr("1"));
}

void    LuxDomoDeviceFolder::hideFolderPage()
{
           QVectorIterator<QString>   it(folderPages);
           while(it.hasNext())
           {
                     QString  pageName=it.next();
                     if(FindChild(pageName)==false)  {  continue;  }
                     QMetaObject::invokeMethod(childItem,"removeAlldevice",Qt::DirectConnection);
                     childItem->deleteLater();
           }
         folderPages.clear();
         showFolderControl();
}

void    LuxDomoDeviceFolder::deviceMoveOutFolder(QString  objectName)
{
               int  value=deviceControls.value(objectName);
               deviceControls.remove(objectName);
               QMap<QString,int>::iterator  it=deviceControls.begin();
               for(;it!=deviceControls.end();it++)
               {
                       if(it.value()>value)
                       {
                             deviceControls.insert(it.key(),it.value()-1);
                       }
               }
              emit   sigPartDeviceLocation(parentItem()->parentItem()->parentItem()->objectName(),value,deviceControls);
}

void    LuxDomoDeviceFolder::createDeviceControl(QString objectName, QString name, QString type, QString id, QString on_off,QString  deviceIndex)
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/home_control_dialog/LuxDomoRoomDeviceDialog.qml"));
    QDeclarativeItem *deviceControlItem = qobject_cast<QDeclarativeItem*>(component.create());
    deviceControlItem->setObjectName(objectName);
    deviceControlItem->setProperty("inwhich",tr("folder"));
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
    deviceControlItem->setProperty("roomObjectName",parentItem()->parentItem()->parentItem()->objectName());
    deviceControlItem->setProperty("screenW",mLMap_6410_Msg.screenW);
    deviceControlItem->setProperty("screenH",mLMap_6410_Msg.screenH);
    deviceControlItem->setWidth(mLMap_6410_Msg.controlW);
    deviceControlItem->setHeight(mLMap_6410_Msg.controlH);
    deviceControlItem->setParentItem(parentItem());
    engines.insert(tr("roomControl"),engine);
    int    number=deviceIndex.toInt();
    if(number>9)
    {
        deviceControlItem->hide();
        deviceControls.insert(objectName,number);
    }
    else   if(number>0||number<=9)
    {
        deviceControlItem->setWidth(mLMap_6410_Msg.controlW/3-mLMap_6410_Msg.interval);
        deviceControlItem->setHeight(mLMap_6410_Msg.controlW/3);
        int   location=GetEmptyDeviceLocation(number).toInt();
        int   y=location%10;
        int   x=(location-y)/10;
        deviceControlItem->setX((x*mLMap_6410_Msg.controlW)/3+mLMap_6410_Msg.interval/2);
        deviceControlItem->setY((y*mLMap_6410_Msg.controlW)/3+mLMap_6410_Msg.interval/2);
        deviceControlItem->setProperty("nameShow",false);
        deviceControlItem->show();
        deviceControls.insert(objectName,number);
    }
}

void   LuxDomoDeviceFolder::SaveDeviceLocation(QString  objectName,QString  deviceIndex)
{
    int  x=parentItem()->property("locationX").toInt();
    int  y=parentItem()->property("locationY").toInt();
    QString  folderName=parentItem()->property("folderName").toString();
    QString  roomObjectName=parentItem()->parentItem()->parentItem()->objectName();
    emit  sigDeviceLocation(roomObjectName,objectName,parentItem()->parentItem()->objectName(),deviceIndex,QString::number(y*5+x+1),folderName);
}

void   LuxDomoDeviceFolder::changAllDeviceLocation()
{
    int  x=parentItem()->property("locationX").toInt();
    int  y=parentItem()->property("locationY").toInt();
   QString  roomObjectName=parentItem()->parentItem()->parentItem()->objectName();
   emit  sigFolderLoaction(roomObjectName,parentItem()->parentItem()->objectName(),QString::number(y*5+x+1),deviceControls);
}

void  LuxDomoDeviceFolder::changName(QString  name)
{
    parentItem()->setProperty("folderName",name);
    QString  roomObjectName=parentItem()->parentItem()->parentItem()->objectName();
    emit   sigchangFolderName(roomObjectName,name,deviceControls);
}
