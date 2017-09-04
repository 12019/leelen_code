#ifndef LUXDOMOSCENETOOLBAR_H
#define LUXDOMOSCENETOOLBAR_H

#include<QDeclarativeItem>

class LuxDomoSceneToolbar : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoSceneToolbar(QDeclarativeItem *parent = 0);
    ~LuxDomoSceneToolbar();
    void  componentComplete();
    QStringList    GetSceneImage(QString);
    Q_INVOKABLE  void    init();
    Q_INVOKABLE  void    showHomeScene(QString,QString);
    Q_INVOKABLE  void    enableHomeScene(QString,QString,int);
signals:
public slots:
    void   slot_addHomeScene(QByteArray);
private:
    QDeclarativeEngine    *sceneEngine;

};

#endif // LUXDOMOSCENETOOLBAR_H
