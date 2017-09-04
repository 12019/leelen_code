#ifndef LUXDOMOTVEDIT_H
#define LUXDOMOTVEDIT_H

#include<QDeclarativeItem>
#include<QDomElement>

class LuxDomoTvEdit : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoTvEdit(QDeclarativeItem *parent = 0);
    ~LuxDomoTvEdit();
    void  componentComplete();
    void  resourceRelease();
    Q_INVOKABLE   void     init(QString,QString);
    Q_INVOKABLE   void     close();
    Q_INVOKABLE   void     save();
    Q_INVOKABLE   void     showModeList();
    Q_INVOKABLE   void     showVolumeList();
signals:
public slots:
private:
    QDomElement  m_deviceElement;
    QDeclarativeItem       *listItem;
    QDeclarativeEngine    *listEngine;
};

#endif // LUXDOMOTVEDIT_H
