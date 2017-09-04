#ifndef HCSTATUSBAR_H
#define HCSTATUSBAR_H

#include <QObject>

#include<QtDeclarative/QDeclarativeView>
#include<QDeclarativeEngine>
#include<QDeclarativeComponent>
#include<QDeclarativeItem>
#include<QDeclarativeView>
#include<QDeclarativeContext>


class HCStatusBar : public QObject
{
    Q_OBJECT
public:
    explicit HCStatusBar(QObject *parent = 0);
    ~HCStatusBar();

    Q_PROPERTY(QString lrName READ getlrName WRITE setlrName NOTIFY sig_lrname)   //一层客厅
    Q_PROPERTY(QString hcselect READ hcselect WRITE setHcselect NOTIFY sig_Hcselect)   //房间选择
    Q_PROPERTY(QString irlearning READ irlearning WRITE setIrlearn NOTIFY sig_Irlearn)   //红外学习
    Q_PROPERTY(QString back READ back WRITE setBack NOTIFY sig_Back)   //返回
    Q_PROPERTY(QString exit READ exit WRITE setExit NOTIFY sig_Exit)   //返回

    QString getlrName(void) const;              //设置房间名称
    void setlrName(const QString& name);

    QString hcselect(void) const;              //房间选择
    void setHcselect(const QString& hcselect);

    QString irlearning(void) const;              //红外学习
    void setIrlearn(const QString& irlearning);

    QString back(void) const;              //返回
    void setBack(const QString& back);

    QString exit(void) const;              //返回
    void setExit(const QString& exit);

    void show();
    void setHeigh();   //

    Q_INVOKABLE void closeHomeControlDlg();
    Q_INVOKABLE void roomChose();
    Q_INVOKABLE void irLearn();
    Q_INVOKABLE void setGround(bool stat);
    void Translate();          //翻译

    
signals:
    void sig_lrname(void);
    void sig_Hcselect(void);
    void sig_Irlearn(void);
    void sig_Back(void);
    void sig_Exit(void);
public slots:
private:
    QString m_lrName; //楼层房间名
    QString m_Hcselect;
    QString m_Irlearn;
    QString m_Back;
    QString m_Exit;
};

#endif // HCSTATUSBAR_H
