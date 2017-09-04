#ifndef LUXDOMOACEDIT_H
#define LUXDOMOACEDIT_H

#include<QDeclarativeItem>
#include<QDomElement>
class LuxDomoAcEdit : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoAcEdit(QDeclarativeItem *parent = 0);
    ~LuxDomoAcEdit();
    void  componentComplete();
    Q_INVOKABLE   void     init(QString,QString);
    Q_INVOKABLE   void     close();
    Q_INVOKABLE   void     save();

    Q_INVOKABLE   void     showModeList();
    Q_INVOKABLE   void     showWindList();
    Q_INVOKABLE   void     showTempList();
    Q_INVOKABLE   void     showSpeedList();

    void     resourceRelease();
signals:

public slots:

private:
    QDomElement  m_deviceElement;
    int   m_acAttr;
    int   m_acAddr;
    QDeclarativeItem       *listItem;
    QDeclarativeEngine    *listEngine;
};

#endif // LUXDOMOACEDIT_H
