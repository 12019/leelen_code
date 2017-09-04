#ifndef LUXDOMODEVICEFOLDER_H
#define LUXDOMODEVICEFOLDER_H

#include<QDeclarativeItem>

class LuxDomoDeviceFolder : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoDeviceFolder(QDeclarativeItem *parent = 0);
     ~LuxDomoDeviceFolder();
     void   componentComplete();

    Q_INVOKABLE  void   createDeviceControl(QString,QString,QString,QString,QString,QString);

     QString  GetEmptyDeviceLocation(int);
     bool    FindChild(QString);
     int       TransformPointX(int);
     int       TransformPointY(int);
     Q_INVOKABLE  void   moveStart();
     Q_INVOKABLE  void   moveStop(int,int);
     Q_INVOKABLE  void   isChangPage(int);
      void   DealError();

     Q_INVOKABLE   void    setDeviceControlLocation(QString);

     void      CreateFolderPage(QString);
     void      ShowPage(QString);
     Q_INVOKABLE void      leftMove(QString);
     Q_INVOKABLE void      rightMove(QString);

     bool    CheckFolderItem();
     void     showFolderControl();
    Q_INVOKABLE   void    showFolderPage();
    Q_INVOKABLE   void    hideFolderPage();

    Q_INVOKABLE   void   deviceMoveOutFolder(QString);

     void   SaveDeviceLocation(QString,QString);
     Q_INVOKABLE   void   changAllDeviceLocation();
     Q_INVOKABLE   void   changName(QString);
signals:
      void  sigDeviceLocation(QString,QString,QString,QString,QString,QString);
      void  sigFolderLoaction(QString,QString,QString,QMap<QString,int>);
      void  sigPartDeviceLocation(QString,int,QMap<QString,int>);
      void  sigchangFolderName(QString,QString,QMap<QString,int>);
public slots:

private:
     QMap<QString,int>              deviceControls;
     QVector<QString>                folderPages;
     QDeclarativeItem                *childItem;
     QString    oldPageName;
      int   pointX;
      int   pointY;
};

#endif // LUXDOMODEVICEFOLDER_H
