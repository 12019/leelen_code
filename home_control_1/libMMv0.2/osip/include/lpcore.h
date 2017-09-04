#ifndef LINPHONECORE_H
#define LINPHONECORE_H


#include <ortp/ortp.h>
#include <ortp/payloadtype.h>
#include <mediastreamer2/mscommon.h>
#include "mediastreamer2/mediastream.h"

#ifdef __cplusplus
extern "C" {
#endif


enum _LinphoneCallDir {
	LinphoneCallOutgoing, /**< outgoing calls*/
	LinphoneCallIncoming  /**< incoming calls*/
};
typedef enum _LinphoneCallDir LinphoneCallDir;
//Enum representing the status of a call
typedef enum _LinphoneCallStatus { 
	LinphoneCallSuccess, /**< The call was sucessful*/
	LinphoneCallAborted, /**< The call was aborted */
	LinphoneCallMissed, /**< The call was missed (unanswered)*/
	LinphoneCallDeclined, /**< The call was declined, either locally or by remote end*/
	LinphoneCallRecord,
} LinphoneCallStatus;
// Structure representing a call log.

typedef struct _LinphoneCallLog{
	LinphoneCallDir dir; /**< The direction of the call*/
	LinphoneCallStatus status; /**< The status of the call*/
	time_t start_time;
	char start_date[128];
	int duration; /**<Duration of the call in seconds*/
	char *user; /**<Originator of the call as a LinphoneAddress object*/
	char *ip;
	char *aviurl;
	//lg
	int b_open;
	//lg
} LinphoneCallLog;





struct _MSSndCard;


typedef struct rtp_config
{
	int audio_rtp_port;
	int video_rtp_port;
	int audio_jitt_comp;  /*jitter compensation*/
	int video_jitt_comp;  /*jitter compensation*/
}rtp_config_t;

typedef struct sound_config
{
	struct _MSSndCard * ring_sndcard;	/* the playback sndcard currently used */
	struct _MSSndCard * play_sndcard;	/* the playback sndcard currently used */
	struct _MSSndCard * capt_sndcard; /* the capture sndcard currently used */
	const char **cards;
	char rec_lev;
	char play_lev;
	char ring_lev;
	char *local_ring;
	char *remote_ring;
} sound_config_t;

typedef struct codecs_config
{
	MSList *audio_codecs;  /* list of audio codecs in order of preference*/
	MSList *video_codecs;	/* for later use*/
}codecs_config_t;

typedef struct video_config{
	bool_t enabled;
}video_config_t;

typedef struct autoreplier_config
{
	int enabled;
	int after_seconds;		/* accept the call after x seconds*/
	int max_users;			/* maximum number of user that can call simultaneously */
	int max_rec_time;  	/* the max time of incoming voice recorded */
	int max_rec_msg;		/* maximum number of recorded messages */
	const char *message;		/* the path of the file to be played */
}autoreplier_config_t;

	
typedef struct _StreamParams
{
	int localport;
	int remoteport;
	char *remoteaddr;
	int pt;
} StreamParams;
typedef enum _notify_t
{

	NID_IDLE_NONE=0,
	NID_IDLE_RCVINV,
	NID_IDLE_RCVMON,
	NID_IDLE_FAILURE,

	NID_ICT_NONE,
	NID_ICT_TIMEOUT,
	//lg
	NID_ICT_OPENTERM,
	NID_ICT_RCVVIDEOOPEN,
	NID_ICT_RCVVIDEOCLOSE,
	//lg
	NID_ICT_RCVBUSY,
	NID_ICT_CONNECTED,
	NID_ICT_NOANSWER,
	NID_ICT_RCVCANCEL,
	NID_ICT_RCVOPEN,
	NID_ICT_TALKING,
	NID_ICT_RECORDING,
	NID_ICT_END,

	NID_IST_NONE,
	NID_IST_TIMEOUT,
	//lg
	NID_IST_OPENTERM,
	NID_IST_RCVOPEN,
	NID_IST_RCVVIDEOOPEN,
	NID_IST_RCVVIDEOCLOSE,
	//lg
	NID_IST_RCVCANCEL,
	NID_IST_RECORDING,
	NID_IST_TALKING,
	NID_IST_END,

	NID_MCT_NONE,
	NID_MCT_TIMEOUT,
	NID_MCT_RCVBUSY,
	NID_MCT_CONNECTED,
	NID_MCT_RCVCANCEL,
	NID_MCT_OPENTERM,
	NID_MCT_END,

	NID_MST_NONE,
	NID_MST_TIMEOUT,
	NID_MST_RCVCANCEL,
	NID_MST_RCVOPEN,
	NID_MST_END,



}notify_t;
typedef void (*notify_f)(struct _LinphoneCore *lc,notify_t notify,char *msg);
//bcom
typedef void (*log_get_calls_num_f)(struct _LinphoneCore *lc, int *num);
//bcom
typedef void (*calllog_f)(struct _LinphoneCore *lc,void *msg);

typedef struct _LinphoneVTable
{
	notify_f notify;
	//bcom
	log_get_calls_num_f log_get_calls_num;
	//bcom
	calllog_f calllog_update;

} LinphoneCoreVTable;


typedef struct _LinphoneCore
{
	bool_t run;
	int port;
	char *user;
	char *sdp;
	LinphoneCoreVTable vtable;
	struct _LpConfig *config;
	//for call log
	LinphoneCallLog *calllog;
	int calls_missed;
	int calls_num;

	
	rtp_config_t rtp_conf;
	sound_config_t sound_conf;
	video_config_t video_conf;
	codecs_config_t codecs_conf;
	autoreplier_config_t autoreplier_conf;
	
	StreamParams audio_params;
	StreamParams video_params;
	
	//lg
	int video_resolution;
	//lg

// 	struct _AudioStream *audiostream;  /**/
// 	struct _RingStream *ringstream;
// 	struct _VideoStream *videostream;
// 	struct _VideoStream *previewstream;
	struct _RtpProfile *local_profile;
	ms_mutex_t lock;
	char *play_file;
	char *rec_file;
	int dw_audio_bw;
	int up_audio_bw;
	int dw_video_bw;
	int up_video_bw;
} LinphoneCore;



//public
LinphoneCore *linphone_core_init (LinphoneCoreVTable *vtable, const char *config_file);
void linphone_core_uninit(LinphoneCore *lc);
LinphoneCore *linphone_core_lc();
void eXosip_reInit(LinphoneCore *lc);
//private 
void sound_config_read(LinphoneCore *lc);
void sip_config_read(LinphoneCore *lc);
void rtp_config_read(LinphoneCore *lc);
void codecs_config_read(LinphoneCore *lc);
void video_config_read(LinphoneCore *lc);


//public
void linphone_core_enable_video(LinphoneCore *lc, bool_t val);

bool_t linphone_core_video_enabled(LinphoneCore *lc);

/* sound functions */
void linphone_core_set_ringback(LinphoneCore *lc,const char *path);
int linphone_core_get_play_level(LinphoneCore *lc);
int linphone_core_get_ring_level(LinphoneCore *lc);
int linphone_core_get_rec_level(LinphoneCore *lc);
void linphone_core_set_ring_level(LinphoneCore *lc, int level);
void linphone_core_set_play_level(LinphoneCore *lc, int level);
void linphone_core_set_rec_level(LinphoneCore *lc, int level);
int linphone_core_set_ringer_device(LinphoneCore *lc, const char * devid);
int linphone_core_set_playback_device(LinphoneCore *lc, const char * devid);
int linphone_core_set_capture_device(LinphoneCore *lc, const char * devid);
const char * linphone_core_get_ringer_device(LinphoneCore *lc);
const char * linphone_core_get_playback_device(LinphoneCore *lc);
const char * linphone_core_get_capture_device(LinphoneCore *lc);
/* returns a static array of string describing the sound devices */ 
const char**  linphone_core_get_sound_devices(LinphoneCore *lc);


void linphone_core_set_ring(LinphoneCore *lc,const char *path);
char *linphone_core_get_ring(LinphoneCore *lc);
//int linphone_core_preview_ring(LinphoneCore *lc, const char *ring,LinphoneCoreCbFunc func,void * userdata);
/*rtp functions*/
int linphone_core_get_audio_jittcomp(LinphoneCore *lc);


int linphone_core_get_audio_port(const LinphoneCore *lc);


int linphone_core_get_video_port(const LinphoneCore *lc);

void linphone_core_set_audio_jittcomp(LinphoneCore *lc, int value);
void linphone_core_set_video_jittcomp(LinphoneCore *lc, int value);

void linphone_core_set_audio_port(LinphoneCore *lc, int port);

void linphone_core_set_video_port(LinphoneCore *lc, int port);

void linphone_core_set_sip(LinphoneCore *lc,int port,char *user);

void payload_type_set_enable(PayloadType *pt,int value) ;

void linphone_core_setup_local_rtp_profile(LinphoneCore *lc);

int linphone_core_set_audio_codecs(LinphoneCore *lc, MSList *codecs);


int linphone_core_set_video_codecs(LinphoneCore *lc, MSList *codecs);




#define PAYLOAD_TYPE_ENABLED	PAYLOAD_TYPE_USER_FLAG_0


//for call log
LinphoneCallLog * linphone_call_log_new(LinphoneCore *lc,char *user, char *ip,LinphoneCallDir dir);
void linphone_call_log_update(LinphoneCore *lc,LinphoneCallStatus status);
void linphone_call_log_completed(LinphoneCore *lc);
void linphone_call_log_destroy(LinphoneCore *lc);
#ifdef __cplusplus
}
#endif

#endif

