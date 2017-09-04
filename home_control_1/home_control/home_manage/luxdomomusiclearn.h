#ifndef LUXDOMOMUSICLEARN_H
#define LUXDOMOMUSICLEARN_H

#include<QDeclarativeItem>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QDomElement>
class LuxDomoMusicLearn : public QDeclarativeItem
{
    Q_OBJECT
public:
       explicit LuxDomoMusicLearn(QDeclarativeItem *parent = 0);
       ~LuxDomoMusicLearn();
       void  componentComplete();
       Q_INVOKABLE   void     close();
       Q_INVOKABLE   void     cancelLearn();
       Q_INVOKABLE   void     learnClicked(int);
       Q_INVOKABLE   void     init(QString);
       Q_INVOKABLE   void     learnCode();
signals:

public slots:

private:
         QDomElement  deviceElement;
         QString   m_musicFunction;
         QSqlDatabase   m_DataBaseDB;
         int  m_code;
};

#endif // LUXDOMOMUSICLEARN_H
