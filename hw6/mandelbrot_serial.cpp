// Mandelbrot Set Program for 256 color
// 640 x 580 window bit map file
// B. Earl Wells, October 2010, University of Alabama in Huntsville
// serial version

using namespace std;
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MX_ROW 480*2
#define MX_COL 640*2

// from the text page 87
class COMPLEX {
   public:
   float real;
   float imag;
};


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
          cout << "Error: This bit map file type is not supported" << endl;
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
                 char display_arry[MX_ROW][MX_COL+3]) {
    int ch,i,j,nm_cols;
    nm_cols = num_cols + num_cols%4; 
//    for (i=num_rows-1;i>=0;i--) {
    for (i=0;i<num_rows;i++) {
       for (j=0;j<nm_cols;j++) {
          ch = fgetc(fp);
          if (ch == EOF) {
             cout << "Bit Map file bad or corrupted" << endl;
             exit(1);
          }
          display_arry[i][j]=ch;
      }
   }
}

       
char head[16],f_info[40],color_mp[1024];
char display_in[MX_ROW][MX_COL+3],display_out[MX_ROW][MX_COL+3];

void read_256_bmp(char * file, int *num_rows,int *num_cols,
                 char display_arry[MX_ROW][MX_COL+3]) {
   FILE *fp;
   int file_size;
   if ((fp=fopen(file,"rb")) == NULL) {
      perror("Unable to open bmp file\n");
      exit(1);
   }

   parse_header(fp, &file_size,head); 
   parse_file_info(fp,num_rows,num_cols,f_info);
   parse_color(fp,color_mp);
   parse_data(fp,(*num_rows),(*num_cols),display_arry);

   fclose(fp);
}

void write_256_bmp(char * file, int num_rows,int num_cols,
                 char display_arry[MX_ROW][MX_COL+3]) {
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
   nm_cols = num_cols + num_cols%4; // pad if necessary
   for (i=0;i<num_rows;i++) {
      for(j=0;j<nm_cols;j++) {
         fputc(display_arry[i][j],fp);
      }
   }
   fclose(fp);
}


// routine to copy an interger in the next four bytes in
// little Endian format
void cpy_val(char *ptr,int val) {
   unsigned int hld;
   hld = val;
   ptr[0]= (char) (hld & 0xff);
   hld >>= 8;
   ptr[1]= (char) (hld & 0xff);
   hld >>= 8;
   ptr[2]= (char) (hld & 0xff);
   hld >>= 8;
   ptr[3]= (char) (hld & 0xff);
}

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
   read_256_bmp(file, &num_rows,&num_cols,display_in);
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
          display_out[i][j]=cal_pixel(c);
       }
   } 

   write_256_bmp("images/mandelbrot_serial.bmp", num_rows,num_cols,display_out);

}

