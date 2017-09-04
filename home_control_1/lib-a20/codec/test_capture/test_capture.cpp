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

#define V4L2_MODE_HIGHQUALITY   0x0001
#define V4L2_MODE_VIDEO         0x0002
#define V4L2_MODE_IMAGE         0x0003
#define V4L2_MODE_PREVIEW       0x0004

#define NB_BUFFER   4

static int cameraFd = -1;
static int frameWidth = 0;
static int frameHeight = 0;
static int bufferCnt = NB_BUFFER;
static int videoFormat;

typedef struct v4l2_mem_map_t {
    void *mem[NB_BUFFER];
    int length;
} v4l2_mem_map_t;
static v4l2_mem_map_t mapMem;

static int openCameraDev()
{
    int ret = -1;
    struct v4l2_input inp;
    struct v4l2_capability cap;

    cameraFd = open("/dev/video0", O_RDWR | O_NONBLOCK, 0);
    if (cameraFd == -1)
    {
        printf("Error opening /dev/video0: %s\n", strerror(errno));
        return -1;
    }

    ret = ioctl(cameraFd, VIDIOC_QUERYCAP, &cap);
    if (ret < 0)
    {
        printf("Error opening device: unable to query device.\n");
        goto END_ERROR;
    }

    if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0)
    {
        printf("Error opening device: video capture not supported.\n");
        goto END_ERROR;
    }

    if ((cap.capabilities & V4L2_CAP_STREAMING) == 0)
    {
        printf("Capture device does not support streaming i/o\n");
        goto END_ERROR;
    }

    inp.index = 0;
    if (ioctl(cameraFd, VIDIOC_S_INPUT, &inp) == -1)
    {
        printf("VIDIOC_S_INPUT error!\n");
    }

    return 0;
END_ERROR:
    if (cameraFd != -1)
    {
        close(cameraFd);
        cameraFd = -1;
    }

    return -1;
}

static void closeCameraDev()
{
    if (cameraFd != -1)
    {
        close(cameraFd);
        cameraFd = -1;
    }
}

static int v4l2SetVideoParams(int width, int height, unsigned int pix_fmt)
{
    int ret = -1;
    struct v4l2_format format;

    printf("w: %d, h: %d, pfmt: %d\n", width, height, pix_fmt);

    memset(&format, 0, sizeof(format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = width;
    format.fmt.pix.height = height;
    format.fmt.pix.pixelformat = pix_fmt;
    format.fmt.pix.field = V4L2_FIELD_NONE;

    ret = ioctl(cameraFd, VIDIOC_S_FMT, &format);
    if (ret < 0)
    {
        printf("VIDIOC_S_FMT failed: %s\n", strerror(errno));
        return ret;
    }

    frameWidth = format.fmt.pix.width;
    frameHeight = format.fmt.pix.height;

    printf("camera params: w: %d, h: %d, pfmt: %d, pfield: %d\n",
        frameWidth, frameHeight, pix_fmt, V4L2_FIELD_NONE);

    return 0;
}

static int v4l2ReqBufs(int *buf_cnt)
{
    int ret = -1;
    struct v4l2_requestbuffers rb;

    printf("TO VIDIOC_REQBUFS count: %d\n", *buf_cnt);

    memset(&rb, 0, sizeof(rb));

    rb.count = *buf_cnt;
    rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    rb.memory = V4L2_MEMORY_MMAP;


    ret = ioctl(cameraFd, VIDIOC_REQBUFS, &rb);
    if (ret < 0)
    {
        printf("Init: VIDIOC_REQBUFS failed: %s\n", strerror(errno));
        return ret;
    }
    *buf_cnt = rb.count;
    printf("VIDIOC_REQBUFS count: %d\n", *buf_cnt);

    return 0;
}

static int v4l2QueryBuf()
{
    int ret = -1;
    struct v4l2_buffer buf;

    for (int i = 0; i < bufferCnt; i++)
    {
        memset(&buf, 0, sizeof(struct v4l2_buffer));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        ret = ioctl(cameraFd, VIDIOC_QUERYBUF, &buf);
        if (ret < 0)
        {
            printf("Unable to query buffer (%s)\n", strerror(errno));
            return ret;
        }

        mapMem.mem[i] = mmap(0, buf.length,
                            PROT_READ | PROT_WRITE,
                            MAP_SHARED,
                            cameraFd,
                            buf.m.offset);
        mapMem.length = buf.length;
        printf("index: %d, mem: %x, len: %x, offset: %x\n",
            i, (int)mapMem.mem[i], buf.length, buf.m.offset);

        if (mapMem.mem[i] == MAP_FAILED)
        {
            printf("Unable to map buffer (%s)\n", strerror(errno));
            return -1;
        }

        ret = ioctl(cameraFd, VIDIOC_QBUF, &buf);
        if (ret < 0)
        {
            printf("VIDIOC_QBUF failed\n");
            return ret;
        }
    }

    return 0;
}

static int v4l2StartStreaming()
{
    int ret = -1;
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    ret = ioctl(cameraFd, VIDIOC_STREAMON, &type);
    if (ret < 0)
    {
        printf("StartStreaming: Unable to start capture: %s\n", strerror(errno));
        return ret;
    }

    return 0;
}

static int v4l2StopStreaming()
{
    int ret = -1;
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    ret = ioctl(cameraFd, VIDIOC_STREAMOFF, &type);
    if (ret < 0)
    {
        printf("Unable to stop capture: %s\n", strerror(errno));
        return ret;
    }

    printf("v4l2StopStreaming OK\n");

    return 0;
}

static int v4l2UnmapBuf()
{
    int ret = -1;

    for (int i = 0; i < bufferCnt; i++)
    {
        ret = munmap(mapMem.mem[i], mapMem.length);
        if (ret < 0)
        {
            printf("v4l2CloseBuf Unmap failed\n");
            return ret;
        }

        mapMem.mem[i] = NULL;
    }

    return 0;
}

static void releasePreviewFrame(int index)
{
    int ret = -1;
    struct v4l2_buffer buf;

    //if (v4l2buf[index].refCnt > 0 && --v4l2buf[index].refCnt == 0)
    {
        memset(&buf, 0, sizeof(v4l2_buffer));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = index;

        ret = ioctl(cameraFd, VIDIOC_QBUF, &buf);
        if (ret != 0)
        {
            printf("releasePreviewFrame: VIDIOC_QBUF failed: index = %d, ret = %d, %s\n",
                buf.index, ret, strerror(errno));
        }
    }
}

static int getPreviewFrame(v4l2_buffer *buf)
{
    int ret = -1;

    buf->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf->memory = V4L2_MEMORY_MMAP;

    ret = ioctl(cameraFd, VIDIOC_DQBUF, buf);
    if (ret < 0)
    {
        printf("GetPreviewFrame: VIDIOC_DQBUF Failed, %s\n", strerror(errno));
        return __LINE__;
    }

    return 0;
}

static int tryFmtSize(int *width, int *height)
{
    int ret = -1;
    struct v4l2_format fmt;

    printf("TryFmtSize: w: %d, h: %d\n", *width, *height);

    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = *width;
    fmt.fmt.pix.height = *height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    ret = ioctl(cameraFd, VIDIOC_TRY_FMT, &fmt);
    if (ret < 0)
    {
        printf("VIDIOC_TRY_FMT Failed: %s\n", strerror(errno));
        return ret;
    }

    *width = fmt.fmt.pix.width;
    *height = fmt.fmt.pix.height;

    return 0;
}

static int v4l2WaitCameraReady()
{
    fd_set fds;
    struct timeval tv;
    int r;

    FD_ZERO(&fds);
    FD_SET(cameraFd, &fds);

    tv.tv_sec = 2;
    tv.tv_usec = 0;

    r = select(cameraFd + 1, &fds, NULL, NULL, &tv);
    if (r == -1)
    {
        printf("select err, %s\n", strerror(errno));
        return -1;
    }
    else if (r == 0)
    {
        printf("select timeout\n");
        return -1;
    }

    return 0;
}

int main()
{
    int ret;
	FILE *fp;
	
    fp = fopen("/tmp/capture.yuv", "wb");
	if (!fp)
	{
		printf("Open capture.yuv failed\n");
		return -1;
	}
	
    ret = openCameraDev();

    v4l2SetVideoParams(1280,720, V4L2_PIX_FMT_YUV420);

    int buf_cnt = NB_BUFFER;
    v4l2ReqBufs(&buf_cnt);
    bufferCnt = buf_cnt;

    v4l2QueryBuf();
    v4l2StartStreaming();

    int i = 0;
    while (i<250)
    {
        printf("frame:%d\n", i++);
    	
        ret = v4l2WaitCameraReady();
        if (ret != 0) continue;

        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(v4l2_buffer));
        ret = getPreviewFrame(&buf);

        char *ptr = (char *)mapMem.mem[buf.index];
        fwrite(ptr, 1, frameWidth * frameHeight * 3/2, fp);

        releasePreviewFrame(buf.index);
    }

    fclose(fp);

    v4l2StopStreaming();

    v4l2UnmapBuf();

    closeCameraDev();

    return 0;
}

