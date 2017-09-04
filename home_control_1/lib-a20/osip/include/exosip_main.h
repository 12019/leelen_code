#ifndef _EXOSIP_MAIN_H_
#define _EXOSIP_MAIN_H_
#include <osip_headers.h>
#include <jpipe.h>
#include "mediastreamer2/mediastream.h"
typedef enum _state_t
{

	IDLE=0,
	/* STATES for invite Client transaction */
	ICT_IDLE,// 1
	ICT_CALLING,// 2
	ICT_RINGBACK,// 3
	ICT_PREREC,//4// 4
	ICT_TALKING, //5
	ICT_TERMINATING, //6
	
	/* STATES for invite Server transaction */
	IST_IDLE, //7
	IST_RING, //8
	IST_PRETALKING, //9
	IST_TALKING, //10
	IST_TERMINATING, //11

	/* STATES for invite transaction */
	MCT_IDLE, //12
	MCT_CALLING, //13
	MCT_MONITOR, //14
	MCT_TERMINATING, //15

	MST_IDLE, //16
	MST_MONITOR, //17
	MST_TERMINATING, //18
	
}
state_t;

typedef enum etype_t
{
    /* TIMEOUT EVENTS  */
    TIMEOUT_A,   /* Timer A */
    TIMEOUT_B,   /* Timer B */


    SND_REQINVITE, 
    SND_REQCANCEL,
    SND_REQHOOK,

    RCV_INVITE,  
	RCV_CANCEL,
    RCV_HOOK,
    RCV_ACKBUSY,
    RCV_ACKOK,
    RCV_ACKNO,
    
    SND_REQMONITOR,
    SND_REQOPEN,
    RCV_MONITOR,
    RCV_OPEN,

    SND_SHUTMON,
	RCV_MUL_MONITOR,
	RCV_ACK_MUL_MONITOR,
	RCV_CANCLE_MUL_MONITOR,
	RCV_OPEN_MUL_MONITOR,

	SND_REQREC,
	RCV_REC,

	//lg
	SND_VIDEO_OPEN,
	SND_VIDEO_CLOSE,
	RCV_VIDEO_OPEN,
	RCV_VIDEO_CLOSE,

	WATCHDOG_TIMEOUT,
	RCV_CANCEL_SELF,
	//lg
}type_t;

  

typedef enum fsm_t
{
	ICT=0,
	IST,
	MCT,
	MST,
}  fsm_t;

//2012-4
#define MAX_EXTENSION_MACHINE_CNT 8

typedef struct remip_t {
    int flag2;
	int flag; 
	char *rem_ip;
	VideoStream *videostream;
	AudioStream *audiostream;//2012-5
}remip_t;


typedef struct eXosip_t {
  char *user;

  char* sdp;   
  char *localip;	/* default local ip */
  int port;
   
  int       j_stop_ua ;/* ask to quit the application */
  void   *j_thread;
  int       j_socket;
  int   ip_family;
  jpipe_t	*j_socketctl;
  osip_fifo_t  *j_events; /**< events must be added in this fifo */

  //2012-4
  remip_t 	remip[MAX_EXTENSION_MACHINE_CNT];
  int 		callflag;
}eXosip_t;

//modified by lc
typedef struct eXosip_element
{
	char *user;	//remote user name

	char* sdp;    //remote user sdp
	char *rem_ipbak; //use to save the ip ,when receieve message
	char *rem_ip;  
	char *rem_port;		//singal port
	int port;			//remote port

	unsigned int cid;  //random, keep unchange during a call
	state_t state;
	fsm_t fsm;	

	struct timeval timer_a_start; //short timer
	struct timeval timer_b_start; //long timer

	AudioStreamRing	*ringstream;
	AudioStream *audiostream;
	VideoStream *videostream;
	AviStream *avistream;
	struct eXosip_element *next;			//modified by lc

}eXosip_element_t;

typedef struct osip_event
{
	type_t type;
	osip_list_t *msg_list;
	eXosip_element_t *element;
}osip_event_t;


struct __eXosip_sockaddr {
  u_char ss_len;
  u_char ss_family;
  u_char padding[128 - 2];
};

int eXosip_add_event(osip_event_t * evt) ;
int eXosip_init( int port,char *user,char *sdp);
int eXosip_uninit();

//RTCP 2014/12/10 DAVID
osip_event_t *eXosip_event_new (type_t type);

////////////////////////

void __eXosip_wakeup(void);
void eXosip_get_localip_for(char *address_to_reach,char **loc);
int eXosip_guess_ip_for_via (int family, char *address, int size);
int eXosip_guess_localip(int family, char *address, int size);
int eXosip_get_addrinfo (struct addrinfo **addrinfo, char *hostname, int service);
int eXosip_send_message(char *host,int port,char *msg,int msg_len);

int eXosip_inet_init(int port);
void eXosip_inet_uninit();

int 
eXosip_element_init(eXosip_element_t *p,char *user,char *sdp);

void eXosip_element_uninit(eXosip_element_t *p);

int 
eXosip_event_add(osip_event_t * evt) ;


void
MulMonCancleOpenDeleteItem(eXosip_element_t *p);

void
MulMonCancleDeleteItem(eXosip_element_t *p);


// void
// eXosip_timers_execute ();
// 
// void
// __eXosip_need_timer_a_event ();
// 
// void
// __eXosip_need_timer_b_event ();

#endif
