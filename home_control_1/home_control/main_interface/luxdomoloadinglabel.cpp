#include "luxdomoloadinglabel.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   QDeclarativeItem    *smartHomeItem;
LuxDomoLoadingLabel::LuxDomoLoadingLabel(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoLoadingLabel::~LuxDomoLoadingLabel()
{
           g_pMainDlg->m_pHomeControlDlg->m_pLoadingLabel=NULL;
}

void  LuxDomoLoadingLabel::componentComplete()
{
          g_pMainDlg->m_pHomeControlDlg->m_pLoadingLabel=this;
          MyMsgLog<<g_pMainDlg->m_pHomeControlDlg->m_pLoadingLabel->objectName();
          parentItem()->setX(mLMap_6410_Msg.screenW/2);
          parentItem()->setY(mLMap_6410_Msg.screenH/2);
          parentItem()->setVisible(false);
}


void  LuxDomoLoadingLabel::ShowLoadingLabel()
{
     if(g_pMainDlg->m_pHomeControlDlg->mainIterface!=NULL)
      {
            parentItem()->setParentItem(g_pMainDlg->m_pHomeControlDlg->mainIterface->parentItem());
            parentItem()->setParentItem(smartHomeItem);
            parentItem()->setVisible(true);
       }
}

void   LuxDomoLoadingLabel::HideLoadingLabel()
{
          parentItem()->setVisible(false);
}
