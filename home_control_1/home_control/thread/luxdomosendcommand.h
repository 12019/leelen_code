#ifndef LUXDOMOSENDCOMMAND_H
#define LUXDOMOSENDCOMMAND_H

#include <QObject>

class LuxDomoSendCommand : public QObject
{
    Q_OBJECT
public:
    explicit LuxDomoSendCommand(QObject *parent = 0);
    ~LuxDomoSendCommand();
    void    ChangDeviceAttribute(QString,QString,QString,QString);

    void    CurtainPowerControl(QString,int,QString,QString);

    void    LightPowerControl(QString,int,QString,QString);
    void    LightBrightnessControl(QString,int,QString,QString);

    void    TvPowerControl(QString,int,QString,QString);
    void    TvModeControl(QString,int,QString,QString);
    int       obtainTVFunOrder(QString,QString);

    void    MusicPowerControl(QString,int,int,QString,QString);
    void    MusicModeControl(QString,int,int,QString,QString);
    void    MusicPauseControl(QString,int,int,QString,QString);
    void    MusicVolumeControl(QString,int,int,QString,QString);
    int       obtainMusicFunOrder(QString,QString);

    void     AcTempControl(QString,int,QString,QString);
    void     AcWindControl(QString,int,QString,QString);
    void     AcModeControl(QString,int,QString,QString);
    void     AcPowerControl(QString,int,QString,QString);
    void     AcSpeedControl(QString,int,QString,QString);
    bool    AcInfraredControl(QString,QString,QString,QString,QString,int,QString,QString);
signals:
public slots:
};

#endif // LUXDOMOSENDCOMMAND_H
