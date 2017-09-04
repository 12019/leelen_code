/***************************************************************************************************************
 ************************************本程序主要是连接云服务器通信 *************************************************
 ****************************************************************************************************************/
#include "linclouddevsdkthread.h"

#include<QSettings>
#include<QDate>
#include<QTime>
#include<QDebug>
#include"homecontrolmaindlg.h"
#include"maindialog.h"

extern MainDialog *g_pMainDlg;


LINCloudDevSDKThread::LINCloudDevSDKThread(QObject *parent) : QThread(parent)
{
    __HNet = 0;
    LinCloudIsValid = false;
    ReCntTime = 0;
    pClientInitIsValid = false;
    startTimer(LIN_ReCntInterval);
}

LINCloudDevSDKThread::~LINCloudDevSDKThread()
{
    LIN_CloudClientExit();
}

/***********************************************************************************
函数名 : run
描  述 : 由于连接服务器时是同步的方式，所以启用线程的方法来实现，防止超时的时候程序卡住，该函数主要是登录云服务器
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void LINCloudDevSDKThread::run()
{
    /*if(pClientInitIsValid)
    {
        __UplowdDataToLinCloud(NULL,NULL,"","delete");
    }*/
    __UplowdDataToLinCloud(NULL,NULL,"","delete");
    LIN_LoginToCloud();
}

/***********************************************************************************
函数名 : LIN_LoginToCloud
描  述 : 登录到云端
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void LINCloudDevSDKThread::LIN_LoginToCloud()
{
//    QString LIN_Pwd;
//    QString LIN_User;
//    LIN_Addr = qsettings.value("LIN_CLOUD_IP").toString();
//    LIN_User = qsettings.value("LIN_CLOUD_Acc").toString();
//    LIN_Pwd = qsettings.value("LIN_CLOUD_PWD").toString();
//    LIN_DevId = qsettings.value("LIN_CLOUD_ID").toString();;
//    LIN_DevId=tr("8BF3E5434C3227C6234084BAF604481A");
//    LIN_Addr=tr("117.29.160.158");
    QString LIN_Addr;
    QString LIN_DevId;
    QSettings qsettings("/mnt/disk/config/homecontrol.ini",QSettings::IniFormat);
    LIN_Addr = qsettings.value(LIN_CLOUD_IP).toString();
    LIN_DevId = qsettings.value(LIN_CLOUD_ID).toString();

    LIN_ExceptionEnm ret;
    MyMsgLog<<"云服务器打开中..............................................................";
    ret = LIN_LeeBusOpenLinCloud(LINExp_NetHandle(this),
                                 LIN_Addr.toLatin1().data(),
                                 &__HNet,
                                 (LIN_OnLeeBusDeviceRcvData)sOnRcvLeeBusData,
                                 (LIN_OnNetClose)sOnNetClose);

    if(ret == LIN_ExceptionEnm_OK)
    {
        MyMsgLog<<"云服务器打开成功"<<LIN_DevId<<LIN_Addr;
        ret = LIN_LogInLinCloud(__HNet,LIN_DevId.toLatin1().data());
        if(ret == LIN_ExceptionEnm_OK)
        {
            LinCloudIsValid = true;
            sigCloudCondition(tr("已连接"));
            MyMsgLog<<"云服务器登录成功:";
            //std::vector<char> InitRetInfo(512);
            //QString msg = LIN_GetRetInfo(ret, &InitRetInfo.front(), InitRetInfo.size());
        }
        else
        {
            LinCloudIsValid = false;
            sigCloudCondition(tr("未连接"));
            MyMsgLog<<"云服务器登录失败:";
            //std::vector<char> InitRetInfo(512);
            //QString msg = LIN_GetRetInfo(ret, &InitRetInfo.front(), InitRetInfo.size());
        }
    }
    else
    {
        LinCloudIsValid = false;
        sigCloudCondition(tr("未连接"));
        MyMsgLog<<"云服务器打开失败:";
        //std::vector<char> InitRetInfo(512);
        //QString msg = LIN_GetRetInfo(ret, &InitRetInfo.front(), InitRetInfo.size());
    }
}

/***********************************************************************************
函数名 : sOnRcvLeeBusData
描  述 : 云服务器下发的数据转到该函数
输入参数说明 :
pWorkObj：工作对象
pRcv：数据缓冲区
RcvLen：数据长度
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
LIN_ExceptionEnm LINCloudDevSDKThread::sOnRcvLeeBusData(LINCloudDevSDKThread*pWorkObj, LINExp_cpByte pRcv, LINExp_Uint RcvLen)
{
   return pWorkObj->__sOnRcvLeeBusData(pRcv,RcvLen);
}

//int count = 0;
LIN_ExceptionEnm LINCloudDevSDKThread::__sOnRcvLeeBusData(LINExp_cpByte pRcv, LINExp_Uint RcvLen)
{
    //count += RcvLen;
    //MyMsgLog<<"TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT"<<count;
    //return LIN_ExceptionEnm_OK;
    HomeControlMainDlg *pHomeControlMainDlg = (HomeControlMainDlg*)parent();
    QByteArray buf;
    buf.resize(RcvLen);
    memcpy(buf.data(),pRcv,RcvLen);
    //MyMsgLog<<buf.toHex();

    while (!buf.isEmpty())
    {
        int size = (int)buf.data()[0];
        if(size > buf.size() || size < 6)
        {
            buf.remove(0,1);
        }
        else
        {
            int sum = 0;
            for(int i = 0;i < size - 1;i++)
            {
                sum += (int)buf.data()[i];
            }
            sum = sum&0xff;
            if(sum == (int)buf.data()[size-1])
            {
                pHomeControlMainDlg->slotRcvFromSvr(buf.left(size),"117.29.160.158",size);
                buf.remove(0,size);
            }
            else
            {
                buf.remove(0,1);
            }
        }
    }
    buf.clear();
    return LIN_ExceptionEnm_OK;
}

/***********************************************************************************
函数名 : sOnNetClose
描  述 : 当云服务器断开时，系统会调用此回调函数
输入参数说明 :
pWorkObj：工作对象
CloseReason：网络关闭的原因
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void LINCloudDevSDKThread::sOnNetClose(LINCloudDevSDKThread *pWorkObj, LIN_ExceptionEnm CloseReason)
{
    pWorkObj->__OnNetClose(CloseReason);
}
void LINCloudDevSDKThread::__OnNetClose(LIN_ExceptionEnm CloseReason)
{
    //pClientInitIsValid = false;
    LinCloudIsValid = false;
    sigCloudCondition(tr("未连接"));
    //MyMsgLog<<"LINCloud Server Off Line";
    startTimer(LIN_ReCntInterval);
}


/***********************************************************************************
函数名 : timerEvent
描  述 : 定时器，当云服务器断开后30s会进行重连
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void LINCloudDevSDKThread::timerEvent(QTimerEvent *e)
{
    //MyMsgLog<<"time id:"<<e->timerId()<<"reCntTime:"<<ReCntTime++;
    killTimer(e->timerId());
    if(!pClientInitIsValid)
    {
        LIN_ExceptionEnm ret;
        ret = LIN_CloudClientInit();
        if (ret == LIN_ExceptionEnm_OK)
        {
            //std::vector<char> InitRetInfo(512);
            pClientInitIsValid = true;
            //MyMsgLog<<"云客户端初始化成功:"<<LIN_GetRetInfo(ret, &InitRetInfo.front(), InitRetInfo.size());
            start();
        }
        else
        {
            //std::vector<char> InitRetInfo(512);
            //MyMsgLog<<"云客户端初始化错误:"<<LIN_GetRetInfo(ret, &InitRetInfo.front(), InitRetInfo.size());
        }
    }
    else
    {
        start();
    }
}

/***********************************************************************************
函数名 : __UplowdDataToLinCloud
描  述 : 此函数主要是发送和释放句柄，这两者同步主要是防止刚释放句柄的时候刚好发送数据导致内存出错
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
//int count1 = 0;
void LINCloudDevSDKThread::__UplowdDataToLinCloud(char *pRcv, int RcvLen, QString SvrIP,QString flag)
{
    mutex.lock();
    if(flag == "delete")
    {
        MyMsgLog<<"releaseCloudSource __HNet:"<<__HNet;
        if(__HNet != 0)
        {
            LIN_LogOutLinCloud(__HNet);
        }
        //LIN_CloudClientExit();
    }
    else
    {
        if(SvrIP != "117.29.160.158" && LinCloudIsValid)
        {
            LIN_ExceptionEnm ret = LIN_LeeBusUplowdDataToLinCloud(__HNet,(uchar*)pRcv,RcvLen);
            if(ret != LIN_ExceptionEnm_OK)
            {
                //MyMsgLog<<"云服务器推送失败";
            }
        }
    }
    mutex.unlock();
}


/***********************************************************************************
函数名 : LIN_GetUpdateInfo
描  述 : 取得当前App的可升级目标文件在服务器位置
输入参数说明 :
    HNet				: LIN_xxxOpenLinCloud 的 pRetHNet，参考Demo
    strVersion			: 当前App版本
输出参数说明 :
    ppRetUpdatePath		: 当前App的可升级目标文件在服务器位置 用于 LIN_DownloadUpdateFile
        注意：此处使用双指针，是为了灵活返回不同长度的数据，此返回值的有效期到下次调用此函数前，之后如果再使用，有不确定的结果，特别注意！！！
返 回 值: LIN_ExceptionEnm　类型，　字面定义大概可了解意思，详细说明，调用　LIN_GetRetInfo　查看
其它说明: 只有当前版本小于服务器版本时，才返回服务器升级路径；版本比较使用区分大小写的字符串比较
***********************************************************************************/
int LINCloudDevSDKThread::GetUpdateInfo()
{

    if(LinCloudIsValid==false)
    {
         return  1;
    }
    HomeControlMainDlg *pHomeControlMainDlg = (HomeControlMainDlg*)parent();

    LIN_ExceptionEnm Enm =
    LIN_GetUpdateInfo(__HNet, pHomeControlMainDlg->Version.toLatin1().data(),
                      &updatePath);
    if(Enm == LIN_ExceptionEnm_OK)
    {

           return 0;
     }
    else
    {
           return 2;
     }

            return  2;
}

/***********************************************************************************
函数名 : LIN_DownloadUpdateFile
描  述 : 下载当前App的可升级目标文件到本机指定位置
输入参数说明 :
    HNet				: LIN_xxxOpenLinCloud 的 pRetHNet，参考Demo
    SvrUpdatePath 	    : 当前App可升级文件在服务器路径，通过 LIN_GetUpdateInfo 取得
    SvrDownloadPath	    : 当前App可升级文件下载位置
返 回 值: LIN_ExceptionEnm　类型，　字面定义大概可了解意思，详细说明，调用　LIN_GetRetInfo　查看
其它说明:
***********************************************************************************/
bool LINCloudDevSDKThread::DownloadUpdateFile()
{
    QFile file(updatename);
    if(file.exists())
    {
        file.remove();
    }
    LIN_ExceptionEnm Enm = LIN_DownloadUpdateFile(__HNet, updatePath,updatename);
    if(Enm == LIN_ExceptionEnm_OK)
    {
       return  true;
    }
   else
    {
       return  false;
    }
    return   false;
}

