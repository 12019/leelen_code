#include "luxdomoversionssettting.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;
LuxDomoVersionsSettting::LuxDomoVersionsSettting(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoVersionsSettting::~LuxDomoVersionsSettting()
{
    MyMsgLog<<"delete..............."<<parentItem()->objectName();
}

void   LuxDomoVersionsSettting::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW*9/16);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(mLMap_6410_Msg.screenW*5/16);
    parentItem()->setY(0);
}
