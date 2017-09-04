#ifndef CURTAINCONTROLDIALOG_H
#define CURTAINCONTROLDIALOG_H

#include <QObject>
#include<QtDeclarative/QDeclarativeView>
#include<QDeclarativeEngine>
#include<QDeclarativeComponent>
#include<QDeclarativeItem>
#include<QDeclarativeView>
#include<QDeclarativeContext>
#include<qdom.h>

class CurtainControlDialog : public QObject
{
    Q_OBJECT
public:
    explicit CurtainControlDialog(QObject *parent = 0);
    ~CurtainControlDialog();

    Q_PROPERTY(QString label READ getLabel WRITE setLabel NOTIFY sig_Label)           //标签名

    QString getLabel(void) const;    //标签名
    void setLabel(const QString& label);

    Q_INVOKABLE void on_pushButtonCurtainClose_clicked();
    Q_INVOKABLE void on_pushButtonCurtainStop_clicked();
    Q_INVOKABLE void on_pushButtonCurtainOpen_clicked();

    QDomElement deviceElement;

    void CurtainstrCmdSend(int cmd);
    void Translate();          //翻译
    
signals:
    void sig_Label(void);
public slots:
private:
    QString m_Label;
};

#endif // CURTAINCONTROLDIALOG_H
