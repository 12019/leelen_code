#include "luxdomoroomdevice.h"
#include "luxdomoroomcontrolpage.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;
extern  OperateType   operatingType;
extern  QMultiMap<QString,QDeclarativeEngine*>   engines;
LuxDomoRoomDevice::LuxDomoRoomDevice(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    pointX=0;
    pointY=0;
    oldPageName=tr("NULL");
    controlItem=NULL;
    fromFolder=false;
}

LuxDomoRoomDevice::~LuxDomoRoomDevice()
{

}

void  LuxDomoRoomDevice::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    g_pMainDlg->m_pHomeControlDlg->roomControl->deviceControls.append(parentItem());
    MyMsgLog<< g_pMainDlg->m_pHomeControlDlg->roomControl->deviceControls.size();
    parentItem()->setProperty("nameSize",mLMap_6410_Msg.controlFont);
}

int   LuxDomoRoomDevice::TransformPointX(int  pointX)
{
       int    x=-1;
       pointX=pointX+mLMap_6410_Msg.controlW/2;

       for(int  i=0;i<mLMap_6410_Msg.xPiece;i++)
       {
               if(pointX>=(i*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin)&&pointX<((i+1)*mLMap_6410_Msg.pieceW+mLMap_6410_Msg.leftMargin))
               {        x=i;       }
       }

       return  x;
}

int   LuxDomoRoomDevice::TransformPointY(int  pointY)
{
        int   y=-1;
        pointY=pointY+mLMap_6410_Msg.controlH/2;

        for(int  j=0;j<mLMap_6410_Msg.yPiece;j++)
        {
               if(pointY>=(j*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin)&&pointY<((j+1)*mLMap_6410_Msg.pieceH+mLMap_6410_Msg.topMargin))
               {         y=j;      }
        }

       return  y;
}

void   LuxDomoRoomDevice::DealError()
{
      int  x=TransformPointX(pointX);
      int  y=TransformPointY(pointY);
      g_pMainDlg->m_pHomeControlDlg->roomControl->ShowPage(oldPageName);
      QVectorIterator<LuxDomoRoomControlPage*>   it(g_pMainDlg->m_pHomeControlDlg->roomControlPages);

    while(it.hasNext())
    {
           LuxDomoRoomControlPage   *temp=it.next();
           if(QString::compare(temp->parentItem()->objectName(),oldPageName)==0)
           {
                     temp->AddDeviceControl(parentItem(),x,y);
                     temp->Printf();
                     return;
            }
     }
}

void  LuxDomoRoomDevice::rememberXY()
{
    pointX=parentItem()->x();
    pointY=parentItem()->y();
}

void   LuxDomoRoomDevice::moveStart()
{
           QString    pageName=parentItem()->parentItem()->objectName();
           oldPageName=pageName;
           QVectorIterator<LuxDomoRoomControlPage*>   it(g_pMainDlg->m_pHomeControlDlg->roomControlPages);
           while(it.hasNext())
           {
                   LuxDomoRoomControlPage   *temp=it.next();
                  if(QString::compare(temp->parentItem()->objectName(),pageName)==0)
                  {
                                   temp->RemoveDevice(parentItem()->objectName());
                                   temp->Printf();
                                   break;
                   }
           }
          parentItem()->setParentItem(g_pMainDlg->m_pHomeControlDlg->roomControl->parentItem());
          parentItem()->setProperty("inwhich",tr("room"));
}

void    LuxDomoRoomDevice::moveStop(int  pointX, int  pointY)
{
         int  x=TransformPointX(pointX);
         int  y=TransformPointY(pointY);

         if(-1==x||-1==y)  //移动出错处理
         {
             if(fromFolder==false)
             {
                 this->DealError();
                 return;
             }
             else  if(fromFolder==true)
             {
                 x=0;
                 y=0;
                 fromFolder=false;
            }
         }

        QString    pageName= g_pMainDlg->m_pHomeControlDlg->roomControl->runningPageName;
        QVectorIterator<LuxDomoRoomControlPage*>   it(g_pMainDlg->m_pHomeControlDlg->roomControlPages);

         while(it.hasNext())
         {
                LuxDomoRoomControlPage   *temp=it.next();
                if(QString::compare(temp->parentItem()->objectName(),pageName)==0)
                {
                          if(temp->DealDeviceMove(parentItem(),x,y)==false)
                           {
                                     this->DealError();
                           }
                          temp->Printf();
                          return;
                 }
          }
}

void    LuxDomoRoomDevice::isChangPage(int pointX)
{
    if(pointX<0)
    {
          g_pMainDlg->m_pHomeControlDlg->roomControl->rightMovePage();
    }
   else  if(pointX>mLMap_6410_Msg.screenW-mLMap_6410_Msg.controlW)
    {
          g_pMainDlg->m_pHomeControlDlg->roomControl->leftMovePage();
     }
}

void    LuxDomoRoomDevice::otherMoveStart(int  pointX,int pointY)
{ 
    int   showAreaW=3*(mLMap_6410_Msg.controlW+3*mLMap_6410_Msg.interval)+3*mLMap_6410_Msg.interval;
    int   showAreaH=3*(mLMap_6410_Msg.controlH+3*mLMap_6410_Msg.interval)+3*mLMap_6410_Msg.interval;
    int   showAreaX=(mLMap_6410_Msg.screenW-showAreaW)/2;
    int   showAreaY=(mLMap_6410_Msg.screenH-showAreaH)/2;
    if(pointX<showAreaX||pointY<showAreaY||pointX>(showAreaX+showAreaW)||pointY>(showAreaY+showAreaH))
    {
        fromFolder=true;
        DeviceMoveOutFolder();
    }
}

void   LuxDomoRoomDevice::DeviceMoveOutFolder()
{
      QDeclarativeItem  *folderPage=parentItem()->parentItem();
      QDeclarativeItem  *folder=parentItem()->parentItem()->parentItem();
      parentItem()->setParentItem(g_pMainDlg->m_pHomeControlDlg->roomControl->parentItem());
      parentItem()->setProperty("inwhich",tr("room"));
     QMetaObject::invokeMethod(folder,"deleteDeviceControl", Qt::DirectConnection,Q_ARG(QVariant,parentItem()->objectName()));
     QMetaObject::invokeMethod(folderPage,"deleteDeviceControl", Qt::QueuedConnection,Q_ARG(QVariant,parentItem()->objectName()));
}


void  LuxDomoRoomDevice::otherMoveStop()
{
    parentItem()->setX(pointX);
    parentItem()->setY(pointY);
}

void   LuxDomoRoomDevice::recordMouseX(int  x)
{
          mouseX=x;
}

void  LuxDomoRoomDevice::moveParent(int x)
{
         parentItem()->parentItem()->setX(parentItem()->parentItem()->x()+(x-mouseX));
}

void  LuxDomoRoomDevice::changPage()
{
    if(parentItem()->parentItem()->x()>=150&&g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
    {
              g_pMainDlg->m_pHomeControlDlg->roomControl->rightMovePage();
    }
   else  if(parentItem()->parentItem()->x()<=-150&&g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
    {
              g_pMainDlg->m_pHomeControlDlg->roomControl->leftMovePage();
    }
   parentItem()->parentItem()->setX(0);
   parentItem()->parentItem()->setY(0);
}

void   LuxDomoRoomDevice::controlDevice()
{ 
    if(g_pMainDlg->m_pHomeControlDlg->roomControl->controlDeviceItem!=NULL)
    {
        return;
    }

    QString  type=parentItem()->property("deviceType").toString();
    QString  roomUid=parentItem()->property("roomObjectName").toString();
    QString  deviceUid=parentItem()->objectName();
    if(tr("ac")==type)
    {
        //CreateElement("qrc:/QML/device_control_dialog/LuxDomoAcControlDialog.qml");
        //QMetaObject::invokeMethod(controlItem,"init",Qt::DirectConnection,Q_ARG(QVariant,roomUid),Q_ARG(QVariant,deviceUid));
        g_pMainDlg->m_pHomeControlDlg->acItem->setParentItem(parentItem()->parentItem());
        g_pMainDlg->m_pHomeControlDlg->acItem->show();
        g_pMainDlg->m_pHomeControlDlg->roomControl->controlDeviceItem=g_pMainDlg->m_pHomeControlDlg->acItem;
        QMetaObject::invokeMethod(g_pMainDlg->m_pHomeControlDlg->acItem,"init",Qt::DirectConnection,Q_ARG(QVariant,roomUid),Q_ARG(QVariant,deviceUid));
    }
    else   if(tr("curtain")==type)
    {
        //CreateElement("qrc:/QML/device_control_dialog/LuxDomoCurtainControlDialog.qml");
        //QMetaObject::invokeMethod(controlItem,"init",Qt::DirectConnection,Q_ARG(QVariant,roomUid),Q_ARG(QVariant,deviceUid));
        g_pMainDlg->m_pHomeControlDlg->curtainItem->setParentItem(parentItem()->parentItem());
        g_pMainDlg->m_pHomeControlDlg->curtainItem->show();
        g_pMainDlg->m_pHomeControlDlg->roomControl->controlDeviceItem=g_pMainDlg->m_pHomeControlDlg->curtainItem;
        QMetaObject::invokeMethod(g_pMainDlg->m_pHomeControlDlg->curtainItem,"init",Qt::DirectConnection,Q_ARG(QVariant,roomUid),Q_ARG(QVariant,deviceUid));
    }
    else   if(tr("var")==type)
    {
        //CreateElement("qrc:/QML/device_control_dialog/LuxDomoLightControlDialog.qml");
        //QMetaObject::invokeMethod(controlItem,"init",Qt::DirectConnection,Q_ARG(QVariant,roomUid),Q_ARG(QVariant,deviceUid));
        g_pMainDlg->m_pHomeControlDlg->varItem->setParentItem(parentItem()->parentItem());
        g_pMainDlg->m_pHomeControlDlg->varItem->show();
        g_pMainDlg->m_pHomeControlDlg->roomControl->controlDeviceItem=g_pMainDlg->m_pHomeControlDlg->varItem;
        QMetaObject::invokeMethod(g_pMainDlg->m_pHomeControlDlg->varItem,"init",Qt::DirectConnection,Q_ARG(QVariant,roomUid),Q_ARG(QVariant,deviceUid));
    }
    else   if(tr("tv")==type)
    {
        //CreateElement("qrc:/QML/device_control_dialog/LuxDomoTVControlDialog.qml");
        //QMetaObject::invokeMethod(controlItem,"init",Qt::DirectConnection,Q_ARG(QVariant,roomUid),Q_ARG(QVariant,deviceUid));
        g_pMainDlg->m_pHomeControlDlg->tvItem->setParentItem(parentItem()->parentItem());
        g_pMainDlg->m_pHomeControlDlg->tvItem->show();
        g_pMainDlg->m_pHomeControlDlg->roomControl->controlDeviceItem=g_pMainDlg->m_pHomeControlDlg->tvItem;
        QMetaObject::invokeMethod(g_pMainDlg->m_pHomeControlDlg->tvItem,"init",Qt::DirectConnection,Q_ARG(QVariant,roomUid),Q_ARG(QVariant,deviceUid));
    }
    else   if(tr("music")==type)
    {
        //CreateElement("qrc:/QML/device_control_dialog/LuxDomoMusicControlDialog.qml");
        //QMetaObject::invokeMethod(controlItem,"init",Qt::DirectConnection,Q_ARG(QVariant,roomUid),Q_ARG(QVariant,deviceUid));
        //QMetaObject::invokeMethod(controlItem,"findMusicInformation",Qt::DirectConnection);
        g_pMainDlg->m_pHomeControlDlg->musicItem->setParentItem(parentItem()->parentItem());
        g_pMainDlg->m_pHomeControlDlg->musicItem->show();
        g_pMainDlg->m_pHomeControlDlg->roomControl->controlDeviceItem=g_pMainDlg->m_pHomeControlDlg->musicItem;
        QMetaObject::invokeMethod(g_pMainDlg->m_pHomeControlDlg->musicItem,"init",Qt::DirectConnection,Q_ARG(QVariant,roomUid),Q_ARG(QVariant,deviceUid));
        QMetaObject::invokeMethod(g_pMainDlg->m_pHomeControlDlg->musicItem,"findMusicInformation",Qt::DirectConnection);
    }
    else   if(tr("pm")==type||tr("humiture")==type)
    {
        g_pMainDlg->m_pHomeControlDlg->sensorItem->setParentItem(parentItem()->parentItem());
        g_pMainDlg->m_pHomeControlDlg->sensorItem->show();
        g_pMainDlg->m_pHomeControlDlg->roomControl->controlDeviceItem=g_pMainDlg->m_pHomeControlDlg->sensorItem;
        QMetaObject::invokeMethod(g_pMainDlg->m_pHomeControlDlg->sensorItem,"init",Qt::DirectConnection,Q_ARG(QVariant,roomUid),Q_ARG(QVariant,deviceUid));
    }
    else   if(tr("light")==type)
    {
        ControlLight();
    }
    else   if(tr("scene")==type)
    {
        ControlScene();
    }
    else
    {

    }
}

void   LuxDomoRoomDevice::ControlLight()
{
    int  cmd=-1;
    QString   on_off=parentItem()->property("on_off").toString();
    if(tr("off")==on_off)
    {
          on_off=tr("on");
          cmd=0x01;
    }
    else
    {
         on_off=tr("off");
         cmd=0x00;
    }
   QString   deviceImage=g_pMainDlg->m_pHomeControlDlg->GetControlImage(tr("light"),on_off);
   parentItem()->setProperty("deviceImage",deviceImage);
   parentItem()->setProperty("on_off",on_off);
   QString   roomUid=parentItem()->property("roomObjectName").toString();
   g_pMainDlg->m_pHomeControlDlg->m_pChangDeviceData->slot_changDeviceAttribute(roomUid,parentItem()->objectName(),"power",on_off);

   int  address=parentItem()->property("deviceAddress").toInt();
   int  attribute=parentItem()->property("deviceAttribute").toInt();
   char strCmd[6];
   strCmd[0]=0x06;
   strCmd[1]=address; //目的地址设备号
   strCmd[2]=0x00;  //源地址
   strCmd[3] = attribute;
   strCmd[4]=cmd;   //指令
   strCmd[5]=0x00;
   g_pMainDlg->m_pHomeControlDlg->cmdBuffer(strCmd);
}

void   LuxDomoRoomDevice::ControlScene()
{
    g_pMainDlg->m_pHomeControlDlg->m_pApplyScene->m_roomObjectName=parentItem()->property("roomObjectName").toString();
    g_pMainDlg->m_pHomeControlDlg->m_pApplyScene->m_sceneObjectName=parentItem()->objectName();
    operatingType=RoomScenePlaying;
    g_pMainDlg->m_pHomeControlDlg->m_pApplyScene->start();
}

void    LuxDomoRoomDevice::CreateElement(QString  str)
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem()->parentItem());
    QDeclarativeComponent component(engine, QUrl(str));
    QDeclarativeItem *createItem = qobject_cast<QDeclarativeItem*>(component.create());
    createItem->setParentItem(parentItem()->parentItem());
    createItem->setWidth(mLMap_6410_Msg.screenW);
    createItem->setHeight(mLMap_6410_Msg.screenH);
    createItem->setX(0);
    createItem->setY(0);
    controlItem=createItem;
    engines.insert(tr("deviceControl"),engine);
}












