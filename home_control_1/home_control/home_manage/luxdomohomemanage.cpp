#include "luxdomohomemanage.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern MainDialog *g_pMainDlg;
extern LMap_6410_Msg mLMap_6410_Msg;
extern  QMultiMap<QString,QDeclarativeEngine*>   engines;
LuxDomoHomeManage::LuxDomoHomeManage(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoHomeManage::~LuxDomoHomeManage()
{
    g_pMainDlg->m_pHomeControlDlg->m_pHomeManage=NULL;
    QMultiMap<QString,QDeclarativeEngine*>::iterator  it;
    int   num=engines.count(tr("homeManage"));
    it=engines.find(tr("homeManage"));
    MyMsgLog<<num<<"homeManage";
    for(int i=0;i<num;i++)
    {
        QDeclarativeEngine   *engine= it.value();
        engine->deleteLater();
        it++;
    }
    engines.remove(tr("homeManage"));
}

void  LuxDomoHomeManage::componentComplete()
{
      g_pMainDlg->m_pHomeControlDlg->m_pHomeManage=this;
      MyMsgLog<<g_pMainDlg->m_pHomeControlDlg->m_pHomeManage->objectName();
      parentItem()->setWidth(mLMap_6410_Msg.screenW*7/8);
      parentItem()->setHeight(mLMap_6410_Msg.screenH);
      parentItem()->setX(mLMap_6410_Msg.screenW/8);
      parentItem()->setY(0);

      CreateElement(tr("qrc:/QML/home_manage_dialog/LuxDomoRoomContainerDialog.qml"));
      CreateElement(tr("qrc:/QML/content/LuxDomoToolbarDialog.qml"),false);
}

void   LuxDomoHomeManage::init()
{
           if(FindChild(tr("roomContaniner"))==true)
              QMetaObject::invokeMethod(childItem,"init",Qt::QueuedConnection);
}

bool   LuxDomoHomeManage::FindChild(QString childName)
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

void   LuxDomoHomeManage::CreateElement(QString  strFile,bool type)
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl(strFile));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem());
    if(false==type)
    {
        item->setProperty("title",tr("家居管理"));
        item->setProperty("flag",false);
   }
  engines.insert(tr("homeManage"),engine);
}


void  LuxDomoHomeManage::EditRoomName(QString roomName)
{
       if(FindChild(tr("roomContaniner"))==false)   {  return; }
       QMetaObject::invokeMethod(childItem,"setRoomName",Qt::DirectConnection, Q_ARG(QVariant,roomName));
}














