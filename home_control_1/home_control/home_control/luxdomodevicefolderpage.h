#ifndef LUXDOMODEVICEFOLDERPAGE_H
#define LUXDOMODEVICEFOLDERPAGE_H

#include<QDeclarativeItem>

class LuxDomoDeviceFolderPage : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoDeviceFolderPage(QDeclarativeItem *parent = 0);
    ~LuxDomoDeviceFolderPage();
    void  componentComplete();
    Q_INVOKABLE   void    setDeviceControlLocation(QString,int);
    QString  GetEmptyDeviceLocation(int);
    bool   FindDevice(QString);
    Q_INVOKABLE   void     folderPageMove(QString);
    Q_INVOKABLE   void     closeFolderPage();
    Q_INVOKABLE   void     removeAllDevice();
    Q_INVOKABLE   void     deviceMoveOutFolder(QString);
    Q_INVOKABLE   void     changFolderName();
    Q_INVOKABLE   void     on_offDevice(QString,bool);
signals:
public slots:
private:
    QMap<QString,int>       deviceControls;
    QDeclarativeItem         *deviceItem;
};

#endif // LUXDOMODEVICEFOLDERPAGE_H
