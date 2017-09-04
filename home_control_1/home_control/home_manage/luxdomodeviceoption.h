#ifndef LUXDOMODEVICEOPTION_H
#define LUXDOMODEVICEOPTION_H

#include<QDeclarativeItem>

class LuxDomoDeviceOption : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoDeviceOption(QDeclarativeItem *parent = 0);
    ~LuxDomoDeviceOption();
    void    componentComplete();
    void     resourceRelease();
    Q_INVOKABLE   void    ensure();
    Q_INVOKABLE   void    inputData(QString,QString);
    Q_INVOKABLE   void    close();
    Q_INVOKABLE   void    setTypeData(QString);
    Q_INVOKABLE   void    showPullDownList(QString);
    void   showDeviceTypeList();
    void   showDeviceAddressList();
    void   showDeviceAttributeList();
    QString   GetControlImage(QString);
signals:

public slots:

private:
    QDeclarativeItem       *listItem;
    QDeclarativeEngine    *listEngine;
};

#endif // LUXDOMODEVICEOPTION_H
