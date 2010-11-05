// bmp.hpp

#define MX_ROW 480*2
#define MX_COL 640*2

// from the text page 87
struct COMPLEX {
   float real;
   float imag;
};

int cal_pixel(COMPLEX c);

#define Display_arry(i,j) display_arry [i*(num_cols+3) + j]
#define Display_in(i,j)   display_in   [i*(num_cols+3) + j]
#define Display_out(i,j)  display_out  [i*(num_cols+3) + j]

extern char head[16], f_info[40], color_mp[1024];
extern unsigned char *display_in, *display_out;

void read_256_bmp(
   char* file,
   int* num_rows,
   int* num_cols,
   unsigned char** display_arry
);

void write_256_bmp(
   char* file,
   int num_rows,
   int num_cols,
   unsigned char* display_arry
);
