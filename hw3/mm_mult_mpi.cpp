/******************************************************************/
/* Matrix Matrix Multiplication MPI Program                       */
/* September 2010 -- Josh Calahan                                 */
/* based on:                                                      */
/* Matrix Matrix Multiplication Program Example -- serial version */
/* September 2010 -- B. Earl Wells -- University of Alabama       */
/*                                    in Huntsville               */
/******************************************************************/
// mm_mult_mpi.cpp
// compilation:
//   general: mpicxx -o mm_mult_mpi mm_multi_mpi.cpp
//   ASC altis: icc -o mm_mult_mpi mm_multi_mpi.cpp -lmpi
/*
   This program is designed to perform matrix matrix multiplication
   A x B = C, where A is a l x m matrix, B is a m x n matrix and
   C is a l x n matrix.  

   The program randomly assigns the elements of the A and B matrix
   with values between 0 and a MAX_VALUE. It then multiples the
   two matrices with the result being placed in the C matrix.
   The program prints out the A, B, and C matrices.

   The program is executed using one or three command line parameters.
   These parameters represent the dimension of the matrices. If only
   one parameter is used then then it is assumed that square matrices are
   to be created and multiplied together that have the specified 
   dimension. In cases where three command line parameters are entered
   then the first parameter is the l dimension, the second the m, and
   the third is the n dimension.

   To execute:
      mpirun -np n_procs mm_mult_serial l_parameter [m_parameter n_parameter] 
*/

using namespace std;
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <mpi.h>

#define MX_SZ 320
#define SEED 2397           /* random number seed */
#define MAX_VALUE  100.0    /* maximum size of array elements A, and B */

/* copied from mpbench */
#define TIMER_CLEAR     (tv1.tv_sec = tv1.tv_usec = tv2.tv_sec = tv2.tv_usec = 0)
#define TIMER_START     gettimeofday(&tv1, (struct timezone*)0)
#define TIMER_ELAPSED   ((tv2.tv_usec-tv1.tv_usec)+((tv2.tv_sec-tv1.tv_sec)*1000000))
#define TIMER_STOP      gettimeofday(&tv2, (struct timezone*)0)
struct timeval tv1,tv2;

/*
This declaration facilitates the creation of a two dimensional 
dynamically allocated arrays (i.e. the lxm A array, the mxn B
array, and the lxn C array).  It allows pointer arithmetic to 
be applied to a single data stream that can be dynamically allocated.
To address the element at row x, and column y you would use the
following notation:  A(x,y),B(x,y), or C(x,y), respectively.
Note that this differs from the normal C notation if A were a
two dimensional array of A[x][y] but is still very descriptive
of the data structure.
*/
#define A(i,j) *(a+i*dim_m+j)
#define B(i,j) *(b+i*dim_n+j)
#define C(i,j) *(c+i*dim_n+j)

/*
   Routine to retrieve the data size of the numbers array from the 
   command line or by prompting the user for the information
*/
void get_index_size(int rank,int argc,char *argv[],int *dim_l,int *dim_m,int *dim_n) {
   if(argc!=2 && argc!=4) {
      if (rank==0) cout<<"Usage: mpirun -np n_procs mm_mult_serial"
                         "l_parameter [m_parameter n_parameter]\n";
      MPI_Finalize();
      exit(1);
   }
   else {
      if (argc == 2) {
         *dim_l = *dim_n = *dim_m = atoi(argv[1]);
      }
      else {
         *dim_l = atoi(argv[1]);
         *dim_m = atoi(argv[2]);
         *dim_n = atoi(argv[3]);
      }
   }
   if (*dim_l<=0 || dim_n<=0 || dim_m<=0) {
      cout<<"Error: number of rows and/or columns must be greater than 0"
          << endl;
      exit(1);
   }
}

/*
   Routine that fills the number matrix with Random Data with values
   between 0 and MAX_VALUE
   This simulates in some way what might happen if there was a 
   single sequential data acquisition source such as a single file
*/
void fill_matrix(float *array,int dim_n,int dim_m)
{
#define M(i,j) *(array+i*dim_m+j)
   int i,j;
   for(i=0;i<dim_m;i++) {
      for (j=0;j<dim_n;j++) {
         array[i*dim_n+j]=drand48()*MAX_VALUE;
         M(i,j) = (i+1)*(j+2);
      }
   }
#undef M
}

/*
   Routine that outputs the matrices to the screen 
*/
void print_matrix(const char *prefix,float *array,int dim_m,int dim_n)
{
   cout << prefix << " matrix =" << endl;
   int i,j;
   for(i=0;i<dim_m;i++) {
      for (j=0;j<dim_n;j++) {
         cout << array[i*dim_n+j] << " ";
      }
      cout << endl;
   }
   cout << endl;
}
void print_vector(const char *prefix,float *array,int dim)
{
   cout << prefix << " vector = ";
   for(int i=0;i<dim;i++) cout << array[i] << " ";
}

/*
   MAIN ROUTINE: summation of a number list
*/

int main( int argc, char *argv[])
{
   bool debug = false;
   bool verbose = false;
   float *a,*b,*c,*v,*r,dot_prod;
   int dim_l,dim_n,dim_m;
   int i,j,k,datasize;
   int numtasks,rank;
   MPI_Status status;

   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank); 

   /* 
   get matrix sizes
   */
   get_index_size(rank,argc,argv,&dim_l,&dim_m,&dim_n);

   /*
   datasize - n elements from the A matrix I have
   */
   datasize = (dim_l*dim_m)/numtasks;
   if (debug && rank==0) cout << "datasize = " << datasize << endl;
  
   // dynamically allocate from heap the numbers in the memory space
   if (rank==0) { // only proc 0 has direct access to A & C matrices
      a = new (nothrow) float[dim_l*dim_m];
      c = new (nothrow) float[dim_l*dim_n];
   }

   b = new (nothrow) float[dim_m*dim_n]; // each proc gets B matrix
   v = new (nothrow) float[datasize]; // input from A to each proc
   r = new (nothrow) float[datasize]; // output to C from each proc
   
   /*
      initialize A & B numbers matrix with random data
   */ 
   if (rank==0) {
      srand48(SEED);
      fill_matrix(a,dim_l,dim_m);
      fill_matrix(b,dim_m,dim_n);
      if (debug) print_vector("A",a,dim_l*dim_m); cout << endl;
   }

   /*
     output numbers matrix
   */
   if (verbose && rank==0) {
      print_matrix("A",a,dim_l,dim_m);
      print_matrix("B",b,dim_m,dim_n);
   }

   /*
   Start recording the execution time
   */
   TIMER_CLEAR;
   TIMER_START;

   MPI_Scatter(a, datasize, MPI_FLOAT, 
               v, datasize, MPI_FLOAT, 
               0, MPI_COMM_WORLD);

   if (debug) {
      cout << "rank = " << rank << " ";
      print_vector("v",v,datasize);
      cout << endl;
   }

   MPI_Bcast(b, dim_m*dim_n, MPI_FLOAT, 0, MPI_COMM_WORLD);

   // multiply local part of matrix
   for (i=0;i<datasize/dim_l;i++) {
      for (j=0;j<dim_n;j++) {
         dot_prod = 0.0;
         for (k=0;k<dim_m;k++) {
            dot_prod += v[i*dim_m+k]*B(k,j);
         }
         r[i*dim_m+j] = dot_prod;
      }
   }

   MPI_Gather(r, datasize, MPI_FLOAT, 
              c, datasize, MPI_FLOAT, 
              0, MPI_COMM_WORLD);

   /*
      stop recording the execution time
   */ 
   TIMER_STOP;

   if (verbose && rank==0) print_matrix("C",c,dim_l,dim_n);

   if (rank==0) cout << " time = " << setprecision(8) 
                     << TIMER_ELAPSED/1000000.0 
                     << " seconds" << endl;

   MPI_Finalize();
}

