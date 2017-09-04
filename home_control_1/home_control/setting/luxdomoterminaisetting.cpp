#include "luxdomoterminaisetting.h"
#include"maindialog.h"
#include"MyBuffer.h"
#include<QHostInfo>
#include<QNetworkInterface>
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   EditDataType     dataType;

LuxDomoTerminaiSetting::LuxDomoTerminaiSetting(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoTerminaiSetting::~LuxDomoTerminaiSetting()
{
    MyMsgLog<<"delete..............."<<parentItem()->objectName();
}

void LuxDomoTerminaiSetting::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW*9/16);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(mLMap_6410_Msg.screenW*5/16);
    parentItem()->setY(0);

    QSettings    qsettings("/mnt/disk/config/homecontrol.ini",QSettings::IniFormat);
    parentItem()->setProperty("terminaiPasswd",qsettings.value(Ter_Pwd).toString());
    parentItem()->setProperty("terminaiAccountNumber",qsettings.value(Ter_Account).toString());

    QList<QHostAddress> vAddressList = QNetworkInterface::allAddresses();
    for(int i=0;i<vAddressList.size();i++)
    {
        if((vAddressList.at(i)!=QHostAddress::LocalHost)&&(vAddressList.at(i).protocol()==QAbstractSocket::IPv4Protocol))
        parentItem()->setProperty("terminaiIP",vAddressList.at(i).toString()) ;
    }
}

void   LuxDomoTerminaiSetting::editInformation(QString  type, QString   data)
{
    if(type=="TerminaiAccountNumber")
    {
           dataType=TerminaiAccountNumber;
    }
   else   if(type=="TerminaiPasswd")
   {
           dataType=TerminaiPasswd;
   }
   g_pMainDlg->m_pHomeControlDlg->m_editName=data;
   g_pMainDlg->m_pHomeControlDlg->setHCLineText(data);
}

void   LuxDomoTerminaiSetting::setInformation(QString  data)
{
    if(dataType==TerminaiAccountNumber)
    {
          parentItem()->setProperty("terminaiAccountNumber",data);
    }
   else  if(dataType==TerminaiPasswd)
    {
          parentItem()->setProperty("terminaiPasswd",data);
    }
}

void  LuxDomoTerminaiSetting::saveInformation()
{
    QSettings    qsettings("/mnt/disk/config/homecontrol.ini",QSettings::IniFormat);
    QString    passwd=parentItem()->property("terminaiPasswd").toString();
    QString    accountNumber=parentItem()->property("terminaiAccountNumber").toString();
    qsettings.setValue(Ter_Account,accountNumber);
    qsettings.setValue(Ter_Pwd,passwd);
}

