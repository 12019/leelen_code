#include "sendimgthread.h"
#include "maindialog.h"
#include "voipmaindialog.h"
#include "lpconfig.h"
#include "lpcore.h"

extern LinphoneCore *lphone;

extern unsigned char *g_jpg_image_buf;
extern int g_jpg_image_ready;
extern char g_strLphoneName[3][50];
extern char g_strNativeName[50];

extern ViewCam *g_pViewCam;
extern MainDialog *g_pMainDlg;
/************************************************************
描述：YUV420图像向RGB565图像格式的转换
参数：src - YUV420图像缓存区，Y、U、V数据连续存储
     dst - RGB565图像缓存区
     width - 图像宽
     height - 图像高
返回：无
************************************************************/
static void yuv420_to_rgb565(unsigned char *src, unsigned short *dst, int width, int height)
{
    int line, col, linewidth;
    int y, u, v, yy, vr, ug, vg, ub;
    int r, g, b;
    const unsigned char *py, *pu, *pv;

    linewidth = width >> 1;
    py = src;
    pu = py + (width * height);
    pv = pu + (width * height) / 4;

    y = *py++;
    yy = y << 8;
    u = *pu - 128;
    ug =   88 * u;
    ub = 454 * u;
    v = *pv - 128;
    vg = 183 * v;
    vr = 359 * v;

    for (line = 0; line < height; line++) {
        for (col = 0; col < width; col++) {
            r = (yy +      vr) >> 8;
            g = (yy - ug - vg) >> 8;
            b = (yy + ub     ) >> 8;

            if (r < 0)   r = 0;
            if (r > 255) r = 255;
            if (g < 0)   g = 0;
            if (g > 255) g = 255;
            if (b < 0)   b = 0;
            if (b > 255) b = 255;
            *dst++ = (((unsigned short)r>>3)<<11) | (((unsigned short)g>>2)<<5) | (((unsigned short)b>>3)<<0);

            y = *py++;
            yy = y << 8;
            if (col & 1) {
                pu++;
                pv++;

                u = *pu - 128;
                ug =   88 * u;
                ub = 454 * u;
                v = *pv - 128;
                vg = 183 * v;
                vr = 359 * v;
            }
        } /* ..for col */
        if ((line & 1) == 0) { // even line: rewind
            pu -= linewidth;
            pv -= linewidth;
        }
    } /* ..for line */
}

SendImgThread::SendImgThread(QObject *parent) :
    QThread(parent)
{
    m_pTcpSock = NULL;
    m_bRun = false;

}

SendImgThread::~SendImgThread()
{

}

int SendImgThread::ConnectToManCenter()
{

    char strIP[200] = "";//192.168.1.139";
    GetPeerIPThread *pGetPeerIPThread = ((VoipMainDialog *)(parent()))->m_pGetPeerIPThread;

    if (m_pTcpSock)
    {
        m_pTcpSock->close();
        delete m_pTcpSock;
    }

    m_pTcpSock = new QTcpSocket;

    printf("g_strL:%s\n",g_strLphoneName[2]);
    if (pGetPeerIPThread->GetPeerIP(g_strLphoneName[2], strIP) != 0)
    {
        printf("david\n");
        return -1;
    }


    char *ptr;
    int iModalType = BASIC_MODAL_TYPE;

    ptr = strstr(strIP, "?");
    if (ptr)
    {
        iModalType = atoi(ptr + strlen("?"));
        *ptr = '\0';
    }

    if (iModalType != MANCENTER_MODAL_TYPE) return -1;

    ptr = strstr(strIP, "->");
    if (ptr)
    {
        ptr[0] = '\0';
    }

    m_pTcpSock->connectToHost(strIP, 17701);

    if (!m_pTcpSock->waitForConnected(500))
    {
        delete m_pTcpSock;
        m_pTcpSock = NULL;

        return -1;
    }

    return 0;
}

void SendImgThread::run()
{
    int image_width = 352,image_height = 288;

    if(lphone->video_resolution == 0)
    {
        image_width = 352;
        image_height = 288;
    }
    else if(lphone->video_resolution == 1)
    {
        image_width = USED_VGA_VIDEO_W;
        image_height = USED_VGA_VIDEO_H;
    }
    else if(lphone->video_resolution == 2)
    {
        image_width = 720;
        image_height = 288;
    }

    char *buf = (char *)malloc(image_width * image_height  * 2);

    int jpgCnt = 0;
    QString strFileName;
    bool ret;

    m_bRun = true;

    if (m_pTcpSock && (m_pTcpSock->state() != QTcpSocket::ConnectedState))
    {
        delete m_pTcpSock;
        m_pTcpSock = NULL;
    }

    if (!m_pTcpSock)
    {
        m_bRun = false;
	free(buf);
        return;
    }

    unsigned char tag = 0x81;

    if (m_pTcpSock) m_pTcpSock->write((char *)&tag, sizeof(unsigned char));

    while (m_bRun && (g_jpg_image_ready != 2))
    {
        usleep(200000);
    }

    g_jpg_image_ready = 0;


    while (m_bRun)
    {

        if (g_jpg_image_ready == 1)
        {
            yuv420_to_rgb565((unsigned char *)g_jpg_image_buf, (unsigned short *)buf, image_width,image_height);
            strFileName = "/mnt/disk/alert_pic/" + QString::number(0) + ".jpg";
            QImage image((unsigned char *)buf,image_width,image_height, QImage::Format_RGB16);

            if(lphone->video_resolution == 2)
            {
                image = image.scaled(720,576);

                image_height = 576;
            }

            ret = image.save(strFileName, "jpg");

            FILE *fp;
            int len;
            fp = fopen(strFileName.toLocal8Bit().data(), "rb");
            len = fread(buf, 1, image_width * image_height * 2, fp);
            fclose(fp);
            printf("send jpg:%d len:%d\n", jpgCnt, len);

            QFile::remove(strFileName);
            int ret;


            if (m_pTcpSock)
            {
                if (m_pTcpSock->state() == QTcpSocket::ConnectedState)
                {
                    ret = m_pTcpSock->write((char *)&len, 4);
                    ret = m_pTcpSock->write(buf, len);

                    m_pTcpSock->flush();
                }
                else
                {
                    delete m_pTcpSock;
                    m_pTcpSock = NULL;
                }
            }

            if (m_pTcpSock == NULL)
            {
                break;
            }

            g_jpg_image_ready = 0;

            break;

        }

        usleep(200000);
    }

    free(buf);

    g_jpg_image_ready = 3;//bug


    if (m_pTcpSock)
    {
        m_pTcpSock->close();
        delete m_pTcpSock;
        m_pTcpSock = NULL;
    }

    if (g_pViewCam)
    {
        ViewCam *tmp_pViewCam = g_pViewCam;
        g_pViewCam = (ViewCam *)-1;
        viewCam_stop(tmp_pViewCam);
        g_pViewCam = NULL;
    }

    m_bRun = false;
}
