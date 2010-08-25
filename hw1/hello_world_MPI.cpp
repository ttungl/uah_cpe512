/* 
MPI Example - Hello World - C++ Version
FILE: hello_world_MPI.cpp

Compilation on altix.asc.edu

   to compile the program type

   icc hello_world_MPI.cpp -o hello_world_MPI -lmpi++ -lmpi

    to run on eight processors type

     mpirun -np 8 ./hello_world_MPI
*/

using namespace std;
#include <iostream>
#include <mpi.h>

int main (int argc, char *argv[]) {

   MPI_Status status;
   int nmtsks, rank;

   MPI_Init(&argc,&argv); // initalize MPI environment
   MPI_Comm_size(MPI_COMM_WORLD,&nmtsks); //get total number of processes
   MPI_Comm_rank(MPI_COMM_WORLD,&rank); // get process identity number

   cout << "Hello World from MPI Process #" << rank << endl;

  /* Only root MPI process does this */
  if (rank == 0) {
     cout << "Number of MPI Processes = " << nmtsks << endl;
  }

  /* Terminate MPI Program -- clear out all buffers */
  MPI_Finalize();

}

