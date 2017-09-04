/*
mediastreamer2 library - modular sound and video processing and streaming
Copyright (C) 2006  Simon MORLAT (simon.morlat@linphone.org)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef msvideo_h
#define msvideo_h

#include "msfilter.h"

/* some global constants for video MSFilter(s) */
#define MS_VIDEO_SIZE_CIF_W 352
#define MS_VIDEO_SIZE_CIF_H 288

#define MS_VIDEO_SIZE_VGA_W 640
#define MS_VIDEO_SIZE_VGA_H 480

#define MS_VIDEO_SIZE_SVGA_W 800
#define MS_VIDEO_SIZE_SVGA_H 600

#define MS_VIDEO_SIZE_XGA_W 1024
#define MS_VIDEO_SIZE_XGA_H 768



/* those structs are part of the ABI: don't change their size otherwise binary plugins will be broken*/
typedef struct MSVideoSize{
	int width,height;
} MSVideoSize;


typedef struct MSRect{
	int x,y,w,h;
} MSRect;

#define MS_VIDEO_SIZE_CIF (MSVideoSize){MS_VIDEO_SIZE_CIF_W,MS_VIDEO_SIZE_CIF_H}

#define MS_VIDEO_SIZE_VGA (MSVideoSize){MS_VIDEO_SIZE_VGA_W,MS_VIDEO_SIZE_VGA_H}

#define VIDEO_SIZE_CIF		0
#define VIDEO_SIZE_VGA		1
#define VIDEO_SIZE_HALF_D1	2
#define VIDEO_SIZE_VGA_A20  3
#define VIDEO_SIZE_720P    4

#define USED_VGA_VIDEO_W		640//(640 - 64)// - 16)
#define USED_VGA_VIDEO_H		(480 - 16)//(480 - 16 - 64)

/*deprecated: use MS_VIDEO_SIZE_SVGA*/
#define MS_VIDEO_SIZE_800X600_W MS_VIDEO_SIZE_SVGA_W
#define MS_VIDEO_SIZE_800X600_H MS_VIDEO_SIZE_SVGA_H
#define MS_VIDEO_SIZE_800X600 MS_VIDEO_SIZE_SVGA
/*deprecated use MS_VIDEO_SIZE_XGA*/
#define MS_VIDEO_SIZE_1024_W 1024
#define MS_VIDEO_SIZE_1024_H 768
#define MS_VIDEO_SIZE_1024 MS_VIDEO_SIZE_XGA

#define TVD_LEFT_SPAN 18
#define  TVD_RIGHT_SPAN 14
#define TVD_TOP_SPAN   10
#define TVD_BOTTOM_SPAN 6

typedef enum{
	MS_YUV420P,
	MS_YUYV,
	MS_RGB24,
	MS_RGB24_REV, /*->microsoft down-top bitmaps */
	MS_MJPEG,
	MS_UYVY,
	MS_YUY2,   /* -> same as MS_YUYV */
	MS_PIX_FMT_UNKNOWN
}MSPixFmt;



#ifdef __cplusplus
extern "C"{
#endif



#ifdef __cplusplus
}
#endif

#define MS_FILTER_SET_VIDEO_SIZE	MS_FILTER_BASE_METHOD(100,MSVideoSize)
#define MS_FILTER_GET_VIDEO_SIZE	MS_FILTER_BASE_METHOD(101,MSVideoSize)

#define MS_FILTER_SET_PIX_FMT		MS_FILTER_BASE_METHOD(102,MSPixFmt)
#define MS_FILTER_GET_PIX_FMT		MS_FILTER_BASE_METHOD(103,MSPixFmt)

#define MS_FILTER_SET_FPS		MS_FILTER_BASE_METHOD(104,float)
#define MS_FILTER_GET_FPS		MS_FILTER_BASE_METHOD(105,float)

/* request a video-fast-update (=I frame for H263,MP4V-ES) to a video encoder*/
#define MS_FILTER_REQ_VFU		MS_FILTER_BASE_METHOD_NO_ARG(106)
#define MS_FILTER_SET_IMAGE	MS_FILTER_BASE_METHOD(107,char)
#define MS_FILTER_REQ_IDR		MS_FILTER_BASE_METHOD_NO_ARG(108)

//bcom
#define MS_FILTER_SET_VIDEO_RECT 	MS_FILTER_BASE_METHOD(109, int *)
#define MS_FILTER_SET_VIDEO_CHANNEL MS_FILTER_BASE_METHOD(110, int)
//bcom

#define MS_FILTER_SET_VIDEO_HIDE 	MS_FILTER_BASE_METHOD(111,int)
#endif
