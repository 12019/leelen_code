#include "luxdomosensorinformation.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   QDeclarativeItem    *smartHomeItem;

LuxDomoSensorInformation::LuxDomoSensorInformation(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoSensorInformation::~LuxDomoSensorInformation()
{

}

void  LuxDomoSensorInformation::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(0);
    parentItem()->setY(0);
    parentItem()->hide();
}

void  LuxDomoSensorInformation::init(QString  roomUid, QString  deviceUid)
{
    QStringList  value=g_pMainDlg->m_pHomeControlDlg->m_pThemebar->GetSensorValue(roomUid);
    if(value.size()==4)
    {
        parentItem()->setProperty("deviceUid",deviceUid);
        parentItem()->setProperty("airValue",value.at(0));
        if(value.at(3)==tr("")||value.at(3).toDouble()<0||value.at(3).toDouble()>500)
        {
            parentItem()->setProperty("airValue1",-8);
        }
        else
        {
            parentItem()->setProperty("airValue1",value.at(3).toDouble());
        }
        parentItem()->setProperty("tempValue",value.at(1));
        parentItem()->setProperty("humidnessValue",value.at(2));
    }
}

void  LuxDomoSensorInformation::close()
{
    parentItem()->hide();
    parentItem()->setParentItem(smartHomeItem);
    if( g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->roomControl->controlDeviceItem=NULL;
    }
}
