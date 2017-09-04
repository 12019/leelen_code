#include "luxdomoroom.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern MainDialog *g_pMainDlg;
extern LMap_6410_Msg mLMap_6410_Msg;

LuxDomoRoom::LuxDomoRoom(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}
LuxDomoRoom::~LuxDomoRoom()
{

}

void  LuxDomoRoom::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setProperty("nameSize",mLMap_6410_Msg.controlFont);
}

void  LuxDomoRoom::showFunctionOption()
{
      QMetaObject::invokeMethod(parentItem()->parentItem(),"showFunctionOption",Qt::QueuedConnection,Q_ARG(QVariant,parentItem()->objectName()));
}

void  LuxDomoRoom::roomManage()
{
    if(g_pMainDlg->m_pHomeControlDlg->m_pRoomManage==NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->CreateRoomManage();
        if(g_pMainDlg->m_pHomeControlDlg->m_pRoomManage!=NULL)
        {
            QString    roomName = parentItem()->property("roomName").toString();
            QString    roomImage = parentItem()->property("roomImage").toString();
            g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->SetRoomInforMation(roomName,parentItem()->objectName(),roomImage);
        }
    }
}

void   LuxDomoRoom::recordMouseY(int  pointY)
{
        mouseY=pointY;
}

void   LuxDomoRoom::moveParent(int  pointY)
{
         parentItem()->parentItem()->setY(parentItem()->parentItem()->y()+(pointY-mouseY));
}







