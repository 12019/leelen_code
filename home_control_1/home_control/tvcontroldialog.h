#ifndef TVCONTROLDIALOG_H
#define TVCONTROLDIALOG_H

#include <QObject>
#include<QtDeclarative/QDeclarativeView>
#include<QDeclarativeEngine>
#include<QDeclarativeComponent>
#include<QDeclarativeItem>
#include<QDeclarativeView>
#include<QDeclarativeContext>
#include<qdom.h>

class TVControlDialog : public QObject
{
    Q_OBJECT
public:
    explicit TVControlDialog(QObject *parent = 0);
    ~TVControlDialog();

    QString id;

    Q_PROPERTY(QString stat READ getStat WRITE setStat NOTIFY sig_stat)   //开关
    Q_PROPERTY(QString mode READ getMode WRITE setMode NOTIFY sig_mode)  //模式
    Q_PROPERTY(QString channel READ channel WRITE setChannel NOTIFY sig_Channel)  //频道
    Q_PROPERTY(QString vol READ vol WRITE setVol NOTIFY sig_Vol)  //音量
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY sig_Label)  //标签名称

    bool tvIsPowerOn;

    QString getStat(void) const;  //开关
    void setStat(const QString& stat);

    QString getMode(void) const;    //模式
    void setMode(const QString& mode);

    QString channel(void) const;    //频道
    void setChannel(const QString& channel);

    QString vol(void) const;    //音量
    void setVol(const QString& vol);

    QString label(void) const;    //标签名称
    void setLabel(const QString& label);

    Q_INVOKABLE void on_pushButtonTVIVolume_clicked(QString action);
    Q_INVOKABLE void on_pushButtonTVChannel_clicked(QString action);
    Q_INVOKABLE void on_pushButtonTVMode_clicked();
    Q_INVOKABLE void on_pushButtonTVPower_clicked(QString action);

    void resetState(QDomElement);
    void tvstrCmdSend(int cmd);
    void Translate();          //翻译
    void pTVRecive(char *buf, QDomElement Element,QString name);

    QDomElement deviceElement;
signals:
    void sig_stat(void);
    void sig_mode(void);
    void sig_Channel(void);
    void sig_Vol(void);
    void sig_Label(void);
public slots:

private:
    QString m_Stat;
    QString m_Mode;
    QString m_Vol;
    QString m_Channel;
    QString m_Label;
};

#endif // TVCONTROLDIALOG_H
