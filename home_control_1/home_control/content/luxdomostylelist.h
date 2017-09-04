#ifndef LUXDOMOSTYLELIST_H
#define LUXDOMOSTYLELIST_H

#include<QDeclarativeItem>

class LuxDomoStyleList : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoStyleList(QDeclarativeItem *parent = 0);
    ~LuxDomoStyleList();
   void    componentComplete();
    Q_INVOKABLE   void    close();
    Q_INVOKABLE   void    setDeviceData(QString);

signals:

public slots:
};

#endif // LUXDOMOSTYLELIST_H
