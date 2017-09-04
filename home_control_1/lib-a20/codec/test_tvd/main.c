#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <time.h>
#include <linux/fb.h>
#include <linux/input.h>
#include "drv_display.h"

typedef enum
{
TVD_UV_SWAP,
TVD_COLOR_SET,
}tvd_param_t;

typedef enum
{
TVD_CVBS,
TVD_YPBPR_I,
TVD_YPBPR_P,
}tvd_interface_t;

typedef enum
{
    TVD_NTSC,
    TVD_PAL,
    TVD_SECAM,
}tvd_system_t;

typedef enum
{
    TVD_PL_YUV420,
    TVD_MB_YUV420,
    TVD_PL_YUV422,
}tvd_fmt_t;

typedef enum
{
    TVD_CHANNEL_ONLY_1,
    TVD_CHANNEL_ONLY_2,
    TVD_CHANNEL_ONLY_3,
    TVD_CHANNEL_ONLY_4,
    TVD_CHANNEL_ALL_2x2,
    TVD_CHANNEL_ALL_1x4,
    TVD_CHANNEL_ALL_4x1,
    TVD_CHANNEL_CH1_CH2,
}tvd_channel_t;

typedef enum e_hwc_format
{
    HWC_FORMAT_MINVALUE  = 0x50,
    HWC_FORMAT_RGBA_8888= 0x51,
    HWC_FORMAT_RGB_565= 0x52,
    HWC_FORMAT_BGRA_8888= 0x53,
    HWC_FORMAT_YCbYCr_422_I= 0x54,
    HWC_FORMAT_CbYCrY_422_I= 0x55,
    HWC_FORMAT_MBYUV420= 0x56,
    HWC_FORMAT_MBYUV422= 0x57,
    HWC_FORMAT_YUV420PLANAR= 0x58,
    HWC_FORMAT_DEFAULT= 0x99,
    HWC_FORMAT_MAXVALUE= 0x100
}e_hwc_format_t;

typedef struct tag_HWCLayerInitPara
{
    __u32 w;
    _u32 h;
    __u32 format;
    __u32 screenid;
}layerinitpara_t;

typedef struct tag_LIBHWCLAYERPARA
{
unsigned long number;
unsigned long top_y;//the address of frame buffer,shick contains top field luminance
unsigned long top_c;//the address of frame buffer,which contains top field chrominance
unsigned long bottom_y;//the address of buffer,which contains bottom field luminance
unsigned long bottom_c;//the address of buffer,which contains bottom field chrominance
signed char bProgressiveSrc;// Indicating the source is progressive or not
signed char  bTopFieldFirst;// VPO should check this flag when bProgressiveSrc is FALSE
unsigned long flag_addr;//dit maf flag address
unsigned long flag_stride;//dit maf flag line stride
unsigned char maf_valid;
unsigned char pre_frame_valid;
}libhwclayerpara_t;

#define CLEAR(x) memset (&(x), 0, sizeof (x))
struct buffer {
    void *start;
    size_t length;
};

static int fd = -1;
struct buffer * buffers  = NULL;
static unsigned int n_buffers = 0;

int first_frame=1;
int disphd;
unsigned int hlay;
int sel = 0;
__disp_layer_info_t layer_para;
__disp_video_fb_t video_fb;
layerinitpara_t layerinit;
libhwclayerpara_t overlay_para;
__u32 arg[4];

int preview_width = 1280;
int preview_height = 800;

tvd_interface_t tvd_interface = TVD_CVBS;
tvd_system_t tvd_system = TVD_NTSC;
tvd_fmt_t  tvd_format = TVD_MB_YUV420;;
tvd_channel_t tvd_channel = TVD_CHANNEL_ONLY_1;


int ch0_enable = 1;
int ch1_enable = 4;
void capture_yuv420_raw_data(int width,int height,void *y_addr,void *c_addr)
{
        FILE * y_file;
        FILE * c_file;
        y_file=fopen("/y_frame.bin","wb");
        if (!y_file)
        {
        fprintf(stderr, "Unable to create file\n");
        }
        fseek(y_file, 0, SEEK_SET);
        if (fwrite(y_addr,1,width*height,y_file)!=width*height)
        printf("Error fwrite\n");
        fclose(y_file);
        c_file=fopen("/c_frame.bin","wb");
        if (!c_file) {
        fprintf(stderr, "Unable to create file\n");
        }

        fseek(c_file, 0, SEEK_SET);
        fwrite(c_addr,1,width*height*3/2,c_file);
        fclose(c_file);
}

int disp_init(int w,int h,layerinitpara_t *layer_info)
{
        __disp_pixel_fmt_t
        disp_format;
        __disp_pixel_mod_t
        fb_mode;
        __disp_pixel_seq_t disp_seq;
        if((disphd = open("/dev/disp",O_RDWR)) == -1)
        {
                printf("open file /dev/disp fail. \n");
                return -1;
        }
        #if 0
        arg[0] = 0;
        ioctl(disphd, DISP_CMD_LCD_ON, (void*)arg);
        #endif
        arg[0] = 0;
        arg[1] = DISP_LAYER_WORK_MODE_SCALER;
        hlay = ioctl(disphd, DISP_CMD_LAYER_REQUEST, (void*)arg);
        if(hlay == 0)
        {
                printf("request layer0 fail\n");
                return -1;
        }
        disp_seq = DISP_SEQ_UVUV;
        if (layer_info->h < 720)
        {
                layer_para.fb.cs_mode
                = DISP_BT601;//SDTV
        }
        else
        {
                layer_para.fb.cs_mode
                = DISP_BT709;//HDTV
        }
        switch(layer_info->format)
        {
            case HWC_FORMAT_DEFAULT:
                     disp_format  = DISP_FORMAT_YUV420;
                       fb_mode = DISP_MOD_NON_MB_UV_COMBINED;
            break;
            case HWC_FORMAT_MBYUV420:
                    disp_format = DISP_FORMAT_YUV420;
                    fb_mode = DISP_MOD_MB_UV_COMBINED;
            break;
            case HWC_FORMAT_MBYUV422:
                    disp_format = DISP_FORMAT_YUV422;
                    fb_mode =  DISP_MOD_NON_MB_UV_COMBINED;
            break;
            case HWC_FORMAT_YUV420PLANAR:
                    disp_format   = DISP_FORMAT_YUV420;
                    fb_mode  = DISP_MOD_NON_MB_PLANAR;
                    disp_seq   = DISP_SEQ_P3210;
            break;
            case HWC_FORMAT_RGBA_8888:
                    disp_format    = DISP_FORMAT_ARGB8888;
                    fb_mode   = DISP_MOD_NON_MB_PLANAR;
                    disp_seq    = DISP_SEQ_P3210;
            break;
            default:
                    disp_format   = DISP_FORMAT_YUV420;
                    fb_mode    = DISP_MOD_NON_MB_PLANAR;
            break;
            }

            layer_para.fb.mode = fb_mode;
            layer_para.fb.format = disp_format;
            layer_para.fb.br_swap = 0;
            layer_para.fb.seq             = disp_seq;
            layer_para.fb.addr[0]       = 0;
            layer_para.fb.addr[1]      = 0;
            layer_para.fb.addr[2]      = 0;
            layer_para.fb.size.width       = layer_info->w;
            layer_para.fb.size.height   = layer_info->h;
            layer_para.mode = DISP_LAYER_WORK_MODE_SCALER;
            layer_para.alpha_en = 1;
            layer_para.alpha_val     = 0xff;
            layer_para.pipe = 1;
            layer_para.src_win.x = 10;
            layer_para.src_win.y = 5;
            layer_para.src_win.width = layer_info->w - 10;

            layer_para.src_win.height  = layer_info->h -5;
            layer_para.fb.b_trd_src    = 0;
            layer_para.b_trd_out     = 0;
            layer_para.fb.trd_mode   = DISP_3D_SRC_MODE_SSH;
            layer_para.out_trd_mode    = DISP_3D_OUT_MODE_FP;
            layer_para.b_from_screen    = 0;
            layer_para.scn_win.x    = 0;
            layer_para.scn_win.y     = 0;
            layer_para.scn_win.width  = w;
            layer_para.scn_win.height  = h;

            arg[0] = sel;
            arg[1] = hlay;
            arg[2] = (__u32)&layer_para;
            ioctl(disphd,DISP_CMD_LAYER_SET_PARA,(void*)arg);
            int fb_fd;
            unsigned long fb_layer;
            fb_fd = open("/dev/graphics/fb0", O_RDWR);
            if (ioctl(fb_fd, FBIOGET_LAYER_HDL_0, &fb_layer) == -1) {
            printf("get fb layer handel failed\n");
            }
            close(fb_fd);
            arg[0] = 0;
            arg[1] = fb_layer;
            ioctl(disphd, DISP_CMD_LAYER_BOTTOM, (void *)arg);
            return 0;
 }

    void disp_start(void)
    {
            arg[0] = sel;
            arg[1] = hlay;
            ioctl(disphd, DISP_CMD_VIDEO_START, (void*)arg);
    }

    void disp_stop(void)
    {
        arg[0] = sel;
        arg[1] = hlay;
        ioctl(disphd, DISP_CMD_VIDEO_STOP,(void*)arg);
    }
void disp_exit()
{
        arg[0] = sel;
        arg[1] = hlay;
        ioctl(disphd, DISP_CMD_LAYER_RELEASE, (void*)arg);
        close (disphd);

        int fb_fd;
        unsigned long fb_layer;
        fb_fd = open("/dev/graphics/fb0", O_RDWR);
        if (ioctl(fb_fd, FBIOGET_LAYER_HDL_0, &fb_layer) == -1) {
        printf("get fb layer handel failed\n");
        }
        close(fb_fd);
        arg[0] = 0;
        arg[1] = fb_layer;
        ioctl(disphd, DISP_CMD_LAYER_TOP, (void *)arg);
  }
void disp_on()
{
        arg[0] = 0;
        ioctl(disphd, DISP_CMD_LCD_ON, (void*)arg);
}
void disp_set_addr(libhwclayerpara_t overlaypara)
{
        __disp_video_fb_t fb_addr;
        memset(&fb_addr, 0, sizeof(__disp_video_fb_t));
        fb_addr.interlace  = overlaypara.bProgressiveSrc?0:1;
        fb_addr.top_field_first = overlaypara.bTopFieldFirst;
        fb_addr.frame_rate = 25;
        fb_addr.addr[0] = overlaypara.top_y;




        fb_addr.addr[1]  = overlaypara.top_c;
        fb_addr.addr[2] = overlaypara.bottom_y;
        fb_addr.addr_right[0] = overlaypara.bottom_y;
        fb_addr.addr_right[1]      = overlaypara.bottom_c;
        fb_addr.addr_right[2] = 0;
        fb_addr.id   = overlaypara.number;

        arg[0] = sel;
        arg[1] = hlay;
        arg[2] = (__u32)&fb_addr;

        ioctl(disphd, DISP_CMD_VIDEO_SET_FB, (void*)arg);
 }
 void layer_open()
 {
        arg[0] = sel;
        arg[1] = hlay;
        ioctl(disphd, DISP_CMD_LAYER_OPEN,(void*)arg);
 }
 void layer_close()
 {
        arg[0] = sel;
        arg[1] = hlay;
        ioctl(disphd, DISP_CMD_LAYER_CLOSE,(void*)arg);
 }

 static int read_frame (void)
 {
        struct v4l2_buffer buf;
        CLEAR (buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        ioctl (fd, VIDIOC_DQBUF, &buf);
        overlay_para.bProgressiveSrc   = 0;
        overlay_para.bTopFieldFirst  = 1;
        overlay_para.top_y = (unsigned int)buf.m.offset;
        overlay_para.top_c  =  (unsigned int)buf.m.offset+layerinit.w*layerinit.h;
        overlay_para.bottom_y  = 0;
        overlay_para.bottom_c = 0;
        overlay_para.number = 0;
        #if 0
        if(first_frame)
        {
            capture_yuv420_raw_data(layerinit.w,layerinit.h,(void*)buffers[buf.index].start,(void*)buffer
            s[buf.index].start+layerinit.w*layerinit.h);
        }
        #endif

        disp_set_addr(overlay_para);
        ioctl (fd, VIDIOC_QBUF, &buf);
        return 1;
 }
  int set_params(void)
  {
        int ret = -1;
        struct v4l2_format v_format;
        memset(&v_format, 0, sizeof(v_format));
        v_format.type = V4L2_BUF_TYPE_PRIVATE;
        switch(tvd_interface)
        {
        case TVD_CVBS:
             v_format.fmt.raw_data[0] = 0;
        break;
        case TVD_YPBPR_I:
               v_format.fmt.raw_data[0] = 1;
        break;
        case TVD_YPBPR_P:
                v_format.fmt.raw_data[0] = 2;
        break;
        default:
        break;
        }
        switch(tvd_system)
        {
        case TVD_NTSC:
                v_format.fmt.raw_data[1] = 0;
        break;
        case TVD_PAL:
                v_format.fmt.raw_data[1]     = 1;
        break;
        case TVD_SECAM:
                 v_format.fmt.raw_data[1]    = 2;
        break;
        default:
        break;
        }
        switch(tvd_format)
        {
        case TVD_PL_YUV420:
                v_format.fmt.raw_data[2] = 0;
        break;
        case TVD_MB_YUV420:
                v_format.fmt.raw_data[2] = 1;
        break;
        case TVD_PL_YUV422:
                v_format.fmt.raw_data[2] = 2;
        break;
        default:
        break;
        }
        switch(tvd_channel)
        {
        case TVD_CHANNEL_ONLY_1:
        v_format.fmt.raw_data[8] = 1;
        v_format.fmt.raw_data[9] = 1;
        v_format.fmt.raw_data[10] = 1;
        v_format.fmt.raw_data[11] = 0;
        v_format.fmt.raw_data[12] = 0;
        v_format.fmt.raw_data[13] = 0;
        break;
        case TVD_CHANNEL_ONLY_2:
        v_format.fmt.raw_data[8] = 1;
        v_format.fmt.raw_data[9] = 1;
        v_format.fmt.raw_data[10] = 0;
        v_format.fmt.raw_data[11] = 1;
        v_format.fmt.raw_data[12] = 0;
        v_format.fmt.raw_data[13] = 0;
        break;
        case TVD_CHANNEL_ONLY_3:
            v_format.fmt.raw_data[8] = 1;
            v_format.fmt.raw_data[9] = 1;
            v_format.fmt.raw_data[10] = 0;
            v_format.fmt.raw_data[11] = 0;
            v_format.fmt.raw_data[12] = 1;
            v_format.fmt.raw_data[13] = 0;
            break;
            case TVD_CHANNEL_ONLY_4:
            v_format.fmt.raw_data[8] = 1;
            v_format.fmt.raw_data[9] = 1;
            v_format.fmt.raw_data[10] = 0;
            v_format.fmt.raw_data[11] = 0;
            v_format.fmt.raw_data[12] = 0;
            v_format.fmt.raw_data[13] = 1;
            break;
            case TVD_CHANNEL_ALL_2x2:
            v_format.fmt.raw_data[8] = 2;
            v_format.fmt.raw_data[9] = 2;
            v_format.fmt.raw_data[10] = 1;
            v_format.fmt.raw_data[11] = 2;
            v_format.fmt.raw_data[12] = 3;
            v_format.fmt.raw_data[13] = 4;
            break;
            case TVD_CHANNEL_ALL_1x4:
            v_format.fmt.raw_data[8] = 1;
            v_format.fmt.raw_data[9] = 4;
            v_format.fmt.raw_data[10] = 1;
            v_format.fmt.raw_data[11] = 2;
            v_format.fmt.raw_data[12] = 3;
            v_format.fmt.raw_data[13] = 4;
            break;
            case TVD_CHANNEL_ALL_4x1:
            v_format.fmt.raw_data[8] = 4;
            v_format.fmt.raw_data[9] = 1;
            v_format.fmt.raw_data[10] = 1;
            v_format.fmt.raw_data[11] = 2;
            v_format.fmt.raw_data[12] = 3;
            v_format.fmt.raw_data[13] = 4;
            break;
            case TVD_CHANNEL_CH1_CH2:
            v_format.fmt.raw_data[8] = 1;
            v_format.fmt.raw_data[9] = 1;
            v_format.fmt.raw_data[10] = ch0_enable;
            v_format.fmt.raw_data[11] = ch1_enable;
            //channel_index0
            //channel_index1
            //v_format.fmt.raw_data[10] = 1;
            //v_format.fmt.raw_data[11] = 0;
            v_format.fmt.raw_data[12] = 0;
            v_format.fmt.raw_data[13] = 0;
            break;
            //channel_index0
            //channel_index1
            //channel_index2
            //channel_index 3
            default:
            break;
            }
            if (-1 == ioctl (fd, VIDIOC_S_FMT, &v_format))
            {
                    printf("VIDIOC_S_FMT error!\n");
                    ret = -1;
                    return ret;
            }
            printf("\n#########################interface  %d format %d\n",tvd_interface,tvd_system,tvd_format);
            struct v4l2_streamparm parms;
            parms.type = V4L2_BUF_TYPE_PRIVATE;
            parms.parm.raw_data[0] = TVD_COLOR_SET;
            parms.parm.raw_data[1] = 0x20; //luma(0~255)
            parms.parm.raw_data[2] = 0x80;//contrast(0~255)
            parms.parm.raw_data[3] = 0x80;//saturation(0~255)
            parms.parm.raw_data[4] = 0x0;//hue(0~255)

            if(ioctl(fd, VIDIOC_S_PARM, &parms)==-1)
            {
                    printf("VIDIOC_S_PARM error!\n");
                    ret = -1;
                    return ret;
            }

            parms.type = V4L2_BUF_TYPE_PRIVATE;
            parms.parm.raw_data[0] = TVD_UV_SWAP;
            parms.parm.raw_data[1] = 0;
            if(ioctl(fd, VIDIOC_S_PARM, &parms)==-1)
            {
                    printf("VIDIOC_S_PARM error!\n");
                    ret = -1;
                    return ret;
            }

            return 0;
}
int get_system(tvd_system_t *system, int *enable_ch0, int *enable_ch1, int *signal_det)
{
            int ret = 0;
            int ch0_signal, ch1_signal;
            struct v4l2_format format;
            memset(&format, 0, sizeof(format));
            format.type = V4L2_BUF_TYPE_PRIVATE;
            if (-1 == ioctl (fd, VIDIOC_G_FMT, &format))
            {
            printf("VIDIOC_G_FMT error!\n");
            ret = -1;
            return ret;
            }
            switch(tvd_channel)
            {
            case TVD_CHANNEL_ONLY_1:
            case TVD_CHANNEL_ALL_2x2:
            case TVD_CHANNEL_ALL_1x4:
            case TVD_CHANNEL_ALL_4x1:
            //printf("0x%x\n", format.fmt.raw_data[16]);
            if((format.fmt.raw_data[16] & 1) == 0)
            {
            printf("no signal detected\n");
            *signal_det = 0;
            ret = -1;
            return ret;
            } else {
            printf("signal detected\n");
            *signal_det = 1;
            }
            if((format.fmt.raw_data[16]&(1<<4))!=0)
            {
            *system = TVD_PAL;
            }
            else if((format.fmt.raw_data[16]&(1<<4))==0)
            {
            *system = TVD_NTSC;
            }
            break;
            case TVD_CHANNEL_ONLY_2:
            if((format.fmt.raw_data[17] & 1) == 0)
            {
            ret = -1;
            return ret;
            }
            if((format.fmt.raw_data[17]&(1<<4))!=0)
            {
            *system = TVD_PAL;
            }
            else if((format.fmt.raw_data[17]&(1<<4))==0)
            {
            *system = TVD_NTSC;
            }
            break;
            case TVD_CHANNEL_ONLY_3:
            if((format.fmt.raw_data[18] & 1) == 0)
            {
            ret = -1;
            return ret;
            }
            if((format.fmt.raw_data[18]&(1<<4))!=0)
            {
            *system = TVD_PAL;
            }
            else if((format.fmt.raw_data[18]&(1<<4))==0)
            {
            *system = TVD_NTSC;
            }
            break;
            case TVD_CHANNEL_ONLY_4:
            if((format.fmt.raw_data[19] & 1) == 0)
            {
            ret = -1;
            return ret;
            }
            if((format.fmt.raw_data[19]&(1<<4))!=0)
            {
            *system = TVD_PAL;
            }
            else if((format.fmt.raw_data[19]&(1<<4))==0)
            {
            *system = TVD_NTSC;
            }
            break;
            case TVD_CHANNEL_CH1_CH2:
            ch0_signal = format.fmt.raw_data[16] & 1;
            ch1_signal = format.fmt.raw_data[17] & 1;
            printf("ch0_signal:%d,ch1_signal:%d\n", ch0_signal, ch1_signal);
            if ((!ch0_signal) && (!ch1_signal))
            {
            printf("no signal detected\n");
            *signal_det = 0;
            ret = -1;
            return ret;
            } else {
            *signal_det = 1;
            }
            if ((ch0_signal & (!ch1_signal)) || (ch0_signal & ch1_signal)) {
            *enable_ch0 = 1;
            *enable_ch1 = 4;
            if((format.fmt.raw_data[16]&(1<<4))!=0)
            {
            *system = TVD_PAL;
            }
            else if((format.fmt.raw_data[16]&(1<<4))==0)
            {
            *system = TVD_NTSC;
            }
            } else if (ch1_signal & (!ch0_signal)) {
            *enable_ch0 = 4;
            *enable_ch1 = 1;
            if((format.fmt.raw_data[17]&(1<<4))!=0)
            {
            *system = TVD_PAL;
            }
            else if((format.fmt.raw_data[17]&(1<<4))==0)
            {
            *system = TVD_NTSC;
            }
            }
            printf("enable_ch0:%d,enable_ch1:%d\n", *enable_ch0, *enable_ch1);
            break;
            }
            return ret;
            }

 int get_geometry(void)
 {
            struct v4l2_format format;
            int ret = -1;
            memset(&format, 0, sizeof(format));
            format.type
            = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            ret = ioctl(fd, VIDIOC_G_FMT, &format);
            if (ret < 0)
            {
            printf("VIDIOC_G_FMT Failed: %s", strerror(errno));
            return ret;
            }
            layerinit.w   = format.fmt.pix.width;
            layerinit.h = format.fmt.pix.height;


            return 0;
 }

            static void terminate(int sig_no)
            {
            enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            int i=0;
            if (-1 == ioctl (fd, VIDIOC_STREAMOFF, &type))
            printf ("VIDIOC_STREAMOFF failed\n");
            for (i = 0; i < n_buffers; ++i) {
            if (-1 == munmap (buffers[i].start, buffers[i].length)) {
            printf ("munmap error");
            }
            }
            disp_stop();
            disp_exit();
            close (fd);
            printf("TVD exit!\n");
            exit(1);
            }
            static void install_sig_handler(void)
            {
            signal(SIGBUS, terminate);
            signal(SIGFPE, terminate);
            signal(SIGHUP, terminate);
            signal(SIGILL, terminate);
            signal(SIGINT, terminate);
            signal(SIGIOT, terminate);
            signal(SIGPIPE, terminate);
            signal(SIGQUIT, terminate);
            signal(SIGSEGV, terminate);
            signal(SIGSYS, terminate);
            signal(SIGTERM, terminate);
            signal(SIGTRAP, terminate);
            signal(SIGUSR1, terminate);
            signal(SIGUSR2, terminate);
            }

 int parse_cmdline(int argc, char **argv)
{
            int i=0;
            while(i < argc)
            {
                            if(!strcmp(argv[i], "-i")){
                                          if (argc > i+1) {
                                                            i++;
                                                            if(!strcmp(argv[i], "0"))
                                                            {
                                                                        tvd_interface = TVD_CVBS;
                                                            }
                                                            else if(!strcmp(argv[i], "1"))
                                                            {
                                                                            tvd_interface = TVD_YPBPR_I;
                                                            }
                                                            else if(!strcmp(argv[i], "2"))
                                                            {
                                                                            tvd_interface = TVD_YPBPR_P;
                                                            }
                                                            continue;
                                          }
                            }

                            if(!strcmp(argv[i], "-f")){
                                            if (argc > i+1) {
                                                            i++;
                                                            if(!strcmp(argv[i], "0"))
                                                            {
                                                                            layerinit.format = WC_FORMAT_DEFAULT;
                                                                            tvd_format = VD_PL_YUV420;
                                                            }
                                                            else if(!strcmp(argv[i], "1"))
                                                            {
                                                                            layerinit.format = HWC_FORMAT_MBYUV420;
                                                                            tvd_format = TVD_MB_YUV420;
                                                            }
                                                            else if(!strcmp(argv[i], "2"))
                                                            {
                                                                            layerinit.format = HWC_FORMAT_MBYUV422;
                                                                            tvd_format = TVD_PL_YUV422;
                                                            }
                                                            continue;
                                          }
                         }

                          if(!strcmp(argv[i], "-c")){
                                            if (argc > i+1) {
                                                            i++;
                                                            if(!strcmp(argv[i], "0"))
                                                            {
                                                                            tvd_channel
                                                                            = TVD_CHANNEL_ONLY_1;
                                                             }
                                                            else if(!strcmp(argv[i], "1"))
                                                            {
                                                                            tvd_channel = TVD_CHANNEL_ONLY_2;
                                                            }
                                                            else if(!strcmp(argv[i], "2"))
                                                            {
                                                                            tvd_channel
                                                                            = TVD_CHANNEL_CH1_CH2;
                                                            }
                                                            continue;
                                                            }
            }
            if(!strcmp(argv[i], "-h")){
            printf("usage:\n"  "\t-i interface
            "\t-f fomat
            0 = cvbs
            1 = ypbpr\n"
            0 = PL_YUV420
            1 =
            MB_YUV420\n");
            return -1;
            }
            i++;
            }
            return 0;
            }


int main(int argc, char **argv)
{
            int i;

            int key_fd = -1;

            int quit = 0;

            int signal_det = 0;

            int ch0_signal;

            int ch1_signal;

            tvd_system_t new_system;

            tvd_interface_t new_tvd_interface;

            struct input_event data;

            layerinit.format = HWC_FORMAT_MBYUV420;

            if(parse_cmdline(argc,argv)==-1)
            return 0;
            install_sig_handler();

            printf("*********************\n");
            printf("TVD start!\n");
            printf("Press KEY_ESC for exit\n");
            printf("*********************\n");
            if((key_fd = open("/dev/input/event0", O_RDONLY | O_NONBLOCK)) < 0){//key:linux=event0, android=event2
                    printf("open event fail!\n");
                    return -1;
             }

reset:
                    fd = open ("/dev/video1", O_RDWR | O_NONBLOCK, 0);

                    first_frame=1;

                    set_params();

                    usleep(200*1000);

                    get_geometry();

                    struct v4l2_requestbuffers req;

                    CLEAR (req);
                    req.count = 5;
                    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                    req.memory = V4L2_MEMORY_MMAP;
                    ioctl (fd, VIDIOC_REQBUFS, &req);
                    buffers = calloc (req.count, sizeof (*buffers));
                    for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
                    {
                    struct v4l2_buffer buf;
                    CLEAR (buf);
                    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                    buf.memory = V4L2_MEMORY_MMAP;
                    buf.index = n_buffers;
                    if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buf))
                    printf ("VIDIOC_QUERYBUF error\n");
                    buffers[n_buffers].length = buf.length;
                    buffers[n_buffers].start = mmap(NULL,buf.length,PROT_READ|PROT_WRITE ,MAP_SHARED,fd,buf.m.offset);

                    if (MAP_FAILED == buffers[n_buffers].start)
                            printf ("mmap failed\n");
                    }

                    for (i = 0; i < n_buffers; ++i)
                    {
                        struct v4l2_buffer buf;
                        CLEAR (buf);
                        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                        buf.memory = V4L2_MEMORY_MMAP;
                        buf.index = i;

                        if (-1 == ioctl (fd, VIDIOC_QBUF, &buf))
                            printf ("VIDIOC_QBUF failed\n");
                        }

                        enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

                        if (-1 == ioctl (fd, VIDIOC_STREAMON, &type))
                                printf ("VIDIOC_STREAMON failed\n");
                        disp_init(preview_width,preview_height,&layerinit);
                        disp_start();
                        disp_on();

                        while(!quit)
                        {
                            read(key_fd, &data, sizeof(data));
                            if( (data.type == EV_KEY)&& (data.code == KEY_ESC)&&(data.value == 1) ){
                                quit = 1;
                            }

                            get_system(&new_system, &ch0_signal, &ch1_signal, &signal_det);

                            if ((ch0_signal != ch0_enable) || (ch1_signal != ch1_enable)) {
                            ch0_enable = ch0_signal;
                             ch1_enable = ch1_signal;
                            printf ("no signal detect==================\n");
                            if (-1 == ioctl (fd, VIDIOC_STREAMOFF, &type))
                                printf ("VIDIOC_STREAMOFF failed\n");
                            for (i = 0; i < n_buffers; ++i) {
                            if (-1 == munmap (buffers[i].start, buffers[i].length)) {
                                 printf ("munmap error");
                            }
                          }

                         layer_close();
                         disp_stop();
                         disp_exit();
                         close (fd);
                         goto reset;
                    }

                    if (!signal_det) {
                            printf ("no signal detect==================\n");
                    if (-1 == ioctl (fd, VIDIOC_STREAMOFF, &type))
                            printf ("VIDIOC_STREAMOFF failed\n");
                    for (i = 0; i < n_buffers; ++i) {
                            if (-1 == munmap (buffers[i].start, buffers[i].length)) {
                                    printf ("munmap error");
                            }
                    }

                    layer_close();

                    disp_stop();

                    disp_exit();

                    close (fd);


                    goto reset;
                    }

                    if(new_system!=tvd_system)
                    {
                            tvd_system = new_system;
                            if (-1 == ioctl (fd, VIDIOC_STREAMOFF, &type))
                                printf ("VIDIOC_STREAMOFF failed\n");
                             for (i = 0; i < n_buffers; ++i) {
                                if (-1 == munmap (buffers[i].start, buffers[i].length)) {
                                    printf ("munmap error");
                                 }
                              }

                            layer_close();
                            disp_stop();
                            disp_exit();
                            close (fd);
                            goto reset;
                    }

                    for (;;)
                    {
                        fd_set fds;
                        struct timeval tv;
                        int r;
                        FD_ZERO (&fds);
                        FD_SET (fd, &fds);
                         /* Timeout. */
                         tv.tv_sec = 2;
                         tv.tv_usec = 0;
                         r = select (fd + 1, &fds, NULL, NULL, &tv);
                        if (-1 == r) {
                            if (EINTR == errno)
                                continue;
                            printf ("select err\n");
                        }

                        if (0 == r) {
                             fprintf (stderr, "select timeout\n");
                            exit (EXIT_FAILURE);
                        }

                        if (read_frame ())
                        {
                            if(first_frame)
                            {
                                layer_open();
                                first_frame=0;
                            }
                            break;
                        }

                       }
                    }
                    if (-1 == ioctl (fd, VIDIOC_STREAMOFF, &type))
                         printf ("VIDIOC_STREAMOFF failed\n");

                    for (i = 0; i < n_buffers; ++i) {
                        if (-1 == munmap (buffers[i].start, buffers[i].length)) {
                             printf ("munmap error");
                        }
                    }

                    disp_stop();
                    disp_exit();
                    close (fd);
                    printf("TVD bye!\n");
                    return 0;
            }
