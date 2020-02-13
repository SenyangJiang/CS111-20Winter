#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <sched.h>

int num_threads = 1;
int num_iter = 1;
long long counter = 0;
pthread_mutex_t mutexadd;
volatile int lock = 0;

int opt_yield = 0;
int sync_flag = 0;
char sync_opt;

void add(long long *pointer, long long value) {
  long long sum = *pointer + value;
  if (opt_yield)
    sched_yield();
  *pointer = sum;
}

void add_m(long long *pointer, long long value) {
  pthread_mutex_lock (&mutexadd);
  long long sum = *pointer + value;
  if (opt_yield)
    sched_yield();
  *pointer = sum;
  pthread_mutex_unlock (&mutexadd);
}

void add_s(long long *pointer, long long value) {
  while (__sync_lock_test_and_set(&lock, 1));
  long long sum = *pointer + value;
  if (opt_yield)
    sched_yield();
  *pointer = sum;
  __sync_lock_release(&lock);
}

void add_c(long long *pointer, long long value) {
  long long old, new;
  do{
    old = *pointer;
    new = old + value;
    if (opt_yield)
      sched_yield();
  } while(__sync_val_compare_and_swap(pointer, old, new) != old);
}

void* add_wrapper(void *arg) {
  arg = arg;
  int i;
  void (*add_func)(long long*, long long);
  if(sync_flag){
    if(sync_opt == 'm')
      add_func = add_m;
    else if(sync_opt == 's')
      add_func = add_s;
    else if(sync_opt == 'c')
      add_func = add_c;
  }
  else {
    add_func = add;
  }
  for(i = 0; i < num_iter; i++) {
    add_func(&counter, 1);
  }
  for(i = 0; i < num_iter; i++) {
    add_func(&counter, -1);
  }
  pthread_exit(NULL);
}

int main(int argc, char **argv)
{
  int opt;
  struct option long_options[] = {{"threads", required_argument, NULL, 't'},
				  {"iterations", required_argument, NULL, 'i'},
				  {"yield", no_argument, NULL, 'y'},
				  {"sync", required_argument, NULL, 's'},
				  {0, 0, 0, 0}};
  while(1)
    {
      opt = getopt_long(argc, argv, "+:", long_options, NULL);
      if(opt == -1)
	break;
      
      switch(opt)
	{
	case 'y':
	  opt_yield = 1;
	  break;
	case 't':
	  num_threads = atoi(optarg);
	  break;
	case 'i':
	  num_iter = atoi(optarg);
	  break;
	case 's':
	  sync_flag = 1;
	  sync_opt = optarg[0];
	  if(sync_opt != 'm' && sync_opt != 's' && sync_opt != 'c') {
	    fprintf(stderr, "invalid sync option\n");
	    exit(1);
	  }
	  break;
	case '?':
	  fprintf(stderr, "Unrecognized argument: %s\n", argv[optind-1]);
	  exit(1);
	  break;
	case ':':
	  fprintf(stderr, "Missing argument for %s\n", argv[optind-1]);
	  exit(1);
	  break;
	}
    }
  
  // check if there is an element that is not an option
  if(optind != argc)
    {
      fprintf(stderr, "Unrecognized argument: %s\n", argv[optind]);
      exit(1);
    }

  // initialize mutex if required
  if(sync_flag & (sync_opt == 'm')) {
    if(pthread_mutex_init(&mutexadd, NULL) != 0) {
      fprintf(stderr, "Fail to initialize mutex\n");
      exit(1);
    }
  }

  // note the starting time for the run
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  
  // create threads
  int t;
  int rc;
  pthread_t* threads = malloc(sizeof(pthread_t)*num_threads);
  if(threads == NULL) {
    fprintf(stderr, "Fail to allocate space for thread ID array\n");
    exit(1);
  }
  for(t = 0; t < num_threads; t++) {
    rc = pthread_create(&threads[t], NULL, add_wrapper, NULL);
    if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(1);
    }
  }
  // wait for all threads to complete
  for(t = 0; t < num_threads; t++) {
    rc = pthread_join(threads[t], NULL);
    if (rc) {
      printf("ERROR; return code from pthread_join() is %d\n", rc);
      exit(1);
    }
  }
  
  // note the ending time for the run
  clock_gettime(CLOCK_MONOTONIC, &end);

  // prints to stdout a comma-separated-value (CSV) record
  long total_time = (end.tv_sec - start.tv_sec)*1000000000 + (end.tv_nsec - start.tv_nsec);
  long operations = num_threads*num_iter*2;
  if(opt_yield) {
    fprintf(stdout, "add-yield");
  }
  else {
    fprintf(stdout, "add");
  }

  if(sync_flag) {
    fprintf(stdout, "-%c,", sync_opt);
  }
  else {
    fprintf(stdout, "-none,");
  }
  fprintf(stdout, "%d,%d,%ld,%ld,%ld,%lld\n", num_threads, num_iter, operations, total_time, total_time/operations, counter);

  // cleaning up and exit
  if(sync_flag & (sync_opt == 'm')) {
    if(pthread_mutex_destroy(&mutexadd)) {
      fprintf(stderr, "Fail to destory mutex\n");
      exit(1);
    }
  }
  free(threads);
  exit(0);
}
