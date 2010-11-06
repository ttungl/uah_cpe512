#!/bin/bash

./mandelbrot_serial.exe images/red_green_256
mpirun -np 8 ./mandelbrot_mpi.exe images/red_green_256
#./mandelbrot_pthread.exe images/red_green_256 8
#./mandelbrot_omp.exe images/red_green_256 8
