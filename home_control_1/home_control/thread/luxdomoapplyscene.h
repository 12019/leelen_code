#ifndef LUXDOMOAPPLYSCENE_H
#define LUXDOMOAPPLYSCENE_H

#include <QThread>
#include<QDomDocument>

class LuxDomoApplyScene : public QThread
{
    Q_OBJECT
public:
    explicit LuxDomoApplyScene(QThread *parent = 0);
    void   run();
    void   DealSceneData(QDomElement,QString);
    void   RefreshControlInterface(QDomElement,QString);
    void   AllOnAndOff(QString);
    void   UsingRoomScene();
    void   UsingHomeScene();
signals:
    void    sigRefreshControlInterface(QByteArray);
public slots:
public:
    QString    m_roomObjectName;
    QString    m_sceneObjectName;
    QString    m_strFile;
};

#endif // LUXDOMOAPPLYSCENE_H
