// bmp.cpp

using namespace std;
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.hpp"

#define MX_ROW 480*2
#define MX_COL 640*2

int cal_pixel(COMPLEX c) {
   int count, max_iter;
   COMPLEX z;
   float temp, lengthsq;
   max_iter = 256;
   z.real = 0;
   z.imag = 0;
   count = 0;
   do {
      temp = z.real * z.real - z.imag *z.imag + c.real;
      z.imag = 2*z.real*z.imag + c.imag;
      z.real = temp;
      lengthsq = z.real * z.real + z.imag*z.imag;
      count++;
   } while ((lengthsq < 4.0) && (count < max_iter));
   return count;
}

void parse_header(FILE *fp, int *file_size, char * head) {
    int ch,byte_cnt=0; 
    for (byte_cnt=0;byte_cnt<14;byte_cnt++) {
       ch = fgetc(fp);
       head[byte_cnt]=ch;
       if (ch == EOF) {
          cout << "Bit Map file bad or corrupted" << endl;
          exit(1);
       }
       if ((byte_cnt == 0 && ch != 'B') || (byte_cnt == 1 && ch != 'M')) {
          cout << "Bad file type -- not a bit map file" << endl;
          exit(1);
       }
       if (byte_cnt == 2) (*file_size) = ch;  
       if (byte_cnt == 3) (*file_size) += (ch*256);  
       if (byte_cnt == 4) (*file_size) += (ch*256*256);  
       if (byte_cnt == 5) (*file_size) += (ch*256*256*256);  
   }
}

void parse_file_info(FILE * fp, int *num_rows,int *num_cols,
                     char *f_info) {
    int byte_cnt,ch;
    for (byte_cnt=0;byte_cnt<40;byte_cnt++) {
       ch = fgetc(fp);
       f_info[byte_cnt]=ch;
       if (ch == EOF) {
          cout << "Bit Map file bad or corrupted" << endl;
          exit(1);
       }
       if ((byte_cnt == 0 && ch != 40) || (byte_cnt == 1 && ch != 0) ||
           (byte_cnt == 2 && ch != 0)  || (byte_cnt == 3 && ch !=0) ||
           (byte_cnt == 12 && ch != 1) || (byte_cnt == 13 && ch != 0) ||
           (byte_cnt == 14 && ch != 8) || (byte_cnt == 15 && ch != 0) ||
           (byte_cnt == 32 && ch != 0) || (byte_cnt == 33 && ch != 0) ||
           (byte_cnt == 34 && ch != 0) || (byte_cnt == 35 && ch != 0)
) {
          cout << "Error: This bit map file type is not supported"
               << endl;
          exit(1);
       }
       if (byte_cnt == 4) (*num_cols) = ch;  
       if (byte_cnt == 5) (*num_cols) += (ch*256);  
       if (byte_cnt == 6) (*num_cols) += (ch*256*256);  
       if (byte_cnt == 7) (*num_cols) += (ch*256*256*256);  

       if (byte_cnt == 8) (*num_rows) = ch;  
       if (byte_cnt == 9) (*num_rows) += (ch*256);  
       if (byte_cnt == 10) (*num_rows) += (ch*256*256);  
       if (byte_cnt == 11) (*num_rows) += (ch*256*256*256);  
   }
}
void parse_color(FILE *fp, char *color_mp) {
    int ch,byte_cnt=0; 
    for (byte_cnt=0;byte_cnt<1024;byte_cnt++) {
       ch = fgetc(fp);
       color_mp[byte_cnt]=ch;
       if (ch == EOF) {
          cout << "Bit Map file bad or corrupted" << endl;
          exit(1);
       }
   }
}

void parse_data(FILE *fp,int num_rows, int num_cols,
                 unsigned char * display_arry) {
    int ch,i,j,nm_cols;
    nm_cols = num_cols + num_cols%4; 
    for (i=0;i<num_rows;i++) {
       for (j=0;j<nm_cols;j++) {
          ch = fgetc(fp);
          if (ch == EOF) {
             cout << "Bit Map file bad or corrupted" << endl;
             exit(1);
          }
          Display_arry(i,j) = ch;
      }
   }
}

char head[16], f_info[40], color_mp[1024];
unsigned char *display_in, *display_out;

void read_256_bmp(char * file, int *num_rows,int *num_cols,
                 unsigned char * * display_arry) {
   FILE *fp;
   int file_size;
   if ((fp=fopen(file,"rb")) == NULL) {
      perror("Unable to open bmp file\n");
      exit(1);
   }

   parse_header(fp, &file_size,head); 
   parse_file_info(fp,num_rows,num_cols,f_info);
   parse_color(fp,color_mp);

   *display_arry = new (nothrow) unsigned char [((*num_cols)+3)*(*num_rows)];
   if(*display_arry==0) {
     cout <<"ERROR:  Insufficient Memory" << endl;
     exit(1);
   }

   parse_data(fp,(*num_rows),(*num_cols),*display_arry);

   fclose(fp);
}

void write_data(FILE *fp,int num_rows, int num_cols,
                 unsigned char * display_arry) {
   int ch,i,j,nm_cols;
   nm_cols = num_cols + num_cols%4; // pad if necessary
   for (i=0;i<num_rows;i++) {
      for(j=0;j<nm_cols;j++) {
         fputc(Display_arry(i,j),fp);
      }
   }
}

void write_256_bmp(char * file, int num_rows,int num_cols,
                 unsigned char * display_arry) {
   FILE *fp;
   int nm_cols,i,j;
   if ((fp=fopen(file,"wb")) == NULL) {
      perror("Unable to open bmp file for write\n");
      exit(1);
   }
   // write header
   for (i=0;i<14;i++) {
      fputc(head[i],fp);
   }
   // write file info section
   for (i=0;i<40;i++) {
      fputc(f_info[i],fp);
   }
   // write color map
   for (i=0;i<1024;i++) {
      fputc(color_mp[i],fp);
   }
   // write display data
   write_data(fp,num_rows,num_cols,display_arry);
   fclose(fp);
}
