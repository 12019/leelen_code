#include "luxdomoriscocontrol.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;

LuxDomoRiscoControl::LuxDomoRiscoControl(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoRiscoControl::~LuxDomoRiscoControl()
{

}

void  LuxDomoRiscoControl::componentComplete()
{
    parentItem()->setWidth(mLMap_6410_Msg.screenW*7/8);
    parentItem()->setHeight(mLMap_6410_Msg.screenH*6/8);
    parentItem()->setX(0);
    parentItem()->setY(mLMap_6410_Msg.screenH*2/8);
}

void  LuxDomoRiscoControl::init()
{
    QStringList   data=g_pMainDlg->m_pHomeControlDlg->m_pRiscoSecurityData->GetOutput();
    if(data.size()!=4)
    {
        return;
    }
    QMetaObject::invokeMethod(parentItem(),"clearModel",Qt::DirectConnection);
    QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                                          Q_ARG(QVariant,tr("输出01-zone8")),
                                          Q_ARG(QVariant,data.at(0)==tr("on")?true:false));
    QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                                          Q_ARG(QVariant,tr("输出02-zone8")),
                                          Q_ARG(QVariant,data.at(1)==tr("on")?true:false));
    QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                                          Q_ARG(QVariant,tr("输出03-zone8")),
                                          Q_ARG(QVariant,data.at(2)==tr("on")?true:false));
    QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                                          Q_ARG(QVariant,tr("输出04-zone8")),
                                          Q_ARG(QVariant,data.at(3)==tr("on")?true:false));
}

void  LuxDomoRiscoControl::strCmdSend(int  index, bool  value)
{
    int  cmd=-1;
    if(value)
    {
        cmd=1;
    }
    else
    {
        cmd=0;
    }

    QByteArray m_array;
    m_array.resize(8);
    m_array.data()[0] = 0x08;
    m_array.data()[1] = 0x01;
    m_array.data()[2] = 0xff;
    m_array.data()[3] = 0xbb;
    m_array.data()[4] = 0x05;
    m_array.data()[5] = index+1;
    m_array.data()[6] = cmd;
    m_array.data()[7] = 0x00;
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
}

void   LuxDomoRiscoControl::activateControl(int  index,bool  value)
{
    QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                              Q_ARG(QVariant,index),
                              Q_ARG(QVariant,tr("activate")),
                              Q_ARG(QVariant,value));
    strCmdSend(index,value);
    if(value)
    {
        g_pMainDlg->m_pHomeControlDlg->m_pRiscoSecurityData->SetOutputCondition(index+1,tr("on"));
    }
    else
    {
        g_pMainDlg->m_pHomeControlDlg->m_pRiscoSecurityData->SetOutputCondition(index+1,tr("off"));
    }
}
