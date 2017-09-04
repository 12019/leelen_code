#include "luxdomodealwlandata.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  OperateWlan    operateWlan;

LuxDomoDealWlanData::LuxDomoDealWlanData(QThread *parent) : QThread(parent)
{
    QFile   file(XmlFilePath+tr("waln.xml"));
    if(file.exists())
    {
        if(!file.open(QIODevice::ReadOnly))
        {
            return;
        }
        if (!document.setContent(&file))
        {
            file.close();
            return;
        }
        file.close();
        return;
    }
    QString strHeader( "version=\"1.0\" encoding=\"UTF-8\"" );
    document.appendChild(document.createProcessingInstruction("xml",strHeader));
    QDomElement root_house = document.createElement("wlan");
    document.appendChild(root_house);
    QDomElement wifiConf = document.createElement("wifiConf");
    wifiConf.setAttribute("ssid",tr(""));
    wifiConf.setAttribute("flags",tr(""));
    root_house.appendChild(wifiConf);
    QDomElement wifiList = document.createElement("wifiList");
     root_house.appendChild(wifiList);

     if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return;
     QTextStream stream(&file);
     document.save(stream, 4);
     file.close();
     sync();
}

LuxDomoDealWlanData::~LuxDomoDealWlanData()
{

}

void   LuxDomoDealWlanData::run()
{
    if(operateWlan==InitWifi)
    {
        if(CheckWifiDev())
        {
            QStringList  data;
            data.clear();
            SetWifiConf(data);
            CheckWlanXml();
            if(WlanOn_off())
            {
                ScanWlan();
                SmartLinkWlan();
                SendWlanData();
                emit sigUpdateListen();
            }
            SaveXmlFile();
            emit  sigIsWlan();
        }
    }
    else   if(operateWlan==OnWifi)
    {
        system("ifconfig wlan0 up");
        if(GetWlanCondition())
        { 
            ScanWlan();
            SendWlanData();
        }
        else
        {         
            ScanWlan();
            SendWlanData();
            SmartLinkWlan();
            SendWlanData();
        }
        emit sigUpdateListen();
    }
    else   if(operateWlan==Offwifi)
    {
        system("ifconfig wlan0 down");
        emit sigUpdateListen();
    }
    else   if(operateWlan==SmartLinkWifi)
    {
        ScanWlan();
        SmartLinkWlan();
        SendWlanData();
        emit sigUpdateListen();
    }
    else   if(operateWlan==ScanWifi)
    {
        CheckWlanXml();
        ScanWlan();
        SendWlanData();
        SaveXmlFile();
    }
    else   if(operateWlan==LinkWifi)
    {
        if(WlanOn_off()==false)
        {
            return;
        }
        if(!LinkWlan(m_bssid,m_ssid,m_psk,m_flags,m_range,m_signal))
        {
            SmartLinkWlan();
            SendWlanData();
        }
        emit sigUpdateListen();
    }
    else   if(operateWlan==SeniorLinkWifi)
    {
        QStringList  data=GetWifiParameter(m_bssid);
        if(data.size()!=4)
        {
            return;
        }
        if(!SettingLinkWlan(m_bssid,m_ssid,m_psk,m_flags,m_range,m_signal,data.at(0),data.at(1),data.at(2),data.at(3)))
        {
            SmartLinkWlan();
            SendWlanData();
        }
        emit sigUpdateListen();
    }
    else   if(operateWlan==SeniorBreakWifi)
    {
        BreakWlan();
        SmartLinkWlan();
        SendWlanData();
        emit sigUpdateListen();
    }
    else   if(operateWlan==DeleteWifi)
    {
        BreakWlan();
        DeleteWlan(m_bssid);
        SmartLinkWlan();
        SendWlanData();
        emit sigUpdateListen();
    }
}

void   LuxDomoDealWlanData::SaveXmlFile()
{
    QFile   file(XmlFilePath+tr("waln.xml"));
    if(!file.exists())
    {
        return;
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return;
    QTextStream stream(&file);
    document.save(stream, 4);
    file.close();
    sync();
}

void   LuxDomoDealWlanData::SetData(QString bssid, QString ssid, QString  psk, QString flags,QString  range,QString signal)
{
    m_bssid=bssid;
    m_ssid=ssid;
    m_psk=psk;
    m_flags=flags;
    m_range=range;
    m_signal=signal;
}

bool   LuxDomoDealWlanData::WlanOn_off()
{
    QSettings qsettings("home_control/config.ini",QSettings::IniFormat);
    if(qsettings.value("isWlan").toBool()==true)
    {
      return true;
    }
    return false;
}

void   LuxDomoDealWlanData::SetWifiConf(QStringList  data)
{
    QDomElement   wlanElement=document.firstChildElement("wlan");
    QDomElement   wifiConfElement=wlanElement.firstChildElement("wifiConf");
    if(wifiConfElement.isNull())
    {
        return;
    }

    if(data.size()==6)
    {
        wifiConfElement.setAttribute("bssid",data.at(0));
        wifiConfElement.setAttribute("ssid",data.at(1));
        wifiConfElement.setAttribute("psk",data.at(2));
        wifiConfElement.setAttribute("flags",data.at(3));
        wifiConfElement.setAttribute("range",data.at(4));
        wifiConfElement.setAttribute("signal",data.at(5));
    }
    else
    {
        wifiConfElement.setAttribute("bssid",tr(""));
        wifiConfElement.setAttribute("ssid",tr(""));
        wifiConfElement.setAttribute("psk",tr(""));
        wifiConfElement.setAttribute("flags",tr(""));
        wifiConfElement.setAttribute("range",tr(""));
        wifiConfElement.setAttribute("signal",tr(""));
    }
}

bool   LuxDomoDealWlanData::LinkWlan(QString bssid,QString  ssid, QString  psk, QString flags,QString range,QString signal)
{
    QStringList  data;
    data.clear();
    SetWifiConf(data);
    sigSetLinking(bssid,ssid,psk,flags,range,signal,tr("连接中.."));
    m_ssid=ssid;
    system("wpa_cli -p/var/run/wpa_supplicant remove_network 0");
    system("wpa_cli -p/var/run/wpa_supplicant ap_scan 1");
    system("wpa_cli -p/var/run/wpa_supplicant add_network");
    QString  str="wpa_cli -p/var/run/wpa_supplicant set_network 0 ssid  '\"" +ssid+"\""+"'";
    MyMsgLog<<"命令: "<<str;
    system(str.toLatin1().data());

    if(flags.contains(tr("WPA")))
    {
        str="wpa_cli -p/var/run/wpa_supplicant set_network 0 psk '\""+psk+"\""+"'";
        MyMsgLog<<"WPA  命令: "<<str;
        system(str.toLatin1().data());
    }
    else    if(flags.contains(tr("WEP")))
    {
        system("wpa_cli -p/var/run/wpa_supplicant set_network 0 key_mgmt NONE");
        str="wpa_cli -p/var/run/wpa_supplicant set_network 0 wep_key0  '\""+psk+"\""+"'";
        MyMsgLog<<"WPA  命令: "<<str;
        system(str.toLatin1().data());
    }
    else
    {
        MyMsgLog<<"无加密";
        system("wpa_cli -p/var/run/wpa_supplicant set_network 0 key_mgmt NONE");
    }

    system("wpa_cli -p/var/run/wpa_supplicant select_network 0");
    system("udhcpc -i wlan0 -n -q -t 3  > /mnt/disk/home_control/wifiLink.txt");
    QFile file("/mnt/disk/home_control/wifiLink.txt");
    if(!file.exists()) {  return false; }
    if (!file.open(QIODevice::ReadOnly))  { return false; }
    QTextStream myStream(&file);
    QString myString;
    while(myStream.atEnd() == 0)
    {
        myString = myStream.readLine();
    }
    file.close();
    system("rm /mnt/disk/home_control/wifiLink.txt");
    MyMsgLog<<myString;
    if(myString.mid(0,2)==tr("No"))
    {
        emit  sigLinkresult(false);
        return  false;
    }
    data.clear();
    data<<bssid<<ssid<<psk<<flags<<range<<signal;
    SetWifiConf(data);
    emit  sigLinkresult(true);
    return  true;
}

void  LuxDomoDealWlanData::BreakWlan()
{
    QStringList  data;
    data.clear();
    SetWifiConf(data);
    system("wpa_cli -p/var/run/wpa_supplicant remove_network 0");
    system("udhcpc -i wlan0 -n -q -t 2");
    emit  sigLinkresult(false);
}

void  LuxDomoDealWlanData::DeleteWlan(QString  bssid)
{
    QDomElement   wlanElement=document.firstChildElement("wlan");
    QDomElement   wifiListElement=wlanElement.firstChildElement("wifiList");
    if(wifiListElement.isNull())
    {
        return;
    }
    QDomElement   dataElement=wifiListElement.firstChildElement("data");
    while(!dataElement.isNull())
    {
        if(dataElement.attribute("bssid")==bssid)
        {
            wifiListElement.removeChild(dataElement);
            SaveXmlFile();
            break;
        }
        dataElement=dataElement.nextSiblingElement("data");
    }
}

void  LuxDomoDealWlanData::ChangInformation(QString  bssid,QString  attribute,QString  attributeValue)
{
    MyMsgLog<<bssid<<attribute<<attributeValue;
    QDomElement   wlanElement=document.firstChildElement("wlan");
    QDomElement   wifiListElement=wlanElement.firstChildElement("wifiList");
    if(wifiListElement.isNull())
    {
        return;
    }
    QDomElement   dataElement=wifiListElement.firstChildElement("data");
    while(!dataElement.isNull())
    {
        if(dataElement.attribute("bssid")==bssid)
        {
            dataElement.setAttribute(attribute,attributeValue);
            SaveXmlFile();
            break;
        }
        dataElement=dataElement.nextSiblingElement("data");
    }
}

bool   LuxDomoDealWlanData::CheckWifiDev()
{
    for(int i=0;i<3;i++)
    {
        system("ifconfig  wlan0  >  /mnt/disk/home_control/wifiCheck.txt");
        QFile file("/mnt/disk/home_control/wifiCheck.txt");
        if(!file.exists()) { continue; }
        if (!file.open(QIODevice::ReadOnly)) { continue; }
        QTextStream myStream(&file);
        QString myString;
        while(myStream.atEnd() == 0)
        {
            myString = myStream.readLine();
            if(myString.contains(tr("addr"))==true||myString.contains(tr("Bcast"))==true
                    ||myString.contains(tr("Mask"))==true||myString.contains("RX")||myString.contains("TX"))
            {
                file.close();
                system("rm /mnt/disk/home_control/wifiCheck.txt");
                return  true;
            }
        }
        file.close();
        system("rm /mnt/disk/home_control/wifiCheck.txt");
    }
    return  false;
}

void  LuxDomoDealWlanData::CheckWlanXml()
{
    QDomElement   wlanElement=document.firstChildElement("wlan");
    QDomElement   wifiListElement=wlanElement.firstChildElement("wifiList");
    if(wifiListElement.isNull())
    {
        return;
    }
    QDomElement   dataElement=wifiListElement.firstChildElement("data");
    while(!dataElement.isNull())
    {
        dataElement.setAttribute("range",tr("off"));
        dataElement.setAttribute("signal",tr("-100"));
        if(dataElement.attribute("psk")==tr(""))
        {
            QDomElement  temp=dataElement;
            dataElement=dataElement.nextSiblingElement("data");
            wifiListElement.removeChild(temp);
        }
        else
        {
            dataElement=dataElement.nextSiblingElement("data");
        }
    }
}

void   LuxDomoDealWlanData::AddWlanData(QString bssid, QString  frequency, QString  signal, QString  flags, QString ssid)
{
    QDomElement   wlanElement=document.firstChildElement("wlan");
    QDomElement   wifiListElement=wlanElement.firstChildElement("wifiList");
    if(wifiListElement.isNull())
    {
        return;
    }
    QDomElement   dataElement=wifiListElement.firstChildElement("data");
    while(!dataElement.isNull())
    {
        if(dataElement.attribute("bssid")==bssid)
        {
            dataElement.setAttribute("bssid",bssid);
            dataElement.setAttribute("frequency",frequency);
            dataElement.setAttribute("signal",signal);
            dataElement.setAttribute("flags",flags);
            dataElement.setAttribute("range",tr("on"));
            return;
        }
        dataElement=dataElement.nextSiblingElement("data");
    }

    QDomElement  data = document.createElement("data");
    data.setAttribute("bssid",bssid);
    data.setAttribute("frequency",frequency);
    data.setAttribute("signal",signal);
    data.setAttribute("flags",flags);
    data.setAttribute("ssid",ssid);
    data.setAttribute("psk","");
    data.setAttribute("range",tr("on"));
    wifiListElement.appendChild(data);
}

void   LuxDomoDealWlanData::ScanWlan()
{
    system("wpa_cli -p/var/run/wpa_supplicant scan");
    system("wpa_cli -p/var/run/wpa_supplicant scan_results  >  /mnt/disk/home_control/wifiScan.txt");

    QFile file("/mnt/disk/home_control/wifiScan.txt");
    if(!file.exists()) {   return;    }
    if (!file.open(QIODevice::ReadOnly))  {  return; }
    QTextStream myStream(&file);
    QString myString;
    while(myStream.atEnd() == 0)
    {
        myString = myStream.readLine();
        if(myString.at(2)!=':'&&myString.at(5)!=':'){  continue ;  }
        QStringList  list=myString.split(QRegExp("\\s+"));
        if(list.size()!=5){ continue ; }
        //AddWlanData(myString.mid(0,17),myString.mid(18,4),myString.mid(23,3),myString.mid(27,myString.lastIndexOf("]")-26),myString.mid(myString.lastIndexOf("]")+2));
        AddWlanData(list.at(0),list.at(1),list.at(2),list.at(3),list.at(4));
    }
    file.close();
    system("rm /mnt/disk/home_control/wifiScan.txt");
}

void   LuxDomoDealWlanData::SmartLinkWlan()
{
    QDomElement   wlanElement=document.firstChildElement("wlan");
    QDomElement   wifiListElement=wlanElement.firstChildElement("wifiList");
    if(wifiListElement.isNull())
    {
        return;
    }
    QDomElement   dataElement=wifiListElement.firstChildElement("data");
    while(!dataElement.isNull())
    {
        if(dataElement.attribute("range")==tr("on"))
        {
            if(dataElement.attribute("psk")!=tr("")||dataElement.attribute("flags")==tr("[ESS]"))
            {
                if(LinkWlan(dataElement.attribute("bssid"),dataElement.attribute("ssid"),dataElement.attribute("psk"),dataElement.attribute("flags"),
                                    dataElement.attribute("range"),dataElement.attribute(tr("signal"))))
                {
                    return;
                }
            }
        }
        dataElement=dataElement.nextSiblingElement("data");
    }
}

void  LuxDomoDealWlanData::SendWlanData()
{
    QDomElement   wlanElement=document.firstChildElement("wlan");
    QDomElement   wifiConfElement=wlanElement.firstChildElement("wifiConf");
    if(wifiConfElement.isNull())
    {
        return;
    }
    QString    bssid=wifiConfElement.attribute("bssid");
    QString    ssid=wifiConfElement.attribute("ssid");
    QString    psk=wifiConfElement.attribute("psk");
    QString    flags=wifiConfElement.attribute("flags");
    QString    range=wifiConfElement.attribute("range");
    QString    signal=wifiConfElement.attribute("signal");
    if(bssid!="")
    {
        emit sigSetLinking(bssid,ssid,psk,flags,range,signal,tr("连接"));
    }
    else
    {
        emit  sigSetLinking(tr(""),tr(""),tr(""),tr(""),tr(""),tr(""),tr("断开"));
    }
    emit  sigClearModel();

    QDomElement   wifiListElement=wlanElement.firstChildElement("wifiList");
    if(wifiListElement.isNull())
    {
        return;
    }
    QDomElement   dataElement=wifiListElement.firstChildElement("data");
    while(!dataElement.isNull())
    {
        if(dataElement.attribute("range")==tr("on"))
        {
            if(dataElement.attribute("bssid")!=bssid)
            {
                emit sigWifiData(dataElement.attribute("bssid"),dataElement.attribute("ssid"),dataElement.attribute("psk"),dataElement.attribute("flags"),
                                             dataElement.attribute("range"),dataElement.attribute("signal"));
            }
        }
        dataElement=dataElement.nextSiblingElement("data");
    }

    dataElement=wifiListElement.firstChildElement("data");
    while(!dataElement.isNull())
    {
        if(dataElement.attribute("range")==tr("off"))
        {
            emit sigWifiData(dataElement.attribute("bssid"),dataElement.attribute("ssid"),dataElement.attribute("psk"),dataElement.attribute("flags"),
                                          dataElement.attribute("range"),dataElement.attribute("signal"));
        }
        dataElement=dataElement.nextSiblingElement("data");
    }
}

bool  LuxDomoDealWlanData::GetWlanCondition()
{
    QDomElement   wlanElement=document.firstChildElement("wlan");
    QDomElement   wifiConfElement=wlanElement.firstChildElement("wifiConf");
    if(wifiConfElement.isNull())
    {
        return  false;
    }
    if(wifiConfElement.attribute("bssid")!=tr(""))
    {
       return  true;
    }
    return  false;
}

QStringList   LuxDomoDealWlanData::GetWifiConf()
{
    QStringList  data;
    data.clear();
    QDomElement   wlanElement=document.firstChildElement("wlan");
    QDomElement   wifiConfElement=wlanElement.firstChildElement("wifiConf");
    if(wifiConfElement.isNull())
    {
        return  data;
    }
    data<<wifiConfElement.attribute("bssid")<<wifiConfElement.attribute("ssid")<<wifiConfElement.attribute("psk");
    data<<wifiConfElement.attribute("flags")<<wifiConfElement.attribute("range")<<wifiConfElement.attribute(tr("signal"));
    return  data;
}

QStringList   LuxDomoDealWlanData::GetWifiParameter(QString bssid)
{
    QStringList   data;
    data.clear();
    QDomElement   wlanElement=document.firstChildElement("wlan");
    QDomElement   wifiListElement=wlanElement.firstChildElement("wifiList");
    if(wifiListElement.isNull())
    {
        return data;
    }
    QDomElement   dataElement=wifiListElement.firstChildElement("data");
    while(!dataElement.isNull())
    {
        if(dataElement.attribute("bssid")==bssid)
        {
            if(dataElement.attribute("ip")==tr("")||dataElement.attribute("gateway")==tr("")||dataElement.attribute("netmask")==tr(""))
            {
                return  data;
            }
            data<<dataElement.attribute("ip")<<dataElement.attribute("netmask")<<dataElement.attribute("gateway")<<dataElement.attribute("dns");
            break;
        }
        dataElement=dataElement.nextSiblingElement("data");
    }
    return  data;
}

bool  LuxDomoDealWlanData::SettingLinkWlan(QString bssid,QString ssid,QString psk,QString flags,QString range,
                                           QString signal,QString ip,QString netmask ,QString gateway,QString dns)
{
    if(ip==tr("")||netmask==tr("")||gateway==tr(""))
    {
        emit  sigLinkresult(false);
        return  false;
    }
    QStringList  data;
    data.clear();
    SetWifiConf(data);
    sigSetLinking(bssid,ssid,psk,flags,range,signal,tr("连接中.."));
    m_ssid=ssid;
    system("wpa_cli -p/var/run/wpa_supplicant remove_network 0");
    system("wpa_cli -p/var/run/wpa_supplicant ap_scan 1");
    system("wpa_cli -p/var/run/wpa_supplicant add_network");
    QString  str="wpa_cli -p/var/run/wpa_supplicant set_network 0 ssid  '\"" +ssid+"\""+"'";
    MyMsgLog<<"命令: "<<str;
    system(str.toLatin1().data());

    if(flags.contains(tr("WPA")))
    {
        str="wpa_cli -p/var/run/wpa_supplicant set_network 0 psk '\""+psk+"\""+"'";
        MyMsgLog<<"WPA  命令: "<<str;
        system(str.toLatin1().data());
    }
    else    if(flags.contains(tr("WEP")))
    {
        system("wpa_cli -p/var/run/wpa_supplicant set_network 0 key_mgmt NONE");
        str="wpa_cli -p/var/run/wpa_supplicant set_network 0 wep_key0  '\""+psk+"\""+"'";
        MyMsgLog<<"WPA  命令: "<<str;
        system(str.toLatin1().data());
    }
    else
    {
        MyMsgLog<<"无加密";
        system("wpa_cli -p/var/run/wpa_supplicant set_network 0 key_mgmt NONE");
    }
    system("wpa_cli -p/var/run/wpa_supplicant select_network 0");
    str="ifconfig wlan0  "+ip+"  netmask  "+netmask;
    MyMsgLog<<"命令: "<<str;
    system(str.toLatin1().data());
    str="route add default gw  "+gateway+"  dev  wlan0";
    MyMsgLog<<"命令: "<<str;
    system(str.toLatin1().data());

    str=tr("ping  -c  2  ")+gateway+tr("  -w 4  > /mnt/disk/home_control/wifiSettingLink.txt");
    MyMsgLog<<"命令: "<<str;
    system(str.toLatin1().data());
    QFile file1("/mnt/disk/home_control/wifiSettingLink.txt");
    if(!file1.exists()) { return  false; }
    if (!file1.open(QIODevice::ReadOnly))  { return  false; }
    QTextStream myStream1(&file1);
    QString myString;
    myString=myStream1.readLine();
    myString=myStream1.readLine();
    if(myString.contains("time"))
    {
        data.clear();
        data<<bssid<<ssid<<psk<<flags<<range<<signal;
        SetWifiConf(data);
        emit  sigLinkresult(true);
        file1.close();
        system("rm  /mnt/disk/home_control/wifiSettingLink.txt");
        return  true;
    }
    file1.close();
    system("rm  /mnt/disk/home_control/wifiSettingLink.txt");
    emit  sigLinkresult(false);
    return  false;
}
