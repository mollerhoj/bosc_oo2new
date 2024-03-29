/******************************************************************************
   list.h 

   Header file with definition of a simple linked list.

******************************************************************************/

#ifndef _LIST_H
#define _LIST_H

/* structures */
typedef struct node {
  void *elm; /* use void type for generality; we cast the element's type to void type */
  struct node *next;
} Node;

typedef struct list {
  int len;
  Node *first;
  Node *last;
  pthread_mutex_t mutex;
} List;

/* functions */
List *list_new(void);            /* return a new list structure */
void list_add(List *l, Node *n); /* add node n to list l as the last element */
void list_print(List *l);        /* print the list */
Node *list_remove(List *l);      /* remove and return the first element from list l*/
Node *node_new(void);            /* return a new node structure */
Node *node_new_str(char *s);     /* return a new node structure, where elm points to new copy of string s */

Node *node_new_int(int i);     /* return a new node structure, where elm points to new copy of string s */
#endif
