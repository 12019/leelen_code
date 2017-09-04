#include "rcvdatathread.h"
#include<QLinkedListIterator>
#include<MyQByteArray/myqbytearray.h>
#include"homecontrolmaindlg.h"
#include<math.h>

extern MyQByteArray *m_pMyQByteArray;
QList<LuxdomoClient*> m_clients;

RcvDataThread::RcvDataThread(QObject *parent) :
    QThread(parent)
{
    MyRcv.IsValid = false;
    __ObjToSvrData.clear();
}

void RcvDataThread::run()
{
    while(1)
    {
        MyRcv = m_pMyQByteArray->RcvLeeBus(NULL,NULL,"First");
        m_pMyQByteArray->RmLBFirstMutex.unlock();
        if(MyRcv.IsValid)
        {
            HomeControlMainDlg *pHome = (HomeControlMainDlg*)parent();
            pHome->slotRecvHcStatus(MyRcv.data.data());
            __ObjToSvrData += MyRcv.data;
            if(__ObjToSvrData.size() >= 250)
            {
                ObjUplowdDataToClient(__ObjToSvrData.data(),__ObjToSvrData.size(),"","send",NULL);
                ObjUplowdDataToLinSvr(__ObjToSvrData.data(),__ObjToSvrData.size(),"");
                __ObjToSvrData.clear();
            }
        }
        else
        {
            if(!__ObjToSvrData.isEmpty())
            {
                ObjUplowdDataToClient(__ObjToSvrData.data(),__ObjToSvrData.size(),"","send",NULL);
                ObjUplowdDataToLinSvr(__ObjToSvrData.data(),__ObjToSvrData.size(),"");
                __ObjToSvrData.clear();
            }
        }

        MyRcv = m_pMyQByteArray->SvrData(NULL,NULL,"First");
        m_pMyQByteArray->RmSDFirstMutex.unlock();
        if(MyRcv.IsValid)
        {
            HomeControlMainDlg *pHome = (HomeControlMainDlg*)parent();
            if(MyRcv.SvrIP != "Ter")
               pHome->slotRcvFromCloud(MyRcv.data.data());
            ObjUplowdDataToClient(MyRcv.data.data(),MyRcv.size,MyRcv.SvrIP,"send",NULL);
            ObjUplowdDataToLinSvr(MyRcv.data.data(),MyRcv.size,MyRcv.SvrIP);
        }
        msleep(50);
    }
}

void RcvDataThread::ObjUplowdDataToLinSvr(char *buf, int len, QString SvrIP)
{
    if(SvrIP != "117.29.160.158" && buf[3] != 0x00)
    {
        HomeControlMainDlg *pHome = (HomeControlMainDlg*)parent();
        pHome->m_pLINCloudDEVSDK->__UplowdDataToLinCloud(buf,len,SvrIP,"");
    }
}

void RcvDataThread::ObjUplowdDataToClient(char *buf, int len, QString pSvr,QString action,LuxdomoClient *psock)
{
    mutex.lock();
    if(action == "append")
    {
        if(!m_clients.contains(psock))
            m_clients.append(psock);
    }
    else if(action == "delete")
    {
        m_clients.removeOne(psock);
    }
    else if(action == "send")
    {
        for(int i=0;i<m_clients.size();i++)
        {
            LuxdomoClient *client = m_clients.at(i);
            if(pSvr != client->m_addr)
            {
                client->SendData(buf,len);
            }
        }
    }
    mutex.unlock();
}
