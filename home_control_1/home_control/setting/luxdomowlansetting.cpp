#include "luxdomowlansetting.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   EditDataType     dataType;
extern   OperateWlan    operateWlan;
LuxDomoWLANSetting::LuxDomoWLANSetting(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    m_count=0;
    m_cursor=0;
}

LuxDomoWLANSetting::~LuxDomoWLANSetting()
{

}

void  LuxDomoWLANSetting::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    connect(g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData,SIGNAL(sigWifiData(QString,QString,QString,QString,QString,QString)),
    this,SLOT(slot_addWifiData(QString,QString,QString,QString,QString,QString)));
    connect(g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData,SIGNAL(sigClearModel()),this,SLOT(slot_clearModel()));
    connect(g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData,SIGNAL(sigSetLinking(QString,QString,QString,QString,QString,QString,QString)),
    this,SLOT(slot_linkData(QString,QString,QString,QString,QString,QString,QString)));
    connect(g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData,SIGNAL(sigLinkresult(bool)),this,SLOT(slot_linkResult(bool)),Qt::QueuedConnection);
    parentItem()->setWidth(mLMap_6410_Msg.screenW*9/16);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(mLMap_6410_Msg.screenW*5/16);
    parentItem()->setY(0);
}

void  LuxDomoWLANSetting::init()
{
    QMetaObject::invokeMethod(parentItem(),"clearModel",Qt::DirectConnection);
    QSettings qsettings("home_control/config.ini",QSettings::IniFormat);
    if(qsettings.value("isWlan").toBool() == false||qsettings.value("isWlan").isNull())
    {
         parentItem()->setProperty("isWlan",false);
    }
    else
    {
        parentItem()->setProperty("isWlan",true);
        g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->SendWlanData();
        if(!g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->isRunning()&&
                !g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->GetWlanCondition())
        {
            operateWlan=SmartLinkWifi;
            g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->start();
        }
        else  if(g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->isRunning())
        {
            parentItem()->setProperty("ssid",g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->m_ssid);
            parentItem()->setProperty("condition",tr("连接中.."));
        }
    }
}

void  LuxDomoWLANSetting::on_offWlan(bool  value)
{
    if(g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->isRunning())
    {
        g_pMainDlg->m_pHomeControlDlg->ShowHintInformation(tr("网卡工作中..稍等"));
        return;
    }

    if(value)
    {
        value=false;
        parentItem()->setProperty("isWlan",false);
        operateWlan=Offwifi;
        g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->start();
    }
    else
    {
        value=true;
        parentItem()->setProperty("isWlan",true);
        operateWlan=OnWifi;
        g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->start();
    }

    QSettings qsettings("home_control/config.ini",QSettings::IniFormat);
    qsettings.setValue("isWlan",value);
}

void  LuxDomoWLANSetting::linkWLAN(bool value)
{
    if(g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->isRunning())
    {
        g_pMainDlg->m_pHomeControlDlg->ShowHintInformation(tr("网卡工作中..稍等"));
        return;
    }
    QString   bssid=parentItem()->property("pressBssid").toString();
    QString   ssid=parentItem()->property("pressSsid").toString();
    QString   psk=parentItem()->property("pressPsk").toString();
    QString   flags=parentItem()->property("pressFlags").toString();
    QString   range=parentItem()->property("pressRange").toString();
    QString   signal=parentItem()->property("pressSignal").toString();
    int    index=parentItem()->property("pressIndex").toInt();
    QMetaObject::invokeMethod(parentItem(),"removeModel",Qt::DirectConnection,Q_ARG(QVariant,index));
    m_count--;
    int  num=GetSignalNumber(signal);
    if(num==3){  m_cursor--;  }
    if(parentItem()->property("bssid").toString()!=tr(""))
    {
        slot_addWifiData(parentItem()->property("bssid").toString(),parentItem()->property("ssid").toString(),parentItem()->property("psk").toString(),
                                      parentItem()->property("flags").toString(),parentItem()->property("range").toString(),parentItem()->property("sig").toString());
    }
    g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->SetData(bssid,ssid,psk,flags,range,signal);
    if(value==true)
    {
        operateWlan=SeniorLinkWifi;
    }
    else
    {
        operateWlan=LinkWifi;
    }
    g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->start();
}

void  LuxDomoWLANSetting::breakWLAN()
{
    if(g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->isRunning())
    {
        g_pMainDlg->m_pHomeControlDlg->ShowHintInformation(tr("网卡工作中..稍等"));
        return;
    }
    parentItem()->setProperty("condition",tr("断开中.."));
    operateWlan=SeniorBreakWifi;
    g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->start();
}

void  LuxDomoWLANSetting::seniorDeleteWLAN()
{
    if(g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->isRunning())
    {
        g_pMainDlg->m_pHomeControlDlg->ShowHintInformation(tr("网卡工作中..稍等"));
        return;
    }
    QString   bssid=parentItem()->property("bssid").toString();
    QString   ssid=parentItem()->property("ssid").toString();
    QString   psk=parentItem()->property("psk").toString();
    QString   flags=parentItem()->property("flags").toString();
    QString   range=parentItem()->property("range").toString();
    QString   signal=parentItem()->property("sig").toString();
     parentItem()->setProperty("condition",tr("断开中.."));
    g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->SetData(bssid,ssid,psk,flags,range,signal);
    operateWlan=DeleteWifi;
    g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->start();
}

void  LuxDomoWLANSetting::deleteWLAN()
{
    int  index=parentItem()->property("pressIndex").toInt();
    QString  bssid=parentItem()->property("pressBssid").toString();
    QString  signal=parentItem()->property("pressSignal").toString();
    QMetaObject::invokeMethod(parentItem(),"removeModel",Qt::DirectConnection,Q_ARG(QVariant,index));
    m_count--;
    int  num=GetSignalNumber(signal);
    if(num==3){  m_cursor--;  }
    g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->DeleteWlan(bssid);
}

void LuxDomoWLANSetting::editInformation(QString  type,QString  data)
{
    if(type==tr("psk"))
    {
        dataType=WifiPasswd;
    }
    else  if(type==tr("ip"))
    {
        dataType=WifiIp;
    }
    else  if(type==tr("dns"))
    {
        dataType=WifiDns;
    }
    else  if(type==tr("netmask"))
    {
        dataType=WifiNetmask;
    }
    else  if(type==tr("gateway"))
    {
        dataType=WifiGateway;
    }
    g_pMainDlg->m_pHomeControlDlg->m_editName=data;
    g_pMainDlg->m_pHomeControlDlg->setHCLineText(data);
}

void  LuxDomoWLANSetting::setInformation(QString data)
{
    MyMsgLog<<data;
    if(dataType==WifiDns)
    {
        parentItem()->setProperty("pressDns",data);
        g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->ChangInformation(parentItem()->property("pressBssid").toString(),tr("dns"),data);
    }
    else   if(dataType==WifiGateway)
    {
        parentItem()->setProperty("pressGateway",data);
        g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->ChangInformation(parentItem()->property("pressBssid").toString(),tr("gateway"),data);
    }
    else   if(dataType==WifiIp)
    {
        parentItem()->setProperty("pressIp",data);
        g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->ChangInformation(parentItem()->property("pressBssid").toString(),tr("ip"),data);
    }
    else   if(dataType==WifiNetmask)
    {
        parentItem()->setProperty("pressNetmask",data);
        g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->ChangInformation(parentItem()->property("pressBssid").toString(),tr("netmask"),data);
    }
    else   if(dataType==WifiPasswd)
    {
        parentItem()->setProperty("pressPsk",data);
        int  index=parentItem()->property("pressIndex").toInt();
        QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                                  Q_ARG(QVariant,index),
                                  Q_ARG(QVariant,tr("listPsk")),
                                  Q_ARG(QVariant,data));
        g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->ChangInformation(parentItem()->property("pressBssid").toString(),tr("psk"),data);

    }
}

void   LuxDomoWLANSetting::scanWLAN()
{
    if(g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->isRunning())
    {
        g_pMainDlg->m_pHomeControlDlg->ShowHintInformation(tr("网卡工作中..稍等"));
        return;
    }
    operateWlan=ScanWifi;
    g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->start();
}

void  LuxDomoWLANSetting::smartLinkWLAN()
{
    if(g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->isRunning())
    {
        g_pMainDlg->m_pHomeControlDlg->ShowHintInformation(tr("网卡工作中..稍等"));
        return;
    }
    operateWlan=SmartLinkWifi;
    g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->start();
}

void  LuxDomoWLANSetting::slot_addWifiData(QString bssid,QString  ssid,QString  psk, QString  flags,QString  range,QString  signal)
{
    QString  image=GetWifiImage(flags,signal,range);
    int  num=GetSignalNumber(signal);

    if(num==1)
    {
        QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,Q_ARG(QVariant,bssid),Q_ARG(QVariant,ssid),
        Q_ARG(QVariant,psk),Q_ARG(QVariant,flags),Q_ARG(QVariant,range),Q_ARG(QVariant,signal),Q_ARG(QVariant,image));
        m_count++;
    }
    else   if(num==2)
    {
        if(m_count<=m_cursor)
        {
            QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,Q_ARG(QVariant,bssid),Q_ARG(QVariant,ssid),
            Q_ARG(QVariant,psk),Q_ARG(QVariant,flags),Q_ARG(QVariant,range),Q_ARG(QVariant,signal),Q_ARG(QVariant,image));
        }
        else
        {
            QMetaObject::invokeMethod(parentItem(),"insertModel",Qt::DirectConnection,Q_ARG(QVariant,m_cursor),Q_ARG(QVariant,bssid),Q_ARG(QVariant,ssid),
            Q_ARG(QVariant,psk),Q_ARG(QVariant,flags),Q_ARG(QVariant,range),Q_ARG(QVariant,signal),Q_ARG(QVariant,image));
        }
        m_count++;
    }
    else  if(num==3)
    {
        QMetaObject::invokeMethod(parentItem(),"insertModel",Qt::DirectConnection,Q_ARG(QVariant,0),Q_ARG(QVariant,bssid),Q_ARG(QVariant,ssid),
        Q_ARG(QVariant,psk),Q_ARG(QVariant,flags),Q_ARG(QVariant,range),Q_ARG(QVariant,signal),Q_ARG(QVariant,image));
        m_count++;
        m_cursor++;
    }
    MyMsgLog<<m_count<<m_cursor;
}

void  LuxDomoWLANSetting::slot_linkData(QString  bssid,QString  ssid, QString  psk, QString  flags,QString  range,QString signal,QString condition)
{
    parentItem()->setProperty("bssid",bssid);
    parentItem()->setProperty("ssid",ssid);
    parentItem()->setProperty("psk",psk);
    parentItem()->setProperty("flags",flags);
    parentItem()->setProperty("range",range);
    parentItem()->setProperty("sig",signal);
    parentItem()->setProperty("condition",condition);
}

void  LuxDomoWLANSetting::slot_linkResult(bool  value)
{
    MyMsgLog<<"HHHHHHHHHHHHHHHH"<<value;
    if(value)
    {
        parentItem()->setProperty("condition",tr("连接"));
    }
    else
    {
        parentItem()->setProperty("condition",tr("断开"));
    }
}

void  LuxDomoWLANSetting::slot_clearModel()
{
    m_count=0;
    m_cursor=0;
    QMetaObject::invokeMethod(parentItem(),"clearModel",Qt::DirectConnection);
}

QString   LuxDomoWLANSetting::GetWifiImage(QString  flags, QString  signal,QString  range)
{
    if(range==tr("off"))
    {
        return  tr("../setting/signal_wpa1.png");
    }

    int  number=GetSignalNumber(signal);
    if(flags.contains(tr("WPA"))||flags.contains(tr("WEP")))
    {
        return   tr("../setting/signal_wpa")+QString::number(number)+tr(".png");
    }
    else
    {
        return   tr("../setting/signal_ess")+QString::number(number)+tr(".png");
    }
    return  tr("../setting/signal_wpa1.png");
}

int  LuxDomoWLANSetting::GetSignalNumber(QString  signal)
{
    int  data=signal.toInt();
    if(data>-50)
    {
        return  3;
    }
    else  if(data<-80)
    {
        return  1;
    }
    else
    {
        return  2;
    }
    return 0;
}

void  LuxDomoWLANSetting::setWifiParameter(QString bssid)
{
    QStringList  data= g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->GetWifiParameter(bssid);
    MyMsgLog<<data.size();
    if(data.size()!=4)
    {
        return;
    }
    parentItem()->setProperty("pressIp",data.at(0));
    parentItem()->setProperty("pressNetmask",data.at(1));
    parentItem()->setProperty("pressGateway",data.at(2));
    parentItem()->setProperty("pressDns",data.at(3));
}
