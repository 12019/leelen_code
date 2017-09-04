#include "luxdomosetting.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;

typedef   enum
{
    None=0,
    TerminalSetting,
    CloudSetting,
    AboutSetting,
    FeedbackSetting,
    MajorSetting,
    SceneSetting,
    VersionsSetting,
    Synchronization,
    WLANSetting,
    Homepage

} ShowingType;

ShowingType     showingType=None;

LuxDomoSetting::LuxDomoSetting(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    showingItem=NULL;
    showingEngine=NULL;
    connect(g_pMainDlg->m_pHomeControlDlg->m_pLINCloudDEVSDK,SIGNAL(sigCloudCondition(QString)),this,SLOT(slot_setCloudCondition(QString)));
    connect(g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData,SIGNAL(sigIsWlan()),this,SLOT(slot_addWifiSetting()));
}
LuxDomoSetting::~LuxDomoSetting()
{
    g_pMainDlg->m_pHomeControlDlg->setting=NULL;
    showingType=None;
}
void   LuxDomoSetting::componentComplete()
{
    g_pMainDlg->m_pHomeControlDlg->setting=this;
    MyMsgLog<<g_pMainDlg->m_pHomeControlDlg->setting->objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW*7/8);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(mLMap_6410_Msg.screenW/8);
    parentItem()->setY(0);
    AddOptions();
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/setting_dialog/LuxDomoWLANSettingDialog.qml"));
    wlanItem = qobject_cast<QDeclarativeItem*>(component.create());
    wlanItem->setParentItem(parentItem());
    wlanItem->hide();
    parentItem()->hide();
}

void   LuxDomoSetting::AddOptions()
{
    QMetaObject::invokeMethod(parentItem(),"clearModel",Qt::DirectConnection);
    QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                                                     Q_ARG(QVariant,tr("version")),
                                                     Q_ARG(QVariant,tr("../setting/versionSetting.png")),
                                                     Q_ARG(QVariant,tr("版本更新")));
    QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                                                     Q_ARG(QVariant,tr("homepage")),
                                                     Q_ARG(QVariant,tr("../setting/homepageSetting.png")),
                                                     Q_ARG(QVariant,tr("设置主界面")));
    QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                                                     Q_ARG(QVariant,tr("synchronization")),
                                                     Q_ARG(QVariant,tr( "../setting/majorSetting.png")),
                                                     Q_ARG(QVariant,tr(" 文件同步")));
    QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                                                     Q_ARG(QVariant,tr("feedback")),
                                                     Q_ARG(QVariant,tr("../setting/feedbackSetting.png")),
                                                     Q_ARG(QVariant,tr("反馈")));
    QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                                                     Q_ARG(QVariant,tr("about")),
                                                     Q_ARG(QVariant,tr("../setting/aboutSetting.png")),
                                                     Q_ARG(QVariant,tr("关于")));
}

void  LuxDomoSetting::CreateElement(QString strFile)
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl(strFile));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem());
    showingItem=item;
    showingEngine=engine;
}

void   LuxDomoSetting::back()
{  
    g_pMainDlg->m_pHomeControlDlg->hide();
}

void  LuxDomoSetting::CloseItemDialog()
{
    if(showingItem!=NULL&&showingItem!=NULL)
    {
        showingEngine->deleteLater();
        showingItem->deleteLater();
        showingItem=NULL;
        showingEngine=NULL;
        showingType=None;
    }
}

void   LuxDomoSetting::chooseSettingOptions(QString type)
{
    MyMsgLog<<type;
    if(type==tr("terminal")&&showingType!=TerminalSetting)
    {
        if(showingItem!=NULL)
        {
            showingEngine->deleteLater();
            showingItem->deleteLater();
            showingEngine=NULL;
            showingItem=NULL;
        }
        wlanItem->hide();
        CreateElement("qrc:/QML/setting_dialog/LuxDomoTerminaiSettingDialog.qml");
        showingType=TerminalSetting;
        parentItem()->setProperty("pressIndex",-1);
    }
    else   if(type==tr("cloud")&&showingType!=CloudSetting)
    {
        if(showingItem!=NULL)
        {
            showingEngine->deleteLater();
            showingItem->deleteLater();
            showingEngine=NULL;
            showingItem=NULL;
        }
        wlanItem->hide();
        CreateElement("qrc:/QML/setting_dialog/LuxDomoCloudSettingDialog.qml");
        showingType=CloudSetting;
    }
    else   if(type==tr("scene")&&showingType!=SceneSetting)
    {
        if(g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->isRunning())
        {
            return;
        }
        if(showingItem!=NULL)
        {
            showingEngine->deleteLater();
            showingItem->deleteLater();
            showingEngine=NULL;
            showingItem=NULL;
        }
        wlanItem->hide();
        CreateElement("qrc:/QML/setting_dialog/LuxDomoSceneToolbarDialog.qml");
        QMetaObject::invokeMethod(showingItem,"init",Qt::DirectConnection);
        showingType=SceneSetting;
     }
    else   if(type==tr("synchronization")&&showingType!=Synchronization)
    {
        if(showingItem!=NULL)
        {
            showingEngine->deleteLater();
            showingItem->deleteLater();
            showingEngine=NULL;
            showingItem=NULL;
        }
        wlanItem->hide();
        CreateElement("qrc:/QML/setting_dialog/LuxDomoSynchronizationDialog.qml");
        showingType=Synchronization;
    }
    else   if(type==tr("version")&&showingType!=VersionsSetting)
    {
        if(showingItem!=NULL)
        {
            showingEngine->deleteLater();
            showingItem->deleteLater();
            showingEngine=NULL;
            showingItem=NULL;
        }
        wlanItem->hide();
        CreateElement("qrc:/QML/setting_dialog/LuxDomoVersionsSetttingDialog.qml");
        showingType=VersionsSetting;
     }
    else   if(type==tr("feedback")&&showingType!=FeedbackSetting)
    {
        if(showingItem!=NULL)
        {
            showingEngine->deleteLater();
            showingItem->deleteLater();
            showingEngine=NULL;
            showingItem=NULL;
        }
        wlanItem->hide();
        CreateElement("qrc:/QML/setting_dialog/LuxDomoFeedbackDialog.qml");
        showingType=FeedbackSetting;
    }
    else   if(type==tr("about")&&showingType!=AboutSetting)
    {
        if(showingItem!=NULL)
        {
            showingEngine->deleteLater();
            showingItem->deleteLater();
            showingEngine=NULL;
            showingItem=NULL;
        }
        wlanItem->hide();
        CreateElement("qrc:/QML/setting_dialog/LuxDomoAboutDialog.qml");
        showingType=AboutSetting;
    }
    else   if(type==tr("wlan")&&showingType!=WLANSetting)
    {
        if(showingItem!=NULL)
        {
            showingEngine->deleteLater();
            showingItem->deleteLater();
            showingEngine=NULL;
            showingItem=NULL;
        }
        wlanItem->show();
        QMetaObject::invokeMethod(wlanItem,"init",Qt::DirectConnection);
        showingType=WLANSetting;
    }
    else   if(type==tr("homepage")&&showingType!=Homepage)
    {
        if(showingItem!=NULL)
        {
            showingEngine->deleteLater();
            showingItem->deleteLater();
            showingEngine=NULL;
            showingItem=NULL;
        }
        wlanItem->hide();
        CreateElement("qrc:/QML/setting_dialog/LuxDomoHomepageSettingDialog.qml");
        showingType=Homepage;
    }
}

void   LuxDomoSetting::SetInformation(QString  data)
{
    if(showingType==TerminalSetting)
    {
           QMetaObject::invokeMethod(showingItem,"setInformation",Qt::DirectConnection,Q_ARG(QVariant,data));
    }
    else  if(showingType==CloudSetting)
    {
           QMetaObject::invokeMethod(showingItem,"setInformation",Qt::DirectConnection,Q_ARG(QVariant,data));
    }
    else  if(showingType==FeedbackSetting)
    {
          QMetaObject::invokeMethod(showingItem,"setInformation",Qt::DirectConnection,Q_ARG(QVariant,data));
    }
    else  if(showingType==WLANSetting)
    {
        QMetaObject::invokeMethod(wlanItem,"setInformation",Qt::DirectConnection,Q_ARG(QVariant,data));
    }
}

void   LuxDomoSetting::slot_setCloudCondition(QString  value)
{
    MyMsgLog<<value;
    if(showingType==CloudSetting&&showingItem!=NULL)
    {
        showingItem->setProperty("condition",value);
    }
}

void  LuxDomoSetting::slot_addWifiSetting()
{
    QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                                                     Q_ARG(QVariant,tr("wlan")),
                                                     Q_ARG(QVariant,tr("../setting/wlanSetting.png")),
                                                     Q_ARG(QVariant,tr("WLAN设置")));
}
