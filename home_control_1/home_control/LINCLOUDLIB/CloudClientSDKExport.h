#ifndef CLOUDCLIENTSDKEXPORT_H
#define CLOUDCLIENTSDKEXPORT_H


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#if (defined (WIN32))
	#define LIN_EXPORT       __declspec(dllimport) 
#else
	#define LIN_EXPORT
#endif

typedef enum
{
	LIN_ExceptionEnm_OK = 0,				
	LIN_ExceptionEnm_NotInit,				
	LIN_ExceptionEnm_ServiceCnError,		
	LIN_ExceptionEnm_UnknowException,	
	LIN_ExceptionEnm_NetShutdown, 
	LIN_ExceptionEnm_TimeOut,
	LIN_ExceptionEnm_NetRefused,
	LIN_ExceptionEnm_NoSck,
	LIN_ExceptionEnm_SndDataTooLong, 
	LIN_ExceptionEnm_ParamErr, 
	LIN_ExceptionEnm_NotInitNetEnv, 
	LIN_ExceptionEnm_LastRcvUndone, 
	LIN_ExceptionEnm_MsgDecErr, 
	LIN_ExceptionEnm_DevNot,
	LIN_ExceptionEnm_RetTimerOut, 
	LIN_ExceptionEnm_SndRetTimerOut, 
	LIN_ExceptionEnm_SameIDLoged,

	LIN_ExceptionEnm_NotUSE					
}LIN_ExceptionEnm;

typedef unsigned char		LINExp_UInt8;
typedef unsigned short		LINExp_UInt16;
typedef unsigned int		LINExp_Uint;
typedef char				LINExp_Int8;
typedef short				LINExp_Int16;
typedef int					LINExp_int;

typedef char *				LINExp_pChar;
typedef const char*			LINExp_cpChar;
typedef const LINExp_UInt8*	LINExp_cpByte;

typedef unsigned long		LINExp_NetHandle;
typedef LINExp_NetHandle*	pLINExp_NetHandle;

#define SPLITE_IDS_SYBOL ";"

typedef enum
{
	LIN_OnlineDevieTypeEnm_LeeBus = 0,	//LeeBus类型	
	LIN_OnlineDevieTypeEnm_PC,		//PC类型	
	LIN_OnlineDevieTypeEnm_Touch,		//Touch类型	
}LIN_OnlineDevieTypeEnm; //在线设备类型枚举



/***********************************************************************************
函数名 : LIN_GetLibVersion
描  述 : 取得当前库版本号相关的信息
输入参数说明 :
	pRcvVerInfo     : 保存版本信息的缓冲区,如果此值为NULL ，将没有输出
	RcvVerInfoLen   : 此缓冲的大小，尽量大一些，否则输出的详细信息不完整，建议　512　字节, 如果此值为0 ，将没有输出
	pRcvSlaveInfo   : 保存版本附加信息的缓冲区,如果此值为NULL ，将没有输出
	RcvSlaveInfoLen : 此缓冲的大小，尽量大一些，否则输出的详细信息不完整，建议　1024以上　字节, 如果此值为0 ，将没有输出
输出参数说明 :
返 回 值: 
其它说明: 此函数是一个功能辅助函数
***********************************************************************************/
LIN_EXPORT void LIN_GetLibVersion(LINExp_pChar pRcvVerInfo, LINExp_Uint RcvVerInfoLen, LINExp_pChar pRcvSlaveInfo, LINExp_Uint RcvSlaveInfoLen);



/***********************************************************************************
函数名 : LIN_GetRetInfo
描  述 : 获取LIN_ExceptionEnm　类型的　详细的结果信息
输入参数说明 :
	EnmVal        : 要获取详细信息的枚举值，如果此值不在枚举范围内，将没有输出
输出参数说明 :
	pRcvEumInfo   : 保存详细信息的缓冲区,如果此值为NULL ，将没有输出
	RcvEumInfoLen : 此缓冲的大小，尽量大一些，否则输出的详细信息不完整，建议　512　字节, 如果此值为0 ，将没有输出
返 回 值: 指向　取得的　详细的结果信息指针，如果没有输出，返回　NULL
其它说明: 此函数是一个功能辅助函数，用于后续函数调用，获取详细的返回结果信息
***********************************************************************************/
LIN_EXPORT LINExp_cpChar LIN_GetRetInfo(LIN_ExceptionEnm EnmVal, LINExp_pChar pRcvEumInfo, LINExp_Uint RcvEumInfoLen);



/***********************************************************************************
函数名 : LIN_OnTouchRcvLeeBusData
描  述 : 当网络层接收到数据时，此回调被执行
输入参数说明 :
	pWorkObj		: 调用　LIN_XXXOpenLinCloud　初始化时传入的 hCurWorkObj
	TargDeviceID	: 此值为对应的输出设备ＩＤ
	pRcv			: 指向此网络层接收到的数据的指针(包括1~n条LeeBus数据)
	RcvLen			: 此网络层接收到的数据的长度
输出参数说明 :
返 回 值: LIN_ExceptionEnm　类型，　字面定义大概可了解意思，详细说明，调用　LIN_GetRetInfo　查看
其它说明: 只有网络成功连接到服务器后，接收到数据，此回调才会执行
	此回调　的实现，应该严格检查并处理各种可能的异常和错误，否则应用层的错误，会导致网络层崩溃
***********************************************************************************/
typedef LIN_ExceptionEnm (*LIN_OnTouchRcvLeeBusData)(LINExp_NetHandle pWorkObj, LINExp_cpChar TargDeviceID, LINExp_cpByte pRcv, LINExp_Uint RcvLen);

typedef enum
{
	LIN_DvcLogStateEnm_LogOut = 0,		//登出
	LIN_DvcLogStateEnm_LogIn,		//登入				

	LIN_DvcLogStateEnm_NotUSE		//保留未用		
}LIN_DvcLogStateEnm; //登录状态枚举

/***********************************************************************************
函数名 : LIN_OnLeeBusDvcLogStateChged
描  述 : 录相关LeeBus设备上线或下线时，此回调被执行
输入参数说明 :
	pWorkObj		: 调用　OpenLinCloud　初始化时传入的 hCurWorkObj
	TargDeviceID	: 此值为对应的输出设备ＩＤ
	CurState		: 此设备当前切换到的状态 //具体参考 LIN_DvcLogStateEnm
	Script			: 相关描述，保留未用
输出参数说明 :
返 回 值: 
其它说明: 此回调为增加客户应用友好体验而增加
***********************************************************************************/
typedef void (*LIN_OnLeeBusDvcLogStateChged)(LINExp_NetHandle pWorkObj, LINExp_cpChar TargDeviceID, LIN_DvcLogStateEnm CurState, LINExp_cpChar Script);

/***********************************************************************************
函数名 : LIN_OnNetClose
描  述 : 当网络连接被关闭时的回调通知
输入参数说明 :
	pWorkObj: 它是调用　LIN_TouchOpenLinCloud或LIN_LeeBusOpenLinCloud　初始化时传入的 hCurWorkObj 业务句柄
	CloseReason : 关闭原因，LIN_ExceptionEnm　类型，　字面定义大概可了解意思，详细说明，调用　LIN_GetRetInfo　查看
输出参数说明 :
返 回 值: 
其它说明: 此回调结束后，一定要调用　LIN_LogOutLinCloud　释放相关网络资源，不要在此回调中调用　LIN_LogOutLinCloud
***********************************************************************************/
typedef void (*LIN_OnNetClose)(LINExp_NetHandle pWorkObj, LIN_ExceptionEnm CloseReason);

/***********************************************************************************
函数名 : LIN_OnLeeBusDeviceRcvData
描  述 : 当网络层接收到数据时，此回调被执行
输入参数说明 :
	pWorkObj		: 调用　LIN_LeeBusOpenLinCloud　初始化时传入的 hCurWorkObj
	pRcv			: 指向此网络层接收到的数据的指针(包括1~n条LeeBus数据)
	RcvLen			: 此网络层接收到的数据的长度
输出参数说明 :
返 回 值: LIN_ExceptionEnm　类型，　字面定义大概可了解意思，详细说明，调用　LIN_GetRetInfo　查看
其它说明: 只有网络成功连接到服务器后，接收到数据，此回调才会执行
	此回调　的实现，应该严格检查并处理各种可能的异常和错误，否则应用层的错误，会导致网络层崩溃
***********************************************************************************/
typedef LIN_ExceptionEnm (*LIN_OnLeeBusDeviceRcvData)(LINExp_NetHandle pWorkObj, LINExp_cpByte pRcv, LINExp_Uint RcvLen);



/***********************************************************************************
函数名 : LIN_CloudClientInit
描  述 : 初始化接入云环境
输入参数说明 :
输出参数说明 :
返 回 值: LIN_ExceptionEnm　类型，　字面定义大概可了解意思，详细说明，调用　LIN_GetRetInfo　查看
其它说明: 与 LIN_CloudClientExit 成对调用
***********************************************************************************/
LIN_EXPORT LIN_ExceptionEnm LIN_CloudClientInit();


/***********************************************************************************
函数名 : LIN_CloudClientExit
描  述 : 关闭接入云环境
输入参数说明 :
输出参数说明 :
返 回 值: 
其它说明: 
***********************************************************************************/
LIN_EXPORT void LIN_CloudClientExit();


/***********************************************************************************
函数名 : LIN_TouchOpenLinCloud
描  述 : 打开输入设备端云连接
输入参数说明 :
	hCurWorkObj         : 当前业务层工作对象，参考Demo
	SvrIP               : 服务器IP
	OnDownLoadCloudData : 数据接收回调， 详见 LIN_OnTouchRcvLeeBusData 说明
	OnClose             : 网络关闭回调， 详见 LIN_OnNetClose 说明
	OnLBusDvcLogSttChged: 相关LeeBus设备上下线通知回调， 详见 LIN_OnLeeBusDvcLogStateChged 说明
输出参数说明 :
	pRetHNet            : 返回的SDK工作对象
返 回 值: LIN_ExceptionEnm　类型，　字面定义大概可了解意思，详细说明，调用　LIN_GetRetInfo　查看
其它说明: 
***********************************************************************************/
LIN_EXPORT LIN_ExceptionEnm LIN_TouchOpenLinCloud(LINExp_NetHandle hCurWorkObj, LINExp_cpChar SvrIP, pLINExp_NetHandle pRetHNet, LIN_OnTouchRcvLeeBusData OnDownLoadCloudData
												  , LIN_OnNetClose OnClose, LIN_OnLeeBusDvcLogStateChged OnLBusDvcLogSttChged);

/***********************************************************************************
函数名 : LIN_LeeBusOpenLinCloud
描  述 : 打开输出设备端云连接
输入参数说明 :
	hCurWorkObj         : 当前业务层工作对象，参考Demo
	SvrIP               : 服务器IP
	OnDownLoadCloudData : 数据接收回调， 详见 LIN_OnLeeBusDeviceRcvData 说明
	OnClose             : 网络关闭回调， 详见 LIN_OnNetClose 说明
输出参数说明 :
	pRetHNet            : 返回的SDK工作对象
返 回 值: LIN_ExceptionEnm　类型，　字面定义大概可了解意思，详细说明，调用　LIN_GetRetInfo　查看
其它说明: 
***********************************************************************************/
LIN_EXPORT LIN_ExceptionEnm LIN_LeeBusOpenLinCloud(LINExp_NetHandle hCurWorkObj, LINExp_cpChar SvrIP, pLINExp_NetHandle pRetHNet, LIN_OnLeeBusDeviceRcvData OnDownLoadCloudData, LIN_OnNetClose OnClose);


/***********************************************************************************
函数名 : LIN_LogInLinCloud
描  述 : 登录到云端
输入参数说明 :
	HNet  : LIN_xxxOpenLinCloud 的 pRetHNet，参考Demo
	strID : 设备ID
输出参数说明 :
返 回 值: LIN_ExceptionEnm　类型，　字面定义大概可了解意思，详细说明，调用　LIN_GetRetInfo　查看
其它说明: 
***********************************************************************************/
LIN_EXPORT LIN_ExceptionEnm LIN_LogInLinCloud(LINExp_NetHandle HNet, LINExp_cpChar strID);

/***********************************************************************************
函数名 : LIN_LogInLinCloudForControlUser
描  述 : 手机或PC类用户登录到云端
输入参数说明 :
	HNet : LIN_xxxOpenLinCloud 的 pRetHNet，参考Demo
	UID  : 在Web管理端注册的设备用户名
	Pwd  : 在Web管理端注册的设备用户密码
	ppRetDvcCodes	: 所有此手机用户关联的设备ID集合，以 ; 分隔（SPLITE_IDS_SYBOL）
		注意：此处使用双指针，是为了灵活返回不同长度的数据，此返回值的有效期到 LIN_LogOutLinCloud之前，之后如果再使用，有不确定的结果，特别注意！！！
输出参数说明 :
返 回 值: LIN_ExceptionEnm　类型，　字面定义大概可了解意思，详细说明，调用　LIN_GetRetInfo　查看
其它说明: 
***********************************************************************************/
LIN_EXPORT LIN_ExceptionEnm LIN_LogInLinCloudForControlUser(LINExp_NetHandle HNet, LINExp_cpChar UID, LINExp_cpChar Pwd, char** ppRetDvcCodes);


/***********************************************************************************
函数名 : LIN_LogOutLinCloud
描  述 : 登录云端
输入参数说明 :
	HNet  : LIN_xxxOpenLinCloud 的 pRetHNet，参考Demo
输出参数说明 :
返 回 值: 
其它说明: 
***********************************************************************************/
LIN_EXPORT void LIN_LogOutLinCloud(LINExp_NetHandle HNet);

/***********************************************************************************
函数名 : LIN_InputObjUplowdDataToLinCloud
描  述 : 输入设备上行数据函数
输入参数说明 :
	HNet  : LIN_xxxOpenLinCloud 的 pRetHNet，参考Demo
	TargDeviceID : 目前输出设备
	pSndData     : 要发送的数据，可以是 Leebus协议数据，或其它已定义的应用层协议数据
	SndLen       : 数据长度
输出参数说明 :
返 回 值: LIN_ExceptionEnm　类型，　字面定义大概可了解意思，详细说明，调用　LIN_GetRetInfo　查看
其它说明: 
***********************************************************************************/
LIN_EXPORT LIN_ExceptionEnm LIN_InputObjUplowdDataToLinCloud(LINExp_NetHandle HNet, LINExp_cpChar TargDeviceID, LINExp_cpByte pSndData, LINExp_Uint SndLen);


/***********************************************************************************
函数名 : LIN_LeeBusUplowdDataToLinCloud
描  述 : 输出设备上行数据函数
输入参数说明 :
	HNet  : LIN_xxxOpenLinCloud 的 pRetHNet，参考Demo
	pSndData     : 要发送的数据，可以是 Leebus协议数据，或其它已定义的应用层协议数据
	SndLen       : 数据长度
输出参数说明 :
返 回 值: LIN_ExceptionEnm　类型，　字面定义大概可了解意思，详细说明，调用　LIN_GetRetInfo　查看
其它说明: 
***********************************************************************************/
LIN_EXPORT LIN_ExceptionEnm LIN_LeeBusUplowdDataToLinCloud(LINExp_NetHandle HNet, LINExp_cpByte pSndData, LINExp_Uint SndLen);



/***********************************************************************************
函数名 : LIN_GetUpdateInfo
描  述 : 取得当前App的可升级目标文件在服务器位置
输入参数说明 :
	HNet				: LIN_xxxOpenLinCloud 的 pRetHNet，参考Demo
	strVersion			: 当前App版本
输出参数说明 :
	ppRetUpdatePath		: 当前App的可升级目标文件在服务器位置 用于 LIN_DownloadUpdateFile
		注意：此处使用双指针，是为了灵活返回不同长度的数据，此返回值的有效期到下次调用此函数前，之后如果再使用，有不确定的结果，特别注意！！！
返 回 值: LIN_ExceptionEnm　类型，　字面定义大概可了解意思，详细说明，调用　LIN_GetRetInfo　查看
其它说明: 只有当前版本小于服务器版本时，才返回服务器升级路径；版本比较使用区分大小写的字符串比较
***********************************************************************************/
LIN_EXPORT LIN_ExceptionEnm LIN_GetUpdateInfo(LINExp_NetHandle HNet, LINExp_cpChar strVersion, char** ppRetUpdatePath);

/***********************************************************************************
函数名 : LIN_DownloadUpdateFile
描  述 : 下载当前App的可升级目标文件到本机指定位置
输入参数说明 :
	HNet				: LIN_xxxOpenLinCloud 的 pRetHNet，参考Demo
	SvrUpdatePath 	    : 当前App可升级文件在服务器路径，通过 LIN_GetUpdateInfo 取得
	SvrDownloadPath	    : 当前App可升级文件下载位置
返 回 值: LIN_ExceptionEnm　类型，　字面定义大概可了解意思，详细说明，调用　LIN_GetRetInfo　查看
其它说明:
***********************************************************************************/
LIN_EXPORT LIN_ExceptionEnm LIN_DownloadUpdateFile(LINExp_NetHandle HNet, LINExp_cpChar SvrUpdatePath, LINExp_cpChar SvrDownloadPath);



/*以上ＡＰＩ的调用流程
		LIN_CloudClientInit 调用成功
				|
		LIN_TouchOpenLinCloud或 LIN_LeeBusOpenLinCloud 调用成功
				|
		LIN_LogInLinCloud 调用成功
				|
		LIN_InputObjUplowdDataToLinCloud或LIN_LeeBusUplowdDataToLinCloud 发送数据或 OnxxxRcvNetData 回调执行
				|
		LIN_OnNetClose 关闭回调
				|
		LIN_LogOutLinCloud
				|
		LIN_CloudClientExit

*/





#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif
