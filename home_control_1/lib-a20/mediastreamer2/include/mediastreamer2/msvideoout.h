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

#ifndef msvideoout_h
#define msvideoout_h

#include <mediastreamer2/msfilter.h>
#include <mediastreamer2/msvideo.h>


typedef enum _MSDisplayEventType{
	MS_DISPLAY_RESIZE_EVENT
}MSDisplayEventType;

typedef struct _MSDisplayEvent{
	MSDisplayEventType evtype;
	int w,h;
}MSDisplayEvent;



#ifdef __cplusplus
extern "C"{
#endif


#define MS_VIDEO_OUT_SET_DISPLAY 	MS_FILTER_METHOD(MS_VIDEO_OUT_ID,0,MSDisplay*)
#define MS_VIDEO_OUT_HANDLE_RESIZING 	MS_FILTER_METHOD_NO_ARG(MS_VIDEO_OUT_ID,1)

#ifdef __cplusplus
}
#endif

#endif
