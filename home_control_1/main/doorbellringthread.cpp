#include "doorbellringthread.h"

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <QStandardItemModel>
#include "maindialog.h"
#include "ui_maindialog.h"

extern MainDialog *g_pMainDlg;
extern char g_strLphoneRingFile[5][50];

DoorBellRingThread::DoorBellRingThread(QObject *parent) :
    QThread(parent)
{
    m_doorbellring = 0;
}

#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/user.h>

typedef struct
{
    unsigned char enable;
    unsigned int active_state;
    unsigned int duty_ns;
    unsigned int period_ns;
}__pwm_info_t;

// 以4分音符为1拍
//#define TEMPO   1000000
#define TEMPO   125000 
#define _1      TEMPO*4     //全音符
#define _1d     TEMPO*6     //附点全音符
#define _2      TEMPO*2     //2音符
#define _2d     TEMPO*3     //附点2音符
#define _4      TEMPO*1     //4分音符
#define _4d     TEMPO*3/2   //附点4分音符
#define _8      TEMPO*1/2   //8分音符
#define _8d     TEMPO*3/4   //附点8音符
#define _16     TEMPO*1/4   //16分音符
#define _16d    TEMPO*3/8   //附点16分音符
#define _32     TEMPO*1/8   //32分音符

/* 低音 */
#define _1DO    262
#define _1RE    294
#define _1MI    330
#define _1FA    349
#define _1SO    392
#define _1LA    440
#define _1TI    494

/* 中音 */
#define _DO    	523
#define _RE    	587
#define _MI    	659
#define _FA    	698
#define _SO    	784
#define _LA    	880
#define _TI    	988

/* 高音 */
#define _DO1    1047
#define _RE1    1175
#define _MI1    1319
#define _FA1    1397
#define _SO1    1568
#define _LA1    1760
#define _TI1    1976

/* 休止符 */
#define _ZERO   0

const unsigned int RING1[] =
{
_FA1,_LA,_FA1,_LA,_FA1,_LA,_FA1,_LA,_ZERO,_ZERO,
_FA1,_LA,_FA1,_LA,_FA1,_LA,_FA1,_LA,_ZERO,_ZERO,
};

const unsigned int RING1_L[] =
{
_8,_4,_8,_4,_8,_4,_8,_4,_1,_1,
_8,_4,_8,_4,_8,_4,_8,_4,_1,_1,
};

const unsigned int RING2[] =
{
_RE1,_LA,_RE1,_LA,_RE1,_LA,_RE1,_LA,_ZERO,_ZERO,
_RE1,_LA,_RE1,_LA,_RE1,_LA,_RE1,_LA,_ZERO,_ZERO,
};

const unsigned int RING2_L[] =
{
_4,_4,_4,_4,_4,_4,_4,_4,_1,_1,
_4,_4,_4,_4,_4,_4,_4,_4,_1,_1,
};

void DoorBellRingThread::start_pwm_export(void)
{
    m_doorbellring = 1;
}

void DoorBellRingThread::stop_pwm_export(void)
{
    m_doorbellring = 0;
}

void DoorBellRingThread::run()
{
    int ret;
    int pwm_node,i,j;
    unsigned int freq;
    __pwm_info_t info;

    m_bRun = true;
    info.enable = 0;
    pwm_node = open("/dev/pwm", O_RDWR, 0);

    int num;

    while(m_bRun)
    {
	if 	( strcmp(g_strLphoneRingFile[2],"rings/ring1") == 0 )
    		num = sizeof(RING1)/sizeof(RING1[0]);
	else if ( strcmp(g_strLphoneRingFile[2],"rings/ring2") == 0 )
    		num = sizeof(RING2)/sizeof(RING2[0]);

        if (m_doorbellring)
        {
            for(i = 0; i < num; i++)
            {
                if (!m_doorbellring)
                    break;
                if 	( strcmp(g_strLphoneRingFile[2],"rings/ring1") == 0 )
                	freq = RING1[i];
                else if ( strcmp(g_strLphoneRingFile[2],"rings/ring2") == 0 )
                	freq = RING2[i];

                if ( freq == _ZERO )
                {
                    info.enable = 0;
                    ioctl(pwm_node,100,(void *)&info);
                    if 	( strcmp(g_strLphoneRingFile[2],"rings/ring1") == 0 )
                                usleep(RING1_L[i]);
                    else if ( strcmp(g_strLphoneRingFile[2],"rings/ring2") == 0 )
                                usleep(RING2_L[i]);

                    continue;
                }

                info.period_ns = 1000000000/freq;
                info.active_state = 1;
                info.enable = 1;
                info.duty_ns = info.period_ns/2;
                ret = write(pwm_node,(void *)&info,sizeof(info));
		if 	( strcmp(g_strLphoneRingFile[2],"rings/ring1") == 0 )
                	usleep(RING1_L[i]);
		else if ( strcmp(g_strLphoneRingFile[2],"rings/ring2") == 0 )
			usleep(RING2_L[i]);
            }
        }
        else
        {
            if (info.enable)
            {
                info.enable = 0;
                ioctl(pwm_node,100,(void *)&info);
            }
            usleep(1000000);
        }
    }
}
