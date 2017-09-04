#include "luxdomoroomfunctionoption.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   EditDataType     dataType;
extern  QMultiMap<QString,QDeclarativeEngine*>   engines;
LuxDomoRoomFunctionOption::LuxDomoRoomFunctionOption(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoRoomFunctionOption::~LuxDomoRoomFunctionOption()
{

}

void    LuxDomoRoomFunctionOption::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
}

void    LuxDomoRoomFunctionOption::deleteRoom()
{
    QMetaObject::invokeMethod(parentItem()->parentItem(),"deleteRoom",Qt::DirectConnection);
    parentItem()->deleteLater();
}

void   LuxDomoRoomFunctionOption::setRoomName()
{
    dataType=RoomName;
    g_pMainDlg->m_pHomeControlDlg->m_editName=parentItem()->property("roomName").toString();
    g_pMainDlg->m_pHomeControlDlg->setHCLineText(parentItem()->property("roomName").toString());
    parentItem()->deleteLater();
}

void   LuxDomoRoomFunctionOption::close()
{
    parentItem()->hide();
    parentItem()->deleteLater();
}

