// Graphic Transformation/Scaling Program for 256 color
// 640 x 580 window bit map file
// Josh Calahan, November 2010, pthread Version, based on
// B. Earl Wells, October 2010, University of Alabama in Huntsville
// serial version more condusive to MPI Scatter and Gather of display
// arrays. Uses dynamic declarations of display arrays similar
// to the matrix multiply example

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

int numtasks, datasize,
    num_rows, num_cols,
    delta_X, delta_Y,
    delta_X_, delta_Y_,
    X, Y, X_, Y_;

void* display_worker(void* arg) {
   thread_params* p = (thread_params*)arg;
   int rank = p->rank;

   for (int x=X+(rank*datasize); x<X+((rank+1)*datasize); ++x) {
      for (int y=Y; y<Y+delta_Y; ++y) {
         int x_ = (float(delta_X_) / float(delta_X)) * float(x-X) + X_; 
         int y_ = (float(delta_Y_) / float(delta_Y)) * float(y-Y) + Y_;
         if (x_<num_rows && y_<num_cols)  
            Display_out(x_,y_) = Display_in(x,y);
      }
   } 

   return 0;
}

void get_params(int *delta_X,int *delta_Y,int *X,int *Y,
                int *delta_X_,int *delta_Y_,int *X_,int *Y_) {
   int num;
   cout << "Enter number of rows (delta X) of selected area"
        << endl;
   cin >> num;
   *delta_X = num;

   cout << "Enter number of columns (delta Y) of selected area"
        << endl;
   cin >> num;
   *delta_Y = num;

   cout << "Enter the X (row) coordinate of selected area"
        << endl;
   cin >> num;
   *X = num;

   cout << "Enter the Y (column) coordinate of selected area"
        << endl;
   cin >> num;
   *Y = num;

   cout << "Enter number of rows (delta X') of targetted (transformed) area"
        << endl;
   cin >> num;
   *delta_X_ = num;

   cout << "Enter number of columns (delta Y') of targetted (transformed) area"
        << endl;
   cin >> num;
   *delta_Y_ = num;

   cout << "Enter the X (row) coordinate of targetted (transformed) area"
        << endl;
   cin >> num;
   *X_ = num;

   cout << "Enter the Y (column) coordinate of targetted (transformed) area"
        << endl;
   cin >> num;
   *Y_ = num;
}

int main( int argc, char *argv[])  {

   if (argc != 3) {
      cout << "Usage: display [file name (no extension)] n_threads" << endl;
      exit(1);
   }

   char file[80];
   numtasks = atoi(argv[2]);

   // read a bit map file such as shut.bmp to get initial image 
   strcpy(file, argv[1]);
   strcat(file, ".bmp");
   read_256_bmp(file, &num_rows, &num_cols, &display_in);

   // declare display out region
   display_out = new (nothrow) unsigned char [(num_cols+3)*num_rows];
   if(display_out==0) {
     cout <<"ERROR:  Insufficient Memory" << endl;
     exit(1);
   }

   cout << "num_rows=" << num_rows << " num_cols=" << num_cols << endl;

   // clear out output display area
   for (int i=0; i<num_rows; ++i)
      for (int j=0; j<num_cols; ++j)
         Display_out(i,j)=255;

   get_params(&delta_X,&delta_Y, &X,&Y, &delta_X_,&delta_Y_, &X_,&Y_);

   int balance = delta_X%numtasks;

   if (balance) {
      cout << "ERROR: this implementation only works for datasizes that" 
           << " are a multiple of the number of threads" << endl;
      exit(1);
   }

   datasize = delta_X/numtasks;

   pthread_t* threads = new pthread_t[numtasks];
   thread_params* params = new thread_params[numtasks];

   for (int i=0; i<numtasks; ++i) {
      params[i].rank = i;
      pthread_create(threads+i, NULL, display_worker, params+i);
   }

   for (int i=0; i<numtasks; ++i) pthread_join(threads[i], NULL);

   strcpy(file, argv[1]);
   strcat(file, "_pthread.bmp");
   write_256_bmp(file, num_rows,num_cols, display_out);
}
