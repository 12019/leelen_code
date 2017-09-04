#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <linux/videodev2.h>
#include "s3c-tvenc.h"
#include "s3c-tvscaler.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
//#include <linux/fb.h>

//#define ENABLE_TVOUT	1

//#if ENABLE_TVOUT == 1
int g_enable_tvout = 1;

#define LCD_WIDTH		800
#define LCD_HEIGHT		480

#define CIF_WIDTH		352
#define CIF_HEIGHT		288

#define DISP_WIDTH		352
#define DISP_HEIGHT		212

extern unsigned int g_tvout_virt_addr;
extern unsigned int g_tvout_phy_addr;
extern int g_is_tvout_start;

s3c_tvenc_mem_alloc_t g_tvout_alloc_info;

int g_tvout_fd = -1;

int tvout_start()
{
	if (!g_enable_tvout)
	{
		return 0;
	}

	g_is_tvout_start = 1;

	return 0;
}

int tvout_stop()
{
	if (!g_enable_tvout)
	{
		return 0;
	}

	g_is_tvout_start = 0;

	memset((void *)g_tvout_virt_addr, 0x00, DISP_WIDTH * DISP_HEIGHT);
	memset((void *)(g_tvout_virt_addr + DISP_WIDTH * DISP_HEIGHT), 0x80, DISP_WIDTH * DISP_HEIGHT / 2);

	return 0;
}

int	tvout_open()
{
	if (!g_enable_tvout)
	{
		return 0;
	}

	struct v4l2_capability cap;
	struct v4l2_control ctrl;
	struct v4l2_input source;
	struct v4l2_output sink;
	struct v4l2_format source_fmt;
	struct v4l2_standard tvout_std;
	int ret;
	int tvout_mem_offset;
	//struct fb_fix_screeninfo finfo;

	//int g_fb_fd  = open("/dev/fb0", O_RDWR);

	g_tvout_fd = open("/dev/video14", O_RDWR);
	//printf("lg g_tvout_fd:%d\n", g_tvout_fd);

	if (g_tvout_fd < 0) {
		printf("TVOUT device file open error = %d\n", g_tvout_fd);
		return -1;
	}

	ret = ioctl(g_tvout_fd, VIDIOC_S_TVOUT_OFF, NULL);

	g_tvout_alloc_info.size = CIF_WIDTH * CIF_HEIGHT * 3/2 + 7;
	if (ioctl(g_tvout_fd, S3C_TVOUT_ALLOC_KMEM, &g_tvout_alloc_info) == -1)
	{
		printf("ioctl S3C_PP_ALLOC_KMEM failed\n");
		goto err;
	}

	tvout_mem_offset = g_tvout_alloc_info.phy_addr & 7;
	if (tvout_mem_offset > 0)
	{
		tvout_mem_offset = 8 - tvout_mem_offset;
	}
	g_tvout_virt_addr = g_tvout_alloc_info.vir_addr + tvout_mem_offset;
	g_tvout_phy_addr = g_tvout_alloc_info.phy_addr + tvout_mem_offset;

	//printf("lg virt_addr:%08x\n", g_tvout_virt_addr);
	//printf("lg phy_addr:%08x\n", g_tvout_phy_addr);

	g_is_tvout_start = 0;
	memset((void *)g_tvout_virt_addr, 0x00, DISP_WIDTH * DISP_HEIGHT);
	memset((void *)(g_tvout_virt_addr + DISP_WIDTH * DISP_HEIGHT), 0x80, DISP_WIDTH * DISP_HEIGHT / 2);

	/* Get capability */
	ret = ioctl(g_tvout_fd, VIDIOC_QUERYCAP, &cap);
	if(ret < 0) {
		printf("V4L2 APPL : ioctl(VIDIOC_QUERYCAP) failed\n");
		goto err;
	}

	/* Check the type of device driver */
	if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		printf("V4L2 APPL : Check capability !!!\n");
		goto err;
	}

	/* Set source path : index 0(LCD FIFO-IN), 1(MSDMA)*/
	source.index = 1;
	while(1) {
		ret = ioctl(g_tvout_fd, VIDIOC_ENUMINPUT, &source);
		if(ret < 0) {
			printf("V4L2 APPL : ioctl(VIDIOC_ENUMINPUT) failed\n");
			goto err;
		}

		/* Test channel.type */
		if(source.type & V4L2_INPUT_TYPE_MSDMA) {
			printf("V4L2 APPL : DMA INPUT \n");
			break;
		}
		source.index++;
	}

	/* Setting for source channel 1 which is channel of DMA */
	ret = ioctl(g_tvout_fd, VIDIOC_S_INPUT, &source);
	if(ret < 0) {
		printf("V4L2 APPL : ioctl(VIDIOC_S_INPUT) failed\n");
		goto err;
	}

	/* Set sink path : index 0(TV-OUT/FIFO-OUT), 1(MSDMA)*/
	sink.index = 0;
	while(1) {
		ret = ioctl(g_tvout_fd, VIDIOC_ENUMOUTPUT, &sink);
		if(ret < 0) {
			printf("V4L2 APPL : ioctl(VIDIOC_ENUMOUTPUT) failed\n");
			goto err;
		}

		/* Test channel.type */
		if(sink.type & V4L2_OUTPUT_TYPE_ANALOG) {
			printf("V4L2 APPL : TV OUT \n");
			break;
		}
		sink.index++;
	}

	/* Setting for sink channel 0 which is channel of TV OUT */
	ret = ioctl(g_tvout_fd, VIDIOC_S_OUTPUT, &sink);
	if(ret < 0) {
		printf("V4L2 APPL : ioctl(VIDIOC_S_OUTPUT) failed\n");
		goto err;
	}

	/* Set source format */
	source_fmt.fmt.pix.width = DISP_WIDTH; //CIF_WIDTH; //LCD_WIDTH;//stream_info.width;
	source_fmt.fmt.pix.height = DISP_HEIGHT; //CIF_WIDTH * LCD_HEIGHT / LCD_WIDTH; //stream_info.height;
	source_fmt.fmt.pix.pixelformat = /*V4L2_PIX_FMT_RGB565;/*/ V4L2_PIX_FMT_YUV420;

	ret = ioctl(g_tvout_fd, VIDIOC_S_FMT, &source_fmt);
	if(ret < 0) {
		printf("V4L2 APPL : ioctl(VIDIOC_S_FMT) failed\n");
		goto err;
	}

	/* Set TV standard format */
	tvout_std.index = 0;

	ret = ioctl(g_tvout_fd, VIDIOC_ENUMSTD, &tvout_std);
	if(ret < 0) {
		printf("V4L2 APPL : ioctl(VIDIOC_ENUMSTD) failed\n");
		goto err;
	}

	tvout_std.id = V4L2_STD_NTSC_M;
	ret = ioctl(g_tvout_fd, VIDIOC_S_STD, &tvout_std.id);
	if(ret < 0) {
		printf("V4L2 APPL : ioctl(VIDIOC_S_STD) failed\n");
		goto err;
	}

	/* Set specific ioctl - TVOUT connect type */
	ctrl.id = V4L2_CID_CONNECT_TYPE;
	ctrl.value = 0;	// COMPOSITE

	ret = ioctl(g_tvout_fd, VIDIOC_S_CTRL, &ctrl);
	if(ret < 0) {
		printf("V4L2 APPL : ioctl(VIDIOC_S_CTRL) failed\n");
		goto err;
	}

	//ioctl(g_fb_fd, FBIOGET_FSCREENINFO, &finfo);
	//unsigned int offset = (CIF_HEIGHT - CIF_WIDTH * LCD_HEIGHT / LCD_WIDTH) / 2;
	ctrl.id = V4L2_CID_MPEG_STREAM_PID_VIDEO;
	ctrl.value = /*finfo.smem_start;/*/g_tvout_phy_addr;// COMPOSITE
	//memset((void *)g_tvout_phy_addr, 0x80, 352 * 288 * 3 / 2);

	ret = ioctl(g_tvout_fd, VIDIOC_S_CTRL, &ctrl);
	if(ret < 0) {
		printf("V4L2 APPL : ioctl(VIDIOC_S_CTRL) failed\n");
		goto err;
	}

	ret = ioctl(g_tvout_fd, VIDIOC_S_TVOUT_ON, NULL);	// Just once executed because of the free-run mode
	if(ret < 0) {
		printf("V4L2 APPL : ioctl(VIDIOC_S_TVOUT_ON) failed\n");
		goto err;
	}

	return 0;

err:
	ret = close(g_tvout_fd);

	return -1;
}

int tvout_close()
{
	if (!g_enable_tvout)
	{
		return 0;
	}

	if (g_tvout_fd == -1)
	{
		return 0;
	}

	g_is_tvout_start = 0;

	/* Stop TV_OUT */
	int ret;

	ret = ioctl(g_tvout_fd, VIDIOC_S_TVOUT_OFF, NULL);
	if(ret < 0) {
		printf("V4L2 APPL : ioctl(VIDIOC_S_TVOUT_OFF) failed\n");
		goto err;
	}

	if (ioctl(g_tvout_fd, S3C_TVOUT_FREE_KMEM, &g_tvout_alloc_info) == -1)
	{
		printf("ioctl S3C_PP_FREE_KMEM failed\n");
		goto err;
	}

	g_tvout_virt_addr = 0;
	g_tvout_phy_addr = 0;

	ret = close(g_tvout_fd);
	g_tvout_fd = -1;

	if (ret < 0)
	{
		goto err;
	}

	return 0;

err:
	return -1;
}

/*#else
int tvout_open()
{
	return 0;
}

int tvout_close()
{
	return 0;
}

int	tvout_start()
{
	return 0;
}

int tvout_stop()
{
	return 0;
}
*/
//#endif
