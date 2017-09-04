#ifndef LUXDOMOROOM_H
#define LUXDOMOROOM_H

#include<QDeclarativeItem>

class LuxDomoRoom : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoRoom(QDeclarativeItem *parent = 0);
    ~LuxDomoRoom();
    void  componentComplete();

    Q_INVOKABLE   void   showFunctionOption();
    Q_INVOKABLE   void   roomManage();

    Q_INVOKABLE  void   moveParent(int);
    Q_INVOKABLE  void   recordMouseY(int);

signals:

public slots:
private:
    int   mouseY;
};

#endif // LUXDOMOROOM_H
