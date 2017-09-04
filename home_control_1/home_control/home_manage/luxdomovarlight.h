#ifndef LUXDOMOVARLIGHT_H
#define LUXDOMOVARLIGHT_H

#include<QDeclarativeItem>
#include<QDomElement>

class LuxDomoVarLight : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoVarLight(QDeclarativeItem *parent = 0);
    ~LuxDomoVarLight();
    void  componentComplete();
    void  resourceRelease();
    Q_INVOKABLE   void     init(QString,QString);
    Q_INVOKABLE   void     close();
    Q_INVOKABLE   void     save();

    Q_INVOKABLE   void     showVolumeList();
signals:

public slots:
private:
    QDomElement  m_deviceElement;
    QDeclarativeItem       *listItem;
    QDeclarativeEngine    *listEngine;
};

#endif // LUXDOMOVARLIGHT_H
