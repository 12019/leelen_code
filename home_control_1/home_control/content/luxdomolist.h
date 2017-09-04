#ifndef LUXDOMOLIST_H
#define LUXDOMOLIST_H

#include<QDeclarativeItem>

class LuxDomoList : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoList(QDeclarativeItem *parent = 0);
    ~LuxDomoList();
   void    componentComplete();
    Q_INVOKABLE   void    setDeviceData(QString);
signals:

public slots:
};

#endif // LUXDOMOLIST_H
