#ifndef LUXDOMOACCONTROL_H
#define LUXDOMOACCONTROL_H

#include<QDeclarativeItem>
#include<QDomElement>
class LuxDomoAcControl : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoAcControl(QDeclarativeItem *parent = 0);
    ~LuxDomoAcControl();
    void  componentComplete();
    Q_INVOKABLE  void   init(QString,QString);
    Q_INVOKABLE  void   close();
    void    AcstrCmdSend(int,int);
    int   obtainAcFunOrder();
    Q_INVOKABLE  void   acPowerClicked();
    Q_INVOKABLE  void   acModeClicked();
    Q_INVOKABLE  void   acTempUpClicked();
    Q_INVOKABLE  void   acTempDownClicked();
    Q_INVOKABLE  void   acWindClicked();
    Q_INVOKABLE  void   acSpeedClicked();
signals:
public slots:
private:
    QDomElement  m_deviceElement;
    QString    m_roomUid;
    int   m_acAttr;
    int   m_acAddr;
};

#endif // LUXDOMOACCONTROL_H
