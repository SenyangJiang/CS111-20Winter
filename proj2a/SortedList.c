// NAME: Senyang Jiang
// EMAIL: senyangjiang@yahoo.com
// ID: 505111806

#include "SortedList.h"
#include <sched.h>
#include <string.h>
void SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
  if(list == NULL || element == NULL)
    return;
  
  SortedListElement_t *curr = list;
  
  while((curr->next->key != NULL) && (strcmp(element->key,curr->next->key) > 0)) {
    curr = curr->next;
  }

  if(opt_yield & INSERT_YIELD)
    sched_yield();
  
  curr->next->prev = element;
  element->next = curr->next;
  element->prev = curr;
  curr->next = element;
}

int SortedList_delete(SortedListElement_t *element) {
  if(element == NULL)
    return 1;
  if((element->next->prev != element) || (element->prev->next != element)) {
    return 1;
  }

  if(opt_yield & DELETE_YIELD)
    sched_yield();
  
  element->next->prev = element->prev;
  element->prev->next = element->next;
  return 0;
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key) {
  if(list == NULL)
    return NULL;
  
  SortedListElement_t *curr = list->next;
  while(curr->key != NULL) {
    if(strcmp(curr->key,key) == 0){
      return curr;
    }
    if(opt_yield & LOOKUP_YIELD)
      sched_yield();
    
    curr = curr->next;
  }
  return NULL;
}

int SortedList_length(SortedList_t *list) {
  if(list == NULL) {
    return -1;
  }
  
  int len = 0;
  SortedListElement_t *curr = list;
  do{
    if((curr->next->prev != curr) || (curr->prev->next != curr)) {
      return -1;
    }
    len++;
    if(opt_yield & LOOKUP_YIELD)
      sched_yield();
    
    curr = curr->next;
  } while(curr->next->key != NULL);
  return len-1;
}
