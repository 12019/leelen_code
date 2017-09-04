#ifndef LIFESYSPOSTDATA_H
#define LIFESYSPOSTDATA_H

#include <QObject>
#include<QNetworkReply>
#include<QNetworkRequest>
#include<QNetworkInterface>

class LifeSysPostData : public QObject
{
    Q_OBJECT
public:
    explicit LifeSysPostData(QObject *parent = 0);
    ~LifeSysPostData();
    QNetworkAccessManager *m_pmanager;
    QNetworkReply *m_preply;
    QNetworkRequest *m_prequest;
    void postData(QByteArray strData);
    void postDataInit();
    char *base64_Encode(unsigned char *data, int data_len);
    QString getBase64Code(QString);
private slots:
    void replyFinished(QNetworkReply*);

signals:

public slots:

};

#endif // LIFESYSPOSTDATA_H
