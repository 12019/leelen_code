#ifndef LUXDOMOHOMEPAGESETTING_H
#define LUXDOMOHOMEPAGESETTING_H

#include<QDeclarativeItem>

class LuxDomoHomepageSetting : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoHomepageSetting(QDeclarativeItem *parent = 0);
    ~LuxDomoHomepageSetting();
    void   componentComplete();
    Q_INVOKABLE  void  setHomepage(bool);
signals:
public slots:
};

#endif // LUXDOMOHOMEPAGESETTING_H
