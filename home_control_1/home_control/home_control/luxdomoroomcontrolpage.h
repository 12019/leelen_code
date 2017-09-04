#ifndef LUXDOMOROOMCONTROLPAGE_H
#define LUXDOMOROOMCONTROLPAGE_H

#include<QDeclarativeItem>

class LuxDomoRoomControlPage : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoRoomControlPage(QDeclarativeItem *parent = 0);
    ~LuxDomoRoomControlPage();
    void   componentComplete();

    Q_INVOKABLE   void     rightMove();
    Q_INVOKABLE   void     leftMove();
    Q_INVOKABLE   void     back();

    void   AddDevice(QString,QString,QString,QString,QString,QString,QString,QString);

    void   CreateDeviceControl(QString,QString,QString,QString,QString,int);
    void   CreateDeviceControl(QString,QString,QString,QString,QString);
    void   CreateFolder(QString,QString,int,int);

    void   AddDeviceControl(QDeclarativeItem*,int,int);
    void   AddFolderControl(QDeclarativeItem*,int,int);

    bool      FindChild(QString);
    QString  GetEmptyDeviceLocation();
    void      RemoveDevice(QString);
    void      RemoveFolder(QString);

    bool      DealDeviceMove(QDeclarativeItem*,int ,int);
    bool      DeviceMoveNewLocation(QDeclarativeItem*,int ,int);
    bool      DeviceMoveToNewFolder(QDeclarativeItem*,int,int);
    bool      DeviceMoveToFolder(QDeclarativeItem*,int,int);

   bool      DeleteFolder(QDeclarativeItem*,QString);

    bool    DealFolderMove(QDeclarativeItem*,int ,int);
    bool    FolderMoveNewLocation(QDeclarativeItem*,int ,int);
    bool    FolderMoveToDevice(QDeclarativeItem*,int,int);
    bool    FolderMoveToFolder(QDeclarativeItem*,int,int);

    void      HideAllItem();
    void      ShowAllItem();

    void      Printf();
    Q_INVOKABLE  void    on_offDevice(QString,bool);
signals:
          void  sigDeviceLocation(QString,QString,QString,QString,QString,QString);
public slots:

public:

private:
     QDeclarativeItem   *childItem;
     QMap<QString,QString>     devices;
     QMap<QString,QString>      folders;
     int    deviceMatrix[5][4];
};

#endif // LUXDOMOROOMCONTROLPAGE_H
