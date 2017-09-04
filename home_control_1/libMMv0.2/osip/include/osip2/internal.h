/*
  The oSIP library implements the Session Initiation Protocol (SIP -rfc3261-)
  Copyright (C) 2001,2002,2003,2004  Aymeric MOIZARD jack@atosc.org
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#include <osip2/osip_port.h>

#ifndef _INTERNAL_H_
#define _INTERNAL_H_

#ifdef OSIP_MT

/* Thread abstraction layer definition */
/* - Unix: native Pthreads. */
#ifndef WIN32
#include <pthread.h>
#include <semaphore.h>

typedef pthread_t osip_thread_t;
typedef pthread_mutex_t osip_mutex_t;
typedef sem_t osip_sem_t;

typedef struct osip_cond
{
  pthread_cond_t cv;
} osip_cond_t;

#endif


#ifdef WIN32
/* Prevent the inclusion of winsock.h */
#define _WINSOCKAPI_
#include <windows.h>
#undef _WINSOCKAPI_
typedef struct
{
  HANDLE h;
  unsigned id;
}osip_thread_t;


typedef struct
{
  HANDLE h;
}
osip_mutex_t;

typedef struct
{
  HANDLE h;
}
osip_sem_t;

typedef struct osip_cond
{
  struct osip_mutex *mut;
  struct osip_sem *sem;
} osip_cond_t;


#endif




#endif /* #ifdef OSIP_MT */


#endif /* #ifndef _INTERNAL_H_ */
