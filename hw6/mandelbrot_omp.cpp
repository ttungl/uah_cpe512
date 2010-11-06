// Mandelbrot Set Program for 256 color
// 640 x 580 window bit map file
// Josh Calahan, November 2010, OpenMP Version, based on
// B. Earl Wells, October 2010, University of Alabama in Huntsville
// serial version

using namespace std;
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "bmp.hpp"

int main( int argc, char *argv[])  {

   if (argc != 3) {
      cout << "Usage: mandelbrot [file name of reference file (no extension)]"
           << " n_threads"
           << endl;
      exit(1);
   }

   float disp_width, disp_height, scale_real, scale_imag;
   int num_rows, num_cols;
   int numtasks = atoi(argv[2]);
   char file[80];

   omp_set_num_threads(numtasks);

   // set to values in the text on page 88
   float real_min = -2.0, real_max = 2.0;
   float imag_min = -2.0, imag_max = 2.0;

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
   
   #pragma omp parallel for 
   for (int i=0; i<num_rows; ++i) {
       for (int j=0; j<num_cols; ++j) {
          COMPLEX c;
          c.real = real_min + float(j) * scale_real;
          c.imag = imag_min + float(i) * scale_imag;
          Display_out(i,j) = cal_pixel(c);
       }
   } 

   write_256_bmp("images/mandelbrot_omp.bmp", num_rows,num_cols,display_out);
}
