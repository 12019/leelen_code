#include "luxdomoscene.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern MainDialog *g_pMainDlg;
extern LMap_6410_Msg mLMap_6410_Msg;
extern   OperateType   operatingType;

LuxDomoScene::LuxDomoScene(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoScene::~LuxDomoScene()
{

}

void   LuxDomoScene::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setWidth(mLMap_6410_Msg.controlW);
    parentItem()->setHeight(mLMap_6410_Msg.controlH);
}

void  LuxDomoScene::showRoomControl()
{

}

void  LuxDomoScene::useHomeScene()
{
    if(parentItem()->objectName()==tr("house-map-user-on.xml"))
    {
        char strCmd[6];
        strCmd[0]=0x06;    //字节长度
        strCmd[1]=0xff; //目的地址设备号
        strCmd[2]=0xff;  //源地址
        strCmd[3]=0xff;   //设备属性
        strCmd[4]=0x01;
        strCmd[5]=0;
        g_pMainDlg->m_pHomeControlDlg->cmdBuffer(strCmd);
    }
    else  if(parentItem()->objectName()==tr("house-map-user-off.xml"))
    {
        char strCmd[6];
        strCmd[0]=0x06;    //字节长度
        strCmd[1]=0xff; //目的地址设备号
        strCmd[2]=0xff;  //源地址
        strCmd[3]=0xff;   //设备属性
        strCmd[4]=0x00;
        strCmd[5]=0;
        g_pMainDlg->m_pHomeControlDlg->cmdBuffer(strCmd);
    }
}

void    LuxDomoScene::recordMouseY(int pointY)
{

}

void   LuxDomoScene::moveParent(int pointY)
{

}
