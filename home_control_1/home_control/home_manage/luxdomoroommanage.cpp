#include "luxdomoroommanage.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;
extern  QMultiMap<QString,QDeclarativeEngine*>   engines;

LuxDomoRoomManage::LuxDomoRoomManage(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    childItem=NULL;
    infraredLearnItem=NULL;
    connect(g_pMainDlg->m_pHomeControlDlg,SIGNAL(sigRefreshInfraredLearnInterface(QByteArray)),this,
                                                                              SLOT(slot_refreshInfraredLearnInterface(QByteArray)),Qt::QueuedConnection);
}

LuxDomoRoomManage::~LuxDomoRoomManage()
{
    g_pMainDlg->m_pHomeControlDlg->m_pRoomManage=NULL;
    QMultiMap<QString,QDeclarativeEngine*>::iterator  it;
    int   num=engines.count(tr("roomManage"));
    it=engines.find(tr("roomManage"));
    MyMsgLog<<num<<"roomManage";
    for(int i=0;i<num;i++)
    {
        QDeclarativeEngine   *engine= it.value();
        engine->deleteLater();
        it++;
    }
    engines.remove(tr("roomManage"));
}

void  LuxDomoRoomManage::componentComplete()
{
    g_pMainDlg->m_pHomeControlDlg->m_pRoomManage=this;
    MyMsgLog<<g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW*7/8);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(mLMap_6410_Msg.screenW/8);
    parentItem()->setY(0);

   CreateElement(tr("qrc:/QML/home_manage_dialog/LuxDomoDeviceContainerDialog.qml"));
   CreateElement(tr("qrc:/QML/content/LuxDomoToolbarDialog.qml"),false);
}

bool   LuxDomoRoomManage::FindChild(QString childName)
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

void   LuxDomoRoomManage::CreateElement(QString  strFile,bool type)
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl(strFile));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem());
    if(false==type)
    {
         item->setProperty("title",tr("房间管理"));
         item->setProperty("flag",true);
   }
   engines.insert(tr("roomManage"),engine);
}

void   LuxDomoRoomManage::SetRoomInforMation(QString name,QString objectName,QString image)
{
           roomName=name;
           roomObjectName=objectName;
           strImage=image;
           parentItem()->setProperty("roomObjectName",roomObjectName);
           if(FindChild(tr("toolbar"))==false)   {  return ; }
           childItem->setProperty("title",roomName);
           if(FindChild(tr("deviceContaniner"))==false) {  return;  }
          {
                childItem->setProperty("roomObjectName",roomObjectName);
                QMetaObject::invokeMethod(childItem,"init",Qt::DirectConnection);
          }
}

void  LuxDomoRoomManage::setDeviceInforMation(QString data)
{
         if(FindChild("deviceContaniner")==false) {  return; }
         QMetaObject::invokeMethod(childItem,"setDeviceInforMation",Qt::DirectConnection, Q_ARG(QVariant,data));
}

void  LuxDomoRoomManage::slot_refreshInfraredLearnInterface(QByteArray  data)
{
    if(infraredLearnItem!=NULL)
    {
        QMetaObject::invokeMethod(infraredLearnItem,"learnCode",Qt::DirectConnection);
    }
}

void  LuxDomoRoomManage::addDevice(QString type,QString address,QString attribute,QString name)
{
    if(FindChild("deviceContaniner")==false) {  return; }
    QMetaObject::invokeMethod(childItem,"addDevice",Qt::DirectConnection,
                                                     Q_ARG(QVariant,type),
                                                     Q_ARG(QVariant,address),
                                                     Q_ARG(QVariant,attribute),
                                                     Q_ARG(QVariant,name));
}










