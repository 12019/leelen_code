#include "luxdomocloudsetting.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   EditDataType     dataType;
LuxDomoCloudSetting::LuxDomoCloudSetting(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoCloudSetting::~LuxDomoCloudSetting()
{
     MyMsgLog<<"delete..............."<<parentItem()->objectName();
}

void  LuxDomoCloudSetting::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW*9/16);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(mLMap_6410_Msg.screenW*5/16);
    parentItem()->setY(0);
    QSettings    qsettings("/mnt/disk/config/homecontrol.ini",QSettings::IniFormat);
    parentItem()->setProperty("iD",qsettings.value(LIN_CLOUD_ID).toString());
    if(g_pMainDlg->m_pHomeControlDlg->m_pLINCloudDEVSDK->LinCloudIsValid)
    {
        parentItem()->setProperty("condition",tr("已连接"));
    }
    else
    {
        parentItem()->setProperty("condition",tr("未连接"));
    }
}

void   LuxDomoCloudSetting::editInformation(QString   type, QString  data)
{
    if(type=="CloudID")
     {
          dataType=CloudID;
      }
      g_pMainDlg->m_pHomeControlDlg->m_editName=data;
      g_pMainDlg->m_pHomeControlDlg->setHCLineText(data);
}

void  LuxDomoCloudSetting::setInformation(QString  data)
{
    if(dataType==CloudID)
     {
         parentItem()->setProperty("iD",data);
     }
}

void  LuxDomoCloudSetting::saveInformation()
{
    QSettings    qsettings("/mnt/disk/config/homecontrol.ini",QSettings::IniFormat);
    QString        iD=parentItem()->property("iD").toString();
    qsettings.setValue(LIN_CLOUD_ID,iD);
}
