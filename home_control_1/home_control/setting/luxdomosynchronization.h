#ifndef LUXDOMOSYNCHRONIZATION_H
#define LUXDOMOSYNCHRONIZATION_H

#include<QDeclarativeItem>

class LuxDomoSynchronization : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoSynchronization(QDeclarativeItem *parent = 0);
    ~LuxDomoSynchronization();
    void   componentComplete();
    void   ChangVersion();
    Q_INVOKABLE  void    fileSynchronization();
signals:

public slots:
};

#endif // LUXDOMOSYNCHRONIZATION_H
