#ifndef LUXDOMOTVCONTROL_H
#define LUXDOMOTVCONTROL_H

#include<QDeclarativeItem>
#include<QDomElement>
class LuxDomoTVControl : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoTVControl(QDeclarativeItem *parent = 0);
    ~LuxDomoTVControl();
    void  componentComplete();
    Q_INVOKABLE  void   init(QString,QString);
    Q_INVOKABLE  void   close();
    void     TvStrCmdSend(int);
    int   obtainTVFunOrder(QString);
    Q_INVOKABLE void    tvVolumeClicked(int);
    Q_INVOKABLE void    tvChannelUpClicked();
    Q_INVOKABLE void    tvChannelDownClicked();
    Q_INVOKABLE void    tvModeClicked();
    Q_INVOKABLE void    tvPowerClicked();
    Q_INVOKABLE void    tvVolumeDownClicked();
    Q_INVOKABLE void    tvVolumelUpClicked();
signals:
public slots:
private:
     QDomElement  m_deviceElement;
     int   m_tvAttr;
     int   m_tvAddr;
     QString  m_roomUid;
};

#endif // LUXDOMOTVCONTROL_H
