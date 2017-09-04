#ifndef ACCONTROLDIALOG_H
#define ACCONTROLDIALOG_H

#include <QObject>
#include<QtDeclarative/QDeclarativeView>
#include<QDeclarativeEngine>
#include<QDeclarativeComponent>
#include<QDeclarativeItem>
#include<QDeclarativeView>
#include<QDeclarativeContext>
#include<qdom.h>

class AccontrolDialog : public QObject
{
    Q_OBJECT
public:
    explicit AccontrolDialog(QObject *parent = 0);
    ~AccontrolDialog();

    Q_PROPERTY(QString stat READ getStat WRITE setStat NOTIFY sig_stat)                    //开关
    Q_PROPERTY(QString mode READ getMode WRITE setMode NOTIFY sig_mode)      //模式
    Q_PROPERTY(QString temp READ getTemp WRITE setTemp NOTIFY sig_temp)         //温度
    Q_PROPERTY(QString wind READ getWind WRITE setWind NOTIFY sig_wind)            //风速
    Q_PROPERTY(QString swing READ getSwing WRITE setSwing NOTIFY sig_swing)       //摆动
    Q_PROPERTY(QString label READ getLabel WRITE setLabel NOTIFY sig_Label)           //标签名

    QString getStat(void) const;  //开关
    void setStat(const QString& stat);

    QString getMode(void) const;    //模式
    void setMode(const QString& mode);

    QString getTemp(void) const;       //温度
    void setTemp(const QString& temp);

    QString getWind(void) const;        //风速
    void setWind(const QString& wind);

    QString getSwing(void) const;    //摆动
    void setSwing(const QString& swing);

    QString getLabel(void) const;    //标签名
    void setLabel(const QString& label);

    bool isPowerOn;
    QDomElement deviceElement;
    QString id;
    short attr;

    Q_INVOKABLE void on_pushButtonACTemperature_clicked(QString action);
    Q_INVOKABLE void on_pushButtonACWind_clicked(QString action);
    Q_INVOKABLE void on_pushButtonACMode_clicked(QString action);
    Q_INVOKABLE bool on_pushButtonACSwing_clicked(QString action);
    Q_INVOKABLE void on_pushButtonACPower_clicked(QString action);

    void resetState(QDomElement);
    void acstrCmdSend(int cmd);
    void Translate();          //翻译
    void expendCmd(int len,int function,int cmd);
    void pAcRecive(char *buf, QDomElement Element,QString name);
    //QString global_ac_mode_display(short);
    //QString global_ac_wind_display(short);
    
signals:
    void sig_stat(void);
    void sig_mode(void);
    void sig_temp(void);
    void sig_swing(void);
    void sig_wind(void);
    void sig_Label(void);
    
public slots:

private:
    QString m_Stat;
    QString m_Mode;
    QString m_Tempture;
    QString m_Swing;
    QString m_Wind;
    QString m_Label;
};

#endif // ACCONTROLDIALOG_H
