#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double sum; /* this data is shared by the threads */
void *runner(void *param); /* the thread */

struct thread_param {
  int min;
  int max;
};

int main(int argc, char* argv[])
{
  pthread_attr_t attr; /* sef of thread attributes */
  pthread_attr_init(&attr);
  int n = atoi(argv[1]);
  int thread_no = atoi(argv[2]);
  int diffsize = n / thread_no;
  pthread_t tid; /* the thread identifier */

  for(int i = 1; i <= thread_no;i++) {
    printf("i: %d\n",i);
    struct thread_param tp[i];
    tp[i].min = 1+(i-1)*diffsize;
    tp[i].max = 1+i*diffsize; 
    pthread_create(&tid,&attr,runner,&tp[i]);
    pthread_join(tid,NULL);
  }

  /* get the default attributes */
  /* create the thread */
  /* wait for the thread to exit */

  printf("sum = %f\n",sum);
}

/* The thread will begin control in this function */
void *runner(void *arg)
{
  struct thread_param *tp = arg;
  for (int i=tp->min;i < tp->max;i++) {
    sum +=sqrt(i); //Requires that this is atomic
  }
  pthread_exit(0);
}
