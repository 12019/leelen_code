#include "luxdomodeviceoption.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   EditDataType     dataType;
extern   DeviceTypes      deviceTypes;
LuxDomoDeviceOption::LuxDomoDeviceOption(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    listEngine=NULL;
    listItem=NULL;
}

LuxDomoDeviceOption::~LuxDomoDeviceOption()
{
    if(listEngine!=NULL)
    {
        listEngine->deleteLater();
        listEngine=NULL;
    }
}

void   LuxDomoDeviceOption::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setProperty("optionType",tr("不可调灯"));
    parentItem()->setProperty("optionAddress",tr("1"));
    parentItem()->setProperty("optionAttribute",tr("1"));
    parentItem()->setProperty("optionName","新设备");
    showDeviceTypeList();
}

void   LuxDomoDeviceOption::resourceRelease()
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

void   LuxDomoDeviceOption::showPullDownList(QString  type)
{
    if(type==tr("typeButton"))
    {
           showDeviceTypeList();
    }
    else   if(type==tr("addressButton"))
    {
          showDeviceAddressList();
    }
    else   if(type==tr("attributeButton"))
    {
           showDeviceAttributeList();
    }
}


void    LuxDomoDeviceOption::showDeviceAddressList()
{
    resourceRelease();
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/content/LuxDomoPullDownListDialog.qml"));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem());
    item->setWidth(mLMap_6410_Msg.screenW*9/16);
    item->setHeight(mLMap_6410_Msg.screenH*7/8);
    item->setX(mLMap_6410_Msg.screenW*5/16);
    item->setY(mLMap_6410_Msg.screenH/8);
    item->setProperty("listTitle",tr("设备地址选择"));
    item->setProperty("dataType",tr("optionAddress"));
    QMetaObject::invokeMethod(item,"clearModel",Qt::DirectConnection);
    int i=1;
    if(parentItem()->property("optionAttribute").toInt()==21||parentItem()->property("optionAttribute").toInt()==22)
    {
       i=0;
    }
    for(;i<240;i++)
    {

        QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,i));
    }
    listEngine=engine;
    listItem=item;
}

void    LuxDomoDeviceOption::showDeviceAttributeList()
{
    resourceRelease();
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/content/LuxDomoPullDownListDialog.qml"));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem());
    item->setWidth(mLMap_6410_Msg.screenW*9/16);
    item->setHeight(mLMap_6410_Msg.screenH*7/8);
    item->setX(mLMap_6410_Msg.screenW*5/16);
    item->setY(mLMap_6410_Msg.screenH/8);
    item->setProperty("listTitle",tr("设备属性选择"));
    item->setProperty("dataType",tr("optionAttribute"));
    QMetaObject::invokeMethod(item,"clearModel",Qt::DirectConnection);

    QString   type=parentItem()->property("optionType").toString();
    if(type==deviceTypes.type.at(0)||type==deviceTypes.type.at(2)||type==deviceTypes.type.at(4))
    {
       QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,13));
    }
    else  if(type==deviceTypes.type.at(1))
    {
       QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,21));
    }
    else  if(type==deviceTypes.type.at(3))
    {
        QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,22));
    }
    else  if(type==deviceTypes.type.at(6))
    {
        QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,24));
    }
    else   if(type==deviceTypes.type.at(7))
    {
        QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,9));
        QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,10));
        QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,11));
        QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,12));
    }
    else   if(type==deviceTypes.type.at(10))
    {
        QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,15));
    }
    else   if(type==deviceTypes.type.at(11))
    {
        QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,20));
    }
   else
    {
        for(int  i=1;i<9;i++)
        {
             QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,i));
        }
    }
    listEngine=engine;
    listItem=item;
}

void   LuxDomoDeviceOption::showDeviceTypeList()
{
    resourceRelease();
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/content/LuxDomoStyleListDialog.qml"));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem());
    item->setWidth(mLMap_6410_Msg.screenW*9/16);
    item->setHeight(mLMap_6410_Msg.screenH*7/8);
    item->setX(mLMap_6410_Msg.screenW*5/16);
    item->setY(mLMap_6410_Msg.screenH/8);
    item->setProperty("listTitle",tr("设备类型选择"));
    QMetaObject::invokeMethod(item,"clearModel",Qt::DirectConnection);
    int  size=deviceTypes.typeEn.size();
    for(int i=0;i<size;i++)
    {
        QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection,
                                   Q_ARG(QVariant,GetControlImage(deviceTypes.typeEn.at(i))),
                                   Q_ARG(QVariant,deviceTypes.type.at(i)));
    }
    listEngine=engine;
    listItem=item;
}

void  LuxDomoDeviceOption::setTypeData(QString  data)
{
    parentItem()->setProperty("optionType",data);
    if(data==deviceTypes.type.at(0)||data==deviceTypes.type.at(2)||data==deviceTypes.type.at(4))
    {
       parentItem()->setProperty("optionAttribute",QString::number(13));
    }
    else  if(data==deviceTypes.type.at(1))
    {
       parentItem()->setProperty("optionAttribute",QString::number(21));
    }
    else  if(data==deviceTypes.type.at(3))
    {
         parentItem()->setProperty("optionAttribute",QString::number(22));
    }
    else  if(data==deviceTypes.type.at(6))
    {
         parentItem()->setProperty("optionAttribute",QString::number(24));
    }
    else   if(data==deviceTypes.type.at(7))
    {
        parentItem()->setProperty("optionAttribute",QString::number(9));
    }
    else   if(data==deviceTypes.type.at(10))
    {
        parentItem()->setProperty("optionAttribute",QString::number(15));
    }
    else   if(data==deviceTypes.type.at(11))
    {
        parentItem()->setProperty("optionAttribute",QString::number(20));
    }
    parentItem()->setProperty("optionName",data);
}

void   LuxDomoDeviceOption::ensure()
{
    parentItem()->hide();
    QString   type=parentItem()->property("optionType").toString();
    type=g_pMainDlg->m_pHomeControlDlg->GetDeviceTypeName(type);
    if(type==tr(""))
    {
        return;
    }
    QString  address=parentItem()->property("optionAddress").toString();
    QString  attribute=parentItem()->property("optionAttribute").toString();
    QString  name=parentItem()->property("optionName").toString();
    QMetaObject::invokeMethod(parentItem()->parentItem(),"addDevice",Qt::DirectConnection,
                                Q_ARG(QVariant,type),
                                Q_ARG(QVariant,address),
                                Q_ARG(QVariant,attribute),
                                Q_ARG(QVariant,name));
   parentItem()->deleteLater();
}

void  LuxDomoDeviceOption::inputData(QString type,QString data)
{
    if(type=="DeviceName")
    {
              dataType=DeviceName;
    }
   else   if(type=="DeviceAddress")
             {
                   dataType=DeviceAdress;
             }
   else   if(type=="DeviceAttribute")
            {
                  dataType=DeviceAttribute;
            }
   g_pMainDlg->m_pHomeControlDlg->m_editName=data;
   g_pMainDlg->m_pHomeControlDlg->setHCLineText(data);
}

void   LuxDomoDeviceOption::close()
{
          parentItem()->deleteLater();
}

QString   LuxDomoDeviceOption::GetControlImage(QString  type)
{
    QString  strImage;
    if(tr("scene")==type)
    {
        strImage=tr("../home_manage/scene_on.png");
    }
    else   if(tr("tv")==type)
    {
        strImage=tr("../home_manage/tv_on.png");
    }
    else   if(tr("ac")==type)
    {
        strImage=tr("../home_manage/ac_on.png");
    }
    else   if(tr("curtain")==type)
    {
        strImage=tr("../home_manage/curtain_on.png");
    }
    else    if(tr("var")==type)
    {
        strImage=tr("../home_manage/var_on.png");
    }
    else    if(tr("light")==type)
    {
        strImage=tr("../home_manage/light_on.png");
    }
    else   if(tr("music")==type)
    {
        strImage=tr("../home_manage/music_on.png");
    }
    else   if(tr("humiture")==type)
    {
        strImage=tr("../home_manage/humiture_on.png");
    }
    else   if(tr("pm")==type)
    {
        strImage=tr("../home_manage/pm_on.png");
    }
    else   if(tr("custom")==type)
    {
        strImage=tr("../room_control/custom_on.png");
    }
    else   if(tr("humansensor")==type)
    {
        strImage=tr("../room_control/humansensor_on.png");
    }

    return   strImage;
}
