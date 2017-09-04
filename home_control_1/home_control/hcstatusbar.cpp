#include "hcstatusbar.h"
#include"homecontrolmaindlg.h"
#include<QBitmap>
#include<unistd.h>
#include<maindialog.h>
#include<QSettings>
#include"MyBuffer.h"

extern MainDialog *g_pMainDlg;
extern LMap_6410_Msg mLMap_6410_Msg;
extern QDeclarativeItem *item;

HCStatusBar::HCStatusBar(QObject *parent) :
    QObject(parent)
{
    QSettings qsettings("home_control/sys.ini",QSettings::IniFormat);
    bool stat = qsettings.value("value").toBool();
    if(stat)
    {
        setBack(tr("解除主屏"));
    }
    else
    {
        setBack(tr("设为主屏"));
    }
    //setGround(stat);

    setExit(tr("退出"));
    setHcselect(tr("房间选择"));
    setIrlearn(tr("红外学习"));
}

HCStatusBar::~HCStatusBar()
{

}

/*************************房间选择********************************/
void HCStatusBar::roomChose()
{
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg*)parent();
    pHomeControlDlg->showFullScreen();
    QMetaObject::invokeMethod(item,"setState",Q_ARG(QVariant, "roomselect"));
}

void HCStatusBar::irLearn()
{
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg*)parent();
    pHomeControlDlg->showFullScreen();
    QMetaObject::invokeMethod(item,"setState",Q_ARG(QVariant, "irpasswd"));
}

/********************显示qml界面***********************/
void HCStatusBar::show()
{
    setlrName("");
    setlrName(mLMap_6410_Msg.Layer_Room_Name);
    setExit(tr(""));
    setExit(tr("退出"));
}



/**********************房间名称**********************/
QString HCStatusBar::getlrName() const
{
    return m_lrName;
}

void HCStatusBar::setlrName(const QString &name)
{
   m_lrName = name;
   emit sig_lrname();
}

/*********************房间选择**************************/
QString HCStatusBar::hcselect()const
{
    return m_Hcselect;
}

void HCStatusBar::setHcselect(const QString &hcselect)
{
    m_Hcselect = hcselect;
    emit sig_Hcselect();
}

/*********************红外学习**************************/
QString HCStatusBar::irlearning()const
{
    return m_Irlearn;
}

void HCStatusBar::setIrlearn(const QString &irlearning)
{
    m_Irlearn = irlearning;
    emit sig_Irlearn();
}

/*********************返回**************************/
QString HCStatusBar::back()const
{
    return m_Back;
}

void HCStatusBar::setBack(const QString &back)
{
    m_Back = back;
    emit sig_Back();
}

/*********************退出**************************/
QString HCStatusBar::exit()const
{
    return m_Exit;
}

void HCStatusBar::setExit(const QString &exit)
{
    m_Exit = exit;
    emit sig_Exit();
}

/************************退出家居界面***************************/
void HCStatusBar::closeHomeControlDlg()
{
    HomeControlMainDlg *pHomeControlDlg=(HomeControlMainDlg*)parent();
    pHomeControlDlg->hide();
}

void HCStatusBar::setGround(bool stat)
{
    if(m_Back == tr("设为主屏"))
    {
        stat = true;
    }
    else
    {
        stat = false;
    }
    if(stat)
    {
        setBack(tr("解除主屏"));
    }
    else
    {
        setBack(tr("设为主屏"));
    }
    QSettings qsettings("home_control/sys.ini",QSettings::IniFormat);
    qsettings.setValue("value",stat);
}

void HCStatusBar::setHeigh()
{
    QMetaObject::invokeMethod(item->findChild<QDeclarativeItem*>("statusBar"),"setHeightHide");
}

void HCStatusBar::Translate()
{
    QSettings qsettings("home_control/sys.ini",QSettings::IniFormat);
    bool stat = qsettings.value("value").toBool();
    if(stat)
        setBack(tr("解除主屏"));
    else
        setBack(tr("设为主屏"));
    setExit(tr("退出"));
    setHcselect(tr("房间选择"));
    setIrlearn(tr("红外学习"));
}
