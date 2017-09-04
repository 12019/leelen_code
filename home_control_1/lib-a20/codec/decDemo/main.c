#define LOG_NDEBUG 0
#define LOG_TAG "main"
#include <CDX_Debug.h>

#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <pthread.h>

#include <linux/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "drv_display.h"
#include <semaphore.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

#include "libcedarv.h"	//* for decoding video
#include "file_parser/pmp_ctrl.h"
#include <sys/time.h>
#include <unistd.h>


static pthread_mutex_t gDecoderMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t gSemMutex = PTHREAD_MUTEX_INITIALIZER;


static const char* gMediaFilePath[4] =
{
	"/mnt/1.pmp",
	"/mnt/2.pmp",
	"/mnt/3.pmp",
	"/mnt/4.pmp"
};

//#define DRAGON_BOARD

static int hlay = 0;
static int disp_fd = 0;
static __disp_layer_info_t lay_info;

static int disp_thread_quit = 0;
static int fb_fd = 0;
struct fb_var_screeninfo var;
struct fb_fix_screeninfo fix;
static unsigned int *fb10_phy, *fb11_phy, *fb10_vir, *fb11_vir;
static int buf_no = 0;

unsigned int cnt_4frame;
unsigned int decode_finished[4] = {0, 0, 0, 0};

sem_t sem_scaler;
sem_t sem_show;
sem_t sem_tid[4];
static int cur_cnt = 4;

static int pos[4][2] =
{
		{0  ,   0},
		{640,   0},
		{0  , 360},
		{640, 360}
};

void FbPanDisp(int i)
{
   	var.yoffset		= i * var.yres;
   	ioctl(fb_fd, FBIOPAN_DISPLAY,&var);
}

void FbInit()
{
	int arg[4];
	__disp_fb_create_para_t fb_para;
	int ret = 0;
	fb_para.mode		= DISP_LAYER_WORK_MODE_NORMAL;
	fb_para.fb_mode 	= FB_MODE_SCREEN0;
	fb_para.buffer_num	= 2;
	fb_para.width		= 1280;
	fb_para.height		= 720;
	arg[0] = 1; //????fb1
	arg[1] = (__u32)&fb_para;
	ret = ioctl(disp_fd, DISP_CMD_FB_REQUEST, (__u32)arg);

	fb_fd = open("/dev/fb1", O_RDWR);
	ioctl(fb_fd, FBIOGET_FSCREENINFO, &fix);
	ioctl(fb_fd, FBIOGET_VSCREENINFO, &var);
	fb10_phy = (unsigned int *)fix.smem_start;
	//fLOGD(stderr, "fb10_phy:%x", fb10_phy);
	fb11_phy = fb10_phy + var.xres * var.yres;
	fb10_vir = mmap(NULL, fix.smem_len, PROT_READ|PROT_WRITE, MAP_SHARED, fb_fd, 0);
	fb11_vir = fb10_vir + var.xres * var.yres;
}

void FbExit()
{
	munmap(fb10_vir, fix.smem_len);
	close(fb_fd);
	int arg[4];
	arg[0] = 1; //?ͷ?fb1
	ioctl(disp_fd, DISP_CMD_FB_RELEASE, (__u32)arg);
}

void DispInit()
{
	int arg[4] = {0};
	disp_fd = open("/dev/disp", O_RDWR);
	arg[0] = 0;
	arg[1] = DISP_LAYER_WORK_MODE_NORMAL;
	hlay = ioctl(disp_fd, DISP_CMD_LAYER_REQUEST, arg);
	LOGD("hlay:%d\n",hlay);
	if (hlay < 0) {
		LOGD("layer request failed\n");
		return ;
	}
	ioctl(disp_fd,DISP_CMD_HDMI_OFF,arg);
	
	arg[1]=DISP_TV_MOD_720P_60HZ;
    ioctl(disp_fd,DISP_CMD_HDMI_ON,arg);
	FbInit();

	sem_init(&sem_scaler, 0, 1);
	sem_init(&sem_show, 0, 0);
	cnt_4frame = 0;
	sem_init(&sem_tid[0], 0, 1);
	sem_init(&sem_tid[1], 0, 1);
	sem_init(&sem_tid[2], 0, 1);
	sem_init(&sem_tid[3], 0, 1);
}

void DispExit( )
{
	int arg[4] = {0};

	arg[0] = 0;
	arg[1] = hlay;
	ioctl(disp_fd, DISP_CMD_LAYER_CLOSE, arg);
	ioctl(disp_fd, DISP_CMD_LAYER_RELEASE, arg);
	sem_destroy(&sem_scaler);
	FbExit();
	close(disp_fd);
}

void SetDispArg(cedarv_picture_t picture, int i)
{
	int arg[4]={0,};
	int scaler_value;
	int width 		= picture.width;
	int height 		= picture.height;

	sem_wait(&sem_tid[i]);
	sem_wait(&sem_scaler);

	int scaler_hdl = ioctl(disp_fd, DISP_CMD_SCALER_REQUEST, (unsigned long)arg);
	__disp_scaler_para_t para;
	para.input_fb.addr[0]		= (unsigned int)picture.y;
	para.input_fb.addr[1]		= (unsigned int)picture.u;
	para.input_fb.size.width	= width;
	para.input_fb.size.height	= height;
	para.input_fb.mode			= DISP_MOD_MB_UV_COMBINED;
	para.input_fb.format		= DISP_FORMAT_YUV420;
	para.input_fb.br_swap		= 0;
	para.input_fb.seq			= DISP_SEQ_UVUV;
	para.input_fb.cs_mode		= DISP_BT601;
	para.input_fb.b_trd_src		= 0;
	para.source_regn.x			= 0;
	para.source_regn.y			= 0;
	para.source_regn.width		= width;
	para.source_regn.height		= height;
	para.output_fb.addr[0]		= (unsigned int)fb10_phy;
	para.output_fb.size.width	= 1280;	//DISP_CMD_SCALER_EXECUTE_EX??Ļ?? DISP_CMD_SCALER_EXECUTE????ͼ????
	para.output_fb.size.height	= 720*2;  //DISP_CMD_SCALER_EXECUTE_EX??Ļ?? DISP_CMD_SCALER_EXECUTE????ͼ????
	para.output_fb.mode			= DISP_MOD_INTERLEAVED;
	para.output_fb.format		= DISP_FORMAT_ARGB8888;
	para.output_fb.br_swap		= 0;
	para.output_fb.seq			= DISP_SEQ_ARGB;
	para.output_fb.cs_mode		= DISP_YCC;
	para.out_regn.x 			= pos[i][0]; //DISP_CMD_SCALER_EXECUTE??Ч
	para.out_regn.y 			= pos[i][1]+buf_no*var.yres; //DISP_CMD_SCALER_EXECUTE??Ч
	para.out_regn.width			= 640;	//DISP_CMD_SCALER_EXECUTE_EX???ź?ͼ???? DISP_CMD_SCALER_EXECUTE??Ч
	para.out_regn.height		= 360; //DISP_CMD_SCALER_EXECUTE_EX???ź?ͼ???? DISP_CMD_SCALER_EXECUTE??Ч
	arg[1] = scaler_hdl;
	arg[2] = (__u32)&para;
	ioctl(disp_fd, DISP_CMD_SCALER_EXECUTE_EX, (__u32)arg);
	arg[1] = scaler_hdl;
	ioctl(disp_fd, DISP_CMD_SCALER_RELEASE, (unsigned long)arg);

	sem_post(&sem_scaler);

	//LOGD("cnt_4frame = %d, cur_cnt = %d", cnt_4frame, cur_cnt);
	pthread_mutex_lock(&gSemMutex);
	cnt_4frame++;
	if (cnt_4frame >= cur_cnt) {
		cnt_4frame = 0;
		sem_post(&sem_show);
	}
	pthread_mutex_unlock(&gSemMutex);
	
}

void* show_thread(void* param)
{	
	static int opened = 0;
	int arg[4] = {0};
	__disp_rectsz_t size;
	
	arg[0] = 0;

	while (1) 
	{
		if (disp_thread_quit == 1)
		{
			break;
		}

		sem_wait(&sem_show);

		size.width 	= 1280;
		size.height	= 720;
		lay_info.fb.addr[0]		= (__u32)(fb10_phy);
		lay_info.fb.size		= size;
		lay_info.fb.br_swap 	= 0;
		lay_info.fb.cs_mode 	= DISP_BT601;
		lay_info.mode			= DISP_LAYER_WORK_MODE_NORMAL;
		lay_info.pipe			= 0;
		lay_info.alpha_val		= 0xff;
		lay_info.alpha_en		= 1;
		lay_info.ck_enable		= 1;

		lay_info.fb.mode		= DISP_MOD_INTERLEAVED;
		lay_info.fb.seq 		= DISP_SEQ_ARGB;
		lay_info.fb.format		= DISP_FORMAT_ARGB8888;

		lay_info.src_win.x		= 0;
		lay_info.src_win.y		= 0;
		lay_info.src_win.width	= 1280;
		lay_info.src_win.height = 720*2;

		lay_info.scn_win.x		= 0;
		lay_info.scn_win.y		= buf_no*var.yres;
		lay_info.scn_win.width	= 1280;
		lay_info.scn_win.height = 720;

		arg[1] = hlay;
		arg[2] = (__u32)(&lay_info);
		if (ioctl(disp_fd, DISP_CMD_LAYER_SET_PARA, arg) < 0) {
			LOGD("fail to set para\n");
			return (void*)0;
		}

		if (!opened) {
			if (ioctl(disp_fd, DISP_CMD_LAYER_TOP, arg) < 0) {
				LOGD("DISP_CMD_LAYER_TOP\n");
				return (void*)0;
			}

			/* 3.DISP */
			if (ioctl(disp_fd, DISP_CMD_LAYER_OPEN, arg) < 0) {
				LOGD("DISP_CMD_LAYER_OPEN\n");
				return (void*)0;
			}
			opened = 1;
		}

		buf_no = 1 - buf_no;

		if (decode_finished[0] == 0) sem_post(&sem_tid[0]);
		if (decode_finished[1] == 0) sem_post(&sem_tid[1]);
		if (decode_finished[2] == 0) sem_post(&sem_tid[2]);
		if (decode_finished[3] == 0) sem_post(&sem_tid[3]);
	}
}

void* decode_thread(void* param)
{
	int            ret;
	int            pkt_type;
	int            pkt_length;
	unsigned char* buf0;
	unsigned int   bufsize0;
	unsigned char* buf1;
	unsigned int   bufsize1;

	cedarv_stream_data_info_t data_info;
	cedarv_stream_info_t      stream_info;
	cedarv_picture_t          picture;
	struct timeval tv;
	struct timezone tz;


	void*             parser;
	cedarv_decoder_t* decoder;
	long  base_time_sec = 0;
	long  base_time_usec = 0;


	char* file_path;
	int   decode_thread_id;
	//*********************************************************************
	decode_thread_id = (int)param;
	file_path = (char*)gMediaFilePath[decode_thread_id];
	LOGD("before openMediaFile ok\n");
	//*********************************************************************
	//* 1. open the file parser.
	//*********************************************************************
	ret = OpenMediaFile(&parser, file_path);
	if(ret < 0)
	{
		LOGD("can not create file parser for media file %s\n", file_path);
		return (void*)-1;
	}

	LOGD("openMediaFile ok\n");
	//*********************************************************************
	//* 2. open the decoder.
	//*********************************************************************

	//* a. initialize libcedarv.
	pthread_mutex_lock(&gDecoderMutex);
	decoder = cedarvDecInit(&ret);
	if (ret < 0)
	{
		LOGD("can not initialize the decoder library.\n");
		pthread_mutex_unlock(&gDecoderMutex);
		CloseMediaFile(&parser);
		return (void*)-1;
	}
	pthread_mutex_unlock(&gDecoderMutex);
	LOGD("cedarvDecInit ok\n");

	//* b. set video stream information to libcedarv.
	GetVideoStreamInfo(parser, &stream_info);
	LOGD("getVideoStreamInfo ok\n");

	decoder->set_vstream_info(decoder, &stream_info);		//* this decoder operation do not use hardware, so need not lock the mutex.
	LOGD("set vstream info ok\n");

	//* c. open libcedarv.
	pthread_mutex_lock(&gDecoderMutex);
	ret = decoder->open(decoder);
	pthread_mutex_unlock(&gDecoderMutex);
	if(ret < 0)
	{
		LOGD("can not open decoder.\n");
		if(stream_info.init_data)
			free(stream_info.init_data);
		CloseMediaFile(&parser);

		pthread_mutex_lock(&gDecoderMutex);
		cedarvDecExit(decoder);
		pthread_mutex_unlock(&gDecoderMutex);

		return (void*)-1;
	}

	LOGD("decoder open ok\n");
	pthread_mutex_lock(&gDecoderMutex);
	decoder->ioctrl(decoder, CEDARV_COMMAND_PLAY, 0);
	pthread_mutex_unlock(&gDecoderMutex);

	//*********************************************************************
	//* 3. decoding loop.
	//*********************************************************************
	do
	{
		//fLOGD(stderr, "%d (%d)\n",decode_thread_id, 0);
		//* a. get media type of the next data packet.
		ret = GetNextChunkInfo(parser, (unsigned int*)&pkt_type, (unsigned int*)&pkt_length);
		if(ret < 0)
		{
			LOGD("get packet information fail, may be file end.");
			pthread_mutex_lock(&gSemMutex);
			cur_cnt --;
			
			decode_finished[decode_thread_id] = 1;

			pthread_mutex_unlock(&gSemMutex);
			break;
		}
		//* b. read packet to decoder and decode.
		if(pkt_type == VIDEO_PACKET_TYPE)
		{
			//* request bit stream data buffer from libcedarv.
			if(pkt_length == 0)	//* for some file format, packet length may be unknown.
				pkt_length = 64*1024;
_read_again:
			ret = decoder->request_write(decoder, pkt_length, &buf0, &bufsize0, &buf1, &bufsize1);
			if(ret < 0)
			{
				//* request bit stream data buffer fail, may be the bit stream FIFO is full.
				//* in this case, we should call decoder->decode(...) to decode stream data and release bit stream buffer.
				//* here we just use a single thread to do the data parsing/decoding/picture requesting work, so it is
				//* invalid to see that the bit stream FIFO is full.
				LOGD("request bit stream buffer fail.\n");
				break;
			}
			//* read bit stream data to the buffer.
			GetChunkData(parser, buf0, bufsize0, buf1, bufsize1, &data_info);
			//* tell libcedarv stream data has been added.
			decoder->update_data(decoder, &data_info);		//* this decoder operation do not use hardware, so need not lock the mutex.
			//* decode bit stream data.
			pthread_mutex_lock(&gDecoderMutex);
			ret = decoder->decode(decoder);
			pthread_mutex_unlock(&gDecoderMutex);
//			LOGD("thread %d return %d\n", decode_thread_id, ret);
			if(ret == CEDARV_RESULT_ERR_NO_MEMORY || ret == CEDARV_RESULT_ERR_UNSUPPORTED)
			{
				LOGD("bit stream is unsupported.\n");
				break;
			}
			//* request picture from libcedarv.
			ret = decoder->display_request(decoder, &picture);		//* this decoder operation do not use hardware, so need not lock the mutex.
			if(ret == 0)
			{

				gettimeofday(&tv, &tz);

				if (base_time_sec == 0)
				{
					/* first time*/
					base_time_sec = tv.tv_sec;
					base_time_usec = tv.tv_usec;
					SetDispArg(picture, decode_thread_id);
				}
				else 
				{
					long long time_spend = (tv.tv_sec - base_time_sec) * 1000 * 1000 + (tv.tv_usec - base_time_usec);
					if (picture.pts > time_spend)
					{
						usleep(picture.pts - time_spend);
						SetDispArg(picture, decode_thread_id);
					}
					else
					{
						SetDispArg(picture, decode_thread_id);
					}
				}

				//* release the picture to libcedarv.
				decoder->display_release(decoder, picture.id);		//* this decoder operation do not use hardware, so need not lock the mutex.
			}
			//fLOGD(stderr, "%d (%d)\n",decode_thread_id, 6);
		}
		else
		{
			//* skip audio or other media packets.
			SkipChunkData(parser);
		}

	} while(1);

	//* 4. close the decoder and parser.
	pthread_mutex_lock(&gDecoderMutex);
	decoder->ioctrl(decoder, CEDARV_COMMAND_STOP, 0);
	pthread_mutex_unlock(&gDecoderMutex);

	pthread_mutex_lock(&gDecoderMutex);
	decoder->close(decoder);
	cedarvDecExit(decoder);
	pthread_mutex_unlock(&gDecoderMutex);

	if(stream_info.init_data)
		free(stream_info.init_data);

	CloseMediaFile(&parser);

	return (void*)0;
}

int main(int argc, char** argv)
{
	int ret;
	pthread_t t0;
	pthread_t t1;
	pthread_t t2;
	pthread_t t3;
	pthread_t t4;

	LOGD("begin cedarx hardware init.\n");
	cedarx_hardware_init(0);
		
	LOGD("create four decode task.\n");
	DispInit();

	pthread_create(&t0, NULL, decode_thread, (void*)0);
#ifndef DRAGON_BOARD
	pthread_create(&t1, NULL, decode_thread, (void*)1);
	pthread_create(&t2, NULL, decode_thread, (void*)2);
	pthread_create(&t3, NULL, decode_thread, (void*)3);
	pthread_create(&t4, NULL, show_thread,   (void*)4);
#endif
	pthread_join(t0, (void**)&ret);
	LOGD("decode task t0 finish.\n");
#ifndef DRAGON_BOARD
	pthread_join(t1, (void**)&ret);
	LOGD("decode task t1 finish.\n");
	pthread_join(t2, (void**)&ret);
	LOGD("decode task t2 finish.\n");
	pthread_join(t3, (void**)&ret);
	LOGD("decode task t3 finish.\n");

	disp_thread_quit = 1; 
	sem_post(&sem_show);
	pthread_join(t4, (void**)&ret);
#endif

	LOGD("decode task all finish.\n");
	
	LOGD("begin cedarx hardware exit.\n");
	cedarx_hardware_exit(0);
	DispExit();
	LOGD("success, quit\n");
	return 0;
}



