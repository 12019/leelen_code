#include "luxdomoroomcontrol.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern MainDialog *g_pMainDlg;
extern LMap_6410_Msg mLMap_6410_Msg;
extern  OperateType   operatingType;
extern  QDeclarativeItem    *smartHomeItem;
extern  QMultiMap<QString,QDeclarativeEngine*>   engines;
LuxDomoRoomControl::LuxDomoRoomControl(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    folderItem=NULL;
    controlDeviceItem=NULL;
    deviceControls.clear();
    roomControlPages.clear();
    childItem=NULL;
    runningPageName=tr("0");
    connect(g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI,
                 SIGNAL(sigRoomControlData(QString,QString,QString,QString,QString,QString,QString,QString,QString)),
    this,SLOT(slot_addDevice(QString,QString,QString,QString,QString,QString,QString,QString,QString)),Qt::BlockingQueuedConnection);
    connect(g_pMainDlg->m_pHomeControlDlg->m_pApplyScene,SIGNAL(sigRefreshControlInterface(QByteArray)),
                  this,SLOT(slot_refreshControlInterface(QByteArray)),Qt::QueuedConnection);
    connect(g_pMainDlg->m_pHomeControlDlg->m_pChangDeviceData,SIGNAL(sigRefreshRoomControlInterfaceFromCmd(QByteArray)),
                 this,SLOT(slot_refreshControlInterface(QByteArray)),Qt::QueuedConnection);
    connect(g_pMainDlg->m_pHomeControlDlg->m_pThemebar,SIGNAL(sigRefreshSensorValue(QString,QString)),this,SLOT(slot_refreshSensorValue(QString,QString)));
}

LuxDomoRoomControl::~LuxDomoRoomControl()
{
    g_pMainDlg->m_pHomeControlDlg->roomControl=NULL;
    g_pMainDlg->m_pHomeControlDlg->roomControlPages.clear();
    QMultiMap<QString,QDeclarativeEngine*>::iterator  it;
    int   num=engines.count(tr("roomControl"));
    it=engines.find(tr("roomControl"));
    MyMsgLog<<num<<"roomControl";
    for(int i=0;i<num;i++)
    {
        QDeclarativeEngine   *engine= it.value();
        engine->deleteLater();
        it++;
    }
    engines.remove(tr("roomControl"));
}

void  LuxDomoRoomControl::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    g_pMainDlg->m_pHomeControlDlg->roomControl=this;
    parentItem()->setWidth(mLMap_6410_Msg.screenW);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(0);
    parentItem()->setY(0);
   QMetaObject::invokeMethod(parentItem(),"clearModel",Qt::DirectConnection);
   CreatePage();
}

void   LuxDomoRoomControl::init(QString objectName)
{
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingLabel->setParentItem(parentItem());
    ShowPage("1");
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->roomObjectName=objectName;
    roomObjectName=objectName;
    parentItem()->setObjectName(objectName);
    QStringList  value=g_pMainDlg->m_pHomeControlDlg->m_pThemebar->GetSensorValue(roomObjectName);
    if(value.size()==4)
    {
        parentItem()->setProperty("airValue",value.at(0));
        parentItem()->setProperty("tempValue",value.at(1));
        parentItem()->setProperty("humidnessValue",value.at(2));
    }
    operatingType=LoadRoomControlData;
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->start();
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingLabel->setParentItem(smartHomeItem);
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingLabel->setVisible(true);
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingLabel->show();
}

void   LuxDomoRoomControl::Back()
{
    parentItem()->hide();
    parentItem()->deleteLater();
}

void  LuxDomoRoomControl::ShowMenu()
{

}

void    LuxDomoRoomControl::CreatePage()
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/home_control_dialog/LuxDomoRoomControlPageDialog.qml"));
    QDeclarativeItem *pageItem = qobject_cast<QDeclarativeItem*>(component.create());
    pageItem->setObjectName(QString::number(roomControlPages.size()+1));
    pageItem->setParentItem(parentItem());
    roomControlPages.append(QString::number(roomControlPages.size()+1));
    engines.insert(tr("roomControl"),engine);
    AddHeader();
}

void  LuxDomoRoomControl::AddHeader()
{
    if(roomControlPages.size()==1)
    {
         return;
    }
    else  if(roomControlPages.size()==2)
    {
       parentItem()->setProperty("listW",30);
       QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection, Q_ARG(QVariant,QString::number(1)));
       QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection, Q_ARG(QVariant,QString::number(2)));
    }
    else
    {
        parentItem()->setProperty("listW",roomControlPages.size()*15);
        QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection, Q_ARG(QVariant,QString::number(roomControlPages.size())));
    }
}

bool   LuxDomoRoomControl::FindChild(QString childName)
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

void  LuxDomoRoomControl::ShowPage(QString pageName)
{
        QVectorIterator<QString>   it(roomControlPages);
        QDeclarativeItem    *page=NULL;
        while(it.hasNext())
        {
               QString    temp=it.next();
               if(FindChild(temp)==false)  {    continue;  }
               if(temp==pageName)          {    page=childItem;  }
                childItem->hide();
        }
        page->show();
        runningPageName=pageName;
        parentItem()->setProperty("pressIndex",pageName);
}

void     LuxDomoRoomControl::rightMovePage()
{
       QString    newPageName(tr("0"));
       int   count=roomControlPages.size();
       if(1==runningPageName.toInt())
       {
            newPageName=QString::number(count) ;
        }
       else  if(1<runningPageName.toInt())
             {
                   newPageName=QString::number(runningPageName.toInt()-1);
             }
        ShowPage(newPageName);
}

void    LuxDomoRoomControl::leftMovePage()
{
       QString    newPageName(tr("0"));
       int   count=roomControlPages.size();
       if(count==runningPageName.toInt())
       {
             newPageName=tr("1");
       }
      else  if(count>runningPageName.toInt())
            {
                   newPageName=QString::number(runningPageName.toInt()+1);
            }
       ShowPage(newPageName);
}

void   LuxDomoRoomControl::slot_addDevice(QString objectName, QString name, QString type, QString id, QString on_off, QString pageIndex, QString deviceIndex, QString folderIndex, QString folderName)
{
        if(pageIndex==tr("0"))
        {
              QVectorIterator<LuxDomoRoomControlPage*>   it(g_pMainDlg->m_pHomeControlDlg->roomControlPages);
              while(it.hasNext())
              {
                    LuxDomoRoomControlPage   *temp=it.next();
                    QString    str=temp->GetEmptyDeviceLocation();
                    if(str!=tr("NULL"))
                    {
                          pageIndex=temp->parent()->objectName();
                          break;
                    }
              }
        }

        if(pageIndex==tr("0"))  {  pageIndex=QString::number(roomControlPages.size()+1); }
        while(!roomControlPages.contains(pageIndex))  {  CreatePage(); }

        QVectorIterator<LuxDomoRoomControlPage*>   iter(g_pMainDlg->m_pHomeControlDlg->roomControlPages);
        while(iter.hasNext())
        {
              LuxDomoRoomControlPage   *temp=iter.next();
              if(temp->parentItem()->objectName()==pageIndex)
              {
                   temp->AddDevice(objectName,name,type,id,on_off,deviceIndex,folderIndex,folderName);
                   break;
              }
        }
}

void   LuxDomoRoomControl::slot_refreshControlInterface(QByteArray  data)
{
    QString   roomUid,deviceUid,attributeName,attributeValue;
    QDataStream  out(&data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>roomUid>>deviceUid>>attributeName>>attributeValue;

    if(roomUid!=roomObjectName)
    {
        return;
    }

    if(attributeName==tr("power"))
    {
        QVector<QDeclarativeItem*>::iterator  it=deviceControls.begin();
        for(;it!=deviceControls.end();it++)
        {
            if(deviceUid==(*it)->objectName())
            {
                QString   type=(*it)->property("deviceType").toString();
                (*it)->setProperty("on_off",attributeValue);
                (*it)->setProperty("deviceImage",g_pMainDlg->m_pHomeControlDlg->GetControlImage(type,attributeValue));
           }
        }
   }

    if(controlDeviceItem!=NULL)
    {
        QString  deviceObjectName=controlDeviceItem->property("deviceUid").toString();
        if(deviceUid==deviceObjectName)
        {
            QMetaObject::invokeMethod(controlDeviceItem,"init",Qt::DirectConnection,Q_ARG(QVariant,roomUid),Q_ARG(QVariant,deviceUid));
        }
    }
}

void  LuxDomoRoomControl::slot_refreshSensorValue(QString roomUid,QString deviceUid)
{
    if(roomUid!=roomObjectName)
    {
        return;
    }

    QStringList  value=g_pMainDlg->m_pHomeControlDlg->m_pThemebar->GetSensorValue(roomObjectName);
    if(value.size()==4)
    {
        parentItem()->setProperty("airValue",value.at(0));
        parentItem()->setProperty("tempValue",value.at(1));
        parentItem()->setProperty("humidnessValue",value.at(2));
    }

    if(controlDeviceItem!=NULL)
    {
        if(controlDeviceItem->objectName()==tr("sensorInformation"))
        {
            QMetaObject::invokeMethod(controlDeviceItem,"init",Qt::DirectConnection,Q_ARG(QVariant,roomUid),Q_ARG(QVariant,deviceUid));
        }
    }
}

void   LuxDomoRoomControl::EditFolderName(QString  name)
{
    if(folderItem!=NULL)
    {
       QMetaObject::invokeMethod(folderItem,"changName",Qt::DirectConnection,Q_ARG(QVariant,name));
    }
    folderItem=NULL;
}
