#ifndef LUXDOMOCHANGDEVICEDATA_H
#define LUXDOMOCHANGDEVICEDATA_H

#include <QObject>
#include<QMap>
#include<QVector>
#include<QDomDocument>

class LuxDomoChangDeviceData : public QObject
{
    Q_OBJECT
public:
    explicit LuxDomoChangDeviceData(QObject *parent = 0);
    QString    TranslateGBKChar(char*);
    void     TranslateDeviceCmd(char*);
    void     TranslateInfraredTvCmd(QDomElement,char*,QString);
    void     TranslateInfraredAcCmd(QDomElement,char*,QString);
    void     TranslateInfraredMusicCmd(QDomElement,char*,QString);
    void     TranslateAcCmd(QDomElement,char*,QString);
    void     TranslateLightCmd(QDomElement,char*,QString);
    void     TranslateVar_LightCmd(QDomElement,char*,QString);
    void     TranslateCurtainCmd(QDomElement,char*,QString);
    void     TranslateMusicCmd(QDomElement,char*,QString);
    void     RefreshRoomControlInterface(QString,QString,QString,QString);
    void     TranslatePMCmd(QDomElement,char*,QString);
    void     TranslateHumitureCmd(QDomElement,char*,QString);
    void     TranslateSecurityCmd(char);
signals:
    void     sigRefreshRoomControlInterfaceFromCmd(QByteArray);
    void     sigRefreshSensorValueFromCmd(QString,QString,QString,QString,QString);
public slots:   
    void   slot_changDeviceLocation(QString,QString,QString,QString,QString,QString);
    void   slot_changFolderLocation(QString,QString,QString,QMap<QString,int>);
    void   slot_changPartDeviceLocation(QString,int,QMap<QString,int>);
    void   slot_changFolderName(QString,QString,QMap<QString,int>);
    void   slot_changDeviceAttribute(QString,QString,QString,QString);
public:
};

#endif // LUXDOMOCHANGDEVICEDATA_H
