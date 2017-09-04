#include "luxdomodevicefolderpage.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;
extern  EditDataType    dataType;
LuxDomoDeviceFolderPage::LuxDomoDeviceFolderPage(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoDeviceFolderPage::~LuxDomoDeviceFolderPage()
{

}

void  LuxDomoDeviceFolderPage::componentComplete()
{
         MyMsgLog<<parentItem()->objectName()<<objectName();
}

QString   LuxDomoDeviceFolderPage::GetEmptyDeviceLocation(int  number)
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
      return   location;
}

bool    LuxDomoDeviceFolderPage::FindDevice(QString objectName)
{
         int size = parentItem()->childItems().size();
         for(int i=0;i<size;i++)
         {
              QDeclarativeItem *item = (QDeclarativeItem*)parentItem()->childItems().at(i);
              if(item->objectName()==objectName)
              {
                     deviceItem=item;
                     return   true;
              }
         }
         return  false;
}

void   LuxDomoDeviceFolderPage::setDeviceControlLocation(QString objectName,int  location)
{
       int   showAreaW=3*(mLMap_6410_Msg.controlW+3*mLMap_6410_Msg.interval)+3*mLMap_6410_Msg.interval;
       int   showAreaH=3*(mLMap_6410_Msg.controlH+3*mLMap_6410_Msg.interval)+3*mLMap_6410_Msg.interval;
       int   showAreaX=(mLMap_6410_Msg.screenW-showAreaW)/2;
       int   showAreaY=(mLMap_6410_Msg.screenH-showAreaH)/2;

       if(FindDevice(objectName)==false) {  return; }
       deviceItem->setWidth(mLMap_6410_Msg.controlW);
       deviceItem->setHeight(mLMap_6410_Msg.controlH);
       int    deviceLocation=GetEmptyDeviceLocation(location).toInt();
       int    y=deviceLocation%10;
       int    x=(deviceLocation-y)/10;
       deviceItem->setX(x*(mLMap_6410_Msg.controlW+3*mLMap_6410_Msg.interval)+3*mLMap_6410_Msg.interval+showAreaX);
       deviceItem->setY(y*(mLMap_6410_Msg.controlH+3*mLMap_6410_Msg.interval)+3*mLMap_6410_Msg.interval+showAreaY);
       deviceItem->setProperty("nameShow",true);
       deviceItem->show();
       deviceControls.insert(objectName,location);
}

void   LuxDomoDeviceFolderPage::folderPageMove(QString  direction)
{
    if(direction==tr("left"))
    {
          QMetaObject::invokeMethod(parentItem()->parentItem(),"leftMove",Qt::DirectConnection,Q_ARG(QVariant,parentItem()->objectName()));
    }
    else  if(direction==tr("right"))
              {
                       QMetaObject::invokeMethod(parentItem()->parentItem(),"rightMove",Qt::DirectConnection,Q_ARG(QVariant,parentItem()->objectName()));
              }
}

void     LuxDomoDeviceFolderPage::closeFolderPage()
{
          QMetaObject::invokeMethod(parentItem()->parentItem(),"hideFolderPage",Qt::QueuedConnection);
}

void    LuxDomoDeviceFolderPage::removeAllDevice()
{
           QMap<QString,int>::iterator   it=deviceControls.begin();
           for(;it!=deviceControls.end();it++)
           {
                  if(FindDevice(it.key())==false) {   continue;  }
                  deviceItem->setParentItem(parentItem()->parentItem());
                  deviceItem->hide();
           }
           deviceControls.clear();
}

void    LuxDomoDeviceFolderPage::deviceMoveOutFolder(QString  objectName)
{
               deviceControls.remove(objectName);
               QMetaObject::invokeMethod(parentItem()->parentItem(),"hideFolderPage",Qt::QueuedConnection);
}

void  LuxDomoDeviceFolderPage::changFolderName()
{
    dataType=FolderName;
    g_pMainDlg->m_pHomeControlDlg->m_editName=parentItem()->parentItem()->property("folderName").toString();
    g_pMainDlg->m_pHomeControlDlg->roomControl->folderItem=parentItem()->parentItem();
    g_pMainDlg->m_pHomeControlDlg->setHCLineText(parentItem()->parentItem()->property("folderName").toString());
}

void     LuxDomoDeviceFolderPage::on_offDevice(QString  deviceUid,bool  value)
{
    if(FindDevice(deviceUid)==false)
    {
        return;
    }
    QString   deviceType=deviceItem->property("deviceType").toString();
    QString   deviceImage;
    if(value)
    {
         deviceImage = g_pMainDlg->m_pHomeControlDlg->GetControlImage(deviceType,tr("on"));
    }
    else
    {
        deviceImage = g_pMainDlg->m_pHomeControlDlg->GetControlImage(deviceType,tr("off"));
    }
    deviceItem->setProperty("deviceImage",deviceImage);
}
