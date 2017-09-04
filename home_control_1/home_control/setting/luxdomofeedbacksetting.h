#ifndef LUXDOMOFEEDBACKSETTING_H
#define LUXDOMOFEEDBACKSETTING_H

#include<QDeclarativeItem>

class LuxDomoFeedbackSetting : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoFeedbackSetting(QDeclarativeItem *parent = 0);
    ~LuxDomoFeedbackSetting();
    void   componentComplete();
    Q_INVOKABLE  void    init();
    Q_INVOKABLE  void    editMessage();
    Q_INVOKABLE  void    optionType(int,QString);
    Q_INVOKABLE  void    setInformation(QString);
    Q_INVOKABLE  void    sendFeedback();
    Q_INVOKABLE  void    cancelFeedback();
signals:
public slots:
private:
    QStringList                types;
    QMap<int,QString>    optionTypes;
};

#endif // LUXDOMOFEEDBACKSETTING_H
