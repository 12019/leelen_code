#include "luxdomorisco.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;

LuxDomoRisco::LuxDomoRisco(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
        showingEngine=NULL;
        showingItem=NULL;
}

LuxDomoRisco::~LuxDomoRisco()
{
    g_pMainDlg->m_pHomeControlDlg->m_pRisco=NULL;
}

void   LuxDomoRisco::componentComplete()
{
    g_pMainDlg->m_pHomeControlDlg->m_pRisco=this;
    MyMsgLog<<g_pMainDlg->m_pHomeControlDlg->m_pRisco->objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW*7/8);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(mLMap_6410_Msg.screenW/8);
    parentItem()->setY(0);
}

void  LuxDomoRisco::CreateElement(QString strFile)
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl(strFile));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem());
    showingItem=item;
    showingEngine=engine;
}

void  LuxDomoRisco::chooseFunction(QString  type)
{
    if(type==condition)
    {
          return;
    }

    if(showingItem!=NULL)
    {
        showingEngine->deleteLater();
        showingEngine=NULL;
        showingItem->deleteLater();
        showingItem=NULL;
    }

    condition=type;
    if(tr("finder")==type)
    {
         CreateElement(tr("qrc:/QML/security_dialog/LuxDomoRiscoFinderDialog.qml"));
    }
    else  if(tr("message")==type)
    {
         CreateElement(tr("qrc:/QML/security_dialog/LuxDomoRiscoHistoryMeaasgeDialog.qml"));
    }
    else  if(tr("control")==type)
    {
         CreateElement(tr("qrc:/QML/security_dialog/LuxDomoRiscoControlDialog.qml"));
    }
    else  if(tr("security")==type)
    {
         CreateElement(tr("qrc:/QML/security_dialog/LuxDomoRiscoSecurityDialog.qml"));
    }
}
