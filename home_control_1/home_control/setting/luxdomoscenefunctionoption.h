#ifndef LUXDOMOSCENEFUNCTIONOPTION_H
#define LUXDOMOSCENEFUNCTIONOPTION_H

#include<QDeclarativeItem>

class LuxDomoSceneFunctionOption : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoSceneFunctionOption(QDeclarativeItem *parent = 0);
    ~LuxDomoSceneFunctionOption();
    void  componentComplete();

     Q_INVOKABLE  void    deleteScene();
     Q_INVOKABLE  void    editSceneName();

signals:
        void  sigDeleteHomeScene(QString);
public slots:
};

#endif // LUXDOMOSCENEFUNCTIONOPTION_H
