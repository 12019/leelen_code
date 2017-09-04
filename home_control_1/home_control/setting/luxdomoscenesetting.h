#ifndef LUXDOMOSCENESETTING_H
#define LUXDOMOSCENESETTING_H

#include<QDeclarativeItem>

class LuxDomoSceneSetting : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoSceneSetting(QDeclarativeItem *parent = 0);
    ~LuxDomoSceneSetting();
    void  componentComplete();
    Q_INVOKABLE  void   init(QString,QString);
    Q_INVOKABLE  void   changSceneData(QString,QString,QString,QString,int index=-1);
    Q_INVOKABLE  void   close();
    Q_INVOKABLE  void   editDevice(QString,QString);
    void      CreateElement(QString);
signals:
    void     sigChangData(QByteArray);
public slots:
    void     slot_addListData(QByteArray);
    void     slot_changListData(QString,QString);
private:
    int   m_deviceCount;
    QMap<QString,int>  m_devices;
    QDeclarativeItem       *editItem;
    QDeclarativeEngine    *editEngine;
};

#endif // LUXDOMOSCENESETTING_H
