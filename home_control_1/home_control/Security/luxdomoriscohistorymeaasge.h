#ifndef LUXDOMORISCOHISTORYMEAASGE_H
#define LUXDOMORISCOHISTORYMEAASGE_H

#include<QDeclarativeItem>

class LuxDomoRiscoHistoryMeaasge : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoRiscoHistoryMeaasge(QDeclarativeItem *parent = 0);
    ~LuxDomoRiscoHistoryMeaasge();
    void  componentComplete();
    Q_INVOKABLE   void    init();
    Q_INVOKABLE   void    showEvent(QString);
signals:

public slots:
};

#endif // LUXDOMORISCOHISTORYMEAASGE_H
