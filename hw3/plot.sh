#!/bin/sh

grep time serial_output/* | cut -d: -f 1 | cut -d. -f2 > datasize.dat
grep time serial_output/* | cut -d= -f2 | cut -d' ' -f2 | paste datasize.dat - > serial.dat
grep time mpi2_output/* | cut -d= -f2 | cut -d' ' -f2 | paste serial.dat - > mpi2.dat
grep time mpi4_output/* | cut -d= -f2 | cut -d' ' -f2 | paste mpi2.dat - > mpi4.dat
grep time mpi8_output/* | cut -d= -f2 | cut -d' ' -f2 | paste mpi4.dat - > plot.dat
unlink datasize.dat
unlink serial.dat
unlink mpi2.dat
unlink mpi4.dat

gnuplot - <<EOF
#set terminal postscript color solid "Times-Roman" 20
#set output "plot.ps"
set term x11 persist
set title "time vs. datasize" 0,0
set xlabel "datasize"
set ylabel "time (seconds)"
plot [][] 'plot.dat' using 1:2 title 'serial' with lp, \
     'plot.dat' using 1:3 title '2 processors' with lp, \
     'plot.dat' using 1:4 title '4 processors' with lp, \
     'plot.dat' using 1:5 title '8 processors' with lp
EOF

unlink plot.dat
