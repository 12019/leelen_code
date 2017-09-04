#include "jpgsavetestthread.h"
#include <QImage>
#include <stdio.h>

extern unsigned char *g_jpg_tvd_image_buf;
extern int g_jpg_tvd_image_ready;

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

JpgSaveTestThread::JpgSaveTestThread(QObject *parent) :
    QThread(parent)
{
	m_bRun = false;
}

void JpgSaveTestThread::run()
{
    printf("JpgSaveTestThread runn");
	int jpgCnt = 0;
	short *buf;
	int ret;

    buf = new short[720 * 576];

    g_jpg_tvd_image_ready = 0;

	m_bRun = true;
	while (m_bRun)
	{
        if (g_jpg_tvd_image_ready == 1)
		{
			printf("jpeg:%d\n", jpgCnt);

            yuv420_to_rgb565((unsigned char *)g_jpg_tvd_image_buf, (unsigned short *)buf, 352, 288);

			QString strFileName = "/mnt/disk/alert_pic/" + QString::number(jpgCnt) + ".jpg";
            QImage image((unsigned char *)buf, 720, 576, QImage::Format_RGB16);
			ret = image.save(strFileName, "jpg");
            g_jpg_tvd_image_ready = 0;

			if (++jpgCnt >= 6)
			{
				break;
			}
		}

		usleep(200000);
	}

	delete [] buf;
}
