#ifndef DEFENSEGPIOSCANTHREAD_H
#define DEFENSEGPIOSCANTHREAD_H

#include <QThread>
#include <QTimer>

struct GPIODefenseData{
    uchar level;
    int number;
    bool type;  //0:报警  1:门铃
    GPIODefenseData *next;
};
class DefenseGPIOScanThread : public QThread
{
    Q_OBJECT
public:
    explicit DefenseGPIOScanThread(QObject *parent = 0);
    ~DefenseGPIOScanThread();
    void run();
    void keyProcess(char areaNumber);
    void writeDefenseCharArray(char areaNumber);
    void writeDoorRingCharArray();
    void copyArray(char buf[]);
    char buf_char[14];
    char DefenseGpioStatus[32]; //记录/dev/YouXianFangQu 32位的状态

    void toGPIODefenseData(uchar buf);
    GPIODefenseData *m_pHeadGPIODefenseData;
    GPIODefenseData * getDataFromList();

    struct {
        int deviceopen;
        int devicemove;
    } DeviceStatus;

    int m_bRun;
    bool m_bCopyDataOneTime;
signals:

public slots:
};

#endif // DEFENSEGPIOSCANTHREAD_H
