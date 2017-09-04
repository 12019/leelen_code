#ifndef LUXDOMOVERSIONSSETTTING_H
#define LUXDOMOVERSIONSSETTTING_H

#include<QDeclarativeItem>

class LuxDomoVersionsSettting : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoVersionsSettting(QDeclarativeItem *parent = 0);
    ~LuxDomoVersionsSettting();
    void   componentComplete();
signals:

public slots:
};

#endif // LUXDOMOVERSIONSSETTTING_H
