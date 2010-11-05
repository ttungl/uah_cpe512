// Graphic Transformation/Scaling Program for 256 color
// 640 x 580 window bit map file
// Josh Calahan, November 2010, MPI Version, based on
// B. Earl Wells, October 2010, University of Alabama in Huntsville
// serial version more condusive to MPI Scatter and Gather of display
// arrays. Uses dynamic declarations of display arrays similar
// to the matrix multiply example

using namespace std;
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "bmp.hpp"

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

   char file[80];

   int numtasks, rank,
       num_rows, num_cols,
       delta_X, delta_Y,
       delta_X_, delta_Y_,
       X, Y,
       X_, Y_;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   if (argc != 2) {
      if (0==rank) cout << "Usage: display [file name (no extension)]" << endl;
      MPI_Finalize();
      exit(1);
   }

   // read a bit map file such as shut.bmp to get initial image 
   strcpy(file, argv[1]);
   strcat(file, ".bmp");
   read_256_bmp(file, &num_rows, &num_cols, &display_in);

   // declare display out region
   display_out = new (nothrow) unsigned char [(num_cols+3)*num_rows];
   if(display_out==0) {
     if (0==rank) cout <<"ERROR:  Insufficient Memory" << endl;
     MPI_Finalize();
     exit(1);
   }

   if (0==rank) cout << "num_rows=" << num_rows << " num_cols=" << num_cols << endl;

   // clear out output display area
   for (int i=0; i<num_rows; ++i)
      for (int j=0; j<num_cols; ++j)
         Display_out(i,j)=255;

   if (0==rank) get_params(&delta_X,&delta_Y, &X,&Y, &delta_X_,&delta_Y_, &X_,&Y_);
   MPI_Bcast(&delta_X, 1, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(&delta_Y, 1, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(&delta_X_, 1, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(&delta_Y_, 1, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(&X, 1, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(&Y, 1, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(&X_, 1, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(&Y_, 1, MPI_INT, 0, MPI_COMM_WORLD);

   int datasize = delta_X/numtasks;

   for (int x=X+(rank*datasize); x<X+((rank+1)*datasize); ++x) {
      for (int y=Y; y<Y+delta_Y; ++y) {
         int x_ = (float(delta_X_) / float(delta_X)) * float(x-X) + X_; 
         int y_ = (float(delta_Y_) / float(delta_Y)) * float(y-Y) + Y_;
         if (x_<num_rows && y_<num_cols)  
            Display_out(x_,y_) = Display_in(x,y);
      }
   } 

   //strcpy(file, argv[1]);
   //strcat(file, "_mpi.bmp");
   //if (0==rank) write_256_bmp(file, num_rows,num_cols, display_out);
   sprintf(file, "%s_mpi_%d.bmp", argv[1], rank);
   write_256_bmp(file, num_rows,num_cols, display_out);

   MPI_Finalize();
}
