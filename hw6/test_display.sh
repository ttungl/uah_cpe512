#!/bin/bash

cat > display.in << EOF
360
360
20
20
200
200
50
50
EOF

./display_serial.exe images/red_green_256 < display.in > /dev/null
mpirun -np 8 ./display_mpi.exe images/red_green_256 < display.in > /dev/null
./display_pthread.exe images/red_green_256 8 < display.in > /dev/null
./display_omp.exe images/red_green_256 8 < display.in > /dev/null

cmp images/red_green_256_{serial,mpi}.bmp
cmp images/red_green_256_{serial,pthread}.bmp
cmp images/red_green_256_{serial,omp}.bmp

./display_serial.exe images/shut < display.in > /dev/null
mpirun -np 8 ./display_mpi.exe images/shut < display.in > /dev/null
./display_pthread.exe images/shut 8 < display.in > /dev/null
./display_omp.exe images/shut 8 < display.in > /dev/null

cmp images/shut_{serial,mpi}.bmp
cmp images/shut_{serial,pthread}.bmp
cmp images/shut_{serial,omp}.bmp

unlink display.in
