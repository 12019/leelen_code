#include "parastyle.h"
#include<maindialog.h>
#include<QSettings>
#include"MyBuffer.h"

extern LIN_ExceptionType mLIN_ExceptionSvr;
extern MainDialog *g_pMainDlg;

ParaStyle::ParaStyle(QObject *parent) : QObject(parent)
{

}

QString ParaStyle::getLabelText(QString type)
{
    if(type == "developerpwd_pwd")
    {
        return tr("密码");
    }
    else if(type == "pwdIsValid")
    {
        return tr("密码正确");
    }
    else if(type == "pwdIsInValid")
    {
        return tr("密码错误");
    }
    else if(type == "developerPwdLabel")
    {
        return tr("确定");
    }
    else if(type == "developer")
    {
        return tr("开发者选项");
    }
    else if(type == "room")
    {
        return tr("房间");
    }
    else if(type == "layer")
    {
        return tr("楼层");
    }
    else if(type == "devtype")
    {
        return tr("设备类型");
    }
    else if(type == "devattr")
    {
        return tr("设备属性");
    }
    else if(type == "devname")
    {
        return tr("设备名称");
    }
    else if(type == "devaddr")
    {
        return tr("设备地址");
    }
    else if(type == "devattrlist")
    {
        return tr("不可调光灯,调光灯,窗帘,电视(红外),背景音乐(红外),空调(红外),自定义");
    }
    else if(type == "btnname")
    {
        return tr("确定");
    }
    else
    {
        return "";
    }
}

void ParaStyle::setDeveloperPwdText(QString arg)
{
    mLIN_ExceptionSvr = LIN_DeveloperPwdText;
    g_pMainDlg->m_pHomeControlDlg->setHCLineText(arg);
}
