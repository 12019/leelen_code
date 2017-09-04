#include "luxdomovar_lightcontrol.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   OperateType   operatingType;
extern   QDeclarativeItem    *smartHomeItem;
LuxDomoVar_LightControl::LuxDomoVar_LightControl(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoVar_LightControl::~LuxDomoVar_LightControl()
{  

}

void  LuxDomoVar_LightControl::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(0);
    parentItem()->setY(0);
    parentItem()->hide();
}

void   LuxDomoVar_LightControl::init(QString   roomUid, QString   deviceUid)
{
    m_deviceElement=g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->LoadingDeviceConditionData(roomUid,deviceUid);
    if(m_deviceElement.isNull())
    {
        parentItem()->hide();
    }
    m_roomUid=roomUid;
    parentItem()->setProperty("deviceUid",deviceUid);
    parentItem()->setProperty("lightName",m_deviceElement.attribute("name"));
    parentItem()->setProperty("lightValue",m_deviceElement.attribute("value").toInt());
    if(m_deviceElement.attribute("power")==tr("on"))
    {
        parentItem()->setProperty("lightCondition",true);
        parentItem()->setProperty("brightnessValue",GetBrightnessValue(m_deviceElement.attribute(tr("value")).toInt()));
    }
    else
    {
        parentItem()->setProperty("lightCondition",false);
    }
    MyMsgLog<<m_deviceElement.attribute("power")<<deviceUid<<m_deviceElement.attribute("uid");
}

void    LuxDomoVar_LightControl::close()
{   
    parentItem()->hide();
    parentItem()->setParentItem(smartHomeItem);
    if( g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->roomControl->controlDeviceItem=NULL;
    }
}

void  LuxDomoVar_LightControl::lightPowerClicked(bool value)
{
    QString    attributeValue;
    int  cmd=-1;
    if(value)
    {
        cmd = 0x01;
        attributeValue=tr("on");
        parentItem()->setProperty("brightnessValue",GetBrightnessValue(m_deviceElement.attribute(tr("value")).toInt()));
    }
    else
    {
        cmd = 0x00;
        attributeValue=tr("off");
    }

    int   deviceId=m_deviceElement.attribute("id").toInt();
    char strCmd[6];
    strCmd[0]=0x06;
    strCmd[1]=deviceId&0xff; //目的地址设备号
    strCmd[2]=0x00;  //源地址
    strCmd[3] = deviceId>>8;
    strCmd[4]=cmd;   //指令
    strCmd[5]=0x00;
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(strCmd);
    QMetaObject::invokeMethod(parentItem()->parentItem(),"on_offDevice",Qt::DirectConnection,Q_ARG(QVariant,m_deviceElement.attribute("uid")),Q_ARG(QVariant,value));
    //g_pMainDlg->m_pHomeControlDlg->m_pChangDeviceData->slot_changDeviceAttribute(m_roomUid,m_deviceElement.attribute("uid"),tr("power"),attributeValue);
    m_deviceElement.setAttribute("power",attributeValue);
}

void  LuxDomoVar_LightControl::lightBrightnessClicked(int value)
{
    g_pMainDlg->m_pHomeControlDlg->m_varValue=true;
    g_pMainDlg->m_pHomeControlDlg->timer->start(3000);

    int  oldVlaue=m_deviceElement.attribute(tr("value")).toInt();
    if(value == oldVlaue )
    {
        return;
    }
    parentItem()->setProperty("brightnessValue",GetBrightnessValue(value));
    int   deviceId=m_deviceElement.attribute("id").toInt();
    char strCmd[7];
    strCmd[0]=0x07;    //字节长度
    strCmd[1]=deviceId&0xff; //目的地址设备号
    strCmd[2]=0x00;  //源地址
    strCmd[3]=deviceId>>8;
    strCmd[4]=0x11;   //指令
    strCmd[5]=value;  //亮度值
    strCmd[6]=0x00;
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(strCmd);
    //g_pMainDlg->m_pHomeControlDlg->m_pChangDeviceData->slot_changDeviceAttribute(m_roomUid,m_deviceElement.attribute("uid"),tr("value"),QString::number(value));
    m_deviceElement.setAttribute("value",QString::number(value));
}

QString   LuxDomoVar_LightControl::GetBrightnessValue(int  value)
{
    if(value>=0&&value<5)
    {
        return   tr("#fbebc7");
    }
    else  if(value>=5&&value<10)
    {
        return   tr("#fbe9c1");
    }
    else  if(value>=10&&value<15)
    {
        return   tr("#fbe6b8");
    }
    else  if(value>=15&&value<20)
    {
         return   tr("#fbe3af");
    }
    else  if(value>=20&&value<25)
    {
         return   tr("#fae0a6");
    }
    else  if(value>=25&&value<30)
    {
         return   tr("#fadd9b");
    }
    else  if(value>=30&&value<35)
    {
         return   tr("#fad990");
    }
    else  if(value>=35&&value<40)
    {
         return   tr("#fad584");
    }
    else  if(value>=40&&value<45)
    {
         return   tr("#fad176");
    }
    else  if(value>=45&&value<50)
    {
         return   tr("#facd6b");
    }
    else  if(value>=50&&value<55)
    {
         return   tr("#f9c95ec");
    }
    else  if(value>=55&&value<60)
    {
         return   tr("#f9c653");
    }
    else  if(value>=60&&value<65)
    {
         return   tr("#f9c246");
    }
    else  if(value>=65&&value<70)
    {
         return   tr("#f9be3a");
    }
    else  if(value>=70&&value<75)
    {
         return   tr("#f9ba2e");
    }
    else  if(value>=75&&value<80)
    {
         return   tr("#f9b724");
    }
    else  if(value>=80&&value<85)
    {
         return   tr("#f8b31a");
    }
    else  if(value>=85&&value<90)
    {
         return   tr("#f8b011");
    }
    else  if(value>=90&&value<95)
    {
         return   tr("#f8ae0a");
    }
    else  if(value>=95&&value<=100)
    {
         return   tr("#f8ab00");
    }
    else
    {
         return   tr("#00000000");
    }
}
