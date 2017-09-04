#ifndef LIGHTCONTROLDIALOG_H
#define LIGHTCONTROLDIALOG_H

#include <QObject>
#include<QDeclarativeItem>
#include<QDeclarativeView>
#include<qdom.h>

class LightControlDialog : public QObject
{
    Q_OBJECT
public:
    explicit LightControlDialog(QObject *parent = 0);
    ~LightControlDialog();
    /*****************stat为要传入qml的值******************/
    Q_PROPERTY(QString stat READ getStat WRITE setStat NOTIFY sendStatChange)        //开关
    Q_PROPERTY(QString value READ getValue WRITE setValue NOTIFY valueChanged)   //亮度值
    Q_PROPERTY(QString label READ getLabel WRITE setLabel NOTIFY sig_Label)           //标签名

    QDomElement lightElement;

    QString getStat(void) const;  //开关
    void setStat(const QString& stat);

    QString getValue(void) const;   //亮度值
    void setValue(const QString& value);

    QString getLabel(void) const;    //标签名
    void setLabel(const QString& label);

    void resetState(QDomElement);
    void Translate();          //翻译
    void pLightRecive(char *buf,QDomElement Element,int len,int deviceAdd);
    QString id;

    Q_INVOKABLE void on_pushButtonBrightness_clicked(int value);
    Q_INVOKABLE void on_pushButtonPower_click(QString cmd);

    
signals:
    void sendStatChange(void);
    void valueChanged(void);
    void sig_Label(void);
    
public slots:
private:
    QString m_Stat;
    QString m_Value;
    QString m_Label;
};

#endif // LIGHTCONTROLDIALOG_H
