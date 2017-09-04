#include "luxdomomusiccontrol.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   QDeclarativeItem    *smartHomeItem;
extern   MusicInfo   musicInfo;
LuxDomoMusicControl::LuxDomoMusicControl(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoMusicControl::~LuxDomoMusicControl()
{

}

void  LuxDomoMusicControl::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(0);
    parentItem()->setY(0);
    parentItem()->hide();
}

void   LuxDomoMusicControl::init(QString   roomUid, QString   deviceUid)
{
    m_deviceElement=g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->LoadingDeviceConditionData(roomUid,deviceUid);
    if(m_deviceElement.isNull())
    {
        parentItem()->hide();
    }
    int deviceId=m_deviceElement.attribute("id").toInt();
    m_musicAddr=deviceId & 0xff;
    m_musicAttr=(deviceId>>8) & 0xff;
    m_roomUid=roomUid;
    if(m_deviceElement.attribute("power")==tr("on")||m_musicAttr==Music_Infrared)
    {
        parentItem()->setProperty("musicCondition",true);
    }
    else
    {
        parentItem()->setProperty("musicCondition",false);
    }
    if(m_deviceElement.attribute("play")==tr("on"))
    {
        parentItem()->setProperty("muiscIsRunning",true);
    }
    else
    {
        parentItem()->setProperty("muiscIsRunning",false);
    }
    if(m_deviceElement.attribute("mute")==tr("on"))
    {
        parentItem()->setProperty("musicMute",true);
    }
    else
    {
        parentItem()->setProperty("musicMute",false);
    }
    parentItem()->setProperty("musicMode",m_deviceElement.attribute("source"));
    parentItem()->setProperty("musicVolume",m_deviceElement.attribute("value").toInt());
    parentItem()->setProperty("deviceUid",deviceUid);
    parentItem()->setProperty("musicName",m_deviceElement.attribute("name"));
    parentItem()->setProperty("musicGallery",m_deviceElement.attribute("channel").toInt()+1);
    parentItem()->setProperty("songName",m_deviceElement.attribute("musicname"));
    MyMsgLog<<m_deviceElement.attribute("power")<<m_deviceElement.attribute("source")<<m_deviceElement.attribute("value");
}

void   LuxDomoMusicControl::findMusicInformation()
{
    if(m_musicAttr==BackAudioMusic)
    {
        MusicstrCmdSend(0x0f);
    }
}

void   LuxDomoMusicControl::close()
{
    parentItem()->hide();
    parentItem()->setParentItem(smartHomeItem);
    if( g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->roomControl->controlDeviceItem=NULL;
    }
}

void  LuxDomoMusicControl::MusicstrCmdSend(int  cmd)
{
    MyMsgLog<<cmd;  MyMsgLog<<m_musicAddr<<m_musicAttr;
    if(cmd < 0)
    {
        return;
    }
    QByteArray m_array;
    switch (m_musicAttr)
    {
    case BackAudioMusic:     //第三方背景音乐
        m_array.resize(6);
        m_array.data()[0] = 0x06;
        m_array.data()[1] = m_deviceElement.attribute("channel").toInt();
        m_array.data()[2] = 0x00;
        m_array.data()[3] = m_musicAttr;
        m_array.data()[4] = cmd;      //指令
        m_array.data()[5] = 0x00;       //校验和，未校验
        break;
    case Music_Infrared:            //  红外背景音乐
        m_array.resize(8);
        m_array.data()[0] = 0x08;
        m_array.data()[1] = m_musicAddr;
        m_array.data()[2] = 0x00;
        m_array.data()[3] = m_musicAttr;
        m_array.data()[4] = 0x20;      //指令
        m_array.data()[5] = cmd;
        m_array.data()[6] = 0x01;
        m_array.data()[7] = 0x00;    //校验和，未校验
        break;
     default:
        break;
    }
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
}

void   LuxDomoMusicControl::musicPowerClicked()
{
    int   cmd=-1;   //指令
    QString    mFunName;
    QString    attributeValue;
    if(m_deviceElement.attribute("power")==tr("off"))
    {
        cmd=0x01;
        mFunName=tr("打开");
        attributeValue=tr("on");
        parentItem()->setProperty("muiscIsRunning",true);
        m_deviceElement.setAttribute("play",attributeValue);
    }
    else
    {
        cmd=0x00;
        mFunName=tr("关闭");
        attributeValue=tr("off");
    }

    switch (m_musicAttr)
    {
    case    BackAudioMusic:     //第三方背景音乐
        break;
    case    Music_Infrared:       //  红外背景音乐
        cmd = obtainMusicFunOrder(mFunName);
        break;
    default:
        break;
    }

    MusicstrCmdSend(cmd);
    if(attributeValue==tr("on")||m_musicAttr==Music_Infrared)
    {
        parentItem()->setProperty("musicCondition",true);
        QMetaObject::invokeMethod(parentItem()->parentItem(),"on_offDevice",Qt::DirectConnection,Q_ARG(QVariant,m_deviceElement.attribute(tr("uid"))),Q_ARG(QVariant,true));
        if(m_musicAttr==BackAudioMusic)
        {
            MusicstrCmdSend(0x0f);
        }
    }
    else
    {
        parentItem()->setProperty("musicCondition",false);
        QMetaObject::invokeMethod(parentItem()->parentItem(),"on_offDevice",Qt::DirectConnection,Q_ARG(QVariant,m_deviceElement.attribute(tr("uid"))),Q_ARG(QVariant,false));
    }
    m_deviceElement.setAttribute("power",attributeValue);
}

void   LuxDomoMusicControl::musicModeClicked()
{
    QString    currentModeValue =m_deviceElement.attribute("source");
    QString    mFunName;
    QByteArray   m_array;
    int   cmd = -1;
    int   nMusic = 0;

   switch (m_musicAttr)
   {
   case BackAudioMusic:  //第三方背景音乐
                  for(int i=0;i<musicInfo.musicBackMode.size();i++)
                  {
                      if(currentModeValue ==musicInfo.musicBackMode.at(i))
                      {
                          nMusic=i;
                          nMusic=(nMusic+1)%musicInfo.musicBackMode.size();
                          mFunName = musicInfo.musicBackMode.at(nMusic);
                          cmd=nMusic;
                          break;
                      }
                  }
                  parentItem()->setProperty("musicMode",mFunName);
                  m_array.resize(7);
                  m_array.data()[0] = 0x07;
                  m_array.data()[1] = m_deviceElement.attribute("channel").toInt();
                  m_array.data()[2] = 0x00;
                  m_array.data()[3] = m_musicAttr;
                  m_array.data()[4] = 0x04;      //指令
                  m_array.data()[5] = cmd;
                  m_array.data()[6] = 0x00;    //校验和，未校验
                   g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
                  m_array.clear();
                  break;
   case Music_Infrared:  //普通音乐
                    for(int i=0;i<musicInfo.musicInfraredMode.size();i++)
                    {
                        if(currentModeValue==musicInfo.musicInfraredMode.at(i))
                        {
                            nMusic=i;
                            nMusic=(nMusic+1)%musicInfo.musicInfraredMode.size();
                            mFunName=musicInfo.musicInfraredMode.at(nMusic);
                            break;
                        }
                    }
                    parentItem()->setProperty("musicMode",mFunName);
                    cmd = obtainMusicFunOrder(mFunName);
                    MusicstrCmdSend(cmd);
                    break;
   default:
                   break;
   }
   m_deviceElement.setAttribute("source",mFunName);
}

void  LuxDomoMusicControl::musicPreviousClicked()
{
    int cmd = -1;
    QString mFunName = "上一曲";
    switch (m_musicAttr)
    {
    case BackAudioMusic:
               cmd = 0x07;
               break;
   case Music_Infrared:
               cmd = obtainMusicFunOrder(mFunName);
               break;
   default:
               break;
    }
    MusicstrCmdSend(cmd);
    if(m_musicAttr==BackAudioMusic)
    {
        MusicstrCmdSend(0x0f);
    }
}

void  LuxDomoMusicControl::musicPauseClicked(QString  value)
{
    int cmd = -1;
    QString mFunName;
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
    switch (m_musicAttr)
    {
    case BackAudioMusic:
                break;
   case Music_Infrared:
                 cmd = obtainMusicFunOrder(mFunName);
                 break;
   default:
                 break;
   }
    MusicstrCmdSend(cmd);
    m_deviceElement.setAttribute("play",value);
}

void  LuxDomoMusicControl::musicNextClicked()
{
    int cmd = -1;
    QString mFunName = "下一曲";
    switch (m_musicAttr)
    {
    case BackAudioMusic:
                 cmd = 0x08;
                 break;
    case Music_Infrared:
                 cmd = obtainMusicFunOrder(mFunName);
                 break;
    default:
                  break;
    }
    MusicstrCmdSend(cmd);
    if(m_musicAttr==BackAudioMusic)
    {
        MusicstrCmdSend(0x0f);
    }
}

void   LuxDomoMusicControl::musicVolumeClicked(int   value)
{
    g_pMainDlg->m_pHomeControlDlg->m_musicVolume=true;
    g_pMainDlg->m_pHomeControlDlg->timer1->start(3000);

    QByteArray   m_array;
    int  oldVlaue=m_deviceElement.attribute(tr("value")).toInt();
    if(value == oldVlaue )
    {
        return;
    }
    QString mFunName;
    int cmd = -1;
    if(value > oldVlaue)
    {
        cmd = 0x05;
        mFunName = "音量递增";
    }
    else
    {
        cmd = 0x06;
        mFunName = "音量递减";
    }
    switch (m_musicAttr)
    {
    case BackAudioMusic:
        m_array.resize(7);
        m_array.data()[0] = 0x07;
        m_array.data()[1] = m_deviceElement.attribute("channel").toInt();
        m_array.data()[2] = 0x00;
        m_array.data()[3] = m_musicAttr;
        m_array.data()[4] = 0x09;      //指令
        m_array.data()[5] = value;
        m_array.data()[6] = 0x00;    //校验和，未校验
        g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
        m_array.clear();
        break;
    case Music_Infrared:
        cmd = obtainMusicFunOrder(mFunName);
        MusicstrCmdSend(cmd);
        break;
    default:
        break;
    }
    m_deviceElement.setAttribute("value",QString::number(value));
}

void   LuxDomoMusicControl::musicChannelClicked()
{
    if(m_musicAttr!=BackAudioMusic)
    {
        return;
    }
    int  channelSize=m_deviceElement.attribute("channelSize").toInt();
    int  value=m_deviceElement.attribute(tr("channel")).toInt();
    value=(value+1)%channelSize;
    parentItem()->setProperty("musicGallery",value+1);
    m_deviceElement.setAttribute("channel",QString::number(value));
    MusicstrCmdSend(0x0f);
    parentItem()->setProperty("musicMode",tr(""));
    parentItem()->setProperty("songName",tr(""));
    m_deviceElement.setAttribute("musicname",tr(""));
}

void   LuxDomoMusicControl::musicMuteClicked(QString   value)
{
    int cmd = -1;
    QString mFunName;
    if(value==tr("on"))
    {
        cmd=0x0a;
        mFunName=tr("静音");
    }
    else
    {
        cmd=0x10;
        mFunName=tr("非静音");
    }
    switch (m_musicAttr)
    {
    case BackAudioMusic:
                break;
   case Music_Infrared:
                 cmd = obtainMusicFunOrder(mFunName);
                 break;
   default:
                 break;
   }
    MusicstrCmdSend(cmd);
    m_deviceElement.setAttribute("mute",value);
}

int     LuxDomoMusicControl::obtainMusicFunOrder(QString funName)
{
    QSqlQuery query;
    QString mSqlOrder;
    int mOrder;
    bool mIsLearned;
    bool mOrderIsValid;
    mSqlOrder = "select code,isLearned from music" + m_deviceElement.attribute("uid") + " where funName = " +
            "'" + funName + "'";
    MyMsgLog<<mSqlOrder;
    mOrderIsValid = query.exec(mSqlOrder);
    if(!mOrderIsValid || !query.next())
    {
        g_pMainDlg->m_pHomeControlDlg->ShowHintInformation(tr("该指令未红外学习"));
        return -1;
    }
    mOrder = query.value(0).toInt();
    mIsLearned = query.value(1).toBool();
    MyMsgLog<<mOrder<<mIsLearned;
    if(!mIsLearned)
    {
        g_pMainDlg->m_pHomeControlDlg->ShowHintInformation(tr("该指令未红外学习"));
        return -2;
    }
    else
    {
        return mOrder;
    }
}
