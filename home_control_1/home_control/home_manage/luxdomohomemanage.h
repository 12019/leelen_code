#ifndef LUXDOMOHOMEMANAGE_H
#define LUXDOMOHOMEMANAGE_H

#include<QDeclarativeItem>
#include"content/luxdomotoolbar.h"
#include"luxdomoroomcontainer.h"

class LuxDomoHomeManage : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoHomeManage(QDeclarativeItem *parent = 0);
    ~LuxDomoHomeManage();
    void      componentComplete();
    void      init();
    bool      FindChild(QString);
    void      CreateElement(QString,bool  type=true);
    void      EditRoomName(QString);
signals:

public slots:

public:
       LuxDomoToolbar    *m_pToolbar;
       LuxDomoRoomContainer      *m_pRoomcontainer;
      QDeclarativeItem   *childItem;
};

#endif // LUXDOMOHOMEMANAGE_H
