#include "curtaincontroldialog.h"
#include<homecontrolmaindlg.h>
#include<QStringList>

extern QDeclarativeItem *item;

CurtainControlDialog::CurtainControlDialog(QObject *parent) :
    QObject(parent)
{
    setLabel(tr("窗帘控制"));
}

CurtainControlDialog::~CurtainControlDialog()
{
    //delete m_pCurtainControlQml;
}

void CurtainControlDialog::CurtainstrCmdSend(int cmd)
{
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg *)parent();
    int deviceId=deviceElement.attribute("id").toInt();
    char strCmd[6];
    strCmd[0]=0x06;    //字节长度
    strCmd[1]=deviceId&0xff; //目的地址设备号
    strCmd[2]=0x00;  //源地址
    strCmd[3]=deviceId>>8;   //设备属性
    strCmd[4]=cmd;
    strCmd[5]=0;
    pHomeControlDlg->cmdBuffer(strCmd);

    if(cmd==0)
    {
        QMetaObject::invokeMethod(item,"hideDevIcon",
                                  Q_ARG(QVariant,"curtain"),
                                  Q_ARG(QVariant,QString::number(deviceId)));
        deviceElement.setAttribute("power","off");
    }
    else if(cmd==1)
    {
        QMetaObject::invokeMethod(item,"showDevIcon",
                                  Q_ARG(QVariant,"curtain"),
                                  Q_ARG(QVariant,QString::number(deviceId)));
        deviceElement.setAttribute("power","on");
    }
}

void CurtainControlDialog::on_pushButtonCurtainClose_clicked()
{
    CurtainstrCmdSend(0);
}

void CurtainControlDialog::on_pushButtonCurtainOpen_clicked()
{
    CurtainstrCmdSend(1);
}

void CurtainControlDialog::on_pushButtonCurtainStop_clicked()
{
    CurtainstrCmdSend(2);
}


/**************************标签名称设置***************************/
QString CurtainControlDialog::getLabel()const
{
    return m_Label;
}

void CurtainControlDialog::setLabel(const QString &label)
{
    m_Label = label;
    emit sig_Label();
}


void CurtainControlDialog::Translate()
{
    setLabel(tr("窗帘控制"));
}










