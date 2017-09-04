#include "luxdomosendcommand.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   MusicInfo   musicInfo;
extern   AcInfo    acInfo;
LuxDomoSendCommand::LuxDomoSendCommand(QObject *parent) : QObject(parent)
{

}

LuxDomoSendCommand::~LuxDomoSendCommand()
{

}

void   LuxDomoSendCommand::ChangDeviceAttribute(QString roomObjectName, QString deviceObjectName, QString  attributeName, QString attributeValue)
{
    QDomElement   houseElement;
    QDomElement   roomElement;
    QDomElement   layerElement;
    QDomElement   deviceElement;
    houseElement = g_pMainDlg->m_pHomeControlDlg->m_domDoc.firstChildElement("house");
    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            if(roomElement.attribute("uid")==roomObjectName)
            {
                deviceElement=roomElement.firstChildElement("device");
                while(!deviceElement.isNull())
                {
                    if(deviceElement.attribute("uid")==deviceObjectName)
                    {
                        deviceElement.setAttribute(attributeName,attributeValue);
                        MyMsgLog<<attributeName<<attributeValue;
                        return;
                    }
                    deviceElement=deviceElement.nextSiblingElement("device");
                }
            }
            roomElement=roomElement.nextSiblingElement("room");
        }
        layerElement=layerElement.nextSiblingElement("layer");
    }
}

void   LuxDomoSendCommand::CurtainPowerControl(QString  value,int  deviceId,QString roomUid,QString deviceUid)
{
    int cmd=-1;
    if(tr("on")==value)
    {
         cmd=0x01;
    }
    else  if(tr("off")==value)
    {
         cmd=0x00;
    }
    else   if(tr("stop")==value)
    {
         cmd=0x02;
    }
    QByteArray m_array;
    m_array.resize(6);
    m_array.data()[0]=0x06;
    m_array.data()[1]=deviceId&0xff;;
    m_array.data()[2]=0x00;
    m_array.data()[3]=deviceId>>8;
    m_array.data()[4]=cmd;      //指令
    m_array.data()[5]=0x00;
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
    ChangDeviceAttribute(roomUid,deviceUid,tr("power"),value);
}

void   LuxDomoSendCommand::LightPowerControl(QString  value,int  deviceId,QString roomUid,QString deviceUid)
{
    int cmd=-1;
    if(tr("on")==value)
    {
         cmd=0x01;
    }
    else  if(tr("off")==value)
    {
         cmd=0x00;
    }
    QByteArray m_array;
    m_array.resize(6);
    m_array.data()[0]=0x06;
    m_array.data()[1]=deviceId&0xff;;
    m_array.data()[2]=0x00;
    m_array.data()[3]=deviceId>>8;
    m_array.data()[4]=cmd;      //指令
    m_array.data()[5]=0x00;
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
    ChangDeviceAttribute(roomUid,deviceUid,tr("power"),value);
}

void  LuxDomoSendCommand::LightBrightnessControl(QString  value,int  deviceId,QString roomUid,QString deviceUid)
{
    int  cmd=value.toInt();
    QByteArray m_array;
    m_array.resize(7);
    m_array.data()[0]=0x07;
    m_array.data()[1]=deviceId&0xff;;
    m_array.data()[2]=0x00;
    m_array.data()[3]=deviceId>>8;
    m_array.data()[4]=0x11;      //指令
    m_array.data()[5]=cmd;
    m_array.data()[6]=0x00;
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
    ChangDeviceAttribute(roomUid,deviceUid,tr("value"),value);
}

int    LuxDomoSendCommand::obtainTVFunOrder(QString funName,QString deviceUid)
{
    QSqlQuery query;
    QString mSqlOrder;
    int     mOrder;
    bool  mIsLearned;
    bool  mOrderIsValid;
    mSqlOrder = "select code,isLearned from tv" +deviceUid + " where funName = " +
            "'" + funName + "'";
    MyMsgLog<<mSqlOrder;
    mOrderIsValid = query.exec(mSqlOrder);
    if(!mOrderIsValid || !query.next())
    {
        return -1;
    }
    mOrder = query.value(0).toInt();
    mIsLearned = query.value(1).toBool();
    MyMsgLog<<mOrder<<mIsLearned;
    if(!mIsLearned)
    {
        return -2;
    }
    else
    {
        return mOrder;
    }
}

void   LuxDomoSendCommand::TvPowerControl(QString  value,int  deviceId,QString roomUid,QString deviceUid)
{
    int   cmd=-1;   //指令
    QString    mFunName;
    if(tr("on")==value)
    {
        mFunName=tr("开机");
    }
    else
    {
        mFunName=tr("关机");
    }
    cmd = obtainTVFunOrder(mFunName,deviceUid);
    QByteArray m_array;
    m_array.resize(8);
    m_array.data()[0] = 0x08;
    m_array.data()[1] = deviceId&0xff;;
    m_array.data()[2] = 0x00;
    m_array.data()[3] = deviceId>>8;
    m_array.data()[4] = 0x20;      //指令
    m_array.data()[5] = cmd;
    m_array.data()[6] = 0x01;
    m_array.data()[7] = 0x00;    //校验和，未校验
    if(cmd>=0)
    {
        g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
    }
    ChangDeviceAttribute(roomUid,deviceUid,tr("power"),value);
}

void   LuxDomoSendCommand::TvModeControl(QString  value,int  deviceId,QString roomUid,QString deviceUid)
{
    int cmd = -1;
    QString  mFunName;
    if(value==tr("tv"))
    {
        mFunName = tr("AV模式");
    }
    else
    {
        mFunName = tr("TV模式");
    }
    cmd = obtainTVFunOrder(mFunName,deviceUid);
    QByteArray m_array;
    m_array.resize(8);
    m_array.data()[0] = 0x08;
    m_array.data()[1] = deviceId&0xff;
    m_array.data()[2] = 0x00;
    m_array.data()[3] = deviceId>>8;
    m_array.data()[4] = 0x20;      //指令
    m_array.data()[5] = cmd;
    m_array.data()[6] = 0x01;
    m_array.data()[7] = 0x00;    //校验和，未校验
    if(cmd>=0)
    {
         g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
    }
    ChangDeviceAttribute(roomUid,deviceUid,tr("mode"),value);
}

int     LuxDomoSendCommand::obtainMusicFunOrder(QString funName,QString  deviceUid)
{
    QSqlQuery query;
    QString mSqlOrder;
    int mOrder;
    bool mIsLearned;
    bool mOrderIsValid;
    mSqlOrder = "select code,isLearned from music" + deviceUid + " where funName = " +
            "'" + funName + "'";
    MyMsgLog<<mSqlOrder;
    mOrderIsValid = query.exec(mSqlOrder);
    if(!mOrderIsValid || !query.next())
    {
        return -1;
    }
    mOrder = query.value(0).toInt();
    mIsLearned = query.value(1).toBool();
    MyMsgLog<<mOrder<<mIsLearned;
    if(!mIsLearned)
    {
        return -2;
    }
    else
    {
        return mOrder;
    }
}

void   LuxDomoSendCommand::MusicPowerControl(QString  value,int  deviceId,int  channel,QString roomUid,QString deviceUid)
{
    int   cmd=-1;   //指令
    QString    mFunName;
    if(tr("on")==value)
    {
        cmd=0x01;
        mFunName=tr("打开");
    }
    else
    {
        cmd=0x00;
        mFunName=tr("关闭");
    }
   QByteArray m_array;
    switch (deviceId>>8)
    {
            case    BackAudioMusic:     //第三方背景音乐
                              m_array.resize(6);
                              m_array.data()[0] = 0x06;
                              m_array.data()[1] = channel;
                              m_array.data()[2] = 0x00;
                              m_array.data()[3] =deviceId>>8;
                              m_array.data()[4] = cmd;      //指令
                              m_array.data()[5] = 0x00;       //校验和，未校验
                              break;
           case    Music_Infrared:   //  红外背景音乐
                              cmd = obtainMusicFunOrder(mFunName,deviceUid);
                              m_array.resize(8);
                              m_array.data()[0] = 0x08;
                              m_array.data()[1] = deviceId&0xff;
                              m_array.data()[2] = 0x00;
                              m_array.data()[3] = deviceId>>8;
                              m_array.data()[4] = 0x20;      //指令
                              m_array.data()[5] = cmd;
                              m_array.data()[6] = 0x01;
                              m_array.data()[7] = 0x00;    //校验和，未校验
                              break;
            default:
                        break;
    }
    if(cmd>=0)
    {
         g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
    }
    ChangDeviceAttribute(roomUid,deviceUid,tr("power"),value);
}

void LuxDomoSendCommand::MusicModeControl(QString  value,int  deviceId,int  channel,QString roomUid,QString deviceUid)
{
    QByteArray   m_array;
    int   cmd = -1;
    int   size=musicInfo.musicBackMode.size();
    switch (deviceId>>8)
    {
            case    BackAudioMusic:     //第三方背景音乐
                              for(int i=0;i<size;i++)
                              {
                                     if(value == musicInfo.musicBackMode.at(i))
                                     {
                                            cmd=i;
                                            break;
                                     }
                              }
                              m_array.resize(7);
                              m_array.data()[0] = 0x07;
                              m_array.data()[1] = channel;
                              m_array.data()[2] = 0x00;
                              m_array.data()[3] = deviceId>>8;
                              m_array.data()[4] = 0x04;      //指令
                              m_array.data()[5] = cmd;
                              m_array.data()[6] = 0x00;    //校验和，未校验
                              break;
           case    Music_Infrared:   //  红外背景音乐
                              cmd = obtainMusicFunOrder(value,deviceUid);
                              m_array.resize(8);
                              m_array.data()[0] = 0x08;
                              m_array.data()[1] = deviceId&0xff;
                              m_array.data()[2] = 0x00;
                              m_array.data()[3] = deviceId>>8;
                              m_array.data()[4] = 0x20;      //指令
                              m_array.data()[5] = cmd;
                              m_array.data()[6] = 0x01;
                              m_array.data()[7] = 0x00;    //校验和，未校验
                              break;
            default:
                              break;
    }
    if(cmd>=0)
    {
         g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
    }
    ChangDeviceAttribute(roomUid,deviceUid,tr("source"),value);
}

void   LuxDomoSendCommand::MusicPauseControl(QString  value,int  deviceId,int  channel,QString roomUid,QString deviceUid)
{
    int cmd = -1;
    QString mFunName;
    QByteArray   m_array;
    if(value==tr("on"))
    {
          cmd=0x02;
          mFunName=tr("播放");
    }
    else
    {
         cmd=0x03;
         mFunName=tr("暂停");
    }
    switch (deviceId>>8)
    {
            case BackAudioMusic:
                        m_array.resize(6);
                        m_array.data()[0] = 0x06;
                        m_array.data()[1] = channel;
                        m_array.data()[2] = 0x00;
                        m_array.data()[3] =deviceId>>8;
                        m_array.data()[4] = cmd;      //指令
                        m_array.data()[5] = 0x00;       //校验和，未校验
                        break;
           case Music_Infrared:
                         cmd = obtainMusicFunOrder(mFunName,deviceUid);
                         m_array.resize(8);
                         m_array.data()[0] = 0x08;
                         m_array.data()[1] = deviceId&0xff;
                         m_array.data()[2] = 0x00;
                         m_array.data()[3] = deviceId>>8;
                         m_array.data()[4] = 0x20;      //指令
                         m_array.data()[5] = cmd;
                         m_array.data()[6] = 0x01;
                         m_array.data()[7] = 0x00;    //校验和，未校验
                         break;
           default:
                         break;
   }
    if(cmd>=0)
    {
         g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
    }
    ChangDeviceAttribute(roomUid,deviceUid,tr("play"),value);
}

void  LuxDomoSendCommand::MusicVolumeControl(QString  value,int  deviceId,int  channel,QString roomUid,QString deviceUid)
{
    QByteArray   m_array;
    if(deviceId>>8==BackAudioMusic)
    {
        m_array.resize(7);
        m_array.data()[0] = 0x07;
        m_array.data()[1] = channel;
        m_array.data()[2] = 0x00;
        m_array.data()[3] = deviceId>>8;
        m_array.data()[4] = 0x09;      //指令
        m_array.data()[5] = value.toInt();
        m_array.data()[6] = 0x00;    //校验和，未校验
        g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
        ChangDeviceAttribute(roomUid,deviceUid,tr("value"),value);
        m_array.clear();
    }
}

void   LuxDomoSendCommand::AcPowerControl(QString  value,int  deviceId,QString roomUid,QString deviceUid)
{
    int   cmd=-1;   //指令
    if(value==tr("on"))
    {
        cmd = 0x01;
    }
    else
    {
         cmd = 0x00;
    }
    QByteArray m_array;
    m_array.resize(6);
    m_array.data()[0]=0x06;
    m_array.data()[1]=deviceId&0xff;;
    m_array.data()[2]=0x00;
    m_array.data()[3]=deviceId>>8;
    m_array.data()[4]=cmd;      //指令
    m_array.data()[5]=0x00;
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
    ChangDeviceAttribute(roomUid,deviceUid,tr("power"),value);
}

void  LuxDomoSendCommand::AcModeControl(QString  value,int  deviceId,QString roomUid,QString deviceUid)
{
    int   cmd=-1;   //指令
   QByteArray   m_array;
    int   size=acInfo.acModeEn.size();
    for(int i=0;i<size;i++)
    {
        if(acInfo.acModeEn.at(i)==value)
        {
            cmd=i;
            break;
        }
    }
    m_array.resize(7);
    m_array.data()[0] = 0x07;
    m_array.data()[1] = deviceId&0xff;
    m_array.data()[2] = 0x00;
    m_array.data()[3] = deviceId>>8;
    m_array.data()[4] = 0x03;
    m_array.data()[5] = cmd;
    m_array.data()[6] = 0x00;
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
    ChangDeviceAttribute(roomUid,deviceUid,tr("mode"),value);
}

void   LuxDomoSendCommand::AcWindControl(QString  value,int  deviceId,QString roomUid,QString deviceUid)
{
    int   cmd=-1;   //指令
   QByteArray   m_array;
    int   size=acInfo.acCentralWindEn.size();
    for(int i=0;i<size;i++)
    {
        if(acInfo.acCentralWindEn.at(i)==value)
        {
            cmd=i;
            break;
        }
    }
    m_array.resize(7);
    m_array.data()[0] = 0x07;
    m_array.data()[1] = deviceId&0xff;
    m_array.data()[2] = 0x00;
    m_array.data()[3] = deviceId>>8;
    m_array.data()[4] = 0x05;
    m_array.data()[5] = cmd;
    m_array.data()[6] = 0x00;
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
    ChangDeviceAttribute(roomUid,deviceUid,tr("swing"),value);
}

void  LuxDomoSendCommand::AcSpeedControl(QString  value,int  deviceId,QString roomUid,QString deviceUid)
{
    int   cmd=-1;   //指令
    QByteArray   m_array;
    int   size=acInfo.acCentralSpeedEn.size();
    for(int i=0;i<size;i++)
    {
        if(acInfo.acCentralSpeedEn.at(i)==value)
        {
            cmd=i;
            break;
        }
    }
    m_array.resize(7);
    m_array.data()[0] = 0x07;
    m_array.data()[1] = deviceId&0xff;
    m_array.data()[2] = 0x00;
    m_array.data()[3] = deviceId>>8;
    m_array.data()[4] = 0x04;
    m_array.data()[5] = cmd;
    m_array.data()[6] = 0x00;
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
    ChangDeviceAttribute(roomUid,deviceUid,tr("wind"),value);
}

void  LuxDomoSendCommand::AcTempControl(QString  value,int  deviceId,QString roomUid,QString deviceUid)
{
    int   cmd=value.toInt();   //指令
    QByteArray   m_array;
    m_array.resize(7);
    m_array.data()[0] = 0x07;
    m_array.data()[1] = deviceId&0xff;
    m_array.data()[2] = 0x00;
    m_array.data()[3] = deviceId>>8;
    m_array.data()[4] = 0x02;
    m_array.data()[5] = cmd;
    m_array.data()[6] = 0x00;
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
    ChangDeviceAttribute(roomUid,deviceUid,tr("temperature"),value);
}

bool   LuxDomoSendCommand::AcInfraredControl(QString power,QString mode,QString wind, QString speed, QString temp, int  deviceId,QString roomUid,QString deviceUid)
{
    int     cmd=-3;
    bool  mIsLearned;
    bool  mOrderIsValid;
    QByteArray   m_array;
    QString   acStatus=tr("开");
    QString   acWind;
    QString   acSpeed;
    QString   acTemp;
    QString   acMode;

    if(power==tr("off"))
    {
        acStatus=tr("关");
        acWind=tr("-");
        acSpeed=tr("-");
        acTemp=tr("-");
        acMode=tr("-");
    }
    else
    {
        acTemp=temp+ tr("℃");
        acMode=g_pMainDlg->m_pHomeControlDlg->GetAcModeName(mode);
        acSpeed=g_pMainDlg->m_pHomeControlDlg->GetAcInfraredSpeedName(speed);
        if(wind==tr("on"))
        {
             acWind=tr("摆风");
        }
        else
        {
             acWind=tr("固定");
        }
    }

    QSqlQuery query;
    QString     head = "select code,isLearned from ac" + deviceUid + " where ";
    QString    _power = "power ='" + acStatus + "'" + " and";
    QString    _wind = " wind = '" + acWind + "'" + " and";
    QString    _mode = " mode = '" + acMode + "'" + " and";
    QString    _speed = " speed = '" + acSpeed + "'" + " and";
    QString    _temp = " temp = '" + acTemp +"'";
    QString    sql = head + _power  +_wind + _mode + _speed + _temp;      //合并数据库查询语句
    mOrderIsValid = query.exec(sql);
    MyMsgLog<<mOrderIsValid<<sql;
    ChangDeviceAttribute(roomUid,deviceUid,tr("mode"),mode);
    ChangDeviceAttribute(roomUid,deviceUid,tr("wind"),speed);
    ChangDeviceAttribute(roomUid,deviceUid,tr("swing"),wind);
    ChangDeviceAttribute(roomUid,deviceUid,tr("power"),power);
    ChangDeviceAttribute(roomUid,deviceUid,tr("temperature"),temp);

    if(!mOrderIsValid || !query.next())
    {
         return false;
    }
    cmd = query.value(0).toInt();
    mIsLearned = query.value(1).toBool();
    if(!mIsLearned)
    {
        return false;
    }
    else
    {
        m_array.resize(8);
        m_array.data()[0] = 0x08;
        m_array.data()[1] =deviceId&0xff;
        m_array.data()[2] = 0x00;
        m_array.data()[3] = deviceId>>8;
        m_array.data()[4] = 0x20;
        m_array.data()[5] = cmd;
        m_array.data()[6] = 0x01;
        m_array.data()[7] = 0x00;
        g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
        return  true;
    }
}
