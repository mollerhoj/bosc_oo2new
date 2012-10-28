/******************************************************************************
  main.c

  Implementation of a simple FIFO buffer as a linked list defined in list.h.

 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"
#include <unistd.h> 
#include <semaphore.h>

// FIFO list;
List *fifo;
void *producer(void *param);
void *consumer(void *param);
int products_max;
int products_created;
int products_removed;

sem_t empty;
sem_t full;
sem_t mutex;
sem_t mutex2;

int main(int argc, char* argv[])
{
  int producers = atoi(argv[1]);
  int consumers = atoi(argv[2]);
  int buffersize = atoi(argv[3]);
  products_max = atoi(argv[4]);

  fifo = list_new();

  sem_init(&empty, 0, buffersize);
  sem_init(&full, 0, 0);
  sem_init(&mutex, 0, 1);

  pthread_attr_t attr; 
  pthread_attr_init(&attr);
  
  pthread_t tid[producers];
  for (int i=0;i<=producers;i++) {
    pthread_create(&tid[i],&attr,producer,(void*)i);
  }
 
  pthread_t tid2[consumers];
  for (int i=0;i<=consumers;i++) {
    pthread_create(&tid2[i],&attr,consumer,(void*)i);
  }
  
  for (int i=0;i<=producers;i++) {
    pthread_join(tid[i],NULL);
  }



  for (int i=0;i<=consumers;i++) {
    pthread_join(tid2[i],NULL);
  }

  list_print(fifo);
  return 0;
}

void randomSleep() {
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 100000000L;
    nanosleep(&delay,NULL);
}

int removeProduct() {
    int removed = 0;
    sem_wait(&mutex2);
    if (products_removed < products_max) {
      products_removed += 1;
      removed = products_removed;
    }
    sem_post(&mutex2);
    return removed;
}

int createProduct() {
    int created = 0;
    sem_wait(&mutex);
    if (products_created < products_max) {
      products_created += 1;
      created = products_created;
    }
    sem_post(&mutex);
    return created;
}

void *consumer(void *arg)
{ 
  while(1) {
    if (removeProduct == 0) {break;}
    sem_wait(&full);
    Node *n1 = list_remove(fifo);
    sem_post(&empty);
    printf("Consumer %d consumed Item_%d (out of %d)\n",(int)arg,(int)n1->elm,products_max);
    randomSleep();
  }
  pthread_exit(0);
}

void *producer(void *arg)
{ 
  while(1) {
    int product = createProduct();
    if (product == 0) {break;}

    sem_wait(&empty);
    list_add(fifo, node_new_int(product));
    sem_post(&full);

    printf("Producer %d produced Item_%d (out of %d)\n",(int)arg,product,products_max);

    randomSleep();
  }
  pthread_exit(0);
}
