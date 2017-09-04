#include "homecontrolsceneedit.h"
#include<QFile>
#include<QTime>
#include<homecontrolmaindlg.h>

extern const char *ac_mode[5];
extern const char *ac_wind[4];
extern const char *music_source[4];
extern char *ac_mode_display[5];
extern char *ac_wind_display[4];
extern QDeclarativeItem *item;
extern QString global_ac_mode_display(short n);
extern QString global_ac_wind_display(short n);
extern short layerId,roomId;

HomeControlSceneEdit::HomeControlSceneEdit(QObject *parent) :
    QObject(parent)
{
    QMetaObject::invokeMethod(item,"hcsceneeditEvent",
                              Q_ARG(QVariant, "clearModel"),
                              Q_ARG(QVariant, ""),
                              Q_ARG(QVariant, ""));
    setCancel(tr("取消"));
    setFinish(tr("完成"));
}


bool HomeControlSceneEdit::loadDevice(int id)
{
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg*)parent();
    if(pHomeControlDlg->m_phouseMapXmlThread->isRunning())
        return false;
    doc.clear();
    m_sceneXmlFileName = "home_control/house-map-user-"+QString::number(id+1)+".xml"; 
    //pHomeControlDlg->m_pLoadSceneDev->start();
    pHomeControlDlg->m_phouseMapXmlThread->eventType = scenedeviceload;
    pHomeControlDlg->m_phouseMapXmlThread->start();
    return true;
}


/*************************判断编辑设备的类型，状态******************************/
void HomeControlSceneEdit::eidtDevice(bool bPowerOn,QString devId,QString devType,bool ir,QString actflag)
{
    QDomElement deviceEelement;
    deviceEelement = m_deviceElement;
    while(!deviceEelement.isNull())
    {
        if(deviceEelement.attribute("id") == devId && deviceEelement.attribute("type")==devType)
        {
            n_deviceElement = deviceEelement;
            if(!actflag.isEmpty())
            {
                deviceEelement.setAttribute("actflag",actflag);
                return;
            }

            if(bPowerOn)
            {
                if(ir)
                {
                    deviceEelement.setAttribute("power","on");
                    return;
                }
                QString typeName = deviceEelement.attribute("type");
                QString sub_type = deviceEelement.attribute("subtype");

                int type = 0;
                if(typeName == "light")
                {
                    if(sub_type == "var")
                        type = 1;
                }
                else if(typeName == "ac")
                    type = 4;
                else if(typeName == "tv")
                    type =5;
                else if(typeName == "music")
                    type = 6;

                QMetaObject::invokeMethod(item,"hcsceneeditEvent",
                                          Q_ARG(QVariant, type),
                                          Q_ARG(QVariant, ""),
                                          Q_ARG(QVariant, ""),
                                          Q_ARG(QVariant, ""),
                                          Q_ARG(QVariant, ""),
                                          Q_ARG(QVariant, ""));
                QString arg1,arg2,arg3,arg4;
                if(type == 1)
                {
                    arg1 = deviceEelement.attribute("value");
                    QMetaObject::invokeMethod(item,"lightresetStat",Q_ARG(QVariant, arg1));
                }
                if(type == 4)
                {
                    arg1 = deviceEelement.attribute("swing");
                    arg2 = deviceEelement.attribute("mode");
                    arg3 = deviceEelement.attribute("wind");
                    arg4 = deviceEelement.attribute("temperature");
                    qDebug()<<arg1<<arg2<<arg3<<arg4;
                    if(arg2 == "auto")
                        arg2 = "automode";
                    if(arg3 == "auto")
                        arg3 = "windauto";

                    if(arg1 == "off")
                        setSwing(tr("固定"));
                    else
                        setSwing(tr("摆风"));
                    for(int i=0;i<5;i++)
                    {
                        if(ac_mode[i] == arg2)
                        {
                            arg2 = global_ac_mode_display(i);
                            setMode(arg2);
                        }
                    }
                    for(int i=0;i<4;i++)
                    {
                        if(ac_wind[i] == arg3)
                        {
                            arg3 = global_ac_wind_display(i);
                            setWind(arg3);
                        }
                    }
                    setTemp(arg4);
                }
                if(type == 5)
                {
                    arg1 = deviceEelement.attribute("mode");
                    QMetaObject::invokeMethod(item,"tvresetStat",Q_ARG(QVariant, arg1));
                }
                if(type == 6)
                {
                    arg1 = deviceEelement.attribute("source");
                    arg2 = deviceEelement.attribute("mute");
                    if(arg2 == "off")
                        setMute(tr("静音"));
                    else
                        setMute(tr("非静音"));
                    setSource(arg1);
                }
            }
            else
            {
                if(ir)
                {
                    deviceEelement.setAttribute("power","off");
                    return;
                }
            }
        }
        deviceEelement = deviceEelement.nextSiblingElement("device");
    }
}

/****************************场景设备编辑***********************/
void HomeControlSceneEdit::irDevEidt(QString type,QString arg)
{
    qDebug()<<type<<arg;
    n_deviceElement.setAttribute(type,arg);
}

/*************************全关*************************/
void HomeControlSceneEdit::setDevAllOff()
{
    QDomElement deviceElement = m_deviceElement;
    while (!deviceElement.isNull())
    {
        deviceElement.setAttribute("power","off");
        deviceElement = deviceElement.nextSiblingElement("device");
    }
}

/************************全开*************************/
void HomeControlSceneEdit::setDevAllOn()
{
    QDomElement deviceElement = m_deviceElement;
    while (!deviceElement.isNull())
    {
        deviceElement.setAttribute("power","on");
        deviceElement = deviceElement.nextSiblingElement("device");
    }
}

/****************************编辑完成*****************************/
void HomeControlSceneEdit::editFinish()
{
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg*)parent();
    QFile file(m_sceneXmlFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;
    QTextStream stream(&file);
    doc.save(stream, 4);
    file.close();
    sync();
    pHomeControlDlg->setMainState("");
}

void HomeControlSceneEdit::close()
{
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg*)parent();
    pHomeControlDlg->setMainState("");
    //pHomeControlDlg->hide();
    //QMetaObject::invokeMethod(item,"setState",Q_ARG(QVariant, ""));
}

/*************************空调模式************************/
void HomeControlSceneEdit::pushButtonACMode(QString action)
{
    short nMode;
    if(action=="next")
    {
        for(int i=0;i<5;i++)
        {
            if(m_Mode==global_ac_mode_display(i))
            {
                nMode=i;
                nMode=(nMode+1)%5;
                setMode(global_ac_mode_display(nMode));
                irDevEidt("mode",ac_mode[nMode]);
                break;
            }
        }
    }
    else if(action=="pre")
    {
        for(int i=0;i<5;i++)
        {
            if(m_Mode==global_ac_mode_display(i))
            {
                nMode=i;
                nMode=nMode-1;
                if(nMode<0)
                    nMode=4;
                setMode(global_ac_mode_display(nMode));
                irDevEidt("mode",ac_mode[nMode]);
                break;
            }
        }
    }
}

/**********************空调固定摆风************************/
void HomeControlSceneEdit::pushButtonACSwing(QString action)
{
    if(action == tr("固定"))
    {
        setSwing(tr("摆风"));
        irDevEidt("swing","on");
    }
    else
    {
        setSwing(tr("固定"));
        irDevEidt("swing","off");
    }
}

/*************************空调温度***************************/
void HomeControlSceneEdit::pushButtonACTemperature(QString action)
{
    short nTemp;
    nTemp=m_Tempture.toShort();
    if(action=="next")
    {
        if(++nTemp>32)
            nTemp=16;
        setTemp(QString::number(nTemp));
    }
    else if(action=="pre")
    {
        if(--nTemp<16)
            nTemp=32;
        setTemp(QString::number(nTemp));
    }
    irDevEidt("temperature",m_Tempture);
}

/**********************空调风速**************************/
void HomeControlSceneEdit::pushButtonACWind(QString action)
{
    short nWind;
    if(action=="next")
    {
        for(int i=0;i<4;i++)
        {
            if(m_Wind==global_ac_wind_display(i))
            {
                nWind=i;
                nWind=(nWind+1)%4;
                setWind(global_ac_wind_display(nWind));
                irDevEidt("wind",ac_wind[nWind]);
                break;
            }
        }
    }
    else if(action=="pre")
    {
        for(int i=0;i<4;i++)
        {
            if(m_Wind==global_ac_wind_display(i))
            {
                nWind=i;
                nWind=nWind-1;
                if(nWind<0)
                    nWind=3;
                setWind(global_ac_wind_display(nWind));
                irDevEidt("wind",ac_wind[nWind]);
                break;
            }
        }
    }
}

/*********************背景音乐静音和费静音设置**************************/
void HomeControlSceneEdit::pushButtonMusicMute(QString action)
{
    if(action == tr("静音"))
    {
        setMute(tr("非静音"));
        irDevEidt("mute","on");
    }
    else
    {
        setMute(tr("静音"));
        irDevEidt("mute","off");
    }
}

/**********************背景音乐资源*************************/
void HomeControlSceneEdit::pushButtonBMSource(QString action)
{
    int nMusic = 0;
    if(action=="next")
    {
        for(int i=0;i<4;i++)
        {
            if(m_Source==music_source[i])
            {
                nMusic=i;
                nMusic=(nMusic+1)%4;
                setSource(music_source[nMusic]);
                irDevEidt("source",music_source[nMusic]);
                break;
            }
        }
    }
    else if(action=="pre")
    {
        for(int i=0;i<4;i++)
        {
            if(m_Source==music_source[i])
            {
                nMusic=i;
                if(--nMusic<0)
                    nMusic=3;
                setSource(music_source[nMusic]);
                irDevEidt("source",music_source[nMusic]);
                break;
            }
        }
    }
}

/**********************空提模式*************************/
QString HomeControlSceneEdit::getMode(void) const
{
    return m_Mode;
}

void HomeControlSceneEdit::setMode(const QString &mode)
{
    m_Mode=mode;
    emit sig_mode();
}


/**********************空调温度*************************/
QString HomeControlSceneEdit::getTemp(void) const
{
    return m_Tempture;
}

void HomeControlSceneEdit::setTemp(const QString &temp)
{
    m_Tempture=temp;
    emit sig_temp();
}

/*******************空调摆风*******************/
QString HomeControlSceneEdit::getSwing(void) const
{
    return m_Swing;
}

void HomeControlSceneEdit::setSwing(const QString &swing)
{
    m_Swing=swing;
    emit sig_swing();
}


/******************空调风速**********************/
QString HomeControlSceneEdit::getWind(void) const
{
    return m_Wind;
}

void HomeControlSceneEdit::setWind(const QString &wind)
{
    m_Wind=wind;
    emit sig_wind();
}

/***********************音乐静音非静音************************/
QString HomeControlSceneEdit::mute()const
{
    return m_Mute;
}

void HomeControlSceneEdit::setMute(const QString &mute)
{
    m_Mute = mute;
    emit sig_Mute();
}

/********************音乐资源********************/
QString HomeControlSceneEdit::source()const
{
    return m_Source;
}
void HomeControlSceneEdit::setSource(const QString &source)
{
    m_Source = source;
    emit sig_Source();
}

/***********************完成****************************/
QString HomeControlSceneEdit::finish()const
{
    return m_Finish;
}

void HomeControlSceneEdit::setFinish(const QString &finish)
{
    m_Finish = finish;
    emit sig_Finish();
}

/************************取消***************************/
QString HomeControlSceneEdit::cancel()const
{
    return m_Cancel;
}
void HomeControlSceneEdit::setCancel(const QString &cancel)
{
    m_Cancel = cancel;
    emit sig_Cancel();
}


void HomeControlSceneEdit::Translate()
{
    setCancel(tr("取消"));
    setFinish(tr("完成"));
}

QString HomeControlSceneEdit::isEnable(QString IsEnable)
{
    if(IsEnable == "enable")
    {
        return tr("已激活");
    }
    else
    {
        return tr("未激活");
    }
}
