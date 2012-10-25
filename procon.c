#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *producer(void *param);


int main(int argc, char* argv[]) {
  printf("running");

  if (argc != 4) {
    //exit(0);
  }

  
  int producers = atoi(argv[1]);
  int consumers = atoi(argv[2]);
  int buffersize = atoi(argv[3]);
  int products = atoi(argv[4]);

  /* INIT LINKED LIST */

  /*Create producers and consumers */

  pthread_attr_t attr; 
  pthread_attr_init(&attr);

  pthread_t tid[producers];
  for (int i=0;i<=producers;i++) {
    pthread_create(&tid[i],&attr,producer,NULL);
  }

  // Wait for the threads to finish
  for (int i=0;i<=producers;i++) {
    pthread_join(tid[i],NULL);
  }

  printf("exiting\n"); 
}

void *producer(void *arg)
{
  while(true) {
    /*check if there are more items to produce*/
    // produce item
    
    //wait for empty space in buffer
    //wait for mutex
    
    //add to buffer
    
    //signal mutex
    //signal empty
  }

  printf("producer end\n"); 
  pthread_exit(0);
}
