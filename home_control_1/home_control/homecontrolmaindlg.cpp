#include "homecontrolmaindlg.h"
#include "ui_syssetpassworddialog.h"
#include<QPixmap>
#include"MyBuffer.h"
#include"maindialog.h"
#include<QDebug>
#include "data_types.h"
#include "maindialog.h"
#include "ui_maindialog.h"
#include "helpdialog.h"
#include <linux/watchdog.h>
#include <sys/ioctl.h>
#include "application.h"
#include<QPointF>
#include"ui_inputinfodialog.h"
#include<QHostInfo>
#include<QNetworkInterface>

MyQByteArray *m_pMyQByteArray;
extern MainDialog *g_pMainDlg;
HCQextSerialPort *myCom;
extern short layerId,roomId;
QDeclarativeItem *item;
LIN_ExceptionType mLIN_ExceptionSvr;

EditDataType    dataType;
OperateType     operatingType=NotData;
OperateWlan    operateWlan;
LMap_6410_Msg mLMap_6410_Msg;
QDeclarativeItem    *smartHomeItem;
AcInfo         acInfo;
MusicInfo   musicInfo;
DeviceTypes   deviceTypes;
QMultiMap<QString,QDeclarativeEngine*>   engines;

HomeControlMainDlg::HomeControlMainDlg(QObject *parent) :
    QObject(parent)
{
    QSettings qsettings("home_control/sys.ini",QSettings::IniFormat);
    qsettings.setValue("isFirstLanuch",true);

    //对配置文件进行同步或者更新，优化
    CheckHomeControlFolder();
    CheckVersionConfigurationFile();
    CheckConfigurationFile();
    CheckRiscoConfigurationFile();
    SynchronizationfigurationFile();
    UpdateConfigurationFile();
    openHouseMapXmlFile("home_control/house-map-default1.xml");
    //对配置文件进行同步或者更新，优化

     //获取屏幕分辨率
    mLMap_6410_Msg.ImgHeightRatio = 1.0;
    mLMap_6410_Msg.ImgWidthRatio = 1.0;
    mLMap_6410_Msg.roomImgH = 0;
    mLMap_6410_Msg.roomImgW = 0;
    mLMap_6410_Msg.screenH = 0;
    mLMap_6410_Msg.screenW = 0;
    mLMap_6410_Msg.screenW=QApplication::desktop()->width();
    mLMap_6410_Msg.screenH=QApplication::desktop()->height();
    mLMap_6410_Msg.xPiece=5;
    mLMap_6410_Msg.yPiece=4;
    MyMsgLog<<"mLMap_6410_Msg.screenW:"<<mLMap_6410_Msg.screenW;
    MyMsgLog<<"mLMap_6410_Msg.screenH:"<<mLMap_6410_Msg.screenH;
    if(mLMap_6410_Msg.screenW==800&&mLMap_6410_Msg.screenH==480)
    {
        mLMap_6410_Msg.controlW=70;                                                                //70        //80           //17    //20
        mLMap_6410_Msg.controlH=90;                                                               //90        //110
        mLMap_6410_Msg.pieceW=130;                                                              //130     //160
        mLMap_6410_Msg.pieceH=90;                                                               //90       //110
        mLMap_6410_Msg.topMargin=80;                                                        //80       //110
        mLMap_6410_Msg.leftMargin=100;                                                     //100    //147
        mLMap_6410_Msg.interval=10;                                                          //10      //10
        mLMap_6410_Msg.topMargin1=18;                                                  //18      //30
        mLMap_6410_Msg.leftMargin1=50;                                                 //50      //83
        mLMap_6410_Msg.controlFont=17;

    }
    else   if(mLMap_6410_Msg.screenW==1024&&mLMap_6410_Msg.screenH==600)
    {
        mLMap_6410_Msg.controlW=80;
        mLMap_6410_Msg.controlH=110;
        mLMap_6410_Msg.pieceW=160;
        mLMap_6410_Msg.pieceH=110;
        mLMap_6410_Msg.topMargin=110;
        mLMap_6410_Msg.leftMargin=147;
        mLMap_6410_Msg.interval=10;
        mLMap_6410_Msg.topMargin1=30;
        mLMap_6410_Msg.leftMargin1=83;
        mLMap_6410_Msg.controlFont=20;
    }
    else
    {
        mLMap_6410_Msg.controlW=100;
        mLMap_6410_Msg.controlH=140;
        mLMap_6410_Msg.pieceW=200;
        mLMap_6410_Msg.pieceH=140;
        mLMap_6410_Msg.topMargin=170;
        mLMap_6410_Msg.leftMargin=190;
        mLMap_6410_Msg.interval=10;
        mLMap_6410_Msg.topMargin1=40;
        mLMap_6410_Msg.leftMargin1=110;
        mLMap_6410_Msg.controlFont=23;
    }
    //获取屏幕分辨率

     //ImageTransform();
    m_pIrpasswd = new Irpasswd(this);
    m_pHCStatusBar=new HCStatusBar(this); //
    m_pHomeControlSceneDlg=new HomeControlSceneDialog(this);
    m_pAccontrolDlg=new AccontrolDialog(this);
    m_pCurtainControlDlg = new CurtainControlDialog(this);
    m_pLightControlDlg = new LightControlDialog(this);
    m_pTVControlDlg = new TVControlDialog(this);
    m_pMusicControlDlg = new MusicControlDialog(this);
    m_pHCRoomSelect = new HCRoomSelect(this);
    m_pIrLearnDlg = new IrLearnDialog(this);
    m_pHCSceneEdit = new HomeControlSceneEdit(this);
    m_pSceneApplyThread = new HCSceneApplyThread(this);
    m_pRcvDataThread = new RcvDataThread(this);
    m_pHCSetting = new HCSetting(this);

    //串口启动与云服务初始化
    myCom = new HCQextSerialPort;
    myCom->startThread();
    m_pMyQByteArray = new MyQByteArray;
    m_pLINCloudDEVSDK = new  LINCloudDevSDKThread(this);
    m_pLINVersionUpdate=new  LinVersionUpdateThread(this);
    //串口启动与云服务初始化

    m_pDeveloper = new Developer(this);
    m_pParaStyle = new ParaStyle(this);
    view = new QDeclarativeView;
    view->rootContext()->setContextProperty("m_pappWindow",this);
    view->rootContext()->setContextProperty("m_pHCStatusBar",m_pHCStatusBar);
    view->rootContext()->setContextProperty("m_pAccontrolDlg",m_pAccontrolDlg);
    view->rootContext()->setContextProperty("m_pCurtainControlDlg",m_pCurtainControlDlg);
    view->rootContext()->setContextProperty("m_pLightDialog",m_pLightControlDlg);
    view->rootContext()->setContextProperty("m_pTVControlDlg",m_pTVControlDlg);
    view->rootContext()->setContextProperty("m_pMusicControlDlg",m_pMusicControlDlg);
    view->rootContext()->setContextProperty("m_pHCSceneDialog",m_pHomeControlSceneDlg);
    view->rootContext()->setContextProperty("m_pHCSceneEdit",m_pHCSceneEdit);
    view->rootContext()->setContextProperty("m_pIrpasswdDlg",m_pIrpasswd);
    view->rootContext()->setContextProperty("m_pIrLearnDialog",m_pIrLearnDlg);
    view->rootContext()->setContextProperty("m_pHCRoomSelect",m_pHCRoomSelect);
    view->rootContext()->setContextProperty("m_pHCSetting",m_pHCSetting);
    view->rootContext()->setContextProperty("m_pDeveloper",m_pDeveloper);
    view->rootContext()->setContextProperty("m_pParaStyle",m_pParaStyle);
    //m_pParaStyle->setObjectName("asdfasfsadfds");
    view->setSource(QUrl("qrc:/Myqml/HCMainDlg.qml"));
    item = qobject_cast<QDeclarativeItem*>(view->rootObject());
    item->setProperty("width",mLMap_6410_Msg.screenW);
    item->setProperty("height",mLMap_6410_Msg.screenH);
    m_phouseMapXmlThread = new houseMapXmlThread(this);
    m_phouseMapXmlThread->eventType = creatscenexml;
   //m_phouseMapXmlThread->start();
    /*********************第一次载入配置文件时创建八个场景文件**********************/


    connect(m_pSceneApplyThread,SIGNAL(sig_sceneBuffer(char*,int,QString)),this,SLOT(slot_sceneBuffer(char*,int,QString)),Qt::BlockingQueuedConnection);
    connect(this,SIGNAL(sigSecurityStatus(int)),this,SLOT(setSecurityStatus(int)));
    m_pHCStatusBar->setHeigh();

    HCLineEdit = new MyLineEdit;
    //connect(HCLineEdit,SIGNAL(textChanged(QString)),this,SLOT(slot_textChanged(QString)));
    view->setParent(g_pMainDlg);
    view->setWindowFlags(Qt::FramelessWindowHint);
    view->setWindowFlags(Qt::WindowStaysOnBottomHint);
    view->hide();
    this->Version=tr("1.1.8");

    m_pServer=new LuxdomoServer(this);
    QSettings settings("/mnt/disk/config/config",QSettings::IniFormat);
    QString  ip=settings.value("system/native_ip").toString();
    if(m_pServer->CloseListen())
        m_pServer->OpenListen(ip,tr("555"));

    this->mainIterface=NULL;
    this->menu=NULL;
    this->m_pThemebar=NULL;
    this->setting=NULL;
    this->m_pHomeManage=NULL;
    this->m_pRoomManage=NULL;
    this->roomControl=NULL;
    this->m_pLoadingLabel=NULL;
    this->m_pRisco=NULL;
    this->m_pHint=NULL;
    this->m_pHintEngine=NULL;
    this->roomControlPages.clear();
    this->m_varValue=false;
    this->m_musicVolume=false;
    timer = new  QTimer(this);
    timer1=new  QTimer(this);
    m_checkThreadTimer=new   QTimer(this);
    m_closeHintTimer=new   QTimer(this);
    connect(timer, SIGNAL(timeout()),this,SLOT(slot_varValue()));
    connect(timer1, SIGNAL(timeout()),this,SLOT(slot_musicVolume()));
    connect(m_checkThreadTimer, SIGNAL(timeout()),this,SLOT(slot_checkThread()));
    connect(m_closeHintTimer, SIGNAL(timeout()),this,SLOT(slot_closeHint()));
    connect(HCLineEdit,SIGNAL(textChanged(QString)),this,SLOT(slot_editData(QString)));
    m_pLoadingDataToUI=new     LuxDomoLoadingDataToUI;
    m_pChangDeviceData=new    LuxDomoChangDeviceData;
    m_pManageHomeData=new  LuxDomoManageHomeData;
    m_pHomeScene=new   LuxDomoHomeScene;
    m_pApplyScene=new   LuxDomoApplyScene;
    m_pSendCommand=new  LuxDomoSendCommand;
    m_pRiscoSecurityData=new   LuxDomoRiscoSecurityData;
    m_pDealWlanData=new  LuxDomoDealWlanData;
    connect(m_pDealWlanData,SIGNAL(sigUpdateListen()),this,SLOT(slot_openListen()));
    m_pCheckWlan=new  LuxDomoCheckWlan;
    connect(m_pLoadingDataToUI,SIGNAL(sigHideLoadingLabel()),this,SLOT(slot_hideLoadingLabel()),Qt::BlockingQueuedConnection);
    qmlRegisterType<LuxDomoLoadingLabel>("LuxDomoLoadingLabel", 1, 0, "LuxDomoLoadingLabelItem");
    qmlRegisterType<LuxDomoMenu>("LuxDomoMenu", 1, 0, "LuxDomoMenuItem");
    qmlRegisterType<LuxDomoMainInterface>("LuxDomoMainInterface", 1, 0, "LuxDomoMainInterfaceItem");
    qmlRegisterType<LuxDomoScene>("LuxDomoScene", 1, 0, "LuxDomoSceneItem");
    qmlRegisterType<LuxDomoSetting>("LuxDomoSetting", 1, 0, "LuxDomoSettingItem");
    qmlRegisterType<LuxDomoTerminaiSetting>("LuxDomoTerminaiSetting", 1, 0, "LuxDomoTerminaiSettingItem");
    qmlRegisterType<LuxDomoCloudSetting>("LuxDomoCloudSetting", 1, 0, "LuxDomoCloudSettingItem");
    qmlRegisterType<LuxDomoSceneSetting>("LuxDomoSceneSetting", 1, 0, "LuxDomoSceneSettingItem");
    qmlRegisterType<LuxDomoSceneFunctionOption>("LuxDomoSceneFunctionOption", 1, 0, "LuxDomoSceneFunctionOptionItem");
    qmlRegisterType<LuxDomoSceneToolbar>("LuxDomoSceneToolbar", 1, 0, "LuxDomoSceneToolbarItem");
    qmlRegisterType<LuxDomoVersionsSettting>("LuxDomoVersionsSettting", 1, 0, "LuxDomoVersionsSetttingItem");
    qmlRegisterType<LuxDomoMajorSetting>("LuxDomoMajorSetting", 1, 0, "LuxDomoMajorSettingItem");
    qmlRegisterType<LuxDomoFeedbackSetting>("LuxDomoFeedbackSetting", 1, 0, "LuxDomoFeedbackSettingItem");
    qmlRegisterType<LuxDomoAboutSetting>("LuxDomoAboutSetting", 1, 0, "LuxDomoAboutSettingItem");
    qmlRegisterType<LuxDomoRoomControl>("LuxDomoRoomControl", 1, 0, "LuxDomoRoomControlItem");
    qmlRegisterType<LuxDomoRoomControlPage>("LuxDomoRoomControlPage", 1, 0, "LuxDomoRoomControlPageItem");
    qmlRegisterType<LuxDomoRoomDevice>("LuxDomoRoomDevice", 1, 0, "LuxDomoRoomDeviceItem");
    qmlRegisterType<LuxDomoDeviceFolder>("LuxDomoDeviceFolder", 1, 0, "LuxDomoDeviceFolderItem");
    qmlRegisterType<LuxDomoDeviceFolderPage>("LuxDomoDeviceFolderPage", 1, 0, "LuxDomoDeviceFolderPageItem");
    qmlRegisterType<LuxDomoRoom>("LuxDomoRoom", 1, 0, "LuxDomoRoomItem");
    qmlRegisterType<LuxDomoRoomContainer>("LuxDomoRoomContainer", 1, 0, "LuxDomoRoomContainerItem");
    qmlRegisterType<LuxDomoHomeManage>("LuxDomoHomeManage", 1, 0, "LuxDomoHomeManageItem");
    qmlRegisterType<LuxDomoToolbar>("LuxDomoToolbar", 1, 0, "LuxDomoToolbarItem");
    qmlRegisterType<LuxDomoPullDownList>("LuxDomoPullDownList", 1, 0, "LuxDomoPullDownListItem");
    qmlRegisterType<LuxDomoRoomFunctionOption>("LuxDomoRoomFunctionOption", 1, 0, "LuxDomoRoomFunctionOptionItem");
    qmlRegisterType<LuxDomoRoomManage>("LuxDomoRoomManage", 1, 0, "LuxDomoRoomManageItem");
    qmlRegisterType<LuxDomoDeviceContainer>("LuxDomoDeviceContainer", 1, 0, "LuxDomoDeviceContainerItem");
    qmlRegisterType<LuxDomoDeviceOption>("LuxDomoDeviceOption", 1, 0, "LuxDomoDeviceOptionItem");
    qmlRegisterType<LuxDomoDevice>("LuxDomoDevice", 1, 0, "LuxDomoDeviceItem");
    qmlRegisterType<LuxDomoDeviceFunctionOption>("LuxDomoDeviceFunctionOption", 1, 0, "LuxDomoDeviceFunctionOptionItem");
    qmlRegisterType<LuxDomoRoomSceneControl>("LuxDomoRoomSceneControl", 1, 0, "LuxDomoRoomSceneControlItem");
    qmlRegisterType<LuxDomoAcControl>("LuxDomoAcControl", 1, 0, "LuxDomoAcControlItem");
    qmlRegisterType<LuxDomoMusicControl>("LuxDomoMusicControl", 1, 0, "LuxDomoMusicControlItem");
    qmlRegisterType<LuxDomoVar_LightControl>("LuxDomoLightControl", 1, 0, "LuxDomoLightControlItem");
    qmlRegisterType<LuxDomoTVControl>("LuxDomoTVControl", 1, 0, "LuxDomoTVControlItem");
    qmlRegisterType<LuxDomoCurtainControl>("LuxDomoCurtainControl", 1, 0, "LuxDomoCurtainControlItem");
    qmlRegisterType<LuxDomoTvLearn>("LuxDomoTvLearn", 1, 0, "LuxDomoTvLearnItem");
    qmlRegisterType<LuxDomoAcLearn>("LuxDomoAcLearn", 1, 0, "LuxDomoAcLearnItem");
    qmlRegisterType<LuxDomoMusicLearn>("LuxDomoMusicLearn", 1, 0, "LuxDomoMusicLearnItem");
    qmlRegisterType<LuxDomoList>("LuxDomoList", 1, 0, "LuxDomoListItem");
    qmlRegisterType<LuxDomoAcEdit>("LuxDomoAcEdit", 1, 0, "LuxDomoAcEditItem");
    qmlRegisterType<LuxDomoTvEdit>("LuxDomoTvEdit", 1, 0, "LuxDomoTvEditItem");
    qmlRegisterType<LuxDomoMusicEdit>("LuxDomoMusicEdit", 1, 0, "LuxDomoMusicEditItem");
    qmlRegisterType<LuxDomoVarLight>("LuxDomoVarLight", 1, 0, "LuxDomoVarLightItem");
    qmlRegisterType<LuxDomoThemebar>("LuxDomoThemebar", 1, 0, "LuxDomoThemebarItem");
    qmlRegisterType<LuxDomoStyleList>("LuxDomoStyleList", 1, 0, "LuxDomoStyleListItem");
    qmlRegisterType<LuxDomoRisco>("LuxDomoRisco", 1, 0, "LuxDomoRiscoItem");
    qmlRegisterType<LuxDomoRiscoControl>("LuxDomoRiscoControl", 1, 0, "LuxDomoRiscoControlItem");
    qmlRegisterType<LuxDomoRiscoFinder>("LuxDomoRiscoFinder", 1, 0, "LuxDomoRiscoFinderItem");
    qmlRegisterType<LuxDomoRiscoHistoryMeaasge>("LuxDomoRiscoHistoryMeaasge", 1, 0, "LuxDomoRiscoHistoryMeaasgeItem");
    qmlRegisterType<LuxDomoRiscoSecurity>("LuxDomoRiscoSecurity", 1, 0, "LuxDomoRiscoSecurityItem");
    qmlRegisterType<LuxDomoSynchronization>("LuxDomoSynchronization", 1, 0, "LuxDomoSynchronizationItem");
    qmlRegisterType<LuxDomoWLANSetting>("LuxDomoWLANSetting", 1, 0, "LuxDomoWLANSettingItem");
    qmlRegisterType<LuxDomoHomepageSetting>("LuxDomoHomepageSetting", 1, 0, "LuxDomoHomepageSettingItem");
    qmlRegisterType<LuxDomoSensorInformation>("LuxDomoSensorInformation", 1, 0, "LuxDomoSensorInformationItem");
}


HomeControlMainDlg::~HomeControlMainDlg()
{
    delete m_pAccontrolDlg;
    delete m_pCurtainControlDlg;
    delete m_pHCRoomSelect;
    delete m_pHCSceneEdit;
    delete m_pHCSetting;
    delete m_pHCStatusBar;
    delete m_pHomeControlSceneDlg;
    delete m_phouseMapXmlThread;
    delete m_pIrLearnDlg;
    delete m_pIrpasswd;
    delete m_pLightControlDlg;
    delete m_pMusicControlDlg;
    delete m_pRcvDataThread;
    delete m_pSceneApplyThread;
    delete m_pTVControlDlg;
    delete view;
}

/***********************************************************************************
函数名 : findDevState
描  述 : 终端机第一个开机则发送一天查询指令来查询设备状态
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::findDevState()
{
    char strCmd[6];
    strCmd[0] = 0x06;
    strCmd[1] = 0xff;
    strCmd[2] = 0xff;
    strCmd[3] = 0xff;
    strCmd[4] = 0x0f;
    strCmd[5] = 0x00;
    cmdBuffer(strCmd);
}

void  HomeControlMainDlg::findRiscoState()
{
    char strCmd[8];
    strCmd[0] = 0x08;
    strCmd[1] = 0x01;
    strCmd[2] = 0xff;
    strCmd[3] = 0xbb;
    strCmd[4] = 0x0f;
    strCmd[5] = 0xbe;
    strCmd[6] = 0x00;
    strCmd[7] = 0x00;
    cmdBuffer(strCmd);
}
/***********************************************************************************
函数名 : findDevice
描  述 : 对点击的左边进行解析查找,如果有被某个设备包含则调用showDlg
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::findDevice(int x, int y)
{
    //MyMsgLog<<"search my device"<<x<<y;
    QPointF XY;
    XY.setX(x);
    XY.setY(y);
    QPolygonF DeviveQPolygonF;
    QDomElement ndeviceElement = m_deviceElement;
    QDomElement deviceShapeElement;
    while(!ndeviceElement.isNull())
    {
        DeviveQPolygonF.clear();
        deviceShapeElement = ndeviceElement.firstChildElement("point");
        while(!deviceShapeElement.isNull())
        {
            DeviveQPolygonF.append(QPointF(
                                       deviceShapeElement.attribute("x").toInt()*mLMap_6410_Msg.ImgWidthRatio,
                                       deviceShapeElement.attribute("y").toInt()*mLMap_6410_Msg.ImgHeightRatio));
            deviceShapeElement = deviceShapeElement.nextSiblingElement("point");
        }
        if(DeviveQPolygonF.containsPoint(XY,Qt::OddEvenFill))
        {
            QDeclarativeItem *Item = item;
            QMetaObject::invokeMethod(Item,"setPos",
                                      Q_ARG(QVariant,x),
                                      Q_ARG(QVariant,y));
            //MyMsgLog<<"iscontainhighlight";
            deviceElement = ndeviceElement;
            showDeviceDlg();
            return;
        }
        ndeviceElement=ndeviceElement.nextSiblingElement("device");
    }
}

/***********************************************************************************
函数名 : showDeviceDlg
描  述 : 当点击屏幕时,若坐标有包含到设备则进入该函数进行设备类型的判断并显示窗口
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::showDeviceDlg()
{
    int attr = (deviceElement.attribute("id").toInt()>>8) & 0xff;
    QString bPowerOn=deviceElement.attribute("power");
    QString Type = deviceElement.attribute("type");
    QString type1=deviceElement.attribute("subtype");
    //MyMsgLog<<deviceElement.attribute("id")<<bPowerOn<<Type<<type1;
    if(Type == "light")
    {
        if(type1 == "var")
            deviceType = HC_VARLIGHT;
        else
            deviceType = HC_LIGHT;
    }
    if(Type == "switch")
    {
        deviceType = HC_LIGHT;
    }
    else if(Type == "curtain")
    {
        deviceType = HC_CURTAIN;
    }
    else if(Type == "ac")
    {
        deviceType = HC_AC;
    }
    else if(Type == "tv")
    {
        deviceType = HC_TV;
    }
    else if(Type == "music")
    {
        deviceType = HC_MUSIC;
    }
    else if(Type == "custom")
    {
        deviceType = HC_USER;
    }
    QDeclarativeItem *Item =item;
    switch (deviceType) {
    case HC_LIGHT:
        m_pLightControlDlg->lightElement = deviceElement;

        if(bPowerOn=="off")
        {
            QMetaObject::invokeMethod(Item,"showDevIcon",
                                      Q_ARG(QVariant,Type),
                                      Q_ARG(QVariant,deviceElement.attribute("id")));
            int deviceId=deviceElement.attribute("id").toInt();
            char strCmd[6];
            strCmd[0]=0x06;
            strCmd[1]=deviceId&0xff; //目的地址设备号
            strCmd[2]=0x00;  //源地址
            strCmd[3] = deviceId>>8;
            strCmd[4]= 1;   //指令
            strCmd[5]=0x00;
            cmdBuffer(strCmd);
            deviceElement.setAttribute("power","on");
        }
        else if(bPowerOn=="on")
        {
            QMetaObject::invokeMethod(Item,"hideDevIcon",
                                      Q_ARG(QVariant,Type),
                                      Q_ARG(QVariant,deviceElement.attribute("id")));
            int deviceId=deviceElement.attribute("id").toInt();
            char strCmd[6];
            strCmd[0]=0x06;
            strCmd[1]=deviceId&0xff; //目的地址设备号
            strCmd[2]=0x00;  //源地址
            strCmd[3] = deviceId>>8;
            strCmd[4]= 0;   //指令
            strCmd[5]=0x00;
            cmdBuffer(strCmd);
            deviceElement.setAttribute("power","off");
        }
        break;
    case HC_VARLIGHT:
    {
        m_pLightControlDlg->id = deviceElement.attribute("id");
        QMetaObject::invokeMethod(item,"setState",
                                  Q_ARG(QVariant,"light"));
        m_pLightControlDlg->resetState(deviceElement);
    }
        break;
    case HC_SWITCH:
        //m_pCurtainControlDlg->show();
        break;
    case HC_CURTAIN:
        m_pCurtainControlDlg->deviceElement = deviceElement;
        QMetaObject::invokeMethod(item,"setState",Q_ARG(QVariant, "curtain"));
        break;
    case HC_AC:
        m_pAccontrolDlg->id = deviceElement.attribute("id");
        QMetaObject::invokeMethod(item,"setState",
                                  Q_ARG(QVariant,"ac"));
        m_pAccontrolDlg->resetState(deviceElement);
        break;
    case HC_TV:
        m_pTVControlDlg->id = deviceElement.attribute("id");
        QMetaObject::invokeMethod(item,"setState",Q_ARG(QVariant, "tv"));
        m_pTVControlDlg->resetState(deviceElement);
        break;
    case HC_MUSIC:
        m_pMusicControlDlg->id = deviceElement.attribute("id");
        QMetaObject::invokeMethod(item,"setState",Q_ARG(QVariant, "music"));
        m_pMusicControlDlg->resetState(deviceElement);
        break;
    case HC_USER:
        if(attr == Central_AC)
        {
            m_pAccontrolDlg->id = deviceElement.attribute("id");
            QMetaObject::invokeMethod(item,"setState",
                                      Q_ARG(QVariant,"ac"));
            m_pAccontrolDlg->resetState(deviceElement);
        }
        break;
    default:
        break;
    }
}

/***********************************************************************************
函数名 : openHouseMapXmlFile
描  述 : 对配置文件进行初始化并打开
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::openHouseMapXmlFile(QString strFile)
{
    QFile file(strFile);
    if(!file.exists())
        return;
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    if (!m_domDoc.setContent(&file))
    {
        MyMsgLog<<"the"<<strFile<<"openfailed";
        file.close();
        return;
    }
    file.close();
}

/***********************************************************************************
函数名 : loadFisrMap
描  述 : 开机载入第一个房间
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::loadFisrMap()
{
    qreal x,y;
    QString roomImage;
    QDomElement roomElement;
    QDomElement deviceElement;
    QDomElement deviceShapeElement;
    roomElement=m_layerElement.firstChildElement("room");
    deviceElement=roomElement.firstChildElement("device");
    QString roomName;          //房间名

    roomName=roomElement.attribute("name");
    mLMap_6410_Msg.Layer_Room_Name = QString::number(layerId)+"层 "+roomName;
    roomImage= "home_control/" + roomElement.attribute("image");
    MyMsgLog<<"roomImg:"<<roomImage;
    qreal roomImageHeight,roomImageWidth;
    QPixmap pixmap(roomImage);
    roomImageHeight=pixmap.height();
    roomImageWidth=pixmap.width();

    QString currentImg = roomImage.insert(13,"m");//"home_control/"  + "m" + roomImage;
    QPixmap pixmap1(currentImg);
    mLMap_6410_Msg.roomImgW = pixmap1.width();
    mLMap_6410_Msg.roomImgH = pixmap1.height();

    mLMap_6410_Msg.ImgHeightRatio = mLMap_6410_Msg.roomImgH/roomImageHeight/1.00;
    mLMap_6410_Msg.ImgWidthRatio = mLMap_6410_Msg.roomImgW/roomImageWidth/1.00;

    QFileInfo info(currentImg);
    m_pHCStatusBar->setlrName(mLMap_6410_Msg.Layer_Room_Name);

    QDeclarativeItem *Item = item;

    #ifdef Q_OS_LINUX
    QMetaObject::invokeMethod(Item,"setRoomImage",
      Q_ARG(QVariant,"file://"+info.absoluteFilePath()));
    #endif

    deviceElement=roomElement.firstChildElement("device");
    m_deviceElement=deviceElement;    //得到房间里所有设备
    while(!deviceElement.isNull())
    {
        QString subtype = deviceElement.attribute("subtype");
        QString type = deviceElement.attribute("type");
        if(subtype == "var")
            type = "var";
        QString deviceId = deviceElement.attribute("id");
        deviceShapeElement=deviceElement.firstChildElement("point");
        x = deviceShapeElement.attribute("x").toInt()*mLMap_6410_Msg.ImgWidthRatio;
        y = deviceShapeElement.attribute("y").toInt()*mLMap_6410_Msg.ImgHeightRatio;
        QMetaObject::invokeMethod(Item,"addItem",
                                  Q_ARG(QVariant,x),
                                  Q_ARG(QVariant,y),
                                  Q_ARG(QVariant,deviceId),
                                  Q_ARG(QVariant,type));
        deviceElement=deviceElement.nextSiblingElement("device");
    }
    UpdateDevice();
}


/***********************************************************************************
函数名 : saveHouseMapXmlFile
描  述 : 保存配置文件到本地
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::saveHouseMapXmlFile()
{
    MapXmlMutex.lock();
    QSettings qsettings("home_control/sys.ini",QSettings::IniFormat);
    if(qsettings.value("isFirstLanuch").toBool() == false || qsettings.value("isFirstLanuch").isNull())
    {
        return;
    }

    QFile housemap("home_control/house-map-default1.xml");
    if(!housemap.exists())
    {
        MapXmlMutex.unlock();
        return;
    }
    if (!housemap.open(QIODevice::WriteOnly | QIODevice::Truncate)) return;
    QTextStream stream(&housemap);
    m_domDoc.save(stream, 4);
    housemap.close();
    sync();
    MapXmlMutex.unlock();
}

/***********************************************************************************
函数名 : saveHouseMapXmlFile1
描  述 : 保存配置文件到本地
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::saveHouseMapXmlFile1()
{
    QFile housemap("home_control/house-map-default.xml");
    if (!housemap.open(QIODevice::WriteOnly | QIODevice::Truncate)) return;
    QTextStream stream(&housemap);
    m_domDoc.save(stream, 4);
    housemap.close();
    sync();
}


/***********************************************************************************
函数名 : UpdateDevice
描  述 : 当家居房间改变后则对当前房间进行刷新
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::UpdateDevice()
{
    QDeclarativeItem *Item = item;
    QDomElement roomElement;
    QDomElement deviceElement;
    roomElement = m_layerElement.firstChildElement("room");
    while (!roomElement.isNull())
    {
        if(roomId == roomElement.attribute("id").toShort())
        {
            break;
        }
        roomElement = roomElement.nextSiblingElement("room");
    }
    deviceElement = roomElement.firstChildElement("device");
    while (!deviceElement.isNull())
    {
        int devId = deviceElement.attribute("id").toInt();
        QString power = deviceElement.attribute("power");
        QString type = deviceElement.attribute("type");
        if(deviceElement.attribute("subtype") == "var")
        {
            type = "var";
        }
        if(type == "ac" || type == "tv" || type == "music")
        {
            if(power == "off")
            {
                if(type == "ac")
                    m_pAccontrolDlg->isPowerOn = false;
                else if(type == "tv")
                    m_pTVControlDlg->tvIsPowerOn = false;
                else
                    m_pMusicControlDlg->musicIsPowerOn = false;
                QMetaObject::invokeMethod(Item,"hideDevIcon",
                                          Q_ARG(QVariant,type),
                                          Q_ARG(QVariant,QString::number(devId)));
            }
            else
            {
                if(type == "ac")
                    m_pAccontrolDlg->isPowerOn = true;
                else if(type == "tv")
                    m_pTVControlDlg->tvIsPowerOn = true;
                else
                    m_pMusicControlDlg->musicIsPowerOn = true;
                QMetaObject::invokeMethod(Item,"showDevIcon",
                                          Q_ARG(QVariant,type),
                                          Q_ARG(QVariant,QString::number(devId)));
            }
        }
        else
        {
            if(power == "off")
            {
                QMetaObject::invokeMethod(Item,"hideDevIcon",
                                          Q_ARG(QVariant,type),
                                          Q_ARG(QVariant,QString::number(devId)));
            }
            else
            {
                QMetaObject::invokeMethod(Item,"showDevIcon",
                                          Q_ARG(QVariant,type),
                                          Q_ARG(QVariant,QString::number(devId)));
            }
        }
        deviceElement = deviceElement.nextSiblingElement("device");
    }
    saveHouseMapXmlFile();
}

/***********************************************************************************
函数名 : homeControlReceive
描  述 : 对家居界面进行刷新操作,设备的开关等
输入参数说明 :
p_sock:暂时没用
buf:要解析的数据
len:长度
strtype:设备类型,若该字符串为空则表示数据是从客户端发过来
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
int  HomeControlMainDlg::homeControlReceive(NetRecvClientSocket *p_sock, char *buf, int len, QString strType)
{
    m_pChangDeviceData->TranslateDeviceCmd(buf);
    return  0;
}

/***********************************************************************************
函数名 : setSecurityStatus
描  述 : 终端机进行布撤防,主要针对远程的操作
输入参数说明 :stat为1进入布防状态为0进入撤防状态
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::setSecurityStatus(int stat)
{
    MyMsgLog<<"setSecurityStatus";
    hide();
    ((Application *)qApp)->setLCDAndTimerOn();
     g_pMainDlg->m_pSecurityMainDlg->show();
     QString strWaveFile;
     int status;
     if (stat == 0x01)
     {
         status = STATUS_ATHOME;
         strWaveFile = "athome";
     }
     else
     {
         status = STATUS_DISARM;
         strWaveFile = "disarm";
     }

     if (g_pMainDlg->m_inputMethodMode == 0)
     {
         strWaveFile += "-en";
     }

     strWaveFile = "wav/" + strWaveFile + ".wav";
     playWaveFile(strWaveFile.toLocal8Bit().data(), g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);

     g_pMainDlg->m_pSecurityMainDlg->setSecurityState(status);
     g_pMainDlg->m_pSecurityMainDlg->m_pSecurityAbnormalDlg->resetState();
     g_pMainDlg->m_pSecurityMainDlg->m_pSecurityAbnormalDlg->show();
}

/***********************************************************************************
函数名 : slot_sceneBuffer
描  述 : 对本地场景数据进行解析,并把场景数据存到缓冲区
输入参数说明 :buf为场景数据,len长度,type为设备类型
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::slot_sceneBuffer(char *buf, int len, QString type)
{
//    phomeControlReceive(NULL, buf, len,type);
//    QByteArray Array;
//    Array.resize(len);
//    memcpy(Array.data(),buf,len);
//    slotRcvFromSvr(Array,"Ter",len);
//    Array.clear();
}

/***********************************************************************************
函数名 : cmdBuffer
描  述 : 终端机把要发送的指令都储存在一个链表里
输入参数说明 :buf为要储存的数据
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::cmdBuffer(char *buf)
{
    uint size = (uint)buf[0];
    QByteArray Array;
    Array.resize(size);
    memcpy(Array.data(),buf,size);
    slotRcvFromSvr(Array,"Ter",size);
    Array.clear();
}

/***********************************************************************************
函数名 : slotRecvHcStatus
描  述 : 对总线发过来的数据进行解析
输入参数说明 :buf为总线数据
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::slotRecvHcStatus(char *buf)
{
    int  size=(int)buf[0];
    if(size<6)
    {
        return;
    }
    int  device_type=buf[3];//设备属性
    if(device_type == 0x10)//按键控制场景
    {
        HomeSceneReceive(buf);
    }
    else  if(device_type == 0x0d)//红外返回
    {
        if(size==6)
        {
            InfraredLearnReceive(buf);
        }
    }
    else   if(device_type==0xbb)//RISCO安防
    {
         RiscoSecurityReceive(buf);
    }
    else   if(device_type==0x12)//终端机安防
    {

    }
    else//设备控制
    {
         phomeControlReceive((NetRecvClientSocket *)-1, buf, size,"");
    }
}

/***********************************************************************************
函数名 : slotRcvFromCloud
描  述 : 对客户端发过来的数据进行解析
输入参数说明 :buf为客户端数据
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::slotRcvFromCloud(char *buf)
{
    phomeControlReceive((NetRecvClientSocket*)-1, buf, (int)buf[0],"");
}


/***********************************************************************************
函数名 : ImageTransform
描  述 : 终端机开机对图片的尺寸进行修改优化,主要是用来自适应屏幕
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::ImageTransform()
{
       QDir *pHomeDir=new QDir("/mnt/disk/home_control");
       QStringList filter;
       filter<<"*.jpg"<<"*.png"<<"*.jpeg";
        pHomeDir->setNameFilters(filter);
        QList<QFileInfo> *fileInfo=new QList<QFileInfo>(pHomeDir->entryInfoList(filter));
        for(int i=0;i<fileInfo->size();i++)
        {
                if(fileInfo->at(i).fileName().left(1) == "m")  {    continue;   }
                MyMsgLog<<fileInfo->at(i).fileName()<<"正在优化中...";
                QString fileName = fileInfo->at(i).fileName();
                QString mroomImg = "home_control/m" + fileName;
                QString roomImg = "home_control/" + fileName;
                QFile pFile(mroomImg);
                if(pFile.exists()) { continue; }
                QPixmap pRawPixmap(roomImg);
                 pRawPixmap.scaled(mLMap_6410_Msg.screenW,mLMap_6410_Msg.screenH).save(mroomImg,"jpg");
         }
        delete pHomeDir;
        delete fileInfo;
}

/***********************************************************************************
函数名 : setMainFace
描  述 : 判断屏保后是否进入家居界面
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::setMainFace()
{
    QSettings qsettings("home_control/config.ini",QSettings::IniFormat);
    if(qsettings.value("isHomepage").toBool() == false||qsettings.value("isHomepage").isNull())
    {
        hide();
    }
    else
    {
        show();
    }
}


/***********************************************************************************
函数名 : setloading
描  述 : 主要是为了等待效果显示
输入参数说明 :若参数为true则转圈圈
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::setloading(bool true_false)
{
    QMetaObject::invokeMethod(item,"setLoading",Q_ARG(QVariant, true_false));
}

/***********************************************************************************
函数名 : setStatusBarmsg
描  述 : 设置状态栏信息
输入参数说明 :msg为要显示的信息
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::setStatusBarmsg(QString msg)
{
    QObject *statusBarItem = item->findChild<QObject*>("statusBar");
    QMetaObject::invokeMethod(statusBarItem,"setconnectState",
                              Q_ARG(QVariant,msg));
}

/***********************************************************************************
函数名 : showToast
描  述 : 提醒窗口
输入参数说明 :msg为要提醒的信息
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::showToast(QString msg)
{
    QMetaObject::invokeMethod(item,"showToast",Q_ARG(QVariant, msg));
}


/***********************************************************************************
函数名 : screenheight
描  述 : 活取屏幕高度
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
int HomeControlMainDlg::screenheight()
{
    return mLMap_6410_Msg.screenH;
}

/***********************************************************************************
函数名 : screenwidth
描  述 : 活取屏幕宽度
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
int HomeControlMainDlg::screenwidth()
{
    return mLMap_6410_Msg.screenW;
}

/*qreal HomeControlMainDlg::screenPerDot()
{
    return mPhoneMsg.DotPerInch;
}

qreal HomeControlMainDlg::screenPerInch()
{
    return mPhoneMsg.screenPerInch;
}

short HomeControlMainDlg::netState()
{
    return mPhoneMsg.LinClound_Terminal_NET;
}

qreal HomeControlMainDlg::minScreenHight()   //最小分辨率小于这个分辨率
{
    return 420;
}*/

/***********************************************************************************
函数名 : Translate
描  述 : 进文字进行国际化
输入参数说明 :
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::Translate()    //国际化
{
    MyMsgLog<<"Bryan->msg:internationalization";
    m_pAccontrolDlg->Translate();
    m_pLightControlDlg->Translate();
    m_pCurtainControlDlg->Translate();
    m_pTVControlDlg->Translate();
    m_pHomeControlSceneDlg->Translate();
    m_pMusicControlDlg->Translate();
    m_pHCStatusBar->Translate();
    m_pIrpasswd->Translate();
    m_pHCSceneEdit->Translate();
    m_pHCRoomSelect->Translate();
}


/***********************************************************************************
函数名 : setMainState
描  述 : 从C++中启动qml中的某个界面
输入参数说明 :
state{ac,music,curtain,tv,light,scene,sceneEdit,roomselect,irpasswd,irlearndlg,setting}
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::setMainState(QString state)
{
    QMetaObject::invokeMethod(item,"setState",Q_ARG(QVariant, state));
}

/***********************************************************************************
函数名 : UpdateXml
描  述 :对全局的xml文件进行更新
输入参数说明 :cmd为总线返回的数据
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::UpdateXml(char *cmd)
{
    int id1 = cmd[1];
    int id2 = cmd[3];
    int id = (id2<<8)|id1;
    QDomElement houseElement;
    QDomElement roomElement;
    QDomElement layerElement;
    QDomElement devElement;
    houseElement = m_domDoc.firstChildElement("house");
    layerElement = houseElement.firstChildElement("layer");
    while (!layerElement.isNull())
    {
       roomElement = layerElement.firstChildElement("room");
       if(m_phouseMapXmlThread->m_roomId == roomElement.attribute("id"))
           roomElement = roomElement.nextSiblingElement("room");
       while (!roomElement.isNull())
       {
           devElement = roomElement.firstChildElement("device");
           while (!devElement.isNull())
           {
               if(id2 >= 0x01 && id2 <= 0x0c)
               {
                   int devId = devElement.attribute("id").toInt();
                   if(devId == id)
                   {
                       QString attributeName,attributeValue;
                       int stat = cmd[4];   //控制指令
                       int len = (int)cmd[0];
                       if(len == 6)
                       {
                           if(stat==0x00)
                           {
                               attributeName="power";
                               attributeValue="off";
                           }
                           else if(stat==0x01)
                           {
                               attributeName="power";
                               attributeValue="on";
                           }
                       }
                       else if(len == 7)   //表示调光灯
                       {
                           int value = cmd[5];   //亮度值
                           attributeName = "value";
                           attributeValue = QString::number(value);
                       }
                       if(!attributeName.isEmpty() && !attributeValue.isEmpty())
                           devElement.setAttribute(attributeName,attributeValue);
                   }
               }
               devElement = devElement.nextSiblingElement("device");
           }
           roomElement = roomElement.nextSiblingElement("room");
       }
       layerElement = layerElement.nextSiblingElement("layer");
    }
}

/***********************************************************************************
函数名 : slotRcvFromSvr
描  述 : 保存远程数据，本地数据
输入参数说明 :
SvrIP
{
1、Ter表示终端机触发的数据，2、若是IP则表示从网络发过来的数据
}
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::slotRcvFromSvr(QByteArray buf,QString SvrIP,short size)
{
    m_svrDataMutex.lock();
    uint sum = 0;
    for(int i=0;i<size-1;i++)
    {
        sum += (uint)buf.data()[i];
    }
    buf.data()[size-1] = sum&0xff;
    m_pMyQByteArray->PushToLeeBus(buf,SvrIP,"Append");
    int  tmp = buf.data()[1];
    buf.data()[1] = buf.data()[2];
    buf.data()[2] = tmp;
    m_pMyQByteArray->SvrData(buf,SvrIP,"Append");
    m_svrDataMutex.unlock();
}

void HomeControlMainDlg::phomeControlReceive(NetRecvClientSocket *p_sock, char *buf, int len, QString strType)
{
    //m_receiveDeviceCmdMutex.lock();
    homeControlReceive(NULL,buf,len,strType);
    //m_receiveDeviceCmdMutex.unlock();
}

/***********************************************************************************
函数名 : setHCLineText
描  述 : qml中调用外部输入法进行编辑
输入参数说明 :
    msg：要修改的名称
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::setHCLineText(QString msg)
{
    HCLineEdit->setText(msg);
    HCLineEdit->mousePressEvent(NULL);
}

/***********************************************************************************
函数名 : setHCLineText
描  述 : 当文本中msg变换时则调用此函数来把变化后的信息显示到控件上
输入参数说明 :
    msg：要修改的名称
输出参数说明 :
返 回 值:
其它说明:
***********************************************************************************/
void HomeControlMainDlg::slot_textChanged(QString msg)
{
    if(msg.isEmpty())
        return;
    if(mLIN_ExceptionSvr == LIN_NotAnything)
        return;
    else if(mLIN_ExceptionSvr == LIN_CloudSvrIP)
    {
        QSettings setings(home_control_config,QSettings::IniFormat);
        setings.setValue(LIN_CLOUD_IP,msg);
        m_pHCSetting->setlin_CloudIP(msg);
    }
    else if(mLIN_ExceptionSvr == LIN_CloudSvrAccount)
    {
        QSettings setings(home_control_config,QSettings::IniFormat);
        setings.setValue(LIN_CLOUD_Acc,msg);
        m_pHCSetting->setlin_CloudAcc(msg);
    }
    else if(mLIN_ExceptionSvr == LIN_CloudSvrID)
    {
        QSettings setings(home_control_config,QSettings::IniFormat);
        setings.setValue(LIN_CLOUD_ID,msg);
        m_pHCSetting->setlin_LocalID(msg);
    }
    else if(mLIN_ExceptionSvr == LIN_CloudSvrPassWord)
    {
        QSettings setings(home_control_config,QSettings::IniFormat);
        setings.setValue(LIN_CLOUD_PWD,msg);
        m_pHCSetting->setlin_CloudPwd(msg);
    }
    else if(mLIN_ExceptionSvr == LIN_TerSvrAccount)
    {
        QSettings setings(home_control_config,QSettings::IniFormat);
        setings.setValue(Ter_Account,msg);
        m_pHCSetting->setTerAccount(msg);
    }
    else if(mLIN_ExceptionSvr == LIN_TerSvrPassWord)
    {
        QSettings setings(home_control_config,QSettings::IniFormat);
        setings.setValue(Ter_Pwd,msg);
        m_pHCSetting->setTerPwd(msg);
    }
    else if(mLIN_ExceptionSvr == LIN_TerSceneFir)
    {
        QSettings settings(home_control_config,QSettings::IniFormat);
        settings.setValue("sceneBtn1",msg);
        m_pHomeControlSceneDlg->setS1text(msg);
        m_pHomeControlSceneDlg->setSceneName(0,"name",msg);
    }
    else if(mLIN_ExceptionSvr == LIN_TerSceneSec)
    {
        QSettings settings(home_control_config,QSettings::IniFormat);
        settings.setValue("sceneBtn2",msg);
        m_pHomeControlSceneDlg->setS2text(msg);
        m_pHomeControlSceneDlg->setSceneName(1,"name",msg);
    }
    else if(mLIN_ExceptionSvr == LIN_TerSceneThird)
    {
        QSettings settings(home_control_config,QSettings::IniFormat);
        settings.setValue("sceneBtn3",msg);
        m_pHomeControlSceneDlg->setS3text(msg);
        m_pHomeControlSceneDlg->setSceneName(2,"name",msg);
    }
    else if(mLIN_ExceptionSvr == LIN_TerSceneFour)
    {
        QSettings settings(home_control_config,QSettings::IniFormat);
        settings.setValue("sceneBtn4",msg);
        m_pHomeControlSceneDlg->setS4text(msg);
        m_pHomeControlSceneDlg->setSceneName(3,"name",msg);
    }
    else if(mLIN_ExceptionSvr == LIN_TerSceneFive)
    {
        QSettings settings(home_control_config,QSettings::IniFormat);
        settings.setValue("sceneBtn5",msg);
        m_pHomeControlSceneDlg->setS5text(msg);
        m_pHomeControlSceneDlg->setSceneName(4,"name",msg);
    }
    else if(mLIN_ExceptionSvr == LIN_TerSceneSix)
    {
        QSettings settings(home_control_config,QSettings::IniFormat);
        settings.setValue("sceneBtn6",msg);
        m_pHomeControlSceneDlg->setS6text(msg);
        m_pHomeControlSceneDlg->setSceneName(5,"name",msg);
    }
    else if(mLIN_ExceptionSvr == LIN_DeveloperPwdText)
    {
        m_pParaStyle->setUserCode(msg);
    }
    else if(mLIN_ExceptionSvr == LIN_DeveloperDevAddr)
    {
        m_pParaStyle->setDevAddr(msg);
    }
    else if(mLIN_ExceptionSvr == LIN_DeveloperDevName)
    {
        m_pParaStyle->setDevName(msg);
    }
}

void  HomeControlMainDlg::slot_getAlarmData(int num,int count,QString time,int number,int property,int channel,int name,int type,QString location,QString path)
{
    qDebug()<<num<<count<<property<<type<<time<<number<<name<<location<<path<<channel;
    QByteArray   qTime=time.toLatin1();
    int  timeLen=qTime.size();
    QByteArray   qLocation=location.toUtf8();
    int  locationLen=qLocation.size();
    QByteArray   qPath=path.toLatin1();
    int  pathLen=qPath.size();

    QByteArray  alarmData;
    alarmData.resize(timeLen+locationLen+pathLen+15);
    alarmData.data()[0]=(timeLen+locationLen+pathLen+15)&0xff;
    alarmData.data()[1]=0x00;
    alarmData.data()[2]=0x00;
    alarmData.data()[3]=0x12;
    alarmData.data()[4]=num&0xff;
    alarmData.data()[5]=count&0xff;
    alarmData.data()[6]=number&0xff;
    alarmData.data()[7]=property&0xff;
    alarmData.data()[8]=channel&0xff;
    alarmData.data()[9]=name&0xff;
    alarmData.data()[10]=type&0xff;

    alarmData.data()[11]=timeLen&0xff;
    for(int i=1;i<=timeLen;i++)
    {
        alarmData.data()[11+i]=qTime.data()[i-1];
    }

    alarmData.data()[12+timeLen]=locationLen&0xff;
    for(int i=1;i<=locationLen;i++)
    {
        alarmData.data()[12+timeLen+i]=qLocation.data()[i-1];
    }

    alarmData.data()[13+timeLen+locationLen]=pathLen&0xff;
    for(int i=1;i<=pathLen;i++)
    {
        alarmData.data()[13+timeLen+locationLen+i]=qPath.data()[i-1];
    }

    int   sum=0;
    for(int i=0;i<timeLen+locationLen+pathLen+14;i++)
    {
         sum+=(int)alarmData.data()[i];
    }
    alarmData.data()[timeLen+locationLen+pathLen+14]=sum&0xff;

    m_pMyQByteArray->RcvLeeBus(alarmData,"leebus","Append");
}

void HomeControlMainDlg::show()
{
    menu->showhomepageDialog();
    smartHomeView->show();
}

void HomeControlMainDlg::showFullScreen()
{
    view->showFullScreen();
}

void HomeControlMainDlg::hide()
{
    if(smartHomeView->isVisible())
    {
        if(g_pMainDlg->m_pInputMethodWidget->isVisible())
        {
            g_pMainDlg->m_pInputMethodWidget->ui->InputInfoTextEdit->clear();
            g_pMainDlg->m_pInputMethodWidget->close();
        }
        smartHomeView->hide();
        if(true==m_pLoadingDataToUI->isRunning()||true==m_pLoadingDataToUI->m_isRunning)
        {
            m_pLoadingDataToUI->m_isRunning=false;
            slot_hideLoadingLabel();
            sleep(1);
        }
        menu->showhomepageDialog();
    }

    if (g_pMainDlg->m_pCurActiveDlg == g_pMainDlg && smartHomeView->isVisible())
    {
        g_pMainDlg->ui->pushButtonHomeControl->setFocus(Qt::TabFocusReason);
    }
}

bool HomeControlMainDlg::isVisible()
{ 
    return  smartHomeView->isVisible();
}

void   HomeControlMainDlg::Init()                     //初始化
{
    connect(g_pMainDlg->m_pSecurityMainDlg->m_pSerial1Thread, SIGNAL(openRecorder(int,int,QString,int,int,int,int,int,QString,QString)),
    this,SLOT(slot_getAlarmData(int,int,QString,int,int,int,int,int,QString,QString)));
    connect(g_pMainDlg->m_pSecurityMainDlg,SIGNAL(sigSetSecurityStatueToPhone(int)),this,SLOT(slot_security(int)));
    connect(g_pMainDlg->m_pSysSetEngineerDlg->m_pAddressSetDlg,SIGNAL(sigIpHasChange()),this,SLOT(slot_openListen()));

    musicInfo.musicInfraredMode<<tr("mp3")<<tr("cd")<<tr("aux")<<tr("radio");
    musicInfo.musicBackMode<<tr("dvd")<<tr("fm1")<<tr("mp3")<<tr("aux")<<tr("fm2")<<tr("ipod")<<tr("cloud");

    acInfo.acMode<<tr("自动")<<tr("制冷")<<tr("除湿")<<tr("通风")<<tr("加热");
    acInfo.acModeEn<<tr("auto")<<tr("refri")<<tr("dehum")<<tr("venti")<<tr("heat");
    acInfo.acInfraredSpeed<<tr("自动")<<tr("低速")<<tr("中速")<<tr("高速");
    acInfo.acInfraredSpeedEn<<tr("auto")<<tr("low")<<tr("medium")<<tr("high");

    acInfo.acCentralSpeed<<tr("自动")<<tr("超低速")<<tr("低速")<<tr("中速")<<tr("高速")<<tr("超高速");
    acInfo.acCentralSpeedEn<<tr("auto")<<tr("ultra-low")<<tr("low")<<tr("medium")<<tr("high")<<tr("ultra-high") ;
    acInfo.acCentralWind<<tr("风向1")<<tr("风向2")<<tr("风向3")<<tr("风向4")<<tr("风向5")<<tr("摆风");
    acInfo.acCentralWindEn<<tr("P0")<<tr("P1")<<tr("P2")<<tr("P3")<<tr("P4")<<tr("on");

    deviceTypes.type<<tr("空调( 红外)")<<tr("中央空调")<<tr("音乐( 红外)")<<tr("泊声音乐")<<tr("电视( 红外)")<<tr("窗帘")<<tr("场景")<<tr("可调灯")<<tr("不可调灯")<<tr("人体红外")<<tr("温湿度")<<tr("PM2.5")<<tr("自定义");
    deviceTypes.typeEn<<tr("ac")<<tr("ac")<<tr("music")<<tr("music")<<tr("tv")<<tr("curtain")<<tr("scene")<<tr("var")<<tr("light")<<tr("humansensor")<<tr("humiture")<<tr("pm")<<tr("custom");

    OpenDataBase();
    engines.clear();
    m_pHomeScene->InitHomeScene();
    m_pHomeScene->InitRoomScene();
    m_pManageHomeData->LoadAllUid();
    findDevState();
    findRiscoState();
    m_pRcvDataThread->start(QThread::LowestPriority);
    m_checkThreadTimer->start(4000);

    smartHomeView=new   QDeclarativeView;
   //smartHomeView->rootContext()->setContextProperty("smartHome",this);
    smartHomeView->setSource(QUrl("qrc:/QML/main_interface_dialog/LuxDomoSmartHomeDialog.qml"));
    smartHomeItem= qobject_cast<QDeclarativeItem*>(smartHomeView->rootObject());
    smartHomeItem->setWidth(mLMap_6410_Msg.screenW);
    smartHomeItem->setHeight(mLMap_6410_Msg.screenH);
    smartHomeView->setParent(g_pMainDlg);
    smartHomeView->setWindowFlags(Qt::FramelessWindowHint);
    smartHomeView->setWindowFlags(Qt::WindowStaysOnBottomHint);
    CreateElement("qrc:/QML/main_interface_dialog/LuxDomoMainInterfaceDialog.qml");
    CreateElement("qrc:/QML/main_interface_dialog/LuxDomoThemebarDialog.qml");
    CreateElement("qrc:/QML/setting_dialog/LuxDomoSettingDialog.qml");
    CreateElement("qrc:/QML/main_interface_dialog/LuxDomoMenuDialog.qml");

    acItem=CreateElement("qrc:/QML/device_control_dialog/LuxDomoAcControlDialog.qml");
    musicItem=CreateElement("qrc:/QML/device_control_dialog/LuxDomoMusicControlDialog.qml");
    tvItem=CreateElement("qrc:/QML/device_control_dialog/LuxDomoTVControlDialog.qml");
    curtainItem=CreateElement("qrc:/QML/device_control_dialog/LuxDomoCurtainControlDialog.qml");
    varItem=CreateElement("qrc:/QML/device_control_dialog/LuxDomoLightControlDialog.qml");
    sensorItem=CreateElement("qrc:/QML/device_control_dialog/LuxDomoSensorInformationDialog.qml");

    QDeclarativeEngine  *engine = new QDeclarativeEngine(smartHomeItem);
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/main_interface_dialog/LuxDomoLoadingLabelDialog.qml"));
    m_pLoadingLabel = qobject_cast<QDeclarativeItem*>(component.create());
    m_pLoadingLabel->setParentItem(smartHomeItem);
    m_pLoadingLabel->setWidth(mLMap_6410_Msg.screenW);
    m_pLoadingLabel->setHeight(mLMap_6410_Msg.screenH);

    operateWlan=InitWifi;
    m_pDealWlanData->start();
    m_pCheckWlan->start();
    mainIterface->Init();
    smartHomeView->hide();
}

QDeclarativeItem*   HomeControlMainDlg::CreateElement(QString  strFile )
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(smartHomeItem);
    QDeclarativeComponent component(engine, QUrl(strFile));
    QDeclarativeItem *createItem = qobject_cast<QDeclarativeItem*>(component.create());
    createItem->setParentItem(smartHomeItem);
    return   createItem;
}

void   HomeControlMainDlg::CreateRoomControl()
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(smartHomeItem);
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/home_control_dialog/LuxDomoRoomControlDialog.qml"));
    QDeclarativeItem *createItem = qobject_cast<QDeclarativeItem*>(component.create());
    createItem->setParentItem(smartHomeItem);
    engines.insert(tr("roomControl"),engine);
}

void  HomeControlMainDlg::CreateHomeManage()
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(smartHomeItem);
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/home_manage_dialog/LuxDomoHomeManageDialog.qml"));
    QDeclarativeItem *createItem = qobject_cast<QDeclarativeItem*>(component.create());
    createItem->setParentItem(smartHomeItem);
    engines.insert(tr("homeManage"),engine);
}

void  HomeControlMainDlg::CreateRoomManage()
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(smartHomeItem);
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/home_manage_dialog/LuxDomoRoomManageDialog.qml"));
    QDeclarativeItem *createItem = qobject_cast<QDeclarativeItem*>(component.create());
    createItem->setParentItem(smartHomeItem);
    engines.insert(tr("roomManage"),engine);
}

void  HomeControlMainDlg::CreateSecurity()
{
    QDeclarativeEngine  *engine = new QDeclarativeEngine(smartHomeItem);
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/security_dialog/LuxDomoRiscoDialog.qml"));
    QDeclarativeItem *createItem = qobject_cast<QDeclarativeItem*>(component.create());
    createItem->setParentItem(smartHomeItem);
    engines.insert(tr("security"),engine);
}

void   HomeControlMainDlg::LoadConfigurationFile(QString strFile)
{
       QFile file(strFile);
       if(!file.exists())
        {
           return;
        }
       if (!file.open(QIODevice::ReadOnly))
       {
             return;
        }
       if (!m_pHome.setContent(&file))
       {
            MyMsgLog<<"the"<<strFile<<"openfailed";
            file.close();
            return;
       }
       file.close();
}

void  HomeControlMainDlg::slot_varValue()
{
    MyMsgLog<<"timer.....stop";
    m_varValue=false;
    timer->stop();
}

void   HomeControlMainDlg::slot_musicVolume()
{
    MyMsgLog<<"timer1......stop";
    m_musicVolume=false;
    timer1->stop();
}

void  HomeControlMainDlg::slot_editData(QString  data)
{
    if(m_editName==data)
    {
        return;
    }

    if(dataType==RoomName)
    {
          m_pHomeManage->EditRoomName(data);
    }
    else  if(dataType==FolderName)
    {
          roomControl->EditFolderName(data);
    }
    else  if(dataType==DeviceName||dataType==DeviceOtherName)
    {
          m_pRoomManage->setDeviceInforMation(data);
    }
    else   if(dataType==TerminaiAccountNumber||dataType==TerminaiPasswd||dataType==CloudID||dataType==FeedbackMessage
              ||dataType==WifiPasswd||dataType==WifiDns||dataType==WifiGateway||dataType==WifiIp||dataType==WifiNetmask)
    {
          setting->SetInformation(data);
    }
}

QString   HomeControlMainDlg::GetControlImage(QString  type, QString  on_off)
{
           QString   strImage("");

           if(tr("room")==type)
           {
               int   number=rand()%4+1;
               strImage=tr("../main_interface/room")+QString::number(number)+tr(".png");
           }
           else  if(tr("scene")==type)
           {
               if(tr("on")==on_off)  {  strImage=tr("../room_control/scene_on.png"); }
               else if(tr("off")==on_off)  {  strImage=tr("../room_control/scene_on.png"); }
           }
           else   if(tr("tv")==type)
           {
               if(tr("on")==on_off)  {  strImage=tr("../room_control/tv_on.png"); }
               else if(tr("off")==on_off)  {  strImage=tr("../room_control/tv_off.png"); }
           }
           else   if(tr("ac")==type)
           {
               if(tr("on")==on_off)  {  strImage=tr("../room_control/ac_on.png"); }
               else if(tr("off")==on_off)  {  strImage=tr("../room_control/ac_off.png"); }
           }
           else   if(tr("curtain")==type)
           {
               if(tr("on")==on_off||on_off==tr("stop"))  {  strImage=tr("../room_control/curtain_on.png"); }
               else if(tr("off")==on_off)  {  strImage=tr("../room_control/curtain_off.png"); }
           }
           else    if(tr("var")==type)
           {
               if(tr("on")==on_off)  {  strImage=tr("../room_control/var_on.png"); }
               else if(tr("off")==on_off)  {  strImage=tr("../room_control/var_off.png"); }
           }
           else    if(tr("light")==type)
           {
               if(tr("on")==on_off)  {  strImage=tr("../room_control/light_on.png"); }
               else if(tr("off")==on_off)  {  strImage=tr("../room_control/light_off.png"); }
           }
           else   if(tr("music")==type)
           {
               if(tr("on")==on_off)  {  strImage=tr("../room_control/music_on.png"); }
               else if(tr("off")==on_off)  {  strImage=tr("../room_control/music_off.png"); }
           }
           else   if(tr("humiture")==type)
           {
               if(tr("on")==on_off)  {  strImage=tr("../room_control/humiture_on.png"); }
               else if(tr("off")==on_off)  {  strImage=tr("../room_control/humiture_on.png"); }
           }
           else   if(tr("pm")==type)
           {
               if(tr("on")==on_off)  {  strImage=tr("../room_control/pm_on.png"); }
               else if(tr("off")==on_off)  {  strImage=tr("../room_control/pm_on.png"); }
           }
           else   if(tr("custom")==type)
           {
               if(tr("on")==on_off)  {  strImage=tr("../room_control/custom_on.png"); }
               else if(tr("off")==on_off)  {  strImage=tr("../room_control/custom_off.png"); }
           }
           else   if(tr("humansensor")==type)
           {
               if(tr("on")==on_off)  {  strImage=tr("../room_control/humansensor_on.png"); }
               else if(tr("off")==on_off)  {  strImage=tr("../room_control/humansensor_off.png"); }
           }
           return   strImage;
}

QStringList   HomeControlMainDlg::GetSceneImage(QString  strFile)
{
      QStringList   data;
      data.clear();

      if(tr("house-map-user-2.xml")==strFile)
      {
           data<<tr("../main_interface/inhomeScene.png")<<tr("在家模式");
      }
      else  if(tr("house-map-user-6.xml")==strFile)
      {
           data<<tr("../main_interface/outhomeScene.png")<<tr("外出模式");
      }
      else  if(tr("house-map-user-3.xml")==strFile)
      {
           data<<tr("../main_interface/getupScene.png")<<tr("起床模式");
      }
      else  if(tr("house-map-user-1.xml")==strFile)
      {
           data<<tr("../main_interface/sleepScene.png")<<tr("睡眠模式");
      }
      else   if(tr("house-map-user-4.xml")==strFile)
      {
           data<<tr("../main_interface/entertainmentScene.png")<<tr("娱乐模式");
      }
      else   if(tr("house-map-user-7.xml")==strFile)
      {
           data<<tr("../main_interface/relaxationScene.png")<<tr("会客模式");
      }
      else   if(tr("house-map-user-5.xml")==strFile)
      {
          data<<tr("../main_interface/filmScene.png")<<tr("影音模式");
      }
      else   if(tr("house-map-user-8.xml")==strFile)
      {
          data<<tr("../main_interface/eatScene.png")<<tr("就餐模式");
      }
      else    if(tr("house-map-user-on.xml")==strFile)
      {
          data<<tr("../main_interface/allOnScene.png")<<tr("全开");
      }
      else   if(tr("house-map-user-off.xml")==strFile)
      {
          data<<tr("../main_interface/allOffScene.png")<<tr("全关");
      }
      return  data;
}

void   HomeControlMainDlg::saveHomeXmlFile()
{
    MapXmlMutex.lock();
    QFile housemap("home_control/house-map-default1.xml");
    if (!housemap.open(QIODevice::WriteOnly | QIODevice::Truncate))  return;
    QTextStream stream(&housemap);
    m_domDoc.save(stream, 4);
    housemap.close();
    sync();
    QFile::copy("/mnt/disk/home_control/house-map-default1.xml","/mnt/disk/home_control/house-map-default_backups.xml");
    MapXmlMutex.unlock();
}

void  HomeControlMainDlg::OpenDataBase()
{
    m_DataBaseDB = QSqlDatabase::addDatabase("QSQLITE");
    m_DataBaseDB.setDatabaseName(XmlFilePath+tr("LuxDomoIrDataBase.db"));
    m_DataBaseDB.setUserName("root");
    m_DataBaseDB.setPassword("123456");
    if(!m_DataBaseDB.open())
    {
        MyMsgLog<<tr("数据库打开失败");
        return;
    }
}

QString   HomeControlMainDlg::GetAcModeName(QString mode)
{
    QString   acMode(tr(""));
    int size=acInfo.acModeEn.size();
    for(int i=0;i<size;i++)
    {
        if(acInfo.acModeEn.at(i)==mode)
        {
            acMode=acInfo.acMode.at(i);
            break;
        }
        else  if(acInfo.acMode.at(i)==mode)
        {
            acMode=acInfo.acModeEn.at(i);
            break;
        }
    }
    return   acMode;
}

QString  HomeControlMainDlg::GetAcCentralSpeedName(QString speed)
{
    QString  acSpeed(tr(""));
    int  size=acInfo.acCentralSpeedEn.size();
    for(int i=0;i<size;i++)
    {
       if(acInfo.acCentralSpeedEn.at(i)==speed)
       {
           acSpeed=acInfo.acCentralSpeed.at(i);
           break;
       }
       else  if(acInfo.acCentralSpeed.at(i)==speed)
       {
           acSpeed=acInfo.acCentralSpeedEn.at(i);
           break;
       }
    }
    return   acSpeed;
}

QString  HomeControlMainDlg::GetAcCentralWindName(QString wind)
{
    QString   acWind(tr(""));
    int  size=acInfo.acCentralWindEn.size();
    for(int  i=0;i<size;i++)
    {
       if(acInfo.acCentralWindEn.at(i)==wind)
       {
           acWind=acInfo.acCentralWind.at(i);
           break;
       }
       else  if(acInfo.acCentralWind.at(i)==wind)
       {
           acWind=acInfo.acCentralWindEn.at(i);
           break;
       }
    }
    return  acWind;
}

QString  HomeControlMainDlg::GetAcInfraredSpeedName(QString  speed)
{
    QString   acSpeed(tr(""));
    int size=acInfo.acInfraredSpeedEn.size();
    for(int i=0;i<size;i++)
    {
        if(acInfo.acInfraredSpeedEn.at(i)==speed)
        {
            acSpeed=acInfo.acInfraredSpeed.at(i);
            break;
        }
        else if(acInfo.acInfraredSpeed.at(i)==speed)
        {
            acSpeed=acInfo.acInfraredSpeedEn.at(i);
            break;
        }
    }
    return  acSpeed;
}

QString  HomeControlMainDlg::GetDeviceTypeName(QString  type)
{
    int size=deviceTypes.type.size();
    for(int i=0;i<size;i++)
    {
        if(type==deviceTypes.type.at(i))
        {
            return  deviceTypes.typeEn.at(i);
        }

        if(type==deviceTypes.typeEn.at(i))
        {
            return  deviceTypes.type.at(i);
        }
    }
    return tr("");
}

void  HomeControlMainDlg::SetSynchronizationfigurationFile()
{
    QDir   synchronizationDir;
    synchronizationDir.mkdir(XmlFilePath+tr("home_control"));
    synchronizationDir.mkpath(XmlFilePath+tr("home_control/"));
}

void    HomeControlMainDlg::GetSynchronizationfigurationFile()
{
    QSettings  qsettings1(XmlFilePath+tr("version.ini"),QSettings::IniFormat);
    QString      version1=qsettings1.value("version").toString();

    QSettings qsettings2(XmlFilePath+tr("home_control/version.ini"),QSettings::IniFormat);
    QString     version2=qsettings2.value("version").toString();

    if(version1==version2||version1==tr("")||version2==tr(""))
    {
        return;
    }

    QDir  *fileDir = new QDir(XmlFilePath+tr("home_control/"));
    QStringList filter;
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(fileDir->entryInfoList(filter));
    for(int i=0;i<fileInfo->size();i++)         //先删除原有的文件，再复制文件
    {
        QString fileName = fileInfo->at(i).fileName();
        if(fileName == "." || fileName == "..")
        {
            continue;
        }
        QFile  file(XmlFilePath+fileName);
        if(file.exists())
        {
            file.remove();
        }
        QFile::copy(XmlFilePath+tr("home_control/")+fileName,XmlFilePath+fileName);
    }

    delete  fileDir;
    delete  fileInfo;
}

void   HomeControlMainDlg::SynchronizationfigurationFile()
{
    QDir   synchronizationDir(XmlFilePath+tr("home_control"));
    if(synchronizationDir.exists())
    {
         GetSynchronizationfigurationFile();
    }
    else
    {
         SetSynchronizationfigurationFile();
    }
}

void   HomeControlMainDlg::CheckRiscoConfigurationFile()
{
    QFile   file(XmlFilePath+tr("risco-security-default.xml"));
    if(file.exists()||file.size()!=0)
    {
        return;
    }
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    QDomDocument document;
    QString strHeader( "version=\"1.0\" encoding=\"UTF-8\"" );      //创建xml的头
    document.appendChild(document.createProcessingInstruction("xml",strHeader));
    QDomElement root_house = document.createElement("security");
    document.appendChild(root_house);
    QDomElement event = document.createElement("event");
    root_house.appendChild(event);
    QDomElement information = document.createElement("information");
    root_house.appendChild(information);
    QDomElement output = document.createElement("output");
    for(int i=1;i<=4;i++)
    {
        QDomElement route = document.createElement("route");
        route.setAttribute("number",QString::number(i));
        route.setAttribute("condition",tr("off"));
        output.appendChild(route);
    }
    root_house.appendChild(output);
    QTextStream stream( &file );
    document.save( stream, 4 );
    file.close();
}

void  HomeControlMainDlg::CheckVersionConfigurationFile()
{
    QSettings  qsettings1(XmlFilePath+tr("version.ini"),QSettings::IniFormat);
    if(qsettings1.value("version").isNull())
    {
        QTime time;
        time= QTime::currentTime();
        qsrand(time.msec()+time.second()*1000);
        int xxx=qrand()%0xffffffff;
        MyMsgLog<<"the srand is"<<xxx;
        qsettings1.setValue("version",xxx);
        MyMsgLog<<qsettings1.value("version").toString();
    }

    QSettings qsettings2("home_control/config.ini",QSettings::IniFormat);
    if(qsettings2.value("isWlan").isNull())
    {
        qsettings2.setValue("isWlan",false);
    }
    if(qsettings2.value("wlanCondition").isNull())
    {
        qsettings2.setValue("wlanCondition",false);
    }
    if(qsettings2.value("isHomepage").isNull())
    {
        qsettings2.setValue("isHomepage",false);
    }
}

void  HomeControlMainDlg::CheckConfigurationFile()
{
    QFile   file(XmlFilePath+tr("house-map-default.xml"));
    if(!file.exists())
    {
        QFile::copy(":/profile/house-map-default.xml",XmlFilePath+tr("house-map-default.xml"));
    }

    QFile  map("/mnt/disk/home_control/house-map-default1.xml");
    if(!map.exists()||map.size()==0)
    {
        QFile::copy("/mnt/disk/home_control/house-map-default.xml","/mnt/disk/home_control/house-map-default1.xml");
    }

    QFile  backups("/mnt/disk/home_control/house-map-default_backups.xml");
    if(!backups.exists()||backups.size()==0)
    {
        QFile::copy("/mnt/disk/home_control/house-map-default1.xml","/mnt/disk/home_control/house-map-default_backups.xml");
    }

    if(map.size()<backups.size())
    {
        QFile::remove("/mnt/disk/home_control/house-map-default1.xml");
        QFile::copy("/mnt/disk/home_control/house-map-default_backups.xml","/mnt/disk/home_control/house-map-default1.xml");
    }
}

void   HomeControlMainDlg::CheckHomeControlFolder()
{
    QDir  dir(tr("/mnt/disk/home_control"));
    if(!dir.exists())
    {
        QDir  homeControlDir;
        homeControlDir.mkdir(tr("/mnt/disk/home_control"));
        homeControlDir.mkpath("/mnt/disk/home_control/");
    }
    else
    {
        MyMsgLog<<"homeControlFolder  exists..............";
    }
}

void   HomeControlMainDlg::UpdateConfigurationFile()
{
    QFile file("home_control/house-map-default1.xml");
    QDomDocument  document;
    int   uid=100;
    if(!file.exists())
    {
          return;
    }
   if (!file.open(QIODevice::ReadOnly))
   {
        return;
    }
    if (!document.setContent(&file))
    {
         file.close();
         return;
    }
    file.close();

    bool   isUpdate=false;
    bool   isDelete=false;
    QDomElement   houseElement;
    QDomElement   layerElement;
    QDomElement   roomElement;
    QDomElement   deviceElement;
    houseElement = document.firstChildElement("house");

    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
          if(false==layerElement.hasAttribute("uid"))
          {
              layerElement.setAttribute("uid",QString::number(uid++));
              isDelete=true;
          }
         roomElement=layerElement.firstChildElement("room");
          while(!roomElement.isNull())
          {
              if(false==roomElement.hasAttribute("uid"))
              {
                  roomElement.setAttribute("uid",QString::number(uid++));
                  isDelete=true;
              }
              deviceElement=roomElement.firstChildElement("device");
              while(!deviceElement.isNull())
              {
                  if(false==deviceElement.hasAttribute("uid"))
                  {
                      deviceElement.setAttribute("uid",QString::number(uid++));
                      isDelete=true;
                  }
                  if(false==deviceElement.hasAttribute("pageIndex"))
                  {
                      deviceElement.setAttribute("deviceIndex",tr("0"));
                      deviceElement.setAttribute("folderName",tr("0"));
                      deviceElement.setAttribute("folderIndex",tr("0"));
                      deviceElement.setAttribute("pageIndex",tr("0"));
                      isUpdate=true;
                  }
                  if(true==deviceElement.hasAttribute("footerIndex")&&true==deviceElement.hasAttribute("pageIndex"))
                  {
                      deviceElement.setAttribute("deviceIndex",tr("0"));
                      deviceElement.setAttribute("folderName",tr("0"));
                      deviceElement.setAttribute("folderIndex",tr("0"));
                      deviceElement.setAttribute("pageIndex",tr("0"));
                      isUpdate=true;
                  }
                  if(true==deviceElement.hasAttribute("footerIndex"))
                  {
                      deviceElement.removeAttribute("footerIndex");
                      isUpdate=true;
                  }
                  if(false==deviceElement.hasAttribute("paly"))
                  {
                      deviceElement.setAttribute("paly",tr("off"));
                      isUpdate=true;
                  }
                  if(false==deviceElement.hasAttribute("source"))
                  {
                      deviceElement.setAttribute("source",tr("mp3"));
                      isUpdate=true;
                  }
                  if(false==deviceElement.hasAttribute("mute"))
                  {
                      deviceElement.setAttribute("mute",tr("off"));
                      isUpdate=true;
                  }
                  if(false==deviceElement.hasAttribute("channel"))
                  {
                      deviceElement.setAttribute("channel",tr("0"));
                      isUpdate=true;
                  }
                  if(false==deviceElement.hasAttribute("musicname"))
                  {
                      deviceElement.setAttribute("musicname",tr(""));
                      isUpdate=true;
                  }
                  if(false==deviceElement.hasAttribute("channelSize"))
                  {
                      deviceElement.setAttribute("channelSize",tr("4"));
                      isUpdate=true;
                  }
                  if(tr("var")==deviceElement.attribute("subtype")&&tr("var")!=deviceElement.attribute("type"))
                  {
                      deviceElement.setAttribute("type",tr("var"));
                      isUpdate=true;
                  }
                  deviceElement=deviceElement.nextSiblingElement("device");
              }
              roomElement=roomElement.nextSiblingElement("room");
          }
          layerElement=layerElement.nextSiblingElement("layer");
    }

    QFile file1("home_control/house-map-default1.xml");
    if(!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return;
    }
    QTextStream stream( &file1 );
    document.save( stream, 4 );
    file1.close();

    if(true==isDelete)
    {
        DeleteConfigurationFile();
    }
    else   if(true==isUpdate)
    {
        UpdateHomeScene();
        UpdateRoomScene();
    }
}

void   HomeControlMainDlg::DeleteConfigurationFile()
{
    QDir  *tempDir = new QDir("home_control/");
    QStringList  filter = (QStringList() << "*.xml");
    tempDir->setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(tempDir->entryInfoList());

    for(int i=0;i<fileInfo->size();i++)
    {
        QString fileName = fileInfo->at(i).fileName();
        if(fileName==tr("house-map-default.xml")||fileName==tr("house-map-default1.xml")||fileName==tr("risco-security-default.xml"))
        {
            continue;
        }
        else
        {
            MyMsgLog<<"(delete file)"<<fileInfo->at(i).fileName();
            QFile::remove(XmlFilePath+fileName);
        }
    }
}

void   HomeControlMainDlg::UpdateHomeScene()
{
    for(int i=1;i<=8;i++)
    {
        QString   strFile=tr("home_control/house-map-user-")+QString::number(i)+tr(".xml");
        MyMsgLog<<"(Updata)"<<strFile;
        QFile       file(strFile);
        QDomDocument  document;
        if(file.exists())
        {
            continue;
        }
        if (!file.open(QIODevice::ReadOnly))
        {
             continue;
         }
        if (!document.setContent(&file))
        {
             file.close();
             continue;
        }
        file.close();

        QDomElement   houseElement;
        QDomElement   layerElement;
        QDomElement   roomElement;
        QDomElement   deviceElement;
        houseElement = document.firstChildElement("house");
        layerElement = houseElement.firstChildElement("layer");
        while(!layerElement.isNull())
        {
            roomElement = layerElement.firstChildElement("room");
            while(!roomElement.isNull())
            {
                 deviceElement=roomElement.firstChildElement("device");
                 while(!deviceElement.isNull())
                 {
                     if(false==deviceElement.hasAttribute("paly"))
                     {
                         deviceElement.setAttribute("paly",tr("off"));
                     }
                     if(false==deviceElement.hasAttribute("source"))
                     {
                         deviceElement.setAttribute("source",tr("mp3"));
                     }
                     if(false==deviceElement.hasAttribute("mute"))
                     {
                         deviceElement.setAttribute("mute",tr("off"));
                     }
                     if(false==deviceElement.hasAttribute("channel"))
                     {
                         deviceElement.setAttribute("channel",tr("0"));
                     }
                     if(false==deviceElement.hasAttribute("musicname"))
                     {
                         deviceElement.setAttribute("musicname",tr(""));
                     }
                     if(false==deviceElement.hasAttribute("channelSize"))
                     {
                         deviceElement.setAttribute("channelSize",tr("4"));
                     }
                     if(tr("var")==deviceElement.attribute("subtype")&&tr("var")!=deviceElement.attribute("type"))
                     {
                         deviceElement.setAttribute("type",tr("var"));
                     }
                     deviceElement=deviceElement.nextSiblingElement("device");
                 }
                 roomElement=roomElement.nextSiblingElement("room");
            }
            layerElement=layerElement.nextSiblingElement("layer");
        }

        QFile file1(strFile);
        if(!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            return;
        }
        QTextStream stream( &file1 );
        document.save( stream, 4 );
        file1.close();
    }
}

void  HomeControlMainDlg::UpdateRoomScene()
{
    QFile file(tr("home_control/house-map-users.xml"));
    QDomDocument  document;
    if(!file.exists())
    {
          return;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
         return;
     }
    if (!document.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement   houseElement;
    QDomElement   layerElement;
    QDomElement   roomElement;
    QDomElement   sceneElement;
    QDomElement   deviceElement;
    houseElement = document.firstChildElement("house");
    layerElement = houseElement.firstChildElement("layer");
    layerElement = houseElement.firstChildElement("layer");
    while(!layerElement.isNull())
    {
        roomElement = layerElement.firstChildElement("room");
        while(!roomElement.isNull())
        {
            sceneElement=roomElement.firstChildElement("scene");
            while(!sceneElement.isNull())
            {
                deviceElement=sceneElement.firstChildElement("device");
                while(!deviceElement.isNull())
                {
                    if(false==deviceElement.hasAttribute("paly"))
                    {
                        deviceElement.setAttribute("paly",tr("off"));
                    }
                    if(false==deviceElement.hasAttribute("source"))
                    {
                        deviceElement.setAttribute("source",tr("mp3"));
                    }
                    if(false==deviceElement.hasAttribute("mute"))
                    {
                        deviceElement.setAttribute("mute",tr("off"));
                    }
                    if(false==deviceElement.hasAttribute("channel"))
                    {
                        deviceElement.setAttribute("channel",tr("0"));
                    }
                    if(false==deviceElement.hasAttribute("musicname"))
                    {
                        deviceElement.setAttribute("musicname",tr(""));
                    }
                    if(false==deviceElement.hasAttribute("channelSize"))
                    {
                        deviceElement.setAttribute("channelSize",tr("4"));
                    }
                    if(tr("var")==deviceElement.attribute("subtype")&&tr("var")!=deviceElement.attribute("type"))
                    {
                        deviceElement.setAttribute("type",tr("var"));
                    }
                    deviceElement=deviceElement.nextSiblingElement("device");
                 }
                 sceneElement=sceneElement.nextSiblingElement("scene");
            }
            roomElement=roomElement.nextSiblingElement("room");
         }
         layerElement=layerElement.nextSiblingElement("layer");
    }

    QFile file1(tr("home_control/house-map-users.xml"));
    if(!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return;
    }
    QTextStream stream( &file1 );
    document.save( stream, 4 );
    file1.close();
}

void   HomeControlMainDlg::InfraredLearnReceive(char * buf)
{
    QByteArray  data;
    data.clear();
    if((int)buf[4]==16)
    {
        if(m_pRoomManage!=NULL)
        {
            emit   sigRefreshInfraredLearnInterface(data);
        }
    }
}

void   HomeControlMainDlg::HomeSceneReceive(char *buf)
{
    int   vlaue=buf[2];
    switch (vlaue) {
    case 1:
        m_pApplyScene->m_strFile=tr("home_control/")+tr("house-map-user-1.xml");
        break;
    case 2:
        m_pApplyScene->m_strFile=tr("home_control/")+tr("house-map-user-2.xml");
        break;
    case 3:
        m_pApplyScene->m_strFile=tr("home_control/")+tr("house-map-user-3.xml");
        break;
    case 4:
        m_pApplyScene->m_strFile=tr("home_control/")+tr("house-map-user-4.xml");
        break;
    case 5:
        m_pApplyScene->m_strFile=tr("home_control/")+tr("house-map-user-5.xml");
        break;
    case 6:
        m_pApplyScene->m_strFile=tr("home_control/")+tr("house-map-user-6.xml");
        break;
    case 7:
        m_pApplyScene->m_strFile=tr("home_control/")+tr("house-map-user-7.xml");
        break;
    case 8:
        m_pApplyScene->m_strFile=tr("home_control/")+tr("house-map-user-8.xml");
        break;
    default:
        m_pApplyScene->m_strFile=tr("");
        break;
    }
    if(m_pApplyScene->isRunning())
    {
        return;
    }
    MyMsgLog<<m_pApplyScene->m_strFile;
    operatingType=HomeScenePlaying;
    m_pApplyScene->start();
}

void   HomeControlMainDlg::RiscoSecurityReceive(char *buf)
{
    int   type=(int)buf[4];
    if(0xbe==type)
    {
        m_pRiscoSecurityData->DealRiscoSecurityInformation(buf);
    }
    else  if(0xbb==type)
    {
        m_pRiscoSecurityData->DealDefenceArea(buf);
    }
    else  if(0xbc==type)
    {
        m_pRiscoSecurityData->DealPartition(buf);
    }
    else  if(0xbd==type)
    {
        m_pRiscoSecurityData->DealControl(buf);
    }
}

void  HomeControlMainDlg::slot_hideLoadingLabel()
{
    m_pLoadingLabel->hide();
    m_pLoadingLabel->setVisible(false);
}

void  HomeControlMainDlg::ShowHintInformation(QString  data)
{
    if(m_closeHintTimer->isActive())
    {
        return;
    }
    m_closeHintTimer->start(1800);
    m_pHintEngine=new QDeclarativeEngine(smartHomeItem);
    QDeclarativeComponent component(m_pHintEngine, QUrl("qrc:/QML/content/LuxDomoHintInformation.qml"));
    m_pHint=qobject_cast<QDeclarativeItem*>(component.create());
    m_pHint->setParentItem(smartHomeItem);
    m_pHint->setProperty("textName",data);
    m_pHint->setWidth(250);
    m_pHint->setHeight(60);
    m_pHint->setX(mLMap_6410_Msg.screenW/2-125);
    m_pHint->setY(mLMap_6410_Msg.screenH);
    m_pHint->setProperty("screenH",mLMap_6410_Msg.screenH);
}

void  HomeControlMainDlg::slot_checkThread()
{
    if(!m_pRcvDataThread->isRunning())
    {
        MyMsgLog<<"RcvDataThread  notRunning";
        m_pRcvDataThread->start(QThread::LowestPriority);
    }
    else
    {
        MyMsgLog<<"RcvDataThread  isRunning";
    }
}

void  HomeControlMainDlg::slot_closeHint()
{
    if(m_pHint!=NULL&&m_pHintEngine!=NULL)
    {
        m_pHintEngine->deleteLater();
        m_pHint->deleteLater();
        m_pHint=NULL;
        m_pHintEngine=NULL;
    }
    m_closeHintTimer->stop();
}

void  HomeControlMainDlg::slot_openListen()
{
    QString  ip;
    if(m_pDealWlanData->WlanOn_off())
    {
        QList<QHostAddress> vAddressList = QNetworkInterface::allAddresses();
        for(int i=0;i<vAddressList.size();i++)
        {
            if((vAddressList.at(i)!=QHostAddress::LocalHost)&&(vAddressList.at(i).protocol()==QAbstractSocket::IPv4Protocol))
                ip=vAddressList.at(i).toString();
        }
    }
    else
    {
        QSettings settings("/mnt/disk/config/config",QSettings::IniFormat);
        ip=settings.value("system/native_ip").toString();
    }

    MyMsgLog<<ip;
    if(m_pServer->CloseListen())
        m_pServer->OpenListen(ip,tr("555"));
}

void   HomeControlMainDlg::slot_security(int cmd)
{
    QByteArray m_array;
    m_array.resize(6);
    m_array.data()[0] = 0x06;
    m_array.data()[1] = 0x00;
    m_array.data()[2] = 0x00;
    m_array.data()[3] = 0x12;
    m_array.data()[4] = cmd;
    int  sum=0;
    for(int i=0;i<5;i++)
    {
        sum+=(int)m_array.data()[i];
    }
    m_array.data()[5] = sum&0xff;
    m_pMyQByteArray->RcvLeeBus(m_array,"leebus","Append");
}
