/***************************************************************/
/* Summation of a Sequence of Numbers Program -- MPI version   */
/* July 2010 -- B. Earl Wells -- University of Alabama         */
/*                                    in Huntsville            */
/***************************************************************/
/*
This program illustrates the basic concepts of SPMD programming using 
MPI.  The program represents a common example that is used throughout 
the CPE 412/512 text, the distributed addition of a sequence of numbers.
The program is written in such a way that it is assumed that the
sequence of numbers is first read from a central source (such as
a data file) by a single MPI process and then partitioned into equal
groups with each part being distributed (scattered) to the other
MPI processes in the system. After which each process computes its own 
partial sum and send this value to the root process which then
adds the partial sums together (reduce operation) and then
outputs this sum to the screen.

The following is a simplified version of the program which you will
be asked to augment in a number of ways in future homework 
assignments. Use the altix.asc.edu system for these assignments. 

To compile type:

   icc add_num_MPI_rv3.cpp -o add_num_MPI_rv3 -lmpi

To execute:
   mpirun -np [num MPI process] add_num_MPI_rv3 [num of numbers] 

Notes: This implementation utilizes a minimum set of MPI function
       call that include MPI_Init, MPI_Finalize, MPI_Comm_size,
       MPI_Comm_rank, MPI_Send, and MPI_Recv.
*/  

using namespace std;
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <mpi.h> /* MPI Prototype Header Files */

#define SEED 2397           /* random number seed */
#define MAX_VALUE  100.0    /* maximum value of any number in list */
#define MIN_VALUE  0.0      /* minimum value of any number in list */
/*
Routine to retrieve the data size of the numbers array from the 
command line or get this number by prompting the user for the 
information.  Note: command line values are sent to ALL MPI processes
by the MPI environment.
*/
int get_data_size(int argc,char *argv[],int rank,int numtasks)
{
   string input = "";
   int size;

   // ERROR if too many command line arguments
   if(argc > 2) {
      if (rank==0)
         cout << "usage:  mpirun -np [num MPI tasks] add_num_MPI_rv3 [data size]" << endl;
      MPI_Finalize(); // Terminate MPI
      exit(1); // Exit Program 
   }
   // One Command Line Argument Case:
   // case where user did not enter number of numbers on command line
   // In this case, only one of the MPI processes needs to communicate
   // directly with the user. Since there will always be a MPI process
   // with rank 0 this is the one that will perform the communication.
   if (argc==1) {
      if (rank==0) {
         while (1) {
            cout << "Enter the number of numbers to be added:" << endl;
            getline(cin, input);
            stringstream myStream(input);
            if (myStream >> size) break;
            cout << "Invalid Input" << endl << endl;
         }
      }     
      // since only the root MPI process is communicating with the
      // user, the root process must send its value to all of the
      // other MPI process. It can do this with the MPI_Bcast()
      // broadcast routine.
      MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
   }
   // Two Command Line Argument case:
   // user supplied the number of numbers on the command line.
   // Each MPI process can retrieve it from there. No need to
   // broadcast it to the other process because each have it at
   // run time.
   else {
      size = atoi(argv[1]);
   }
   return size;
}

/*
Routine that fills the number matrix with Random Data with values
between MIN_VALUE and MAX_VALUE
This simulates in some way what might happen if there was a 
single sequential data acquisition source such as a single file
*/
void fill_matrix(double *numbers,int data_size)
{
   int i;
   srand48(SEED);
   for(i=0;i<data_size;i++) {
      numbers[i]=drand48()*(MAX_VALUE-MIN_VALUE) + MIN_VALUE;
      //to verify may want to initialize the numbers array with a pattern
      //that has a known answer such as the sum of numbers from 0 to N-1
      // The result of that summation is (N+1)*N/2!!
      numbers[i]=i; // to do so uncomment this line
   }
}

/*
Routine that outputs the numbers matrix to the screen 
*/
void print_matrix(double *numbers,int data_size)
{
   int i;
   for(i=0;i<data_size;i++) {
      cout << numbers[i] << endl;
   }
}

/*
MAIN ROUTINE: summation of numbers in a list
*/
int main( int argc, char *argv[])
{
   double *numbers,*group;
   double sum, pt_sum, minv, p_minv, maxv, p_maxv;
   int data_size,group_size,num_group,i;
   int numtasks,rank,num;
   MPI_Status status;


   MPI_Init(&argc,&argv); // initalize MPI environment
   MPI_Comm_size(MPI_COMM_WORLD,&numtasks); // get total number of MPI processes
   MPI_Comm_rank(MPI_COMM_WORLD,&rank); // get unique task id number 

   //get data size from command line or prompt
   //the user for input
   data_size=get_data_size(argc,argv,rank,numtasks);

   num_group = data_size/numtasks; // determine local list size 
   if (rank==numtasks-1) num_group += data_size%numtasks;

   // dynamically allocate from heap the numbers and group arrays
   numbers = new (nothrow) double[data_size];
   group = new (nothrow) double[num_group+1];
   if (numbers==0 || group==0) { // check for null pointers
      if (rank==0) cout << "Memory Allocation Error" << endl;
      MPI_Finalize(); // Exit MPI
      exit(1); // exit the program
   }
 
   //  if root MPI Process (0) then
   if(rank==0) {
      // initialize numbers matrix with random data
      fill_matrix(numbers,data_size);

      // and print the numbers matrix
      cout << "numbers matrix =" << endl; 
      print_matrix(numbers,data_size);
      cout << endl;
   }
   // scatter the numbers matrix to all processing elements in
   // the system
   MPI_Scatter(numbers, num_group, MPI_DOUBLE, 
               group, num_group, MPI_DOUBLE, 
               0, MPI_COMM_WORLD);

   int balance = data_size%numtasks;
   if (balance) {
      if (0==rank) {
         MPI_Send(numbers+(num_group*numtasks), balance, MPI_DOUBLE, numtasks-1, 123, MPI_COMM_WORLD);
      }
      if (numtasks-1==rank) {
         MPI_Recv(group+num_group-balance, balance, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, &status);
      }
   }

   // sum up elements in the group associated with the
   // current process
                                   // group
   pt_sum =0;                      // clear out partial sum
   for (i=0;i<num_group;i++) {
      pt_sum += group[i];
      if (0==i) {
         p_minv=group[i];
         p_maxv=group[i];
      } else {
         if (p_minv>group[i]) p_minv=group[i];
         if (p_maxv<group[i]) p_maxv=group[i];
      }
   }

   // obtain final sum, min, and max by summing up partial sums from other MPI tasks/
   //sum=reduce(pt_sum,rank,numtasks);
   MPI_Reduce(&pt_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 
   MPI_Reduce(&p_minv, &minv, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD); 
   MPI_Reduce(&p_maxv, &maxv, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); 

   // output sum from root MPI process
   if (rank==0) {
      cout << "Sum of numbers is " << setprecision(8) << sum << endl;
      cout << "Minimum number is " << setprecision(8) << minv << endl;
      cout << "Maximum number is " << setprecision(8) << maxv << endl;
   }

   // reclaim dynamiclly allocated memory
   delete numbers; delete group;

   // Terminate MPI Program -- perform necessary MPI housekeeping
   // clear out all buffers, remove handlers, etc.
   MPI_Finalize();
}
