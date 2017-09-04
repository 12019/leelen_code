#ifndef __MY_TEST_RTSP_CLIENT__
#define __MY_TEST_RTSP_CLIENT__

#include "liveMedia.hh"
#include "UsageEnvironment.hh"

void openURL(UsageEnvironment& env, char const* progName, char const* rtspUrl);
void shutdownStream(RTSPClient* rtspClient, int exitCode = 1);

#endif
