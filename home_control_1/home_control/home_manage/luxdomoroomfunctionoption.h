#ifndef LUXDOMOROOMFUNCTIONOPTION_H
#define LUXDOMOROOMFUNCTIONOPTION_H

#include<QDeclarativeItem>

class LuxDomoRoomFunctionOption : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoRoomFunctionOption(QDeclarativeItem *parent = 0);
    ~LuxDomoRoomFunctionOption();
    void  componentComplete();

    Q_INVOKABLE   void    deleteRoom();
    Q_INVOKABLE   void    setRoomName();
    Q_INVOKABLE   void    close();
signals:

public slots:
};

#endif // LUXDOMOROOMFUNCTIONOPTION_H
