// Mandelbrot Set Program for 256 color
// 640 x 580 window bit map file
// Josh Calahan, November 2010, pthread Version, based on
// B. Earl Wells, October 2010, University of Alabama in Huntsville
// serial version

using namespace std;
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "bmp.hpp"

/*
   WORKER THREAD
*/
struct thread_params {
   int rank;
};

// set to values in the text on page 88
const float real_min = -2.0, real_max = 2.0;
const float imag_min = -2.0, imag_max = 2.0;

float scale_real, scale_imag;

int numtasks, num_rows, num_cols;

void* display_worker(void* arg) {
   thread_params* p = (thread_params*)arg;
   int rank = p->rank;
   int datasize = num_rows/numtasks;
   int balance = rank==numtasks-1 ? num_rows%numtasks : 0;

   for (int i=(rank*datasize); i<((rank+1)*(datasize+balance)); ++i) {
       for (int j=0; j<num_cols; ++j) {
          COMPLEX c;
          c.real = real_min + float(j) * scale_real;
          c.imag = imag_min + float(i) * scale_imag;
          Display_out(i,j) = cal_pixel(c);
       }
   } 

   return 0;
}

int main( int argc, char *argv[])  {

   if (argc != 3) {
      cout << "Usage: mandelbrot [file name of reference file (no extension)]"
           << " n_threads"
           << endl;
      exit(1);
   }

   numtasks = atoi(argv[2]);
   float disp_width, disp_height;
   char file[80];

   // read 256 color bitmap to get color map & other default info
   strcpy(file,argv[1]);
   strcat(file,".bmp");
   read_256_bmp(file, &num_rows,&num_cols,&display_in);

   // declare display out region
   display_out = new (nothrow) unsigned char [(num_cols+3)*num_rows];
   if(display_out==0) {
     cout <<"ERROR:  Insufficient Memory" << endl;
     exit(1);
   }

   cout << "num_rows=" << num_rows << " num_cols=" << num_cols << endl;

   disp_width = num_cols;
   disp_height = num_rows;
   scale_real = (real_max - real_min)/disp_width;
   scale_imag = (imag_max - imag_min)/disp_height;
   
   pthread_t* threads = new pthread_t[numtasks];
   thread_params* params = new thread_params[numtasks];

   for (int i=0; i<numtasks; ++i) {
      params[i].rank = i;
      pthread_create(threads+i, NULL, display_worker, params+i);
   }

   for (int i=0; i<numtasks; ++i) pthread_join(threads[i], NULL);

   write_256_bmp("images/mandelbrot_pthread.bmp", num_rows,num_cols,display_out);
}
