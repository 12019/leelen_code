#include "luxdomoriscohistorymeaasge.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;

LuxDomoRiscoHistoryMeaasge::LuxDomoRiscoHistoryMeaasge(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoRiscoHistoryMeaasge::~LuxDomoRiscoHistoryMeaasge()
{

}

void  LuxDomoRiscoHistoryMeaasge::componentComplete()
{
    parentItem()->setWidth(mLMap_6410_Msg.screenW*7/8);
    parentItem()->setHeight(mLMap_6410_Msg.screenH*6/8);
    parentItem()->setX(0);
    parentItem()->setY(mLMap_6410_Msg.screenH*2/8);
}

void  LuxDomoRiscoHistoryMeaasge::init()
{  
    showEvent(tr("AllEvent"));
}

void   LuxDomoRiscoHistoryMeaasge::showEvent(QString  type)
{
    QMetaObject::invokeMethod(parentItem(),"clearModel",Qt::DirectConnection);
    QStringList     data=g_pMainDlg->m_pHomeControlDlg->m_pRiscoSecurityData->GetEvent(type);
    for(int i=0;i<data.size();i++)
    {
        QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,Q_ARG(QVariant,data.at(i)));
    }
}
