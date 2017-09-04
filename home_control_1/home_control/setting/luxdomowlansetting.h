#ifndef LUXDOMOWLANSETTING_H
#define LUXDOMOWLANSETTING_H

#include<QDeclarativeItem>

class LuxDomoWLANSetting : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoWLANSetting(QDeclarativeItem *parent = 0);
    ~LuxDomoWLANSetting();
    void   componentComplete();
    Q_INVOKABLE  void    init();
    Q_INVOKABLE  void    linkWLAN(bool);
    Q_INVOKABLE  void    breakWLAN();
    Q_INVOKABLE  void    deleteWLAN();
    Q_INVOKABLE  void    seniorDeleteWLAN();
    Q_INVOKABLE  void    editInformation(QString,QString);
    Q_INVOKABLE  void    setInformation(QString);
    Q_INVOKABLE  void    scanWLAN();
    Q_INVOKABLE  void    smartLinkWLAN();
    Q_INVOKABLE  void    on_offWlan(bool);
    Q_INVOKABLE  void    setWifiParameter(QString);
    QString   GetWifiImage(QString,QString,QString);
    int    GetSignalNumber(QString);
signals:
public slots:
    void  slot_addWifiData(QString,QString,QString,QString,QString,QString);
    void  slot_linkData(QString,QString,QString,QString,QString,QString,QString);
    void  slot_clearModel();
    void  slot_linkResult(bool);
private:
    int   m_count;
    int   m_cursor;
};

#endif // LUXDOMOWLANSETTING_H
