// Graphic Transformation/Scaling Program for 256 color
// 640 x 580 window bit map file
// B. Earl Wells, October 2010, University of Alabama in Huntsville
// serial version more condusive to MPI Scatter and Gather of display
// arrays. Uses dynamic declarations of display arrays similar
// to the matrix multiply example

using namespace std;
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define MX_ROW 480*2
#define MX_COL 640*2

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

#define Display_arry(i,j) display_arry[i*(num_cols+3) + j]
#define Display_in(i,j) display_in[i*(num_cols+3) + j]
#define Display_out(i,j) display_out[i*(num_cols+3) + j]

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

       
char head[16],f_info[40],color_mp[1024];
unsigned char *display_in,*display_out;

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
   int byte_cnt,num_rows, num_cols;
   char file[80];
   int i,j;
   int delta_X,delta_Y,delta_X_,delta_Y_,X,Y,X_,Y_,x,y,x_,y_;

   if (argc != 2) {
      cout << "Usage: display [file name (no extension)]" << endl;
      exit(1);
   }

   // read a bit map file such as shut.bmp to get initial image 
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
   // clear out output display area
   for (i=0;i<num_rows;i++) for (j=0;j<num_cols;j++) Display_out(i,j)=255;

   get_params(&delta_X,&delta_Y,&X,&Y,&delta_X_,&delta_Y_,&X_,&Y_);
   for (x=X;x<X+delta_X;x++) {
      for (y=Y;y<Y+delta_Y;y++) {
         x_= ((float) delta_X_/(float) delta_X)*(float) (x-X) + X_; 
         y_= ((float) delta_Y_/(float) delta_Y)*(float) (y-Y) + Y_;
         if (x_<num_rows && y_<num_cols)  
            Display_out(x_,y_) = Display_in(x,y);
      }
   } 

   strcpy(file,argv[1]);
   strcat(file,"_.bmp");
   write_256_bmp(file, num_rows,num_cols,display_out);

}

