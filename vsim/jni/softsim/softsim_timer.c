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
 * Filename: softsim_timer.c
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-08-13
 ******************************************************************************/

/*****************************************************************************
* Include
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include "softsim_type.h"
#include "softsim_trace.h"
#include "softsim_timer.h"

/*****************************************************************************
* Define
*****************************************************************************/
#undef 	SOFTSIM_MD_ID
#undef 	SOFTSIM_TRACE_HEADER
#define SOFTSIM_MD_ID					SOFTSIM_MD_TIMER
#define SOFTSIM_TRACE_HEADER			"[TIMER]"

#define SOFTSIM_TIMER_INTERVAL_MS	(1000)
#define SOFTSIM_TIMER_ARGUMENT_SIZE	(100)

/*****************************************************************************
* Typedef
*****************************************************************************/
typedef struct
{
    s32 index;
    u16 argument_size;
    u8 argument[SOFTSIM_TIMER_ARGUMENT_SIZE];
}SoftsimTimerArg;

/*****************************************************************************
* Static Local Variable
*****************************************************************************/
static SOFTSIM_TIMER_STRUCT softsim_timer_manager[SOFTSIM_TIMER_ID_MAX]; /*定时器队列*/
static SoftsimTimerArg timer_argument[SOFTSIM_TIMER_ID_MAX];
static pthread_mutex_t timer_add_del_mutex;  /*添加定时器队列添加与删除锁*/

/*****************************************************************************
* Global Variable
*****************************************************************************/

/*****************************************************************************
* Extern Global Variable
*****************************************************************************/

/*****************************************************************************
* Static Local Function
*****************************************************************************/

/*****************************************************************************
* Global Function
*****************************************************************************/

/*****************************************************************************
* Extern Global Function
*****************************************************************************/

/*****************************************************************************
* Function
*****************************************************************************/
/*从队列中释放定时器*/
static s32 softsim_release_timer(s32 index)
{
    pthread_mutex_lock(&timer_add_del_mutex);   /*设置释放定时器加锁*/

    if(index>=0 && index<SOFTSIM_TIMER_ID_MAX)
    {
        softsim_timer_manager[index].used =  false;
    }

    pthread_mutex_unlock(&timer_add_del_mutex);
}

/*从队列中获取空闲定时器*/
static s32 softsim_find_free_timer(void)
{
    s32 index;
    pthread_mutex_lock(&timer_add_del_mutex);   /*设置获取定时器加锁*/

    for(index = 0; index < SOFTSIM_TIMER_ID_MAX; index++)
    {
        if(!softsim_timer_manager[index].used)
        {
            softsim_timer_manager[index].used =  true;
            break;
        }
    }

    pthread_mutex_unlock(&timer_add_del_mutex);
    return index;
}

/*删除定时器数据*/
static void softsim_timer_delete(s32 timer_id)
{
    s32 index;
    pthread_mutex_lock(&timer_add_del_mutex);   /*设置删除加锁*/

    for(index = 0; index < SOFTSIM_TIMER_ID_MAX; index++)
    {
        if((softsim_timer_manager[index].timer_id == timer_id))
        {
            softsim_timer_manager[index].used = false;
            softsim_timer_manager[index].timer_id = SOFTSIM_INVAILD_TIMERID;
            softsim_timer_manager[index].delay = 0;
            softsim_timer_manager[index].arg = NULL;
            softsim_timer_manager[index].handle.callback = NULL;
            softsim_timer_manager[index].handle.ex_callback = NULL;
            break;
        }
    }

    pthread_mutex_unlock(&timer_add_del_mutex);
    return;
}

/*设置定时器数据*/
static void softsim_timer_add(s32 index, s32 timer_id, u32 delay, softsim_timer_call_back callback)
{
    if(timer_id == SOFTSIM_INVAILD_TIMERID)
    {
        return;
    }

    if(softsim_timer_manager[index].used)
    {
        softsim_timer_manager[index].used = true;
        softsim_timer_manager[index].timer_id = timer_id;
        softsim_timer_manager[index].delay = delay;
        softsim_timer_manager[index].arg = NULL;
        softsim_timer_manager[index].handle.callback = callback;
    }
}

/*设置定时器数据*/
static void softsim_timer_ex_add(s32 index, s32 timer_id, u32 delay, softsim_timer_ex_call_back callback, void* arg)
{
    if(SOFTSIM_INVAILD_TIMERID == timer_id)
    {
        return;
    }

    if(softsim_timer_manager[index].used)
    {
        softsim_timer_manager[index].used = true;
        softsim_timer_manager[index].timer_id = timer_id;
        softsim_timer_manager[index].delay = delay;
        softsim_timer_manager[index].arg = arg;
        softsim_timer_manager[index].handle.ex_callback = callback;
    }
}

/*定时器线程*/
static void *softsim_timer_ex_thread(void *arg)
{
    SoftsimTimerArg param;
    SoftsimTimerArg *p = NULL;
    softsim_timer_ex_call_back timer_expiry = NULL;
    p = (SoftsimTimerArg *)arg;

    softsim_memset(&param, 0x00, sizeof(param));
    param.index = p->index;
    param.argument_size = p->argument_size;
    softsim_memcpy(param.argument, p->argument, param.argument_size);

    while(softsim_timer_manager[param.index].used)
    {
        usleep(softsim_timer_manager[param.index].delay * SOFTSIM_TIMER_INTERVAL_MS);

        if(softsim_timer_manager[param.index].timer_id != pthread_self())   //判断本次线程是否为定时器管理线程
        {
            //SOFTSIM_ERROR("softsim_timer_thread  NULL : timer_id = %d pthread_t = %d", softsim_timer_manager[param.index].timer_id,pthread_self());
            return NULL;
        }

        timer_expiry = softsim_timer_manager[param.index].handle.ex_callback;  //执行超时任务
        if(NULL != timer_expiry)
        {
            timer_expiry(softsim_timer_manager[param.index].timer_id, param.argument);
        }
    }

    return NULL;
}

/*定时器线程*/
static void *softsim_timer_thread(void *arg)
{
    u8 timer_id = (u8)arg;
    softsim_timer_call_back timer_expiry = NULL;

    while(softsim_timer_manager[timer_id].used)
    {
        usleep(softsim_timer_manager[timer_id].delay * SOFTSIM_TIMER_INTERVAL_MS);

        if(softsim_timer_manager[timer_id].timer_id != pthread_self())   //判断本次线程是否为定时器管理线程
        {
            //SOFTSIM_ERROR("softsim_timer_thread  NULL : timer_id = %d pthread_t = %d", softsim_timer_manager[timer_id].timer_id,pthread_self());
            return NULL;
        }

        timer_expiry = softsim_timer_manager[timer_id].handle.callback;  //执行超时任务
        if(NULL != timer_expiry)
        {
            timer_expiry();
        }
    }

    return NULL;
}

/*定时器创建*/
s32 softsim_timer_create(softsim_timer_call_back callback, s32 delay)
{
    s32 index;
    s32 timer_id;
    s32 ret = -1;
    pthread_t thread_id;

    index = softsim_find_free_timer();
    if(index >= SOFTSIM_TIMER_ID_MAX)
    {
        SOFTSIM_ERROR("timer create failed, too much timer!");
        return SOFTSIM_INVAILD_TIMERID;
    }

    ret = pthread_create(&thread_id, NULL, softsim_timer_thread, (void *)index);
    if (ret != 0)
    {
        softsim_release_timer(index);
        return SOFTSIM_INVAILD_TIMERID;
    }
    else
    {
        pthread_detach(thread_id);
        softsim_timer_add(index, thread_id, delay, callback);
    }

    return thread_id;
}

/*定时器创建*/
s32 softsim_timer_ex_create(softsim_timer_ex_call_back callback, s32 delay, void *arg, u16 arg_size)
{
    s32 index;
    s32 timer_id;
    s32 ret = -1;
    pthread_t thread_id;

    index = softsim_find_free_timer();
    if(index >= SOFTSIM_TIMER_ID_MAX)
    {
        SOFTSIM_ERROR("timer create failed, too much timer!");
        return SOFTSIM_INVAILD_TIMERID;
    }

    softsim_memset(&timer_argument[index], 0x00, sizeof(SoftsimTimerArg));
    timer_argument[index].index = index;
    timer_argument[index].argument_size = arg_size;
    softsim_memcpy(timer_argument[index].argument, arg, arg_size);

    ret = pthread_create(&thread_id, NULL, softsim_timer_ex_thread, (void *)&timer_argument[index]);

    if (ret != 0)
    {
        softsim_release_timer(index);
        return SOFTSIM_INVAILD_TIMERID;
    }
    else
    {
        pthread_detach(thread_id);
        softsim_timer_ex_add(index, thread_id, delay, callback, arg);
    }

    return thread_id;
}

//关闭定时器
s32 softsim_timer_die(s32 timer_id)
{
    s32 ret;

    if(timer_id == SOFTSIM_INVAILD_TIMERID)
    {
        return -1;
    }

    softsim_timer_delete(timer_id);
    return 0;
}

//初始化定时器子模块
void softsim_timer_init(void)
{
    s32 index;
    pthread_mutex_init(&timer_add_del_mutex,NULL);

    for(index = 0; index < SOFTSIM_TIMER_ID_MAX; index ++)
    {
        softsim_timer_manager[index].used = false;
        softsim_timer_manager[index].timer_id = SOFTSIM_INVAILD_TIMERID;
        softsim_timer_manager[index].delay = 0;
        softsim_timer_manager[index].arg = NULL;
        softsim_timer_manager[index].handle.callback = NULL;
        softsim_timer_manager[index].handle.ex_callback = NULL;
    }
}
