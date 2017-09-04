#ifndef LUXDOMOCURTAINCONTROL_H
#define LUXDOMOCURTAINCONTROL_H

#include<QDeclarativeItem>
#include<QDomElement>
class LuxDomoCurtainControl : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoCurtainControl(QDeclarativeItem *parent = 0);
    ~LuxDomoCurtainControl();
    void  componentComplete();
    Q_INVOKABLE  void   init(QString,QString);
    Q_INVOKABLE  void   close();
     void    CurtainstrCmdSend(int);
    Q_INVOKABLE void    curtainCloseClicked();
    Q_INVOKABLE void    curtainPauseClicked();
    Q_INVOKABLE void    curtainOpenClicked();
    Q_INVOKABLE void    curtainPowerClicked(bool);
signals:

public slots:

private:
          QDomElement  m_deviceElement;
          QString     m_roomUid;
};

#endif // LUXDOMOCURTAINCONTROL_H
