#include "luxdomotvedit.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;
extern   QMultiMap<QString,QDeclarativeEngine*>   engines;
LuxDomoTvEdit::LuxDomoTvEdit(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
      listEngine=NULL;
      listItem=NULL;
}

LuxDomoTvEdit::~LuxDomoTvEdit()
{
     resourceRelease();
}

void  LuxDomoTvEdit::componentComplete()
{

}

void   LuxDomoTvEdit::resourceRelease()
{
    if(listEngine!=NULL)
    {
        listEngine->deleteLater();
        listEngine=NULL;
    }
    if(listItem!=NULL)
    {
        listItem->deleteLater();
        listItem=NULL;
    }
}

void  LuxDomoTvEdit::init(QString  type, QString  deviceUid)
{
    if(tr("roomScene")==type)
    {
        m_deviceElement=g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->LoadingRoomSceneDeviceData(deviceUid);
        if(m_deviceElement.isNull()){  parentItem()->deleteLater();  }
    }
    else  if(tr("homeScene")==type)
    {
        m_deviceElement=g_pMainDlg->m_pHomeControlDlg->m_pHomeScene->LoadingHomeSceneDeviceData(deviceUid);
        if(m_deviceElement.isNull()){  parentItem()->deleteLater();  }
    }
    MyMsgLog<<m_deviceElement.attribute("mode")<<m_deviceElement.attribute("value");
    QString  mode=m_deviceElement.attribute("mode");
    if(mode!=tr("av")&&mode!=tr("tv"))
    {
        mode=tr("av");
        m_deviceElement.setAttribute("mode",mode);
    }
    parentItem()->setProperty("mode",m_deviceElement.attribute("mode"));
    parentItem()->setProperty("volume",m_deviceElement.attribute("value"));
}

void  LuxDomoTvEdit::close()
{
    parentItem()->deleteLater();
}

void  LuxDomoTvEdit::showModeList()
{
    resourceRelease();
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/content/LuxDomoListDialog.qml"));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem());
    int   pointX=parentItem()->property("pointX").toInt();
    item->setWidth(mLMap_6410_Msg.screenW*7/40);
    item->setHeight(mLMap_6410_Msg.screenH/3);
    item->setX(mLMap_6410_Msg.screenW*7/40+pointX);
    item->setY(mLMap_6410_Msg.screenH*4/9);
    item->setProperty("dataType",tr("mode"));
    QMetaObject::invokeMethod(item,"clearModel",Qt::DirectConnection);
    QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,tr("av")));
    QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,tr("tv")));
    listEngine=NULL;
    listItem=NULL;
}

void  LuxDomoTvEdit::showVolumeList()
{
    resourceRelease();
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/content/LuxDomoListDialog.qml"));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem());
    int   pointX=parentItem()->property("pointX").toInt();
    item->setWidth(mLMap_6410_Msg.screenW*7/40);
    item->setHeight(mLMap_6410_Msg.screenH/3);
    item->setX(mLMap_6410_Msg.screenW*7/40+pointX);
    item->setY(mLMap_6410_Msg.screenH*5/9);
    item->setProperty("dataType",tr("volume"));
    QMetaObject::invokeMethod(item,"clearModel",Qt::DirectConnection);
    for(int i=0;i<=10;i++)
    {
         QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,QString::number(i*10)));
    }
    listEngine=NULL;
    listItem=NULL;
}

void  LuxDomoTvEdit::save()
{
    QString   mode=parentItem()->property("mode").toString();
    QString   volume=parentItem()->property("volume").toString();
    m_deviceElement.setAttribute("mode",mode);
    m_deviceElement.setAttribute("value",volume);
    parentItem()->deleteLater();
}
