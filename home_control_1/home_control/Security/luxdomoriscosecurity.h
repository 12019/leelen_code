#ifndef LUXDOMORISCOSECURITY_H
#define LUXDOMORISCOSECURITY_H

#include<QDeclarativeItem>

class LuxDomoRiscoSecurity : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoRiscoSecurity(QDeclarativeItem *parent = 0);
    ~LuxDomoRiscoSecurity();
    void   componentComplete();
    Q_INVOKABLE   void    init();
    Q_INVOKABLE   void    changDefence(int,int,QString);
    Q_INVOKABLE   void    controlAllDefence(QString);
     void   strCmdSend(int,QString);
signals:
public slots:
private:
    QMap<QString,int>   partitions;
};

#endif // LUXDOMORISCOSECURITY_H
