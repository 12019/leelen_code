#include "luxdomovarlight.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;
extern   QMultiMap<QString,QDeclarativeEngine*>   engines;
LuxDomoVarLight::LuxDomoVarLight(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    listItem=NULL;
    listEngine=NULL;
}

LuxDomoVarLight::~LuxDomoVarLight()
{
     resourceRelease();
}

void LuxDomoVarLight::componentComplete()
{

}

void   LuxDomoVarLight::resourceRelease()
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

void   LuxDomoVarLight::init(QString  type, QString deviceUid)
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
    MyMsgLog<<m_deviceElement.attribute("value");
    parentItem()->setProperty("volume",m_deviceElement.attribute("value"));
}


void   LuxDomoVarLight::close()
{
    parentItem()->deleteLater();
}

void    LuxDomoVarLight::showVolumeList()
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
    item->setProperty("dataType",tr("volume"));
    QMetaObject::invokeMethod(item,"clearModel",Qt::DirectConnection);
    for(int i=0;i<=10;i++)
    {
         QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,QString::number(i*10)));
    }
    listEngine=NULL;
    listItem=NULL;
}

void   LuxDomoVarLight::save()
{
    QString   volume=parentItem()->property("volume").toString();
    m_deviceElement.setAttribute("value",volume);
    parentItem()->deleteLater();
}
