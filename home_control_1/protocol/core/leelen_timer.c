/********************************************************************************
**
** 文件名:     leelen_timer.c
** 版权所有:   (c) 2015 厦门立林科技有限公司
** 文件描述:   实现定时器功能
**
*********************************************************************************
**             修改历史记录
**===============================================================================
**| 日期       | 作者   |  修改记录
**===============================================================================
**| 2015/09/28 | zzh    |  创建该文件
**
*********************************************************************************/

#include "leelen_comdef.h"
#include "leelen_timer.h"

#define MAX_TIMER            5
#define TIME_TICK            20           /* 单位:ms */



/*
********************************************************************************
* 定义结构体
********************************************************************************
*/
typedef struct {
    bool_t                is_period;      /* 标识是否为周期性定时器; true: 重复, 0: 单次 */
    void                  *p_data;        /* 用户参数 */
    uint32_t              t_set;          /* 设定的超时时间, 单位: ms */
    uint32_t              t_run;          /* 已运行的时间, 单位: ms */
    timer_cb              p_fn;           /* 定时器超时处理函数 */
} timer_control_t;

/*
********************************************************************************
* 定义模块局部变量
********************************************************************************
*/
static timer_control_t    s_tcb[MAX_TIMER];



/*******************************************************************
** 函数名:     timer_handler
** 函数描述:   定时器处理函数
** 参数:       无
** 返回:       无
********************************************************************/
void timer_handler(void)
{
    uint8_t   i;
    uint32_t  sysclk;

    //LEE_LOG("timer_handler");
    for (i = 0; i < MAX_TIMER; i++) {
        if (s_tcb[i].p_fn != 0 && s_tcb[i].t_set > 0) {                 /* 定时器已安装并且也设定有超时时间 */
            s_tcb[i].t_run += TIME_TICK;                                 /* 定时器已运行时间累加 */
            if (s_tcb[i].t_run >= s_tcb[i].t_set) {                     /* 运行时间超过设定的超时时间 */
                if (s_tcb[i].is_period) {                               /* 处理重复周期性定时器 */
                    s_tcb[i].t_run = 0;
                } else {                                                /* 处理单次定时器 */
                    s_tcb[i].t_set = 0;                                 
                }

                s_tcb[i].p_fn();                                        /* 执行超时回调函数 */
            }
        }
    }
}

/*******************************************************************
** 函数名:     install_timer
** 函数描述:   安装一个定时器
** 参数:       [in]  p_fn:      定时器超时处理函数
** 返回:       定时器id; 如安装失败, 则返回0xff
** 注意:       安装完定时器还需调用start_timer才能启动定时器运行
********************************************************************/
uint8_t install_timer(timer_cb p_fn)
{
    uint8_t id;
    
    LEE_ASSERT(p_fn != 0);
    for (id = 0; id < MAX_TIMER; id++) {                            /* 查找一个空定时器 */
        if (s_tcb[id].p_fn == 0) {
            break;
        }
    }
    if (id >= MAX_TIMER) {                                          /* 找不到一个空定时器 */
        LEE_ASSERT(0);
        return 0xff;
    } else {
        s_tcb[id].p_data   = 0;
        s_tcb[id].p_fn     = (timer_cb)p_fn;
        s_tcb[id].t_set    = 0;
        s_tcb[id].t_run    = 0;
    }
    return id;
}

/*******************************************************************
** 函数名:     remove_timer
** 函数描述:   删除一个已创建的定时器
** 参数:       [in]  id:        已创建的一个定时器id
** 返回:       无
********************************************************************/
void remove_timer(uint8_t id)
{
    if (id >= MAX_TIMER) {
        LEE_ASSERT(0);
        return;
    }

    s_tcb[id].p_fn = 0;
}

/*******************************************************************
** 函数名:     start_timer
** 函数描述:   启动定时器
** 参数:       [in]  id:        已创建的一个定时器id
**             [in]  overtime:  定时器超时时间, 单位: ms(注意: 不是tick数)
**             [in]  is_period: true:  重复定时器
**                              false: 单次定时器
** 返回:       无
********************************************************************/
void start_timer(uint8_t id, uint32_t overtime, uint8_t is_period)
{
    LEE_ASSERT(overtime <= 0xffff0000);                                 /* 防止超时时间太大, 导致定时器无法溢出 */

    if (id >= MAX_TIMER || s_tcb[id].p_fn == 0) {
        LEE_ASSERT(0);
    } else {
        s_tcb[id].t_set = overtime;
        s_tcb[id].is_period = is_period;
        s_tcb[id].t_run = 0;
    }
}

/*******************************************************************
** 函数名:     stop_timer
** 函数描述:   停止定时器
** 参数:       [in]  id:        已创建的一个定时器id
** 返回:       无
********************************************************************/
void stop_timer(uint8_t id)
{
    if (id >= MAX_TIMER) {
        LEE_ASSERT(0);
        return;
    }
    s_tcb[id].t_set = 0;
}

/*******************************************************************
** 函数名:     left_timer
** 函数描述:   获取定时器剩余的运行时间
** 参数:       [in]  id:        已创建的一个定时器id
** 返回:       剩余的运行时间, 单位: ms
********************************************************************/
uint32_t left_timer(uint8_t id)
{
    if (id >= MAX_TIMER) {
        LEE_ASSERT(0);
        return 0;
    }
    LEE_ASSERT(s_tcb[id].p_fn != 0);

    if (s_tcb[id].t_set > 0) {
        LEE_ASSERT(s_tcb[id].t_set > s_tcb[id].t_run);
        return s_tcb[id].t_set - s_tcb[id].t_run;
    } else {
        return 0;
    }
}

/*******************************************************************
** 函数名:     timer_is_run
** 函数描述:   判断定时器是否处于运行状态
** 参数:       [in]  id:        已创建的一个定时器id
** 返回:       true:  定时器处于运行状态
**             false: 定时器处于停止状态
********************************************************************/
uint8_t timer_is_run(uint8_t id){
    if (left_timer(id) > 0) {
        return true;
    } else {
        return false;
    }
}

/*******************************************************************
** 函数名:     timer_count_thread
** 函数描述:   时间计时函数
** 参数:       无
** 返回:       无
********************************************************************/
static void *timer_count_thread(void *param)
{
    pthread_detach(pthread_self());

//	int tick_count = 0;
    while (1)
    {
        usleep(TIME_TICK * 1000);
        timer_handler();
    }

    pthread_exit(NULL);
    return NULL;
}


/*******************************************************************
** 函数名:     init_timer
** 函数描述:   模块初始化函数
** 参数:       无
** 返回:       无
********************************************************************/
void init_timer(void)
{
    uint8_t i;
    
    for (i = 0; i < MAX_TIMER; i++) {
        s_tcb[i].p_fn = 0;
    }

	pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, timer_count_thread, NULL) < 0)
    {
        // error
    }
}


