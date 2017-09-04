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
	LIN_OnlineDevieTypeEnm_LeeBus = 0,	//LeeBus����	
	LIN_OnlineDevieTypeEnm_PC,		//PC����	
	LIN_OnlineDevieTypeEnm_Touch,		//Touch����	
}LIN_OnlineDevieTypeEnm; //�����豸����ö��



/***********************************************************************************
������ : LIN_GetLibVersion
��  �� : ȡ�õ�ǰ��汾����ص���Ϣ
�������˵�� :
	pRcvVerInfo     : ����汾��Ϣ�Ļ�����,�����ֵΪNULL ����û�����
	RcvVerInfoLen   : �˻���Ĵ�С��������һЩ�������������ϸ��Ϣ�����������顡512���ֽ�, �����ֵΪ0 ����û�����
	pRcvSlaveInfo   : ����汾������Ϣ�Ļ�����,�����ֵΪNULL ����û�����
	RcvSlaveInfoLen : �˻���Ĵ�С��������һЩ�������������ϸ��Ϣ�����������顡1024���ϡ��ֽ�, �����ֵΪ0 ����û�����
�������˵�� :
�� �� ֵ: 
����˵��: �˺�����һ�����ܸ�������
***********************************************************************************/
LIN_EXPORT void LIN_GetLibVersion(LINExp_pChar pRcvVerInfo, LINExp_Uint RcvVerInfoLen, LINExp_pChar pRcvSlaveInfo, LINExp_Uint RcvSlaveInfoLen);



/***********************************************************************************
������ : LIN_GetRetInfo
��  �� : ��ȡLIN_ExceptionEnm�����͵ġ���ϸ�Ľ����Ϣ
�������˵�� :
	EnmVal        : Ҫ��ȡ��ϸ��Ϣ��ö��ֵ�������ֵ����ö�ٷ�Χ�ڣ���û�����
�������˵�� :
	pRcvEumInfo   : ������ϸ��Ϣ�Ļ�����,�����ֵΪNULL ����û�����
	RcvEumInfoLen : �˻���Ĵ�С��������һЩ�������������ϸ��Ϣ�����������顡512���ֽ�, �����ֵΪ0 ����û�����
�� �� ֵ: ָ��ȡ�õġ���ϸ�Ľ����Ϣָ�룬���û����������ء�NULL
����˵��: �˺�����һ�����ܸ������������ں����������ã���ȡ��ϸ�ķ��ؽ����Ϣ
***********************************************************************************/
LIN_EXPORT LINExp_cpChar LIN_GetRetInfo(LIN_ExceptionEnm EnmVal, LINExp_pChar pRcvEumInfo, LINExp_Uint RcvEumInfoLen);



/***********************************************************************************
������ : LIN_OnTouchRcvLeeBusData
��  �� : ���������յ�����ʱ���˻ص���ִ��
�������˵�� :
	pWorkObj		: ���á�LIN_XXXOpenLinCloud����ʼ��ʱ����� hCurWorkObj
	TargDeviceID	: ��ֵΪ��Ӧ������豸�ɣ�
	pRcv			: ָ����������յ������ݵ�ָ��(����1~n��LeeBus����)
	RcvLen			: ���������յ������ݵĳ���
�������˵�� :
�� �� ֵ: LIN_ExceptionEnm�����ͣ������涨���ſ��˽���˼����ϸ˵�������á�LIN_GetRetInfo���鿴
����˵��: ֻ������ɹ����ӵ��������󣬽��յ����ݣ��˻ص��Ż�ִ��
	�˻ص�����ʵ�֣�Ӧ���ϸ��鲢������ֿ��ܵ��쳣�ʹ��󣬷���Ӧ�ò�Ĵ��󣬻ᵼ����������
***********************************************************************************/
typedef LIN_ExceptionEnm (*LIN_OnTouchRcvLeeBusData)(LINExp_NetHandle pWorkObj, LINExp_cpChar TargDeviceID, LINExp_cpByte pRcv, LINExp_Uint RcvLen);

typedef enum
{
	LIN_DvcLogStateEnm_LogOut = 0,		//�ǳ�
	LIN_DvcLogStateEnm_LogIn,		//����				

	LIN_DvcLogStateEnm_NotUSE		//����δ��		
}LIN_DvcLogStateEnm; //��¼״̬ö��

/***********************************************************************************
������ : LIN_OnLeeBusDvcLogStateChged
��  �� : ¼���LeeBus�豸���߻�����ʱ���˻ص���ִ��
�������˵�� :
	pWorkObj		: ���á�OpenLinCloud����ʼ��ʱ����� hCurWorkObj
	TargDeviceID	: ��ֵΪ��Ӧ������豸�ɣ�
	CurState		: ���豸��ǰ�л�����״̬ //����ο� LIN_DvcLogStateEnm
	Script			: �������������δ��
�������˵�� :
�� �� ֵ: 
����˵��: �˻ص�Ϊ���ӿͻ�Ӧ���Ѻ����������
***********************************************************************************/
typedef void (*LIN_OnLeeBusDvcLogStateChged)(LINExp_NetHandle pWorkObj, LINExp_cpChar TargDeviceID, LIN_DvcLogStateEnm CurState, LINExp_cpChar Script);

/***********************************************************************************
������ : LIN_OnNetClose
��  �� : ���������ӱ��ر�ʱ�Ļص�֪ͨ
�������˵�� :
	pWorkObj: ���ǵ��á�LIN_TouchOpenLinCloud��LIN_LeeBusOpenLinCloud����ʼ��ʱ����� hCurWorkObj ҵ����
	CloseReason : �ر�ԭ��LIN_ExceptionEnm�����ͣ������涨���ſ��˽���˼����ϸ˵�������á�LIN_GetRetInfo���鿴
�������˵�� :
�� �� ֵ: 
����˵��: �˻ص�������һ��Ҫ���á�LIN_LogOutLinCloud���ͷ����������Դ����Ҫ�ڴ˻ص��е��á�LIN_LogOutLinCloud
***********************************************************************************/
typedef void (*LIN_OnNetClose)(LINExp_NetHandle pWorkObj, LIN_ExceptionEnm CloseReason);

/***********************************************************************************
������ : LIN_OnLeeBusDeviceRcvData
��  �� : ���������յ�����ʱ���˻ص���ִ��
�������˵�� :
	pWorkObj		: ���á�LIN_LeeBusOpenLinCloud����ʼ��ʱ����� hCurWorkObj
	pRcv			: ָ����������յ������ݵ�ָ��(����1~n��LeeBus����)
	RcvLen			: ���������յ������ݵĳ���
�������˵�� :
�� �� ֵ: LIN_ExceptionEnm�����ͣ������涨���ſ��˽���˼����ϸ˵�������á�LIN_GetRetInfo���鿴
����˵��: ֻ������ɹ����ӵ��������󣬽��յ����ݣ��˻ص��Ż�ִ��
	�˻ص�����ʵ�֣�Ӧ���ϸ��鲢������ֿ��ܵ��쳣�ʹ��󣬷���Ӧ�ò�Ĵ��󣬻ᵼ����������
***********************************************************************************/
typedef LIN_ExceptionEnm (*LIN_OnLeeBusDeviceRcvData)(LINExp_NetHandle pWorkObj, LINExp_cpByte pRcv, LINExp_Uint RcvLen);



/***********************************************************************************
������ : LIN_CloudClientInit
��  �� : ��ʼ�������ƻ���
�������˵�� :
�������˵�� :
�� �� ֵ: LIN_ExceptionEnm�����ͣ������涨���ſ��˽���˼����ϸ˵�������á�LIN_GetRetInfo���鿴
����˵��: �� LIN_CloudClientExit �ɶԵ���
***********************************************************************************/
LIN_EXPORT LIN_ExceptionEnm LIN_CloudClientInit();


/***********************************************************************************
������ : LIN_CloudClientExit
��  �� : �رս����ƻ���
�������˵�� :
�������˵�� :
�� �� ֵ: 
����˵��: 
***********************************************************************************/
LIN_EXPORT void LIN_CloudClientExit();


/***********************************************************************************
������ : LIN_TouchOpenLinCloud
��  �� : �������豸��������
�������˵�� :
	hCurWorkObj         : ��ǰҵ��㹤�����󣬲ο�Demo
	SvrIP               : ������IP
	OnDownLoadCloudData : ���ݽ��ջص��� ��� LIN_OnTouchRcvLeeBusData ˵��
	OnClose             : ����رջص��� ��� LIN_OnNetClose ˵��
	OnLBusDvcLogSttChged: ���LeeBus�豸������֪ͨ�ص��� ��� LIN_OnLeeBusDvcLogStateChged ˵��
�������˵�� :
	pRetHNet            : ���ص�SDK��������
�� �� ֵ: LIN_ExceptionEnm�����ͣ������涨���ſ��˽���˼����ϸ˵�������á�LIN_GetRetInfo���鿴
����˵��: 
***********************************************************************************/
LIN_EXPORT LIN_ExceptionEnm LIN_TouchOpenLinCloud(LINExp_NetHandle hCurWorkObj, LINExp_cpChar SvrIP, pLINExp_NetHandle pRetHNet, LIN_OnTouchRcvLeeBusData OnDownLoadCloudData
												  , LIN_OnNetClose OnClose, LIN_OnLeeBusDvcLogStateChged OnLBusDvcLogSttChged);

/***********************************************************************************
������ : LIN_LeeBusOpenLinCloud
��  �� : ������豸��������
�������˵�� :
	hCurWorkObj         : ��ǰҵ��㹤�����󣬲ο�Demo
	SvrIP               : ������IP
	OnDownLoadCloudData : ���ݽ��ջص��� ��� LIN_OnLeeBusDeviceRcvData ˵��
	OnClose             : ����رջص��� ��� LIN_OnNetClose ˵��
�������˵�� :
	pRetHNet            : ���ص�SDK��������
�� �� ֵ: LIN_ExceptionEnm�����ͣ������涨���ſ��˽���˼����ϸ˵�������á�LIN_GetRetInfo���鿴
����˵��: 
***********************************************************************************/
LIN_EXPORT LIN_ExceptionEnm LIN_LeeBusOpenLinCloud(LINExp_NetHandle hCurWorkObj, LINExp_cpChar SvrIP, pLINExp_NetHandle pRetHNet, LIN_OnLeeBusDeviceRcvData OnDownLoadCloudData, LIN_OnNetClose OnClose);


/***********************************************************************************
������ : LIN_LogInLinCloud
��  �� : ��¼���ƶ�
�������˵�� :
	HNet  : LIN_xxxOpenLinCloud �� pRetHNet���ο�Demo
	strID : �豸ID
�������˵�� :
�� �� ֵ: LIN_ExceptionEnm�����ͣ������涨���ſ��˽���˼����ϸ˵�������á�LIN_GetRetInfo���鿴
����˵��: 
***********************************************************************************/
LIN_EXPORT LIN_ExceptionEnm LIN_LogInLinCloud(LINExp_NetHandle HNet, LINExp_cpChar strID);

/***********************************************************************************
������ : LIN_LogInLinCloudForControlUser
��  �� : �ֻ���PC���û���¼���ƶ�
�������˵�� :
	HNet : LIN_xxxOpenLinCloud �� pRetHNet���ο�Demo
	UID  : ��Web�����ע����豸�û���
	Pwd  : ��Web�����ע����豸�û�����
	ppRetDvcCodes	: ���д��ֻ��û��������豸ID���ϣ��� ; �ָ���SPLITE_IDS_SYBOL��
		ע�⣺�˴�ʹ��˫ָ�룬��Ϊ�����ز�ͬ���ȵ����ݣ��˷���ֵ����Ч�ڵ� LIN_LogOutLinCloud֮ǰ��֮�������ʹ�ã��в�ȷ���Ľ�����ر�ע�⣡����
�������˵�� :
�� �� ֵ: LIN_ExceptionEnm�����ͣ������涨���ſ��˽���˼����ϸ˵�������á�LIN_GetRetInfo���鿴
����˵��: 
***********************************************************************************/
LIN_EXPORT LIN_ExceptionEnm LIN_LogInLinCloudForControlUser(LINExp_NetHandle HNet, LINExp_cpChar UID, LINExp_cpChar Pwd, char** ppRetDvcCodes);


/***********************************************************************************
������ : LIN_LogOutLinCloud
��  �� : ��¼�ƶ�
�������˵�� :
	HNet  : LIN_xxxOpenLinCloud �� pRetHNet���ο�Demo
�������˵�� :
�� �� ֵ: 
����˵��: 
***********************************************************************************/
LIN_EXPORT void LIN_LogOutLinCloud(LINExp_NetHandle HNet);

/***********************************************************************************
������ : LIN_InputObjUplowdDataToLinCloud
��  �� : �����豸�������ݺ���
�������˵�� :
	HNet  : LIN_xxxOpenLinCloud �� pRetHNet���ο�Demo
	TargDeviceID : Ŀǰ����豸
	pSndData     : Ҫ���͵����ݣ������� LeebusЭ�����ݣ��������Ѷ����Ӧ�ò�Э������
	SndLen       : ���ݳ���
�������˵�� :
�� �� ֵ: LIN_ExceptionEnm�����ͣ������涨���ſ��˽���˼����ϸ˵�������á�LIN_GetRetInfo���鿴
����˵��: 
***********************************************************************************/
LIN_EXPORT LIN_ExceptionEnm LIN_InputObjUplowdDataToLinCloud(LINExp_NetHandle HNet, LINExp_cpChar TargDeviceID, LINExp_cpByte pSndData, LINExp_Uint SndLen);


/***********************************************************************************
������ : LIN_LeeBusUplowdDataToLinCloud
��  �� : ����豸�������ݺ���
�������˵�� :
	HNet  : LIN_xxxOpenLinCloud �� pRetHNet���ο�Demo
	pSndData     : Ҫ���͵����ݣ������� LeebusЭ�����ݣ��������Ѷ����Ӧ�ò�Э������
	SndLen       : ���ݳ���
�������˵�� :
�� �� ֵ: LIN_ExceptionEnm�����ͣ������涨���ſ��˽���˼����ϸ˵�������á�LIN_GetRetInfo���鿴
����˵��: 
***********************************************************************************/
LIN_EXPORT LIN_ExceptionEnm LIN_LeeBusUplowdDataToLinCloud(LINExp_NetHandle HNet, LINExp_cpByte pSndData, LINExp_Uint SndLen);



/***********************************************************************************
������ : LIN_GetUpdateInfo
��  �� : ȡ�õ�ǰApp�Ŀ�����Ŀ���ļ��ڷ�����λ��
�������˵�� :
	HNet				: LIN_xxxOpenLinCloud �� pRetHNet���ο�Demo
	strVersion			: ��ǰApp�汾
�������˵�� :
	ppRetUpdatePath		: ��ǰApp�Ŀ�����Ŀ���ļ��ڷ�����λ�� ���� LIN_DownloadUpdateFile
		ע�⣺�˴�ʹ��˫ָ�룬��Ϊ�����ز�ͬ���ȵ����ݣ��˷���ֵ����Ч�ڵ��´ε��ô˺���ǰ��֮�������ʹ�ã��в�ȷ���Ľ�����ر�ע�⣡����
�� �� ֵ: LIN_ExceptionEnm�����ͣ������涨���ſ��˽���˼����ϸ˵�������á�LIN_GetRetInfo���鿴
����˵��: ֻ�е�ǰ�汾С�ڷ������汾ʱ���ŷ��ط���������·�����汾�Ƚ�ʹ�����ִ�Сд���ַ����Ƚ�
***********************************************************************************/
LIN_EXPORT LIN_ExceptionEnm LIN_GetUpdateInfo(LINExp_NetHandle HNet, LINExp_cpChar strVersion, char** ppRetUpdatePath);

/***********************************************************************************
������ : LIN_DownloadUpdateFile
��  �� : ���ص�ǰApp�Ŀ�����Ŀ���ļ�������ָ��λ��
�������˵�� :
	HNet				: LIN_xxxOpenLinCloud �� pRetHNet���ο�Demo
	SvrUpdatePath 	    : ��ǰApp�������ļ��ڷ�����·����ͨ�� LIN_GetUpdateInfo ȡ��
	SvrDownloadPath	    : ��ǰApp�������ļ�����λ��
�� �� ֵ: LIN_ExceptionEnm�����ͣ������涨���ſ��˽���˼����ϸ˵�������á�LIN_GetRetInfo���鿴
����˵��:
***********************************************************************************/
LIN_EXPORT LIN_ExceptionEnm LIN_DownloadUpdateFile(LINExp_NetHandle HNet, LINExp_cpChar SvrUpdatePath, LINExp_cpChar SvrDownloadPath);



/*���ϣ��Уɵĵ�������
		LIN_CloudClientInit ���óɹ�
				|
		LIN_TouchOpenLinCloud�� LIN_LeeBusOpenLinCloud ���óɹ�
				|
		LIN_LogInLinCloud ���óɹ�
				|
		LIN_InputObjUplowdDataToLinCloud��LIN_LeeBusUplowdDataToLinCloud �������ݻ� OnxxxRcvNetData �ص�ִ��
				|
		LIN_OnNetClose �رջص�
				|
		LIN_LogOutLinCloud
				|
		LIN_CloudClientExit

*/





#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif
