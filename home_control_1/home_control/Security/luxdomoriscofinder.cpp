#include "luxdomoriscofinder.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;

LuxDomoRiscoFinder::LuxDomoRiscoFinder(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoRiscoFinder::~LuxDomoRiscoFinder()
{

}

void  LuxDomoRiscoFinder::componentComplete()
{
    parentItem()->setWidth(mLMap_6410_Msg.screenW*7/8);
    parentItem()->setHeight(mLMap_6410_Msg.screenH*6/8);
    parentItem()->setX(0);
    parentItem()->setY(mLMap_6410_Msg.screenH*2/8);
    parentItem()->setProperty("childHeight",mLMap_6410_Msg.screenH/8);
    parentItem()->setProperty("screenH",mLMap_6410_Msg.screenH);
}

void  LuxDomoRiscoFinder::setParentH()
{
    int  num=parentItem()->property("showNumber").toInt();
    if(num==0)
    {
        parentItem()->setHeight(mLMap_6410_Msg.screenH*6/8);
    }
    else   if(num>=1&&num<=4)
    {
        parentItem()->setHeight(num*3*mLMap_6410_Msg.screenH/8+mLMap_6410_Msg.screenH/2);
    }
}

void  LuxDomoRiscoFinder::init()
{
    QMetaObject::invokeMethod(parentItem(),"clearModel",Qt::DirectConnection);
    QStringList   data=g_pMainDlg->m_pHomeControlDlg->m_pRiscoSecurityData->GetDefenceArea();
    for(int i=0;i<data.size();i++)
    {
        QStringList  listData=data.at(i).split(tr("*"));
        if(listData.size()!=3)
        {
            continue;
        }
        QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                                              Q_ARG(QVariant,listData.at(0)),
                                              Q_ARG(QVariant,listData.at(1)),
                                              Q_ARG(QVariant,listData.at(2)));
    }
}

void  LuxDomoRiscoFinder::strCmdSend(int  defenceAreaNumber, QString  value)
{
    int  cmd=-1;
    if(value==tr("on"))
    {
        cmd=0x03;
    }
    else   if(value==tr("off"))
    {
        cmd=0x04;
    }
    else
    {
        return;
    }

    QByteArray m_array;
    m_array.resize(7);
    m_array.data()[0] = 0x07;
    m_array.data()[1] = 0x01;
    m_array.data()[2] = 0xff;
    m_array.data()[3] = 0xbb;
    m_array.data()[4] = cmd;
    m_array.data()[5] = defenceAreaNumber;
    m_array.data()[6] = 0x00;
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
}

void  LuxDomoRiscoFinder::bypassDefenceArea(int  index,int  partitionNumber, int  defenceAreaNumber,QString  value)
{
    QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                              Q_ARG(QVariant,index),
                              Q_ARG(QVariant,tr("defenceAreaBypass")),
                              Q_ARG(QVariant,value));
    g_pMainDlg->m_pHomeControlDlg->m_pRiscoSecurityData->DefenceAreaDypass(partitionNumber,defenceAreaNumber,value);
    strCmdSend(defenceAreaNumber,value);
}
