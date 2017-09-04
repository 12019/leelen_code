#ifndef LUXDOMOACLEARN_H
#define LUXDOMOACLEARN_H

#include<QDeclarativeItem>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QDomElement>

class LuxDomoAcLearn : public QDeclarativeItem
{
    Q_OBJECT
public:
       explicit LuxDomoAcLearn(QDeclarativeItem *parent = 0);
       ~LuxDomoAcLearn();
       void     componentComplete();
       void     resourceRelease();
       Q_INVOKABLE   void     close();
       Q_INVOKABLE   void     cancelLearn();
       Q_INVOKABLE   void     learnClicked(int);
       Q_INVOKABLE   void     init(QString);

       Q_INVOKABLE   void     showWindList();
       Q_INVOKABLE   void     showTempList();
       Q_INVOKABLE   void     showModeList();
       Q_INVOKABLE   void     showSpeedList();
       Q_INVOKABLE   void     deleteCode(int,int);
       Q_INVOKABLE   void     addCode();
       Q_INVOKABLE   void     learnCode();
signals:

public slots:

private:
       QDeclarativeItem       *listItem;
       QDeclarativeEngine    *listEngine;
      QDomElement  deviceElement;
      QSqlDatabase   m_DataBaseDB;
      QString   wind ;
      QString   mode ;
      QString   speed;
      int    m_code;
};

#endif // LUXDOMOACLEARN_H
