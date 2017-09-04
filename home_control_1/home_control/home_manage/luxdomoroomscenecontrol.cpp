#include "luxdomoroomscenecontrol.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   OperateType   operatingType;

LuxDomoRoomSceneControl::LuxDomoRoomSceneControl(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    connect(g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI,SIGNAL(sigRoomSceneData(QByteArray)),this,SLOT(slot_addListData(QByteArray)),Qt::BlockingQueuedConnection);
    connect(this,SIGNAL(sigChangData(QByteArray)),g_pMainDlg->m_pHomeControlDlg->m_pHomeScene,SLOT(slot_changRoomSceneData(QByteArray)));
    connect(g_pMainDlg->m_pHomeControlDlg->m_pHomeScene,SIGNAL(sigChangRoomSceneData(QString,QString)),this,SLOT(slot_changListData(QString,QString)));
    m_roomObjectName=tr("");
    m_sceneObjectName=tr("");
    editEngine=NULL;
    editItem=NULL;
    m_deviceCount=0;
    m_devices.clear();
}

LuxDomoRoomSceneControl::~LuxDomoRoomSceneControl()
{
    if(editEngine!=NULL)
    {
       editEngine->deleteLater();
    }
}

void   LuxDomoRoomSceneControl::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
}

void   LuxDomoRoomSceneControl::init(QString  roomObjectName, QString  sceneObjectName)
{
    m_roomObjectName=roomObjectName;
    m_sceneObjectName=sceneObjectName;
    QMetaObject::invokeMethod(parentItem(),"clearModel",Qt::DirectConnection);
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->roomUid=m_roomObjectName;
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->sceneUid=m_sceneObjectName;
    operatingType=LoadRoomSceneData;
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->start();
}

void  LuxDomoRoomSceneControl::slot_addListData(QByteArray data)
{
    QString  deviceObjectName,name,actflag,power,type;
    QDataStream  out(&data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>deviceObjectName>>name>>power>>actflag>>type;
    MyMsgLog<<deviceObjectName<<name<<power<<actflag<<type;
    QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                                              Q_ARG(QVariant,deviceObjectName),
                                              Q_ARG(QVariant,name),
                                              Q_ARG(QVariant,power),
                                              Q_ARG(QVariant,actflag),
                                              Q_ARG(QVariant,type));

    m_devices.insert(deviceObjectName,m_deviceCount);
    m_deviceCount++;
}

void   LuxDomoRoomSceneControl::changSceneData(QString type, QString  uid,QString  value,int index)
{
           QString   changType;
           QByteArray  data;
           QDataStream   in(&data,QIODevice::WriteOnly);
           in.setVersion(QDataStream::Qt_4_7);

           if(tr("Save")==uid)
           {
                   changType=tr("Save");
                   in<<changType;
                   emit  sigChangData(data);
                   parentItem()->deleteLater();
           }
           else   if(tr("Memory")==uid)
           {
                  changType=tr("Memory");
                  in<<changType;
                  emit   sigChangData(data);
           }
           else   if(tr("AKeyOperate")==uid)
           {
                  changType=tr("AKeyOperate");
                  in<<changType<<type<<value;
                  emit   sigChangData(data);
                  for(int i=0;i<m_deviceCount;i++)
                  {
                      QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                                                Q_ARG(QVariant,i),
                                                Q_ARG(QVariant,tr("device_")+type),
                                                Q_ARG(QVariant,value));
                  }
           }
           else
           {
                  changType=tr("SingleOperate");
                  in<<changType<<uid<<type<<value;
                  emit   sigChangData(data);
                  QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                                            Q_ARG(QVariant,index),
                                            Q_ARG(QVariant,tr("device_")+type),
                                            Q_ARG(QVariant,value));
           }
}

void   LuxDomoRoomSceneControl::editDevice(QString  deviceUid,QString deviceType)
{
    MyMsgLog<<deviceUid<<deviceType;
    if(tr("ac")==deviceType)
    {
        CreateElement("qrc:/QML/home_manage_dialog/LuxDomoAcEditDialog.qml");
        QMetaObject::invokeMethod(editItem,"init",Qt::DirectConnection,Q_ARG(QVariant,tr("roomScene")),Q_ARG(QVariant,deviceUid));
    }
    else   if(tr("music")==deviceType)
    {
        CreateElement("qrc:/QML/home_manage_dialog/LuxDomoMusicEditDialog.qml");
        QMetaObject::invokeMethod(editItem,"init",Qt::DirectConnection,Q_ARG(QVariant,tr("roomScene")),Q_ARG(QVariant,deviceUid));
    }
    else   if(tr("tv")==deviceType)
    {
        CreateElement("qrc:/QML/home_manage_dialog/LuxDomoTvEditDialog.qml");
        QMetaObject::invokeMethod(editItem,"init",Qt::DirectConnection,Q_ARG(QVariant,tr("roomScene")),Q_ARG(QVariant,deviceUid));
    }
    else   if(tr("var")==deviceType)
    {
        CreateElement("qrc:/QML/home_manage_dialog/LuxDomoVarLightDialog.qml");
        QMetaObject::invokeMethod(editItem,"init",Qt::DirectConnection,Q_ARG(QVariant,tr("roomScene")),Q_ARG(QVariant,deviceUid));
    }
    else {}
}

void  LuxDomoRoomSceneControl::CreateElement(QString str)
{
    if(editEngine!=NULL)
    {
       editEngine->deleteLater();
    }
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem()->parentItem());
    QDeclarativeComponent component(engine, QUrl(str));
    QDeclarativeItem *createItem = qobject_cast<QDeclarativeItem*>(component.create());
    createItem->setParentItem(parentItem()->parentItem());
    createItem->setWidth(mLMap_6410_Msg.screenW*7/8);
    createItem->setHeight(mLMap_6410_Msg.screenH);
    createItem->setX(0);
    createItem->setY(0);
    createItem->setProperty("pointX",(mLMap_6410_Msg.screenW*7/32));
    editItem=createItem;
    editEngine=engine;
}

void    LuxDomoRoomSceneControl::slot_changListData(QString  deviceUid, QString  value)
{
    int   index=m_devices.value(deviceUid);
    QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                              Q_ARG(QVariant,index),
                              Q_ARG(QVariant,tr("device_power")),
                              Q_ARG(QVariant,value));
}

void    LuxDomoRoomSceneControl::close()
{    
    parentItem()->deleteLater();
}
