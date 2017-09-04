#include "packetHandler.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "libcedarv.h"
extern "C"
{
#include "cedarv_osal_linux.h"
}
#include "render.h"
#include <sys/ioctl.h>
#include <linux/soundcard.h>
#include <pthread.h>
#include "mediastreamer2/mediastream.h"
#include <QDateTime>

static cedarv_decoder_t* decoder = NULL;
//FILE *fp_out = NULL;
static int audio_fd = -1;

extern VIDEO_RENDER_CONTEXT_TYPE *hnd_video_render;

int g_rtsp_player_vrect[4] = {0, 0, 0, 0};
int g_vrect_changed = 0;

#define AUDIO_BUF_SIZE      350 * 8000 / 1000

static short* audio_buffer = NULL;
static int audio_write_indx = 0;
static int audio_read_indx = 0;
static int audio_frequency = 8000;
static int audio_thread_run = 0;
static pthread_t audio_thread;
static pthread_mutex_t audio_mutex = PTHREAD_MUTEX_INITIALIZER;
QDateTime g_last_ipcdate_arrive_time;
int initHandler()
{
    int ret;

    render_init();

    decoder = cedarvDecInit(&ret);
    if (ret < 0)
    {
        printf("can not initialize the decoder library.\n");
        decoder = NULL;
        return -1;
    }

    cedarv_stream_info_t stream_info;
    stream_info.format 			 = CEDARV_STREAM_FORMAT_H264;
    stream_info.sub_format 		 = CEDARV_SUB_FORMAT_UNKNOW;
    stream_info.container_format = CEDARV_CONTAINER_FORMAT_UNKNOW;
    stream_info.video_width 	 = 1280;
    stream_info.video_height	 = 720;
    stream_info.frame_rate		 = 30*1000;
    stream_info.frame_duration	 = 0;
    stream_info.aspect_ratio	 = 1000;
    stream_info.init_data_len	 = 0;
    stream_info.init_data		 = 0;
    decoder->set_vstream_info(decoder, &stream_info);

    ret = decoder->open(decoder);
    if(ret < 0)
    {
        printf("can not open decoder.\n");
        cedarvDecExit(decoder);

        decoder = NULL;

        return -1;
    }

    decoder->ioctrl(decoder, CEDARV_COMMAND_PLAY, 0);

    //fp_out = fopen("/tmp/foreman_rec.yuv", "wb");

	return 0;
}

int unInitHandler()
{
    if (!decoder) return -1;
    decoder->ioctrl(decoder, CEDARV_COMMAND_STOP, 0);
    decoder->close(decoder);
    cedarvDecExit(decoder);

    //fclose(fp_out);

    decoder = NULL;

    render_exit();
    if (audio_thread_run)
    {
        audio_thread_run = 0;

        void *ret;
        pthread_join(audio_thread, &ret);
    }

    if (audio_buffer)
	{
		delete audio_buffer;
		audio_buffer = NULL;
	}
    if (audio_fd)
	{
		close(audio_fd);
		audio_fd = -1;
	}
	
    //fclose(fp);
	return 0;
}

//#define MIN(x, y) ((x) <= (y) ? (x) : (y))
extern char eventLoopWatchVariable;

char header[4] = {0x00, 0x00, 0x00, 0x01};
int myHandleVideo(char *pCodecName, unsigned char *pBuf, int iLen, struct timeval tvTimeStamp)
{
    g_last_ipcdate_arrive_time = QDateTime::currentDateTime();

    if (!decoder) return -1;

    if (strcmp(pCodecName, "H264") != 0)
	{
		return -1;
	}

    /*static int cnt = 0;
    int ten_sec = cnt % 10;
    int val = cnt / 10;
    int sec = val % 60;
    val /= 60;
    int minute = val % 60;
    val /= 60;
    int hour = val;
    printf("myHandleVideo: %4d:%02d:%02d.%d\n", hour, minute, sec, ten_sec);
    cnt++;
    */
    /*int nal_type = pBuf[0] & 0x1f;
    if (!((nal_type == 7) || (nal_type == 8) || (nal_type == 5)
            || (nal_type == 1)))
        return -1;
    */

	//int time = tvTimeStamp.tv_sec * 1000 + tvTimeStamp.tv_usec / 1000;
	//printf("video tvTimeStamp:%d\n", time);

    cedarv_stream_data_info_t data_info;
    cedarv_picture_t picture;
    unsigned char* buf0;
    unsigned int   bufsize0;
    unsigned char* buf1;
    unsigned int   bufsize1;
    int ret;

    iLen += 4;
    ret = decoder->request_write(decoder, iLen, &buf0, &bufsize0, &buf1, &bufsize1);
    if (ret < 0)
    {
        printf("request bit stream buffer fail.\n");
        return -1;
    }

    memset(&data_info, 0, sizeof(data_info));
    if (iLen <= bufsize0)
    {
        memcpy(buf0, header, 4);
        memcpy(&buf0[4], pBuf, iLen - 4);
    }
    else
    {
        if (bufsize0 <= 4)
        {
            memcpy(buf0, header, bufsize0);
            memcpy(buf1, header + bufsize0, 4 - bufsize0);
            memcpy(buf1 + 4 - bufsize0, pBuf, iLen - 4);
        }
        else
        {
            memcpy(buf0, header, 4);
            memcpy(&buf0[4], pBuf, bufsize0 - 4);
            memcpy(buf1, pBuf + bufsize0 - 4, iLen - bufsize0);
        }
    }
    data_info.length = iLen;

    data_info.flags |= CEDARV_FLAG_FIRST_PART;
    data_info.flags |= CEDARV_FLAG_LAST_PART;

    decoder->update_data(decoder, &data_info);

    ret = decoder->decode(decoder);

    if (ret == CEDARV_RESULT_ERR_NO_MEMORY || ret == CEDARV_RESULT_ERR_UNSUPPORTED)
    {
        printf("bit stream is unsupported.\n");
        return -1;
    }

    ret = decoder->display_request(decoder, &picture);

    if (ret == 0)
    {
        //printf("decoded ok:%d %d\n", picture.width, picture.height);
        /*char tmp[1280 * 720 * 3/2];
        char *ptr = (char *)cedarv_address_phy2vir((void *)picture.y);
        map32x32_to_yuv_Y((unsigned char *)ptr, (unsigned char *)tmp, 1280, 720);
        ptr = (char *)cedarv_address_phy2vir((void *)picture.u);
        map32x32_to_yuv_C(0, (unsigned char *)ptr, (unsigned char *)(tmp + 1280 * 720),
            (unsigned char *)(tmp + 1280 * 720 * 5/4), 640, 360);
        fwrite(tmp, 1, 1280 * 720 * 3/2, fp_out);
        fflush(fp_out);
        */

        if (g_vrect_changed)
        {
            hnd_video_render->first_frame_flag = 1;
            g_vrect_changed = 0;
        }
        render_render(&picture, picture.id, g_rtsp_player_vrect[0], g_rtsp_player_vrect[1],
                      g_rtsp_player_vrect[2], g_rtsp_player_vrect[3], 0); //960, 540);

        decoder->display_release(decoder, picture.id);
    }

	return 0;
}
static void renderSetScale()
{
    hnd_video_render->first_frame_flag = 1;
}

void closeVidevShow()
{
    cedarv_picture_t picture;

    memset(&picture,0,sizeof(picture));
    picture.width = 0;
    picture.height = 0;
    picture.display_width = 0;
    picture.display_height = 0;
    picture.frame_rate = 25;
    picture.aspect_ratio = 1331;
    picture.is_progressive = 1;
    picture.top_field_first = 1;
    picture.repeat_bottom_field = 1;
    picture.pixel_format == CEDARV_PIXEL_FORMAT_YUV420;
    picture.anaglath_transform_mode = CEDARV_ANAGLAGH_NONE;
    picture.size_y = 0;
    picture.size_u = 0;
    picture.size_v = 0;
    picture.id = 0;

    renderSetScale();

    render_render(&picture,picture.id,0,0,0,0,0);
}
static int ulaw_to_s16(unsigned char u_val)
{
	int t;

	// Complement to obtain normal u-law value.
	u_val = ~u_val;

	//
	// Extract and bias the quantization bits. Then
	// shift up by the segment number and subtract out the bias.
	//
	t = ((u_val & 0x0f) << 3) + 0x84;
	t <<= (u_val & 0x70) >> 4;

	return ((u_val & 0x80) ? (0x84 - t) : (t - 0x84));
}

static int alaw_to_s16(unsigned char a_val)
{
	int		t;
	int		seg;

	a_val ^= 0x55;
	t = a_val & 0x7f;
	if (t < 16)
		t = (t << 4) + 8;
	else {
		seg = (t >> 4) & 0x07;
		t = ((t & 0x0f) << 4) + 0x108;
		t <<= seg -1;
	}
	return ((a_val & 0x80) ? t : -t);
}

/*static double audio_time_ms = 0.0;
static struct timeval start_time_ms;
static int audio_len = 0;
*/

static void *audioPlayThread(void *lpVoid)
{
    long long elapsed_ms = 0;
    int delta_time_ms = 20;
    short buf[1024];
    memset(buf, 0, sizeof(buf));
    write(audio_fd, buf, 50 * 2 * audio_frequency / 1000);

    struct timeval start_tv;
    gettimeofday(&start_tv, NULL);

    audio_thread_run = 1;

    while (audio_thread_run)
    {
        struct timeval tv;

        gettimeofday(&tv, NULL);

        int time_ms = (tv.tv_sec - start_tv.tv_sec) * 1000
                + (tv.tv_usec - start_tv.tv_usec) / 1000;

        time_ms = elapsed_ms - time_ms;

        if (time_ms > 0)
        {
            usleep(time_ms * 1000);
        }

        pthread_mutex_lock(&audio_mutex);

//        int len = (audio_write_indx + AUDIO_BUF_SIZE - audio_read_indx) % AUDIO_BUF_SIZE;
//        int len2;
//
//        int data_len = delta_time_ms * audio_frequency / 1000;
//        if (elapsed_ms % 700 == 0)
//        {
//            data_len--;
//        }
//
//        if (data_len <= len)
//        {
//            len2 = data_len;
//        }
//        else
//        {
//            len2 = len;
//        }
//
//        if (audio_read_indx + len2 < AUDIO_BUF_SIZE)
//        {
//            write(audio_fd, audio_buffer + audio_read_indx, len2 * 2);
//            audio_read_indx += len2;
//        }
//        else
//        {
//            write(audio_fd, audio_buffer + audio_read_indx, (AUDIO_BUF_SIZE - audio_read_indx) * 2);
//            write(audio_fd, audio_buffer, (len2 - (AUDIO_BUF_SIZE - audio_read_indx)) * 2);
//            audio_read_indx = len2 - (AUDIO_BUF_SIZE - audio_read_indx);
//        }
//
//        if (len2 < data_len)
//        {
//            printf("underflow: iLen:%d data_len:%d\n", data_len - len2, data_len);
//            write(audio_fd, buf, (data_len - len2) * 2);
//        }

        pthread_mutex_unlock(&audio_mutex);

        elapsed_ms += delta_time_ms;
    }

    return NULL;
}

int myHandleAudio(char *pCodecName, unsigned int uSampleFrequency, 
	unsigned int uNumChannels, unsigned char *pBuf, int iLen, struct timeval tvTimeStamp)
{
	//int time = tvTimeStamp.tv_sec * 1000 + tvTimeStamp.tv_usec / 1000;
	//printf("audio tvTimeStamp:%d\n", time);

	if (audio_fd == -1)
	{
//        stopWaveFile();			// 会导致出现爆音现象，暂时屏蔽

        audio_fd = open("/dev/dsp", O_WRONLY);
		
	    int p;
		int err;
		int frag;

		ioctl(audio_fd, SNDCTL_DSP_RESET, 0);

		frag = ((180 << 16) | 6);//((32767 << 16 ) | 6);
		if (ioctl(audio_fd, SNDCTL_DSP_SETFRAGMENT, &frag))
		{
		    printf("oss_open-: can't set fragment size:%s.\n",strerror(errno));
		}

		p = AFMT_S16_NE;
		err = ioctl(audio_fd, SNDCTL_DSP_SETFMT, &p);
		if (err < 0){
		    printf("oss_open-: can't set sample format:%s.\n", strerror(errno));
		}

		p = 16;
		err = ioctl(audio_fd, SNDCTL_DSP_SAMPLESIZE, &p);
		if (err < 0){
		    printf("oss_open-: can't set sample size to %i:%s.\n", 16, strerror(errno));
		}

        audio_frequency = p = uSampleFrequency;
		err = ioctl(audio_fd, SNDCTL_DSP_SPEED, &p);
		if (err < 0){
		    printf("oss_open-: can't set sample rate to %i:%s.\n", 8000, strerror(errno));
		}

        p = 0;
		err = ioctl(audio_fd, SNDCTL_DSP_STEREO, &p);
		if (err < 0){
		    printf("oss_open-: can't set mono/stereo mode:%s.\n", strerror(errno));
		}
		
		if (!audio_buffer)
		{
            audio_buffer = new short [AUDIO_BUF_SIZE];
		}

        audio_write_indx = 0;
        audio_read_indx = 0;

        //audio_time_ms = 50.0;
        //audio_len = 0; //50 * 4 * uSampleFrequency / 1000;
        //memset(audio_buffer, 0, 50 * 2 * uSampleFrequency / 1000);
        //write(audio_fd, audio_buffer, 50 * 2 * uSampleFrequency / 1000);

        pthread_create(&audio_thread, NULL, audioPlayThread, NULL);

        //gettimeofday(&start_time_ms, NULL);
	}

	int i;
    /*if (strcmp(pCodecName, "PCMU") == 0)
	{
		for (i = 0; i < iLen; i++)
		{
            audio_buffer[i * 2] = audio_buffer[i * 2 + 1] = (short)ulaw_to_s16(pBuf[i]);
		}
	}
    else*/
    if (strcmp(pCodecName, "PCMU") == 0 || strcmp(pCodecName, "PCMA") == 0)
	{
        pthread_mutex_lock(&audio_mutex);

        int max_len = (audio_read_indx + AUDIO_BUF_SIZE - 1 - audio_write_indx) % AUDIO_BUF_SIZE;
        if (iLen > max_len)
        {
            printf("overflow: iLen:%d max_len:%d\n", iLen, max_len);
            iLen = max_len;
        }

        if (audio_write_indx + iLen < AUDIO_BUF_SIZE)
        {
            if (strcmp(pCodecName, "PCMU") == 0)
            {
                for (i = 0; i < iLen; i++)
                {
                    audio_buffer[audio_write_indx + i] = 0.05 * (short)ulaw_to_s16(pBuf[i]);
                }
            }
            else
            {
                for (i = 0; i < iLen; i++)
                {
                    audio_buffer[audio_write_indx + i] = 0.05 * (short)alaw_to_s16(pBuf[i]);
                }
            }

            audio_write_indx += iLen;
        }
        else
        {
            if (strcmp(pCodecName, "PCMU") == 0)
            {
                for (i = 0; i < AUDIO_BUF_SIZE - audio_write_indx; i++)
                {
                    audio_buffer[audio_write_indx + i] = 0.05 * (short)ulaw_to_s16(pBuf[i]);
                }

                for (i = 0; i < iLen - (AUDIO_BUF_SIZE - audio_write_indx); i++)
                {
                    audio_buffer[i] = 0.05 * (short)ulaw_to_s16(pBuf[i + (AUDIO_BUF_SIZE - audio_write_indx)]);
                }
            }
            else
            {
                for (i = 0; i < AUDIO_BUF_SIZE - audio_write_indx; i++)
                {
                    audio_buffer[audio_write_indx + i] = 0.05 * (short)alaw_to_s16(pBuf[i]);
                }

                for (i = 0; i < iLen - (AUDIO_BUF_SIZE - audio_write_indx); i++)
                {
                    audio_buffer[i] = 0.05 * (short)alaw_to_s16(pBuf[i + (AUDIO_BUF_SIZE - audio_write_indx)]);
                }
            }

            audio_write_indx = iLen - (AUDIO_BUF_SIZE - audio_write_indx);
        }

        pthread_mutex_unlock(&audio_mutex);

        /*for (i = 0; i < iLen; i++)
		{
            audio_buffer[i] /* * 2] = audio_buffer[i * 2 + 1]*/ /*= 0.05 * (short)alaw_to_s16(pBuf[i]);
        }*/
	}
	else
	{
		return -1;
	}

    /*audio_time_ms += iLen * 1000.0 / uSampleFrequency;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    double delta_time_ms = (tv.tv_sec - start_time_ms.tv_sec) * 1000.0
            + (tv.tv_usec - start_time_ms.tv_usec) / 1000.0;

    printf("audio:%10d %6.2f\n", audio_len - (int)(delta_time_ms * 4 * uSampleFrequency / 1000), delta_time_ms);

    audio_len += iLen * 4;

    if (audio_time_ms > 750.0)
    {
        iLen--;
        audio_len -= 4;
        audio_time_ms = 0.0;
    }*/
    //write(audio_fd, audio_buffer, iLen * 2);

	return 0;
}
