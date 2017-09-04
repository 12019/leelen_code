#ifndef LUXDOMODEVICECONTAINER_H
#define LUXDOMODEVICECONTAINER_H

#include<QDeclarativeItem>

class LuxDomoDeviceContainer : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoDeviceContainer(QDeclarativeItem *parent = 0);
    ~LuxDomoDeviceContainer();
    void     componentComplete();
   Q_INVOKABLE    void   init();

    bool       FindChild(QString);
    void       setAddButtonLocation();
    void       setDeviceLocation(QString);
    void       AddDeviceContainerHeight();
    void       deleteDeviceContainerHeight();

    Q_INVOKABLE   void     showDeviceOption();

    Q_INVOKABLE    void  addDevice(QString,QString,QString,QString);
    void    CreateDevice(QString,QString,QString,QString,QString);

    Q_INVOKABLE   void     setDeviceInforMation(QString);
    Q_INVOKABLE   void     deleteDevice(QString);
    Q_INVOKABLE   void     setOperateDevice(QString);
signals:
       void     sigAddDevice(QByteArray);
       void     sigDeleteDevice(QByteArray);
       void     sigChangDeviceInforMation(QByteArray);
public slots:
       void     slot_addDevice(QString,QString,QString,QString,QString);
private:
       QDeclarativeItem     *childItem;
       QMap<QString,int>     devices;
       QDeclarativeItem      *deviceOptionDialog;
       QString      operateDevice;
};

#endif // LUXDOMODEVICECONTAINER_H
