#ifndef LUXDOMOSETTING_H
#define LUXDOMOSETTING_H

#include<QDeclarativeItem>

class LuxDomoSetting : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoSetting(QDeclarativeItem *parent = 0);
    ~LuxDomoSetting();
    void   componentComplete();
    void    AddOptions();
    void    CloseItemDialog();
    void    CreateElement(QString);
    Q_INVOKABLE   void    back();
    Q_INVOKABLE   void    chooseSettingOptions(QString);
    void    SetInformation(QString);
signals:
public slots:
    void   slot_setCloudCondition(QString);
    void   slot_addWifiSetting();
private:
    QDeclarativeItem      *showingItem;
    QDeclarativeEngine  *showingEngine;
    QDeclarativeItem      *wlanItem;
};

#endif // LUXDOMOSETTING_H
