#include "luxdomomusicedit.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;
extern  MusicInfo   musicInfo;
extern   QMultiMap<QString,QDeclarativeEngine*>   engines;
LuxDomoMusicEdit::LuxDomoMusicEdit(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    listEngine=NULL;
    listItem=NULL;
}

LuxDomoMusicEdit::~LuxDomoMusicEdit()
{
    resourceRelease();
}

void  LuxDomoMusicEdit::componentComplete()
{

}

void   LuxDomoMusicEdit::resourceRelease()
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

void  LuxDomoMusicEdit::init(QString  type, QString  deviceUid)
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
    MyMsgLog<<m_deviceElement.attribute("source")<<m_deviceElement.attribute("value")<<m_deviceElement.attribute("open");
    parentItem()->setProperty("mode",m_deviceElement.attribute("source"));
    parentItem()->setProperty("volume",m_deviceElement.attribute("value"));
    if(m_deviceElement.attribute("play")==tr("on"))
    {
         parentItem()->setProperty("condition",tr("播放"));
    }
    else
    {
        parentItem()->setProperty("condition",tr("暂停"));
    }
}

void   LuxDomoMusicEdit::close()
{
    parentItem()->deleteLater();
}

void   LuxDomoMusicEdit::showModeList()
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
    item->setY(mLMap_6410_Msg.screenH*7/18);
    item->setProperty("dataType",tr("mode"));
    QMetaObject::invokeMethod(item,"clearModel",Qt::DirectConnection);
    int deviceId=m_deviceElement.attribute("id").toInt();
    int m_acAttr=(deviceId>>8) & 0xff;
    if(m_acAttr==BackAudioMusic)
    {
        int  size=musicInfo.musicBackMode.size();
        for(int i=0;i<size;i++)
        {
            QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant, musicInfo.musicBackMode.at(i)));
        }
    }
    else  if(m_acAttr==Music_Infrared)
    {
        int  size=musicInfo.musicInfraredMode.size();
        for(int i=0;i<size;i++)
        {
            QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,musicInfo.musicInfraredMode.at(i)));
        }
    }
    listEngine=NULL;
    listEngine=NULL;
}

void  LuxDomoMusicEdit::showVolumeList()
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
    item->setY(mLMap_6410_Msg.screenH*9/18);
    item->setProperty("dataType",tr("volume"));
    QMetaObject::invokeMethod(item,"clearModel",Qt::DirectConnection);
    for(int i=0;i<11;i++)
    {
        QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,QString::number(i*10)));
    }
    listEngine=NULL;
    listEngine=NULL;
}

void  LuxDomoMusicEdit::showConditionList()
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
    item->setY(mLMap_6410_Msg.screenH*11/18);
    item->setProperty("dataType",tr("condition"));
    QMetaObject::invokeMethod(item,"clearModel",Qt::DirectConnection);
    QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,tr("播放")));
    QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,tr("暂停")));
    listEngine=NULL;
    listEngine=NULL;
}

void  LuxDomoMusicEdit::save()
{
    QString  mode=parentItem()->property("mode").toString();
    QString  volume=parentItem()->property("volume").toString();
    QString  condition=parentItem()->property("condition").toString();
    m_deviceElement.setAttribute("source",mode);
    m_deviceElement.setAttribute("value",volume);
    if(tr("播放")==condition)
    {
          m_deviceElement.setAttribute("play",tr("on"));
    }
    else
    {
          m_deviceElement.setAttribute("play",tr("off"));
    }
    parentItem()->deleteLater();
}
