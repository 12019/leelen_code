#ifndef LUXDOMOVAR_LIGHTCONTROL_H
#define LUXDOMOVAR_LIGHTCONTROL_H

#include<QDeclarativeItem>
#include<QDomElement>
class LuxDomoVar_LightControl : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoVar_LightControl(QDeclarativeItem *parent = 0);
    ~LuxDomoVar_LightControl();
    void  componentComplete();
    Q_INVOKABLE  void   init(QString,QString);
    Q_INVOKABLE  void   close();
    Q_INVOKABLE  void   lightPowerClicked(bool);
    Q_INVOKABLE  void   lightBrightnessClicked(int);
    QString   GetBrightnessValue(int);
signals:
public slots:
private:
    QDomElement   m_deviceElement;
    QString               m_roomUid;
};

#endif // LUXDOMOVAR_LIGHTCONTROL_H
