/******************************************************************/
/* Summation, Maximum, and Minimum of a list of Numbers Program   */
/* OpenMP version                                                 */
/* September 2009 -- B. Earl Wells -- University of Alabama       */
/*                                    in Huntsville               */
/******************************************************************/
// File: add_num_omp.c
// This program sums a list of numbers, and finds the maximum
// and minimum number in the list. The program is written in a
// manner where the size of list does not have to be an even
// multiple of the number of threads used. The program distributes
// the list of numbers as evenly as possible between the threads
// resulting in the most busy thread having only one more item
// than the least busy thread.

/*

Compilation on altix.asc.edu

      icc -o add_num_omp -openmp add_num_omp.c

  to run 

     ./add_num_omp  
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>            /* openMP headers */
#include <sys/types.h>
#include <sys/time.h>

#define MX_SZ 640001        /* maxium size of list */
#define MX_THREADS 100      /* maximum number of threads */
#define SEED 2397           /* random number seed */
#define MAX_VALUE  100.0    /* maximum size of array A or vector x element */

/* Special Timing Macros -- copied from mpbench */
#define TIMER_CLEAR     (tv1.tv_sec = tv1.tv_usec = tv2.tv_sec = tv2.tv_usec = 0)
#define TIMER_START     gettimeofday(&tv1, (struct timezone*)0)
#define TIMER_ELAPSED   ((tv2.tv_usec-tv1.tv_usec)+((tv2.tv_sec-tv1.tv_sec)*1000000))
#define TIMER_STOP      gettimeofday(&tv2, (struct timezone*)0)
struct timeval tv1,tv2;

/*
Routine to retrieve the size of the numbers array and the
number of threads from the command line or by prompting the 
user for the information
*/
int get_list_size(int argc,char *argv[],int * threads) {
   unsigned int size,thrd;
   if(argc>3) {
      printf("usage:  add_num_OMP [data size] [no. of threads]\n");
      exit(1);
   }
   if (argc<2) {
      printf("Enter number of numbers to be added:\n");
      scanf("%d",&size);
   }
   else {
      size=atoi(argv[1]);
   }
   if (size > MX_SZ) {
      printf("Number of numbers is too large for data structures\n");
      exit(1);
   }
   if (argc<3) {
      printf("Enter number of threads to be used:\n");
      scanf("%d",&thrd);
      *threads = thrd;
   }
   else {
      *threads=atoi(argv[2]);
   }
   return size;
}

/*
Routine that fills the number list with Random Data with values
between 0 and MAX_VALUE
This simulates in some way what might happen if there was a 
single sequential data acquisition source such as a single file
*/
void fill_list(float *numbers,int list_size)
{
   int i;
   void srand48();
   double drand48();
   srand48(SEED);
   for(i=0;i<list_size;i++) {
      numbers[i]=drand48()*MAX_VALUE;
   }
}

/*
Routine that outputs the numbers to be summed list to the screen 
*/
void print_list(float *numbers,int list_size)
{
   int i;
   for(i=0;i<list_size;i++) {
         printf("%e\n",numbers[i]);
   }
}

#define MIN_VAL -3.4028234663852886E+38 // minimal possible value of a fp num
#define MAX_VAL  3.4028234663852886E+38 // minimal possible value of a fp num

/*
MAIN ROUTINE: summation of a number list
*/

int main(int argc, char *argv[]) {

   float g_sum = 0.0;     // global sum variable
   float g_max = MIN_VAL; // global max value variable
   float g_min = MAX_VAL; // global min value variable
   float numbers[MX_SZ];  // number list
   int nm_threads = 1;    // number of threads

// macro that computes the number of items in the list associated
// with a particular thread of the specified tid
// Assumes as evenly as possible distribution of data among the 
// threads (most heavily loaded thread has at most one more data
// item than the least heavily loaded one)
// inputs:
//    tid:      thread id
//    list_size: number of items in global numbers list
//    num_threads: total number of threads
// outputs:
//    if (list_size%num_threads) return list_size/num_threads + 1;
//    else return list_size/num_threads;
#define grp_sz(tid,n,p) (n/p+(tid<(n%p)))

// macro that computes the global starting location of the portion
// of data that thread id, tid, is to compute.
// assumes as evenly as possible distribution of data among the
// threads (most heavily loaded thread has at most one more data
// item than the least heavily loaded one)
// inputs:
//    tid:    thread id
//    list_size: number items in global numbers list
//    num_threads: total number of threads available
//    if (tid<list_size%num_threads) return tid + list_size/num_threads;
//    else return list_size/num_threads + list_size%num_threads;
#define begin_index(tid,n,p) ((tid<(n%p)) ? tid+n/p*tid : (n%p)+n/p*tid)

   int list_size,i;
/* 
   get data size from command line or prompt
   the user for input
*/
   list_size=get_list_size(argc,argv,&nm_threads);

   omp_set_num_threads(nm_threads); // set the number of threads in parallel
                                    // omp pragma

   /* initialize numbers list with random data */ 
   fill_list(numbers,list_size);

   /* print numbers list */
   printf("numbers list =\n");
   print_list(numbers,list_size);
   printf("\n");

   /* 
      sum up elements in the group associated with the
      current process
   */
   TIMER_CLEAR;

   // use high resolution timer
   TIMER_START;

   /* Fork a team of threads giving them their own copies of local */
   /* variables */
   int num_threads, tid,st_index,nm_ele;
   float l_sum,l_max,l_min;
   #pragma omp parallel private(num_threads,tid,st_index,nm_ele,l_sum,l_max,l_min) 
   {
      l_sum=0;       // clear local sum
      l_max=MIN_VAL; // reset local max and min
      l_min=MAX_VAL;
      tid = omp_get_thread_num(); //obtain thread number
      num_threads = omp_get_num_threads();
      nm_ele=grp_sz(tid,list_size,num_threads);
      st_index= begin_index(tid,list_size,num_threads);

      for (i=st_index;i<st_index+nm_ele;i++) {
         l_sum+=numbers[i];
         if (l_max<numbers[i]) l_max = numbers[i];
         if (l_min>numbers[i]) l_min = numbers[i];

      }

      /* Only master thread does this */
      if (tid == 0) {
         printf("Number of threads = %d\n", num_threads);
      }
      #pragma omp critical 
      {
         g_sum += l_sum;
         if (g_max<l_max) g_max = l_max;
         if (g_min>l_min) g_min = l_min;
      }

   }  /* All threads join master thread and disband */

   // stop recording the execution time
   TIMER_STOP;
  
   /* output global sum */
   printf("Sum of numbers is %e\n",g_sum);
   printf("Maximum number in list is %e\n",g_max);
   printf("Minimum number in list is %e\n",g_min);
   printf("time=%e seconds\n\n",(double) TIMER_ELAPSED/1000000.0);
}

