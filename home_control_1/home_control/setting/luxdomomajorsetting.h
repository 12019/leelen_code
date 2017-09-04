#ifndef LUXDOMOMAJORSETTING_H
#define LUXDOMOMAJORSETTING_H

#include<QDeclarativeItem>

class LuxDomoMajorSetting : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoMajorSetting(QDeclarativeItem *parent = 0);
    ~LuxDomoMajorSetting();
    void  componentComplete();
signals:

public slots:
};

#endif // LUXDOMOMAJORSETTING_H
