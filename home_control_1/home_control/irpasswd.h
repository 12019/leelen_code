#ifndef IRPASSWD_H
#define IRPASSWD_H

#include <QObject>
#include<QtDeclarative/QDeclarativeView>
#include<QDeclarativeEngine>
#include<QDeclarativeComponent>
#include<QDeclarativeItem>
#include<QDeclarativeView>
#include<QDeclarativeContext>
#include<irlearndialog.h>

class Irpasswd : public QObject
{
    Q_OBJECT
public:
    explicit Irpasswd(QObject *parent = 0);

    Q_PROPERTY(QString passwd READ passwd WRITE setPasswd NOTIFY sig_Passwd)                //密码编辑框

    ~Irpasswd();

    QString passwd(void) const;         //密码编辑框
    void setPasswd(const QString& passwd);

    Q_INVOKABLE void close();
    Q_INVOKABLE void loadIrDevice();
    Q_INVOKABLE void setPwdName(QString str);
    void Translate();          //翻译
signals:
    void sig_Passwd(void);
    
public slots:
private:
    QString m_Pwd;
};

#endif // IRPASSWD_H
