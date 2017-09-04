#ifndef MUSICCONTROLDIALOG_H
#define MUSICCONTROLDIALOG_H

#include <QObject>
#include<QtDeclarative/QDeclarativeView>
#include<QDeclarativeEngine>
#include<QDeclarativeComponent>
#include<QDeclarativeItem>
#include<QDeclarativeView>
#include<QDeclarativeContext>
#include<qdom.h>

class MusicControlDialog : public QObject
{
    Q_OBJECT
public:
    explicit MusicControlDialog(QObject *parent = 0);
    ~MusicControlDialog();

    Q_PROPERTY(bool isPlay READ getStat WRITE setStat NOTIFY sig_stat)    //播放暂停
    Q_PROPERTY(QString source READ getSource WRITE setSource NOTIFY sig_source)  //资源
    Q_PROPERTY(bool mute READ getmute WRITE setmute NOTIFY sig_mute)   //静音非静音
    Q_PROPERTY(QString power READ getpower WRITE setpower NOTIFY sig_power)   //开关
    Q_PROPERTY(QString vol READ vol WRITE setVol NOTIFY sig_Vol)  //音量
    Q_PROPERTY(QString label1 READ label1 WRITE setLabel1 NOTIFY sig_Label1)  //音量
    Q_PROPERTY(QString label2 READ label2 WRITE setLabel2 NOTIFY sig_Label2)  //音量



    QString id;

    bool getStat(void) const;  //播放暂停
    void setStat(const bool& stat);

    QString getSource(void) const;  //资源
    void setSource(const QString& bsource);

    bool getmute(void) const;  //静音非静音
    void setmute(const bool& stat);

    QString getpower(void) const;  //开关
    void setpower(const QString& bsource);

    QString vol(void) const;    //音量
    void setVol(const QString& vol);

    QString label1(void) const;    //标签名称
    void setLabel1(const QString& label1);

    QString label2(void) const;    //声音模式
    void setLabel2(const QString& label2);

    Q_INVOKABLE void on_pushButtonBMUnmute_clicked();
    Q_INVOKABLE void on_pushButtonBMVolume_clicked(QString action);
    Q_INVOKABLE void on_pushButtonBMPause_clicked();
    Q_INVOKABLE void on_pushButtonBMPlay_clicked();
    Q_INVOKABLE void on_pushButtonBMNextMusic_clicked();
    Q_INVOKABLE void on_pushButtonBMPrevMusic_clicked();
    Q_INVOKABLE void on_pushButtonBMSource_clicked(QString action);
    Q_INVOKABLE void on_pushButtonBMMute_clicked();
    Q_INVOKABLE void on_pushButtonBMPower_clicked(QString action);
    Q_INVOKABLE bool musicPowerStat();

    void musicstrCmdSend(int cmd);
    void resetState(QDomElement);
    void Translate();          //翻译
    void pMusicRecive(char *buf, QDomElement Element,QString name);
    QDomElement deviceElement;

    bool musicIsPowerOn;
signals:
    void sig_stat(void);
    void sig_source(void);
    void sig_mute(void);
    void sig_power(void);
    void sig_Label1(void);
    void sig_Label2(void);
    void sig_Vol(void);
public slots:
private:
    bool musicIsPlay;
    QString m_Source;
    bool m_Mute;
    QString Power;
    QString m_Vol;
    QString m_Label1;
    QString m_Label2;
};

#endif // MUSICCONTROLDIALOG_H
