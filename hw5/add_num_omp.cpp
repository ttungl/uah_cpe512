/*****************************************************************/
/* Summation, Maximum, and Minimum of a list of Numbers Program  */
/* OpenMP version                                                */
/* October 2010 -- B. Earl Wells -- University of Alabama        */
/*                                    in Huntsville              */
/*****************************************************************/
// File: add_num_omp.cpp
// This program sums a list of numbers, and finds the maximum
// and minimum number in the list. 
// This program is written in a manner that uses the #pragma omp for
// which automatically divides the the number of index interations
// as evenly as possible when the size of list is not an even
// multiple of the number of threads used. The index variable i
// becomes a private variable that has the specified static range.
// The number of threads or the thread id is not explicity used by
// the programmer.

/*

Compilation on altix.asc.edu

      icc -o add_num_omp -openmp add_num_omp.cpp

  to run 

     ./add_num_omp  
*/

using namespace std;
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <omp.h>            /* openMP headers */
#include <sys/types.h>
#include <sys/time.h>

#define MX_THREADS 100       /* maximum number of threads */
#define SEED 2397           /* random number seed */
#define MAX_VALUE  100.0    /* maximum value of any number in list */
#define MIN_VALUE  0.0      /* minimum value of any number in list */

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

   string input = "";
   unsigned int size,thrd;

   // ERROR if too many command line arguments
   if(argc>3) {
      cout << "usage:  add_num_omp [data size] [no. of threads]" 
           << endl;
      exit(1);
   }
   if (argc<2) {
      while (1) {
         cout << "Enter the number of numbers to be added:" << endl;
         getline(cin, input);
         stringstream myStream(input);
         if (myStream >> size) break;
            cout << "Invalid Input" << endl << endl;
      }
   }
   else {
      size=atoi(argv[1]);
   }
   if (argc<3) {
      while (1) {
         cout << "Enter number of threads to be used:" << endl;
         getline(cin, input);
         stringstream myStream(input);
         if (myStream >> thrd) break;
            cout << "Invalid Input" << endl << endl;
      }
      *threads = thrd;
   }
   else {
      *threads=atoi(argv[2]);
   }
   return size;
}

/*
Routine that fills the number matrix with Random Data with values
between MIN_VALUE and MAX_VALUE
This simulates in some way what might happen if there was a 
single sequential data acquisition source such as a single file
*/
void fill_matrix(double *numbers,int data_size)
{
   int i;
   srand48(SEED);
   for(i=0;i<data_size;i++) {
      numbers[i]=drand48()*(MAX_VALUE-MIN_VALUE) + MIN_VALUE;
      //to verify may want to initialize the numbers array with a pattern
      //that has a known answer such as the sum of numbers from 0 to N-1
      // The result of that summation is (N+1)*N/2!!
      // numbers[i]=i; // to do so uncomment this line
   }
}

/*
Routine that outputs the numbers to be summed list to the screen 
*/
void print_list(double *numbers,int list_size)
{
   int i;
   for(i=0;i<list_size;i++) {
         cout << numbers[i] << endl;
   }
}
#define MIN_VAL -numeric_limits<double>::max() // minimal recorded value of a num
#define MAX_VAL numeric_limits<double>::max()// maxmum recorded value of a num

/*
MAIN ROUTINE: summation of a number list
*/

int main(int argc, char *argv[]) {

   double g_sum = 0.0;     // global sum variable
   double g_max = MIN_VAL; // global max value variable
   double g_min = MAX_VAL; // global min value variable
   double *numbers;        // number list
   int nm_threads = 1;    // number of threads
   int list_size,i;

   // get data size from command line or prompt
   // the user for input
   list_size=get_list_size(argc,argv,&nm_threads);

   omp_set_num_threads(nm_threads); // set the number of threads in parallel
                                    // omp pragma

   // dynamically allocate space for array of  numbers
   numbers = new (nothrow) double[list_size];
   if (numbers==0) { // check for null pointer
      cout << "Memory Allocation Error" << endl;
      exit(1); // exit the program
   }

   /* initialize numbers list with random data */ 
   fill_matrix(numbers,list_size);
 
   /* print numbers list */
   cout << "numbers list =" << endl;
   print_list(numbers,list_size);
   cout << endl; 


   // use high resolution timer
   TIMER_CLEAR;
   TIMER_START;

   /* Fork a team of threads giving them their own copies of local */
   /* variables */
   double l_sum,l_max,l_min;

   cout << "Number of threads = " << nm_threads << endl;
   #pragma omp parallel private(l_sum,l_max,l_min) 
   {
      l_sum=0;       // clear local sum
      l_max=MIN_VAL; // reset local max and min
      l_min=MAX_VAL;

      #pragma omp for
      for (i=0;i<list_size;i++) {
         l_sum+=numbers[i];
         if (l_max<numbers[i]) l_max = numbers[i];
         if (l_min>numbers[i]) l_min = numbers[i];

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
   cout << "Sum of numbers is " << g_sum << endl;
   cout << "Maximum number in list is " << g_max << endl;
   cout << "Minimum number in list is " << g_min << endl;
   cout << "time=" << (double) TIMER_ELAPSED/1000000.0
        << " seconds" << endl << endl;
}

