#ifndef LUXDOMOTOOLBAR_H
#define LUXDOMOTOOLBAR_H

#include<QDeclarativeItem>

class LuxDomoToolbar : public QDeclarativeItem
{
    Q_OBJECT
public:
     explicit LuxDomoToolbar(QDeclarativeItem *parent = 0);
     ~LuxDomoToolbar();
     void    componentComplete();
    Q_INVOKABLE   void    back();
signals:

public slots:
};

#endif // LUXDOMOTOOLBAR_H
