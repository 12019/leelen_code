#include "luxdomoacedit.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;
extern  AcInfo         acInfo;
extern   QMultiMap<QString,QDeclarativeEngine*>   engines;
LuxDomoAcEdit::LuxDomoAcEdit(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    listEngine=NULL;
    listItem=NULL;
}

LuxDomoAcEdit::~LuxDomoAcEdit()
{
     resourceRelease();
}

void  LuxDomoAcEdit::componentComplete()
{

}

void  LuxDomoAcEdit::init(QString  type,QString  deviceUid)
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
    MyMsgLog<<m_deviceElement.attribute("wind")<<m_deviceElement.attribute("mode")<<m_deviceElement.attribute("swing");
    parentItem()->setProperty("mode",g_pMainDlg->m_pHomeControlDlg->GetAcModeName(m_deviceElement.attribute("mode")));
    parentItem()->setProperty("temp",m_deviceElement.attribute("temperature"));
    int deviceId=m_deviceElement.attribute("id").toInt();
    m_acAddr=deviceId & 0xff;
    m_acAttr=(deviceId>>8) & 0xff;
    QString  wind=m_deviceElement.attribute("swing");
    QString  speed=m_deviceElement.attribute("wind");

    if(m_acAttr==Ac_Infrared)
    {
        if(tr("on")==wind)
        {
            wind=tr("摆风");
        }
        else
        {
            wind=tr("固定");
        }
        parentItem()->setProperty("wind",wind);
        parentItem()->setProperty("speed",g_pMainDlg->m_pHomeControlDlg->GetAcInfraredSpeedName(speed));
    }
    else  if(m_acAttr==Central_AC)
    {
        parentItem()->setProperty("wind",g_pMainDlg->m_pHomeControlDlg->GetAcCentralWindName(wind));
        parentItem()->setProperty("speed",g_pMainDlg->m_pHomeControlDlg->GetAcCentralSpeedName(speed));
    }
}

void   LuxDomoAcEdit::resourceRelease()
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

void  LuxDomoAcEdit::showModeList()
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
    item->setY(mLMap_6410_Msg.screenH*3/9);
    item->setProperty("dataType",tr("mode"));
    QMetaObject::invokeMethod(item,"clearModel",Qt::DirectConnection);
    int    size=acInfo.acMode.size();
    for(int i=0;i<size;i++)
    {    QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant, acInfo.acMode.at(i))); }
    listItem=item;
    listEngine=engine;
}

void  LuxDomoAcEdit::showWindList()
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
    item->setY(mLMap_6410_Msg.screenH*6/9);
    item->setProperty("dataType",tr("wind"));
    QMetaObject::invokeMethod(item,"clearModel",Qt::DirectConnection);
    if(m_acAttr==Central_AC)
    {
        int  size=acInfo.acCentralWind.size();
        for(int i=0;i<size;i++)
         {    QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant, acInfo.acCentralWind.at(i))); }
    }
    else  if(m_acAttr==Ac_Infrared)
    {
         QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant, tr("摆风")));
         QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant, tr("固定")));
    }
    listItem=item;
    listEngine=engine;
}

void  LuxDomoAcEdit::showTempList()
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
    item->setProperty("dataType",tr("temp"));
    QMetaObject::invokeMethod(item,"clearModel",Qt::DirectConnection);
    for(int i=16;i<33;i++)
    {    QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,QString::number(i))); }
    listItem=item;
    listEngine=engine;
}

void  LuxDomoAcEdit::showSpeedList()
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
    item->setProperty("dataType",tr("speed"));
    QMetaObject::invokeMethod(item,"clearModel",Qt::DirectConnection);
    if(m_acAttr==Central_AC)
    {
        int  size=acInfo.acCentralSpeed.size();
        for(int i=0;i<size;i++)
         {    QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant, acInfo.acCentralSpeed.at(i))); }
    }
    else  if(m_acAttr==Ac_Infrared)
    {
        int  size=acInfo.acInfraredSpeed.size();
        for(int i=0;i<size;i++)
         {    QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant, acInfo.acInfraredSpeed.at(i))); }
    }
    listItem=item;
    listEngine=engine;
}

void  LuxDomoAcEdit::close()
{
    parentItem()->deleteLater();
}

void  LuxDomoAcEdit::save()
{     
    QString  mode=parentItem()->property("mode").toString();
    QString  wind=parentItem()->property("wind").toString();
    QString  speed=parentItem()->property("speed").toString();
    QString  temp=parentItem()->property("temp").toString();

    m_deviceElement.setAttribute("mode",g_pMainDlg->m_pHomeControlDlg->GetAcModeName(mode));
    m_deviceElement.setAttribute("temperature",temp);
    if(m_acAttr==Ac_Infrared)
    {
        if(wind==tr("摆风"))
        {
            m_deviceElement.setAttribute("swing",tr("on"));
        }
        else
        {
            m_deviceElement.setAttribute("swing",tr("off"));
        }
        m_deviceElement.setAttribute("wind",g_pMainDlg->m_pHomeControlDlg->GetAcInfraredSpeedName(speed));
    }
    else  if(m_acAttr==Central_AC)
    {
        m_deviceElement.setAttribute("swing",g_pMainDlg->m_pHomeControlDlg->GetAcCentralWindName(wind));
        m_deviceElement.setAttribute("wind",g_pMainDlg->m_pHomeControlDlg->GetAcCentralSpeedName(speed));
    }
    parentItem()->deleteLater();
}


