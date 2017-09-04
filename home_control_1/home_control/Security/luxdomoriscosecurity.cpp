#include "luxdomoriscosecurity.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;

LuxDomoRiscoSecurity::LuxDomoRiscoSecurity(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{  

}

LuxDomoRiscoSecurity::~LuxDomoRiscoSecurity()
{

}

void   LuxDomoRiscoSecurity::componentComplete()
{
    parentItem()->setWidth(mLMap_6410_Msg.screenW*7/8);
    parentItem()->setHeight(mLMap_6410_Msg.screenH*31/40);
    parentItem()->setX(0);
    parentItem()->setY(mLMap_6410_Msg.screenH*9/40);
}

void   LuxDomoRiscoSecurity::init()
{
    QStringList   data=g_pMainDlg->m_pHomeControlDlg->m_pRiscoSecurityData->GetPartition();
    partitions.clear();
    if(data.size()>4||data.size()<=0)
    {
        QMetaObject::invokeMethod(parentItem(),"clearModel",Qt::DirectConnection);
        return;
    }

    for(int i=0;i<data.size();i++)
    {
        QStringList  listData=data.at(i).split(tr("*"));
        if(listData.size()!=2)
        {
            continue;
        }
        int  size=partitions.size();
        if(size==0)
        {
            QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                                  Q_ARG(QVariant,0), Q_ARG(QVariant,tr("partitionNumber")),Q_ARG(QVariant,listData.at(0)));
            QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                                  Q_ARG(QVariant,0),Q_ARG(QVariant,tr("defence")), Q_ARG(QVariant,listData.at(1)));
        }
        else
        {
            QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                                    Q_ARG(QVariant,listData.at(0)),  Q_ARG(QVariant,listData.at(1)));
        }
        partitions.insert(listData.at(0),size);
    }
}

void  LuxDomoRiscoSecurity::strCmdSend(int  partitionNumber, QString  value)
{
    int  cmd=-1;
    if(value==tr("on"))
    {
          cmd=0x01;
    }
    else   if(value==tr("wait"))
    {
         cmd=0x02;
    }
    else   if(value==tr("off"))
    {
         cmd=0x00;
    }
    else
    {
        return;
    }

    QByteArray m_array;
    m_array.resize(8);
    m_array.data()[0] = 0x08;
    m_array.data()[1] = 0x01;
    m_array.data()[2] = 0xff;
    m_array.data()[3] = 0xbb;
    m_array.data()[4] = cmd;
    m_array.data()[5] = partitionNumber;
    m_array.data()[6] = 0x00;
    m_array.data()[7] = 0x00;
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
}

void  LuxDomoRiscoSecurity::changDefence(int  index,int  partitionNumber,QString  value)
{
    QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                           Q_ARG(QVariant,index), Q_ARG(QVariant,tr("defence")), Q_ARG(QVariant,value));
    g_pMainDlg->m_pHomeControlDlg->m_pRiscoSecurityData->PartitionDefence(partitionNumber,value);
    strCmdSend(partitionNumber,value);
}

void    LuxDomoRiscoSecurity::controlAllDefence(QString  value)
{
    QMap<QString,int>::iterator   it=partitions.begin();
    for(;it!=partitions.end();it++)
    {
        QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                              Q_ARG(QVariant,it.value()), Q_ARG(QVariant,tr("defence")), Q_ARG(QVariant,value));
        strCmdSend(it.key().toInt(),value);
        g_pMainDlg->m_pHomeControlDlg->m_pRiscoSecurityData->PartitionDefence(it.key().toInt(),value);
    }
}
