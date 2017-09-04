#ifndef HCROOMSELECT_H
#define HCROOMSELECT_H

#include <QObject>
#include<QtDeclarative/QDeclarativeView>
#include<QDeclarativeEngine>
#include<QDeclarativeComponent>
#include<QDeclarativeItem>
#include<QDeclarativeView>
#include<QDeclarativeContext>
#include <QtXml>
#include<QTimer>


class HCRoomSelect : public QObject
{
    Q_OBJECT
public:
    explicit HCRoomSelect(QObject *parent = 0);
    ~HCRoomSelect();

    Q_PROPERTY(short roomId READ getroomId WRITE setroomId NOTIFY sendroomIdChange)
    Q_PROPERTY(short layerId READ getlayerId WRITE setlayerId NOTIFY layerIdChange)
    Q_PROPERTY(QString cancel READ cancel WRITE setCancel NOTIFY sig_Cancel)  //取消

    short getroomId(void) const;  //把值返回到qml
    void setroomId(const short& roomId);//设置传入的值是开或者关

    short getlayerId(void) const;
    void setlayerId(const short& layerId);

    QString cancel(void) const;
    void setCancel(const QString& cancel);


    QStringList roomIdList;
    int layerIndex;

    Q_INVOKABLE void loadLayerlist();
    Q_INVOKABLE void close();
    Q_INVOKABLE void layerList(QString name);
    Q_INVOKABLE void roomList(short index);
    Q_INVOKABLE void loadRoomMap();
    Q_INVOKABLE void hideAll();
    void Translate();          //翻译
    
signals:
    void sendroomIdChange(void);   //启动getStat()
    void layerIdChange(void);
    void sig_Cancel(void);

private:
    short m_layerId;
    short m_roomId;
    QString m_Cancel;
};

#endif // HCROOMSELECT_H
