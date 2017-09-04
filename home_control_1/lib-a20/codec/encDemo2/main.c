#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define __OS_ANDROID

#define LOG_NDEBUG 0
#define LOG_TAG "main"
#include "CDX_Debug.h"

#include<sys/time.h>
#include<unistd.h>
#include "render.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "venc.h"
#include "libcedarv.h"	//* for decoding video
#include "CameraSource.h"
#include "transform_color_format.h"

unsigned int mwidth = 640;
unsigned int mheight = 480;

typedef struct Venc_context
{
	cedarv_encoder_t *venc_device;
	cedarv_decoder_t *vdec_device;

	VencBaseConfig base_cfg;
	VencInputBuffer input_buffer;
	VencOutputBuffer output_buffer;
	VencAllocateBufferParam alloc_parm;
	VencSeqHeader header_data;
	AWCameraDevice *CameraDevice;	

	pthread_t thread_enc_id;
	pthread_t thread_dec_id;
	pthread_t thread_dec_id1;
	pthread_mutex_t mutex_ve;

	int mstart;
	FILE *out_file;
}Venc_context;


int CameraSourceCallback(void *cookie,  void *data)
{
	Venc_context * venc_cam_cxt = (Venc_context *)cookie;
	cedarv_encoder_t *venc_device = venc_cam_cxt->venc_device;
	AWCameraDevice *CameraDevice = venc_cam_cxt->CameraDevice;
	
	VencInputBuffer input_buffer;
	int result = 0;
	struct v4l2_buffer *p_buf = (struct v4l2_buffer *)data;

	memset(&input_buffer, 0, sizeof(VencInputBuffer));

	input_buffer.id = p_buf->index;
	input_buffer.addrphyY = p_buf->m.offset;
	input_buffer.addrphyC = p_buf->m.offset + venc_cam_cxt->base_cfg.input_width*venc_cam_cxt->base_cfg.input_height;

#if 1
	if(input_buffer.addrphyY >=  0x40000000)
		input_buffer.addrphyY -=0x40000000;
#endif

	if(!venc_cam_cxt->mstart) {
		LOGD("p_buf->index = %d\n", p_buf->index);
		CameraDevice->returnFrame(CameraDevice, p_buf->index);
	}

    // enquene buffer to input buffer quene
	result = venc_device->ioctrl(venc_device, VENC_CMD_ENQUENE_INPUT_BUFFER, &input_buffer);

	if(result < 0) {
		CameraDevice->returnFrame(CameraDevice, p_buf->index);
		LOGW("video input buffer is full , skip this frame");
	}

	return 0;
}

void* DecoderThread(void* pThreadData) {
	Venc_context *venc_cam_cxt = (Venc_context *)pThreadData;
	cedarv_decoder_t *vdec_device = venc_cam_cxt->vdec_device;
	cedarv_picture_t picture;
	char* addr_out = (char*)malloc(800*480*3/2);
	struct timeval tv;
	struct timezone tz;
	long time0, time1;
	int ret;
		
	gettimeofday (&tv , &tz);
	time0 = tv.tv_usec;
	while(venc_cam_cxt->mstart) {
		
		if (vdec_device->display_request(vdec_device, &picture) == 0) {
			render_render(&picture, picture.id);
			
			gettimeofday (&tv , &tz);
			time1 = tv.tv_usec;
			LOGD("time = %d", time1 - time0);

			time0 = time1;			
	//		TransformToYUVPlaner(&picture, addr_out, 0);
			vdec_device->display_release(vdec_device, picture.id);
			continue;
		}


		pthread_mutex_lock(&venc_cam_cxt->mutex_ve);
		ret = vdec_device->decode(vdec_device);
		pthread_mutex_unlock(&venc_cam_cxt->mutex_ve);

		if (ret == CEDARV_RESULT_NO_FRAME_BUFFER) {
			// no buffer to use
			usleep(10*1000);
			continue;
		} else if (ret == CEDARV_RESULT_NO_BITSTREAM) {
			// no data to decode
			usleep(5*1000);
			continue;
		} else if (ret < CEDARV_RESULT_OK) {
			// decode error
			continue;
		}
	}
	free(addr_out);

	return (void*)0;
}



static void* EncoderThread(void* pThreadData) {

	Venc_context *venc_cam_cxt = (Venc_context *)pThreadData;
	cedarv_encoder_t *venc_device = venc_cam_cxt->venc_device;
	cedarv_decoder_t *vdec_device = venc_cam_cxt->vdec_device;
	AWCameraDevice *CameraDevice = venc_cam_cxt->CameraDevice;
	cedarv_stream_data_info_t stream_data_info;
	
	VencInputBuffer input_buffer;
	VencOutputBuffer output_buffer;
	int result = 0;
	int isFirst = 1;

	u8* buf0;
	u8* buf1;
	u32 buf0_size;
	u32 buf1_size;
	
	while(venc_cam_cxt->mstart) {

		memset(&input_buffer, 0, sizeof(VencInputBuffer));

		// dequene buffer from input buffer quene;
	    result = venc_device->ioctrl(venc_device, VENC_CMD_DEQUENE_INPUT_BUFFER, &input_buffer);
		if(result<0) {
			//LOGV("enquene input buffer is empty");
			usleep(10*1000);
			continue;
		}

		pthread_mutex_lock(&venc_cam_cxt->mutex_ve);
		result = venc_device->ioctrl(venc_device, VENC_CMD_ENCODE, &input_buffer);
		pthread_mutex_unlock(&venc_cam_cxt->mutex_ve);

		//LOGD("test, result,encoder result: %d, input_buffer.id: %d", result, input_buffer.id);
		
		CameraDevice->returnFrame(CameraDevice, input_buffer.id);
		
		if(result == 0) {

			memset(&output_buffer, 0, sizeof(VencOutputBuffer));
			result = venc_device->ioctrl(venc_device, VENC_CMD_GET_BITSTREAM, &output_buffer);
			
			//set to decode
			if (isFirst) {
				isFirst = 0;

				memset(&stream_data_info, 0x0, sizeof(cedarv_stream_data_info_t));
				result = vdec_device->request_write(vdec_device, 
						venc_cam_cxt->header_data.length, &buf0, &buf0_size, &buf1, &buf1_size);
				if (result == 0) {
					if (buf0_size >= venc_cam_cxt->header_data.length) {
						memcpy(buf0, venc_cam_cxt->header_data.bufptr, venc_cam_cxt->header_data.length);
					}
				}
				stream_data_info.pts = -1;
				stream_data_info.length = venc_cam_cxt->header_data.length;
				stream_data_info.flags = CEDARV_FLAG_FIRST_PART | CEDARV_FLAG_LAST_PART;
				vdec_device->update_data(vdec_device, &stream_data_info);
			} 
			

			if (output_buffer.size0 != 0) {
				result = vdec_device->request_write(vdec_device, 
						output_buffer.size0, &buf0, &buf0_size, &buf1, &buf1_size);
				if (result == 0) {

					memset(&stream_data_info, 0x0, sizeof(cedarv_stream_data_info_t));
					if (buf0_size >= output_buffer.size0) {
						memcpy(buf0, output_buffer.ptr0, output_buffer.size0);
						cedarx_cache_op(buf0, buf0 + buf0_size, 1);
					} else {
						memcpy(buf0, output_buffer.ptr0, buf0_size);
						cedarx_cache_op(buf0, buf0 + buf0_size, 1);
						memcpy(buf1, output_buffer.ptr0 + buf0_size, output_buffer.size0 - buf0_size);
						cedarx_cache_op(buf1, buf1 + buf1_size, 1);
					}

					stream_data_info.pts = -1;
					stream_data_info.length = output_buffer.size0;
					if (output_buffer.size1 != 0)
						stream_data_info.flags = CEDARV_FLAG_FIRST_PART;
					else 
						stream_data_info.flags = CEDARV_FLAG_FIRST_PART | CEDARV_FLAG_LAST_PART;

					vdec_device->update_data(vdec_device, &stream_data_info);



				}
			}

			if (output_buffer.size1 != 0) {
				result = vdec_device->request_write(vdec_device, 
						output_buffer.size1, &buf0, &buf0_size, &buf1, &buf1_size);
				if (result == 0) {

					memset(&stream_data_info, 0x0, sizeof(cedarv_stream_data_info_t));
					if (buf0_size >= output_buffer.size1) {
						memcpy(buf0, output_buffer.ptr1, output_buffer.size1);
					} else {
						memcpy(buf0, output_buffer.ptr1, buf0_size);
						memcpy(buf1, output_buffer.ptr1 + buf0_size, output_buffer.size1 - buf0_size);
					}

					stream_data_info.pts = -1;
					stream_data_info.length = output_buffer.size1;
					stream_data_info.flags = CEDARV_FLAG_LAST_PART;

					vdec_device->update_data(vdec_device, &stream_data_info);
				}
			}
			 
			//write to file
			fwrite(output_buffer.ptr0, 1, output_buffer.size0, venc_cam_cxt->out_file);

			if(output_buffer.size1) {
				fwrite(output_buffer.ptr1, 1, output_buffer.size1, venc_cam_cxt->out_file);
			}
			
			if(result == 0) {
				venc_device->ioctrl(venc_device, VENC_CMD_RETURN_BITSTREAM, &output_buffer);
			}
			
		}else {
			LOGD("encoder fatal error");
		}
	}
	
	return (void*)0;
}

int main()
{
	int err = 0;
	cedarv_stream_info_t stream_info;
	Venc_context *venc_cam_cxt = (Venc_context *)malloc(sizeof(Venc_context));

	//init hardware
	cedarx_hardware_init(0);
	LOGD("cedarx_hardware_init success");

	LOGD("Codec version = %s", getCodecVision());
	//init and open encode
	memset(venc_cam_cxt, 0, sizeof(Venc_context));
	venc_cam_cxt->venc_device = cedarvEncInit();
	venc_cam_cxt->base_cfg.codectype = VENC_CODEC_H264;
	venc_cam_cxt->base_cfg.framerate = 30;
	venc_cam_cxt->base_cfg.input_width = mwidth;
	venc_cam_cxt->base_cfg.input_height= mheight;
	venc_cam_cxt->base_cfg.dst_width = mwidth;
	venc_cam_cxt->base_cfg.dst_height = mheight;
	venc_cam_cxt->base_cfg.maxKeyInterval = 25;
	venc_cam_cxt->base_cfg.inputformat = VENC_PIXEL_YUV420; //uv combined
	venc_cam_cxt->base_cfg.targetbitrate = 3*1024*1024;
	venc_cam_cxt->venc_device->ioctrl(venc_cam_cxt->venc_device, VENC_CMD_BASE_CONFIG, &venc_cam_cxt->base_cfg);
	venc_cam_cxt->venc_device->ioctrl(venc_cam_cxt->venc_device, VENC_CMD_OPEN, 0);
	venc_cam_cxt->venc_device->ioctrl(venc_cam_cxt->venc_device, VENC_CMD_HEADER_DATA, &venc_cam_cxt->header_data);

	LOGD("open encode success");

	//init and open decode
	venc_cam_cxt->vdec_device = cedarvDecInit(NULL);
	memset(&stream_info, 0x0, sizeof(cedarv_stream_info_t));
	stream_info.video_width = mwidth;
	stream_info.video_height = mheight;
	stream_info.format = CEDARV_STREAM_FORMAT_H264;
	stream_info.container_format = CEDARV_CONTAINER_FORMAT_UNKNOW;
	stream_info.sub_format = CEDARV_SUB_FORMAT_UNKNOW;
	stream_info.frame_rate = 30000; 
	stream_info.frame_duration = 0;
	stream_info.init_data = NULL;
	stream_info.init_data_len = 0;
	venc_cam_cxt->vdec_device->set_vstream_info(venc_cam_cxt->vdec_device, &stream_info);
	venc_cam_cxt->vdec_device->open(venc_cam_cxt->vdec_device);
	venc_cam_cxt->vdec_device->ioctrl(venc_cam_cxt->vdec_device, CEDARV_COMMAND_PLAY, 0);

	LOGD("open decode success");

	pthread_mutex_init(&venc_cam_cxt->mutex_ve, NULL);

	// open file for saving data
	venc_cam_cxt->out_file = fopen("/mnt/out2.h264", "wb");
	if(venc_cam_cxt->out_file == NULL) {
		LOGE("open file error");
		return 0;
	}
	fwrite(venc_cam_cxt->header_data.bufptr, 1, venc_cam_cxt->header_data.length, venc_cam_cxt->out_file);

	LOGD("render_init");

	//int display
	render_init();

	// create and open camera 
	venc_cam_cxt->CameraDevice = CreateCamera(mwidth, mheight);
	venc_cam_cxt->CameraDevice->setCameraDatacallback(venc_cam_cxt->CameraDevice, 
		(void *)venc_cam_cxt, (void *)&CameraSourceCallback);
	venc_cam_cxt->CameraDevice->startCamera(venc_cam_cxt->CameraDevice);

	// start encoder 
	venc_cam_cxt->mstart = 1;

	// create encode thread
	err = pthread_create(&venc_cam_cxt->thread_enc_id, NULL, EncoderThread, venc_cam_cxt);
	if (err || !venc_cam_cxt->thread_enc_id) {
		LOGE("Create thread_enc_id fail !\n");
	}

	// create decode thread
	err = pthread_create(&venc_cam_cxt->thread_dec_id, NULL, DecoderThread, venc_cam_cxt);
	if (err || !venc_cam_cxt->thread_dec_id) {
		LOGE("Create thread_dec_id fail !\n");
	}

	// create decode thread1
//	err = pthread_create(&venc_cam_cxt->thread_dec_id1, NULL, DecoderThread, venc_cam_cxt);
//	if (err || !venc_cam_cxt->thread_dec_id1) {
//		LOGE("Create thread_dec_id1 fail !\n");
//	}

	sleep(600);

    // stop encoder 
	venc_cam_cxt->mstart = 0;

	if(venc_cam_cxt->thread_enc_id !=0) {                 
        pthread_join(venc_cam_cxt->thread_enc_id,NULL);
    }

	if(venc_cam_cxt->thread_dec_id !=0) {                 
        pthread_join(venc_cam_cxt->thread_dec_id,NULL);
    }

	if(venc_cam_cxt->thread_dec_id1 !=0) {                 
        pthread_join(venc_cam_cxt->thread_dec_id1,NULL);
    }

	// stop camera 
	venc_cam_cxt->CameraDevice->stopCamera(venc_cam_cxt->CameraDevice);
	DestroyCamera(venc_cam_cxt->CameraDevice);
	venc_cam_cxt->CameraDevice = NULL;

	// close display
	render_exit();

	// close encode 
	venc_cam_cxt->venc_device->ioctrl(venc_cam_cxt->venc_device, VENC_CMD_CLOSE, 0);
	cedarvEncExit(venc_cam_cxt->venc_device);
	venc_cam_cxt->venc_device = NULL;
	
	// close decode
	venc_cam_cxt->vdec_device->ioctrl(venc_cam_cxt->vdec_device, CEDARV_COMMAND_STOP, 0);
	venc_cam_cxt->vdec_device->close(venc_cam_cxt->vdec_device);
	cedarvDecExit(venc_cam_cxt->vdec_device);	
	venc_cam_cxt->vdec_device = NULL;
	


	pthread_mutex_destroy(&venc_cam_cxt->mutex_ve);
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

