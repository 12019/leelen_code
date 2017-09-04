#include "homecontrolscenedialog.h"
#include<QBitmap>
#include"homecontrolmaindlg.h"
#include"maindialog.h"
#include"ui_inputinfodialog.h"
#include<QSettings>

extern QDeclarativeItem *item;
extern MainDialog *g_pMainDlg;
extern LIN_ExceptionType mLIN_ExceptionSvr;

HomeControlSceneDialog::HomeControlSceneDialog(QObject *parent) :
    QObject(parent)
{
    sceneBtnNameIni();
    setS7text(tr("全开"));
    setS8text(tr("全关"));
    setS9text(tr("编辑"));
}

HomeControlSceneDialog::~HomeControlSceneDialog()
{

}

/***********************************************************************************
函数名 : sceneBtnNameIni
描  述 : 初始化场景按钮的名称
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlSceneDialog::sceneBtnNameIni()
{
    QSettings settings(home_control_config,QSettings::IniFormat);
    if(settings.value("sceneBtn1").toString().isEmpty())
    {
        settings.setValue("sceneBtn1","起床模式");
    }
    if(settings.value("sceneBtn2").toString().isEmpty())
    {
        settings.setValue("sceneBtn2","就餐模式");
    }
    if(settings.value("sceneBtn3").toString().isEmpty())
    {
        settings.setValue("sceneBtn3","影音模式");
    }
    if(settings.value("sceneBtn4").toString().isEmpty())
    {
        settings.setValue("sceneBtn4","睡眠模式");
    }
    if(settings.value("sceneBtn5").toString().isEmpty())
    {
        settings.setValue("sceneBtn5","自定义A");
    }
    if(settings.value("sceneBtn6").toString().isEmpty())
    {
        settings.setValue("sceneBtn6","自定义A");
    }
    setS1text(settings.value("sceneBtn1").toString());
    setS2text(settings.value("sceneBtn2").toString());
    setS3text(settings.value("sceneBtn3").toString());
    setS4text(settings.value("sceneBtn4").toString());
    setS5text(settings.value("sceneBtn5").toString());
    setS6text(settings.value("sceneBtn6").toString());
}

void HomeControlSceneDialog::pushBtnScene1Clicked(QString PlainText)
{
    mLIN_ExceptionSvr = LIN_NotAnything;
    HomeControlMainDlg *pHCMainDlg = (HomeControlMainDlg*)parent();
    pHCMainDlg->setHCLineText(PlainText);
    mLIN_ExceptionSvr = LIN_TerSceneFir;
}
void HomeControlSceneDialog::pushBtnScene2Clicked(QString PlainText)
{
    mLIN_ExceptionSvr = LIN_NotAnything;
    HomeControlMainDlg *pHCMainDlg = (HomeControlMainDlg*)parent();
    pHCMainDlg->setHCLineText(PlainText);
    mLIN_ExceptionSvr = LIN_TerSceneSec;
}
void HomeControlSceneDialog::pushBtnScene3Clicked(QString PlainText)
{
    mLIN_ExceptionSvr = LIN_NotAnything;
    HomeControlMainDlg *pHCMainDlg = (HomeControlMainDlg*)parent();
    pHCMainDlg->setHCLineText(PlainText);
    mLIN_ExceptionSvr = LIN_TerSceneThird;
}
void HomeControlSceneDialog::pushBtnScene4Clicked(QString PlainText)
{
    mLIN_ExceptionSvr = LIN_NotAnything;
    HomeControlMainDlg *pHCMainDlg = (HomeControlMainDlg*)parent();
    pHCMainDlg->setHCLineText(PlainText);
    mLIN_ExceptionSvr = LIN_TerSceneFour;
}
void HomeControlSceneDialog::pushBtnScene5Clicked(QString PlainText)
{
    mLIN_ExceptionSvr = LIN_NotAnything;
    HomeControlMainDlg *pHCMainDlg = (HomeControlMainDlg*)parent();
    pHCMainDlg->setHCLineText(PlainText);
    mLIN_ExceptionSvr = LIN_TerSceneFive;
}
void HomeControlSceneDialog::pushBtnScene6Clicked(QString PlainText)
{
    mLIN_ExceptionSvr = LIN_NotAnything;
    HomeControlMainDlg *pHCMainDlg = (HomeControlMainDlg*)parent();
    pHCMainDlg->setHCLineText(PlainText);
    mLIN_ExceptionSvr = LIN_TerSceneSix;
}

/************************场景按钮控制***************************/
void HomeControlSceneDialog::pushButtonScenePressed(int sceneId)
{
    if(sceneId <= 0 || sceneId >=7)
    {
        qDebug()<<"sceneId not exits";
        return;
    }
    short cnt = 0;
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg*)parent();
    pHomeControlDlg->m_pSceneApplyThread->isQuit = true;
    while (pHomeControlDlg->m_pSceneApplyThread->isRunning()) {
        usleep(50000);
        if(cnt ++ >= 20)
            break;
    }
    pHomeControlDlg->m_pSceneApplyThread->isQuit = false;
    pHomeControlDlg->m_pSceneApplyThread->indx = sceneId;
    pHomeControlDlg->m_pSceneApplyThread->start(QThread::LowestPriority);
}


void HomeControlSceneDialog::pushButtonScene7Pressed()
{
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg*)parent();
    char cmd[6];
    cmd[0]=0x06;    //字节长度
    cmd[1]=0xff; //目的地址设备号
    cmd[2]=0x00;  //源地址
    cmd[3]=0xff;   //设备属性

    cmd[4]=0x01;
    cmd[5]=0x00;
    pHomeControlDlg->cmdBuffer(cmd);
}

void HomeControlSceneDialog::pushButtonScene8Pressed()
{
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg*)parent();

    char cmd[6];
    cmd[0]=0x06;    //字节长度
    cmd[1]=0xff; //目的地址设备号
    cmd[2]=0x00;  //源地址
    cmd[3]=0xff;   //设备属性

    cmd[4]=0x00;
    cmd[5]=0x00;
    pHomeControlDlg->cmdBuffer(cmd);
}

void HomeControlSceneDialog::pushButtonScene9Pressed()
{
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg*)parent();
    pHomeControlDlg->setMainState("sceneEdit");
}

void HomeControlSceneDialog::setSceneName(int index, QString name, QString newname)
{
    QMetaObject::invokeMethod(item,"setSceneViewName",
                              Q_ARG(QVariant, index),
                              Q_ARG(QVariant, name),
                              Q_ARG(QVariant, newname));
}

void HomeControlSceneDialog::Translate()
{
    setS7text(tr("全开"));
    setS8text(tr("全关"));
    setS9text(tr("编辑"));
}

