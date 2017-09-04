#include "irlearndialog.h"
#include<irpasswd.h>
#include<QFile>
#include<QTextCodec>
#include<homecontrolmaindlg.h>

extern QDeclarativeItem *item;

IrLearnDialog::IrLearnDialog(QObject *parent) :
    QObject(parent)
{
    deviceId = "";

}

/***************退出红外学习****************/
void IrLearnDialog::closeLearnStat()
{
    if(deviceId.isEmpty())
        return;
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg*)parent();
    char strCmd[6];
    strCmd[0] = 0x06;
    strCmd[1] = deviceId.toInt() & 0xff;  //设备号
    strCmd[2] = 0x00;    //源地址
    strCmd[3] = 0x0d;
    strCmd[4] = 0x00;
    strCmd[5] = 0x00;  //校验和
    pHomeControlDlg->cmdBuffer(strCmd);
    deviceId = "";
}

void IrLearnDialog::close()
{
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg*)parent();
    closeLearnStat();
    pHomeControlDlg->setMainState("");
}

void IrLearnDialog::irTypeChose(QString type,QString Id)
{
    deviceType = type;
    deviceId = Id;
    QSettings settings("/mnt/disk/config/config", QSettings::IniFormat);
    QString strLang = settings.value("language/lang", "zh_CN").toString();


    if(type == "tv")
    {
        loadIrAction(":/Ir/"+strLang+"TV.txt");
    }
    else if(type == "music")
    {
        loadIrAction(":/Ir/"+strLang+"MUSIC.txt");
    }
    else if(type == "ac")
    {
        loadIrAction(":/Ir/"+strLang+"AIR.txt");
    }
}

void IrLearnDialog::loadIrAction(QString fileName)
{
    QStringList irAction;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly) )
    {
        QTextStream stream(&file);
        stream.setCodec(QTextCodec::codecForName("GB18030"));
        irAction=stream.readAll().split(" ");
    }
    file.close();
    for(int i=0;i<irAction.count();i++)
    {
        QMetaObject::invokeMethod(item,"irLearnEvent",
                                  Q_ARG(QVariant, "irAction"),
                                  Q_ARG(QVariant, irAction.at(i)),
                                  Q_ARG(QVariant, ""));
    }
}

void IrLearnDialog::irLearn(int indx)
{
   qDebug()<<"ir indx="<<indx<<deviceType;
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg*)parent();
    char strCmd[7];
    strCmd[0] = 0x07;
    strCmd[1] = deviceId.toInt() & 0xff;  //设备号
    strCmd[2] = 0x00;    //源地址
    strCmd[3] = 0x0d;
    strCmd[4] = 0x21;
    strCmd[5] = 0; //指令
    strCmd[6] = 0x00;  //校验和

    QDomElement deviceElement = pHomeControlDlg->m_deviceElement;
    QDomElement actionElement;
    while(!deviceElement.isNull())
    {
        if(deviceElement.attribute("id") == deviceId && deviceElement.attribute("type") == deviceType)
        {
            pHomeControlDlg->deviceElement = deviceElement;
            actionElement = deviceElement.firstChildElement("action");
            break;
        }
        deviceElement = deviceElement.nextSiblingElement("device");
    }


    if(deviceType == "ac")
    {
        QStringList IrLearnAction;
        QFile file(":/Ir/enAIR.txt");
        if (file.open(QIODevice::ReadOnly) )
        {
            QTextStream stream(&file);
            stream.setCodec(QTextCodec::codecForName("GB18030"));
            IrLearnAction=stream.readAll().split(" ");
        }
        file.close();
        int cmd = actionElement.attribute(IrLearnAction.at(indx)).toInt();
        strCmd[5] = cmd;
        //qDebug()<<IrLearnAction<<cmd<<IrLearnAction.at(indx);
    }
    else if(deviceType == "tv")
    {
        QStringList IrLearnAction;
        QFile file(":/Ir/enTV.txt");
        if (file.open(QIODevice::ReadOnly) )
        {
            QTextStream stream(&file);
            stream.setCodec(QTextCodec::codecForName("GB18030"));
            IrLearnAction=stream.readAll().split(" ");
        }
        file.close();
        int cmd = actionElement.attribute(IrLearnAction.at(indx)).toInt();
        strCmd[5] = cmd;
    }
    else if(deviceType == "music")
    {
        QStringList IrLearnAction;
        QFile file(":/Ir/enMUSIC.txt");
        if (file.open(QIODevice::ReadOnly) )
        {
            QTextStream stream(&file);
            stream.setCodec(QTextCodec::codecForName("GB18030"));
            IrLearnAction=stream.readAll().split(" ");
        }
        file.close();
        int cmd = actionElement.attribute(IrLearnAction.at(indx)).toInt();
        strCmd[5] = cmd;
    }
    pHomeControlDlg->cmdBuffer(strCmd);
}


int IrLearnDialog::getirIndx() const
{
    return m_irIndx;
}

void IrLearnDialog::setirIndx(const int &irIndx)
{
    m_irIndx = irIndx;
    emit sig_irIndx();
}


QString IrLearnDialog::toast()
{
    return tr("请按遥控器上的按键进行学习");
}
