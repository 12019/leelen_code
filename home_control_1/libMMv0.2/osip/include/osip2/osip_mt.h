#ifndef _OSIP_MT_H_
#define _OSIP_MT_H_

#ifdef OSIP_MT

#ifdef ENABLE_MPATROL
#include <mpatrol.h>
#endif

#include <stdio.h>
#include <errno.h>

/**
 * @file osip_mt.h
 * @brief oSIP Thread, Mutex and Semaphore definitions
 *
 * Those methods are only available if the library is compile
 * in multi threaded mode. This is the default for oSIP.
 */


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Structure for referencing a thread
 * @struct osip_thread
 */
  struct osip_thread;

/**
 * Allocate (or initialise if a thread address is given)
 * @param stacksize The stack size of the thread. (20000 is a good value)
 * @param func The method where the thread start.
 * @param arg A pointer on the argument given to the method 'func'.
 */
  struct osip_thread *osip_thread_create (void *(*func) (void *), void *arg);

/**
 * Join a thread.
 * @param thread The thread to join.
 */
  int osip_thread_join (struct osip_thread *thread);

/**
 * Set the priority of a thread. (NOT IMPLEMENTED ON ALL SYSTEMS)
 * @param thread The thread to work on.
 * @param priority The priority value to set.
 */
  int osip_thread_set_priority (struct osip_thread *thread, int priority);
/**
 * Exit from a thread.
 */
  void osip_thread_exit (void);

#ifdef __cplusplus
}
#endif

/** @} */

/**
 * @defgroup oSIP_SEMA oSIP semaphore definitions
 * @ingroup osip2_port
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Structure for referencing a semaphore element.
 * @struct osip_sem
 */
  struct osip_sem;

/**
 * Allocate and Initialise a semaphore.
 * @param value The initial value for the semaphore.
 */
  struct osip_sem *osip_sem_init (unsigned int value);
/**
 * Destroy a semaphore.
 * @param sem The semaphore to destroy.
 */
  int osip_sem_destroy (struct osip_sem *sem);
/**
 * Post operation on a semaphore.
 * @param sem The semaphore to destroy.
 */
  int osip_sem_post (struct osip_sem *sem);
/**
 * Wait operation on a semaphore.
 * NOTE: this call will block if the semaphore is at 0.
 * @param sem The semaphore to destroy.
 */
  int osip_sem_wait (struct osip_sem *sem);
/**
 * Wait operation on a semaphore.
 * NOTE: if the semaphore is at 0, this call won't block.
 * @param sem The semaphore to destroy.
 */
  int osip_sem_trywait (struct osip_sem *sem);


#ifdef __cplusplus
}
#endif

/** @} */

/**
 * @defgroup oSIP_MUTEX oSIP mutex definitions
 * @ingroup osip2_port
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Structure for referencing a mutex element.
 * @struct osip_mutex
 */
  struct osip_mutex;

/**
 * Allocate and Initialise a mutex.
 */
  struct osip_mutex *osip_mutex_init (void);
/**
 * Destroy the mutex.
 * @param mut The mutex to destroy.
 */
  void osip_mutex_destroy (struct osip_mutex *mut);
/**
 * Lock the mutex.
 * @param mut The mutex to lock.
 */
  int osip_mutex_lock (struct osip_mutex *mut);
/**
 * Unlock the mutex.
 * @param mut The mutex to unlock.
 */
  int osip_mutex_unlock (struct osip_mutex *mut);

#ifdef __cplusplus
}
#endif

/** @} */

#endif                          /* OSIP_MT */

#endif                          /* end of _THREAD_H_ */
