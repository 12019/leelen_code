#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include "render.h"
extern "C"
{
#include "cedarv_osal_linux.h"
}
#include "libcedarv.h"

typedef enum
{
    TVD_UV_SWAP             = 0,
    TVD_COLOR_SET           = 1,
} tvd_param_t;

typedef enum
{
    TVD_CVBS                = 0,
    TVD_YPBPR_I             = 1,
    TVD_YPBPR_P             = 2,
} tvd_interface_t;

typedef enum
{
    TVD_NTSC                = 0,
    TVD_PAL                 = 1,
    TVD_SECAM               = 2,
} tvd_system_t;

typedef enum
{
    TVD_PL_YUV420           = 0,
    TVD_MB_YUV420           = 1,
    TVD_PL_YUV422           = 2,
} tvd_fmt_t;

typedef enum
{
    TVD_CHANNEL_ONLY_1      = 0,
    TVD_CHANNEL_ONLY_2      = 1,
    TVD_CHANNEL_ONLY_3      = 2,
    TVD_CHANNEL_ONLY_4      = 3,
    TVD_CHANNEL_ALL_2x2     = 4,
    TVD_CHANNEL_ALL_1x4     = 5,
    TVD_CHANNEL_ALL_4x1     = 6,
}tvd_channel_t;

static int v4l2Handle = -1;;

typedef struct v4l2_mem_map_t{
    void *mem[4];
    //int phyaddr[4];
    int  length;
} v4l2_mem_map_t;
static v4l2_mem_map_t mapMem;

static int luma = 50;
static int contrast = 50;
static int saturation = 50;
static int hue = 50;

static int interface = TVD_CVBS;
static int i_system = TVD_PAL;
static int format = TVD_PL_YUV420;
static int channel = TVD_CHANNEL_ONLY_2;

static int bufferCnt = 4;

static int videoWidth = 0;
static int videoHeight = 0;

static int openCameraDev()
{
    v4l2Handle = open("/dev/video1", O_RDWR | O_NONBLOCK, 0);
    if (v4l2Handle == -1)
    {
        printf("ERROR opening V4L interface: %s\n", strerror(errno));
        return -1;
    }
    int ret = -1;
    struct v4l2_capability cap;
    ret = ioctl (v4l2Handle, VIDIOC_QUERYCAP, &cap);
    if (ret < 0)
    {
        printf("Error opening device: unable to query device.\n");
        return -1;
    }
    if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0)
    {
        printf("Error opening device: video capture not supported.\n");
        return -1;
    }
    if ((cap.capabilities & V4L2_CAP_STREAMING) == 0)
    {
        printf("Capture device does not support streaming i/o\n");
        return -1;
    }

    return 0;
}

static void closeCameraDev()
{
    if (v4l2Handle >= 0)
    {
        close(v4l2Handle);
        v4l2Handle = 0;
    }
}

static int v4l2SetColor()
{
    struct v4l2_streamparm parms;
    int ret = -1;
    int tmp_hue = 0;
    if (hue >= 50 && hue <= 100)
        tmp_hue = hue - 50;
    else if (hue >= 0 && hue < 50)
        tmp_hue = 50 - hue;
    parms.type = V4L2_BUF_TYPE_PRIVATE;
    parms.parm.raw_data[0] = TVD_COLOR_SET;
    parms.parm.raw_data[1] = luma/1.2;			        //luma(0~255)
    parms.parm.raw_data[2] = contrast * 255/100;			//contrast(0~255)
    parms.parm.raw_data[3] = saturation * 255/100;			//saturation(0~255)
    parms.parm.raw_data[4] = tmp_hue * 256/100;		        //hue(0~255)
    if (ioctl(v4l2Handle, VIDIOC_S_PARM, &parms) == -1)
    {
        printf("VIDIOC_S_PARM error! line %d\n",__LINE__);
        ret = -1;
        return ret;
    }
    return 0;
}

static int v4l2SetVideoParams()
{
    int ret = -1;
    struct v4l2_format v_format;
    memset(&v_format, 0, sizeof(v_format));

    v_format.type = V4L2_BUF_TYPE_PRIVATE;
    v_format.fmt.raw_data[0] = interface; //mInterface;
    v_format.fmt.raw_data[1] = i_system; //mSystem;
    v_format.fmt.raw_data[2] = format; //mFormat;

    switch(channel)
    {
        case TVD_CHANNEL_ONLY_1:
            v_format.fmt.raw_data[8]  = 1;	        //row
            v_format.fmt.raw_data[9]  = 1;	        //column
            v_format.fmt.raw_data[10] = 1;		//channel_index0
            v_format.fmt.raw_data[11] = 0;		//channel_index1
            v_format.fmt.raw_data[12] = 0;		//channel_index2
            v_format.fmt.raw_data[13] = 0;		//channel_index3
            break;
        case TVD_CHANNEL_ONLY_2:
            v_format.fmt.raw_data[8]  = 1;	        //row
            v_format.fmt.raw_data[9]  = 1;	        //column
            v_format.fmt.raw_data[10] = 0;		//channel_index0
            v_format.fmt.raw_data[11] = 1;		//channel_index1
            v_format.fmt.raw_data[12] = 0;		//channel_index2
            v_format.fmt.raw_data[13] = 0;		//channel_index3
            break;
        case TVD_CHANNEL_ONLY_3:
            v_format.fmt.raw_data[8]  = 1;	        //row
            v_format.fmt.raw_data[9]  = 1;	        //column
            v_format.fmt.raw_data[10] = 0;          //channel_index0
            v_format.fmt.raw_data[11] = 0;          //channel_index1
            v_format.fmt.raw_data[12] = 1;          //channel_index2
            v_format.fmt.raw_data[13] = 0;          //channel_index3
            break;
        case TVD_CHANNEL_ONLY_4:
            v_format.fmt.raw_data[8]  = 1;	        //row
            v_format.fmt.raw_data[9]  = 1;	        //column
            v_format.fmt.raw_data[10] = 0;          //channel_index0
            v_format.fmt.raw_data[11] = 0;          //channel_index1
            v_format.fmt.raw_data[12] = 0;          //channel_index2
            v_format.fmt.raw_data[13] = 1;          //channel_index3
            break;
        case TVD_CHANNEL_ALL_2x2:
            v_format.fmt.raw_data[8]  = 2;	        //row
            v_format.fmt.raw_data[9]  = 2;	        //column
            v_format.fmt.raw_data[10] = 1;          //channel_index0
            v_format.fmt.raw_data[11] = 2;          //channel_index1
            v_format.fmt.raw_data[12] = 3;          //channel_index2
            v_format.fmt.raw_data[13] = 4;          //channel_index3
            break;
        case TVD_CHANNEL_ALL_1x4:
            v_format.fmt.raw_data[8]  = 1;	        //row
            v_format.fmt.raw_data[9]  = 4;	        //column
            v_format.fmt.raw_data[10] = 1;          //channel_index0
            v_format.fmt.raw_data[11] = 2;          //channel_index1
            v_format.fmt.raw_data[12] = 3;          //channel_index2
            v_format.fmt.raw_data[13] = 4;          //channel_index3
            break;
        case TVD_CHANNEL_ALL_4x1:
            v_format.fmt.raw_data[8]  = 4;	        //row
            v_format.fmt.raw_data[9]  = 1;	        //column
            v_format.fmt.raw_data[10] = 1;          //channel_index0
            v_format.fmt.raw_data[11] = 2;          //channel_index1
            v_format.fmt.raw_data[12] = 3;          //channel_index2
            v_format.fmt.raw_data[13] = 4;          //channel_index 3
            break;
        default:
            break;
    }
    if (-1 == ioctl(v4l2Handle, VIDIOC_S_FMT, &v_format))
    {
        printf("VIDIOC_S_FMT error!\n");
        ret = -1;
        return ret;
    }

    printf("#########################interface %d  system %d format %d\n", interface, i_system, format);
    struct v4l2_streamparm parms;
    //v4l2SetColor();
    if (1) //format == TVD_MB_YUV420)
    {
        parms.type = V4L2_BUF_TYPE_PRIVATE;
        parms.parm.raw_data[0] = TVD_UV_SWAP;
        parms.parm.raw_data[1] = 0;
        if (ioctl(v4l2Handle, VIDIOC_S_PARM, &parms) == -1)
        {
            printf("VIDIOC_S_PARM error!\n");
            ret = -1;
            return ret;
        }
    }
    else
    {
        parms.type = V4L2_BUF_TYPE_PRIVATE;
        parms.parm.raw_data[0] = TVD_UV_SWAP;
        parms.parm.raw_data[1] = 1;
        if (ioctl(v4l2Handle, VIDIOC_S_PARM, &parms) == -1)
        {
            printf("VIDIOC_S_PARM error!\n");
            ret = -1;
            return ret;
        }
    }
    return 0;
}

static int v4l2ReqBufs()
{
    int ret = -1;
    struct v4l2_requestbuffers rb;
    printf("TO VIDIOC_REQBUFS count: %d\n", bufferCnt);
    memset(&rb, 0, sizeof(rb));
    rb.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    rb.memory = V4L2_MEMORY_MMAP;
    rb.count  = bufferCnt;

    ret = ioctl(v4l2Handle, VIDIOC_REQBUFS, &rb);
    if (ret < 0)
    {
        printf("Init: VIDIOC_REQBUFS failed: %s\n", strerror(errno));
        return ret;
    }
    if (bufferCnt != rb.count)
    {
        bufferCnt = rb.count;
        printf("VIDIOC_REQBUFS count: %d\n", bufferCnt);
    }
    return 0;
}

int getGeometry(void)
{
    struct v4l2_format format;
    int ret = -1;

    memset(&format, 0, sizeof(format));
    format.type     = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(v4l2Handle, VIDIOC_G_FMT, &format);
    if (ret < 0)
    {
        printf("VIDIOC_G_FMT Failed: %s\n", strerror(errno));
        return ret;
    }

    videoWidth     = format.fmt.pix.width;
    videoHeight    = format.fmt.pix.height;
    printf("Set buffer geometry # mVideoWidth: %d, mVideoHeight: %d\n", videoWidth, videoHeight);
    return 0;
}

static int v4l2QueryBuf()
{
    int ret = -1;
    struct v4l2_buffer buf;

    for (unsigned int i = 0; i < bufferCnt; i++)
    {
        memset(&buf, 0, sizeof (struct v4l2_buffer));
        buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index  = i;

        ret = ioctl(v4l2Handle, VIDIOC_QUERYBUF, &buf);
        if (ret < 0)
        {
            printf("Unable to query buffer (%s)\n", strerror(errno));
            return ret;
        }

        mapMem.mem[i] = mmap(0, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, v4l2Handle, buf.m.offset);
		//mapMem.phyaddr[i] = buf.m.offset;
        mapMem.length = buf.length;
        printf("index: %d, mem: %x, len: %x, offset: %x\n", i, (int)mapMem.mem[i], buf.length, buf.m.offset);

        if (mapMem.mem[i] == MAP_FAILED)
        {
            printf("Unable to map buffer (%s)\n", strerror(errno));
            return -1;
        }

        ret = ioctl(v4l2Handle, VIDIOC_QBUF, &buf);
        if (ret < 0)
        {
            printf("VIDIOC_QBUF Failed\n");
            return ret;
        }
        getGeometry();
    }
    return 0;
}

int v4l2StartStreaming()
{
    int ret = -1;
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    ret = ioctl(v4l2Handle, VIDIOC_STREAMON, &type);
    if (ret < 0)
    {
        printf("StartStreaming: Unable to start capture: %s\n",strerror(errno));
        return ret;
    }
    return 0;
}

int v4l2StopStreaming()
{
    int ret = -1;
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    ret = ioctl(v4l2Handle, VIDIOC_STREAMOFF, &type);
    if (ret < 0)
    {
        printf("StopStreaming: Unable to stop capture: %s\n", strerror(errno));
        return ret;
    }
    printf("V4L2Camera::v4l2StopStreaming OK\n");
    return 0;
}

int v4l2UnmapBuf()
{
    int ret = -1;

    for (unsigned int i = 0; i < bufferCnt; i++)
    {
        ret = munmap(mapMem.mem[i], mapMem.length);
        if (ret < 0)
        {
            printf("v4l2CloseBuf Unmap failed\n");
            return ret;
        }
    }
    return 0;
}

int v4l2WaitCamerReady()
{
    fd_set fds;
    struct timeval tv;
    int r;

    FD_ZERO(&fds);
    FD_SET(v4l2Handle, &fds);

    /* Timeout */
    tv.tv_sec  = 2;
    tv.tv_usec = 0;

    r = select(v4l2Handle + 1, &fds, NULL, NULL, &tv);
    if (r == -1)
    {
        printf("select err\n");
        return -1;
    }
    else if (r == 0)
    {
        printf("select timeout\n");
        return -1;
    }
    return 0;
}

int getPreviewFrame(v4l2_buffer *buf)
{
    int ret = -1;

    buf->type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf->memory = V4L2_MEMORY_MMAP;

    ret = ioctl(v4l2Handle, VIDIOC_DQBUF, buf);
    if (ret < 0)
    {
        printf("GetPreviewFrame: VIDIOC_DQBUF Failed\n");
        return __LINE__; 			                // can not return false
    }

    return 0;
}

void releasePreviewFrame(int index)
{
    struct v4l2_buffer buf;
    int ret = -1;
    memset(&buf, 0, sizeof(v4l2_buffer));
    buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = index;
    ret = ioctl(v4l2Handle, VIDIOC_QBUF, &buf);
    if (ret != 0)
    {
        printf("releasePreviewFrame: VIDIOC_QBUF Failed: index = %d, ret = %d, %s\n",buf.index, ret, strerror(errno));
    }
}

/*int resetTVDecoder()
{
    int32_t ret;
    ret = v4l2StopStreaming();
    if (ret != 0)
    {
        printf("ERROR: v4l2StopStreaming error, line: %d\n", __LINE__);
        return -1;
    }

    ret = v4l2UnmapBuf();
    if (ret != 0)
    {
        printf("ERROR: v4l2UnmapBuf error, line: %d\n", __LINE__);
        return -1;
    }

    ret = v4l2SetVideoParams();
    if (ret != 0)
    {
        printf("ERROR: v4l2SetVideoParams error, line: %d\n", __LINE__);
        return -1;
    }
    ret = v4l2ReqBufs();
    if (ret != 0)
    {
        printf("ERROR: v4l2ReqBufs error, line: %d\n", __LINE__);
        return -1;
    }
    ret = v4l2QueryBuf();
    if (ret != 0)
    {
        printf("ERROR: v4l2QueryBuf error, line: %d\n", __LINE__);
        return -1;
    }
    ret = v4l2StartStreaming();
    if (ret != 0)
    {
        printf("ERROR: v4l2SetVideoParams error, line: %d\n", __LINE__);
        return -1;
    }
    return ret;
}
*/

/*int getSystem(int *i_system)
{
    int ret = OK;
    struct v4l2_format format;
    memset(&format, 0, sizeof(format));
    format.type = V4L2_BUF_TYPE_PRIVATE;
    if (-1 == ioctl(v4l2Handle, VIDIOC_G_FMT, &format))
    {
        ALOGE("VIDIOC_G_FMT error!  a\n");
        ret = -1;
        return ret;
    }
    switch(channel)
    {
        case TVD_CHANNEL_ONLY_1:
        case TVD_CHANNEL_ALL_2x2:
        case TVD_CHANNEL_ALL_1x4:
        case TVD_CHANNEL_ALL_4x1:
            if((format.fmt.raw_data[16] & 1) == 0)
            {
                ret = -1;
                return ret;
            }
            if((format.fmt.raw_data[16]&(1<<4))!=0)
            {
                *i_system = TVD_PAL;
            }
            else if((format.fmt.raw_data[16]&(1<<4))==0)
            {
                *i_system = TVD_NTSC;
            }
            ALOGV("format.fmt.raw_data[16] =0x%x",format.fmt.raw_data[16]);
            break;
        case TVD_CHANNEL_ONLY_2:
            if((format.fmt.raw_data[17] & 1) == 0)
            {
                ret = -1;
                return ret;
            }
            if((format.fmt.raw_data[17]&(1<<4))!=0)
            {
                *i_system = TVD_PAL;
            }
            else if((format.fmt.raw_data[17]&(1<<4))==0)
            {
                *i_system = TVD_NTSC;
            }
            ALOGV("format.fmt.raw_data[17] =0x%x",format.fmt.raw_data[17]);
            break;
        case TVD_CHANNEL_ONLY_3:
            if((format.fmt.raw_data[18] & 1) == 0)
            {
                ret = -1;
                return ret;
            }
            if((format.fmt.raw_data[18]&(1<<4))!=0)
            {
                *i_system = TVD_PAL;
            }
            else if((format.fmt.raw_data[18]&(1<<4))==0)
            {
                *i_system = TVD_NTSC;
            }
            ALOGV("format.fmt.raw_data[18] =0x%x",format.fmt.raw_data[18]);
            break;
        case TVD_CHANNEL_ONLY_4:
            if((format.fmt.raw_data[19] & 1) == 0)
            {
                ret = -1;
                return ret;
            }
            if((format.fmt.raw_data[19]&(1<<4))!=0)
            {
                *i_system = TVD_PAL;
            }
            else if((format.fmt.raw_data[19]&(1<<4))==0)
            {
                *i_system = TVD_NTSC;
            }
            ALOGV("format.fmt.raw_data[19] =0x%x",format.fmt.raw_data[19]);
            break;
    }
    return ret;
}*/

#define LEFT_SPAN           18
#define RIGHT_SPAN          14

int main()
{
    int ret;

    FILE *fp = fopen("/tmp/tv_capture.yuv", "wb");
	if (!fp)
	{
		printf("Open tv_capture.yuv failed\n");
		return -1;
    }

	ret = cedarx_hardware_init(0);
	if (ret < 0)
	{
		printf("cedarx_hardware_init failed\n");
		return -1;
	}

	render_init();

    memset(&mapMem, 0, sizeof(v4l2_mem_map_t));

    openCameraDev();
    ret = v4l2SetVideoParams();
    if (ret != 0)
    {
        printf("ERROR: v4l2SetVideoParams error, line: %d\n", __LINE__);
        return -1;
    }
    ret = v4l2ReqBufs();
    if (ret != 0)
    {
        printf("ERROR: v4l2ReqBufs error, line: %d\n", __LINE__);
        return -1;
    }
    ret = v4l2QueryBuf();
    if (ret != 0)
    {
        printf("ERROR: v4l2QueryBuf error, line: %d\n", __LINE__);
        return -1;
    }

	char *img;
    img = (char *)cedar_sys_phymalloc_map(720 * 576 * 3/2, 1024);//(720 - LEFT_SPAN - RIGHT_SPAN) * 576 * 3 / 2, 1024);
    unsigned int phy_img = cedarv_address_vir2phy((void*)img);

    ret = v4l2StartStreaming();
    if (ret != 0)
    {
        printf("ERROR: v4l2StartStreaming error, line: %d\n", __LINE__);
        return -1;
    }

	int i = 0;
    while (1)
    {
    	printf("%d\n", i++);
    
        ret = v4l2WaitCamerReady();
        if (ret != 0) continue;

        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(v4l2_buffer));
        ret = getPreviewFrame(&buf);
        if (ret != 0)
        {
            usleep(10000);
            continue;
        }
        
        //char tmp_buf[720 * 576 / 2];
        char *ptr = (char *)mapMem.mem[buf.index];
        /*char *ptr2 = img;
        int i;
        for (i = 0; i < 576 + 288; i++)
        {
            memcpy(ptr2, ptr + LEFT_SPAN, 720 - LEFT_SPAN - RIGHT_SPAN);
            ptr2 += 720 - LEFT_SPAN - RIGHT_SPAN;
            ptr += 720;
        }*/

        memcpy(img, ptr, 720 * 576);
        //memcpy(img + 720 * 576 * 5 / 4, ptr + 720 * 576, 720 * 576 / 4);
        //memcpy(img + 720 * 576, ptr + 720 * 576 * 5 / 4, 720 * 576 / 4);
        int j;
        ptr += 720 * 576;
        for (j = 0; j < 720 * 576 / 4; j++)
        {
            img[720 * 576 + j] = ptr[j * 2];
            img[720 * 576 + 720 * 576 / 4 + j] = ptr[j * 2 + 1];
        }

        ////fwrite(mapMem.mem[buf.index], 1, 720 * 576, fp);
        fwrite(img, 1, 720 * 576 * 3 / 2, fp);
        fflush(fp);

        cedarv_picture_t pic;
        memset(&pic, 0, sizeof(pic));
        pic.width = 720 - LEFT_SPAN - RIGHT_SPAN;
		pic.height = 576;
        pic.display_width = 720 - LEFT_SPAN - RIGHT_SPAN;
		pic.display_height = 576;
		pic.frame_rate = 25;
		pic.aspect_ratio = 1331;
		pic.is_progressive = 1;
		pic.top_field_first = 1;
		pic.repeat_bottom_field = 1;
		pic.pixel_format == CEDARV_PIXEL_FORMAT_YUV420;
		pic.anaglath_transform_mode = CEDARV_ANAGLAGH_NONE;
        pic.size_y = (720 - LEFT_SPAN - RIGHT_SPAN) * 576;
        pic.size_u = (720 - LEFT_SPAN - RIGHT_SPAN) * 576 / 4;
        pic.size_v = (720 - LEFT_SPAN - RIGHT_SPAN) * 576 / 4;

		pic.y = (u8 *)phy_img;
        pic.u = (u8 *)phy_img + (720 - LEFT_SPAN - RIGHT_SPAN) * 576;
        pic.v = (u8 *)phy_img + (720 - LEFT_SPAN - RIGHT_SPAN) * 576 * 5 / 4;
        
        render_render((void *)&pic, i, 0, 0, 1024, 600);
        
        releasePreviewFrame(buf.index);
    }

    ////fclose(fp);

    v4l2StopStreaming();
    v4l2UnmapBuf();
    closeCameraDev();

	render_exit();

	cedar_sys_phyfree_map(img);

	cedarx_hardware_exit(0);

    return 0;
}

