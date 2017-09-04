#ifndef WRITEAVI_H
#define WRITEAVI_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "mediastreamer2/mediastream.h"
#include "mediastreamer2/msfilter.h"
#include "mediastreamer2/msvideo.h"
#include "mediastreamer2/mswebcam.h"
#include "mediastreamer2/mssndcard.h"
#include "mediastreamer2/avilib.h"
#include "mediastreamer2/msavi.h"
#include "mediastreamer2/mstee.h"


#ifdef HAVE_CONFIG_H
#include "mediastreamer-config.h"
#endif

struct _writeAVI
{
	MSTicker *ticker;
	MSTicker *ticker1;
	MSFilter *ausource;
	MSFilter *visource;
	MSFilter *H264encoder;
	MSFilter *G711encoder;
	MSFilter *writeG711;
	MSFilter *writeH264;
	//MSFilter *au_tee;
	MSFilter *vi_tee;
	MSFilter *videoout;
	//MSFilter *soundout;
	//MSFilter *mixer;
	MSFilter *mfc_pp;
};

typedef struct _writeAVI writeAVI;

void writeAVI_free(writeAVI *stream);
writeAVI *writeAVI_start(MSWebCam *device,MSSndCard *sndcard, char *outfile, int v_channel);
void writeAVI_stop(writeAVI *stream);

#endif // WRITEAVI_H
