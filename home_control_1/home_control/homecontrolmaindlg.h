#ifndef HOMECONTROLMAINDLG_H
#define HOMECONTROLMAINDLG_H

#include<QDeclarativeEngine>
#include<QDeclarativeComponent>
#include<QDeclarativeItem>
#include<QDeclarativeView>
#include<QDeclarativeContext>
#include<qdom.h>
#include<QPaintEvent>

#include"hcstatusbar.h"
#include"homecontrolscenedialog.h"
#include"accontroldialog.h"
#include"curtaincontroldialog.h"
#include"tvcontroldialog.h"
#include<lightcontroldialog.h>
#include"musiccontroldialog.h"
#include"hcroomselect.h"
#include"housemapxmlthread.h"
#include "netrecvsocket.h"
#include<irpasswd.h>
#include<QPaintEvent>
#include<irlearndialog.h>
#include<homecontrolsceneedit.h>
#include<netrecvsocket.h>
#include"linrcvsever.h"
#include"hcsceneapplythread.h"
#include<rcvdatathread.h>
#include<hcqextserialport.h>
#include<hcsetting.h>
#include"mylineedit.h"
#include<LIN_CLOUD_SDK/linclouddevsdkthread.h>
#include<LIN_CLOUD_SDK/linversionupdatethread.h>
#include<MyQByteArray/myqbytearray.h>
#include<QMutex>
#include<Developer/developer.h>
#include<ParaStyle/parastyle.h>

#include"main_interface/luxdomoloadinglabel.h"
#include"main_interface/luxdomomenu.h"
#include"main_interface/luxdomomaininterface.h"
#include"main_interface/luxdomoscene.h"
#include"setting/luxdomosetting.h"
#include"setting/luxdomoaboutsetting.h"
#include"setting/luxdomocloudsetting.h"
#include"setting/luxdomofeedbacksetting.h"
#include"setting/luxdomomajorsetting.h"
#include"setting/luxdomoscenesetting.h"
#include"setting/luxdomoscenetoolbar.h"
#include"setting/luxdomoterminaisetting.h"
#include"setting/luxdomoversionssettting.h"
#include"setting/luxdomoscenefunctionoption.h"
#include"setting/luxdomowlansetting.h"
#include"setting/luxdomosynchronization.h"
#include"home_control/luxdomoroomcontrol.h"
#include"home_control/luxdomoroomcontrolpage.h"
#include"home_control/luxdomoroomdevice.h"
#include"home_control/luxdomodevicefolder.h"
#include"home_control/luxdomodevicefolderpage.h"
#include"home_manage/luxdomohomemanage.h"
#include"home_manage/luxdomoroom.h"
#include"home_manage/luxdomoroomcontainer.h"
#include"home_manage/luxdomodevicefunctionoption.h"
#include"home_manage/luxdomoroomfunctionoption.h"
#include"content/luxdomotoolbar.h"
#include"content/luxdomopulldownlist.h"
#include"home_manage/luxdomoroommanage.h"
#include"home_manage/luxdomodevicecontainer.h"
#include"home_manage/luxdomodevice.h"
#include"home_manage/luxdomodeviceoption.h"
#include"home_manage/luxdomoaclearn.h"
#include"home_manage/luxdomomusiclearn.h"
#include"home_manage/luxdomotvlearn.h"
#include"home_manage/luxdomoroomscenecontrol.h"
#include"device_control/luxdomoaccontrol.h"
#include"device_control/luxdomocurtaincontrol.h"
#include"device_control/luxdomomusiccontrol.h"
#include"device_control/luxdomotvcontrol.h"
#include"device_control/luxdomovar_lightcontrol.h"
#include"thread/luxdomoloadingdatatoui.h"
#include"thread/luxdomochangdevicedata.h"
#include"thread/luxdomomanagehomedata.h"
#include"thread/luxdomoapplyscene.h"
#include"thread/luxdomohomescene.h"
#include"content/luxdomolist.h"
#include"home_manage/luxdomoacedit.h"
#include"home_manage/luxdomomusicedit.h"
#include"home_manage/luxdomotvedit.h"
#include"home_manage/luxdomovarlight.h"
#include"thread/luxdomosendcommand.h"
#include"device_control/luxdomosensorinformation.h"
#include"main_interface/luxdomothemebar.h"
#include"content/luxdomostylelist.h"
#include"Security/luxdomorisco.h"
#include"Security/luxdomoriscocontrol.h"
#include"Security/luxdomoriscofinder.h"
#include"Security/luxdomoriscohistorymeaasge.h"
#include"Security/luxdomoriscosecurity.h"
#include"Security/luxdomoriscosecuritydata.h"
#include"thread/luxdomodealwlandata.h"
#include"thread/luxdomocheckwlan.h"
#include"setting/luxdomohomepagesetting.h"
#include"server_client/luxdomoserver.h"

#define HC_LIGHT                        0
#define HC_VARLIGHT                 1
#define HC_SWITCH                     2
#define HC_CURTAIN				   3
#define HC_AC					           4
#define	HC_TV					           5
#define HC_MUSIC				       6
#define HC_USER					       7
#define HC_CNT					           (HC_USER + 1)
#define devID                               0
#define TYPE                                 1

class HomeControlMainDlg : public QObject
{
    Q_OBJECT
public:
    explicit HomeControlMainDlg(QObject *parent = 0);
    ~HomeControlMainDlg();
    Q_INVOKABLE void setMainState(QString state);
    Q_INVOKABLE void showToast(QString msg);
    Q_INVOKABLE void findDevice(int x,int y);
    Q_INVOKABLE void setloading(bool true_false);
    Q_INVOKABLE void setStatusBarmsg(QString msg);
    Q_INVOKABLE int screenwidth();
    Q_INVOKABLE int screenheight();
    Q_INVOKABLE void showDeviceDlg();

    HCStatusBar *m_pHCStatusBar;
    HomeControlSceneDialog *m_pHomeControlSceneDlg;
    AccontrolDialog *m_pAccontrolDlg;
    CurtainControlDialog *m_pCurtainControlDlg;
    TVControlDialog *m_pTVControlDlg;
    LightControlDialog *m_pLightControlDlg;
    MusicControlDialog *m_pMusicControlDlg;
    HCRoomSelect *m_pHCRoomSelect;
    houseMapXmlThread *m_phouseMapXmlThread;
    Irpasswd *m_pIrpasswd;
    IrLearnDialog *m_pIrLearnDlg;
    HomeControlSceneEdit *m_pHCSceneEdit;
    HCSceneApplyThread *m_pSceneApplyThread;
    RcvDataThread *m_pRcvDataThread;
    LinRcvSever *m_pLinRcvSever;
    HCSetting *m_pHCSetting;
    MyLineEdit *HCLineEdit;
    Developer *m_pDeveloper;
    ParaStyle *m_pParaStyle;
    QMutex MapXmlMutex;
    LINCloudDevSDKThread *m_pLINCloudDEVSDK;
    LinVersionUpdateThread  *m_pLINVersionUpdate;
    QDeclarativeView *view;
    QDomDocument m_domDoc;
    QDomElement m_layerElement;
    QDomElement m_roomElement;
    QDomElement m_deviceElement;
    QDomElement deviceElement;
    QString Version;
    short deviceType;
    bool isFirstLaunch;

    int       homeControlReceive(NetRecvClientSocket *p_sock, char *buf, int len, QString strType = "");
    void    phomeControlReceive(NetRecvClientSocket *p_sock, char *buf, int len, QString strType);
    void    InfraredLearnReceive(char *);
    void    HomeSceneReceive(char*);
    void    RiscoSecurityReceive(char*);
    void    cmdBuffer(char *buf);
    void    openHouseMapXmlFile(QString strFile);
    void    setHCLineText(QString msg);

    void    setMainFace();
    void    UpdateDevice();
    void    saveHouseMapXmlFile();
    void    saveHouseMapXmlFile1();
    void    loadFisrMap();
    void    Translate();
    void    UpdateXml(char *cmd);
signals:
    void   sigSecurityStatus(int stat);
    void   sigRefreshInfraredLearnInterface(QByteArray);
    void   sigRefreshSensor(QString);
    void   sigRefreshTempAndHumidness(QString,QString);
public slots:
    void slot_sceneBuffer(char *buf,int len,QString type);
    void slot_getAlarmData(int,int,QString, int, int, int, int, int,QString, QString);
    void slot_textChanged(QString text);
    void slotRcvFromSvr(QByteArray buf,QString SvrIP,short size);
    void setSecurityStatus(int stat);
    void slotRecvHcStatus(char *buf);
    void slotRcvFromCloud(char *buf);
public slots:
    void     slot_editData(QString);
    void     slot_varValue();
    void     slot_musicVolume();
    void     slot_hideLoadingLabel();
    void     slot_checkThread();
    void     slot_openListen();
    void     slot_security(int);
    void     slot_closeHint();
 public:  
    QDeclarativeView *smartHomeView;                    //底层对话框
    LuxDomoMainInterface   *mainIterface;
    LuxDomoMenu       *menu;                                   //菜单
    QDeclarativeItem    *m_pLoadingLabel;
    LuxDomoThemebar   *m_pThemebar;
    LuxDomoSetting   *setting;                                  //设置界面

    LuxDomoHomeManage     *m_pHomeManage;
    LuxDomoRoomManage     *m_pRoomManage;
    LuxDomoRisco                    *m_pRisco;
    LuxDomoRoomControl      *roomControl;
    QVector<LuxDomoRoomControlPage*>   roomControlPages;

    LuxDomoLoadingDataToUI      *m_pLoadingDataToUI;
    LuxDomoChangDeviceData     *m_pChangDeviceData;
    LuxDomoManageHomeData    *m_pManageHomeData;
    LuxDomoHomeScene               *m_pHomeScene;
    LuxDomoSendCommand         *m_pSendCommand;
    LuxDomoRiscoSecurityData     *m_pRiscoSecurityData;
    LuxDomoApplyScene                *m_pApplyScene;
    LuxDomoDealWlanData            *m_pDealWlanData ;
    LuxDomoCheckWlan                  *m_pCheckWlan;
    LuxdomoServer                            *m_pServer;

    QDeclarativeItem    *tvItem;
    QDeclarativeItem    *musicItem;
    QDeclarativeItem    *acItem;
    QDeclarativeItem     *varItem;
    QDeclarativeItem     *curtainItem;
    QDeclarativeItem     *m_pHint;
    QDeclarativeEngine  *m_pHintEngine;
    QDeclarativeItem     *sensorItem;

    QSqlDatabase          m_DataBaseDB;
    QDomDocument     m_pHome;
    QString                     m_editName;
    bool                          m_varValue;
    QTimer                     *timer;
    bool                          m_musicVolume;
    QTimer                     *timer1;
    QTimer                     *m_checkThreadTimer;
    QTimer                     *m_closeHintTimer;
    QMutex  m_svrDataMutex;
    QMutex  m_receiveDeviceCmdMutex;

    void     Init();                           //初始化函数
    QDeclarativeItem*     CreateElement(QString);//创建ＱＭＬ对话框
    void     CreateRoomControl();
    void     CreateHomeManage();
    void     CreateRoomManage();
    void     CreateSecurity();
    void     LoadConfigurationFile(QString);
    QString          GetControlImage(QString,QString on_off=tr("off"));
    QStringList    GetSceneImage(QString);
    void    saveHomeXmlFile();
    void    OpenDataBase();
    QString   GetAcModeName(QString);
    QString   GetAcInfraredSpeedName(QString);
    QString   GetAcCentralSpeedName(QString);
    QString   GetAcCentralWindName(QString);
    QString   GetDeviceTypeName(QString);
public:
    void    ImageTransform();
    void    show();
    void    showFullScreen();
    void    hide();
    bool    isVisible();
    void    findDevState();
    void    findRiscoState();
    void    GetSynchronizationfigurationFile();
    void    SetSynchronizationfigurationFile();
    void    SynchronizationfigurationFile();
    void    CheckHomeControlFolder();
    void    CheckConfigurationFile();
    void    CheckRiscoConfigurationFile();
    void    CheckVersionConfigurationFile();
    void    UpdateConfigurationFile();
    void    DeleteConfigurationFile();
    void    UpdateHomeScene();
    void    UpdateRoomScene();
    void    ShowHintInformation(QString);
};

#endif // HOMECONTROLMAINDLG_H
