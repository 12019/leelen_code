/*******************************************************************************
--                                                                            --
--                    CedarX Multimedia Framework                             --
--                                                                            --
--          the Multimedia Framework for Linux/Android System                 --
--                                                                            --
--       This software is confidential and proprietary and may be used        --
--        only as expressly authorized by a licensing agreement from          --
--                         Softwinner Products.                               --
--                                                                            --
--                   (C) COPYRIGHT 2011 SOFTWINNER PRODUCTS                   --
--                            ALL RIGHTS RESERVED                             --
--                                                                            --
--                 The entire notice above must be reproduced                 --
--                  on all copies and should not be removed.                  --
--                                                                            --
*******************************************************************************/
#include "render.h"

VIDEO_RENDER_CONTEXT_TYPE *hnd_video_render = NULL;

unsigned long args[4];

static int config_de_parameter(unsigned int width, unsigned int height, __disp_pixel_fmt_t format, 
	int scn_win_x, int scn_win_y, int scn_win_width, int scn_win_height) {
	__disp_layer_info_t tmpLayerAttr;
	int ret;
	args[0] = 0;
	args[1] = hnd_video_render->de_layer_hdl;
	args[2] = (unsigned long)(&tmpLayerAttr);
	ioctl(hnd_video_render->de_fd, DISP_CMD_LAYER_GET_PARA,args);

	//set color space
    if (height < 720) {
		tmpLayerAttr.fb.cs_mode = DISP_BT601;
	} else {
		tmpLayerAttr.fb.cs_mode = DISP_BT709;
	}

	if(format == DISP_FORMAT_ARGB8888){
		tmpLayerAttr.fb.mode 	= DISP_MOD_NON_MB_PLANAR;
		tmpLayerAttr.fb.format 	= format; //
		tmpLayerAttr.fb.br_swap = 0;
		tmpLayerAttr.fb.cs_mode	= DISP_YCC;
		tmpLayerAttr.fb.seq 	= DISP_SEQ_P3210;
	}
	else
	{
		tmpLayerAttr.fb.mode 	= DISP_MOD_MB_UV_COMBINED;
		tmpLayerAttr.fb.format 	= format; //DISP_FORMAT_YUV420;
		tmpLayerAttr.fb.br_swap = 0;
		tmpLayerAttr.fb.seq 	= DISP_SEQ_UVUV;
	}

	tmpLayerAttr.fb.mode 	= DISP_MOD_NON_MB_UV_COMBINED;
	tmpLayerAttr.fb.format 	= DISP_FORMAT_YUV420;
	tmpLayerAttr.fb.br_swap = 0;
	tmpLayerAttr.fb.cs_mode	= DISP_BT601;
	tmpLayerAttr.fb.seq 	= DISP_SEQ_P3210;
	
	tmpLayerAttr.fb.addr[0] = 0;
	tmpLayerAttr.fb.addr[1] = 0;

	tmpLayerAttr.fb.size.width 	= width;
	tmpLayerAttr.fb.size.height = height;

	//set video layer attribute
	tmpLayerAttr.mode = DISP_LAYER_WORK_MODE_SCALER;
	tmpLayerAttr.alpha_en = 1;
	tmpLayerAttr.alpha_val = 0xff;
#ifdef CONFIG_DFBCEDAR
	tmpLayerAttr.pipe = 1;
#else
	tmpLayerAttr.pipe = 0;
#endif
	//screen window information
	tmpLayerAttr.scn_win.x = scn_win_x; //50;//0;
	tmpLayerAttr.scn_win.y = scn_win_y; //50;//0;
    tmpLayerAttr.scn_win.width  = scn_win_width;
    tmpLayerAttr.scn_win.height = scn_win_height;
	tmpLayerAttr.prio           = 0xff;
	//frame buffer pst and size information
    tmpLayerAttr.src_win.x = 0;
    tmpLayerAttr.src_win.y = 0;
    tmpLayerAttr.src_win.width = width;
    tmpLayerAttr.src_win.height = height;
    //printf("width %d, height %d\n",tmpLayerAttr.src_win.width ,tmpLayerAttr.src_win.height );
	hnd_video_render->src_frm_rect.x = tmpLayerAttr.src_win.x;
	hnd_video_render->src_frm_rect.y = tmpLayerAttr.src_win.y;
	hnd_video_render->src_frm_rect.width = tmpLayerAttr.src_win.width;
	hnd_video_render->src_frm_rect.height = tmpLayerAttr.src_win.height;

    //printf("set video layer param\n");
	args[0] = 0;
	args[1] = hnd_video_render->de_layer_hdl;
	args[2] = (unsigned long) (&tmpLayerAttr);
	args[3] = 0;
	ioctl(hnd_video_render->de_fd, DISP_CMD_LAYER_SET_PARA, args);
	
	args[0]							= 0;
	args[1]                 		= hnd_video_render->de_layer_hdl;
	args[2]                 		= 0;
	args[3]                 		= 0;
	ret                     		= ioctl(hnd_video_render->de_fd, DISP_CMD_LAYER_TOP,args);
	if(ret != 0)
	{		//open display layer failed, need send play end command, and exit
        //printf("Open video display layer failed!\n");
		return NULL;
	}	
	return 0;
}

int render_init() 
{
	int ret;
	printf("render_init\n");
	
	if (hnd_video_render != (VIDEO_RENDER_CONTEXT_TYPE *) 0) {
		printf("Cedar:vply: video play back has been opended already!\n");
		return -1;
	}

	hnd_video_render = (VIDEO_RENDER_CONTEXT_TYPE *) malloc(sizeof(VIDEO_RENDER_CONTEXT_TYPE ));
	if (hnd_video_render == (VIDEO_RENDER_CONTEXT_TYPE *) 0) {
		printf("Cedar:vply: malloc hnd_video_render error!\n");
		return -1;
	}
	memset(hnd_video_render, 0, sizeof(VIDEO_RENDER_CONTEXT_TYPE ));

	hnd_video_render->first_frame_flag = 1;

	hnd_video_render->de_fd = open("/dev/disp", O_RDWR);
	if (hnd_video_render->de_fd < 0) {
		printf("Open display driver failed!\n");
		return -1;
	}

    args[0] = 0;
    args[1] = 101;
    args[2] = 0;
    args[3] = 0;
    ioctl(hnd_video_render->de_fd,DISP_CMD_LAYER_RELEASE,args);


	args[0] = 0;
	args[1] = DISP_LAYER_WORK_MODE_SCALER;
	args[2] = 0;
	args[3] = 0;
	hnd_video_render->de_layer_hdl = ioctl(hnd_video_render->de_fd, DISP_CMD_LAYER_REQUEST,	args);
	if (hnd_video_render->de_layer_hdl == 0) {
		printf("Open display layer failed! de fd:%d \n", hnd_video_render->de_fd);
		return -1;
	}


	//set video window information to default value, full screen
	args[0] = 0;
	args[1] = hnd_video_render->de_layer_hdl;
	args[2] = 0;
	args[3] = 0;

	return 0;
}

void render_exit(void) {
	if (hnd_video_render == NULL) {
		printf("video playback has been closed already!\n");
		return;
	}
	int			ret;

	//close displayer driver context
	if (hnd_video_render->de_fd) {
		args[0] = 0;
		args[1] = hnd_video_render->de_layer_hdl;
		args[2] = 0;
		args[3] = 0;
		ioctl(hnd_video_render->de_fd, DISP_CMD_VIDEO_STOP, args);

		args[0] = 0;
		args[1] = hnd_video_render->de_layer_hdl;
		args[2] = 0;
		args[3] = 0;
		ioctl(hnd_video_render->de_fd, DISP_CMD_LAYER_RELEASE, args);

		args[0]	= 0;
		ret = ioctl(hnd_video_render->de_fd, DISP_CMD_GET_OUTPUT_TYPE, args);

		if(ret == DISP_OUTPUT_TYPE_HDMI)
		{
			args[0] 					= 0;
			args[1] 					= 0;
			args[2] 					= 0;
			args[3] 					= 0;
			ioctl(hnd_video_render->de_fd,DISP_CMD_HDMI_OFF,(unsigned long)args);

			args[0] 					= 0;
			args[1] 					= hnd_video_render->hdmi_mode;
			args[2] 					= 0;
			args[3] 					= 0;
			ioctl(hnd_video_render->de_fd, DISP_CMD_HDMI_SET_MODE, args);

			args[0] 					= 0;
			args[1] 					= 0;
			args[2] 					= 0;
			args[3] 					= 0;
			ioctl(hnd_video_render->de_fd,DISP_CMD_HDMI_ON,(unsigned long)args);

		}
		close(hnd_video_render->de_fd);
		hnd_video_render->de_fd = 0;
	}

	if (hnd_video_render) {
		free(hnd_video_render);
		hnd_video_render = NULL;
	}
}

int render_render(void *frame_info, int frame_id, int scn_win_x, int scn_win_y, 
	int scn_win_width, int scn_win_height)
{
	cedarv_picture_t *display_info = (cedarv_picture_t *) frame_info;
	__disp_video_fb_t tmpFrmBufAddr;
	 __disp_layer_info_t         layer_info;
	int ret;

	memset(&tmpFrmBufAddr, 0, sizeof(__disp_video_fb_t ));
	tmpFrmBufAddr.interlace 		= display_info->is_progressive? 0: 1;
	tmpFrmBufAddr.top_field_first 	= display_info->top_field_first;
	tmpFrmBufAddr.frame_rate 		= display_info->frame_rate;
	tmpFrmBufAddr.addr[0] = display_info->y;
	tmpFrmBufAddr.addr[1] = display_info->u;	

	tmpFrmBufAddr.id = frame_id;
	if (hnd_video_render->first_frame_flag == 1) 
	{
		__disp_layer_info_t         layer_info;
		__disp_pixel_fmt_t			pixel_format;
		pixel_format = display_info->pixel_format==CEDARV_PIXEL_FORMAT_AW_YUV422 ? DISP_FORMAT_YUV422 : DISP_FORMAT_YUV420;
		if(display_info->display_width && display_info->display_height)
			config_de_parameter(display_info->display_width, display_info->display_height,
			 	pixel_format, scn_win_x, scn_win_y, scn_win_width, scn_win_height);
		else
			config_de_parameter(display_info->width, display_info->height,pixel_format,
				scn_win_x, scn_win_y, scn_win_width, scn_win_height);

		args[0] = 0;
	    args[1] = hnd_video_render->de_layer_hdl;
	    args[2] = (unsigned long) (&layer_info);
	    args[3] = 0;
	    ioctl(hnd_video_render->de_fd, DISP_CMD_LAYER_GET_PARA, args);
	
	    layer_info.fb.addr[0] 	= tmpFrmBufAddr.addr[0];
	    layer_info.fb.addr[1] 	= tmpFrmBufAddr.addr[1];
	    
	    args[0] 				= 0;
	    args[1] 				= hnd_video_render->de_layer_hdl;
	    args[2] 				= (unsigned long) (&layer_info);
	    args[3] 				= 0;
	    ret = ioctl(hnd_video_render->de_fd, DISP_CMD_LAYER_SET_PARA, args);
	    	
		args[0] = 0;
		args[1] = hnd_video_render->de_layer_hdl;
		args[2] = 0;
		args[3] = 0;
		ret = ioctl(hnd_video_render->de_fd, DISP_CMD_LAYER_OPEN, args);
        //printf("layer open hdl:%d,ret:%d w:%d h:%d\n", hnd_video_render->de_layer_hdl, ret,display_info->width,display_info->height);
		if (ret != 0){
			//open display layer failed, need send play end command, and exit
            //printf("Open video display layer failed!\n");
			return -1;
		}
		args[0] = 0;
		args[1] = hnd_video_render->de_layer_hdl;
		args[2] = 0;
		args[3] = 0;
		ret = ioctl(hnd_video_render->de_fd, DISP_CMD_VIDEO_START, args);
		if(ret != 0) {
			printf("Start video layer failed!\n");
			return -1;
		}
		
		hnd_video_render->layer_open_flag = 1;
		hnd_video_render->first_frame_flag = 0;	
	}
	else
	{
		args[0] = 0;
		args[1] = hnd_video_render->de_layer_hdl;
		args[2] = (unsigned long) (&tmpFrmBufAddr);
		args[3] = 0;
		ioctl(hnd_video_render->de_fd, DISP_CMD_VIDEO_SET_FB, args);
	}	
	return 0;
}

int render_get_disp_frame_id(void)
{
	return ioctl(hnd_video_render->de_fd, DISP_CMD_VIDEO_GET_FRAME_ID, args);
}

