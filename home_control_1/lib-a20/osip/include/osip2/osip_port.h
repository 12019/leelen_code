#ifndef _OSIP_PORT_H_
#define _OSIP_PORT_H_

#if !defined(WIN32) && !defined(_WIN32_WCE)
/********************************/
/* definitions for UNIX flavour */
/********************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


#include <errno.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>


#ifdef __linux
#include <stdint.h>
#endif


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include <sys/time.h>
#include <netdb.h>

//#ifdef ORTP_INET6
//#include <netdb.h>
//#endif


#else
/*********************************/
/* definitions for WIN32 flavour */
/*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#define snprintf _snprintf


#ifdef _MSC_VER
#pragma push_macro("_WINSOCKAPI_")
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif

typedef  unsigned __int64 uint64_t;
typedef  __int64 int64_t;
typedef  unsigned short uint16_t;
typedef  unsigned int uint32_t;
typedef  int int32_t;
typedef  unsigned char uint8_t;
typedef __int16 int16_t;
#else
#include <stdint.h> /*provided by mingw32*/
#endif

#endif



#define ENABLE_TRACE
#define VA_START(a, f)  va_start(a, f)

#ifdef __cplusplus
extern "C"
{
#endif

/**************************/
/* MALLOC redirections    */
/**************************/

//#include "memwatch.h"

//#define osip_malloc malloc
//#define osip_realloc realloc
//#define osip_free free
#if 1
void *osip_malloc (size_t size);
void *osip_realloc (void *, size_t size);
void osip_free (void *);
#endif 

unsigned int
osip_build_random_number();
/**************************/
/* RANDOM number support  */
/**************************/


/**************************/
/* TIMER support          */
/**************************/


/**************************/
/* STRING support         */
/**************************/


/**************************/
/* LOG&DEBUG support      */
/**************************/

#define LOG_TRUE  1
#define LOG_FALSE 0
/* levels */
typedef enum _trace_level
{
    TRACE_LEVEL0 = 0,
#define OSIP_FATAL    0
    TRACE_LEVEL1 = 1,
#define OSIP_BUG      1
    TRACE_LEVEL2 = 2,
#define OSIP_ERROR    2
    TRACE_LEVEL3 = 3,
#define OSIP_WARNING  3
    TRACE_LEVEL4 = 4,
#define OSIP_INFO1    4
    TRACE_LEVEL5 = 5,
#define OSIP_INFO2    5
    TRACE_LEVEL6 = 6,
#define OSIP_INFO3    6
    TRACE_LEVEL7 = 7,
#define OSIP_INFO4    7
    END_TRACE_LEVEL = 8
}
osip_trace_level_t;


typedef void osip_trace_func_t (char *fi, int li, osip_trace_level_t level,
                                  char *chfr, va_list ap);

/* these are defined in all cases, but are empty when oSIP is compiled
   without trace */
void osip_trace_initialize_func (osip_trace_level_t level,
                                   osip_trace_func_t * func);
void osip_trace_initialize_syslog (osip_trace_level_t level, char *ident);
void osip_trace_initialize (osip_trace_level_t level, FILE * file);
void osip_trace_enable_until_level (osip_trace_level_t level);
void osip_trace_enable_level (osip_trace_level_t level);
void osip_trace_disable_level (osip_trace_level_t level);
int osip_is_trace_level_activate (osip_trace_level_t level);

#ifndef ENABLE_TRACE

#define TRACE_INITIALIZE(level, file)  do { } while (0)
#define TRACE_ENABLE_LEVEL(level)      do { } while (0)
#define TRACE_DISABLE_LEVEL(level)     do { } while (0)
#define IS_TRACE_LEVEL_ACTIVATE(level) (-1)

#else

#define TRACE_INITIALIZE(level, file)  osip_trace_initialize(level, file)
#define TRACE_ENABLE_LEVEL(level)      osip_trace_enable_level ( level )
#define TRACE_DISABLE_LEVEL(level)     osip_trace_disable_level ( level )
#define IS_TRACE_LEVEL_ACTIVATE(level) osip_is_trace_level_activate( level )

#endif

/* log facility. */
/* if f is NULL, current default log file is used. */
/* INPUT: level | level of the trace               */
/* INPUT: f | use f instead of default log file    */
/* INPUT: chfr | format string for next args       */
  int osip_trace (char *fi, int li, osip_trace_level_t level, FILE * f,
                  char *chfr, ...);

#ifdef ENABLE_TRACE
#define OSIP_TRACE(P) P
#else
#define OSIP_TRACE(P) do {} while (0)
#endif

#ifdef __cplusplus
}
#endif

#endif                          /* _PORT_H_ */
