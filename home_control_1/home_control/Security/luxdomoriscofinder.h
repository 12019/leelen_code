#ifndef LUXDOMORISCOFINDER_H
#define LUXDOMORISCOFINDER_H

#include<QDeclarativeItem>

class LuxDomoRiscoFinder : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoRiscoFinder(QDeclarativeItem *parent = 0);
    ~LuxDomoRiscoFinder();
    void   componentComplete();
    Q_INVOKABLE   void    setParentH();
    Q_INVOKABLE   void    init();
    Q_INVOKABLE   void    bypassDefenceArea(int,int,int,QString);
    void   strCmdSend(int,QString);
signals:

public slots:
};

#endif // LUXDOMORISCOFINDER_H
