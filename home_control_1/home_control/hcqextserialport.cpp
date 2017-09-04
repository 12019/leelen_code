
#include "hcqextserialport.h"
#include<MyBuffer.h>
#include<MyQByteArray/myqbytearray.h>

extern MyQByteArray *m_pMyQByteArray;
int serialStatus = LeeBusEasy0;
extern HCQextSerialPort *myCom;
bool ttys7isValue = false;
RcvFromObj mLeeBusCommonCmd;

/*****************发送握手协议*******************/
void HandShakeToSer()
{
    QByteArray fa;
    fa.resize(1);
    fa.data()[0] = 0xfa;
    myCom->writeSerialData(fa);
}

/********************接收串口数据**********************/
void readMyCom()
{
    while (1)
    {
        static int TimeOut = 0;
        //static int count = 0;
        //static QByteArray myBuf;
        /*bool bytesIsValid = myCom->waitForReadyRead(3000);
        if(!bytesIsValid)
        {
            MyMsgLog<<"Timeout"<<myCom->bytesAvailable()<<myCom->readAll().toHex();
            serialStatus = LeeBusTimeOut;     //超时３００ｍｓ需要重新建立关系
            break;
        }*/
        if(myCom->bytesAvailable() <= 0)
        {
            usleep(1000);
            if(++TimeOut >= 300)
            {
                //MyMsgLog<<"Timeout"<<myCom->bytesAvailable()<<myCom->readAll().toHex();
                serialStatus = LeeBusTimeOut;
                TimeOut = 0;
                return;
            }
            else
            {
                continue;
            }
        }
        ttys7isValue = true;
        int pRcvLen = myCom->bytesAvailable();
        QByteArray pRcv;
        pRcv.resize(pRcvLen);
        myCom->read(pRcv.data(),pRcvLen);
        int pRcvHead = (int)pRcv.data()[0];
        //MyMsgLog<<"TTTTTTTTTTTTTTTTTTT:"<<pRcvLen<<pRcv.toHex();
        if(pRcvLen > 1 && pRcvHead != 0xfc && pRcvHead != 0xfd)      //表示帧头出错
        {
            MyMsgLog<<"帧头错误："<<(pRcv+myCom->readAll()).toHex();
            usleep(100000);   //等待１００ｍｓ总线空闲重新建立通信
            serialStatus = LeeBusError;         //帧头错误，主机不能往从机发数据
            return;
        }

        switch (pRcvHead)
        {
            case 0xfa:
                serialStatus = LeeBusEasy0;
                break;
             case 0xfb:     //总线处于忙碌状态
                serialStatus = LeeBusBusy0;
                break;
            case 0xfc:      //从机空闲且带数据上传
            case 0xfd:    //从机忙碌且带数据上传到主机
                MyMsgLog<<"RcvLen And RcvData:"<<pRcvLen<<pRcv.toHex();
                if(pRcvHead == 0xfc)
                {
                    serialStatus = LeeBusEasy1;
                }
                else
                {
                    serialStatus = LeeBusBusy1;
                }
                pRcv.remove(0,1);
                pRcvLen --;
                if((int)pRcv.data()[0] == pRcvLen)   //进行校验
                {
                    int num = 0;
                    for(int i=0;i<pRcvLen - 1;i++)
                    {
                        num += (int)pRcv.data()[i];
                    }
                    num = num&0xff;
                    if(num == (int)pRcv.data()[pRcvLen-1])   //校验正确
                    {
                        QByteArray ff;
                        ff.resize(1);
                        ff.data()[0] = 0xff;
                        myCom->writeSerialData(ff);
                        m_pMyQByteArray->RcvLeeBus(pRcv,"leebus","Append");
                        ff.clear();
                    }
                    else                       //校验出错
                    {
                        qDebug()<<"rev check num error";
                        QByteArray fe;
                        fe.resize(1);
                        fe.data()[0] = 0xfe;
                        myCom->writeSerialData(fe);
                        fe.clear();
                    }
                }
                else    //长度不对直接回校验出错
                {
                    qDebug()<<"rev check num error";
                    QByteArray fe;
                    fe.resize(1);
                    fe.data()[0] = 0xfe;
                    myCom->writeSerialData(fe);
                    fe.clear();
                }
                break;
            case 0xfe:    //从机回复校验错误
                MyMsgLog<<"TTTTTTTTTTTTTCheck Num Error";
                break;
            case 0xff:     //从机回复校验成功
                mLeeBusCommonCmd.IsValid = false;
                mLeeBusCommonCmd.size = 0;
                mLeeBusCommonCmd.UsedTime = 0;
                MyMsgLog<<"TTTTTTTTTTTTTCheck Num Success";
                break;
            default:
                break;
        }
        pRcv.clear();
        TimeOut = 0;
        return;


        /*int RcvLen = myCom->bytesAvailable();
        if(RcvLen == 1)                       //终端机发指令出去,总线模块回复一个字节判断是否正确
        {
            ttys7isValue = true;
            myBuf.resize(RcvLen);
            myCom->read(myBuf.data(),RcvLen);
            int head = (int)myBuf.data()[0];
            if(head == 0xfc)
            {
                serialStatus = LeeBusEasy;
            }
            else if(head == 0xfb)
            {
                serialStatus = LeeBusBusy;
            }
            else if(head == 0xfd)    //校驗和正確
            {
                MyMsgLog<<"snd check num is true";
                mLeeBusCommonCmd.IsValid = false;
                mLeeBusCommonCmd.size = 0;
                mLeeBusCommonCmd.UsedTime = 0;
                serialStatus = LeeBusEasy;
            }
            else if(head == 0xfe) //主机发总线数据过去从机返回fe校验错误
            {
                MyMsgLog<<"snd check num is false";
            }
            myBuf.clear();
            //count = 0;
            break;
            //continue;
        }
        else if(RcvLen >= 7 && RcvLen < LeeBusCmdMax)
        {
            ttys7isValue = true;
            myBuf.resize(RcvLen);
            myCom->read(myBuf.data(),RcvLen);
            short PageAgeLen = myBuf.size();
            int head = (int)myBuf.data()[0];
            MyMsgLog<<"rcvlen and rcvdata"<<PageAgeLen<<myBuf.toHex();
            PageAgeLen --;
            myBuf.remove(0,1);

            if(PageAgeLen != (int)myBuf.data()[0])
                PageAgeLen = 1;  //跳過校驗和,直接校驗失敗

            int num = 0;
            for(int i=0;i<PageAgeLen-1;i++)
            {
                num += (int)myBuf.data()[i];
            }
            num = num&0xff;

            if(num == (int)myBuf.data()[PageAgeLen-1])
            {
                QByteArray fd;
                fd.resize(1);
                fd.data()[0] = 0xfd;
                myCom->writeSerialData(fd);
                //m_pMyQByteArray->RcvFromLeeBusAppend(myBuf.data(),"leebus",PageAgeLen);
                //m_pMyQByteArray->LeebusApend(myBuf,"leebus");
                m_pMyQByteArray->RcvLeeBus(myBuf,"leebus","Append");
                fd.clear();
            }
            else   //校验和失败返回fe，从机会重发
            {
                myBuf.clear();
                qDebug()<<"rev check num error";
                QByteArray fe;
                fe.resize(1);
                fe.data()[0] = 0xfe;
                myCom->writeSerialData(fe);
                fe.clear();
            }

            if(head == 0xfc)
            {
                serialStatus = LeeBusEasy;
            }
            else if(head == 0xfb)
            {
                serialStatus = LeeBusBusy;
            }
            else
            {
                QByteArray fe;   //包頭錯誤
                fe.resize(1);
                fe.data()[0] = 0xfe;
                myCom->writeSerialData(fe);
                fe.clear();
                MyMsgLog<<"the head bag is error"<<hex<<head;
            }
            myBuf.clear();
            //count = 0;
            break;
        }
        else if(RcvLen > 0)
        {
            ttys7isValue = true;
            QByteArray buf = myCom->readAll();
            MyMsgLog<<"readAll:"<<buf.toHex()<<buf.count();
            QByteArray fe;   //包頭錯誤
            fe.resize(1);
            fe.data()[0] = 0xfe;
            myCom->writeSerialData(fe);
            break;
        }

        /*if(count ++ <= 200)
        {
            usleep(1000);
        }
        else
        {
            myBuf.clear();
            count = 0;
            serialStatus = LeeBusEasy;
            break;
        }*/
    }
}

void *SerialFunThread(void *arg)
{
    while (1)
    {
        /*if(serialStatus == LeeBusEasy)
        {
            serialStatus = LeeBusBusy;
            HandShakeToSer();                          //握手
        }*/

        serialStatus = LeeBusBusy0;
        HandShakeToSer();                          //握手发０ｘｆａ
        readMyCom();

        if(serialStatus == LeeBusEasy0 | serialStatus == LeeBusEasy1)    //总线处于空闲状态,可以发送控制指令
        {
            //MyMsgLog<<"YYYYYYYYYYYYYYYYYYYYYYY";
            if(!mLeeBusCommonCmd.IsValid)
            {
                mLeeBusCommonCmd = m_pMyQByteArray->PushToLeeBus(NULL,NULL,"First");//m_pMyQByteArray->SvrDataFirst();//m_pMyQByteArray->GetObjToLeeBusLast();
                m_pMyQByteArray->RmPushToLBMutex.unlock();
            }
            if(mLeeBusCommonCmd.IsValid)
            {
                if(++mLeeBusCommonCmd.UsedTime > 3)
                {
                    mLeeBusCommonCmd.size = 0;
                    mLeeBusCommonCmd.IsValid = false;
                    mLeeBusCommonCmd.UsedTime = 0;
                    continue;
                }
                usleep(10000);
                myCom->PackData(mLeeBusCommonCmd.data);
                readMyCom();
            }
        }
    }
}

HCQextSerialPort::HCQextSerialPort()
{
    mLeeBusCommonCmd.IsValid = false;
    mLeeBusCommonCmd.size = 0;
    mLeeBusCommonCmd.UsedTime = 0;
    openSerialPort();
}

HCQextSerialPort::~HCQextSerialPort()
{
    pthread_attr_destroy(&attr1);
    close();
}

/********************打包數據****************************/
void HCQextSerialPort::PackData(QByteArray buf)
{
    buf.insert(0,0xfc);
    writeSerialData(buf);
    MyMsgLog<<"write data:"<<buf.toHex();
}

/******************發送串口數據**********************/
void HCQextSerialPort::writeSerialData(QByteArray buf)
{
    int len = buf.size();
    write(buf.data(),len);

}

/******************配置串口********************/
void HCQextSerialPort::openSerialPort()
{
    setPortName("/dev/ttyS7");
    setBaudRate(BAUD19200);
    setDataBits(DATA_8);
    setParity(PAR_NONE);
    setStopBits(STOP_1);
    setFlowControl(FLOW_OFF);
    setTimeout(10);
    if(open(QIODevice::ReadWrite))
    {
        qDebug()<<"open 485 serial success"<<portName();
    }
    else
    {
        qDebug()<<"open 485 serial failed";
    }
}


void HCQextSerialPort::startThread()
{
    param1.sched_priority=1;              //设为最高优先级
    pthread_attr_init(&attr1);
    pthread_attr_setinheritsched(&attr1,PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr1,SCHED_FIFO);
    pthread_attr_setschedparam(&attr1,&param1);
    pthread_create(&tid1,&attr1,SerialFunThread,NULL);
}

