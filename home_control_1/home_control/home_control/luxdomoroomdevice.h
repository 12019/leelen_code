#ifndef LUXDOMOROOMDEVICE_H
#define LUXDOMOROOMDEVICE_H

#include<QDeclarativeItem>

class LuxDomoRoomDevice : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoRoomDevice(QDeclarativeItem *parent = 0);
    ~LuxDomoRoomDevice();
      void  componentComplete();

      int    TransformPointX(int);
      int    TransformPointY(int);

      Q_INVOKABLE  void   rememberXY();
      Q_INVOKABLE  void   moveStart();
      Q_INVOKABLE  void   moveStop(int,int);
      Q_INVOKABLE  void   isChangPage(int);
      Q_INVOKABLE  void   otherMoveStart(int,int);
      Q_INVOKABLE  void   otherMoveStop();

      void   DeviceMoveOutFolder();
      void   DeviceMoveInFolder();

      void   DealError();

      Q_INVOKABLE  void    moveParent(int);
      Q_INVOKABLE  void    recordMouseX(int);
      Q_INVOKABLE  void    changPage();

      Q_INVOKABLE  void    controlDevice();
      void      ControlLight();
      void      ControlScene();
      void      CreateElement(QString);
signals:
public slots:
public:
      int  pointX;
      int  pointY;
      QString   oldPageName;
      int   mouseX;
      QDeclarativeItem   *controlItem;
      bool   fromFolder;
};

#endif // LUXDOMOROOMDEVICE_H
