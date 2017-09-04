#ifndef LUXDOMOLOADINGLABEL_H
#define LUXDOMOLOADINGLABEL_H

#include<QDeclarativeItem>

class LuxDomoLoadingLabel : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoLoadingLabel(QDeclarativeItem *parent = 0);
    ~LuxDomoLoadingLabel();
    void   componentComplete();

    void      ShowLoadingLabel();
    void      HideLoadingLabel();
signals:

public slots:
};

#endif // LUXDOMOLOADINGLABEL_H
