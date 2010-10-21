#!/bin/sh

grep time serial_output/* | cut -d: -f 1 | cut -d/ -f2 > datasize.dat
grep time serial_output/* | cut -d= -f2 | cut -d' ' -f2 | paste datasize.dat - > serial.dat
grep time omp2_output/* | cut -d= -f2 | cut -d' ' -f2 | paste serial.dat - > omp2.dat
grep time omp4_output/* | cut -d= -f2 | cut -d' ' -f2 | paste omp2.dat - > omp4.dat
grep time omp8_output/* | cut -d= -f2 | cut -d' ' -f2 | paste omp4.dat - > omp8.dat
unlink datasize.dat
unlink serial.dat
unlink omp2.dat
unlink omp4.dat

python - > plot.dat <<EOF
import sys
f=open("omp8.dat", "r")
k_serial = 8.15e-9
k_proc2 = 4.07e-9
k_proc4 = 2.04e-9
k_proc8 = 1.02e-9
for line in f.readlines():
  datasize, serial, proc2, proc4, proc8 = line.split()
  datasize = int(datasize)
  serial = float(serial)
  proc2 = float(proc2)
  proc4 = float(proc4)
  proc8 = float(proc8)
  print datasize, \
        serial, k_serial*datasize**3, \
        proc2, k_proc2*datasize**3, \
        proc4, k_proc4*datasize**3, \
        proc8, k_proc8*datasize**3, \
        serial/proc2, \
        serial/proc4, \
        serial/proc8, \
        serial/(2*proc2), \
        serial/(4*proc4), \
        serial/(8*proc8)
EOF

gnuplot - <<EOF
set terminal postscript color solid "Times-Roman" 20
set output "serial.ps"
#set term x11 persist
set title "matrix/matrix multiplication - serial implementation" 0,0
set xlabel "Data Size (problem dimension)"
set ylabel "Run Time (seconds)"
plot [][] 'plot.dat' using 1:2 title 'measured' with lp, \
          'plot.dat' using 1:3 title 'K*(Data Size)^3 K=8.15e-9' with lp
EOF

gnuplot - <<EOF
set terminal postscript color solid "Times-Roman" 20
set output "proc2.ps"
#set term x11 persist
set title "matrix/matrix multiplication - 2  OpenMP Thread implementation" 0,0
set xlabel "Data Size (problem dimension)"
set ylabel "Run Time (seconds)"
plot [][] 'plot.dat' using 1:4 title 'measured' with lp, \
          'plot.dat' using 1:5 title 'K*(Data Size)^3 K=4.07e-9' with lp
EOF

gnuplot - <<EOF
set terminal postscript color solid "Times-Roman" 20
set output "proc4.ps"
#set term x11 persist
set title "matrix/matrix multiplication - 4  OpenMP Thread implementation" 0,0
set xlabel "Data Size (problem dimension)"
set ylabel "Run Time (seconds)"
plot [][] 'plot.dat' using 1:6 title 'measured' with lp, \
          'plot.dat' using 1:7 title 'K*(Data Size)^3 K=2.04e-9' with lp
EOF

gnuplot - <<EOF
set terminal postscript color solid "Times-Roman" 20
set output "proc8.ps"
#set term x11 persist
set title "matrix/matrix multiplication - 8  OpenMP Thread implementation" 0,0
set xlabel "Data Size (problem dimension)"
set ylabel "Run Time (seconds)"
plot [][] 'plot.dat' using 1:8 title 'measured' with lp, \
          'plot.dat' using 1:9 title 'K*(Data Size)^3 K=1.02e-9' with lp
EOF

gnuplot - <<EOF
set terminal postscript color solid "Times-Roman" 20
set output "all.ps"
#set term x11 persist
set title "matrix/matrix multiplication - run-times" 0,0
set xlabel "Data Size (problem dimension)"
set ylabel "Run Time (seconds)"
plot [][] 'plot.dat' using 1:2 title 'serial' with lp, \
          'plot.dat' using 1:4 title '2  OpenMP Threads' with lp, \
          'plot.dat' using 1:6 title '4  OpenMP Threads' with lp, \
          'plot.dat' using 1:8 title '8  OpenMP Threads' with lp
EOF

gnuplot - <<EOF
set terminal postscript color solid "Times-Roman" 20
set output "speedup.ps"
#set term x11 persist
set title "matrix/matrix multiplication - Speed ups" 0,0
set xlabel "Data Size (problem dimension)"
plot [][] 'plot.dat' using 1:10 title '2  OpenMP Threads' with lp, \
          'plot.dat' using 1:11 title '4  OpenMP Threads' with lp, \
          'plot.dat' using 1:12 title '8  OpenMP Threads' with lp
EOF

gnuplot - <<EOF
set terminal postscript color solid "Times-Roman" 20
set output "efficiency.ps"
#set term x11 persist
set title "matrix/matrix multiplication - Efficiencies" 0,0
set xlabel "Data Size (problem dimension)"
plot [][] 'plot.dat' using 1:13 title '2  OpenMP Threads' with lp, \
          'plot.dat' using 1:14 title '4  OpenMP Threads' with lp, \
          'plot.dat' using 1:15 title '8  OpenMP Threads' with lp
EOF

unlink omp8.dat
unlink plot.dat
