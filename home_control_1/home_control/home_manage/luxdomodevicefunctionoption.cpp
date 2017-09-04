#include "luxdomodevicefunctionoption.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   EditDataType     dataType;
extern   QMultiMap<QString,QDeclarativeEngine*>    engines;
LuxDomoDeviceFunctionOption::LuxDomoDeviceFunctionOption(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoDeviceFunctionOption::~LuxDomoDeviceFunctionOption()
{

}

void   LuxDomoDeviceFunctionOption::componentComplete()
{
          MyMsgLog<<parentItem()->objectName()<<objectName();
}

void    LuxDomoDeviceFunctionOption::close()
{
    parentItem()->hide();
    parentItem()->deleteLater();
}

void  LuxDomoDeviceFunctionOption::deleteDevice()
{
    QString   deviceUid=   parentItem()->property("deviceUid").toString();
    QMetaObject::invokeMethod(parentItem()->parentItem(),"deleteDevice",Qt::DirectConnection,Q_ARG(QVariant,deviceUid));
    parentItem()->deleteLater();
}

void   LuxDomoDeviceFunctionOption::showDeviceEdit()
{
    QString   deviceUid=   parentItem()->property("deviceUid").toString();
    QMetaObject::invokeMethod(parentItem()->parentItem(),"setOperateDevice",Qt::DirectConnection,Q_ARG(QVariant,deviceUid));
    dataType=DeviceOtherName;
    g_pMainDlg->m_pHomeControlDlg->m_editName=parentItem()->property("deviceName").toString();
    g_pMainDlg->m_pHomeControlDlg->setHCLineText(parentItem()->property("deviceName").toString());
    parentItem()->deleteLater();
}

void   LuxDomoDeviceFunctionOption::showInfraredLearn()
{
    parentItem()->hide();
    QString   type=parentItem()->property("deviceType").toString();
    if(tr("tv")==type)
    {
            TVInfraredLearn();
    }
    else    if(tr("ac")==type)
    {
            ACInfraredLearn();
    }
   else     if(tr("music")==type)
    {
            MusicInfraredLearn();
    }
   parentItem()->deleteLater();
}

void    LuxDomoDeviceFunctionOption::TVInfraredLearn()
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem()->parentItem()->parentItem());
    QDeclarativeComponent component(engine, QUrl(tr("qrc:/QML/home_manage_dialog/LuxDomoTvLearnDialog.qml")));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem()->parentItem()->parentItem());
    item->setWidth(mLMap_6410_Msg.screenW*7/8);
    item->setHeight(mLMap_6410_Msg.screenH);
    item->setX(0);
    item->setY(0);
    engines.insert(tr("roomManage"),engine);
    QString  deviceUid=parentItem()->property("deviceUid").toString();
    QMetaObject::invokeMethod(item,"init",Qt::DirectConnection,Q_ARG(QVariant,deviceUid));
}

void  LuxDomoDeviceFunctionOption::ACInfraredLearn()
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem()->parentItem()->parentItem());
    QDeclarativeComponent component(engine, QUrl(tr("qrc:/QML/home_manage_dialog/LuxDomoAcLearnDialog.qml")));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem()->parentItem()->parentItem());
    item->setWidth(mLMap_6410_Msg.screenW*7/8);
    item->setHeight(mLMap_6410_Msg.screenH);
    item->setX(0);
    item->setY(0);
    engines.insert(tr("roomManage"),engine);
    QString  deviceUid=parentItem()->property("deviceUid").toString();
    QMetaObject::invokeMethod(item,"init",Qt::DirectConnection,Q_ARG(QVariant,deviceUid));
}

void  LuxDomoDeviceFunctionOption::MusicInfraredLearn()
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem()->parentItem()->parentItem());
    QDeclarativeComponent component(engine, QUrl(tr("qrc:/QML/home_manage_dialog/LuxDomoMusicLearnDialog.qml")));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem()->parentItem()->parentItem());
    item->setWidth(mLMap_6410_Msg.screenW*7/8);
    item->setHeight(mLMap_6410_Msg.screenH);
    item->setX(0);
    item->setY(0);
    engines.insert(tr("roomManage"),engine);
    QString  deviceUid=parentItem()->property("deviceUid").toString();
    QMetaObject::invokeMethod(item,"init",Qt::DirectConnection,Q_ARG(QVariant,deviceUid));
}
