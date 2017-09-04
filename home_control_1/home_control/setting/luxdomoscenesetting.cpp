#include "luxdomoscenesetting.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   OperateType   operatingType;

LuxDomoSceneSetting::LuxDomoSceneSetting(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    m_deviceCount=0;
    m_devices.clear();
    editItem=NULL;
    editEngine=NULL;
    connect(g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI,SIGNAL(sigHomeSceneData(QByteArray)),this,SLOT(slot_addListData(QByteArray)),Qt::BlockingQueuedConnection);
    connect(this,SIGNAL(sigChangData(QByteArray)),g_pMainDlg->m_pHomeControlDlg->m_pHomeScene,SLOT(slot_changHomeSceneData(QByteArray)));
    connect(g_pMainDlg->m_pHomeControlDlg->m_pHomeScene,SIGNAL(sigChangHomeSceneData(QString,QString)),this,SLOT(slot_changListData(QString,QString)));
}

LuxDomoSceneSetting::~LuxDomoSceneSetting()
{
    if(editEngine!=NULL)
    {
       editEngine->deleteLater();
    }
}

void  LuxDomoSceneSetting::componentComplete()
{


}

void   LuxDomoSceneSetting::init(QString strFile,QString name)
{
    QMetaObject::invokeMethod(parentItem(),"clearModel",Qt::DirectConnection);
    parentItem()->setProperty("sceneName",name);
    m_deviceCount=0;
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->strFile=tr("home_control/")+strFile;
    operatingType=LoadHomeSceneData;
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->start();
    MyMsgLog<<strFile;
}

void    LuxDomoSceneSetting::slot_addListData(QByteArray  data)
{
    QString   roomObjectName,deviceObjectName,name,actflag,open,type;
    QDataStream  out(&data,QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out>>roomObjectName>>deviceObjectName>>name>>open>>actflag>>type;
    MyMsgLog<<roomObjectName<<deviceObjectName<<name<<open<<actflag<<type;
    QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                                       Q_ARG(QVariant,roomObjectName),
                                       Q_ARG(QVariant,deviceObjectName),
                                       Q_ARG(QVariant,name),
                                       Q_ARG(QVariant,open),
                                       Q_ARG(QVariant,type),
                                       Q_ARG(QVariant,actflag));

    m_devices.insert(deviceObjectName,m_deviceCount);
    m_deviceCount++;
}

void   LuxDomoSceneSetting::changSceneData(QString type, QString  roomUId,QString  deviceUid,QString  value,int index)
{
           QString   changType;
           QByteArray  data;
           QDataStream   in(&data,QIODevice::WriteOnly);
           in.setVersion(QDataStream::Qt_4_7);

           if(tr("Save")==deviceUid)
           {
                   changType=tr("Save");
                   in<<changType;
                   emit  sigChangData(data);
                   parentItem()->deleteLater();
           }
           else   if(tr("Memory")==deviceUid)
           {
                  changType=tr("Memory");
                  in<<changType;
                  emit   sigChangData(data);
           }
           else   if(tr("AKeyOperate")==deviceUid)
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
                  in<<changType<<roomUId<<deviceUid<<type<<value;
                  emit   sigChangData(data);
                  QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                                            Q_ARG(QVariant,index),
                                            Q_ARG(QVariant,tr("device_")+type),
                                            Q_ARG(QVariant,value));
           }
}

void  LuxDomoSceneSetting::close()
{
    parentItem()->deleteLater();
}

void   LuxDomoSceneSetting::editDevice(QString  deviceUid,QString deviceType)
{  MyMsgLog<<deviceUid<<deviceType;
    if(tr("ac")==deviceType)
    {
        CreateElement("qrc:/QML/home_manage_dialog/LuxDomoAcEditDialog.qml");
        QMetaObject::invokeMethod(editItem,"init",Qt::DirectConnection,Q_ARG(QVariant,tr("homeScene")),Q_ARG(QVariant,deviceUid));
    }
    else   if(tr("music")==deviceType)
    {
        CreateElement("qrc:/QML/home_manage_dialog/LuxDomoMusicEditDialog.qml");
        QMetaObject::invokeMethod(editItem,"init",Qt::DirectConnection,Q_ARG(QVariant,tr("homeScene")),Q_ARG(QVariant,deviceUid));
    }
    else   if(tr("tv")==deviceType)
    {
        CreateElement("qrc:/QML/home_manage_dialog/LuxDomoTvEditDialog.qml");
        QMetaObject::invokeMethod(editItem,"init",Qt::DirectConnection,Q_ARG(QVariant,tr("homeScene")),Q_ARG(QVariant,deviceUid));
    }
    else   if(tr("var")==deviceType)
    {
        CreateElement("qrc:/QML/home_manage_dialog/LuxDomoVarLightDialog.qml");
        QMetaObject::invokeMethod(editItem,"init",Qt::DirectConnection,Q_ARG(QVariant,tr("homeScene")),Q_ARG(QVariant,deviceUid));
    }
    else {}
}

void  LuxDomoSceneSetting::CreateElement(QString str)
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
    createItem->setX(-(mLMap_6410_Msg.screenW*5/16));
    createItem->setY(0);
    createItem->setProperty("pointX",(mLMap_6410_Msg.screenW*3/8));
    editEngine=engine;
    editItem=createItem;
}

void   LuxDomoSceneSetting::slot_changListData(QString  deviceUid, QString  value)
{
    int  index=m_devices.value(deviceUid);
    QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                              Q_ARG(QVariant,index),
                              Q_ARG(QVariant,tr("device_power")),
                              Q_ARG(QVariant,value));
}
