#ifndef HOUSEMAPXMLTHREAD_H
#define HOUSEMAPXMLTHREAD_H

#include <QThread>
#include <QtXml>
#include<QPolygonF>

#define loadroom                        1
#define irlearn                              2
#define creatscenexml                3
#define scenedeviceload             4




class houseMapXmlThread : public QThread
{
    Q_OBJECT
public:
    explicit houseMapXmlThread(QObject *parent = 0);
    ~houseMapXmlThread();
    void run();
    QString m_roomId;
    short eventType;
    void loadRoomMapDev();
    void loadIrDevice();
    void creatSceneXml();
    void sceneDevLoad();
    void creatSceneAllOffXml();
    short indx;
    qreal roomImageHeight,roomImageWidth;
    QString currentImg;
    
signals:
    void loadRoomMapSig(QString roomImage);
    void loadRoomDevSig(QString deviceId,QString type,int x,int y,bool isLast);
    void sig_updateDevice();
    void sig_Pixmap(QString);
public slots:
    void slot_Pixmap(QString);
};

#endif // HOUSEMAPXMLTHREAD_H
