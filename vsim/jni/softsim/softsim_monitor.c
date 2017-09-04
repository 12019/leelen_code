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
 * Filename: softsim_monitor.c
 *
 * Description:
 *
 * Author: tongtong.sun
 * 
 * Date: 2016-08-03
 ******************************************************************************/
 
/*****************************************************************************
* Include																	   
*****************************************************************************/
#include "softsim_include.h"
#if defined(__SOFTSIM_ANDROID__)
#include <dlfcn.h>
#include <stdlib.h>
#include <pthread.h>  
#include <string.h>  
#include <sys/types.h>  
#include <unistd.h>
#include <fcntl.h>
#include "stdio.h"
#include "memory.h"
#include <jni.h>
#include <semaphore.h>
/*****************************************************************************
* Define																	   
*****************************************************************************/
#undef 	SOFTSIM_MD_ID
#undef 	SOFTSIM_TRACE_HEADER
#define SOFTSIM_MD_ID					SOFTSIM_MD_MONITOR
#define SOFTSIM_TRACE_HEADER			"[MONITOR]" 
/*****************************************************************************
* Typedef																	   
*****************************************************************************/

/*****************************************************************************
* Static Local Variable															  
*****************************************************************************/
static JavaVM *gJavaVM;
static jobject gJavaObj;
static pthread_t monitor_threadID;
static bool gIsThreadExit = false ;
/*****************************************************************************
* Global Variable														 
*****************************************************************************/
pthread_mutex_t callback_mutex;
pthread_cond_t callback_cond; 
SoftsimCallbackQueue callbackqueue;
/*****************************************************************************
* Extern Global Variable															
*****************************************************************************/

/*****************************************************************************
* Static Local Function															  
*****************************************************************************/
static int InitCallbackQueue(SoftsimCallbackQueue *myroot);
static int DestoryCallbackQueue(SoftsimCallbackQueue *myroot);
static bool IsCallbackQueueEmpty(SoftsimCallbackQueue *myroot);
static int callbackqueue_mutex_cond_init(void);
static void monitor_thread_exist_check(void);
static void * softsim_monitor_pthread(void * arg);
/*****************************************************************************
* Global Function														
*****************************************************************************/
	 
/*****************************************************************************
* Extern Global Function													  
*****************************************************************************/

/****************************************************************************
* Function
****************************************************************************/
static int InitCallbackQueue(SoftsimCallbackQueue *myroot)
{
	SoftsimCallbackNode* temp = (SoftsimCallbackNode*)malloc(sizeof(SoftsimCallbackNode));
	if(temp == NULL)
	{
		SOFTSIM_DEBUG("InitQueue error");
		return -1;
	}
	temp->next = NULL;
	myroot->head = temp;
	myroot->tail = temp;
	return 0;
}

static int DestoryCallbackQueue(SoftsimCallbackQueue *myroot)
{
	if(NULL == myroot)
	{
		SOFTSIM_DEBUG("myroot is NULL !!!");
		return -1;
	}
	
	while(NULL != myroot->head)
	{
		myroot->tail = myroot->head->next;
		free(myroot->head);
		myroot->head = myroot->tail;
	}
	return 0;
}

static bool IsCallbackQueueEmpty(SoftsimCallbackQueue *myroot)
{
	if(NULL == myroot || myroot->head == myroot->tail)
	{
		return true;
	}
	else
	{
		return false;
	}
}

static int EnCallbackQueue(SoftsimCallbackQueue *myroot,SoftsimCallbackNode callback_node)
{
	if(NULL == myroot)
	{
		SOFTSIM_DEBUG("myroot is NULL !!!");
		return -1;
	}
	
	SoftsimCallbackNode *temp = (SoftsimCallbackNode *)malloc(sizeof(SoftsimCallbackNode));
	if(temp == NULL)
	{
		SOFTSIM_DEBUG("EnCallbackQueue_ERROR");
		return -1;
	}
	softsim_memset(temp,0,sizeof(SoftsimCallbackNode));
	temp->callbacktype = callback_node.callbacktype;
	temp->socket_status = callback_node.socket_status;
	temp->exception = callback_node.exception;
	temp->auth_event.timestamp = callback_node.auth_event.timestamp;
	temp->auth_event.auth_status = callback_node.auth_event.auth_status;
	softsim_memset(temp->auth_event.auth_information, 0x00, sizeof(temp->auth_event.auth_information));
	softsim_strncpy(temp->auth_event.auth_information,callback_node.auth_event.auth_information,\
		            sizeof(temp->auth_event.auth_information));
	temp->auth_event.auth_sequence_num= callback_node.auth_event.auth_sequence_num;
	
	temp->next = NULL;
	myroot->tail->next = temp;
	myroot->tail = temp;
	return 0;
}

static int DeCallbackQueue(SoftsimCallbackQueue *myroot,SoftsimCallbackNode* callback_node)
{
	if(NULL == myroot || NULL == callback_node)
	{
		SOFTSIM_DEBUG("CallbackQueue is NULL !!!");
		return -1;
	}
	
	if(IsCallbackQueueEmpty(myroot))
	{
		SOFTSIM_DEBUG("DeCallbackQueue_ERROR");
		softsim_memset(callback_node,0,sizeof(SoftsimCallbackNode));
		return 0;
	}
	
	SoftsimCallbackNode *temp = myroot->head->next;
	callback_node->callbacktype = temp->callbacktype; 
	callback_node->socket_status = temp->socket_status;
	callback_node->exception = temp->exception;
	callback_node->auth_event.timestamp =temp->auth_event.timestamp;
	callback_node->auth_event.auth_status = temp->auth_event.auth_status;
	softsim_memset(callback_node->auth_event.auth_information, 0x00, sizeof(callback_node->auth_event.auth_information));
	softsim_strncpy(callback_node->auth_event.auth_information,temp->auth_event.auth_information,\
		            sizeof(callback_node->auth_event.auth_information));
	callback_node->auth_event.auth_sequence_num = temp->auth_event.auth_sequence_num;
	if(myroot->head->next == myroot->tail)
		myroot->tail = myroot->head;
	myroot->head->next = temp->next;
	free(temp);
	return 0;
}


static int callbackqueue_mutex_cond_init(void)
{
	s32 ret =-1;
	
	ret = InitCallbackQueue(&callbackqueue);
	if(ret != 0)  
	{  
		SOFTSIM_DEBUG("WQueue Init error");
		return ret;
	}
	
	ret = pthread_cond_init(&callback_cond,NULL);  
	if(ret != 0)  
	{  
		SOFTSIM_DEBUG("pthread_cond Init error");
		return ret;
	} 

	ret = pthread_mutex_init(&callback_mutex,NULL);  
	if(ret != 0)  
	{  
		SOFTSIM_DEBUG("pthread_mutex Init error");
		return ret;
	}
	
	return 0;
}

int socket_status_monitor_Start(JNIEnv *env,jobject object)
{
	s32 ret = -1;
	SOFTSIM_DEBUG("socket_status_monitor_Start");

	(*env)->GetJavaVM(env,&gJavaVM);
	if(gJavaVM == NULL)
	{
		SOFTSIM_ERROR("get gJavaVM error!!!");
		return -1;
	}
	
	gJavaObj = (*env)->NewGlobalRef(env,object);
	if(gJavaObj == NULL)
	{
		SOFTSIM_ERROR("get gJavaObj error!!!");
		return -1;
	}

	monitor_thread_exist_check();

	ret =callbackqueue_mutex_cond_init();
	if(ret != 0)  
	{  
		SOFTSIM_ERROR("structs_mutex_cond_init error");
		return -1;
	}

	gIsThreadExit = true;
	ret = pthread_create(&monitor_threadID,NULL,softsim_monitor_pthread,NULL);
	if(ret != 0)
	{
		gIsThreadExit = false;
		pthread_cond_destroy(&callback_cond);
		pthread_mutex_destroy(&callback_mutex);
		DestoryCallbackQueue(&callbackqueue);
		(*env)->DeleteGlobalRef(env,gJavaObj);
		return ret;
	}
	else
	{
		pthread_detach(monitor_threadID);
		SOFTSIM_DEBUG("monitor_threadID = %lu", monitor_threadID);
	}
	return 0;
}


void socket_status_monitor_Stop(void)
{
	pthread_mutex_lock(&callback_mutex);
	gIsThreadExit = false; 
	pthread_mutex_unlock(&callback_mutex);
	pthread_cond_signal(&callback_cond);
	
}

static void monitor_thread_exist_check(void)
{
	s32 pthread_kill_errorcode;
	pthread_kill_errorcode = pthread_kill(monitor_threadID, 0);
	
	if(ESRCH == pthread_kill_errorcode)
	{
		SOFTSIM_DEBUG("monitor_thread (%lu) did not exist or already quit!",monitor_threadID);
	}
	else if(EINVAL == pthread_kill_errorcode)
	{
		SOFTSIM_DEBUG("monitor_thread signal!");
	}
	else
	{
		SOFTSIM_DEBUG("monitor_thread (%lu) is alive!", monitor_threadID);
		//pthread_cancel(callback_threadID);
		//pthread_exit((void*)0);
	}
}

static void * softsim_monitor_pthread(void * arg)
{
	JNIEnv *env;
	SoftsimCallbackNode callbacknode;
	
	(*gJavaVM)->AttachCurrentThread(gJavaVM,&env,NULL);
	jclass javaClass = NULL;
	jmethodID javaCallback_Status = NULL;
	jmethodID javaCallback_Heart = NULL;
	jmethodID javaCallback_Auth = NULL;
	jstring javaCharst = NULL;

	javaClass = (*env)->GetObjectClass(env,gJavaObj);
	if(javaClass == NULL)
	{
		SOFTSIM_ERROR("Find Class fail !!!");
		goto Release;
	}
	
	javaCallback_Status = (*env)->GetMethodID(env,javaClass,"VSIMLinkStatus","(I)V");
	if(javaCallback_Status == NULL)
	{
		SOFTSIM_ERROR("Find merhodID_Status fail !!!");
		goto Release;
	}

	javaCallback_Heart = (*env)->GetMethodID(env,javaClass,"VSIMExceptionNotify","(I)V");
	if(javaCallback_Heart == NULL)
	{
		SOFTSIM_ERROR("Find merhodID_Heart fail !!!");
		goto Release;
	} 
	
	javaCallback_Auth = (*env)->GetMethodID(env,javaClass,"GetVSIMAuthEvent","(IILjava/lang/String;I)V");
	if(javaCallback_Auth == NULL)
	{
		SOFTSIM_ERROR("Find merhodID_Auth fail !!!");
		goto Release;
	} 

	pthread_mutex_lock(&callback_mutex);
	while(gIsThreadExit)
	{
		while(IsCallbackQueueEmpty(&callbackqueue) && (gIsThreadExit))
		{
			SOFTSIM_DEBUG("Wait");
			pthread_cond_wait(&callback_cond, &callback_mutex);
		}
		if(!gIsThreadExit)
		{
			pthread_mutex_unlock(&callback_mutex);
			SOFTSIM_DEBUG("softsim_monitor_pthread stop"); 
			break;
		}
		if(DeCallbackQueue(&callbackqueue,&callbacknode) < 0)
		{
			pthread_mutex_unlock(&callback_mutex);
			SOFTSIM_DEBUG("CallbackQueue is NULL");
			break;
		}
		pthread_mutex_unlock(&callback_mutex);
		SOFTSIM_DEBUG("callbacktype   = %d",callbacknode.callbacktype);
		switch(callbacknode.callbacktype)
		{
			case SOFTSIM_CALLBACK_SCOKET_STATUS:
				(*env)->CallVoidMethod(env,gJavaObj,javaCallback_Status,(jint)callbacknode.socket_status);
				break;
			case SOFTSIM_CALLBACK_EXCEPTION:
				(*env)->CallVoidMethod(env,gJavaObj,javaCallback_Heart,(jint)callbacknode.exception);
				break;
			case SOFTSIM_CALLBACK_AUTH_EVENT:
				javaCharst = (*env)->NewStringUTF(env,callbacknode.auth_event.auth_information);  
				(*env)->CallVoidMethod(env,gJavaObj,javaCallback_Auth,(jint)callbacknode.auth_event.timestamp,\
									                                  (jint)callbacknode.auth_event.auth_status,javaCharst,\
							                                          (jint)callbacknode.auth_event.auth_sequence_num);
				(*env)->DeleteLocalRef(env, javaCharst);
				break;
			default:break;	
		}
		pthread_mutex_lock(&callback_mutex);
	}
	goto Release;
	
Release:	
	pthread_cond_destroy(&callback_cond);
	pthread_mutex_destroy(&callback_mutex);
	DestoryCallbackQueue(&callbackqueue);
	(*env)->DeleteLocalRef(env, javaClass);
	(*env)->DeleteLocalRef(env, javaCallback_Status);
	(*env)->DeleteLocalRef(env, javaCallback_Heart);
	(*env)->DeleteLocalRef(env, javaCallback_Auth);

	(*env)->DeleteGlobalRef(env,gJavaObj);
	
	(*gJavaVM)->DetachCurrentThread(gJavaVM);
	SOFTSIM_DEBUG("Monitor pthread is end!!!");
	pthread_exit(NULL);
}

void softsim_socket_status_update(SoftsimSocketStatus socket_status)
{
	return;		//2017.03.22 delete
	SOFTSIM_DEBUG("socket_status = %d",socket_status);
	#if !defined(__SOFTSIM_MONITOR__)
		return;
	#endif
	
	SoftsimCallbackNode temp;

	if(!gIsThreadExit)
	{
		return;
	}
	
	pthread_mutex_lock(&callback_mutex);

	softsim_memset(&temp,0,sizeof(SoftsimCallbackNode));
	
	temp.callbacktype = SOFTSIM_CALLBACK_SCOKET_STATUS;
	temp.socket_status = socket_status;
	
	EnCallbackQueue(&callbackqueue,temp);
	pthread_mutex_unlock(&callback_mutex);
	pthread_cond_signal(&callback_cond);
	
}

void softsim_socket_heart_exception(void)
{
	SoftsimCallbackNode temp;

	#if !defined(__SOFTSIM_MONITOR__)
		return;
	#endif
	

	softsim_memset(&temp,0,sizeof(SoftsimCallbackNode));
	temp.callbacktype = SOFTSIM_CALLBACK_EXCEPTION;
	temp.exception = SOFTSIM_EXCEPTION_HEART;
	
	pthread_mutex_lock(&callback_mutex);
	EnCallbackQueue(&callbackqueue,temp);	
	pthread_mutex_unlock(&callback_mutex);
	
	pthread_cond_signal(&callback_cond);
	
}

void softsim_exception_notify(u16 exception_code)
{
	SoftsimCallbackNode temp;

	#if !defined(__SOFTSIM_MONITOR__)
		return;
	#endif

	softsim_memset(&temp,0,sizeof(SoftsimCallbackNode));
	temp.callbacktype = SOFTSIM_CALLBACK_EXCEPTION;
	temp.exception =  exception_code;
	
	pthread_mutex_lock(&callback_mutex);
	EnCallbackQueue(&callbackqueue,temp);	
	pthread_mutex_unlock(&callback_mutex);
	
	pthread_cond_signal(&callback_cond);
}

void softsim_AuthEvent_notify(SoftsimAuthStatus auth_status,const char *auth_information ,int auth_sequence_num)
{
	SoftsimCallbackNode temp;

	#if !defined(__SOFTSIM_MONITOR__)
		return;
	#endif
	softsim_memset(&temp,0,sizeof(SoftsimCallbackNode));
	temp.callbacktype = SOFTSIM_CALLBACK_AUTH_EVENT;
	temp.auth_event.timestamp = time(NULL); 
	temp.auth_event.auth_status =  auth_status;
	softsim_memset(temp.auth_event.auth_information, 0x00, sizeof(temp.auth_event.auth_information));
	softsim_strncpy(temp.auth_event.auth_information, auth_information, sizeof(temp.auth_event.auth_information));
	temp.auth_event.auth_sequence_num = auth_sequence_num;
	pthread_mutex_lock(&callback_mutex);
	EnCallbackQueue(&callbackqueue,temp);	
	pthread_mutex_unlock(&callback_mutex);
	
	pthread_cond_signal(&callback_cond);
}


#elif defined(__SOFTSIM_LINUX__) || defined(__SOFTSIM_ARM_NONE_LINUX_GNUEABI__)

void softsim_socket_status_update(SoftsimSocketStatus socket_status)
{

}

void softsim_socket_heart_exception(void)
{

}

void softsim_exception_notify(u16 exception_code)
{

}

void softsim_AuthEvent_notify(SoftsimAuthStatus auth_status,const char *auth_information ,int auth_sequence_num)
{

}


#else
s32 softsim_jni_init(JNIEnv *env, jobject object)
{

}

#endif	/*__SOFTSIM_ANDROID__*/
