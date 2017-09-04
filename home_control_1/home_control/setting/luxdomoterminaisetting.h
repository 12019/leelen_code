#ifndef LUXDOMOTERMINAISETTING_H
#define LUXDOMOTERMINAISETTING_H

#include<QDeclarativeItem>

class LuxDomoTerminaiSetting : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoTerminaiSetting(QDeclarativeItem *parent = 0);
    ~LuxDomoTerminaiSetting();
    void   componentComplete();
    Q_INVOKABLE  void    editInformation(QString,QString);
    Q_INVOKABLE  void    saveInformation();
    Q_INVOKABLE  void    setInformation(QString);
signals:

public slots:
};

#endif // LUXDOMOTERMINAISETTING_H
