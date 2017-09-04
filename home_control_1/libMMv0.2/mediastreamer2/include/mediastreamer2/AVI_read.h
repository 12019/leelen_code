#ifndef AVI_read_h
#define AVI_read_h

#include "mediastreamer2/msfilter.h"


/*methods*/
#define MS_FILE_readavi_OPEN	MS_FILTER_METHOD(MS_FILE_readavi_ID,0,const char*)
#define MS_FILE_readavi_START	MS_FILTER_METHOD_NO_ARG(MS_FILE_readavi_ID,1)
#define MS_FILE_readavi_STOP	MS_FILTER_METHOD_NO_ARG(MS_FILE_readavi_ID,2)
#define MS_FILE_readavi_CLOSE	MS_FILTER_METHOD_NO_ARG(MS_FILE_readavi_ID,3)
#define MS_FILE_readavi_DONE   MS_FILTER_METHOD_NO_ARG(MS_FILE_readavi_ID,4)
#define MS_FILE_readavi_QUERY_AUDIO_FORMAT MS_FILTER_METHOD(MS_FILE_readavi_ID,5, int *)
/* set loop mode: 
	-1: no looping, 
	0: loop at end of file, 
	x>0, loop after x miliseconds after eof
*/

/*events*/
#define MS_FILE_readavi_EOF	MS_FILTER_EVENT_NO_ARG(MS_FILE_readavi_ID,0)
//bcom
#define MS_AVI_PLAYER_EOF	MS_FILTER_EVENT_NO_ARG(MS_FILE_PLAYER_ID,0)
//bcom
#endif

