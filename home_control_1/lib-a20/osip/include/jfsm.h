



#ifndef _JFSM_H_
#define _JFSM_H_

#include <exosip_main.h>
#include "eXosip_t_list.h"
typedef struct osip_statemachine
{
  osip_list_t *transitions;
}osip_statemachine_t;


typedef struct _transition_t
{
  int state;
  int type;
  void (*method) (void *, void *);
}transition_t;


void __ict_load_fsm (void);
void __ict_unload_fsm (void);
osip_statemachine_t *__ict_get_fsm (void);



void __ist_load_fsm (void);
void __ist_unload_fsm (void);
osip_statemachine_t *__ist_get_fsm (void);

void __mct_load_fsm (void);
void __mct_unload_fsm (void);
osip_statemachine_t *__mct_get_fsm (void);



void __mst_load_fsm (void);
void __mst_unload_fsm (void);
osip_statemachine_t *__mst_get_fsm (void);



int 
fsm_callmethod(type_t type, state_t state,
                osip_statemachine_t * statemachine, void *sipevent,
                void *transaction);


#define DEFAULT_1S 1000
#define DEFAULT_50MS 100

void __set_state_mod(state_t state,eXosip_element_t *p);

void
mst_terminate_all_mod(void * ict, osip_event_t * evt);

void
mst_terminate_all(void * ict, osip_event_t * evt);
#endif
