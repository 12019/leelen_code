#ifndef MYBUFFER_H
#define MYBUFFER_H
#include"linrcvsever.h"
#include<QTime>
#include<QDate>
#include<QStringList>

extern "C"
{
    #include"CloudClientSDKExport.h"
}
#define  LIN_ReCntInterval 30000    //云服务器重连的时间间隔
#define  home_control_config   "/mnt/disk/config/homecontrol.ini"      //家居的配置信息包括云服务器等相关信息
#define  updatename   "/mnt/disk/lib/update.so"
#define  XmlFilePath     "/mnt/disk/home_control/"
#define  CloudBreak    1;
#define  NeedUpdate  0;
#define  NotUpdate     2;

#define LeeBusBusy0 0xfb         //总线处于忙碌状态
#define LeeBusBusy1 0xfd         //总线处于忙碌状态
#define LeeBusEasy0 0xfa          //总线处于空闲状态
#define LeeBusEasy1 0xfc          //总线处于空闲状态
#define LeeBusTimeOut 0x00    //总线超时
#define LeeBusError 0xfe         //表示出错,未用到

#define LIN_CLOUD_IP                "LIN_CLOUD_IP"                //云服务器IP
#define LIN_CLOUD_ID                "LIN_CLOUD_ID"                 //要登录到云服务器的设备ID
#define LIN_CLOUD_Acc               "LIN_CLOUD_Acc"              //云服务器帐号
#define LIN_CLOUD_PWD            "LIN_CLOUD_PWD"           //登录云服务器的密码
#define Ter_Account                     "Ter_Account"                       //终端机远程登录的帐号
#define Ter_Pwd                            "Ter_Pwd"                                 //终端机远程登录的密码

#define Life_Monitor                            0x11                                               //生命监测属性
#define Ter_Defence                             0x12                                               //终端机防区属性
#define GateWay                                   0x13                                              //智能网关
#define SenSor                                      0x14                                              //传感器
#define RISCO_Defence                        0xBB                                             //RISCO报警系统
#define General_Infrared                     0x0D                                              //普通红外

#define BackAudioMusic                       0x16                     //背景音乐非红外(博生背景音乐)
#define Music_Infrared                         0x0D                  //普通红外背景音乐
#define Ac_Infrared                               0x0D                //红外空调
#define Central_AC                                0x15               //中央空调

#define LeeBusCmdMax 10                                         //每帧指令的最大长度
#define RcvFromEveryMax 2000                                //从各个客户端接收的缓冲区大小
#define RcvFromLeeBusMax 2000                            //从总线接收到的数据的最大缓冲区，优先级最高，数据性最强
#define ClientMax 80                                               //客户端的最大数

#define Append 0
#define First 1

#define MyMsgLog qDebug()<<QDate::currentDate().toString()+" "+QTime::currentTime().toString()<<__FILE__<<"Line:"<<__LINE__

typedef struct RcvFromClient{
    bool IsValid;
    QByteArray buf;
    QString SvrIP;
}RcvFromClientBuf;


typedef struct RcvFrom{
    bool IsValid;
    QByteArray data;
    QString SvrIP;
    short size;
    short UsedTime;
}RcvFromObj;

typedef struct LinRcvClient{
    bool IsValid;
    LinRcvClientSocket *m_pClient;
    short count;
}LinRcvClient;

/******************************************************
 ****************房间图片，伸缩率，屏幕大小***************
 **************** **************************************/
typedef struct Lin_Map_Msg{
    float ImgWidthRatio;
    float ImgHeightRatio;
    int roomImgW;
    int roomImgH;
    QString Layer_Room_Name;
    int   screenW;                     //屏幕宽
    int   screenH;                     //屏幕高
    int   xPiece;                      //
    int   yPiece;
    int   controlW;                                              //70        //80
    int   controlH;                                          //90        //110
    int   pieceW;                                           //130     //160
    int   pieceH;                                              //90       //110
    int   topMargin;                                    //80       //110
    int   leftMargin;                                  //100     //147
    int   interval;                                      //10       //10
    int   topMargin1;                            //18       //30
    int   leftMargin1;                          //50       //83
    int   controlFont;
}LMap_6410_Msg;

typedef enum
{
    LIN_NotAnything = 0,
    LIN_CloudSvrIP,
    LIN_CloudSvrAccount,
    LIN_CloudSvrPassWord,
    LIN_CloudSvrID,
    LIN_TerSvrAccount,
    LIN_TerSvrPassWord,
    LIN_TerSceneFir,
    LIN_TerSceneSec,
    LIN_TerSceneThird,
    LIN_TerSceneFour,
    LIN_TerSceneFive,
    LIN_TerSceneSix,
    LIN_DeveloperPwdText,
    LIN_DeveloperDevName,
    LIN_DeveloperDevAddr
}LIN_ExceptionType;

typedef   enum
{
    RoomName=0,
    SceneName,
    DeviceName,
    DeviceOtherName,
    DeviceType,
    DeviceAdress,
    DeviceAttribute,
    TerminaiIP,
    TerminaiID,
    TerminaiAccountNumber,
    TerminaiPasswd,
    CloudAccountNumber,
    CloudID,
    FolderName,
    HomeManagePasswd,
    FeedbackMessage,
    WifiPasswd,
    WifiIp,
    WifiNetmask,
    WifiGateway,
    WifiDns

} EditDataType;

typedef   enum
{
    FreeWifi=0,
    InitWifi,
    SmartLinkWifi,
    ScanWifi,
    LinkWifi,
    SeniorLinkWifi,
    SeniorBreakWifi,
    DeleteWifi,
    OnWifi,
    Offwifi

}OperateWlan;

typedef   enum
{
    NotData=0,
    LoadMainInterfaceData,
    LoadRoomManageData,
    LoadHomeManageData,
    LoadRoomControlData,
    LoadRoomSceneData,
    LoadHomeSceneData,
    LoadHomeScene,
    CreateHomeScene,
    RoomScenePlaying,
    HomeScenePlaying,
    AddRoom,
    DeleteRoom,
    EditRoom,
    AddDevice,
    DeleteDevice,
    EditDevice

} OperateType;

typedef   struct  LuxDomoMusicInfo{
     QStringList   musicInfraredMode;
     QStringList   musicBackMode;
}MusicInfo;

typedef struct MyLuxDomoAcInfo{
    QStringList  acMode;
    QStringList  acModeEn ;

    QStringList  acInfraredSpeed;
    QStringList  acInfraredSpeedEn;

    QStringList  acCentralSpeed ;
    QStringList  acCentralSpeedEn ;
    QStringList  acCentralWind ;
    QStringList  acCentralWindEn ;
}AcInfo;

typedef struct  MyDeviceTypes{
    QStringList   type;
    QStringList   typeEn;
}DeviceTypes;

typedef struct MySensorValue{
    int  index;
    QString  roomUid;
    QString  deviceUid;
    QString  airValue;
    QString  airValue1;
    QString  tempValue;
    QString  humidnessValue;
    bool        airValueUpdate;
    bool        tempAndHumidnessValueUpdate;
}SensorValue;
#endif // MYBUFFER_H
