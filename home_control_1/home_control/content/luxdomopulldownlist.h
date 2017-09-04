#ifndef LUXDOMOPULLDOWNLIST_H
#define LUXDOMOPULLDOWNLIST_H

#include<QDeclarativeItem>

class LuxDomoPullDownList : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoPullDownList(QDeclarativeItem *parent = 0);
     ~LuxDomoPullDownList();
    void    componentComplete();
     Q_INVOKABLE   void    close();
     Q_INVOKABLE   void    setDeviceData(QString);
signals:

public slots:
};

#endif // LUXDOMOPULLDOWNLIST_H
