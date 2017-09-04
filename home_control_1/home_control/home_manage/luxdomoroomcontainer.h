#ifndef LUXDOMOROOMCONTAINER_H
#define LUXDOMOROOMCONTAINER_H

#include<QDeclarativeItem>

class LuxDomoRoomContainer : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoRoomContainer(QDeclarativeItem *parent = 0);
    ~LuxDomoRoomContainer();
    void  componentComplete();
    Q_INVOKABLE   void   init();

    bool      FindChild(QString);
    void      setRoomLocation(QString);
    void      setAddButtonLocation();
    void      AddRoomContainerHeight();
    void      deleteRoomContainerHeight();

    Q_INVOKABLE    void  addRoom();
    void    CreateRoom(QString,QString);

   Q_INVOKABLE   void    showFunctionOption(QString);
   Q_INVOKABLE   void    deleteRoom();
   Q_INVOKABLE   void    setRoomName(QString);

signals:
        void   sigAddRoom(QString,QString,QString);
        void   sigDeleteRoom(QString);
        void   sigChangName(QString,QString);
public slots:
        void  slot_addRoom(QString,QString);
private:
     QDeclarativeItem        *childItem;
     QMap<QString,int>      rooms;
     QDeclarativeItem   *operateRoomItem;
};

#endif // LUXDOMOROOMCONTAINER_H
