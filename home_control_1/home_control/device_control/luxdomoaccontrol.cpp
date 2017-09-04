#include "luxdomoaccontrol.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   AcInfo     acInfo;
extern   QDeclarativeItem    *smartHomeItem;
LuxDomoAcControl::LuxDomoAcControl(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoAcControl::~LuxDomoAcControl()
{

}

void  LuxDomoAcControl::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(0);
    parentItem()->setY(0);
    parentItem()->hide();
}

void   LuxDomoAcControl::init(QString   roomUid, QString   deviceUid)
{
    m_deviceElement=g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->LoadingDeviceConditionData(roomUid,deviceUid);
    if(m_deviceElement.isNull())
    {
        parentItem()->hide();
    }
    m_roomUid=roomUid;
    int deviceId=m_deviceElement.attribute("id").toInt();
    m_acAddr=deviceId & 0xff;
    m_acAttr=(deviceId>>8) & 0xff;
    if(m_deviceElement.attribute("power")==tr("on")||m_acAttr==Ac_Infrared)
    {
        parentItem()->setProperty("acCondition",true);
    }
    else
    {
        parentItem()->setProperty("acCondition",false);
    }
    parentItem()->setProperty("deviceUid",deviceUid);
    parentItem()->setProperty("acName",m_deviceElement.attribute("name"));
    QString  mode=g_pMainDlg->m_pHomeControlDlg->GetAcModeName(m_deviceElement.attribute("mode"));
    parentItem()->setProperty("acMode",mode);
    parentItem()->setProperty("acTemp",m_deviceElement.attribute("temperature"));
    QString wind=m_deviceElement.attribute("swing");
    if(m_acAttr==Central_AC&&wind==tr("off"))
    {
        wind=tr("on");
        m_deviceElement.setAttribute("swing",wind);
    }

    if(m_acAttr==Central_AC)
    {
        QString   swing=g_pMainDlg->m_pHomeControlDlg->GetAcCentralWindName(m_deviceElement.attribute("swing"));
        parentItem()->setProperty("acSwing",swing);
        QString   speed=g_pMainDlg->m_pHomeControlDlg->GetAcCentralSpeedName(m_deviceElement.attribute("wind"));
        parentItem()->setProperty("acSpeed",speed);
    }
    else
    {
        if(m_deviceElement.attribute("swing")==tr("on"))
        {
            parentItem()->setProperty("acSwing",tr("摆风"));
        }
        else
        {
             parentItem()->setProperty("acSwing",tr("固定"));
        }
        QString   speed=g_pMainDlg->m_pHomeControlDlg->GetAcInfraredSpeedName(m_deviceElement.attribute("wind"));
        parentItem()->setProperty("acSpeed",speed);
    }
    MyMsgLog<<m_deviceElement.attribute("power")<<m_deviceElement.attribute("mode")<<m_deviceElement.attribute("swing")<<m_deviceElement.attribute("temperature");
}

void   LuxDomoAcControl::close()
{
    parentItem()->hide();
    parentItem()->setParentItem(smartHomeItem);
    if( g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->roomControl->controlDeviceItem=NULL;
    }
}

void   LuxDomoAcControl::AcstrCmdSend(int  cmd,int cmdAttr)
{
    MyMsgLog<<cmd<<cmdAttr;
    if(0 > cmd)
    {
        return;
    }
    QByteArray m_array;
    switch (m_acAttr)
    {
    case Ac_Infrared://红外空调
        m_array.resize(8);
        m_array.data()[0] = 0x08;
        m_array.data()[1] = m_acAddr;
        m_array.data()[2] = 0x00;
        m_array.data()[3] = m_acAttr;
        m_array.data()[4] = 0x20;
        m_array.data()[5] = cmd;
        m_array.data()[6] = 0x01;
        m_array.data()[7] = 0x00;
        break;
    case Central_AC: //中央空调
        m_array.resize(7);
        m_array.data()[0] = 0x07;
        m_array.data()[1] = m_acAddr;
        m_array.data()[2] = 0x00;
        m_array.data()[3] = m_acAttr;
        m_array.data()[4] = cmdAttr;
        m_array.data()[5] = cmd;
        m_array.data()[6] = 0x00;
        break;
    default:
        break;
    }
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
    m_array.clear();
}

void  LuxDomoAcControl::acPowerClicked()
{
    int   cmd=-1;   //指令
    QString    attributeValue;
    char m_array[6]={0};
    if(m_deviceElement.attribute("power")==tr("off"))
    {
        cmd = 0x01;
        attributeValue=tr("on");
    }
    else
    {
        cmd = 0x00;
        attributeValue=tr("off");
    }

    switch (m_acAttr)
    {
    case    Central_AC:
        m_array[0] = 0x06;
        m_array[1] = m_acAddr;
        m_array[2] = 0x00;
        m_array[3] = m_acAttr;
        m_array[4] = cmd;
        m_array[5] = 0x00;
        g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array);
        break;
    case    Ac_Infrared:
        if(tr("on") == attributeValue)
        {
            cmd=obtainAcFunOrder();
            AcstrCmdSend(cmd,0x00);
        }
        else
        {
            QString m_Sql = "select code,isLearned from ac" + m_deviceElement.attribute("uid") +
                          " where power = '"+tr("关")+"' and wind = '-' and mode = '-' and speed = '-' and temp = '-'";
            QSqlQuery query;
            bool m_SqlIsValid=false;
            bool m_IsLearned=false;
            m_SqlIsValid = query.exec(m_Sql);
            MyMsgLog<<m_Sql<<m_IsLearned;
            if(!m_SqlIsValid || !query.next())
            {
                g_pMainDlg->m_pHomeControlDlg->ShowHintInformation(tr("该指令未红外学习"));
                cmd = -1;
            }
            else
            {
                m_IsLearned = query.value(1).toBool();
                if(m_IsLearned)
                {
                    cmd = query.value(0).toInt();
                }
                else
                {
                    g_pMainDlg->m_pHomeControlDlg->ShowHintInformation(tr("该指令未红外学习"));
                    cmd = -2;
                }
            }
            AcstrCmdSend(cmd,0x00);
        }
        break;
    default:
                break;
    }

    if(attributeValue==tr("on")||m_acAttr==Ac_Infrared)
    {
        parentItem()->setProperty("acCondition",true);
        QMetaObject::invokeMethod(parentItem()->parentItem(),"on_offDevice",Qt::DirectConnection,Q_ARG(QVariant,m_deviceElement.attribute(tr("uid"))),Q_ARG(QVariant,true));
    }
    else
    {
        parentItem()->setProperty("acCondition",false);
        QMetaObject::invokeMethod(parentItem()->parentItem(),"on_offDevice",Qt::DirectConnection,Q_ARG(QVariant,m_deviceElement.attribute(tr("uid"))),Q_ARG(QVariant,false));
    }
    m_deviceElement.setAttribute("power",attributeValue);
}

void   LuxDomoAcControl::acTempDownClicked()
{
    int   cmd = -1;
    int   temp = parentItem()->property("acTemp").toInt();
    if(16 > --temp)
    {
        temp = 32;
    }
    parentItem()->setProperty("acTemp",QString::number(temp));
    switch (m_acAttr)
    {
    case Ac_Infrared:
         cmd = obtainAcFunOrder();
         AcstrCmdSend(cmd,0x00);
        break;
    case Central_AC:
        cmd = temp;
        AcstrCmdSend(cmd,0x02);
        break;
    }
    m_deviceElement.setAttribute("temperature",QString::number(temp));
}

void   LuxDomoAcControl::acTempUpClicked()
{
    int   cmd = -1;
    int   temp = parentItem()->property("acTemp").toInt();
    if(32 < ++temp)
    {
        temp = 16;
    }
    parentItem()->setProperty("acTemp",QString::number(temp));
    switch (m_acAttr)
    {
    case Ac_Infrared:
         cmd = obtainAcFunOrder();
         AcstrCmdSend(cmd,0x00);
        break;
    case Central_AC:
        cmd = temp;
        AcstrCmdSend(cmd,0x02);
        break;
    }
    m_deviceElement.setAttribute("temperature",QString::number(temp));
}

void  LuxDomoAcControl::acWindClicked()
{
    int  cmd=-1;
    QString    wind=m_deviceElement.attribute("swing");
    QStringList  acCentralWindEn=acInfo.acCentralWindEn;
    switch (m_acAttr)
    {
    case Ac_Infrared:
        if(wind==tr("on"))
        {
            wind=tr("off");
            parentItem()->setProperty("acSwing",tr("固定"));
        }
        else  if(wind==tr("off"))
        {
            wind=tr("on");
            parentItem()->setProperty("acSwing",tr("摆风"));
        }
        cmd = obtainAcFunOrder();
        AcstrCmdSend(cmd,0x00);
        break;
    case Central_AC:
        int  size=acCentralWindEn.size();
        for(int i=0;i<size;i++)
        {
            if(wind==acCentralWindEn.at(i))
            {
                i=(i+1)%size;
                wind=acCentralWindEn.at(i);
                cmd=i;
                break;
            }
        }
        parentItem()->setProperty("acSwing",g_pMainDlg->m_pHomeControlDlg->GetAcCentralWindName(wind));
        AcstrCmdSend(cmd,0x05);
        break;
    }
    m_deviceElement.setAttribute("swing",wind);
}

void   LuxDomoAcControl::acModeClicked()
{
    int  cmd=-1;
    QString        mode=m_deviceElement.attribute("mode");
    QStringList  acModeEn=acInfo.acModeEn;
    int    size=acModeEn.size();
    for(int  i=0;i<size;i++)
    {
        if(mode==acModeEn.at(i))
        {
            i=(i+1)%size;
            mode=acModeEn.at(i);
            cmd=i;
            break;
        }
    }
    parentItem()->setProperty("acMode",g_pMainDlg->m_pHomeControlDlg->GetAcModeName(mode));
    switch (m_acAttr)
    {
    case Ac_Infrared:
         cmd = obtainAcFunOrder();
         AcstrCmdSend(cmd,0x00);
        break;
    case Central_AC:
        AcstrCmdSend(cmd,0x03);
        break;
    }
    m_deviceElement.setAttribute("mode",mode);
}

void  LuxDomoAcControl::acSpeedClicked()
{
    int  cmd=-1;
    QStringList  acCentralSpeedEn=acInfo.acCentralSpeedEn;
    QStringList  acInfraredSpeedEn=acInfo.acInfraredSpeedEn;
    QString        speed=m_deviceElement.attribute("wind");
    if(m_acAttr==Ac_Infrared)
    {
        int   count=acInfraredSpeedEn.size();
        for(int i=0;i<count;i++)
        {
            if(speed==acInfraredSpeedEn.at(i))
            {
                i=(i+1)%count;
                speed=acInfraredSpeedEn.at(i);
                cmd=i;
                break;
            }
        }
        parentItem()->setProperty("acSpeed",g_pMainDlg->m_pHomeControlDlg->GetAcInfraredSpeedName(speed));
        cmd = obtainAcFunOrder();
        AcstrCmdSend(cmd,0x00);
    }
    else   if(m_acAttr==Central_AC)
    {
        int  size=acCentralSpeedEn.size();
        for(int i=0;i<size;i++)
        {
            if(speed==acCentralSpeedEn.at(i))
            {
                i=(i+1)%size;
                speed=acCentralSpeedEn.at(i);
                cmd=i;
                break;
            }
        }
        AcstrCmdSend(cmd,0x04);
        parentItem()->setProperty("acSpeed",g_pMainDlg->m_pHomeControlDlg->GetAcCentralSpeedName(speed));
    }
    m_deviceElement.setAttribute("wind",speed);
 }

int  LuxDomoAcControl::obtainAcFunOrder()
{
    int     cmd=-3;
    bool  mIsLearned;
    bool  mOrderIsValid;
    QString   acStatus=tr("开");
    QString   acWind=parentItem()->property("acSwing").toString();
    QString   acSpeed=parentItem()->property("acSpeed").toString();
    QString   acTemp=parentItem()->property("acTemp").toString();
    QString   acMode=parentItem()->property("acMode").toString();

    QSqlQuery query;
    QString     head = "select code,isLearned from ac" + m_deviceElement.attribute("uid") + " where ";
    QString    _power = "power ='" + acStatus + "'" + " and";
    QString    _wind = " wind = '" + acWind + "'" + " and";
    QString    _mode = " mode = '" + acMode + "'" + " and";
    QString    _speed = " speed = '" + acSpeed + "'" + " and";
    QString    _temp = " temp = '" + acTemp + tr("℃")+"'";
    QString    sql = head + _power  +_wind + _mode + _speed + _temp;      //合并数据库查询语句
    mOrderIsValid = query.exec(sql);
    MyMsgLog<<mOrderIsValid<<sql;
    if(!mOrderIsValid || !query.next())
    {
        g_pMainDlg->m_pHomeControlDlg->ShowHintInformation(tr("该指令未红外学习"));
        return -1;
    }
    cmd = query.value(0).toInt();
    mIsLearned = query.value(1).toBool();
    if(!mIsLearned)
    {
        g_pMainDlg->m_pHomeControlDlg->ShowHintInformation(tr("该指令未红外学习"));
        return -2;
    }
    else
    {
        return cmd;
    }
}
