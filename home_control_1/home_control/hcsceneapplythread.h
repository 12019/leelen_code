#ifndef HCSCENEAPPLYTHREAD_H
#define HCSCENEAPPLYTHREAD_H

#include <QThread>
#include<qdom.h>

class HCSceneApplyThread : public QThread
{
    Q_OBJECT
public:
    explicit HCSceneApplyThread(QObject *parent = 0);
    short indx;
    bool isQuit;
    int useHCScene(int index);
    int applyScene(QString strSceneFileName);
    int applyDevice(QDomElement &deviceElement);
    void run();
signals:
    void sig_sceneBuffer(char *buf,int len,QString type);
public slots:

};

#endif // HCSCENEAPPLYTHREAD_H
