#ifndef LUXDOMOMAININTERFACE_H
#define LUXDOMOMAININTERFACE_H

#include<QDeclarativeItem>

class LuxDomoMainInterface : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoMainInterface(QDeclarativeItem *parent = 0);
    ~LuxDomoMainInterface();
    void      componentComplete();
    void      Init();

    bool      FindChild(QString);
    void      CreateElement(QString,QString,QString);
    void      setSceneLocation(QString);
    void      AddSceneContainerHeight();
    void      DeleteSceneContainerHeight();

    Q_INVOKABLE  void   showRoomControl(QString);
    Q_INVOKABLE  void   useHomeScene(QString);
signals:
public slots:
    void   slot_addElement(QString,QString,QString);
    void   slot_deleteElement(QString);
    void   slot_changName(QString,QString);
    void   slot_changScene(QString,QString);
public:
    QMap<QString,int>     scenes;
    QDeclarativeItem   *childItem;
    QMap<QString,int>     m_scenes;
    QMap<QString,int>     m_rooms;
};

#endif // LUXDOMOMAININTERFACE_H
