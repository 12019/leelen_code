#include "lightcontroldialog.h"
#include<homecontrolmaindlg.h>

extern QDeclarativeItem *item;

LightControlDialog::LightControlDialog(QObject *parent) :
    QObject(parent)
{
    setLabel(tr("调光灯控制"));
    setStat(tr("关"));
    setValue("50");
    id.clear();
}

LightControlDialog::~LightControlDialog()
{
    //delete m_pLightControlQml;
}

void LightControlDialog::resetState(QDomElement deviceElement)
{
    if(id != deviceElement.attribute("id"))
        return;
    lightElement = deviceElement;
    setStat("");
    setValue("");
    QString value;
    QString status;
    status = lightElement.attribute("power");
    value = lightElement.attribute("value");
    if(status=="off")
        setStat(tr("关"));
    else
        setStat(tr("开"));
    setValue(value);
}

/***********************************************************************************
函数名 :pLightRecive
描  述 : 灯光命令解析
输入参数说明 :
    buf:指令缓冲区
    deviceElement:定位到当期的设备
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void LightControlDialog::pLightRecive(char *buf, QDomElement Element,int len,int deviceId)
{
    //short sub_type=0xff;

    //int attribute = buf[3];   //属性
    //int deviceAdd = buf[1];    //目的地址
    //int deviceId = (attribute<<8) + deviceAdd;
    QString attributeName;
    QString attributeValue;

    QDomElement deviceElement = Element;
    int cmd = buf[4];   //控制指令
    while(!deviceElement.isNull())
    {
        if(deviceElement.attribute("id").toInt() == deviceId)
            break;
        deviceElement=deviceElement.nextSiblingElement("device");
    }
    if(deviceElement.isNull())    //没有找到设备
    {
        MyMsgLog<<"没有找到对应的设备";
        return;
    }
    QString mtype = deviceElement.attribute("type");
    if(deviceElement.attribute("subtype") == "var")
    {
        //sub_type = 1;  //调光灯
        mtype = "var";
    }
    if(len == 6)
    {
        if(cmd==0x00)
        {
            QMetaObject::invokeMethod(item,"hideDevIcon",
                                      Q_ARG(QVariant,mtype),
                                      Q_ARG(QVariant,QString::number(deviceId)));
            attributeName="power";
            attributeValue="off";
        }
        else if(cmd==0x01)
        {
            QMetaObject::invokeMethod(item,"showDevIcon",
                                      Q_ARG(QVariant,mtype),
                                      Q_ARG(QVariant,QString::number(deviceId)));
            attributeName="power";
            attributeValue="on";
        }
    }
    else if(len == 7)   //表示调光灯
    {
        int value = buf[5];   //亮度值
        attributeName = "value";
        attributeValue = QString::number(value);
    }
    if(!attributeName.isEmpty() && !attributeValue.isEmpty())
        deviceElement.setAttribute(attributeName,attributeValue);
    resetState(deviceElement);
}

void LightControlDialog::on_pushButtonBrightness_clicked(int value)
{
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg *)parent();
    QString bPowerOn=lightElement.attribute("power");
    if(bPowerOn=="off")
        return;
    int deviceId=lightElement.attribute("id").toInt();
    char strCmd[7];
    strCmd[0]=0x07;    //字节长度
    strCmd[1]=deviceId&0xff; //目的地址设备号
    strCmd[2]=0x00;  //源地址
    strCmd[3]=deviceId>>8;
    strCmd[4]=0x11;   //指令
    strCmd[5]=value;  //亮度值
    strCmd[6]=0x00;
    pHomeControlDlg->cmdBuffer(strCmd);
}

void LightControlDialog::on_pushButtonPower_click(QString cmd)
{

    if(cmd == tr("开"))
    {
        cmd = "0";
        setStat(tr("关"));
    }
    else
    {
        cmd = "1";
        setStat(tr("开"));
    }

    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg *)parent();
    int deviceId=lightElement.attribute("id").toInt();
    char strCmd[6];
    strCmd[0]=0x06;
    strCmd[1]=deviceId&0xff; //目的地址设备号
    strCmd[2]=0x00;  //源地址
    strCmd[3] = deviceId>>8;
    strCmd[4]=cmd.toInt();   //指令
    strCmd[5]=0x00;
    //pHomeControlDlg->sendHcSerialCmd(strCmd,6);
    //pHomeControlDlg->SerialBuf(strCmd,6);
    pHomeControlDlg->cmdBuffer(strCmd);

    QString bPowerOn=lightElement.attribute("power");
    QString type = lightElement.attribute("type");
    if(lightElement.attribute("subtype") == "var")
        type = "var";
    if(bPowerOn=="off")
    {
        QMetaObject::invokeMethod(item,"showDevIcon",
                                  Q_ARG(QVariant,type),
                                  Q_ARG(QVariant,QString::number(deviceId)));
        lightElement.setAttribute("power","on");
    }
    else if(bPowerOn=="on")
    {
        QMetaObject::invokeMethod(item,"hideDevIcon",
                                  Q_ARG(QVariant,type),
                                  Q_ARG(QVariant,QString::number(deviceId)));
        lightElement.setAttribute("power","off");
    }
}

/************************开关***********************/
QString LightControlDialog::getStat(void) const
{
    return m_Stat;
}

void LightControlDialog::setStat(const QString &stat)
{
    m_Stat=stat;
    emit sendStatChange();
}

/**********************亮度值**********************/
QString LightControlDialog::getValue(void)const
{
    return m_Value;
}

void LightControlDialog::setValue(const QString &value)
{
    m_Value=value;
    emit valueChanged();
}

/*****************************标签名称******************************/
QString LightControlDialog::getLabel(void)const
{
    return m_Label;
}

void LightControlDialog::setLabel(const QString &label)
{
    m_Label = label;
    emit sig_Label();
}

void LightControlDialog::Translate()
{
    setLabel(tr("调光灯控制"));
    setStat(m_Stat);
}
