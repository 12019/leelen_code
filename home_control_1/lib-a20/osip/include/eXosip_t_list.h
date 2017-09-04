#ifndef _EXOSIP_LIST_H_
#define _EXOSIP_LIST_H_

#include "exosip_main.h"



void InitExosipList();

eXosip_element_t *FindItemFromExosipList(unsigned int cid);

void AddItemToExosipList();

void DeleteItemFromExosipList(eXosip_element_t *eXosip_list,unsigned int cid);

eXosip_element_t *FindItemFromExosipListUsedElement(eXosip_element_t *item);

void DeleteItemFromExosipListUsedItem(eXosip_element_t *item);

void DestroyExosipList(eXosip_element_t *eXosip_list);

int LengthofExosipList(eXosip_element_t *eXosip_list);

void DestroyFromThisItem(eXosip_element_t *item);

eXosip_element_t *LastOfExosipList(eXosip_element_t *p);

eXosip_element_t *FindItemFromExosipListUsedIpaddress(char *ipaddress);


#endif