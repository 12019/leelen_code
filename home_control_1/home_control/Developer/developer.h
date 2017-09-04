#ifndef DEVELOPER_H
#define DEVELOPER_H

#include <QObject>
#include<qdom.h>
#include<pthread.h>

class Developer : public QObject
{
    Q_OBJECT
public:
    explicit Developer(QObject *parent = 0);
    Q_INVOKABLE void setState(QString stat);
    Q_INVOKABLE QString layerList();
    Q_INVOKABLE void setLayer(QString attr);
    Q_INVOKABLE QString roomList();
    Q_INVOKABLE void setRoom(QString attr);
    Q_INVOKABLE QString deviceInfo(QString id,QString type);
    Q_INVOKABLE void saveconfigFile(QString type,QString attr,QString name,QString addr);
    Q_INVOKABLE void setDevName(QString name);
    Q_INVOKABLE void setDevAddr(QString name);
    void saveHouseMapUser(QString type,QString id,QString name);
    void saveHouseMapDefault(QString type,QString id,QString nam);
    QDomElement mlayerElement;
    QDomElement mroomElement;
    QDomElement mdeviceElement;
    QDomDocument doc;
    QString house_map_user_x;

    pthread_t  LoadDevFD;
signals:

public slots:
};

#endif // DEVELOPER_H
