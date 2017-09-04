#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include "render.h"

extern "C"
{
#include "cedarv_osal_linux.h"
}
#include "libcedarv.h"

int main()
{
	int ret = -1;
	cedarv_picture_t pic;
	int cnt;
	
	ret = cedarx_hardware_init(0);
	if (ret < 0)
	{
		printf("cedarx_hardware_init failed\n");
		return -1;
	}

	render_init();
	
	char *img, *img_u;
    img = (char *)cedar_sys_phymalloc_map(352 * 288, 1024);
    img_u = (char *)cedar_sys_phymalloc_map(352 * 288/2, 1024);
    unsigned int phy_img = cedarv_address_vir2phy((void*)img);
    unsigned int phy_img_u = cedarv_address_vir2phy((void*)img_u);

	FILE *fp = fopen("/tmp/foreman.yuv", "rb");
	if (!fp)
	{
		printf("open yuv failed\n");
		goto finish;
	}

    printf("planner yuv420\n");
	cnt = 0;
	while (1)
	{
		memset(&pic, 0, sizeof(pic));
		pic.width = 352;
		pic.height = 288;
		pic.display_width = 352;
		pic.display_height = 288;
		pic.frame_rate = 23;
		pic.aspect_ratio = 1331;
		pic.is_progressive = 1;
		pic.top_field_first = 1;
		pic.repeat_bottom_field = 1;
        pic.pixel_format == CEDARV_PIXEL_FORMAT_PLANNER_YUV420;//CEDARV_PIXEL_FORMAT_YUV420;
		pic.anaglath_transform_mode = CEDARV_ANAGLAGH_NONE;
		pic.size_y = 352 * 288;
		pic.size_u = 352 * 288/4;
		pic.size_v = 352 * 288/4;

		pic.y = (u8 *)phy_img;
		pic.u = (u8 *)phy_img_u;
		pic.v = (u8 *)phy_img_u + 352 * 288/4;
	
		int size = fread(img, 1, 352 * 288, fp);
		if (size < 352 * 288) break;
		char buf[352 * 288/2];
		size = fread(buf, 1, 352 * 288/2, fp);
		if (size < 352 * 288/2) break;
		int j;
		for (j = 0; j < 352 * 288/4; j++)
		{
			img_u[j * 2] = buf[j];
			img_u[j * 2 + 1] = buf[j + 352 * 288/4];
		}
		//memset(img_u, 0x80, 352 * 288/2);

		pic.id = cnt;		
		pic.pts = cnt * 33333;
		render_render((void *)&pic, cnt++);
		
		usleep(33333);
	}

	fclose(fp);
	
	//sleep(10);
	
finish:	
	render_exit();
	
	cedar_sys_phyfree_map(img);
	cedar_sys_phyfree_map(img_u);

	cedarx_hardware_exit(0);

	return 0;
}
