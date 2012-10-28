/******************************************************************************
  main.c

  Implementation of a simple FIFO buffer as a linked list defined in list.h.

 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"
#include <unistd.h> 

// FIFO list;
List *fifo;
List *fifo2;
void *producer(void *param);
void *consumer(void *param);

int main(int argc, char* argv[])
{
  fifo = list_new();

  list_add(fifo, node_new_str("s1"));
  list_add(fifo, node_new_str("s2"));
  list_add(fifo, node_new_str("s3"));

  //Print the full list
  list_print(fifo);

  //Remove and return one element
  Node *n1 = list_remove(fifo);
  printf("%s\n",n1->elm);

  //Print the list with one element removed
  list_print(fifo);

  //Test with 2 processes 
  fifo2 = list_new();

  pthread_attr_t attr; 
  pthread_attr_init(&attr);
  pthread_t tid;
  pthread_t tid2;
  pthread_t tid3;
  pthread_t tid4;
  
  pthread_create(&tid ,&attr,producer,(void*)"a");
  pthread_create(&tid2,&attr,producer,(void*)"b");
  pthread_join(tid,NULL);
  pthread_join(tid2,NULL);

  list_print(fifo2);
  pthread_create(&tid3 ,&attr,consumer,NULL);
  pthread_create(&tid4 ,&attr,consumer,NULL);

  pthread_join(tid3,NULL);
  pthread_join(tid4,NULL);
  
  return 0;
}

void *producer(void *arg)
{ 
  for(int i = 0;i < 5;i++) {
    char str[10];
    sprintf(str,"%s%d",(char *)arg,i);
    list_add(fifo2, node_new_str(str));
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 50000000L;
    nanosleep(&delay,NULL);
  }
  pthread_exit(0);
}

void *consumer(void *arg)
{ 
  for(int i = 0;i < 5;i++) {
    Node *n1 = list_remove(fifo2);
    printf("consumed: %s\n",n1->elm);
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 50000000L;
    nanosleep(&delay,NULL);
  }
  pthread_exit(0);
}
