#include "accontroldialog.h"
#include<homecontrolmaindlg.h>
#include<QStringList>
#include<MyBuffer.h>

const char *ac_mode[5] = {"automode","refri","dehum", "venti","heat"};

const char *ac_wind[4] = {"windauto", "low", "medium", "high"};

const char *central_wind_speed_en[6] = {"windauto", "ultra-low","low","medium", "high","ultra-high"};

const char *central_wind_direction_en[6] = {"P0", "P1", "P2", "P3","P4","on"};

const char *ac_mode_display[5] = {
                                      QT_TRANSLATE_NOOP("ac", "自动"),
                                      QT_TRANSLATE_NOOP("ac", "制冷"),
                                      QT_TRANSLATE_NOOP("ac", "除湿"),
                                      QT_TRANSLATE_NOOP("ac", "通风"),
                                      QT_TRANSLATE_NOOP("ac", "加热")
                                      };

const char *ac_wind_display[4] = {
                                         QT_TRANSLATE_NOOP("ac", "自动"),
                                         QT_TRANSLATE_NOOP("ac", "低风"),
                                         QT_TRANSLATE_NOOP("ac", "中风"),
                                         QT_TRANSLATE_NOOP("ac", "高风")
                                         };

const char *central_wind_speed[6] = {
    QT_TRANSLATE_NOOP("ac", "无限定"),
    QT_TRANSLATE_NOOP("ac", "超低速"),
    QT_TRANSLATE_NOOP("ac", "低速"),
    QT_TRANSLATE_NOOP("ac", "中速"),
    QT_TRANSLATE_NOOP("ac", "高速"),
    QT_TRANSLATE_NOOP("ac", "超高速")
};

const char *central_wind_direction[6] = {
    QT_TRANSLATE_NOOP("ac", "P0"),
    QT_TRANSLATE_NOOP("ac", "P1"),
    QT_TRANSLATE_NOOP("ac", "P2"),
    QT_TRANSLATE_NOOP("ac", "P3"),
    QT_TRANSLATE_NOOP("ac", "P4"),
    QT_TRANSLATE_NOOP("ac", "摆风")
};

QString global_ac_mode_display(short n)
{
    return qApp->translate( "ac",ac_mode_display[n] );
}

QString global_ac_wind_display(short n)
{
    return qApp->translate( "ac",ac_wind_display[n] );
}

QString global_central_wind_speed_display(short n)
{
    return qApp->translate( "ac",central_wind_speed[n] );
}

QString global_central_wind_direction_display(short n)
{
    return qApp->translate( "ac",central_wind_direction[n] );
}

extern QDeclarativeItem *item;


AccontrolDialog::AccontrolDialog(QObject *parent) :
    QObject(parent)
{
    setMode(tr("制冷"));
    setStat(tr("关"));
    setTemp(tr("16"));
    setWind(tr("自动"));
    setSwing(tr("摆风"));
    setLabel(tr("空调控制"));
    isPowerOn = false;
}

AccontrolDialog::~AccontrolDialog()
{
    //delete m_pAccontrolQml;
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
void AccontrolDialog::pAcRecive(char *buf, QDomElement Element, QString name)
{
    int attribute = buf[3];   //属性
    int deviceAdd = buf[1];    //目的地址
    int deviceId = (attribute<<8) + deviceAdd;
    QString attributeName;
    QString attributeValue;
    QDomElement ndeviceElement = Element;
    int pIndex;
    if(attribute == General_Infrared)   //表示普通红外
    {
        if(name == "poweron" || name == "poweroff" || name == "swingon" || name == "swingoff")
        {
            if(name == "poweron")
            {
                QMetaObject::invokeMethod(item,"showDevIcon",
                                          Q_ARG(QVariant,ndeviceElement.attribute("type")),
                                          Q_ARG(QVariant,QString::number(deviceId)));
                isPowerOn = true;
            }
            else if(name == "poweroff")
            {
                QMetaObject::invokeMethod(item,"hideDevIcon",
                                          Q_ARG(QVariant,ndeviceElement.attribute("type")),
                                          Q_ARG(QVariant,QString::number(deviceId)));
                isPowerOn = false;
            }
            attributeName = name.left(5);
            attributeValue = name.right(name.count()-5);
        }
        else if(name.left(1) == "t")
        {
            attributeName = "temperature";
            attributeValue = name.right(2);
        }
        else if(name == "windauto" || name == "low" || name == "high" || name == "medium")
        {
            attributeName = "wind";
            attributeValue = name;
        }
        else if(name == "refri" || name == "heat" || name == "venti" ||name == "dehum"||name == "automode")
        {
            attributeName = "mode";
            if(name == "automode")
                attributeValue = "auto";
            else
                attributeValue = name;
            //attributeValue = name;
        }
    }
    else if(attribute == Central_AC) //中央空调
    {
        int function = buf[4];
        MyMsgLog<<"TTTTTTTTTTTTTTTTTTTTT"<<function;
        switch (function)
        {
        case 0:       //中央空调关
            isPowerOn = false;
            attributeName = "power";
            attributeValue = "off";
            break;
        case 1:    //
            isPowerOn = true;
            attributeName = "power";
            attributeValue = "on";
            break;
        case 2:  //中央空调温度
            pIndex = buf[5];
            attributeName = "temperature";
            attributeValue = QString::number(pIndex);
            break;
        case 3: //中央空调模式
            pIndex = buf[5];
            attributeName = "mode";
            attributeValue = ac_mode[pIndex];
            break;
        case 4: //中央空调风速
            pIndex = buf[5];
            attributeName = "wind";
            attributeValue = central_wind_speed_en[pIndex];
            break;
        case 5:  //风向
            pIndex = buf[5];
            attributeName = "swing";
            attributeValue = central_wind_direction_en[pIndex];
            break;
        default:
            break;
        }
    }
    MyMsgLog<<attributeName<<attributeValue;
    if(!attributeName.isEmpty() && !attributeValue.isEmpty())
        ndeviceElement.setAttribute(attributeName,attributeValue);
    resetState(ndeviceElement);
}

void AccontrolDialog::resetState(QDomElement ndeviceElement)
{
    MyMsgLog<<id<<ndeviceElement.attribute("id");
    if(id != ndeviceElement.attribute("id"))
        return;
    attr = (id.toInt()>>8) & 0xff;
    deviceElement = ndeviceElement;

     /*****************************开关*****************************/
    QString bPowerOn = deviceElement.attribute("power");
    if(bPowerOn=="on")     //电源
    {
        setStat(tr("开"));
        isPowerOn=true;
    }
    else
    {
        setStat(tr("关"));
        isPowerOn=false;
    }

    /***************************摆风，固定*****************************/
    QString strSwing = deviceElement.attribute("swing");   //摆风，固定
    if(attr == Central_AC)             //中央空调
    {
        if (strSwing == "on"|| strSwing == "")
        {
            setSwing(tr("摆风"));
        }
        else if(strSwing == "off")
        {
            deviceElement.setAttribute("swing","P0");
            setSwing("P0");
        }
        else
            setSwing(strSwing);
    }
    else if(attr == General_Infrared)   //普通空调
    {
        if (strSwing == "off")
        {
            setSwing(tr("固定"));
        }
        else
        {
            setSwing(tr("摆风"));
        }
    }

    /*****************************模式*******************************/
    QString strMode = deviceElement.attribute("mode", ac_mode[0]);
    if(strMode == "auto"){
        strMode = "automode";
    }
    short m_nMode=0;
    for (int i = 0; i < 5; i++)
    {
        if (strMode == ac_mode[i])
        {
            m_nMode = i;
            break;
        }
    }
    setMode(global_ac_mode_display(m_nMode));
    MyMsgLog<<"TTTTTTTTTTTTTTT"<<m_nMode<<strMode;


    /*****************************风速*********************************/
    QString strWind = deviceElement.attribute("wind", ac_wind[0]);
    if(strWind == "auto" || strWind == "")
    {
        strWind = "windauto";
    }
    if(attr == General_Infrared)
    {
        short m_nWind = 0;
        for (int i = 0; i < 4; i++)
        {
            if (strWind == ac_wind[i])
            {
                m_nWind = i;
                break;
            }
        }
        setWind(global_ac_wind_display(m_nWind));
    }
    else if(attr == Central_AC)
    {
        short m_nWind = 0;
        for (int i = 0; i < 6; i++)
        {
            if (strWind == central_wind_speed_en[i])
            {
                m_nWind = i;
                break;
            }
        }
        MyMsgLog<<m_nWind<<global_central_wind_speed_display(m_nWind)<<strWind;
        setWind(global_central_wind_speed_display(m_nWind));
    }


    /****************************温度*********************************/
    QString tempture = deviceElement.attribute("temperature");
    setTemp(tempture);
}

void AccontrolDialog::acstrCmdSend(int cmd)
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
    //pHomeControlDlg->SerialBuf(strCmd,8);
    pHomeControlDlg->cmdBuffer(strCmd);
}

void AccontrolDialog::expendCmd(int len, int function, int cmd)
{
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg *)parent();
    int deviceId=deviceElement.attribute("id").toInt();
    char strCmd[len];
    memset(strCmd,0,len);
    strCmd[0]=len;    //字节长度
    strCmd[1]=deviceId&0xff; //目的地址设备号
    strCmd[2]=0x00;  //源地址
    strCmd[3]=(deviceId >> 8)&0xff; //0x0D;   //设备属性
    if(function > 0)
    {
        strCmd[4] = function;
        strCmd[5] = cmd;
    }
    else
    {
        strCmd[4] = cmd;
    }
    pHomeControlDlg->cmdBuffer(strCmd);
    //pHomeControlDlg->sendHcSerialCmd(strCmd,len);
}

/*************************************************************************
 函数名：on_pushButtonACPower_clicked
 说明：空调的开关
 参数：action表示当前的状态值
 * ************************************************************************/
void AccontrolDialog::on_pushButtonACPower_clicked(QString action)
{
    //QObject *Item = ApplicationItem->findChild<QObject*>("mainMap");
    QString deviceId = deviceElement.attribute("id");



    if(attr == General_Infrared)
    {
        if(action == tr("开"))
        {
            action = "poweroff";
            setStat(tr("关"));
            isPowerOn = false;
            QMetaObject::invokeMethod(item,"hideDevIcon",
                                      Q_ARG(QVariant,"ac"),
                                      Q_ARG(QVariant,deviceId));
            deviceElement.setAttribute("power","off");
        }
        else
        {
            action = "poweron";
            setStat(tr("开"));

            isPowerOn = true;
            QMetaObject::invokeMethod(item,"showDevIcon",
                                      Q_ARG(QVariant,"ac"),
                                      Q_ARG(QVariant,deviceId));
            deviceElement.setAttribute("power","on");
        }
        int cmd = deviceElement.firstChildElement("action").attribute(action).toInt();
        acstrCmdSend(cmd);
    }
    else if(attr == Central_AC)
    {
        if(action == tr("开"))
        {
            action = "poweroff";
            setStat(tr("关"));
            isPowerOn = false;
            QMetaObject::invokeMethod(item,"hideDevIcon",
                                      Q_ARG(QVariant,"custom"),
                                      Q_ARG(QVariant,deviceId));
            deviceElement.setAttribute("power","off");
        }
        else
        {
            action = "poweron";
            setStat(tr("开"));

            isPowerOn = true;
            QMetaObject::invokeMethod(item,"showDevIcon",
                                      Q_ARG(QVariant,"custom"),
                                      Q_ARG(QVariant,deviceId));
            deviceElement.setAttribute("power","on");
        }
        if(action == "poweroff")
            expendCmd(0x06,-1,0x00);
        else
            expendCmd(0x06,-1,0x01);
    }
}

/*************************************************************************
 函数名：on_pushButtonACSwing_clicked
 说明：风向，摆风、固定、P0等
 参数：action表示当前的状态值
 * ************************************************************************/
bool AccontrolDialog::on_pushButtonACSwing_clicked(QString action)
{
    if(!isPowerOn)
        return false;

    if(attr == General_Infrared)
    {
        action = m_Swing;
        if(action == tr("固定"))
        {
            action = "swingon";
            deviceElement.setAttribute("swing","on");
            setSwing(tr("摆风"));
        }
        else
        {
            action = "swingoff";
            deviceElement.setAttribute("swing","off");
            setSwing(tr("固定"));
        }
        int cmd = deviceElement.firstChildElement("action").attribute(action).toInt();
        acstrCmdSend(cmd);
    }
    else if(attr == Central_AC)   //中央空调
    {
        short nMode = 0;
        if(action=="next")
        {
            for(int i=0;i<6;i++)
            {
                if(m_Swing == global_central_wind_direction_display(i))
                {
                    nMode=i + 1;
                    if(nMode >= 6)
                        nMode = 0;;
                    //setSwing(global_central_wind_direction_display(nMode));
                    break;
                }
            }
        }
        else if(action=="pre")
        {
            for(int i=0;i<6;i++)
            {
                if(m_Swing==global_central_wind_direction_display(i))
                {
                    nMode=i;
                    nMode=nMode-1;
                    if(nMode<0)
                        nMode=5;
                    //setSwing(global_central_wind_direction_display(nMode));
                    break;
                }
            }
        }
        MyMsgLog<<central_wind_direction_en[nMode]<<nMode;
        setSwing(global_central_wind_direction_display(nMode));
        deviceElement.setAttribute("swing",central_wind_direction_en[nMode]);
        expendCmd(0x07,0x05,nMode);
    }
    return true;
}

void AccontrolDialog::on_pushButtonACMode_clicked(QString action)
{
    if(!isPowerOn)
        return;
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
                break;
            }
        }
    }
    if(attr == General_Infrared)
    {
        QDomElement actionElement = deviceElement.firstChildElement("action");
        int cmd = actionElement.attribute(ac_mode[nMode]).toInt();
        acstrCmdSend(cmd);
        //deviceElement.setAttribute("mode",ac_mode[nMode]);
    }
    else if(attr == Central_AC)
    {
        expendCmd(0x07,0x03,nMode);
        //deviceElement.setAttribute("mode",ac_mode[nMode]);
    }
    deviceElement.setAttribute("mode",ac_mode[nMode]);
}

void AccontrolDialog::on_pushButtonACWind_clicked(QString action)
{
    if(!isPowerOn)
        return;

    short nWind;
    if(attr == General_Infrared)
    {
        if(action=="next")
        {
            for(int i=0;i<4;i++)
            {
                if(m_Wind==global_ac_wind_display(i))
                {
                    nWind=i;
                    nWind=(nWind+1)%4;
                    setWind(global_ac_wind_display(nWind));
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
                    break;
                }
            }
        }
        QDomElement actionElement = deviceElement.firstChildElement("action");
        int cmd = actionElement.attribute(ac_wind[nWind]).toInt();
        acstrCmdSend(cmd);
        deviceElement.setAttribute("wind",ac_wind[nWind]);
    }
    else if(attr == Central_AC)
    {
        int nWind = 0;
        if(action=="next")
        {
            for(int i=0;i<6;i++)
            {
                if(m_Wind==global_central_wind_speed_display(i))
                {
                    nWind=i + 1;
                    if(nWind >= 6)
                        nWind = 0;
                    //setWind(global_central_wind_speed_display(nWind));
                    break;
                }
            }
        }
        else if(action=="pre")
        {
            for(int i=0;i<6;i++)
            {
                if(m_Wind==global_central_wind_speed_display(i))
                {
                    nWind=i;
                    nWind=nWind-1;
                    if(nWind<0)
                        nWind=5;
                    //setWind(global_central_wind_speed_display(nWind));
                    break;
                }
            }
        }
        setWind(global_central_wind_speed_display(nWind));
        expendCmd(0x07,0x04,nWind);
        deviceElement.setAttribute("wind",central_wind_speed_en[nWind]);
    }
}

void AccontrolDialog::on_pushButtonACTemperature_clicked(QString action)
{
    if(!isPowerOn)
        return;

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
    QDomElement actionElement = deviceElement.firstChildElement("action");
    int cmd = actionElement.attribute("t"+QString::number(nTemp)).toInt();
    deviceElement.setAttribute("temperature",QString::number(nTemp));
    if(attr == General_Infrared)
    {
        acstrCmdSend(cmd);
    }
    else if(attr == Central_AC)
    {
        expendCmd(0x07,0x02,nTemp);
    }
}




/***************************状态值****************************/
QString AccontrolDialog::getStat(void) const  //把值传回qml
{
    return m_Stat;
}

void AccontrolDialog::setStat(const QString &stat)
{
    m_Stat=stat;
    emit sig_stat();
}


/**********************模式*************************/
QString AccontrolDialog::getMode(void) const
{
    return m_Mode;
}

void AccontrolDialog::setMode(const QString &mode)
{
    m_Mode=mode;
    emit sig_mode();
}


/**********************温度*************************/
QString AccontrolDialog::getTemp(void) const
{
    return m_Tempture;
}

void AccontrolDialog::setTemp(const QString &temp)
{
    m_Tempture=temp;
    emit sig_temp();
}

/*******************摆风*******************/
QString AccontrolDialog::getSwing(void) const
{
    return m_Swing;
}

void AccontrolDialog::setSwing(const QString &swing)
{
    m_Swing=swing;
    emit sig_swing();
}


/******************风速**********************/
QString AccontrolDialog::getWind(void) const
{
    return m_Wind;
}

void AccontrolDialog::setWind(const QString &wind)
{
    m_Wind=wind;
    emit sig_wind();
}


/*******************标签名称设置********************/
QString AccontrolDialog::getLabel()const
{
    return m_Label;
}

void AccontrolDialog::setLabel(const QString &label)
{
    m_Label = label;
    emit sig_Label();
}


/*************************翻译国际化**********************/
void AccontrolDialog::Translate()
{
    setLabel(tr("空调控制"));
    setMode(m_Mode);
    setStat(m_Stat);
    setWind(m_Wind);
    setSwing(m_Swing);
}




























