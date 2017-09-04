#include "luxdomocurtaincontrol.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   QDeclarativeItem    *smartHomeItem;
LuxDomoCurtainControl::LuxDomoCurtainControl(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoCurtainControl::~LuxDomoCurtainControl()
{

}

void   LuxDomoCurtainControl::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(0);
    parentItem()->setY(0);
    parentItem()->hide();
}

void   LuxDomoCurtainControl::init(QString   roomUid, QString   deviceUid)
{
    m_deviceElement=g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->LoadingDeviceConditionData(roomUid,deviceUid);
    if(m_deviceElement.isNull())
    {
        parentItem()->hide();
    }
    m_roomUid=roomUid;
    MyMsgLog<<m_deviceElement.attribute("power");
    parentItem()->setProperty("deviceUid",deviceUid);
    parentItem()->setProperty("curtainName",m_deviceElement.attribute("name"));
    if(tr("on")==m_deviceElement.attribute("power"))
    {
        parentItem()->setProperty("curtainCondition",tr("on"));
        parentItem()->setProperty("curtainImage",tr("../device_control/curtain_open.png"));
    }
    else   if(tr("off")==m_deviceElement.attribute("power"))
    {
        parentItem()->setProperty("curtainCondition",tr("off"));
        parentItem()->setProperty("curtainImage",tr("../device_control/curtain_close.png"));
    }
    else   if(tr("stop")==m_deviceElement.attribute("power"))
    {
        parentItem()->setProperty("curtainCondition",tr("stop"));
        parentItem()->setProperty("curtainImage",tr("../device_control/curtain_stop.png"));
    }
}

void   LuxDomoCurtainControl::close()
{  
    parentItem()->hide();
    parentItem()->setParentItem(smartHomeItem);
    if( g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->roomControl->controlDeviceItem=NULL;
    }
}

void   LuxDomoCurtainControl::curtainOpenClicked()
{
    MyMsgLog<<m_deviceElement.attribute("power");
    if(m_deviceElement.attribute("power")==tr("on"))
    {
          return;
    }
    QMetaObject::invokeMethod(parentItem()->parentItem(),"on_offDevice",Qt::DirectConnection,Q_ARG(QVariant,m_deviceElement.attribute(tr("uid"))),Q_ARG(QVariant,true));
    //g_pMainDlg->m_pHomeControlDlg->m_pChangDeviceData->slot_changDeviceAttribute(m_roomUid,m_deviceElement.attribute("uid"),tr("power"),tr("on"));
    m_deviceElement.setAttribute("power",tr("on"));
    CurtainstrCmdSend(0x01);
}

void   LuxDomoCurtainControl::curtainPauseClicked()
{
    MyMsgLog<<m_deviceElement.attribute("power");
    if(m_deviceElement.attribute("power")==tr("stop"))
    {
          return;
    }
    QMetaObject::invokeMethod(parentItem()->parentItem(),"on_offDevice",Qt::DirectConnection,Q_ARG(QVariant,m_deviceElement.attribute(tr("uid"))),Q_ARG(QVariant,true));
    //g_pMainDlg->m_pHomeControlDlg->m_pChangDeviceData->slot_changDeviceAttribute(m_roomUid,m_deviceElement.attribute("uid"),tr("power"),tr("stop"));
    m_deviceElement.setAttribute("power",tr("stop"));
    CurtainstrCmdSend(0x02);
}

void   LuxDomoCurtainControl::curtainCloseClicked()
{
    MyMsgLog<<m_deviceElement.attribute("power");
    if(tr("off")==m_deviceElement.attribute("power"))
    {
          return;
    }
    QMetaObject::invokeMethod(parentItem()->parentItem(),"on_offDevice",Qt::DirectConnection,Q_ARG(QVariant,m_deviceElement.attribute(tr("uid"))),Q_ARG(QVariant,false));
    //g_pMainDlg->m_pHomeControlDlg->m_pChangDeviceData->slot_changDeviceAttribute(m_roomUid,m_deviceElement.attribute("uid"),tr("power"),tr("off"));
    m_deviceElement.setAttribute("power",tr("off"));
    CurtainstrCmdSend(0x00);
}

void  LuxDomoCurtainControl::curtainPowerClicked(bool  value)
{
    QString    attributeValue;
    int  cmd=-1;
    if(value)
    {
        cmd = 0x01;
        attributeValue=tr("on");
    }
    else
    {
         cmd = 0x00;
         attributeValue=tr("off");
    }
    QMetaObject::invokeMethod(parentItem()->parentItem(),"on_offDevice",Qt::DirectConnection,Q_ARG(QVariant,m_deviceElement.attribute(tr("uid"))),Q_ARG(QVariant,value));
    g_pMainDlg->m_pHomeControlDlg->m_pChangDeviceData->slot_changDeviceAttribute(m_roomUid,m_deviceElement.attribute("uid"),tr("power"),attributeValue);
    CurtainstrCmdSend(cmd);
}

void    LuxDomoCurtainControl::CurtainstrCmdSend(int cmd)
{
    int deviceId=m_deviceElement.attribute("id").toInt();
    char strCmd[6];
    strCmd[0]=0x06;    //字节长度
    strCmd[1]=deviceId&0xff; //目的地址设备号
    strCmd[2]=0x00;  //源地址
    strCmd[3]=deviceId>>8;   //设备属性
    strCmd[4]=cmd;
    strCmd[5]=0;
   g_pMainDlg->m_pHomeControlDlg->cmdBuffer(strCmd);
}










