#include "tvcontroldialog.h"
#include<homecontrolmaindlg.h>

extern QDeclarativeItem *item;

TVControlDialog::TVControlDialog(QObject *parent) :
    QObject(parent)
{
    tvIsPowerOn=false;
    setMode("TV");
    setStat(tr("关"));
    setVol(tr("音量"));
    setChannel(tr("频道"));
    setLabel(tr("电视控制"));
}

TVControlDialog::~TVControlDialog()
{
    //delete m_pTVControlQml;
}

/***********************************************************************************
函数名 :pAcRecive
描  述 : 灯光命令解析
输入参数说明 :
    buf:指令缓冲区
    deviceElement:定位到当期的设备
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void TVControlDialog::pTVRecive(char *buf, QDomElement Element,QString name)
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
            tvIsPowerOn = true;
        }
        else if(name == "poweroff")
        {
            QMetaObject::invokeMethod(item,"hideDevIcon",
                                      Q_ARG(QVariant,deviceElement.attribute("type")),
                                      Q_ARG(QVariant,QString::number(deviceId)));
            tvIsPowerOn = false;
        }
        attributeName = name.left(5);
        attributeValue = name.right(name.count()-5);
    }
    else if(name == "tv" || name == "av")
    {
        attributeName = "mode";
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

void TVControlDialog::resetState(QDomElement ndeviceElement)
{
    if(id != ndeviceElement.attribute("id"))
        return;
    deviceElement = ndeviceElement;
    QString m_bTVModeTV = deviceElement.attribute("mode", "tv");

    //setMode("");
    //setStat("");

    if(m_bTVModeTV == "tv")
        setMode("TV");
    else
        setMode("AV");

    QString bPowerOn = deviceElement.attribute("power");
    if(bPowerOn == "on")
        setStat(tr("开"));
    else
        setStat(tr("关"));
}

void TVControlDialog::tvstrCmdSend(int cmd)
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

void TVControlDialog::on_pushButtonTVChannel_clicked(QString action)
{
    if(!tvIsPowerOn)
        return;
    QDomElement actionElement = deviceElement.firstChildElement("action");
    int cmd = actionElement.attribute(action).toInt();
    tvstrCmdSend(cmd);
}

void TVControlDialog::on_pushButtonTVIVolume_clicked(QString action)
{
    if(!tvIsPowerOn)
        return;
    QDomElement actionElement = deviceElement.firstChildElement("action");
    int cmd = actionElement.attribute(action).toInt();
    tvstrCmdSend(cmd);
}

void TVControlDialog::on_pushButtonTVMode_clicked()
{
    if(!tvIsPowerOn)
        return;
    QDomElement actionElement = deviceElement.firstChildElement("action");
    if(m_Mode=="TV")
    {
        int cmd = actionElement.attribute("av").toInt();
        tvstrCmdSend(cmd);
        setMode("AV");
        deviceElement.setAttribute("mode","AV");
    }
    else
    {
        int cmd = actionElement.attribute("tv").toInt();
        tvstrCmdSend(cmd);
        setMode("TV");
        deviceElement.setAttribute("mode","TV");
    }
}

void TVControlDialog::on_pushButtonTVPower_clicked(QString action)
{
    if(action == tr("开"))
    {
        action = "poweroff";
        setStat(tr("关"));
    }
    else
    {
        action = "poweron";
        setStat(tr("开"));
    }
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg*)parent();
    QDomElement actionElement = deviceElement.firstChildElement("action");
    int cmd = actionElement.attribute(action).toInt();
    QString deviceId = deviceElement.attribute("id");
    tvstrCmdSend(cmd);

    if(action == "poweroff")
    {
        tvIsPowerOn=false;
        QMetaObject::invokeMethod(item,"hideDevIcon",
                                  Q_ARG(QVariant,"tv"),
                                  Q_ARG(QVariant,deviceId));
        deviceElement.setAttribute("power","off");
    }
    else if(action == "poweron")
    {
        tvIsPowerOn=true;
        QMetaObject::invokeMethod(item,"showDevIcon",
                                  Q_ARG(QVariant,"tv"),
                                  Q_ARG(QVariant,deviceId));
        deviceElement.setAttribute("power","on");
    }
}


/***********************开关***************************/
QString TVControlDialog::getStat(void) const
{
    return m_Stat;
}

void TVControlDialog::setStat(const QString &stat)
{
    m_Stat=stat;
    emit sig_stat();
}

/********************模式******************/
QString TVControlDialog::getMode(void) const
{
    return m_Mode;
}

void TVControlDialog::setMode(const QString &mode)
{
    m_Mode=mode;
    emit sig_mode();
}

/******************频道**********************/
QString TVControlDialog::channel()const
{
    return m_Channel;
}

void TVControlDialog::setChannel(const QString &channel)
{
    m_Channel = channel;
    emit sig_Channel();
}

/******************音量**********************/
QString TVControlDialog::vol()const
{
    return m_Vol;
}

void TVControlDialog::setVol(const QString &vol)
{
    m_Vol = vol;
    emit sig_Vol();
}

/******************标签名称**********************/
QString TVControlDialog::label()const
{
    return m_Label;
}

void TVControlDialog::setLabel(const QString &label)
{
    m_Label = label;
    emit sig_Label();
}


void TVControlDialog::Translate()
{
    setStat(tr("关"));
    setVol(tr("音量"));
    setChannel(tr("频道"));
    setLabel(tr("电视控制"));
}


