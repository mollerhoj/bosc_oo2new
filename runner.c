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

pthread_mutex_t lock;

int main(int argc, char* argv[])
{
  int n = atoi(argv[1]);
  int thread_no = atoi(argv[2]);
  
  if (thread_no < 1) {
    printf("Error: There must be atleast one thread");
    exit(0);
  }

  if (n % thread_no != 0) {
    printf("Error: n must be divisible with thread_no");
    exit(0);
  }

  pthread_mutex_init(&lock,NULL);

  pthread_attr_t attr;
  pthread_attr_init(&attr);
 
  pthread_t *tid = malloc(sizeof(pthread_t)*thread_no);
  struct thread_param *tp = malloc(sizeof(struct thread_param));

  int diffsize = n / thread_no;
  for(int i = 0; i < thread_no;i++) {
    tp[i].min = 1+i*diffsize;
    tp[i].max = (i+1)*diffsize; 
    pthread_create(&tid[i],&attr,runner,&tp[i]);
  }

  for(int i = 0; i < thread_no;i++) {
    pthread_join(tid[i],NULL);
  }
  
  printf("sum = %f\n",sum);

  free(tid);
  free(tp);
}

/* The thread will begin control in this function */
void *runner(void *arg)
{
  struct thread_param *tp = arg;
  double value = 0;
  for (int i=tp->min;i <= tp->max;i++) {
    value +=sqrt(i);
  }
  pthread_mutex_lock(&lock);
  sum += value;
  pthread_mutex_unlock(&lock);
  pthread_exit(0);
}
