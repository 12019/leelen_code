#ifndef IRLEARNDIALOG_H
#define IRLEARNDIALOG_H

#include <QObject>
#include<QStringList>

class IrLearnDialog : public QObject
{
    Q_OBJECT
public:
    explicit IrLearnDialog(QObject *parent = 0);

    Q_PROPERTY(int irIndx READ getirIndx WRITE setirIndx NOTIFY sig_irIndx)

    int getirIndx(void) const;  //把值返回到qml
    void setirIndx(const int& irIndx);

    QString deviceType;
    QString deviceId;

    void closeLearnStat();
    void loadIrAction(QString fileName);

    Q_INVOKABLE void close();
    Q_INVOKABLE void irTypeChose(QString type,QString Id);
    Q_INVOKABLE void irLearn(int indx);
    Q_INVOKABLE QString toast();
signals:
    void sig_irIndx(void);
public slots:
private:
    int m_irIndx;
};

#endif // IRLEARNDIALOG_H
