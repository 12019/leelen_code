#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <stdint.h>
#include <QEvent>
#include <QString>
#include <QStyledItemDelegate>

typedef QEvent::Type ltype;
class Levent:public QEvent
{
public:
   Levent(ltype t, QString message) : QEvent(Type(t))
   {
	  msg = message;
   }

   ~Levent()
   {
   }

   QString msg;
};

#define TABLE_VIEW_COLOR QColor(239, 243, 255)

void  PostEventWithMessage(QObject *receiver, QEvent::Type type, QString message);

#define PACKAGE_FLAG_UINT8		0xff
#define PACKAGE_FLAG_UINT32		0xfb
#define CMD_TYPE_INFO_SEND		143
#define CMD_TYPE_HOME_CONTROL	0xf0
#define CMD_TYPE_REMOTECHECK    0xf1

//信息发布
typedef struct {
	unsigned char PackageFlag;
	union
	{
		uint8_t       PackageLength8;
		uint32_t      PackageLength32;
	} PackageLength;
	uint8_t       CmdType;
	//void*         pPackageData;
} REQUEST_FRAME_T;

typedef struct {
	unsigned char PackageFlag;
	union
	{
		int8_t       PackageLength8;
		int32_t      PackageLength32;
	} PackageLength;
	unsigned char Result;
	//void*         pResultData;
} RESPONSE_FRAME_T;

/*
uint16_t	SubType;							// 当前为0
uint16_t	SenderNameLen;						// 发送者的长度
uint8_t		SenderNameData[SenderNameLen]		// 发送者内容
uint16_t	TitleLen							// 标题内容的长度
uint8_t		TitleData[TitleLen]					// 标题内容   如TitleLen=0, 则没有TitleData成员
uint16_t	MsgLen								// 文字信息内容的长度
uint8_t     MsgData[MsgLen]						// 文字信息内容   如MsgLen=0, 则没有MsgData成员

uint16_t	PicCount							// 图片个数

uint16_t	Pic0FileNameLen						// 第一个图片的文件名长度
uint8_t		Pic0FileNameData[Pic0FileNameLen]	// 第一个图片的文件名
uint32_t    Pic0Len								// 第一个图片文件的长度
uint8_t     Pic0Data[Pic0Len]					// 第一个图片文件的数据

uint16_t	Pic1FileNameLen						// 第二个图片的文件名长度
uint8_t		Pic1FileNameData[Pic1FileNameLen]	// 第二个图片的文件名
uint32_t    Pic1Len								// 第二个图片文件的长度
uint8_t     Pic1Data[Pic1Len]					// 第二个图片文件的数据
...
*/

//17731端口 终端求助
/*struct RequestPackageHead{
	uint8_t flag;
	uint8_t cmd;
	uint16_t length;
};

struct AnswerPackageHead{
	uint8_t flag;
	uint8_t Result;
	uint16_t length;
};

struct PackageBody{
	uint16_t device_class;
	uint16_t device_type;
	uint32_t device_id;
	uint32_t alerttype;
	uint32_t alertdatalength;
	uint8_t alertdata[4];
};*/

#define FULLSCREEN_BORDER_WIDTH 1 //6

#define TABLE_ROWS_PER_PAGE		9

#define TOUCHZ   0

#define TOUCHR   1
//david 2014.3.30
int psystem(const char *cmd);
void systemRunLog(int nOption);
#endif // DATA_TYPES_H
