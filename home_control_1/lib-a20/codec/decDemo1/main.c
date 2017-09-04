//#define LOG_NDEBUG 0
#define LOG_TAG "main"
#include "CDX_Debug.h"

#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <pthread.h>
#include "render.h"
#include "libcedarv.h"	//* for decoding video
#include "file_parser/pmp_ctrl.h"
#include <sys/time.h>
#include <unistd.h>

static pthread_mutex_t gDecoderMutex = PTHREAD_MUTEX_INITIALIZER;
static const char* gMediaFilePath[4] =
{
	"/mnt/1.pmp",
	"/mnt/2.pmp",
	"/mnt/3.pmp",
	"/mnt/4.pmp"
};

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

	char* file_path;
	int   decode_thread_id;
	long  base_time_sec = 0;
	long  base_time_usec = 0;

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
		//* request picture from libcedarv.
		ret = decoder->display_request(decoder, &picture);		
		if(ret == 0)
		{
			gettimeofday(&tv, &tz);
		//	LOGD("tv.usec = %ld", tv.tv_usec);
		//	LOGD("tv.sec = %ld", tv.tv_sec);
		//	LOGD("picture.pts = %lld", picture.pts);

			if (base_time_sec == 0)
			{
				/* first time*/
				base_time_sec = tv.tv_sec;
				base_time_usec = tv.tv_usec;
				render_render(&picture, picture.id);
			}
			else 
			{
				long long time_spend = (tv.tv_sec - base_time_sec) * 1000 * 1000 + (tv.tv_usec - base_time_usec);
		//		LOGD("time_spend = %ld", time_spend);
				
				if (picture.pts > time_spend)
				{
					//LOGD("need sleep = %lld", picture.pts - time_spend);
					usleep(picture.pts - time_spend);
					render_render(&picture, picture.id);
				}
				else
				{
					render_render(&picture, picture.id);
				}
			}

			decoder->display_release(decoder, picture.id);		//* this decoder operation do not use hardware, so need not lock the mutex.
			continue;
		}

		//* a. get media type of the next data packet.
		ret = GetNextChunkInfo(parser, (unsigned int*)&pkt_type, (unsigned int*)&pkt_length);
		if(ret < 0)
		{
			LOGD("get packet information fail, may be file end.");
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
			
			LOGV("decoder %d return %d\n", decode_thread_id, ret);
			if(ret == CEDARV_RESULT_ERR_NO_MEMORY || ret == CEDARV_RESULT_ERR_UNSUPPORTED)
			{
				LOGD("bit stream is unsupported.\n");
				break;
			}
		}
		else
		{
			//* skip audio or other media packets.
			SkipChunkData(parser);
		}

	}while(1);

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

	LOGD("begin cedarx hardware init.\n");
	cedarx_hardware_init(0);
		
	render_init();
	LOGD("create four decode task.\n");

	pthread_create(&t0, NULL, decode_thread, (void*)0);

	pthread_join(t0, (void**)&ret);
	LOGD("decode task finish.\n");
	
	render_exit();
	LOGD("begin cedarx hardware exit.\n");
	cedarx_hardware_exit(0);

	LOGD("success, quit\n");
	return 0;
}



