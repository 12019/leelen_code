#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include <sys/time.h>

int initHandler();
int unInitHandler();
void closeVidevShow();

int myHandleVideo(char *pCodecName, unsigned char *pBuf, int iLen, struct timeval tvTimeStamp);

int myHandleAudio(char *pCodecName, unsigned int uSampleFrequency, 
	unsigned int uNumChannels, unsigned char *pBuf, int iLen, struct timeval tvTimeStamp);

#endif // PACKETHANDLER_H
