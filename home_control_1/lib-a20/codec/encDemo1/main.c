#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define __OS_ANDROID

#define LOG_NDEBUG 0
#define LOG_TAG "venc-file"
#include "CDX_Debug.h"

#include "H264encLibApi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "venc.h"
#include "CameraSource.h"
#include "water_mark.h"
#include "text.h"
#include "V4L2.h"
#include <time.h>
#include <sys/time.h>
#include "cedarv_osal_linux.h"

unsigned int mwidth = 640;
unsigned int mheight = 480;

#define ENCODE_H264 
//#define ENCODE_MJPEG

typedef struct Venc_context
{
	cedarv_encoder_t *venc_device;
	VencBaseConfig base_cfg;
	VencInputBuffer input_buffer;
	VencOutputBuffer output_buffer;
	VencAllocateBufferParam alloc_parm;
	VencSeqHeader header_data;
	AWCameraDevice *CameraDevice;	
	pthread_t thread_enc_id;
	int mstart;
	FILE *out_file;
	WaterMark*	waterMark;
}Venc_context;


int CameraSourceCallback(void *cookie,  void *data)
{
	Venc_context * venc_cam_cxt = (Venc_context *)cookie;
	cedarv_encoder_t *venc_device = venc_cam_cxt->venc_device;
	AWCameraDevice *CameraDevice = venc_cam_cxt->CameraDevice;

	VencInputBuffer input_buffer;
	int result = 0;
	int has_alloc_buffer = 0;
	struct v4l2_buffer *p_buf = (struct v4l2_buffer *)data;
	v4l2_mem_map_t* p_v4l2_mem_map = GetMapmemAddress(getV4L2ctx(CameraDevice));	

	void *buffer = (void *)p_v4l2_mem_map->mem[p_buf->index];
	int size_y = venc_cam_cxt->base_cfg.input_width*venc_cam_cxt->base_cfg.input_height; 

	memset(&input_buffer, 0, sizeof(VencInputBuffer));

	input_buffer.id = p_buf->index;
	input_buffer.addrphyY = p_buf->m.offset;
	input_buffer.addrphyC = p_buf->m.offset + size_y;

	LOGD("buffer address is %x", buffer);
	input_buffer.addrvirY = buffer;
	input_buffer.addrvirC = buffer + size_y;

	input_buffer.pts = 1000000 * (long long)p_buf->timestamp.tv_sec + (long long)p_buf->timestamp.tv_usec;
	LOGD("pts = %ll", input_buffer.pts);

#if 1
	if(input_buffer.addrphyY >=  0x40000000)
		input_buffer.addrphyY -=0x40000000;
#endif

	if(!venc_cam_cxt->mstart) {
		LOGD("p_buf->index = %d\n", p_buf->index);
		CameraDevice->returnFrame(CameraDevice, p_buf->index);
	}

    // enquene buffer to input buffer quene
    
	//LOGD("ID = %d\n", input_buffer.id);
	result = venc_device->ioctrl(venc_device, VENC_CMD_ENQUENE_INPUT_BUFFER, &input_buffer);

	if(result < 0) {
		CameraDevice->returnFrame(CameraDevice, p_buf->index);
		LOGW("video input buffer is full , skip this frame");
	}

	return 0;
}

static void* EncoderThread(void* pThreadData) {

	Venc_context * venc_cam_cxt = (Venc_context *)pThreadData;
	cedarv_encoder_t *venc_device = venc_cam_cxt->venc_device;
	AWCameraDevice *CameraDevice = venc_cam_cxt->CameraDevice;
	
	time_t	now;         
	struct	tm   *timenow;
	VencInputBuffer input_buffer;
	int result = 0;
	int motion_flag = 0;

	int count = 0;
	VencOutputBuffer output_buffer;

	while(venc_cam_cxt->mstart)
	{

		memset(&input_buffer, 0, sizeof(VencInputBuffer));

		// dequene buffer from input buffer quene;
	    result = venc_device->ioctrl(venc_device, VENC_CMD_DEQUENE_INPUT_BUFFER, &input_buffer);

		if(result<0)
		{
			//LOGV("enquene input buffer is empty");
			usleep(10*1000);
			continue;
		}

		venc_cam_cxt->waterMark->bgInfo.y = input_buffer.addrvirY;
		venc_cam_cxt->waterMark->bgInfo.c = input_buffer.addrvirC;

		waterMarkShowTime(venc_cam_cxt->waterMark);

		cedarx_cache_op(input_buffer.addrvirY, 
				input_buffer.addrvirY + venc_cam_cxt->waterMark->bgInfo.width * venc_cam_cxt->waterMark->bgInfo.height * 3/2, 0);

		result = venc_device->ioctrl(venc_device, VENC_CMD_ENCODE, &input_buffer);

		CameraDevice->returnFrame(CameraDevice, input_buffer.id);
		
		static int count = 0;
		if(result == 0)
		{
			memset(&output_buffer, 0, sizeof(VencOutputBuffer));
			result = venc_device->ioctrl(venc_device, VENC_CMD_GET_BITSTREAM, &output_buffer);

#ifdef ENCODE_H264
			fwrite(output_buffer.ptr0, 1, output_buffer.size0, venc_cam_cxt->out_file);
			if(output_buffer.size1)
			{
				fwrite(output_buffer.ptr1, 1, output_buffer.size1, venc_cam_cxt->out_file);
			}
#else
			if (count == 10) 
			{
				LOGD("output_buffer.size0 = %d", output_buffer.size0);
				LOGD("output_buffer.size1 = %d", output_buffer.size1);
				fwrite(output_buffer.ptr0, 1, output_buffer.size0, venc_cam_cxt->out_file);
				if(output_buffer.size1)
				{
					fwrite(output_buffer.ptr1, 1, output_buffer.size1, venc_cam_cxt->out_file);
				}
				fclose(venc_cam_cxt->out_file);
				venc_cam_cxt->out_file = NULL;
				count ++;
			}
			else if (count != 11)
			{
				count ++;
			}
#endif
		
			venc_device->ioctrl(venc_device, VENC_CMD_GET_MOTION_FLAG, &motion_flag);
			if (motion_flag == 1)
			{
				/*检测到移动侦测*/
				LOGD("motion_flag = %d", motion_flag);
			}

			if(result == 0)
			{
				venc_device->ioctrl(venc_device, VENC_CMD_RETURN_BITSTREAM, &output_buffer);
			}
			
		}
		else 
		{
			LOGD("encoder fatal error");
		}
	}
	
	return (void*)0;
}

int main()
{

	int err = 0;
	motion_par motionPar;
	Venc_context *venc_cam_cxt = (Venc_context *)malloc(sizeof(Venc_context));

	memset(venc_cam_cxt, 0, sizeof(Venc_context));

	// init base config param
#ifdef ENCODE_H264
	venc_cam_cxt->base_cfg.codectype = VENC_CODEC_H264;
#else
	venc_cam_cxt->base_cfg.codectype = VENC_CODEC_JPEG;
#endif
	
	venc_cam_cxt->base_cfg.framerate = 30;
	venc_cam_cxt->base_cfg.input_width = mwidth;
	venc_cam_cxt->base_cfg.input_height= mheight;
	venc_cam_cxt->base_cfg.dst_width = mwidth;
	venc_cam_cxt->base_cfg.dst_height = mheight;
	venc_cam_cxt->base_cfg.maxKeyInterval = 25;
	venc_cam_cxt->base_cfg.inputformat = VENC_PIXEL_YUV420; //uv combined
	venc_cam_cxt->base_cfg.targetbitrate = 3*1024*1024;

	LOGD("cedarx_hardware_init");
	cedarx_hardware_init(0);
	
	LOGD("Codec version = %s", getCodecVision());
	venc_cam_cxt->waterMark = malloc(sizeof(WaterMark));	
	memset(venc_cam_cxt->waterMark, 0x0, sizeof(WaterMark));

	venc_cam_cxt->waterMark->bgInfo.width = mwidth;
	venc_cam_cxt->waterMark->bgInfo.height = mheight;
	venc_cam_cxt->waterMark->srcPathPrefix = "/mnt/res/icon_720p_";
	venc_cam_cxt->waterMark->srcNum = 13;
	waterMarkInit(venc_cam_cxt->waterMark);

	/*移动侦测*/
	motionPar.motion_detect_enable = 1;
	motionPar.motion_detect_ratio = 0;

	venc_cam_cxt->venc_device = cedarvEncInit();
	venc_cam_cxt->venc_device->ioctrl(venc_cam_cxt->venc_device, VENC_CMD_BASE_CONFIG, &venc_cam_cxt->base_cfg);
	venc_cam_cxt->venc_device->ioctrl(venc_cam_cxt->venc_device, VENC_CMD_OPEN, 0);
	venc_cam_cxt->venc_device->ioctrl(venc_cam_cxt->venc_device, VENC_CMD_HEADER_DATA, &venc_cam_cxt->header_data);
	venc_cam_cxt->venc_device->ioctrl(venc_cam_cxt->venc_device, VENC_CMD_SET_MOTION_PAR_FLAG, &motionPar);

#ifdef ENCODE_H264
    venc_cam_cxt->out_file = fopen("/tmp/out1.h264", "wb");
#else
	venc_cam_cxt->out_file = fopen("/mnt/out1.jpg", "wb");
#endif
	if(venc_cam_cxt->out_file == NULL) {
		LOGE("open file error");
		return 0;
	}

	fwrite(venc_cam_cxt->header_data.bufptr, 1, venc_cam_cxt->header_data.length, venc_cam_cxt->out_file);

	LOGD("create encoder ok");

	/* create source */
	venc_cam_cxt->CameraDevice = CreateCamera(mwidth, mheight);
	

	LOGD("create camera ok");

	/* set camera source callback */

	venc_cam_cxt->CameraDevice->setCameraDatacallback(venc_cam_cxt->CameraDevice, 
		(void *)venc_cam_cxt, (void *)&CameraSourceCallback);

	/* start camera */
	venc_cam_cxt->CameraDevice->startCamera(venc_cam_cxt->CameraDevice);

	/* start encoder */
	venc_cam_cxt->mstart = 1;
	/* create encode thread*/
	err = pthread_create(&venc_cam_cxt->thread_enc_id, NULL, EncoderThread, venc_cam_cxt);
	if (err || !venc_cam_cxt->thread_enc_id) {
		LOGE("Create thread_enc_id fail !\n");
	}

	sleep(30);

    /* stop encoder */
	venc_cam_cxt->mstart = 0;
	if(venc_cam_cxt->thread_enc_id !=0) {                 
        pthread_join(venc_cam_cxt->thread_enc_id,NULL);
    }

	/* stop camera */
	venc_cam_cxt->CameraDevice->stopCamera(venc_cam_cxt->CameraDevice);

	DestroyCamera(venc_cam_cxt->CameraDevice);
	venc_cam_cxt->CameraDevice = NULL;

	waterMarkExit(venc_cam_cxt->waterMark);
	free(venc_cam_cxt->waterMark);
	venc_cam_cxt->waterMark = NULL;

	venc_cam_cxt->venc_device->ioctrl(venc_cam_cxt->venc_device, VENC_CMD_CLOSE, 0);
	cedarvEncExit(venc_cam_cxt->venc_device);
	venc_cam_cxt->venc_device = NULL;
	
	cedarx_hardware_exit(0);

	if(venc_cam_cxt->out_file) {
		fclose(venc_cam_cxt->out_file);
		venc_cam_cxt->out_file = NULL;
	}

	free(venc_cam_cxt);

	venc_cam_cxt = NULL;

	return 0;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

