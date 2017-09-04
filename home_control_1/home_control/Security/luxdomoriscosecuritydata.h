#ifndef LUXDOMORISCOSECURITYDATA_H
#define LUXDOMORISCOSECURITYDATA_H

#include <QObject>
#include<QStringList>
class LuxDomoRiscoSecurityData : public QObject
{
    Q_OBJECT
public:
    explicit LuxDomoRiscoSecurityData(QObject *parent = 0);

    QStringList   GetDefenceAreaCount(char);
    void    DealRiscoSecurityInformation(char*);
    void    DealPartition(char*);
    void    DealDefenceArea(char*);
    void    DealControl(char*);

    void    SetPartitionCondition(int,QString);
    void    SetDefenceAreaCondition(int,int,QString);
    void    SetOutputCondition(int,QString);

    QStringList    GetPartition();
    QStringList    GetDefenceArea();
    QStringList    GetEvent(QString);
    QStringList    GetOutput();

    void   PartitionDefence(int,QString);
    void   DefenceAreaDypass(int,int,QString);

    QString   ReceiveDefenceAreaCondition(QString);
    QString   ReceivePartitionCondition(QString);
    QString   ConditionTransition1(QString);
    QString   ConditionTransition2(QString,QString);

    void     AddRiscoEvent(QString,QString,QString);
signals:

public slots:
public:
};

#endif // LUXDOMORISCOSECURITYDATA_H
