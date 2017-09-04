#ifndef LUXDOMOCLOUDSETTING_H
#define LUXDOMOCLOUDSETTING_H

#include<QDeclarativeItem>

class LuxDomoCloudSetting : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoCloudSetting(QDeclarativeItem *parent = 0);
   ~LuxDomoCloudSetting();
    void   componentComplete();
    Q_INVOKABLE  void    editInformation(QString,QString);
    Q_INVOKABLE  void    saveInformation();
    Q_INVOKABLE  void    setInformation(QString);
signals:

public slots:
};

#endif // LUXDOMOCLOUDSETTING_H
