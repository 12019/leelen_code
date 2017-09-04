#ifndef HOMECONTROLSCENEEDIT_H
#define HOMECONTROLSCENEEDIT_H

#include <QObject>
#include<QtDeclarative/QDeclarativeView>
#include<QDeclarativeEngine>
#include<QDeclarativeComponent>
#include<QDeclarativeItem>
#include<QDeclarativeView>
#include<QDeclarativeContext>
#include<qdom.h>

class HomeControlSceneEdit : public QObject
{
    Q_OBJECT
public:

    explicit HomeControlSceneEdit(QObject *parent = 0);

    Q_PROPERTY(QString mode READ getMode WRITE setMode NOTIFY sig_mode)      //模式
    Q_PROPERTY(QString temp READ getTemp WRITE setTemp NOTIFY sig_temp)         //温度
    Q_PROPERTY(QString wind READ getWind WRITE setWind NOTIFY sig_wind)            //风速
    Q_PROPERTY(QString swing READ getSwing WRITE setSwing NOTIFY sig_swing)       //摆动

    Q_PROPERTY(QString mute READ mute WRITE setMute NOTIFY sig_Mute)   //静音非静音
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sig_Source)  //资源

    Q_PROPERTY(QString finish READ finish WRITE setFinish NOTIFY sig_Finish)  //完成
    Q_PROPERTY(QString cancel READ cancel WRITE setCancel NOTIFY sig_Cancel)  //取消

    QString getMode(void) const;    //模式
    void setMode(const QString& mode);

    QString getTemp(void) const;       //温度
    void setTemp(const QString& temp);

    QString getWind(void) const;        //风速
    void setWind(const QString& wind);

    QString getSwing(void) const;    //摆动
    void setSwing(const QString& swing);

    QString mute(void) const;  //静音非静音
    void setMute(const QString& mute);

    QString source(void) const;  //资源
    void setSource(const QString& source);

    QString finish(void) const;  //完成
    void setFinish(const QString& finish);

    QString cancel(void) const;  //取消
    void setCancel(const QString& cancel);

    QString m_sceneXmlFileName;

    QDomDocument doc;
    QDomElement m_deviceElement;
    QDomElement n_deviceElement;

    Q_INVOKABLE void setDevAllOn();
    Q_INVOKABLE void setDevAllOff();

    Q_INVOKABLE void close();
    Q_INVOKABLE bool loadDevice(int id);
    Q_INVOKABLE void eidtDevice(bool bPowerOn,QString devId,QString devType,bool ir,QString actflag);
    Q_INVOKABLE void editFinish();
    Q_INVOKABLE void irDevEidt(QString type,QString arg);

    Q_INVOKABLE void pushButtonACTemperature(QString action);
    Q_INVOKABLE void pushButtonACWind(QString action);
    Q_INVOKABLE void pushButtonACMode(QString action);
    Q_INVOKABLE void pushButtonACSwing(QString action);

    Q_INVOKABLE void pushButtonMusicMute(QString action);
    Q_INVOKABLE void pushButtonBMSource(QString action);
    Q_INVOKABLE QString isEnable(QString IsEnable);

    void Translate();          //翻译
    
signals:
    void sig_mode(void);
    void sig_temp(void);
    void sig_swing(void);
    void sig_wind(void);
    void sig_Mute(void);
    void sig_Source(void);
    void sig_Finish(void);
    void sig_Cancel(void);
public slots:

private:
    QString m_Mode;
    QString m_Tempture;
    QString m_Swing;
    QString m_Wind;
    QString m_Mute;
    QString m_Source;
    QString m_Cancel;
    QString m_Finish;
};

#endif // HOMECONTROLSCENEEDIT_H
