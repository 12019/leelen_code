#ifndef HCQEXTSERIALPORT_H
#define HCQEXTSERIALPORT_H

#include<serialport/posix_qextserialport.h>
#include<QTimer>
#include<QObject>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sched.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<string.h>
#include<MyBuffer.h>

class HCQextSerialPort : public Posix_QextSerialPort
{
    Q_OBJECT
public:
    explicit HCQextSerialPort();
    ~HCQextSerialPort();

    void PackData(QByteArray buf);
    void writeSerialData(QByteArray buf);
    void openSerialPort();

    pthread_t tid1;
    pthread_attr_t attr1;
    struct sched_param param1;
    void startThread();
public slots:
signals:
};

#endif // HCQEXTSERIALPORT_H
