#include "luxdomohomepagesetting.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
LuxDomoHomepageSetting::LuxDomoHomepageSetting(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoHomepageSetting::~LuxDomoHomepageSetting()
{

}

void  LuxDomoHomepageSetting::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW*9/16);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(mLMap_6410_Msg.screenW*5/16);
    parentItem()->setY(0);

    QString   describe(tr("       本系统是由多模块组成，有智能家居，楼宇对讲，警报系统等等，"
                                       "该系统默认结束屏保后显示楼宇界面。通过该项设置可使智能家居界，"
                                       "面作为屏保后界面。"));
    parentItem()->setProperty("describe",describe);

    QSettings qsettings("home_control/config.ini",QSettings::IniFormat);
    if(qsettings.value("isHomepage").toBool() == false||qsettings.value("isHomepage").isNull())
    {
         parentItem()->setProperty("result",false);
    }
    else
    {
         parentItem()->setProperty("result",true);
    }
}

void   LuxDomoHomepageSetting::setHomepage(bool value)
{
    QSettings qsettings("home_control/config.ini",QSettings::IniFormat);
    qsettings.setValue("isHomepage",value);
}
