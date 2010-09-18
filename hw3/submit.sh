#!/bin/sh

SIZES="0100 0200 0300 0400 0500 0600 0700 0800 0900 1000 1100 1200 1300 1400 1500 1600 1700 1800 1900 2000"

mkdir -p serial_output
icc -o mm_mult_serial mm_mult_serial.cpp

for size in $SIZES
do
echo $size
filename=r1s$size.scr
cat>>$filename <<EOF
./mm_mult_serial $size > serial_output/mm_mult_serial.$size
EOF
chmod +x $filename
run_script $filename 1>> submit.log 2>&1 <<EOF
class
1



r1s$size
altix
EOF
done

icc -o mm_mult_mpi mm_mult_mpi.cpp -lmpi

PROCS="2 4 8"

for np in $PROCS
do
echo $np
dirname=mpi${np}_output
mkdir -p $dirname
for size in $SIZES
do
echo $size
filename=r${np}s$size.scr
cat>>$filename <<EOF
mpirun -np $np ./mm_mult_mpi $size > $dirname/mm_mult_mpi.$size
EOF
chmod +x $filename
run_script $filename 1>> submit.log 2>&1 <<EOF
class
$np



r${np}s$size
altix
EOF
done
done
