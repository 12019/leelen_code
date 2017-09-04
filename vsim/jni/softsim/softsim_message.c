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
 * Filename: softsim_message.h
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-08-04
 ******************************************************************************/

/*****************************************************************************
* Include																	   
*****************************************************************************/
#include "softsim_include.h"
#include "memory.h"
/*****************************************************************************
* Define																	   
*****************************************************************************/
#undef 	SOFTSIM_MD_ID
#undef 	SOFTSIM_TRACE_HEADER
#define SOFTSIM_MD_ID					SOFTSIM_MD_MESSAGE
#define SOFTSIM_TRACE_HEADER			"[MESSAGE]" 
/*****************************************************************************
* Typedef																	   
*****************************************************************************/

/*****************************************************************************
* Static Local Variable															  
*****************************************************************************/

static SoftsimThreadQueue thread_queue;
static SoftsimMessageQueue message_queue;

static s32 m_msg_id = 0;
static s32 t_msg_id = 0;
static pthread_t m_tid = -1;
static pthread_t t_tid = -1;

static u32 times = 0;
static bool send_flag = false;

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

/****************************************************************************
* Function
****************************************************************************/
#if 0
s32 softsim_thread_queue_init(SoftsimThreadQueue *queue)
{
	SoftsimThreadQNode *pthreadnode = (SoftsimThreadQNode*)malloc(sizeof(SoftsimThreadQNode));

	if(NULL == queue)
	{
		SOFTSIM_DEBUG("Argument invalid!");
		return -1;
	}

	if(NULL == pthreadnode)
	{
		SOFTSIM_DEBUG("Softsim thread queue init fail!");
		return -1;
	}
	
	//pthread_mutex_lock(&queue->mutex);
	pthread_mutex_init (&queue->mutex,NULL);
	queue->front = pthreadnode;
	queue->rear = pthreadnode;
	queue->front->next = NULL;
	//pthread_mutex_unlock(&queue->mutex);

	return 0;
}

s32 softsim_thread_queue_enter(SoftsimThreadQueue *queue, SoftsimThreadInfo *data)
{
	SoftsimThreadQNode *pthreadnode = (SoftsimThreadQNode*)malloc(sizeof(SoftsimThreadQNode));
	
	if(NULL == queue || NULL == data)
	{
		SOFTSIM_DEBUG("Argument invalid!");
		return -1;
	}

	if(NULL == pthreadnode)
	{
		SOFTSIM_DEBUG("Softsim thread queue enter fail!");
		return -1;
	}

	pthreadnode->info.tid = data->tid;
	pthreadnode->info.thread_type = data->thread_type;
	pthreadnode->next = NULL;
	
	pthread_mutex_lock(&queue->mutex);
	queue->rear->next = pthreadnode;
	queue->rear = pthreadnode;	
	pthread_mutex_unlock(&queue->mutex);

	return 0;
}

s32 softsim_thread_queue_exit(SoftsimThreadQueue *queue, SoftsimThreadInfo *info)
{
	SoftsimThreadQNode *p = NULL;
	
	if(NULL == queue || NULL == info)
	{
		SOFTSIM_DEBUG("Argument invalid!");
		return -1;
	}

	if(queue->front == queue->rear)
	{
		return 0;
	}
	
	pthread_mutex_lock(&queue->mutex);
	p = queue->front->next;
	info->tid = p->info.tid;
	info->thread_type = p->info.thread_type;
	queue->front->next = p->next;
	free(queue->front);
	p = NULL;
	pthread_mutex_unlock(&queue->mutex);
	
	return 0;
}

s32 softsim_thread_queue_destroy(SoftsimThreadQueue *queue)
{
	if(NULL == queue)
	{
		SOFTSIM_DEBUG("Argument invalid!");
		return -1;
	}
	
	pthread_mutex_lock(&queue->mutex);
	while(NULL != queue->front)
	{
		queue->rear = queue->front->next;
		free(queue->front);
		queue->front = queue->rear;
	}
	pthread_mutex_unlock(&queue->mutex);

	return 0;
}

s32 softsim_message_queue_init(SoftsimMessageQueue *queue)
{
	SoftsimMessageQNode *pmessagenode = (SoftsimMessageQNode*)malloc(sizeof(SoftsimMessageQNode));

	if(NULL == queue)
	{
		SOFTSIM_DEBUG("Argument invalid!");
		return -1;
	}

	if(NULL == pmessagenode)
	{
		SOFTSIM_DEBUG("Softsim message queue init fail!");
		return -1;
	}
	
	//pthread_mutex_lock(&queue->mutex);
	
	pthread_mutex_init (&queue->mutex,NULL);
	queue->front = pmessagenode;
	queue->rear = pmessagenode;
	queue->front->next = NULL;
	pmessagenode = NULL;
	//pthread_mutex_unlock(&queue->mutex);

	return 0;
}

s32 softsim_message_queue_enter(SoftsimMessageQueue *queue, SoftsimMessageInfo *data)
{
	SoftsimMessageQNode *pmessagenode = (SoftsimMessageQNode*)malloc(sizeof(SoftsimMessageQNode));
	
	if(NULL == queue || NULL == data)
	{
		SOFTSIM_DEBUG("Argument invalid!");
		return -1;
	}

	if(NULL == pmessagenode)
	{
		SOFTSIM_DEBUG("Softsim message queue enter fail!");
		return -1;
	}

	pmessagenode->info.msg_id = data->msg_id;
	pmessagenode->info.thread_type = data->thread_type;
	pmessagenode->next = NULL;
	
	pthread_mutex_lock(&queue->mutex);
	queue->rear->next = pmessagenode;
	queue->rear = pmessagenode;	
	pthread_mutex_unlock(&queue->mutex);
	pmessagenode = NULL;
	
	return 0;
}

s32 softsim_message_queue_exit(SoftsimMessageQueue *queue, SoftsimMessageInfo *info)
{
	SoftsimMessageQNode *p = NULL;
	
	if(NULL == queue || NULL == info)
	{
		SOFTSIM_DEBUG("Argument invalid!");
		return -1;
	}

	if(queue->front == queue->rear)
	{
		return 0;
	}
	
	pthread_mutex_lock(&queue->mutex);
	p = queue->front->next;
	info->msg_id = p->info.msg_id;
	info->thread_type = p->info.thread_type;
	queue->front->next = p->next;
	free(queue->front);
	p = NULL;
	pthread_mutex_unlock(&queue->mutex);
	
	return 0;
}

s32 softsim_message_queue_destroy(SoftsimMessageQueue *queue)
{
	if(NULL == queue)
	{
		SOFTSIM_DEBUG("Argument invalid!");
		return -1;
	}
	
	pthread_mutex_lock(&queue->mutex);
	while(NULL != queue->front)
	{
		queue->rear = queue->front->next;
		free(queue->front);
		queue->front = queue->rear;
	}
	pthread_mutex_unlock(&queue->mutex);

	return 0;
}


void softsim_send_message(u32 type, void *msg)
{
	s32 result = -1;
	SoftsimMessageInfo info;
	s32 msg_id = 0;
	SoftsimMsg *message = (SoftsimMsg*)msg;

	if(NULL == msg)
	{
		return result;
	}
	
	softsim_memset(&info, 0x00, sizeof(info));
	
	switch(type)
	{
		case SOFTSIM_THREAD_MAIN:
			info.msg_id = m_msg_id;
			break;

		case SOFTSIM_THREAD_TEST:
			info.msg_id = t_msg_id;
			break;

		default:
			break;
	}

	info.thread_type = type;

	if(softsim_get_message_id(type, &msg_id) > 0)
	{
		SOFTSIM_DEBUG("msg_id = %d, info.msg_id = %d", msg_id, info.msg_id);
		softsim_message_queue_enter(&message_queue, &info);
		if(msgsnd(msg_id, (void*)message, softsim_strlen(message->msg_buffer)+1, IPC_NOWAIT) < 0)
		{
			SOFTSIM_DEBUG("Send msg error: %s", strerror(errno));
			return;
		}
		SOFTSIM_DEBUG("Send msg success!");
	}
	else
	{
		SOFTSIM_DEBUG("msg_id = %d, info.msg_id = %d", msg_id, info.msg_id);
	}

	
}

s32 softsim_get_message_id(u32 type, s32 *msg_id)
{
	s32 result = -1;
	SoftsimMessageQNode *pmessagenode = NULL;
	SoftsimMessageQNode *ptempnode = NULL;

	if(NULL == msg_id)
	{
		SOFTSIM_DEBUG_MARK;
		return result;
	}

	SOFTSIM_DEBUG("softsim_get_message_id");
	
	//pthread_mutex_lock(&message_queue.mutex);

	pmessagenode = message_queue.front;

	while(1)
	{
	
		SOFTSIM_DEBUG_MARK;
		
		if(NULL == pmessagenode)
		{
			break;
		}
		
		if((type == pmessagenode->info.thread_type))
		{
			*msg_id = pmessagenode->info.msg_id;
			result = 1;
			SOFTSIM_DEBUG_MARK;
			break;
		}
		else
		{
			SOFTSIM_DEBUG_MARK;
			ptempnode = pmessagenode->next;
			pmessagenode = ptempnode;
		}
	}
	
	//pthread_mutex_unlock(&message_queue.mutex);
	
	return result;
}

s32 softsim_creat_message(u32 thread_type)
{	
	s32 msg_id = 0;
	s32 key = 1050 + thread_type;
	
	msg_id = msgget(key, IPC_CREAT|0666);

	if(-1 == msg_id)
	{
		SOFTSIM_DEBUG("Creat msg Error !!!");
	}

	return msg_id;
}

void *softsim_thread_test(void *arg)
{
	SoftsimMsg msg;
	SoftsimMsg trmsg;
	s32 msgid = 0;
	char buffer[100] = {0};
	s32 recv_size = 0;
	SoftsimMessageInfo info;

	softsim_memset(&msg, 0x00, sizeof(msg));
	softsim_strncpy(msg.msg_buffer, "Hello,world!", softsim_strlen("Hello,world!"));
	
	SOFTSIM_DEBUG("softsim_thread_test");
	while(1)
	{
		if(!send_flag)
		{
			SOFTSIM_DEBUG("send message start");
			softsim_send_message(SOFTSIM_THREAD_MAIN, (void*)&msg);
			times++;
			send_flag = true;
			continue;
		}

		if(softsim_get_message_id(SOFTSIM_THREAD_TEST, &msgid) < 0)
		{
			sleep(2);
			continue;
		}
		
		SOFTSIM_DEBUG("softsim_thread_test 000");
		softsim_memset(&trmsg, 0x00, sizeof(trmsg));
		recv_size = msgrcv(msgid, (void*)&trmsg, sizeof(trmsg), SOFTSIM_THREAD_TEST, 0/*MSG_NOERROR*/);
		softsim_message_queue_exit(&message_queue, &info);
		if(recv_size < 0)
		{
			SOFTSIM_DEBUG("Receive message error, %s", strerror(errno));
			continue;
		}
		else
		{
			SOFTSIM_DEBUG("msgid: %d, trmsg.msg_buffer: %s, times:%d", msgid, trmsg.msg_buffer, times);
			if(times < 19)
			{
				softsim_send_message(SOFTSIM_THREAD_MAIN, (void*)&msg);
				times++;
			}
		}
	}
}

void *softsim_thread_main(void *arg)
{
	SoftsimMsg msg;
	SoftsimMsg mrmsg;
	s32 msgid = 0;
	char buffer[100] = {0};
	s32 recv_size = 0;
	SoftsimMessageInfo info;

	softsim_memset(&msg, 0x00, sizeof(msg));
	softsim_strncpy(msg.msg_buffer, "Stay hungary, Stay foolsih!", softsim_strlen("Stay hungary, Stay foolsih!"));
	
	SOFTSIM_DEBUG("softsim_thread_main");
	while(1)
	{
		if(softsim_get_message_id(SOFTSIM_THREAD_MAIN, &msgid) < 0)
		{
			//SOFTSIM_DEBUG_MARK;
			sleep(3);
			continue;
		}
		
		softsim_memset(&mrmsg, 0x00, sizeof(mrmsg));
		recv_size = msgrcv(msgid, (void*)&mrmsg, sizeof(mrmsg), SOFTSIM_THREAD_MAIN, 0/*MSG_NOERROR*/);
		softsim_message_queue_exit(&message_queue, &info);
		if(recv_size < 0)
		{
			SOFTSIM_DEBUG("Receive message error, %s", strerror(errno));
			continue;
		}
		else
		{
			SOFTSIM_DEBUG("msgid: %d, mrmsg.msg_buffer: %s", msgid, mrmsg.msg_buffer);
			softsim_send_message(SOFTSIM_THREAD_TEST, (void*)&msg);
		}
	}
}

void softsim_message_test(void)
{
	s32 ret = 0;
	SoftsimThreadInfo info;
	pthread_attr_t	mThreadAttr;
	pthread_attr_init(&mThreadAttr);  
	pthread_attr_setschedpolicy(&mThreadAttr, SCHED_FIFO);
	pthread_attr_setscope(&mThreadAttr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setstacksize(&mThreadAttr, 10240);
		
	ret = pthread_create(&t_tid, NULL, (void*)softsim_thread_test, NULL);
	
	if(0 == ret)
	{
		pthread_detach(t_tid);
		softsim_memset(&info, 0x00, sizeof(info));
		info.thread_type = SOFTSIM_THREAD_TEST;
		info.tid = t_tid;
		softsim_thread_queue_enter(&thread_queue, &info);
		t_msg_id = softsim_creat_message(SOFTSIM_THREAD_TEST);
		SOFTSIM_DEBUG("t_tid = %lu, t_msg_id = %d", t_tid, t_msg_id);
	}
	else
	{
		SOFTSIM_DEBUG("Can't create thread: %s\n",strerror(ret));  
	}
}

void softsim_message_main(void)
{
	s32 ret = 0;
	SoftsimThreadInfo info;
	pthread_attr_t	mThreadAttr;
	pthread_attr_init(&mThreadAttr);  
	pthread_attr_setschedpolicy(&mThreadAttr, SCHED_FIFO);
	pthread_attr_setscope(&mThreadAttr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setstacksize(&mThreadAttr, 10240);
		
	ret = pthread_create(&m_tid, NULL, (void*)softsim_thread_main, NULL);
	
	SOFTSIM_DEBUG("softsim_message_main ret = %d", ret);

	if(0 == ret)
	{
		pthread_detach(m_tid);
		softsim_memset(&info, 0x00, sizeof(info));
		info.thread_type = SOFTSIM_THREAD_MAIN;
		info.tid = m_tid;
		softsim_thread_queue_enter(&thread_queue, &info);
		m_msg_id = softsim_creat_message(SOFTSIM_THREAD_MAIN);
		SOFTSIM_DEBUG("m_tid = %lu, m_msg_id = %d", m_tid, m_msg_id);
 	}
 	else
 	{
		SOFTSIM_DEBUG("Can't create thread: %s\n",strerror(ret));  
	}

}

void softsim_message_init(void)
{
	softsim_thread_queue_init(&thread_queue);
	softsim_message_queue_init(&message_queue);
}
#endif

void softsim_msg_test(void)
{
#if 0
	SOFTSIM_DEBUG("softsim message!!");
	times = 0;
	softsim_message_init();
	
	softsim_message_main();
	
	softsim_message_test();
#endif
}

