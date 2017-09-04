#ifndef LUXDOMORISCOCONTROL_H
#define LUXDOMORISCOCONTROL_H

#include<QDeclarativeItem>

class LuxDomoRiscoControl : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoRiscoControl(QDeclarativeItem *parent = 0);
    ~LuxDomoRiscoControl();
    void  componentComplete();
    Q_INVOKABLE   void    init();
    Q_INVOKABLE   void    activateControl(int,bool);
     void   strCmdSend(int,bool);
signals:

public slots:
};

#endif // LUXDOMORISCOCONTROL_H
