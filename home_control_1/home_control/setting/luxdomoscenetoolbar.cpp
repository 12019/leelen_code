#include "luxdomoscenetoolbar.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   OperateType   operatingType;

LuxDomoSceneToolbar::LuxDomoSceneToolbar(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    connect(g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI,SIGNAL(sigHomeScene(QByteArray)), this,SLOT(slot_addHomeScene(QByteArray)),Qt::BlockingQueuedConnection);
    sceneEngine=NULL;
}

LuxDomoSceneToolbar::~LuxDomoSceneToolbar()
{
     if(sceneEngine!=NULL)
     {
         sceneEngine->deleteLater();
     }
}

void   LuxDomoSceneToolbar::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW*9/16);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(mLMap_6410_Msg.screenW*5/16);
    parentItem()->setY(0);
}

void  LuxDomoSceneToolbar::init()
{
    QMetaObject::invokeMethod(parentItem(),"clearModel",Qt::DirectConnection);
    operatingType=LoadHomeScene;
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->start();
}

void   LuxDomoSceneToolbar::slot_addHomeScene(QByteArray  data)
{
    QString   actflag,sceneFile;
    QDataStream  out(&data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>actflag>>sceneFile;
    QStringList  image=GetSceneImage(sceneFile);
     if(image.size()!=2)
     {
         return;
     }
     QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                                           Q_ARG(QVariant,image.at(0)),
                                           Q_ARG(QVariant,image.at(1)),
                                           Q_ARG(QVariant,sceneFile),
                                           Q_ARG(QVariant,(actflag==tr("enable")?true:false)));
}


QStringList   LuxDomoSceneToolbar::GetSceneImage(QString  strFile)
{
      QStringList   data;
      data.clear();

      if(tr("house-map-user-2.xml")==strFile)
      {
           data<<tr("../setting/inhomeScene.png")<<tr("在家模式");
      }
      else  if(tr("house-map-user-6.xml")==strFile)
      {
           data<<tr("../setting/outhomeScene.png")<<tr("外出模式");
      }
      else  if(tr("house-map-user-3.xml")==strFile)
      {
           data<<tr("../setting/getupScene.png")<<tr("起床模式");
      }
      else  if(tr("house-map-user-1.xml")==strFile)
      {
           data<<tr("../setting/sleepScene.png")<<tr("睡眠模式");
      }
      else   if(tr("house-map-user-4.xml")==strFile)
      {
           data<<tr("../setting/entertainmentScene.png")<<tr("娱乐模式");
      }
      else   if(tr("house-map-user-7.xml")==strFile)
      {
           data<<tr("../setting/relaxationScene.png")<<tr("会客模式");
      }
      else   if(tr("house-map-user-5.xml")==strFile)
      {
          data<<tr("../setting/filmScene.png")<<tr("影音模式");
      }
      else   if(tr("house-map-user-8.xml")==strFile)
      {
          data<<tr("../setting/eatScene.png")<<tr("就餐模式");
      }
      return  data;
}

void   LuxDomoSceneToolbar::showHomeScene(QString  strFile,QString  name)
{
    if(g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->isRunning())
    {
        return;
    }
    if(sceneEngine!=NULL)
    {
        sceneEngine->deleteLater();
    }
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/setting_dialog/LuxDomoSceneSettingDialog.qml"));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem());
    item->setHeight(parentItem()->height());
    item->setWidth(parentItem()->width());
    QMetaObject::invokeMethod(item,"init",Qt::DirectConnection,Q_ARG(QVariant,strFile),Q_ARG(QVariant,name));
    sceneEngine=engine;
}

void   LuxDomoSceneToolbar::enableHomeScene(QString  strFile,QString  value,int  index)
{
    QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                              Q_ARG(QVariant,index),
                              Q_ARG(QVariant,tr("isEnable")),
                              Q_ARG(QVariant,(value==tr("enable")?true:false)));
     g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->EnableHomeScene(strFile,value);
}
