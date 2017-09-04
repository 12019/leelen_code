#include "musiccontroldialog.h"
#include<homecontrolmaindlg.h>

const char *music_source[4] = {"mp3", "cd", "aux", "radio"};
extern QDeclarativeItem *item;

MusicControlDialog::MusicControlDialog(QObject *parent) :
    QObject(parent)
{
    setStat(false);
    setmute(true);
    setSource("mp3");
    setpower(tr("关"));
    setVol(tr("音量"));
    setLabel1(tr("音乐控制"));
    setLabel2(tr("声音模式"));
}

MusicControlDialog::~MusicControlDialog()
{
    //delete m_pMusicControlQml;
}

/***********************************************************************************
函数名 :pMusicRecive
描  述 : 灯光命令解析
输入参数说明 :
    buf:指令缓冲区
    deviceElement:定位到当期的设备
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void MusicControlDialog::pMusicRecive(char *buf, QDomElement Element,QString name)
{
    int attribute = buf[3];   //属性
    int deviceAdd = buf[1];    //目的地址
    int deviceId = (attribute<<8) + deviceAdd;
    QString attributeName;
    QString attributeValue;
    QDomElement deviceElement = Element;
    if(name == "poweron" || name == "poweroff")
    {
        if(name == "poweron")
        {
            QMetaObject::invokeMethod(item,"showDevIcon",
                                      Q_ARG(QVariant,deviceElement.attribute("type")),
                                      Q_ARG(QVariant,QString::number(deviceId)));
            musicIsPowerOn = true;
        }
        else if(name == "poweroff")
        {
            QMetaObject::invokeMethod(item,"hideDevIcon",
                                      Q_ARG(QVariant,deviceElement.attribute("type")),
                                      Q_ARG(QVariant,QString::number(deviceId)));
            musicIsPowerOn = false;
        }
        attributeName = name.left(5);
        attributeValue = name.right(name.count()-5);
    }
    else if(name == "umute")
    {
        attributeName = "mute";
        attributeValue = "off";
    }
    else if(name == "mute")
    {
        attributeName = "mute";
        attributeValue = "on";
    }
    else if(name == "play" || name == "stop")
    {
        attributeName = "play";
        if(name == "play")
            attributeValue = "on";
        else
            attributeValue = "off";
    }
    else if(name == "cd" || name == "aux" || name == "mp3" || name == "radio")
    {
        attributeName = "source";
        attributeValue = name;
    }
    else
    {
        attributeName.clear();
        attributeValue.clear();
    }
    if(!attributeName.isEmpty() && !attributeValue.isEmpty())
        deviceElement.setAttribute(attributeName,attributeValue);
    resetState(deviceElement);
}

void MusicControlDialog::resetState(QDomElement ndeviceElement)
{
    if(id != ndeviceElement.attribute("id"))
        return;
    deviceElement = ndeviceElement;
    QString strSource = deviceElement.attribute("source");
    qDebug()<<"source:"<<strSource;

    setSource(strSource);

    bool bPlay = deviceElement.attribute("play","on") == "on";
    if(bPlay)
        setStat(true);
    else
        setStat(false);
    qDebug()<<"bPlay:"<<bPlay;

    bool bmute = deviceElement.attribute("mute","on") == "on";
    if(bmute)
        setmute(true);
    else
        setmute(false);
    qDebug()<<"bmute:"<<bmute;

    bool bPowerOn = deviceElement.attribute("power","on") == "on";
    if(bPowerOn)
        setpower(tr("开"));
    else
        setpower(tr("关"));
}

void MusicControlDialog::musicstrCmdSend(int cmd)
{
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg *)parent();
    int deviceId=deviceElement.attribute("id").toInt();

    char strCmd[8];
    strCmd[0]=0x08;    //字节长度
    strCmd[1]=deviceId&0xff; //目的地址设备号
    strCmd[2]=0x00;  //源地址
    strCmd[3]=0x0D;   //设备属性
    strCmd[4]=0x20;
    strCmd[5]=cmd;
    strCmd[6]=0x01;
    strCmd[7]=0;
    //pHomeControlDlg->sendHcSerialCmd(strCmd,8);
    pHomeControlDlg->cmdBuffer(strCmd);
    //pHomeControlDlg->SerialBuf(strCmd,8);
}

void MusicControlDialog::on_pushButtonBMPause_clicked()
{
    if(!musicIsPowerOn)
        return;

    QDomElement actionElement = deviceElement.firstChildElement("action");
    int cmd = actionElement.attribute("play").toInt();
    musicstrCmdSend(cmd);
    deviceElement.setAttribute("play","off");
}

void MusicControlDialog::on_pushButtonBMPlay_clicked()
{
    if(!musicIsPowerOn)
        return;

    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg*)parent();
    QDomElement actionElement = deviceElement.firstChildElement("action");
    int cmd = actionElement.attribute("stop").toInt();
    musicstrCmdSend(cmd);
    deviceElement.setAttribute("play","on");
}

void MusicControlDialog::on_pushButtonBMMute_clicked()
{
    if(!musicIsPowerOn)
        return;
    QDomElement actionElement = deviceElement.firstChildElement("action");
    int cmd = actionElement.attribute("mute").toInt();
    musicstrCmdSend(cmd);
    deviceElement.setAttribute("mute","on");
}

void MusicControlDialog::on_pushButtonBMUnmute_clicked()
{
    if(!musicIsPowerOn)
        return;
    QDomElement actionElement = deviceElement.firstChildElement("action");
    int cmd = actionElement.attribute("umute").toInt();
    musicstrCmdSend(cmd);
    deviceElement.setAttribute("mute","off");
}

void MusicControlDialog::on_pushButtonBMNextMusic_clicked()
{
    if(!musicIsPowerOn)
        return;
    QDomElement actionElement = deviceElement.firstChildElement("action");
    int cmd = actionElement.attribute("next").toInt();
    musicstrCmdSend(cmd);
}


void MusicControlDialog::on_pushButtonBMPrevMusic_clicked()
{
    if(!musicIsPowerOn)
        return;
    QDomElement actionElement = deviceElement.firstChildElement("action");
    int cmd = actionElement.attribute("pre").toInt();
    musicstrCmdSend(cmd);
}

void MusicControlDialog::on_pushButtonBMSource_clicked(QString action) //资源
{
    if(!musicIsPowerOn)
        return;
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
                break;
            }
        }
    }
    QDomElement actionElement = deviceElement.firstChildElement("action");
    int cmd = actionElement.attribute(music_source[nMusic]).toInt();
    musicstrCmdSend(cmd);
    deviceElement.setAttribute("source",music_source[nMusic]);
}

void MusicControlDialog::on_pushButtonBMPower_clicked(QString action)
{
    //HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg*)parent();


    QString deviceId = deviceElement.attribute("id");
    if(Power == tr("关"))
        action = "poweron";
    else
        action = "poweroff";
    if(action == "poweron")
    {
        QMetaObject::invokeMethod(item,"showDevIcon",
                                  Q_ARG(QVariant,"music"),
                                  Q_ARG(QVariant,deviceId));
        musicIsPowerOn=true;
        deviceElement.setAttribute("power","on");
        setpower(tr("开"));
    }
    else if(action == "poweroff")
    {
        QMetaObject::invokeMethod(item,"hideDevIcon",
                                  Q_ARG(QVariant,"music"),
                                  Q_ARG(QVariant,deviceId));
        musicIsPowerOn=false;
        deviceElement.setAttribute("power","off");
        setpower(tr("关"));
    }
    QDomElement actionElement = deviceElement.firstChildElement("action");
    int cmd = actionElement.attribute(action).toInt();
    musicstrCmdSend(cmd);
}

void MusicControlDialog::on_pushButtonBMVolume_clicked(QString action)
{
    if(!musicIsPowerOn)
        return;
    QDomElement actionElement = deviceElement.firstChildElement("action");
    int cmd = actionElement.attribute(action).toInt();
    musicstrCmdSend(cmd);
}

/******************静音非静音**********************/
bool MusicControlDialog::getmute(void)const
{
    return m_Mute;
}

void MusicControlDialog::setmute(const bool &stat)
{
    m_Mute = stat;
    emit sig_mute();
}

/*********************播放暂停*********************/
bool MusicControlDialog::getStat(void)const
{
    return musicIsPlay;
}

void MusicControlDialog::setStat(const bool &stat)
{
    musicIsPlay=stat;
    emit sig_stat();
}


/***********************资源**************************/
QString MusicControlDialog::getSource(void)const
{
    return m_Source;
}

void MusicControlDialog::setSource(const QString &bsource)
{
    m_Source=bsource;
    emit sig_source();
}


/************************开关************************/
QString MusicControlDialog::getpower(void)const
{
    return Power;
}

void MusicControlDialog::setpower(const QString &bsource)
{
    Power = bsource;
    emit sig_power();
}


/*************************音量**************************/
QString MusicControlDialog::vol()const
{
    return m_Vol;
}

void MusicControlDialog::setVol(const QString &vol)
{
    m_Vol = vol;
    emit sig_Vol();
}

/*************************音乐控制**************************/
QString MusicControlDialog::label1()const
{
    return m_Label1;
}

void MusicControlDialog::setLabel1(const QString &label1)
{
    m_Label1 = label1;
    emit sig_Label1();
}

/*************************声音模式**************************/
QString MusicControlDialog::label2()const
{
    return m_Label2;
}

void MusicControlDialog::setLabel2(const QString &label2)
{
    m_Label2 = label2;
    emit sig_Label2();
}

void MusicControlDialog::Translate()
{
    setpower(tr("关"));
    setVol(tr("音量"));
    setLabel1(tr("音乐控制"));
    setLabel2(tr("声音模式"));
}

bool MusicControlDialog::musicPowerStat()
{
    return musicIsPowerOn;
}
