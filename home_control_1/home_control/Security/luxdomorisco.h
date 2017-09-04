#ifndef LUXDOMORISCO_H
#define LUXDOMORISCO_H

#include<QDeclarativeItem>

class LuxDomoRisco : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoRisco(QDeclarativeItem *parent = 0);
    ~LuxDomoRisco();
    void    componentComplete();
    void    CreateElement(QString);
    Q_INVOKABLE   void    chooseFunction(QString);
signals:

public slots:

private:
    QDeclarativeItem      *showingItem;
    QDeclarativeEngine   *showingEngine;
    QString    condition;
};

#endif // LUXDOMORISCO_H
