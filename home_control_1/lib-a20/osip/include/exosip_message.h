
#ifndef _JMESSAGE_H_
#define _JMESSAGE_H_

#include "lpcore.h"
#include "exosip_main.h"
#include "eXosip_t_list.h"
//for cmd field
typedef enum msg_cmd_t{
	CNONE=0,
	INVITE,				//REQ
	MONITOR,


	
	ACKOK=0x10,			//ACK
	ACKBUSY,
	ACKNO,
	ACKLAST,



	CANCEL=20,			//OTHER
	HOOK,
	OPEN,
	REC,
	//lg
	VIDEOOPEN,
	VIDEOCLOSE,
	CANCELSELF,
	//lg
}msg_cmd_t;

typedef enum msg_type_t{
	REQ=0,				//REQ
	ACK=1,				//ACK
	OTHER=2,			//OTHER
}msg_type_t;

typedef struct msg_header{
	uint16_t cmd:8;			//command
	uint16_t type:8;			//ACK TYPE
	uint32_t cid;				//a session id
}msg_header_t;

#define MSG_KEY_FROM 	"From"
#define MSG_KEY_AUDIO	"Audio"
#define MSG_KEY_VIDEO  	"Video"
#define MSG_KEY_AUDIO_PORT		"AudioPort"
#define MSG_KEY_VIDEO_PORT  	"VideoPort"

#define MSG_FROM 	"From="
#define MSG_TO		"To="
#define MSG_AUDIO	"Audio="
#define MSG_VIDEO  	"Video="
#define MSG_AUDIO_PORT		"AudioPort="
#define MSG_VIDEO_PORT  	"VideoPort="



typedef struct msg_item_t{
	char *key;
	char *value;
}msg_item_t;

#define MSG_IS_REQ(type)  ((type)==REQ)
#define MSG_IS_ACK(type)  ((type)==ACK)
#define MSG_IS_OTHER(type)	((type)==OTHER)


#define MSG_MAX_LEN 512

int msg_snd_invite(LinphoneCore *lc, char *args);
int msg_snd_invite_again(void);

int 
msg_snd_ack(msg_cmd_t cmd,uint32_t cid);

int 
msg_snd_ack_req(LinphoneCore *lc,uint32_t cid);

void 
msg_snd_cancle_cmd(void);
int 
msg_snd_cancle(void);
void 
msg_snd_open_cmd(void);
int 
msg_snd_open(void);

void 
msg_snd_hook_cmd(void);
int 
msg_snd_hook(void);
int
msg_snd_mon(LinphoneCore *lc, char *args);

int
msg_snd_mon_again(void);

void 
msg_snd_shut_mon_cmd(void);

void 
msg_snd_shut_mon(void);

void
jparse_msg(char *buf, char *ipaddress);

void
msg_rcv_invite(char *buf);

void
msg_rcv_mon(char *buf);

void
msg_rcv_ack(char *buf,type_t type);

void
msg_rcv_cancle(char *buf);

void
msg_rcv_hook(char *buf);

void
msg_rcv_open(char *buf);

void
msg_list_free(osip_list_t * msg_list);

int 
msg_snd_cancle_monitor_list(void);

void
msg_rcv_mul_mon(char *ipaddress,char *buf,msg_header_t * msg);

void
msg_rcv_ack_mul_mon(char *buf,msg_header_t * msg);

void
msg_rcv_cancle_mul_mon(char *buf,msg_header_t * msg);

void
msg_rcv_open_mul_mon(char *buf,msg_header_t * msg);

void
msg_snd_cancle_mon_cmd(char *args);


#endif

