#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#include <pthread.h>

pthread_mutex_t lock;

typedef struct state {
  int *resource;
  int *available;
  int **max;
  int **allocation;
  int **need;
} State;

// Global variables
int m, n;
State *s = NULL;

// Mutex for access to state.
pthread_mutex_t state_mutex;

/* Random sleep function */
void Sleep(float wait_time_ms)
{
  // add randomness
  wait_time_ms = ((float)rand())*wait_time_ms / (float)RAND_MAX;
  usleep((int) (wait_time_ms * 1e3f)); // convert from ms to us
}

/* Allocate resources in request for process i, only if it 
   results in a safe state and return 1, else return 0 */
int resource_request(int j, int *request)
{
  pthread_mutex_lock(&lock);
  //printf("Process %d: Requesting resources ",j);
  int i;
  for(i=0;i<n;i++) {
    printf("%d, ",request[i]);
  }
  
  if (!allIsSmaller(request,s->need[j],n)) {
      printf("Error process exceeds max claim\n");
  }

  if (allIsSmaller(request,s->available,n)) {
    //printf("taking ");
    for(i=0;i<n;i++) {
      s->available[i] = s->available[i] - request[i];
      s->allocation[j][i] = s->allocation[j][i] + request[i];
      s->need[j][i] = s->need[j][i] - request[i];
    }
    if (safe() == 1) {
      //printf("granted. Availability vector: ");
      for(i=0;i<n;i++) {
        //printf("%d, ",s->available[i]);
      }
      //printf("\n");

      pthread_mutex_unlock(&lock);
      return 1;
    }else{
      for(i=0;i<n;i++) {
        s->available[i] = s->available[i] + request[i];
        s->allocation[j][i] = s->allocation[j][i] - request[i];
        s->need[j][i] = s->need[j][i] + request[i];
      }
    }
  }

  //printf("not given.\n");
  pthread_mutex_unlock(&lock);
  return 0;
}

/* Release the resources in request for process i */
void resource_release(int j, int *request)
{
  pthread_mutex_lock(&lock);
  int i;
  //printf("--release: ");
  for(i=0;i<n;i++) {
    //printf("%d, ",request[i]);
  }
  for(i=0;i<n;i++) {
    s->available[i] = s->available[i] + request[i];
    s->allocation[j][i] = s->allocation[j][i] - request[i];
    s->need[j][i] = s->need[j][i] + request[i];
  }
  //printf("RELEASE Availability vector: ");
  for(i=0;i<n;i++) {
    //printf("%d, ",s->available[i]);
  }
  //printf("\n");
  pthread_mutex_unlock(&lock);
}

/* Generate a request vector */
void generate_request(int i, int *request)
{
  int j, sum = 0;
  while (!sum) {
    for (j = 0;j < n; j++) {
      request[j] = s->need[i][j] * ((double)rand())/ (double)RAND_MAX;
      sum += request[j];
    }
  }
}

/* Generate a release vector */
void generate_release(int i, int *request)
{
  int j, sum = 0;
  while (!sum) {
    for (j = 0;j < n; j++) {
      request[j] = s->allocation[i][j] * ((double)rand())/ (double)RAND_MAX;
      sum += request[j];
    }
  }
}

/* Threads starts here */
void *process_thread(void *param)
{
  /* Process number */
  int i = (int) (long) param, j;
  /* Allocate request vector */
  int *request = malloc(n*sizeof(int));
  while (1) {
    /* Generate request */
    generate_request(i, request);
    while (!resource_request(i, request)) {
      /* Wait */
      Sleep(10);
    }
    /* Generate release */
    generate_release(i, request);
    /* Release resources */
    resource_release(i, request);
    /* Wait */
    Sleep(100);
  }
  free(request);
}

int **allocate_double_matrix(int m, int n)
{
  /* Allocate memory for the elements */
  int *mem = malloc(m * n * sizeof(int));
  /* Allocate memory for the matrix array */
  int **mat = malloc(m * sizeof(int *));
  /* Setup array */
  if (mem != NULL && mat != NULL) {
    int i;
    for (i = 0; i < m; ++i) {
      mat[i] = &mem[i * n];
    }
  } else {
    printf("Out of memory!\n"); exit(-1);
  }
  return mat;
}

void free_double_matrix(int **mat)
{
  /* Free elements */
  free(*mat);
  /* Free matrix */
  free(mat);
}

int main(int argc, char* argv[])
{
  //Init lock
  pthread_mutex_init(&lock,NULL);

  /* Get size of current state as input */
  int i, j;
  printf("Number of processes: ");
  scanf("%d", &m);
  printf("Number of resources: ");
  scanf("%d", &n);

  s = (State *) malloc(sizeof(State));
  s->resource = (int *) malloc(n * sizeof(int));
  s->available = (int *) malloc(n * sizeof(int));
  s->max = allocate_double_matrix(m,n);
  s->allocation = allocate_double_matrix(m,n);
  s->need = allocate_double_matrix(m,n);

  if (s == NULL) { printf("\nYou need to allocate memory for the state!\n"); exit(0); };

  /* Get current state as input */
    printf("Resource vector: ");
  for(i = 0; i < n; i++)
    scanf("%d", &s->resource[i]);
  printf("Enter max matrix: ");
  for(i = 0;i < m; i++)
    for(j = 0;j < n; j++)
      scanf("%d", &s->max[i][j]);
  printf("Enter allocation matrix: ");
  for(i = 0; i < m; i++)
    for(j = 0; j < n; j++) {
      scanf("%d", &s->allocation[i][j]);
    }
  printf("\n");

  /* Calcuate the need matrix */
  for(i = 0; i < m; i++)
    for(j = 0; j < n; j++)
      s->need[i][j] = s->max[i][j]-s->allocation[i][j];

  /* Calcuate the availability vector */
  for(j = 0; j < n; j++) {
    int sum = 0;
    for(i = 0; i < m; i++)
      sum += s->allocation[i][j];
    s->available[j] = s->resource[j] - sum;
  }

  /* Output need matrix and availability vector */
  printf("Need matrix:\n");
  for(i = 0; i < n; i++)
    printf("R%d ", i+1);
  printf("\n");
  for(i = 0; i < m; i++) {
    for(j = 0; j < n; j++)
      printf("%d  ",s->need[i][j]);
    printf("\n");
  }
  printf("Availability vector:\n");
  for(i = 0; i < n; i++)
    printf("R%d ", i+1);
  printf("\n");
  for(j = 0; j < n; j++)
    printf("%d  ",s->available[j]);
  printf("\n");

  /* If initial state is unsafe then terminate with error */
  if (safe() == 0) {
    printf("An error occurred. The state is not safe\n");
    exit(0);
  }

  /* Seed the random number generator */
  struct timeval tv;
  gettimeofday(&tv, NULL);
  srand(tv.tv_usec);

  /* Create m threads */
  pthread_t *tid = malloc(m*sizeof(pthread_t));
  for (i = 0; i < m; i++)
    pthread_create(&tid[i], NULL, process_thread, (void *) (long) i);

  /* Wait for threads to finish */
  pthread_exit(0);
  free(tid);

  free(s->available);
  free(s->resource);
  free_double_matrix(s->max);
  free_double_matrix(s->allocation);
  free_double_matrix(s->need);
  free(s);

  printf("done");
}

/* Check wheter a s is safe or not */
int safe() {
  //1. Copy available to work and init finished
  int *work = (int *) malloc(n * sizeof(int));
  int *finished = (int *) malloc(m * sizeof(int));
  int i;
  int j;
  
  for(i = 0;i<n;i++) {
    work[i] = s->available[i];
  }

  for(j = 0;j<m;j++) {
    finished[j] = 0; 
  }

  //2. Find an index
  j=0;
  while(j<m) {
    if ((finished[j] == 0) && (allIsSmaller(s->need[j],work,n))) {
      //3.
      for(i = 0;i<n;i++) {
        work[i] = work[i] + s->allocation[j][i];
      }
      finished[j] = 1;
      j = 0;
    }else{
      j++;
    }
  }

  //4.
  for(j=0;j<m;j++) {
    if (finished[j] == 0) {
      return 0;
      break;
    }
  }
  return 1;
}

/* Check if all elements of the first array is smaller than the other. */
int allIsSmaller(int a[],int b[],int s) {
  int i;
  for(i=0;i<s;i++) {
    if (a[i] > b[i]) {
      return 0;
    }
  }
  return 1; 
}
