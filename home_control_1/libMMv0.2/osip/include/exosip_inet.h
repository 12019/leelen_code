#ifndef _EXOSIP_INET_H_
#define _EXOSIP_INET_H_

#include "exosip_main.h"
int eXosip_read_message   ( int max_message_nb, int sec_max, int usec_max );

void eXosipElement_inet_init(eXosip_element_t *p);

void eXosipElement_inet_uninit(eXosip_element_t *p);
#endif
