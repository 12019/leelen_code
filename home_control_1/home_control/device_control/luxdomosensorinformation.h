#ifndef LUXDOMOSENSORINFORMATION_H
#define LUXDOMOSENSORINFORMATION_H

#include<QDeclarativeItem>

class LuxDomoSensorInformation : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoSensorInformation(QDeclarativeItem *parent = 0);
    void   componentComplete();
     ~LuxDomoSensorInformation();
    Q_INVOKABLE  void   init(QString,QString);
    Q_INVOKABLE  void   close();
signals:
public slots:
};

#endif // LUXDOMOSENSORINFORMATION_H
