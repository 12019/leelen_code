#ifndef LUXDOMOABOUTSETTING_H
#define LUXDOMOABOUTSETTING_H

#include<QDeclarativeItem>

class LuxDomoAboutSetting : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoAboutSetting(QDeclarativeItem *parent = 0);
    ~LuxDomoAboutSetting();
    void   componentComplete();

     Q_INVOKABLE  void    init();
signals:

public slots:
};

#endif // LUXDOMOABOUTSETTING_H
