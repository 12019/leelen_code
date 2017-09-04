#include "luxdomomaininterface.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;
extern  OperateType   operatingType;
LuxDomoMainInterface::LuxDomoMainInterface(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    scenes.clear();
    m_scenes.clear();
    m_rooms.clear();
    connect(g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI,SIGNAL(sigMainInterfaceData(QString,QString,QString)),this,SLOT(slot_addElement(QString,QString,QString)));
    connect(g_pMainDlg->m_pHomeControlDlg->m_pHomeScene,SIGNAL(sigChangScene(QString,QString)),this,SLOT(slot_changScene(QString,QString)));
}
LuxDomoMainInterface::~LuxDomoMainInterface()
{
    disconnect(g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI,SIGNAL(sigMainInterfaceData(QString,QString,QString)),this,SLOT(slot_addElement(QString,QString,QString)));
    g_pMainDlg->m_pHomeControlDlg->mainIterface=NULL;
}
void  LuxDomoMainInterface::componentComplete()
{
    g_pMainDlg->m_pHomeControlDlg->mainIterface=this;
    MyMsgLog<<g_pMainDlg->m_pHomeControlDlg->mainIterface->objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW*7/8);
    parentItem()->setHeight(mLMap_6410_Msg.screenH*3/5);
    parentItem()->setX(mLMap_6410_Msg.screenW/8);
    parentItem()->setY(mLMap_6410_Msg.screenH*2/5);
    parentItem()->setProperty("controlH",mLMap_6410_Msg.controlH);
    parentItem()->setProperty("controlW",mLMap_6410_Msg.controlW);
    parentItem()->setProperty("pieceH",mLMap_6410_Msg.pieceH);
    parentItem()->setProperty("pieceW",mLMap_6410_Msg.pieceW);
    parentItem()->setProperty("leftMargin1",mLMap_6410_Msg.leftMargin1);
    parentItem()->setProperty("nameSize",mLMap_6410_Msg.controlFont);
}

void   LuxDomoMainInterface::Init()
{
    QMetaObject::invokeMethod(parentItem(),"clearroomModel",Qt::DirectConnection);
    QMetaObject::invokeMethod(parentItem(),"clearsceneModel",Qt::DirectConnection);
    operatingType=LoadMainInterfaceData;
    g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->start();
}

bool   LuxDomoMainInterface::FindChild(QString childName)
{
          int size = parentItem()->childItems().size();
          for(int i=0;i<size;i++)
         {
                 QDeclarativeItem *item = (QDeclarativeItem*)parentItem()->childItems().at(i);
                 if(item->objectName()==childName)
                 {
                        childItem=item;
                        return   true;
                 }
          }
         return  false;
}

void   LuxDomoMainInterface::CreateElement(QString objectName, QString name, QString type)
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/main_interface_dialog/LuxDomoSceneDialog.qml"));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
     item->setParentItem(parentItem());
     item->setObjectName(objectName);
     item->setProperty("type",type);
     if(type==tr("room"))
     {
         item->setProperty("sceneName",name);
         item->setProperty("sceneImage",g_pMainDlg->m_pHomeControlDlg->GetControlImage(type));
     }
     else   if(type==tr("scene"))
     {
         QStringList    image=g_pMainDlg->m_pHomeControlDlg->GetSceneImage(objectName);
         if(image.size()!=2)  {  return;  }
         item->setProperty("sceneImage",image.at(0));
         item->setProperty("sceneName",image.at(1));
     }
}

void   LuxDomoMainInterface::setSceneLocation(QString sceneName)
{
    int  number=scenes.value(sceneName);
    int  x=-1,y=-1;

    x=number%5;
    y=number/5;
    if(0==x)  {  x=4;   y-=1; }
    else   {  x-=1;  }

    if(FindChild(sceneName)==false)  {   return; }
    childItem->setX(x*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin1);
    childItem->setY(y*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin1);
}

void  LuxDomoMainInterface::AddSceneContainerHeight()
{
    int size =scenes.size()+1;
    if(size%5!=0) { return ; }
    if(size/5<3)  { return; }
    parentItem()->setHeight(parentItem()->height()+mLMap_6410_Msg.pieceH);
}

void  LuxDomoMainInterface::DeleteSceneContainerHeight()
{
    int size=scenes.size();
    if(size%5!=0)  { return;}
    if(size/5<3)     { return;}
    parentItem()->setHeight(parentItem()->height()-mLMap_6410_Msg.pieceH);
}

void   LuxDomoMainInterface::slot_addElement(QString objectName, QString name, QString type)
{
    if(type==tr("room"))
    {
        QString   image=g_pMainDlg->m_pHomeControlDlg->GetControlImage(type);
        QMetaObject::invokeMethod(parentItem(),"addroomModel",Qt::DirectConnection,
                                              Q_ARG(QVariant,objectName),
                                              Q_ARG(QVariant,image),
                                              Q_ARG(QVariant,name));
        int   size=m_rooms.size();
        m_rooms.insert(objectName,size);
    }
    else   if(type==tr("scene"))
    {
        QStringList    image=g_pMainDlg->m_pHomeControlDlg->GetSceneImage(objectName);
        if(image.size()!=2)  {  return;  }
        QMetaObject::invokeMethod(parentItem(),"addsceneModel",Qt::DirectConnection,
                                              Q_ARG(QVariant,objectName),
                                              Q_ARG(QVariant,image.at(0)),
                                              Q_ARG(QVariant,image.at(1)));
        int  size=m_scenes.size();
        m_scenes.insert(objectName,size);
    }
}

void   LuxDomoMainInterface::slot_deleteElement(QString objectName)
{
    int   index=m_rooms.value(objectName);
    QMetaObject::invokeMethod(parentItem(),"removeroomModel",Qt::DirectConnection,Q_ARG(QVariant,index));
    m_rooms.remove(objectName);

    QMap<QString,int>::iterator  it=m_rooms.begin();
    for(;it!=m_rooms.end();it++)
    {
        if(it.value()>index)
        {
            m_rooms.insert(it.key(),it.value()-1);
        }
    }
}

void   LuxDomoMainInterface::slot_changScene(QString  objectName,QString  value)
{
    if(value==tr("enable"))
    {
         slot_addElement(objectName,tr(""),tr("scene"));
    }
    else
    {
        int   index=m_scenes.value(objectName);
        QMetaObject::invokeMethod(parentItem(),"removesceneModel",Qt::DirectConnection,Q_ARG(QVariant,index));
        m_scenes.remove(objectName);

        QMap<QString,int>::iterator  it=m_scenes.begin();
        for(;it!=m_scenes.end();it++)
        {
            if(it.value()>index)
            {
                m_scenes.insert(it.key(),it.value()-1);
            }
        }
    }
}

void    LuxDomoMainInterface::slot_changName(QString  objectName, QString  name)
{
    int   index=m_rooms.value(objectName);
    QMetaObject::invokeMethod(parentItem(),"changroomModel",Qt::DirectConnection,
                              Q_ARG(QVariant,index),
                              Q_ARG(QVariant,tr("roomName")),
                              Q_ARG(QVariant,name));
}

void  LuxDomoMainInterface::showRoomControl(QString  objectName)
{
    if(g_pMainDlg->m_pHomeControlDlg->roomControl==NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->CreateRoomControl();
        if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
        {
            g_pMainDlg->m_pHomeControlDlg->roomControl->init(objectName);
        }
    }
}

void   LuxDomoMainInterface::useHomeScene(QString strFile)
{
    if(strFile==tr("house-map-user-on.xml"))
    {
        char strCmd[6];
        strCmd[0]=0x06;    //字节长度
        strCmd[1]=0xff; //目的地址设备号
        strCmd[2]=0xff;  //源地址
        strCmd[3]=0xff;   //设备属性
        strCmd[4]=0x01;
        strCmd[5]=0;
        g_pMainDlg->m_pHomeControlDlg->m_pApplyScene->AllOnAndOff(tr("on"));
        g_pMainDlg->m_pHomeControlDlg->cmdBuffer(strCmd);
    }
    else  if(strFile==tr("house-map-user-off.xml"))
    {
        char strCmd[6];
        strCmd[0]=0x06;    //字节长度
        strCmd[1]=0xff; //目的地址设备号
        strCmd[2]=0xff;  //源地址
        strCmd[3]=0xff;   //设备属性
        strCmd[4]=0x00;
        strCmd[5]=0;
        g_pMainDlg->m_pHomeControlDlg->m_pApplyScene->AllOnAndOff(tr("off"));
        g_pMainDlg->m_pHomeControlDlg->cmdBuffer(strCmd);
    }
    else
    {
        operatingType=HomeScenePlaying;
        g_pMainDlg->m_pHomeControlDlg->m_pApplyScene->m_strFile=tr("home_control/")+strFile;
        g_pMainDlg->m_pHomeControlDlg->m_pApplyScene->start();
    }
}
