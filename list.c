/******************************************************************************
   list.c

   Implementation of simple linked list defined in list.h.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "list.h"

pthread_mutex_t lock;

/* list_new: return a new list structure */
List *list_new(void)
{
  List *l;

  pthread_mutex_init(&lock,NULL);
  //pthread_mutex_init(&l->mutex,NULL);

  l = (List *) malloc(sizeof(List));
  l->len = 0;

  /* insert root element which should never be removed */
  l->first = l->last = (Node *) malloc(sizeof(Node));
  l->first->elm = NULL;
  l->first->next = NULL;
  return l;
}

/* print function implemented for testing */
void list_print(List *l)
{
  Node* current = l->first;
  while(current != NULL)
  {
    if (current->next == NULL) {
      printf("%s\n",current->elm);
    }else{
      printf("%s :: ",current->elm);
    }
    current = current->next;
  }
} 

/* list_add: add node n to list l as the last element */
void list_add(List *l, Node *n)
{
  //pthread_mutex_lock(&l->mutex);
  //pthread_mutex_lock(&lock);
  l->len += 1;
  l->last->next = n;
  l->last = n;
  l->last->next = NULL;
  //pthread_mutex_unlock(&lock);
  //pthread_mutex_unlock(&l->mutex);
}

/* list_remove: remove and return the first (non-root) element from list l */
Node *list_remove(List *l)
{
  Node *n;
  l->len -=1;
  n = l->first->next;
  l->first->next = l->first->next->next;
  return n;
}

/* node_new: return a new node structure */
Node *node_new(void)
{
  Node *n;
  n = (Node *) malloc(sizeof(Node));
  n->elm = NULL;
  n->next = NULL;
  return n;
}

/* node_new_str: return a new node structure, where elm points to new copy of s */
Node *node_new_str(char *s)
{
  Node *n;
  n = (Node *) malloc(sizeof(Node));
  n->elm = (void *) malloc((strlen(s)+1) * sizeof(char));
  strcpy((char *) n->elm, s);
  n->next = NULL;
  return n;
}
