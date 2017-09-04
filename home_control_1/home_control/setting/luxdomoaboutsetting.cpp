#include "luxdomoaboutsetting.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
LuxDomoAboutSetting::LuxDomoAboutSetting(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoAboutSetting::~LuxDomoAboutSetting()
{

}

void   LuxDomoAboutSetting::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW*9/16);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(mLMap_6410_Msg.screenW*5/16);
    parentItem()->setY(0);
}

void   LuxDomoAboutSetting::init()
{
    QMetaObject::invokeMethod(parentItem(),"clearModel",Qt::DirectConnection);
    QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                                          Q_ARG(QVariant,tr("1:用户可远程控制家电及音乐")),
                                          Q_ARG(QVariant,tr("../setting/about_11.png")),
                                          Q_ARG(QVariant,tr("../setting/about_12.png")));
    QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                                          Q_ARG(QVariant,tr("2:用户可操作性强")),
                                          Q_ARG(QVariant,tr("../setting/about_21.png")),
                                          Q_ARG(QVariant,tr("../setting/about_22.png")));
}
