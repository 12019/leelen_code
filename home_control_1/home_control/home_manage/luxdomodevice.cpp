#include "luxdomodevice.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern MainDialog *g_pMainDlg;
extern LMap_6410_Msg mLMap_6410_Msg;
extern   QMultiMap<QString,QDeclarativeEngine*>   engines;

LuxDomoDevice::LuxDomoDevice(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoDevice::~LuxDomoDevice()
{

}

void   LuxDomoDevice::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setProperty("nameSize",mLMap_6410_Msg.controlFont);
}

void   LuxDomoDevice::showFunctionOption()
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem()->parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/home_manage_dialog/LuxDomoDeviceFunctionOptionDialog.qml"));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem()->parentItem());
    item->setWidth(mLMap_6410_Msg.screenW*7/8);
    item->setHeight(mLMap_6410_Msg.screenH);
    item->setX(0);
    item->setY(-parentItem()->parentItem()->y());
    item->setProperty("deleteButtonName",tr("删除"));
    item->setProperty("editButtonName",tr("编辑名称"));
    item->setProperty("learnButtonName",tr("红外学习"));
    QString    type=parentItem()->property("deviceType").toString();
    item->setProperty("deviceType",type);
    item->setProperty("deviceUid",parentItem()->objectName());
    item->setProperty("deviceName",parentItem()->property("deviceName").toString());
    engines.insert(tr("roomManage"),engine);

    if(type==tr("ac")||type==tr("music")||type==tr("tv"))
    {
        int   attribute=parentItem()->property("deviceAttribute").toInt();
        if(attribute==0x0D)
        {
           item->setProperty("isShowlearnButton",true);
        }
        else
        {
            item->setProperty("isShowlearnButton",false);
        }
    }
    else
    {
            item->setProperty("isShowlearnButton",false);
    }
}

void   LuxDomoDevice::recordMouseY(int  pointY)
{
         mouseY=pointY;
}

void   LuxDomoDevice::moveParent(int  pointY)
{
         parentItem()->parentItem()->setY(parentItem()->parentItem()->y()+(pointY-mouseY));
}

void   LuxDomoDevice::showControlDialog()
{
    QString  type=parentItem()->property("deviceType").toString();
    QString  roomUid=parentItem()->property("roomObjectName").toString();
    QString  deviceUid=parentItem()->objectName();
    if(tr("scene")==type)
    {
        QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem()->parentItem()->parentItem());
        QDeclarativeComponent component(engine, QUrl("qrc:/QML/home_manage_dialog/LuxDomoRoomSceneControlDialog.qml"));
        QDeclarativeItem *createItem = qobject_cast<QDeclarativeItem*>(component.create());
        createItem->setParentItem(parentItem()->parentItem()->parentItem());
        createItem->setWidth(mLMap_6410_Msg.screenW*7/8);
        createItem->setHeight(mLMap_6410_Msg.screenH);
        createItem->setX(0);
        createItem->setY(0);
        engines.insert(tr("roomManage"),engine);
        QMetaObject::invokeMethod(createItem,"init",Qt::DirectConnection,Q_ARG(QVariant,roomUid),Q_ARG(QVariant,deviceUid));
    }
}
