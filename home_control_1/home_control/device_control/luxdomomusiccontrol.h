#ifndef LUXDOMOMUSICCONTROL_H
#define LUXDOMOMUSICCONTROL_H

#include<QDeclarativeItem>
#include<QDomElement>
class LuxDomoMusicControl : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoMusicControl(QDeclarativeItem *parent = 0);
    ~LuxDomoMusicControl();
    void  componentComplete();
    Q_INVOKABLE  void   init(QString,QString);
    Q_INVOKABLE  void   findMusicInformation();
    Q_INVOKABLE  void   close();
    void    MusicstrCmdSend(int);
    Q_INVOKABLE void    musicPowerClicked();
    Q_INVOKABLE void    musicModeClicked();
    Q_INVOKABLE void    musicPreviousClicked();
    Q_INVOKABLE void    musicPauseClicked(QString);
    Q_INVOKABLE void    musicNextClicked();
    Q_INVOKABLE void    musicVolumeClicked(int);
    Q_INVOKABLE void    musicChannelClicked();
    Q_INVOKABLE void    musicMuteClicked(QString);
    int   obtainMusicFunOrder(QString);
signals:
public slots:
private:
    QDomElement  m_deviceElement;
    int   m_musicAttr;
    int   m_musicAddr;
    QString  m_roomUid;
};

#endif // LUXDOMOMUSICCONTROL_H
