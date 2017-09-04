#ifndef LUXDOMOMUSICEDIT_H
#define LUXDOMOMUSICEDIT_H

#include<QDeclarativeItem>
#include<QDomElement>

class LuxDomoMusicEdit : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoMusicEdit(QDeclarativeItem *parent = 0);
   ~LuxDomoMusicEdit();
    void  componentComplete();
    void  resourceRelease();
    Q_INVOKABLE   void     init(QString,QString);
    Q_INVOKABLE   void     close();
    Q_INVOKABLE   void     save();
    Q_INVOKABLE   void     showModeList();
    Q_INVOKABLE   void     showVolumeList();
    Q_INVOKABLE   void     showConditionList();
signals:
public slots:
private:
    QDomElement  m_deviceElement;
    QDeclarativeItem       *listItem;
    QDeclarativeEngine    *listEngine;
};

#endif // LUXDOMOMUSICEDIT_H
