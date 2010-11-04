// Mandelbrot Set Program for 256 color
// 640 x 580 window bit map file
// B. Earl Wells, October 2010, University of Alabama in Huntsville
// serial version

using namespace std;
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.hpp"

int main( int argc, char *argv[])  {
   int byte_cnt,num_rows, num_cols;
   char file[80];
   int i,j;
   COMPLEX c;
   float disp_width,disp_height,scale_real,scale_imag;

   // set to values in the text on page 88
   float real_min = -2.0, real_max = 2.0;
   float imag_min = -2.0, imag_max = 2.0;

   if (argc != 2) {
      cout << "Usage: mandelbrot [file name of reference file (no extension)]"
           << endl;
      exit(1);
   }
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

   
   // clear out output display area
   for (i=0;i<num_rows;i++) {
       for (j=0;j<num_cols;j++) {
          c.real = real_min + (float) j * scale_real;
          c.imag = imag_min + (float) i * scale_imag;
          Display_out(i,j) = cal_pixel(c);
       }
   } 

   write_256_bmp("images/mandelbrot_serial.bmp", num_rows,num_cols,display_out);
}

