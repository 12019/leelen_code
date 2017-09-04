/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of TGT Limited. (C) 2016
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("TGT SOFTWARE")
*  RECEIVED FROM TGT AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. TGT EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES TGT PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE TGT SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. TGT SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY TGT SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND TGT'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE TGT SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT TGT'S OPTION, TO REVISE OR REPLACE THE TGT SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  TGT FOR SUCH TGT SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/


/*****************************************************************************
 *
 * Filename: softsim_iso_7816.c
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-07-08
 ******************************************************************************/

/*****************************************************************************
* Include																	   
*****************************************************************************/
#include "softsim_include.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <linux/input.h>
#include <string.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/time.h>
#include <pthread.h>  
/*****************************************************************************
* Define																	   
*****************************************************************************/
#undef 	SOFTSIM_MD_ID
#undef 	SOFTSIM_TRACE_HEADER
#define SOFTSIM_MD_ID					SOFTSIM_MD_ISO_7816
#define SOFTSIM_TRACE_HEADER			"[ISO7816]" 
	
#define SOFTSIM_RX_SIZE 512	 
//we assue each line max number
#define MAXLINE 80

#if defined(__SOFTSIM_ISO_7816_DATA_TRACE__)
#define SOFTSIM_ISO7816_TRACE_HEX	SOFTSIM_TRACE_HEX
#define SOFTSIM_ISO7816_TRACE	SOFTSIM_DEBUG
#else
#define SOFTSIM_ISO7816_TRACE_HEX(header, data, data_len)
#define SOFTSIM_ISO7816_TRACE(...)
#endif

#if defined(__SOFTSIM_T2C__)
#define SOFTSIM_PIN		"/sys/devices/platform/sim_uart/sim_pin"
#define SOFTSIM_UART_BAUDRATE	(8736)
#define SOFTSIM_UART	"/dev/ttyMT0"
#define SOFTSIM_LEVEL_LOW	"0"
#define SOFTSIM_LEVEL_HIGH	"1"
#elif defined(__SOFTSIM_T2S__)
#define SOFTSIM_PIN		"/sys/devices/platform/sim_uart/sim_pin"
#define SOFTSIM_UART_BAUDRATE	(8736)
#define SOFTSIM_UART	"/dev/ttyMT2"
#define SOFTSIM_LEVEL_LOW	"1"
#define SOFTSIM_LEVEL_HIGH	"0"
#elif defined(__SOFTSIM_T3__)
#define SOFTSIM_PIN	"/sys/devices/platform/ecard_power_control.0/state"
#define SOFTSIM_UART_BAUDRATE	(115200)
#define SOFTSIM_UART	"/dev/ttyMSM1"
#define SOFTSIM_LEVEL_LOW	"0"
#define SOFTSIM_LEVEL_HIGH	"1"
#else
#define SOFTSIM_UART	"/dev/ttyMT0"
#define SOFTSIM_LEVEL_LOW	"0"
#define SOFTSIM_LEVEL_HIGH	"1"
#endif

#define SOFTSIM_RESET_GPIO	"/sys/class/gpio/gpio1016/value"
#define SOFTSIM_RESET_GPIO_EXPORT	"/sys/class/gpio/export"
#define SOFTSIM_RESET_GPIO_NUM	(1016)
/*****************************************************************************
* Typedef																	   
*****************************************************************************/

/*****************************************************************************
* Static Local Variable															  
*****************************************************************************/
static char event[6]; 
static int uart_fd = -1;
static SoftsimATRStatus softsim_atr_status = SOFTSIM_ATR_NONE;
static u16 send_null_len = 0;
static s32 vsim_fd = -1;
static s32 counter = 0;
static u16 send_len = 0;	
static u16 le = 0;
static bool tsend = false;
static pthread_t softsim_tid = 0;
static pthread_t softsim_thread_id;
static char buffer[SOFTSIM_RX_SIZE] = {0x00};
static u8 socket_channel = 0xff;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#if defined(__SOFTSIM_T2C__)
static u8 ATR[] = {0x3B, 0x97, 0x95, 0x80, 0x1F, 0x07, 0x80, 0x31, 0xA0, 0x73, 0xBE, 0x21, 0x00, 0x67};
static u8 PPS[] = {0xFF, 0x10, 0x95, 0x7A};
#elif defined(__SOFTSIM_T3__)
static u8 ATR[] = {0x3B, 0x9E, 0x11, 0x80, 0x1F, 0xC7, 0x80, 0x31, 0xE0, 0x73, 0xFE, 0x21, 0x1B, 0x66, 0xD0, 0xFE, 0x97, 0xD1, 0x73, 0x00, 0x4C};
#else
static u8 ATR[] = {0x3B, 0x1B, 0x94, 0x80, 0x69, 0x06, 0x08, 0x42, 0x18, 0x00, 0x00, 0x08, 0x00, 0x00};
static u8 PPS[] = {0xFF, 0x10, 0x94, 0x7B};
//static u8 ATR[] = {0x3B,0x10,0x95};
//static u8 ATR[] = {0x3B, 0x16, 0x95, 0xD0, 0x02, 0x17, 0xE1, 0x12, 0x00};
//static u8 PPS[] = {0xFF, 0x10, 0x95, 0x7A};
#endif

/*****************************************************************************
* Global Variable														 
*****************************************************************************/

/*****************************************************************************
* Extern Global Variable															
*****************************************************************************/

/*****************************************************************************
* Static Local Function															  
*****************************************************************************/
void *softsim_thread(void *arg);
/*****************************************************************************
* Global Function														
*****************************************************************************/
	 
/*****************************************************************************
* Extern Global Function													  
*****************************************************************************/

/****************************************************************************
* Function
****************************************************************************/
s32 softsim_uart_send(u8*data, u32 data_len)
{
	s32 write_len = 0;

	if(uart_fd < 0)
	{
		SOFTSIM_DEBUG("UART not open!");
		return write_len;
	}
	
	tcflush(uart_fd, TCOFLUSH);
	write_len = write(uart_fd, data, data_len);

	if(write_len < 0)
	{
		SOFTSIM_ISO7816_TRACE("UART write fail!");
	}
	else
	{
                SOFTSIM_ISO7816_TRACE_HEX("UART-WX:", data, write_len);
	}

	return write_len;
}

s32 softsim_atr_send(u8*data, u32 data_len)
{
	s32 write_len = 0;
	
	tcflush(uart_fd, TCOFLUSH);
	write_len = write(uart_fd, data, data_len);

	if(write_len < 0)
	{
		SOFTSIM_ISO7816_TRACE("UART write fail!");
	}

	return write_len;
}


void softsim_null_byte_send(u8*data, u32 data_len)
{
	s32 write_len = 0;
	tcflush(uart_fd, TCOFLUSH);
	write_len = softsim_uart_send(data, data_len);
	send_null_len += write_len;
}

void softsim_null_len_reset(void)
{
	send_null_len = 0;
}

void softsim_thread_init(void)
{
	vsim_fd = -1;
	uart_fd = -1;
	send_null_len = 0;
	counter = 0;
	send_len = 0;
	le = 0;
	softsim_tid = 0;	
	tsend = false;
	softsim_memset(buffer, 0x00, sizeof(buffer));
}

void softsim_thread_exit(void)
{
	SOFTSIM_DEBUG("softsim_thread_exit");

	softsim_status_update(false);
	softsim_stop_status_update(false);

	if(-1 != vsim_fd)
	{
		close(vsim_fd);
		vsim_fd = -1;
	}

	if(-1 != uart_fd)
	{
		close(uart_fd);
		uart_fd = -1;
	}
	send_null_len = 0;
	counter = 0;
	send_len = 0;
	le = 0;
	softsim_tid = 0;	
	tsend = false;
	softsim_memset(buffer, 0x00, sizeof(buffer));
}

void softsim_thread_status_check(void)
{
	s32 pthread_kill_errorcode;
	pthread_kill_errorcode = pthread_kill(softsim_thread_id, 0);
	
	if(ESRCH == pthread_kill_errorcode)
	{
		SOFTSIM_DEBUG("Theread (%lu) did not exist or already quit!",softsim_thread_id);
	}
	else if(EINVAL == pthread_kill_errorcode)
	{
		SOFTSIM_DEBUG("Illegal signal!");
	}
	else
	{
		SOFTSIM_DEBUG("Thread (%lu) is alive!", softsim_thread_id);
		//pthread_exit((void*)0);
	}
}


void softsim_scan_init(void)
{
	vsim_fd = -1;
	uart_fd = -1;
	send_null_len = 0;
	counter = 0;
	send_len = 0;
	le = 0;
	softsim_tid = 0;	
	tsend = false;
	softsim_memset(buffer, 0x00, sizeof(buffer));
}

void softsim_atr_status_update(u8 status)
{
	softsim_atr_status = status;
}

u8 softsim_get_atr_status(void)
{
	return softsim_atr_status;
}

#if defined(__SOFTSIM_T2C__)
//get sim_uart handler
char * softsim_get_reset_event_handler(void)
{
    FILE *fp;
    char *handler = NULL;
    char line[MAXLINE];
	
    fp = fopen("/proc/bus/input/devices", "r");
    if (NULL == fp)
    {
        SOFTSIM_ISO7816_TRACE("fopen /proc/bus/input/devices");
		return NULL;
    }
	
    while(fgets(line, sizeof(line), fp) != NULL)
    {
        if (strstr(line, "sim_uart") != NULL)
        {
            while (fgets(line, sizeof(line), fp) != NULL)
            {
                //here assume the the line begin with "Handler" or "handler"
                if ((strstr(line, "Handlers") != NULL) || (strstr(line, "handlers") != NULL))
                {
                    break;
                }
            }
            break;
        }
    }
    handler = strstr(line, "event");
    //printf("%s", handler);
    memmove(event, handler, 6);
	fclose(fp);
	fp = NULL;
    return handler;
}

bool softsim_reset(void)
{
	s32 sim_pin = -1;
	s32 ret1 = 0;
	s32 ret2 = 0;

	SOFTSIM_ISO7816_TRACE("softsim_reset");
	
	sim_pin = open(SOFTSIM_PIN, O_RDWR | O_NONBLOCK);

	if(sim_pin<= 0)
	{
		SOFTSIM_ISO7816_TRACE("open device error\n");
		return false;
	}

	#if defined(__SOFTSIM_T2C__)
		ret1 = write(sim_pin, SOFTSIM_LEVEL_LOW, 1);
		sleep(1);
                ret2 = write(sim_pin, SOFTSIM_LEVEL_HIGH, 1);
	#else
		ret1 = write(sim_pin, SOFTSIM_LEVEL_LOW, 1);
		sleep(1);
		ret2 = write(sim_pin, SOFTSIM_LEVEL_HIGH, 1);
	#endif

	SOFTSIM_ISO7816_TRACE("ret1 = %d, ret2 = %d", ret1, ret2);

	if((ret1>0) && (ret2>0))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool softsim_reset_event_detect(void)
{
	struct input_event ev;
	s32 rd = 0;
	bool ret = false;
	
	softsim_memset((void*)&ev, 0, sizeof(ev));
	if(rd = read(vsim_fd, (void*)&ev, sizeof(ev)) > 0)
	{	
		SOFTSIM_ISO7816_TRACE("ev.value = %d", ev.value);
		if(ev.value == 2)
		{
			SOFTSIM_ISO7816_TRACE("event:%d,code:%d,value:%d,rd: %d", ev.type, ev.code, ev.value, rd);
			softsim_atr_status = SOFTSIM_ATR_RESET;
			ret = true;
		}
	}

	return ret;
}

void softsim_reset_event_handle(void)
{
	s32 ret = 0;
	s32 len = 0;	/*byte len once*/
    s32 total_len = 0;	/*total length*/
	u8 rx[SOFTSIM_RX_SIZE];	/*read uart buffer*/
	struct timeval timeout;
    fd_set readfds, rfd;
	counter = 0;

	softsim_memset(rx, 0, sizeof(rx));

	while((SOFTSIM_ATR_RESET == softsim_atr_status) && (softsim_is_running()))
	{
		timeout.tv_sec = 0; 
    	timeout.tv_usec=100*1000; //100ms
    	FD_ZERO(&readfds);
    	FD_SET(uart_fd, &readfds);
		rfd = readfds;
        ret = select(uart_fd+1, &rfd, NULL, NULL, &timeout);

        if(ret < 0)
        {
            SOFTSIM_ISO7816_TRACE("select error!\n");
            return false;
        }

		if(ret > 0)
		{
			len = 0;
			softsim_memset(rx, 0, sizeof(rx));
            len = read(uart_fd, rx, sizeof(rx));
			
			if(len > 0)
        	{
				SOFTSIM_ISO7816_TRACE_HEX("UART-RX:", rx, len);
				total_len += len;

				if(0 == counter)
				{
					softsim_memcpy(buffer+(total_len - len), rx, len);
					if((sizeof(ATR) + sizeof(PPS) == total_len))
					{
						SOFTSIM_DEBUG("Receive PPS!");
						if((PPS[0] == buffer[sizeof(ATR)]) && (PPS[1] == buffer[sizeof(ATR)+1]) && (PPS[2] == buffer[sizeof(ATR)+2]) && (PPS[3] == buffer[sizeof(ATR)+3]))
						{
                                                        counter = 1;
                                                        total_len = 0;
                                                        softsim_memset(buffer, 0x00, sizeof(buffer));
						}
					}
					else if((sizeof(ATR) + sizeof(PPS) + 1 == total_len))
					{
						SOFTSIM_DEBUG("Receive PPS!");
						if((PPS[0] == buffer[sizeof(ATR)+1]) && (PPS[1] == buffer[sizeof(ATR)+1+1]) && (PPS[2] == buffer[sizeof(ATR)+2+1]) && (PPS[3] == buffer[sizeof(ATR)+3+1]))
						{
							counter = 1;
					   		total_len = 0;
							softsim_memset(buffer, 0x00, sizeof(buffer));
						}
					}
				}
                else if((counter == 2) && (total_len == 4) && (SOFTSIM_ATR_SUCCESS != softsim_atr_status)) //recv PPS response ourself sent, change baudrate
                {
                    softsim_baudrate_set(uart_fd, 101562);
                    SOFTSIM_DEBUG("Set UART Baudrate 101562");
                    total_len = 0;
					softsim_atr_status = SOFTSIM_ATR_SUCCESS;
					SOFTSIM_ISO7816_TRACE("total_len = %d", total_len);
					SOFTSIM_DEBUG("ATR Success!");
					break;
                }

				softsim_memset(rx, 0, sizeof(rx));
	            if(counter == 1)
	            {
	            	usleep(2000);
	                softsim_atr_send(PPS, sizeof(PPS));
					usleep(1000);
					SOFTSIM_PRINT_HEX("Response PPS:", PPS, sizeof(PPS));
	                counter = 2;
	                total_len = 0;
	            }
			}
			else
			{
				SOFTSIM_ISO7816_TRACE("Read file error!!!");
			}
		}

		if(softsim_is_stopping())
		{
			break;
		}
	}

	softsim_status_update(false);
	softsim_stop_status_update(false);
}

bool softsim_scan(void)
{
	s32 sim_pin = -1;
	s32 rd = 0;
	s32 wr = 0;
	s32 ret = -1;
	u8 times = 0;
        u8 ATR1[6] = {0x3B, 0xB0, 0x11, 0x00, 0x40, 0xFF};
	char path[20] = {0x00};
	struct input_event ev;
	u16 tsend_len = 0;
    s32 len = 0;	/*byte len once*/
    s32 total_len = 0;	/*total length*/
	u8 rx[SOFTSIM_RX_SIZE];	/*read uart buffer*/
    struct timeval timeout;
    fd_set readfds, rfd;
	
        char *handler = softsim_get_reset_event_handler();
		
	if(NULL == handler)
	{
		SOFTSIM_ISO7816_TRACE("gethandler of sim_uart");
		return false;
	}
		
	SOFTSIM_DEBUG("softsim_scan");
		
	softsim_scan_init();
		
        softsim_memset(path, 0x00, sizeof(path));
        softsim_memmove(path, "/dev/input/", 11);
        softsim_memmove(path+11, event, 6);
		
	vsim_fd = open(path, O_RDONLY | O_NONBLOCK);
	if (vsim_fd <= 0)
        {
                SOFTSIM_ISO7816_TRACE("open %s fail", path);
                return false;
	}
			
	uart_fd = open(SOFTSIM_UART, O_RDWR | O_NOCTTY | O_NONBLOCK | O_SYNC);
		
        if(uart_fd <= 0)
        {
            SOFTSIM_ISO7816_TRACE("open uart fail");
            return false;
        }
		
	softsim_baudrate_set(uart_fd, SOFTSIM_UART_BAUDRATE);

	softsim_atr_status_update(SOFTSIM_ATR_HOT_PLUG);
	
	if(!softsim_reset())
	{
		SOFTSIM_ISO7816_TRACE("reset fail");
		return false;
	}

	if(SOFTSIM_ATR_HOT_PLUG != softsim_atr_status)
	{
		return false;
	}
	
	softsim_atr_status_update(SOFTSIM_ATR_EVENT);
	
	while(SOFTSIM_ATR_EVENT == softsim_atr_status)
	{
		softsim_memset((void*)&ev, 0, sizeof(ev));
		if (rd = read(vsim_fd, (void*)&ev, sizeof(ev)) > 0)
		{			
			if(ev.value == 2)
			{
                            SOFTSIM_ISO7816_TRACE("event:%d,code:%d,value:%d,rd: %d", ev.type, ev.code, ev.value, rd);
                            softsim_atr_status = SOFTSIM_ATR_CHECK;
                            usleep(4000);// 123us<=t1<=12300us/12.3ms
                            softsim_uart_send(ATR, sizeof(ATR));
                            SOFTSIM_PRINT_HEX("ATR:", ATR, sizeof(ATR));
                            break;
			}
		}
	}

	if(SOFTSIM_ATR_CHECK != softsim_atr_status)
	{
		return false;
	}

	softsim_memset(rx, 0, sizeof(rx));

	while(SOFTSIM_ATR_CHECK == softsim_atr_status)
	{
		timeout.tv_sec = 0; 
    	timeout.tv_usec=100*1000; //100ms
    	FD_ZERO(&readfds);
    	FD_SET(uart_fd, &readfds);
		rfd = readfds;
        ret = select(uart_fd+1, &rfd, NULL, NULL, &timeout);

        if(ret < 0)
        {
            SOFTSIM_ISO7816_TRACE("select error!\n");
            return false;
        }

		if(ret > 0)
		{
			len = 0;
			softsim_memset(rx, 0, sizeof(rx));
            len = read(uart_fd, rx, sizeof(rx));
			
			if(len > 0)
        	{
				SOFTSIM_ISO7816_TRACE_HEX("UART-RX:", rx, len);
				total_len += len;

				if(0 == counter)
				{
					softsim_memcpy(buffer+(total_len - len), rx, len);
					if((sizeof(ATR) + sizeof(PPS) == total_len))
					{
						SOFTSIM_DEBUG("Receive PPS!");
						if((PPS[0] == buffer[sizeof(ATR)]) && (PPS[1] == buffer[sizeof(ATR)+1]) && (PPS[2] == buffer[sizeof(ATR)+2]) && (PPS[3] == buffer[sizeof(ATR)+3]))
						{
							counter = 1;
					   		total_len = 0;
							softsim_memset(buffer, 0x00, sizeof(buffer));
						}
					}
					else if((sizeof(ATR) + sizeof(PPS) + 1 == total_len))
					{
						SOFTSIM_DEBUG("Receive PPS!");
						if((PPS[0] == buffer[sizeof(ATR)+1]) && (PPS[1] == buffer[sizeof(ATR)+1+1]) && (PPS[2] == buffer[sizeof(ATR)+2+1]) && (PPS[3] == buffer[sizeof(ATR)+3+1]))
						{
							counter = 1;
					   		total_len = 0;
							softsim_memset(buffer, 0x00, sizeof(buffer));
						}
					}
				}
                else if((counter == 2) && (total_len == 4) && (SOFTSIM_ATR_SUCCESS != softsim_atr_status)) //recv PPS response ourself sent, change baudrate
                {
                    softsim_baudrate_set(uart_fd, 101562);
                    SOFTSIM_DEBUG("Set UART Baudrate 101562");
                    total_len = 0;
					softsim_atr_status = SOFTSIM_ATR_SUCCESS;
					ret = softsim_thread_entry();
					SOFTSIM_ISO7816_TRACE("total_len = %d", total_len);
					SOFTSIM_DEBUG("ATR Success!");
                }

				softsim_memset(rx, 0, sizeof(rx));
	            if(counter == 1)
	            {
	            	usleep(2000);
	                softsim_uart_send(PPS, sizeof(PPS));		
					SOFTSIM_PRINT_HEX("Response PPS:", PPS, sizeof(PPS));
	                counter = 2;
	                total_len = 0;
	            }
			}
			else
			{
				SOFTSIM_ISO7816_TRACE("Read file error!!!");
			}
		}
	}
	
	if(SOFTSIM_ATR_SUCCESS == softsim_atr_status)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void *softsim_thread(void *arg)
{
	u8 apdu_buffer[261] = {0};
	u32 apdu_len = 0;
	u32 delay_ms = 0;
	u16 tsend_len = 0;
	s32 ret = 0;
    s32 len = 0;	/*byte len once*/
    s32 total_len = 0;	/*total length*/
	u8 rx[SOFTSIM_RX_SIZE];	/*read uart buffer*/
	s32 rd = 0;
	struct input_event ev;
    struct timeval timeout;
    fd_set readfds, rfd;
	
	timeout.tv_sec = 0; 
    timeout.tv_usec=100*1000; //100ms
    FD_ZERO(&readfds);
    FD_SET(uart_fd, &readfds);
	
	softsim_tid = (unsigned long int)pthread_self();
	SOFTSIM_ISO7816_TRACE("softsim_thread softsim_tid = %lu", softsim_tid);
    softsim_memset(rx, 0, sizeof(rx));
	
	while(softsim_is_running())
	{
		softsim_memset((void*)&ev, 0, sizeof(ev));
		if(rd = read(vsim_fd, (void*)&ev, sizeof(ev)) > 0)
		{		
			if(2 == ev.value)
			{
				softsim_atr_status = SOFTSIM_ATR_RESET;
				softsim_baudrate_set(uart_fd, SOFTSIM_UART_BAUDRATE);
                                tcflush(uart_fd, TCIFLUSH);
				tcflush(uart_fd, TCOFLUSH);
				usleep(4000);
				softsim_atr_send(ATR, sizeof(ATR));
				usleep(1000);
				SOFTSIM_PRINT_HEX("ATR:", ATR, sizeof(ATR));
				SOFTSIM_DEBUG("ev.type:%d, ev.code:%d, ev.value:%d, rd: %d", ev.type, ev.code, ev.value, rd);
				SOFTSIM_DEBUG("Softsim reset event detected!");
				softsim_reset_event_handle();
				total_len = 0;
				send_len = 0;
				le = 0;
				softsim_null_len_reset();
				softsim_tpdu_process_state_reset();
			}
			else
			{
				SOFTSIM_ISO7816_TRACE("ev.type:%d, ev.code:%d, ev.value:%d, rd: %d", ev.type, ev.code, ev.value, rd);
			}
		}

		timeout.tv_sec = 0; 
    	timeout.tv_usec=100*1000; //100ms
    	FD_ZERO(&readfds);
    	FD_SET(uart_fd, &readfds);
		
		rfd = readfds;
        ret = select(uart_fd+1, &rfd, NULL, NULL, &timeout);

        if(ret < 0)
        {
            SOFTSIM_ISO7816_TRACE("select error!\n");
            return NULL;
        }
				
        if (ret > 0)
        {
        	len = 0;
			softsim_memset(rx, 0, sizeof(rx));
            len = read(uart_fd, rx, sizeof(rx));
			
            if(len > 0)
            {
				SOFTSIM_ISO7816_TRACE_HEX("UART-RX:", rx, len);
				total_len += len;
				SOFTSIM_ISO7816_TRACE("total_len:%d,len:%d,send_len:%d,le:%d",total_len,len,send_len,le);
				if((total_len > send_len))
                {
	                softsim_memset(apdu_buffer, 0x00, sizeof(apdu_buffer));
					apdu_len = 0;

					if(softsim_command_is_tpdu_processing())
					{
						if((total_len-send_len == le) && (le != 0))
						{
							if(len >= (total_len-send_len))
							{
                                                            SOFTSIM_ISO7816_TRACE_HEX("UART-tpdu1:", rx+(len-(total_len-send_len)), total_len-send_len);
                                                            softsim_tpdu_command(TGT_SOFTSIM_SLOT_ID, rx+(len-(total_len-send_len)), total_len-send_len, apdu_buffer, &apdu_len);
                                                            SOFTSIM_ISO7816_TRACE_HEX("UART-tpdu1:", apdu_buffer, apdu_len);
							}
							else
                                                        {
                                                                SOFTSIM_ISO7816_TRACE_HEX("UART-tpdu2:", rx, len);
                                                                softsim_tpdu_command(TGT_SOFTSIM_SLOT_ID, rx, len, apdu_buffer, &apdu_len);
                                                                SOFTSIM_ISO7816_TRACE_HEX("UART-tpdu2:", apdu_buffer, apdu_len);
							}

							tsend = false;
						}
						else
						{
							if(!tsend)
							{
								if(total_len == len)
								{
									tsend_len = send_len;
									tsend = true;
								}
								else
								{
									tsend_len = 0;
								}
							}
							else
							{
								tsend_len = 0;
							}
								
                                                        SOFTSIM_ISO7816_TRACE_HEX("UART-tpdu3:", rx+tsend_len, len-tsend_len);
                                                        softsim_tpdu_command(TGT_SOFTSIM_SLOT_ID, rx+tsend_len, len-tsend_len, apdu_buffer, &apdu_len);
                                                        SOFTSIM_ISO7816_TRACE_HEX("UART-tpdu3:", apdu_buffer, apdu_len);
                                                        continue;
						}
					}
					else
					{
						if(total_len - send_len >= 5)
						{
							if(total_len == len)
							{
								if(5 == len)
								{
									le = rx[4];
								}
								else
								{
									le = rx[len-1];
								}
							}
							else
							{
								le = rx[send_len-(total_len-len) + 4];
							}

                                                        SOFTSIM_ISO7816_TRACE_HEX("UART-tpdu4:", rx+(len-(total_len-send_len)), total_len-send_len);
                                                        softsim_tpdu_command(TGT_SOFTSIM_SLOT_ID, rx+(len-(total_len-send_len)), total_len-send_len, apdu_buffer, &apdu_len);
                                                        SOFTSIM_ISO7816_TRACE_HEX("UART-tpdu4:", apdu_buffer, apdu_len);
						}
						else
						{
							continue;
						}
					}
						
					if(apdu_len > 0)
					{
                                            len = 0;
                                            len = softsim_uart_send(apdu_buffer, apdu_len);

						if(len > 0)
						{
                                                    delay_ms = ((428*len)/1000) + 1;
                                                    SOFTSIM_ISO7816_TRACE("Sleep %d ms", delay_ms);
                                                    usleep(delay_ms*1000);
						}
						else
						{
							len = 0;
						}
					}
						
	                total_len = 0;
	                counter = 3;
					send_len = len+send_null_len;
                }

				softsim_memset(rx, 0, sizeof(rx));
            }
			else if(0 == len)
			{
				SOFTSIM_ISO7816_TRACE("Read file end!!!");
			}
			else
			{
				SOFTSIM_ISO7816_TRACE("Read file error!!!");
			}
        }

		if(softsim_is_stopping())
		{
			break;
		}
	}

	softsim_thread_exit();
    return NULL;  
}

s32 softsim_thread_entry(void)
{
    s32 ret = 0;
	pthread_t id;
	
	pthread_mutex_lock(&mutex);
	softsim_tid = (unsigned long int)pthread_self();
	
	ret = pthread_create(&id, NULL, (void*)softsim_thread, NULL);
	
	if(ret !=  0)  
	{  
	     SOFTSIM_DEBUG("can't create thread: %s\n",strerror(ret));  
	}
	else
	{
		pthread_detach(id);
		softsim_thread_id = id;
		SOFTSIM_DEBUG("softsim_thread_entry softsim_thread_id = %lu, softsim_tid = %lu", softsim_thread_id, softsim_tid);
	}

	pthread_mutex_unlock(&mutex);
	return ret;
}

#elif defined(__SOFTSIM_T3__)

bool softsim_reset(void)
{
	if(softsim_echo_value(SOFTSIM_PIN, 10) < 0)
	{
		SOFTSIM_DEBUG("write pin fail");
		return false;
	}

	SOFTSIM_DEBUG("echo 10 > /sys/devices/platform/ecard_power_control.0/state");
		
	if(softsim_echo_value(SOFTSIM_PIN, 13) < 0)
	{
		SOFTSIM_DEBUG("write pin fail");
		return false;
	}

	SOFTSIM_DEBUG("echo 13 > /sys/devices/platform/ecard_power_control.0/state");
		
	if(softsim_echo_value(SOFTSIM_PIN, 9) < 0)
	{
		SOFTSIM_DEBUG("write pin fail");
		return false;
	}

	SOFTSIM_DEBUG("echo 9 > /sys/devices/platform/ecard_power_control.0/state");

	return true;
}

bool softsim_reset_gpio_export(void)
{
	if(softsim_export(SOFTSIM_RESET_GPIO_EXPORT, SOFTSIM_RESET_GPIO_NUM) < 0)
	{
		SOFTSIM_DEBUG("Export gpio fail");
		return false;
	}

	return true;
}

bool softsim_reset_event_detect(void)
{
	s32 value = -1;
	
	value = softsim_cat_gpio_value(SOFTSIM_RESET_GPIO_NUM);

	//SOFTSIM_DEBUG("value = %d", value);
	
	if(1 == value)
	{
		return true;
	}

	return false;
}

bool softsim_scan(void)
{	
	if(!softsim_reset())
	{
		SOFTSIM_DEBUG("Reset fail!");
		return false;
	}

	if(0 != softsim_thread_entry())
	{
		SOFTSIM_DEBUG("Creat thread fail!");
		return false;
	}

	return true;
}

void *softsim_thread(void *arg)
{
	u8 apdu_buffer[SOFTSIM_APDU_MAX_LEN] = {0};
	u32 apdu_len = 0;
	u32 delay_ms = 0;
	u16 tsend_len = 0;
	s32 ret = 0;
    s32 len = 0;	/*byte len once*/
    s32 total_len = 0;	/*total length*/
	u8 rx[SOFTSIM_RX_SIZE] = {0x00};
	
	struct timeval timeout;
	fd_set readfds, rfd;

	if((-1) == uart_fd)
	{
		uart_fd = open(SOFTSIM_UART, O_RDWR | O_NOCTTY | O_NONBLOCK | O_SYNC);
	}
	else
	{
		SOFTSIM_DEBUG("UART is ready!");
	}
	
	if(uart_fd < 0)
	{
		SOFTSIM_DEBUG("Open UART error:%s", strerror(errno));
		return NULL;
	}
	
	SOFTSIM_DEBUG("Open UART success!");
	
	softsim_baudrate(uart_fd, 115200);
	
	while(softsim_is_running())
	{
		if(softsim_reset_event_detect())
		{
			if(SOFTSIM_ATR_SUCCESS != softsim_atr_status)
			{
				SOFTSIM_DEBUG("Reset event detected!");
				total_len = 0;
				send_len = 0;
				le = 0;
				softsim_null_len_reset();
				softsim_tpdu_process_state_reset();
				softsim_auth_status_reset(true);
				tcflush(uart_fd, TCIFLUSH);
				tcflush(uart_fd, TCOFLUSH);
				usleep(46*1000);
				send_len = softsim_uart_send(ATR, sizeof(ATR));
				usleep(100*1000);
			}
		}
		else
		{
			softsim_atr_status_update(SOFTSIM_ATR_RESET);
			continue;
		}

		timeout.tv_sec = 0; 
  		timeout.tv_usec=100*1000; //100ms
 		FD_ZERO(&readfds);
  		FD_SET(uart_fd, &readfds);
		
		rfd = readfds;
        ret = select(uart_fd+1, &rfd, NULL, NULL, &timeout);

        if(ret < 0)
        {
            SOFTSIM_DEBUG("select error!");
            return NULL;
        }
				
        if (ret > 0)
        {
        	len = 0;
			softsim_memset(rx, 0x00, sizeof(rx));
            len = read(uart_fd, rx, sizeof(rx));
			
            if((1 == len) && (0x00 == rx[0]) && (SOFTSIM_ATR_SUCCESS != softsim_atr_status))
            {
            	SOFTSIM_ISO7816_TRACE_HEX("UART-RX:", rx, len);
				softsim_atr_status_update(SOFTSIM_ATR_EVENT);
				usleep(46*1000);
            	send_len = softsim_uart_send(ATR, sizeof(ATR));
            	usleep(100*1000);
				softsim_atr_status_update(SOFTSIM_ATR_CHECK);
            }
            else if(len > 0)
            {
            	SOFTSIM_ISO7816_TRACE_HEX("UART-RX:", rx, len);
				SOFTSIM_ISO7816_TRACE("len = %d, send_len = %d, total_len = %d", len, send_len, total_len);

				if(SOFTSIM_ATR_SUCCESS != softsim_atr_status)
				{
					if((0 == softsim_memcmp(ATR, rx, sizeof(ATR))) && (SOFTSIM_TPDU_HEADER_LEN == (len - send_len)))
					{
						SOFTSIM_ISO7816_TRACE("ATR Success!");
						softsim_atr_status_update(SOFTSIM_ATR_SUCCESS);
					}
					else
					{
						continue;
					}
				}				

				total_len += len;

				if((total_len > send_len))
                {
	                softsim_memset(apdu_buffer, 0x00, sizeof(apdu_buffer));
					apdu_len = 0;

					if(softsim_command_is_tpdu_processing())
					{
						if((total_len-send_len == le) && (le != 0))
						{
							if(len >= (total_len-send_len))
							{
	                			softsim_tpdu_command(TGT_SOFTSIM_SLOT_ID, rx+(len-(total_len-send_len)), total_len-send_len, apdu_buffer, &apdu_len);
							}
							else
							{
								softsim_tpdu_command(TGT_SOFTSIM_SLOT_ID, rx, len, apdu_buffer, &apdu_len);
							}

							tsend = false;
						}
						else
						{
							if(!tsend)
							{
								if(total_len == len)
								{
									tsend_len = send_len;
									tsend = true;
								}
								else
								{
									tsend_len = 0;
								}
							}
							else
							{
								tsend_len = 0;
							}
								
							softsim_tpdu_command(TGT_SOFTSIM_SLOT_ID, rx+tsend_len, len-tsend_len, apdu_buffer, &apdu_len);
							continue;
						}
					}
					else
					{
						if(total_len - send_len >= 5)
						{
							if(total_len == len)
							{
								if(5 == len)
								{
									le = rx[4];
								}
								else
								{
									le = rx[len-1];
								}
							}
							else
							{
								le = rx[send_len-(total_len-len) + 4];
							}

							softsim_tpdu_command(TGT_SOFTSIM_SLOT_ID, rx+(len-(total_len-send_len)), total_len-send_len, apdu_buffer, &apdu_len);
						}
						else
						{
							continue;
						}
					}
						
					if((apdu_len > 0) && (SOFTSIM_ATR_SUCCESS == softsim_atr_status))
					{
	                    len = 0;
	                    len = softsim_uart_send(apdu_buffer, apdu_len);

						if(len > 0)
						{
							delay_ms = ((500*len)/1000) + 1;
							SOFTSIM_ISO7816_TRACE("Sleep %d ms", delay_ms);
							usleep(delay_ms*1000);
						}
						else
						{
							len = 0;
						}
					}
						
	                total_len = 0;
					send_len = len+send_null_len;
                }

				softsim_memset(rx, 0, sizeof(rx));
            }
            else if((0 == len))
            {
            	
            }
        }

		if(softsim_is_stopping())
		{
			break;
		}
	}
	
	softsim_thread_exit();
	return NULL;
}

s32 softsim_thread_entry(void)
{
    s32 ret = 0;
	pthread_t id;

	ret = pthread_create(&id, NULL, (void*)softsim_thread, NULL);
	
	if(ret !=  0)  
	{  
	     SOFTSIM_DEBUG("can't create thread: %s\n",strerror(ret));  
	}
	else
	{
		pthread_detach(id);
		softsim_thread_id = id;
	}
	
	return ret;
}

#else

bool softsim_scan(void)
{
	return false;
}

#endif
