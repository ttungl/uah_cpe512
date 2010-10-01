#!/bin/sh

SIZES="0100 0200 0300 0400 0500 0600 0700 0800 0900 1000 1100 1200 1300 1400 1500 1600 1700 1800 1900 2000"
SIZES="0100 0200 0300 0400 0500 0600 0700 0800 0900"

mkdir -p serial_output
icc -o mm_mult_serial mm_mult_serial.cpp

for size in $SIZES
do
echo $size
./mm_mult_serial $size > serial_output/$size
done

mpicxx -o mm_mult_MPI_sol mm_mult_MPI_sol.cpp

PROCS="2 4 8"

for np in $PROCS
do
echo $np
dirname=mpi${np}_output
mkdir -p $dirname
for size in $SIZES
do
echo $size
mpirun -np $np ./mm_mult_MPI_sol $size > $dirname/$size
done
done

DIFF="diff -Bqr"
$DIFF serial_output mpi2_output
$DIFF mpi2_output mpi4_output
$DIFF mpi4_output mpi8_output
