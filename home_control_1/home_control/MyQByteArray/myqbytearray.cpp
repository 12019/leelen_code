#include "myqbytearray.h"

MyQByteArray::MyQByteArray(QObject *parent) : QObject(parent)
{
    pRcvFromLeeBus.clear();
    pRcvFromSvr.clear();
    pPushToLeeBus.clear();
}

/***********************************************************************************
函数名 : RcvLeeBus
描  述 : 从总线收到的数据,添加与删除操作
输入参数说明 :buf:数据，SvrIP：暂时没用，action：操作类型
输出参数说明 :
返 回 值:  第一个节点的数据
其它说明:
***********************************************************************************/
RcvFromObj MyQByteArray::RcvLeeBus(QByteArray buf, QString SvrIP, QString action)
{
    RmLBFirstMutex.lock();
    RcvFromObj pRcv;
    pRcv.IsValid = false;
    if(action == "Append")
    {
        LeebusApend(buf,SvrIP);
    }
    else if(action == "First")
    {
        pRcv = LeeBusFirst();
        return pRcv;
    }
    RmLBFirstMutex.unlock();
    return pRcv;
}

/***********************************************************************************
函数名 : LeebusApend
描  述 : 添加从总线收到的数据
输入参数说明 :buf:数据，SvrIP：暂时没用，size：大小
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void MyQByteArray::LeebusApend(QByteArray buf, QString SvrIP)
{
    RcvFromObj mRcvFromObj;
    mRcvFromObj.data = buf;
    mRcvFromObj.IsValid = true;
    mRcvFromObj.SvrIP = SvrIP;
    mRcvFromObj.UsedTime = 0;
    mRcvFromObj.size = buf.size();
    pRcvFromLeeBus.append(mRcvFromObj);
    if(pRcvFromLeeBus.size() >= 2000)
        rmLeeBusFirst();
}

/***********************************************************************************
函数名 : LeeBusFirst
描  述 : 删除从总线收到的数据
输入参数说明 :
输出参数说明 :
返 回 值:第一个节点的数据
其它说明:
***********************************************************************************/
RcvFromObj MyQByteArray::LeeBusFirst()
{
    RcvFromObj mRcvFromObj;
    mRcvFromObj.IsValid = false;
    if(pRcvFromLeeBus.size() <= 0)
    {
        return mRcvFromObj;
    }
    else
    {
        mRcvFromObj = pRcvFromLeeBus.first();
        rmLeeBusFirst();
        return mRcvFromObj;
    }
}
void MyQByteArray::rmLeeBusFirst()
{
    if(pRcvFromLeeBus.size() > 0)
        pRcvFromLeeBus.removeFirst();
}

/***********************************************************************************
函数名 : SvrData
描  述 : 从其客服端（手机，ipad）收到的数据,添加与删除操作
输入参数说明 :buf:数据，SvrIP：暂时没用，action：操作类型
输出参数说明 :
返 回 值:  第一个节点的数据
其它说明:
***********************************************************************************/
RcvFromObj MyQByteArray::SvrData(QByteArray buf, QString SvrIP, QString action)
{
    RmSDFirstMutex.lock();
    RcvFromObj pRcv;
    pRcv.IsValid = false;
    if(action == "Append")
    {
        SvrDataApend(buf,SvrIP);
    }
    else if(action == "First")
    {
        pRcv = SvrDataFirst();
        return pRcv;
    }
    RmSDFirstMutex.unlock();
    return pRcv;
}

/***********************************************************************************
函数名 : SvrDataApend
描  述 : 把所有接收到的数据存到该缓冲区，包括本地数据，远程数据等，除了总线数据
输入参数说明 :buf:数据，SvrIP：从哪个IP得到的数据，若是总线时则表示为空
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void MyQByteArray::SvrDataApend(QByteArray buf, QString SvrIP)
{
    RcvFromObj mRcvFromObj;
    mRcvFromObj.data = buf;
    mRcvFromObj.IsValid = true;
    mRcvFromObj.SvrIP = SvrIP;
    mRcvFromObj.UsedTime = 0;
    mRcvFromObj.size = buf.size();
    pRcvFromSvr.append(mRcvFromObj);
    if(pRcvFromSvr.size() >= 1000)
        rmSvrDataFirst();
}

/***********************************************************************************
函数名 : SvrDataFirst
描  述 : 删除从其客服端（手机，ipad）收到的数据
输入参数说明 :
输出参数说明 :
返 回 值:第一个节点的数据
其它说明:
***********************************************************************************/
RcvFromObj MyQByteArray::SvrDataFirst()
{
    RcvFromObj mRcvFromObj;
    mRcvFromObj.IsValid = false;
    if(pRcvFromSvr.size() <= 0)
    {
        return mRcvFromObj;
    }
    else
    {
        mRcvFromObj = pRcvFromSvr.first();
        rmSvrDataFirst();
        return mRcvFromObj;
    }
}
void MyQByteArray::rmSvrDataFirst()
{
    if(pRcvFromSvr.size() > 0)
        pRcvFromSvr.removeFirst();
}

/***********************************************************************************
函数名 : PushToLeeBus
描  述 :  要发送到总线的缓冲区数据,添加与删除操作
输入参数说明 :buf:数据，SvrIP：暂时没用，action：操作类型
输出参数说明 :
返 回 值:  第一个节点的数据
其它说明:
***********************************************************************************/
RcvFromObj MyQByteArray::PushToLeeBus(QByteArray buf, QString SvrIP, QString action)
{
    RmPushToLBMutex.lock();
    RcvFromObj pRcv;
    pRcv.IsValid = false;
    if(action == "Append")
    {
        PushToLeeBusApend(buf,SvrIP);
    }
    else if(action == "First")
    {
        pRcv = PushToLeeBusFirst();
        return pRcv;
    }
    RmPushToLBMutex.unlock();
    return pRcv;
}

/***********************************************************************************
函数名 : PushToLeeBusApend
描  述 : 添加要发送到总线的缓冲区数据
输入参数说明 :buf:数据，SvrIP：暂时没用，size：大小
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void MyQByteArray::PushToLeeBusApend(QByteArray buf, QString SvrIP)
{
    RcvFromObj mRcvFromObj;
    mRcvFromObj.data = buf;
    mRcvFromObj.IsValid = true;
    mRcvFromObj.SvrIP = SvrIP;
    mRcvFromObj.UsedTime = 0;
    mRcvFromObj.size = buf.size();
    pPushToLeeBus.append(mRcvFromObj);
    if(pPushToLeeBus.size() >= 1000)
        rmPushToLeeBusFirst();
}

/***********************************************************************************
函数名 : PushToLeeBusFirst
描  述 : 删除要发送到总线的缓冲区数据
输入参数说明 :
输出参数说明 :
返 回 值:第一个节点的数据
其它说明:
***********************************************************************************/
RcvFromObj MyQByteArray::PushToLeeBusFirst()
{
    RcvFromObj mRcvFromObj;
    mRcvFromObj.IsValid = false;
    if(pPushToLeeBus.size() <= 0)
    {
        return mRcvFromObj;
    }
    else
    {
        mRcvFromObj = pPushToLeeBus.first();
        rmPushToLeeBusFirst();
        return mRcvFromObj;
    }
}
void MyQByteArray::rmPushToLeeBusFirst()
{
    if(pPushToLeeBus.size() > 0)
        pPushToLeeBus.removeFirst();
}
