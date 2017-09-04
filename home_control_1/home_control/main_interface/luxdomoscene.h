#ifndef LUXDOMOSCENE_H
#define LUXDOMOSCENE_H

#include<QDeclarativeItem>

class LuxDomoScene : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoScene(QDeclarativeItem *parent = 0);
    ~LuxDomoScene();
    void   componentComplete();

    Q_INVOKABLE  void   showRoomControl();
    Q_INVOKABLE  void   useHomeScene();

    Q_INVOKABLE  void   moveParent(int);
    Q_INVOKABLE  void   recordMouseY(int);
signals:

public slots:

private:
      int  mouseY;

};

#endif // LUXDOMOSCENE_H
