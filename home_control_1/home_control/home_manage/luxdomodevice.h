#ifndef LUXDOMODEVICE_H
#define LUXDOMODEVICE_H

#include<QDeclarativeItem>

class LuxDomoDevice : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoDevice(QDeclarativeItem *parent = 0);
    ~LuxDomoDevice();
    void  componentComplete();

    Q_INVOKABLE  void    moveParent(int);
    Q_INVOKABLE  void    recordMouseY(int);

    Q_INVOKABLE   void    showFunctionOption();
    Q_INVOKABLE  void     showControlDialog();
signals:

public slots:

private:
    int  mouseY;
};

#endif // LUXDOMODEVICE_H
