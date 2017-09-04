#include "hcsetting.h"
#include<mylineedit.h>
#include<maindialog.h>
#include"ui_inputinfodialog.h"
#include<QDebug>
#include<QSettings>
#include"MyBuffer.h"

extern LIN_ExceptionType mLIN_ExceptionSvr;
extern MainDialog *g_pMainDlg;
extern bool ttys7isValue;

HCSetting::HCSetting(QObject *parent) :
    QObject(parent)
{
    initInfo();

    QSettings qsettings(home_control_config,QSettings::IniFormat);
    if(qsettings.value("ResTartTime").isNull())
    {
        qsettings.setValue("ResTartTime",0);
    }
    else
    {
        int time = qsettings.value("ResTartTime").toInt();
        qsettings.setValue("ResTartTime",++time);
    }

}

/***************************************************************
 * *************云服务器IP的设置调用输入法*************************
 * *************************************************************/
void HCSetting::setCloundIpText(QString strIP)
{
    mLIN_ExceptionSvr = LIN_CloudSvrIP;
    g_pMainDlg->m_pHomeControlDlg->setHCLineText(strIP);
    /**QString str = showInputMethodDlg(strIP);
    if(!str.isEmpty())
    {
        setlin_CloudIP(str);
        QSettings qsettings(home_control_config,QSettings::IniFormat);
        qsettings.setValue(LIN_CLOUD_IP,str);
    }*/
}

/***************************************************************
 * *************云服务器ID的设置调用输入法*************************
 * *************************************************************/
void HCSetting::setCloundIDText(QString strID)
{
    mLIN_ExceptionSvr = LIN_CloudSvrID;
    g_pMainDlg->m_pHomeControlDlg->setHCLineText(strID);
    /*QString str = showInputMethodDlg(strID);
    if(!str.isEmpty())
    {
        setlin_LocalID(str);
        QSettings qsettings(home_control_config,QSettings::IniFormat);
        qsettings.setValue(LIN_CLOUD_ID,str);
    }*/
}

/***************************************************************
 * *************云服务器账号的设置调用输入法*************************
 * *************************************************************/
void HCSetting::setCloundAccountText(QString strAcc)
{
    mLIN_ExceptionSvr = LIN_CloudSvrAccount;
    g_pMainDlg->m_pHomeControlDlg->setHCLineText(strAcc);
    /*QString str = showInputMethodDlg(strAcc);
    if(!str.isEmpty())
    {
        setlin_CloudAcc(str);
        QSettings qsettings(home_control_config,QSettings::IniFormat);
        qsettings.setValue(LIN_CLOUD_ID,str);
    }*/
}

/***************************************************************
 * *************云服务器密码的设置调用输入法*************************
 * *************************************************************/
void HCSetting::setCloundPWDText(QString strPWD)
{
    mLIN_ExceptionSvr = LIN_CloudSvrPassWord;
    g_pMainDlg->m_pHomeControlDlg->setHCLineText(strPWD);
    /*QString str = showInputMethodDlg(strPWD);
    if(!str.isEmpty())
    {
        setlin_CloudPwd(strPWD);
        QSettings qsettings(home_control_config,QSettings::IniFormat);
        qsettings.setValue(LIN_CLOUD_ID,str);
    }*/
}

/***************************************************************
 ***************终端机账号的设置调用输入法*************************
 ***************************************************************/
void HCSetting::setTerAccountText(QString strAcc)
{
    mLIN_ExceptionSvr = LIN_TerSvrAccount;
    g_pMainDlg->m_pHomeControlDlg->setHCLineText(strAcc);
    /*QString str = showInputMethodDlg(strAcc);
    if(!str.isEmpty())
    {
        setTerAccount(str);
        QSettings qsettings(home_control_config,QSettings::IniFormat);
        qsettings.setValue(Ter_Account,str);
    }*/
}

/***************************************************************
 ***************终端机密码的设置调用输入法*************************
 ***************************************************************/
void HCSetting::setTerPwdText(QString strPwd)
{
    mLIN_ExceptionSvr = LIN_TerSvrPassWord;
    g_pMainDlg->m_pHomeControlDlg->setHCLineText(strPwd);
    /*QString str = showInputMethodDlg(strPwd);
    if(!str.isEmpty())
    {
        QSettings qsettings(home_control_config,QSettings::IniFormat);
        qsettings.setValue(Ter_Pwd,str);
        setTerPwd(str);
    }*/
}

/***************************************************************
 *************************显示输入法窗口**************************
 ***************************************************************/
QString HCSetting::showInputMethodDlg(QString PlainText)
{
    //g_pMainDlg->m_pHomeControlDlg->view->hide();
    g_pMainDlg->m_pInputMethodWidget->ui->InputInfoTextEdit->setText(PlainText);

    g_pMainDlg->m_pInputMethodWidget->exec();

    QString str = g_pMainDlg->m_pInputMethodWidget->ui->InputInfoTextEdit->toPlainText();
    //g_pMainDlg->m_pHomeControlDlg->view->showFullScreen();
    return str;
}

void HCSetting::initInfo()
{
    QString id = "8BF3E5434C3227C6234084BAF604481A";
    QSettings qsettings(home_control_config,QSettings::IniFormat);
    if(qsettings.value(LIN_CLOUD_IP).toString().isEmpty())
    {
        qsettings.setValue(LIN_CLOUD_IP,"117.29.160.158");
    }
    if(qsettings.value(LIN_CLOUD_ID).toString().isEmpty())
    {
        qsettings.setValue(LIN_CLOUD_ID,id);
    }
    if(qsettings.value(LIN_CLOUD_Acc).toString().isEmpty())
    {
        qsettings.setValue(LIN_CLOUD_Acc,"1111-4444");
    }
    if(qsettings.value(LIN_CLOUD_PWD).toString().isEmpty())
    {
        qsettings.setValue(LIN_CLOUD_PWD,"666666");
    }
    if(qsettings.value(Ter_Account).toString().isEmpty())
    {
        qsettings.setValue(Ter_Account,"1111-4444");
    }
    if(qsettings.value(Ter_Pwd).toString().isEmpty())
    {
        qsettings.setValue(Ter_Pwd,"666666");
    }
    setlin_CloudIP(qsettings.value(LIN_CLOUD_IP).toString());
    setlin_LocalID(qsettings.value(LIN_CLOUD_ID).toString());
    setlin_CloudAcc(qsettings.value(LIN_CLOUD_Acc).toString());
    setlin_CloudPwd(qsettings.value(LIN_CLOUD_PWD).toString());
    setTerAccount(qsettings.value(Ter_Account).toString());
    setTerPwd(qsettings.value(Ter_Pwd).toString());
}


/***************************************************************
 * *************************服务器IP的设置************************
 * **************************************************************/
QString HCSetting::getlin_CloudIP() const
{
    return mLIN_CLOUD_IP;
}

void HCSetting::setlin_CloudIP(const QString &stat)
{
    mLIN_CLOUD_IP = stat;
    emit sig_lin_CloudIP();
}

/***************************************************************
 * *************************服务器账号的设置************************
 * **************************************************************/
QString HCSetting::getlin_CloudAcc() const
{
    return mLIN_CLOUD_Account;
}

void HCSetting::setlin_CloudAcc(const QString &stat)
{
    mLIN_CLOUD_Account = stat;
    emit sig_lin_CloudAcc();
}

/***************************************************************
 * *************************服务器密码的设置************************
 * **************************************************************/
QString HCSetting::getlin_CloudPwd() const
{
    return mLIN_CLOUD_Pwd;
}

void HCSetting::setlin_CloudPwd(const QString &stat)
{
    mLIN_CLOUD_Pwd = stat;
    emit sig_lin_CloudPwd();
}


/***************************************************************
 * *************************服务器ID的设置************************
 * **************************************************************/
QString HCSetting::getlin_LocalID() const
{
    return mLIN_CLOUD_ID;
}

void HCSetting::setlin_LocalID(const QString &stat)
{
    mLIN_CLOUD_ID = stat;
    emit sig_lin_LocalID();
}

/***************************************************************
 * *************************终端机远程账号的设置************************
 * **************************************************************/
QString HCSetting::getTerAccount() const
{
    return TER_ACCOUNT;
}

void HCSetting::setTerAccount(const QString &stat)
{
    TER_ACCOUNT = stat;
    emit sig_TerAccount();
}


/***************************************************************
 * *************************终端机远程密码的设置************************
 * **************************************************************/
QString HCSetting::getTerPwd() const
{
    return TER_PWD;
}

void HCSetting::setTerPwd(const QString &stat)
{
    TER_PWD = stat;
    emit sig_TerPwd();
}

void HCSetting::setMainHide()
{
    g_pMainDlg->m_pHomeControlDlg->view->hide();
}

void HCSetting::serialdetect()
{
    ttys7isValue = false;
}

QString HCSetting::serialInfo()
{
    if(ttys7isValue == true)
    {
        return tr("串口检测成功");
    }
    else
    {
        return tr("串口检测失败,可能是未接线");
    }
}

QString HCSetting::getSettingNameText()
{
    return tr("设置");
}

QString HCSetting::getTerAccountText()
{
    return tr("账号:");
}

QString HCSetting::getTerPwdText()
{
    return tr("密码:");
}

QString HCSetting::getSvrIPText()
{
    return tr("云服务器IP:");
}

QString HCSetting::getSvrAccText()
{
    return tr("账号:");
}

QString HCSetting::getSvrPwdText()
{
    return tr("密码:");
}

QString HCSetting::getSvrIDText()
{
    return tr("云服务器ID:");
}

QString HCSetting::getSvrSetLabelText()
{
    return tr("云设置");
}

QString HCSetting::getTerSetLabelText()
{
    return tr("远程设置");
}

QString HCSetting::getSerialLabelText()
{
    return tr("串口信息");
}

QString HCSetting::getHomeLabelText()
{
    return tr("家居版本");
}

QString  HCSetting::getHomeVersionText()
{
      HomeControlMainDlg *pHomeControlMainDlg = (HomeControlMainDlg*)parent();
      return   tr("当前版本:")+pHomeControlMainDlg->Version;
}

QString   HCSetting::getNewVersionText()
{
    HomeControlMainDlg *pHomeControlMainDlg = (HomeControlMainDlg*)parent();
    int     flag=pHomeControlMainDlg->m_pLINCloudDEVSDK->GetUpdateInfo();
    if(flag==0)
      {   return  tr("存在版本更新");   }
    else  if(flag==2)
               {  return  tr("当前为最新版本");  }
    else
       {   return  tr("云断开");  }

   return  tr("");
}

bool    HCSetting::getIsUpdate()
{
    HomeControlMainDlg *pHomeControlMainDlg = (HomeControlMainDlg*)parent();
    int     flag=pHomeControlMainDlg->m_pLINCloudDEVSDK->GetUpdateInfo();
    if(flag==0)
     {
           return   true;
     }
    else
    {
           return   false;
     }

    return  false;
}
void  HCSetting::downloadNewVersion()
{
      HomeControlMainDlg *pHomeControlMainDlg = (HomeControlMainDlg*)parent();
      pHomeControlMainDlg->m_pLINVersionUpdate->start();
}

QString    HCSetting::getUpdateVersionText()
{
     return  tr("更新版本") ;
}

QString HCSetting::getCloseLabelText()
{
    return tr("退出");
}

QString HCSetting::getFinishLabelText()
{
    return tr("完成");
}
