#!/bin/sh

SIZES="0100 0200 0300 0400 0500 0600 0700 0800 0900 1000 1100 1200 1300 1400 1500 1600 1700 1800 1900 2000"

mkdir -p serial_output
icc -o mm_mult_serial mm_mult_serial.cpp

for size in $SIZES
do
echo $size
./mm_mult_serial $size > serial_output/$size
done

icc -o mm_mult_omp mm_mult_omp.cpp -openmp

PROCS="2 4 8"

for np in $PROCS
do
echo $np
export OMP_NUM_THREADS=$np
dirname=omp${np}_output
mkdir -p $dirname
for size in $SIZES
do
echo $size
./mm_mult_omp $size > $dirname/$size
done
diff -Bqr serial_output $dirname
done
