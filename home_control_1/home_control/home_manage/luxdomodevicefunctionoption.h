#ifndef LUXDOMODEVICEFUNCTIONOPTION_H
#define LUXDOMODEVICEFUNCTIONOPTION_H

#include<QDeclarativeItem>

class LuxDomoDeviceFunctionOption : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoDeviceFunctionOption(QDeclarativeItem *parent = 0);
    ~LuxDomoDeviceFunctionOption();
    void  componentComplete();

     Q_INVOKABLE   void    deleteDevice();
     Q_INVOKABLE   void    showDeviceEdit();
     Q_INVOKABLE   void    showInfraredLearn();
     Q_INVOKABLE   void    close();

    void        MusicInfraredLearn();
    void        TVInfraredLearn();
    void        ACInfraredLearn();
signals:

public slots:

private:

};

#endif // LUXDOMODEVICEFUNCTIONOPTION_H
