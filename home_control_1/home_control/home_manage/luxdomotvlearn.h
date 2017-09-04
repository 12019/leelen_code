#ifndef LUXDOMOTVLEARN_H
#define LUXDOMOTVLEARN_H

#include<QDeclarativeItem>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QDomElement>

class LuxDomoTvLearn : public QDeclarativeItem
{
    Q_OBJECT
public:
      explicit LuxDomoTvLearn(QDeclarativeItem *parent = 0);
      ~LuxDomoTvLearn();
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
      QString   m_tvFunction;
      QSqlDatabase   m_DataBaseDB;
      int  m_code;
};

#endif // LUXDOMOTVLEARN_H
