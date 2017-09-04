#include "luxdomotoolbar.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern MainDialog *g_pMainDlg;
extern LMap_6410_Msg mLMap_6410_Msg;

LuxDomoToolbar::LuxDomoToolbar(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoToolbar::~LuxDomoToolbar()
{

}


void  LuxDomoToolbar::componentComplete()
{         MyMsgLog<<parentItem()->objectName()<<objectName();
          parentItem()->setWidth(mLMap_6410_Msg.screenW*7/8);
          parentItem()->setHeight(mLMap_6410_Msg.screenH/8);
          parentItem()->setX(0);
          parentItem()->setY(0);
}

void   LuxDomoToolbar::back()
{  
    if(parentItem()->parentItem()->objectName()==tr("roomManage"))
      {
          parentItem()->parentItem()->hide();
          parentItem()->parentItem()->deleteLater();
      }
}

