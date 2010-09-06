/* 
MPI/PThread Hybrid Example
FILE: hello_world_MPI_PTH.cpp
To compile:
   $ icc hello_world_MPI_PTH.cpp -o hello_world_MPI_PTH -lmpi -lmpi++ -lpthread 
To run (on N processors and M threads):
   $ mpirun -np N ./hello_world_MPI_PTH M
*/

#include <pthread.h>
#include <stdio.h>
#include <mpi.h>

pthread_mutex_t MUTEX;

struct thread_params { int rank, thread; };

void* hello_world_MPI_PTH(void* arg) {
   thread_params* params = (thread_params*)arg;

   pthread_mutex_lock(&MUTEX);

   printf("Hello World from MPI Process %d Thread %d\n",
          params->rank,
          params->thread);
   fflush(stdout);

   pthread_mutex_unlock(&MUTEX);

   return arg;
}

int main (int argc, char** argv)
{
   int nprocs, rank;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   if (2!=argc) {
      if (0==rank) fprintf(stderr, "ERROR: Usage: mpirun"
        " -np n_procs ./hello_world_MPI_PTH n_threads\n");
      MPI_Finalize();
      return 1;
   }

   int nthreads = atoi(argv[1]);

   pthread_mutex_init(&MUTEX, NULL);

   if (0==rank) {
      printf("nprocs=%d, nthreads=%d\n", nprocs, nthreads);
      fflush(stdout);
   }

   pthread_t* threads = new pthread_t[nthreads];
   thread_params* params = new thread_params[nthreads];

   for (int i=0; i<nthreads; ++i) {
      params[i].rank = rank;
      params[i].thread = i;
      pthread_create(threads+i, NULL, hello_world_MPI_PTH, params+i);
   }

   for (int i=0; i<nthreads; ++i) 
      pthread_join(threads[i], NULL);        

   MPI_Finalize();
   return 0;
}

