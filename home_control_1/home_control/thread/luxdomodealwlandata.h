#ifndef LUXDOMODEALWLANDATA_H
#define LUXDOMODEALWLANDATA_H

#include <QThread>
#include<QDomDocument>
class LuxDomoDealWlanData : public QThread
{
    Q_OBJECT
public:
    explicit LuxDomoDealWlanData(QThread *parent = 0);
    ~LuxDomoDealWlanData();
    void    run();
    void    SaveXmlFile();
    void    SetWifiConf(QStringList);
    void    SetData(QString bssid,QString ssid,QString psk,QString flags,QString range,QString signal);
    bool    WlanOn_off();

    bool    GetWlanCondition();
    QStringList  GetWifiConf();
    QStringList  GetWifiParameter(QString);

    bool   LinkWlan(QString,QString,QString,QString,QString,QString);
    bool   SettingLinkWlan(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);
    void   BreakWlan();
    void   DeleteWlan(QString);
    void   ChangInformation(QString,QString,QString);
    void   SmartLinkWlan();

    void   CheckWlanXml();
    bool   CheckWifiDev();

    void   ScanWlan();
    void   AddWlanData(QString,QString,QString,QString,QString);
    void   SendWlanData();
signals:
    void   sigSetLinking(QString,QString,QString,QString,QString,QString,QString);
    void   sigWifiData(QString,QString,QString,QString,QString,QString);
    void   sigLinkresult(bool);
    void   sigClearModel();
    void   sigUpdateListen();
    void   sigIsWlan();
public slots:
public:
    QDomDocument  document;
    QString   m_bssid;
    QString   m_ssid;
    QString   m_psk;
    QString   m_flags;
    QString   m_range;
    QString   m_signal;
};

#endif // LUXDOMODEALWLANDATA_H
