#ifndef mediastream_h
#define mediastream_h

//
#include "mediastreamer2/msfilter.h"

#include "mediastreamer2/msticker.h"
#include "mediastreamer2/mssndcard.h"
#include "mediastreamer2/msaudiomixer.h"
#include "mediastreamer2/msfileplayer.h"
#include "mediastreamer2/msrtp.h"
#include "mediastreamer2/msavi.h"
#include "mediastreamer2/mstee.h"
#include "mediastreamer2/msvideo.h"
#include "mediastreamer2/mswebcam.h"

#define MAX_BRANCHES 16

struct _LaumixBranch{
	struct _Laumix *laumix;
	MSFilter *rtprcv;	
	MSFilter *decoder;	
	int pin;
};
typedef struct _LaumixBranch LaumixBranch;

struct _Laumix {
	MSFilter *mixer;
	MSFilter *soundout;
	MSTicker *ticker;
	int pinused;
	MSFilter *wave_player;//lg
	LaumixBranch branches[MAX_BRANCHES];
};
typedef struct _Laumix Laumix;

struct _LaucetBranch{
	struct _Laucet *laucet;
	MSFilter *rtpsend;	
	int pin;
};
typedef struct _LaucetBranch LaucetBranch;

struct _Laucet {
	MSFilter *capture;
	MSFilter *encoder;
	MSFilter *tee;
	MSTicker *ticker;
	int pinused;
	LaucetBranch branches[MAX_BRANCHES];
};
typedef struct _Laucet Laucet;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//for video multi-branch , multi-sending
struct _LvicetBranch{
	struct _Lvicet *lvicet;
	MSFilter *rtpsend;	
	int pin;
};
typedef struct _LvicetBranch LvicetBranch;

struct _Lvicet {
	MSFilter *capture;
	MSFilter *encoder;
	MSFilter *tee;
	MSTicker *ticker;
	int pinused;
	LvicetBranch branches[MAX_BRANCHES];
};
typedef struct _Lvicet Lvicet;

//receieve decode display
struct _LvirddBranch{
	struct _Lvirdd *lvirdd;
	MSFilter *rtprcv;
	MSFilter *decoder;
	MSFilter *display;	
};
typedef struct _LvirddBranch LvirddBranch;
//just one branch
struct _Lvirdd {
	MSTicker *ticker;
	int pinused;
	LvirddBranch branch;
};
typedef struct _Lvirdd Lvirdd;

///////////////////////////////////////////////////////////////////////////////
//audio stream
///////////////////////////////////////////////////////////////////////////////
typedef struct _AudioStream
{
	RtpSession *session;
	LaucetBranch *aucetbranch;
	LaumixBranch *aumixbranch;
}AudioStream;

typedef struct _AudioStreamRing
{
	LaumixBranch *aumixbranch;
}AudioStreamRing;


///////////////////////////////////////////////////////////////////////////////
//video stream
///////////////////////////////////////////////////////////////////////////////
typedef struct _VideoStream
{
	RtpSession *session;
	LvicetBranch *vicetbranch;
	LvirddBranch *virddbranch;
}VideoStream;

typedef struct _AviStream
{
	MSFilter *avi_write_g711;	
	MSFilter *avi_write_h264;

	MSFilter *g711_decoder;	//for save the pointer
	MSFilter *h264_decoder;  //for save the pointer
}AviStream;

typedef struct _AviStreamOut
{	
	MSFilter *avi_file;
	MSFilter *decoder_au;
	MSFilter *decoder_vi;
    	MSFilter *video_out;	
    	MSFilter *sound_out;
	MSTicker *ticker;
}AviStreamOut;


#ifdef __cplusplus
extern "C" {
#endif

AudioStreamRing * ring_start(const char *file, int interval, MSSndCard *sndcard);
void ring_stop(AudioStreamRing *stream);

AudioStream *audio_stream_start_now(RtpProfile * profile,  const char *remip, int remport, int rem_rtcp_port, int payload, int jitt_comp, MSSndCard *sndcard, int locport);
void audio_stream_stop(AudioStream *stream);
void audio_stream_set_rtcp_information(AudioStream *st, const char *cname, const char *tool);

void audio_stream_mute_send(AudioStream *stream);
void audio_stream_unmute_send(AudioStream *stream);

//bcom
void audio_stream_ring_set_level(AudioStreamRing *stream,int level);
void audio_stream_set_level(AudioStream *stream,int level);
//bcom

VideoStream *video_stream_start_now(RtpProfile * profile,  const char *remip, int remport, int rem_rtcp_port, int payload, int jitt_comp, int locport);
//2012-7
VideoStream *video_stream_start_now_mul(RtpProfile * profile,  const char *remip, int remport, int rem_rtcp_port, int payload, int jitt_comp, int locport);
void video_stream_stop(VideoStream *stream);
void*video_stream_mul_start_rcv_now(VideoStream *stream, RtpProfile * profile,int payload,const char *remip, int remport);

//2012-7
void video_stream_stop_mul(VideoStream *stream);
void video_stream_set_rtcp_information(VideoStream *st, const char *cname, const char *tool);

void video_stream_mute_send(VideoStream *stream);
void video_stream_unmute_send(VideoStream *stream);
void video_stream_send_idr(VideoStream *stream);

AviStream *avi_stream_write_start(AudioStream *austream,VideoStream *vistream,char *filename, /*bcom*/int duration/*bcom*/);
void avi_stream_write_stop(AviStream *avistream);

//bcom
typedef struct _ReadAVI
{
    MSFilter *vdecoder;
    MSFilter *videoout;	
    MSFilter *decoder;
	//bcom
	MSFilter *mixer;
	//bcom
    MSFilter *soundwrite;
    MSFilter *readavi;
	MSTicker *ticker;

}ReadAVI;

ReadAVI * avi_start(const char *infile, MSSndCard *sndcard, MSFilterNotifyFunc avinotify);
void avi_stop(ReadAVI * stream);

void avi_set_audio_level(ReadAVI *stream, int level);

//AviStreamOut *avi_stream_out_start(char *filename,MSSndCard *sndcard, MSFilterNotifyFunc func);
//void avi_stream_out_stop(AviStreamOut *stream );
//bcom

//lg add: record video and audio to avi file
struct _WriteAVI
{
	MSTicker *ticker;
	MSTicker *ticker1;
	MSTicker *ticker2;
	MSFilter *ausource;
	MSFilter *visource;
	MSFilter *H264encoder;
	MSFilter *G711encoder;
	MSFilter *writeG711;
	MSFilter *writeH264;
	MSFilter *au_tee;
	MSFilter *vi_tee;
	MSFilter *videoout;
	MSFilter *soundout;
	MSFilter *mixer;
	MSFilter *mfc_pp;
	MSFilter *wave_player;
};

typedef struct _WriteAVI WriteAVI;

void writeAVI_free(WriteAVI *stream);
WriteAVI *writeAVI_start(MSWebCam *device,/*MSSndCard *sndcard,*/ char *outfile, int v_channel, int duration);
void writeAVI_stop(WriteAVI *stream);
void voip_playWaveFile(char *file_name, int volume, int b_interr);
void writeAVI_playWaveFile(char *file_name, int volume, int b_interr, int b_loop);

struct _ViewCam
{
	MSTicker *ticker;
	MSFilter *visource;
	MSFilter *videoout;
};

typedef struct _ViewCam ViewCam;

ViewCam *viewCam_start(MSWebCam *device, int channel);
void viewCam_stop(ViewCam *stream);

int my_ring_start(char *ring_file_path, int interval);
int my_ring_stop();
void my_ring_set_level(int level);

int playWaveFile(char *wave_file_path, int volume, int b_interruptible);
int stopWaveFile();

//void stopWavePlay();
//void startWavePlay();

int startWaveThread();
int stopWaveThread();

#ifdef __cplusplus
}
#endif
#endif


