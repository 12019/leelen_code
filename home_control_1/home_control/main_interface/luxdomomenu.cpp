#include "luxdomomenu.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   EditDataType     dataType;
extern   QDeclarativeItem    *smartHomeItem;
LuxDomoMenu::LuxDomoMenu(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoMenu::~LuxDomoMenu()
{
     g_pMainDlg->m_pHomeControlDlg->menu=NULL;
}
void   LuxDomoMenu::componentComplete()
{
    g_pMainDlg->m_pHomeControlDlg->menu=this;
    MyMsgLog<< g_pMainDlg->m_pHomeControlDlg->menu->objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW/8);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(0);
    parentItem()->setY(0);
    parentItem()->setProperty("passwdDialogW",mLMap_6410_Msg.screenW*7/8);
}

void   LuxDomoMenu::showHomeManageDialog()
{
    parentItem()->setProperty("showPasswd",false);
    if( g_pMainDlg->m_pHomeControlDlg->setting!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->setting->parentItem()->hide();
        g_pMainDlg->m_pHomeControlDlg->setting->CloseItemDialog();
    }

    if(g_pMainDlg->m_pHomeControlDlg->m_pRisco!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->m_pRisco->parentItem()->hide();
        g_pMainDlg->m_pHomeControlDlg->m_pRisco->parentItem()->deleteLater();
    }

    if(g_pMainDlg->m_pHomeControlDlg->m_pHomeManage!=NULL)
    {
         g_pMainDlg->m_pHomeControlDlg->m_pHomeManage->parentItem()->show();
         if(g_pMainDlg->m_pHomeControlDlg->m_pRoomManage!=NULL)
         {
            g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->parentItem()->show();
         }
    }
    else
    {
        g_pMainDlg->m_pHomeControlDlg->CreateHomeManage();
        g_pMainDlg->m_pHomeControlDlg->m_pHomeManage->init();
    }
}

void  LuxDomoMenu::showSecurityDialog()
{
    parentItem()->setProperty("showPasswd",false);
    if( g_pMainDlg->m_pHomeControlDlg->setting!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->setting->parentItem()->hide();
        g_pMainDlg->m_pHomeControlDlg->setting->CloseItemDialog();
    }

    if(g_pMainDlg->m_pHomeControlDlg->m_pHomeManage!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->m_pHomeManage->parentItem()->hide();
        if(g_pMainDlg->m_pHomeControlDlg->m_pRoomManage!=NULL)
        {
           g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->parentItem()->hide();
           if(g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->infraredLearnItem!=NULL)
           {
               g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->infraredLearnItem->deleteLater();
           }
           g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->parentItem()->deleteLater();
        }
        g_pMainDlg->m_pHomeControlDlg->m_pHomeManage->parentItem()->deleteLater();
    }

    if(g_pMainDlg->m_pHomeControlDlg->m_pRisco!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->m_pRisco->parentItem()->show();
    }
    else
    {
        g_pMainDlg->m_pHomeControlDlg->CreateSecurity();
    }
}

void   LuxDomoMenu::showSettingDialog()
{          
     parentItem()->setProperty("showPasswd",false);
    if(g_pMainDlg->m_pHomeControlDlg->m_pHomeManage!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->m_pHomeManage->parentItem()->hide();
        if(g_pMainDlg->m_pHomeControlDlg->m_pRoomManage!=NULL)
        {
           g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->parentItem()->hide();
           if(g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->infraredLearnItem!=NULL)
           {
               g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->infraredLearnItem->deleteLater();
           }
           g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->parentItem()->deleteLater();
        }
        g_pMainDlg->m_pHomeControlDlg->m_pHomeManage->parentItem()->deleteLater();
    }

    if(g_pMainDlg->m_pHomeControlDlg->m_pRisco!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->m_pRisco->parentItem()->hide();
        g_pMainDlg->m_pHomeControlDlg->m_pRisco->parentItem()->deleteLater();
    }

     if( g_pMainDlg->m_pHomeControlDlg->setting!=NULL)
     {
         g_pMainDlg->m_pHomeControlDlg->setting->parentItem()->show();
         g_pMainDlg->m_pHomeControlDlg->setting->chooseSettingOptions(tr("terminal"));
     }
}

void  LuxDomoMenu::showhomepageDialog()
{
    parentItem()->setProperty("showPasswd",false);
    if(g_pMainDlg->m_pHomeControlDlg->m_pHomeManage!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->m_pHomeManage->parentItem()->hide();
        if(g_pMainDlg->m_pHomeControlDlg->m_pRoomManage!=NULL)
        {
           g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->parentItem()->hide();
           if(g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->infraredLearnItem!=NULL)
           {
               g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->infraredLearnItem->deleteLater();
           }
           g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->parentItem()->deleteLater();
        }
        g_pMainDlg->m_pHomeControlDlg->m_pHomeManage->parentItem()->deleteLater();
    }

    if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->roomControl->parentItem()->hide();
        if(g_pMainDlg->m_pHomeControlDlg->roomControl->controlDeviceItem!=NULL)
        {
            g_pMainDlg->m_pHomeControlDlg->roomControl->controlDeviceItem->hide();
            g_pMainDlg->m_pHomeControlDlg->roomControl->controlDeviceItem->setParentItem(smartHomeItem);
            g_pMainDlg->m_pHomeControlDlg->roomControl->controlDeviceItem=NULL;
        }
        g_pMainDlg->m_pHomeControlDlg->roomControl->parentItem()->deleteLater();
    }

    if(g_pMainDlg->m_pHomeControlDlg->m_pRisco!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->m_pRisco->parentItem()->hide();
        g_pMainDlg->m_pHomeControlDlg->m_pRisco->parentItem()->deleteLater();
    }

    if( g_pMainDlg->m_pHomeControlDlg->setting!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->setting->parentItem()->hide();
        g_pMainDlg->m_pHomeControlDlg->setting->CloseItemDialog();
    }
    parentItem()->setProperty("menuCondition",tr("homepage"));
    g_pMainDlg->m_pHomeControlDlg->mainIterface->parentItem()->setY(mLMap_6410_Msg.screenH*2/5);
    g_pMainDlg->m_pHomeControlDlg->mainIterface->parentItem()->setProperty("hintHurdle",false);
}


void  LuxDomoMenu::showInputPasswd()
{
    if( g_pMainDlg->m_pHomeControlDlg->setting!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->setting->parentItem()->hide();
        g_pMainDlg->m_pHomeControlDlg->setting->CloseItemDialog();
    }

    if(g_pMainDlg->m_pHomeControlDlg->m_pRisco!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->m_pRisco->parentItem()->hide();
        g_pMainDlg->m_pHomeControlDlg->m_pRisco->parentItem()->deleteLater();
    }
}

void   LuxDomoMenu::inputPasswd()
{
    dataType=HomeManagePasswd;
    g_pMainDlg->m_pHomeControlDlg->m_editName=tr("");
    g_pMainDlg->m_pHomeControlDlg->setHCLineText(tr(""));
}

void   LuxDomoMenu::checkPasswd(QString  data)
{
    if(data==tr("666666"))
    {
        showHomeManageDialog();
    }
}
