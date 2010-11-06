#!/bin/bash

./display_serial.exe images/red_green_256 << EOF
360
360
0
0
200
200
50
50
EOF

mpirun -np 8 ./display_mpi.exe images/red_green_256 << EOF
360
360
0
0
200
200
50
50
EOF

./display_pthread.exe images/red_green_256 8 << EOF
360
360
0
0
200
200
50
50
EOF

cmp images/red_green_256_{serial,mpi}.bmp
cmp images/red_green_256_{serial,pthread}.bmp

./display_serial.exe images/shut << EOF
360
360
0
0
200
200
50
50
EOF

mpirun -np 8 ./display_mpi.exe images/shut << EOF
360
360
0
0
200
200
50
50
EOF

./display_pthread.exe images/shut 8 << EOF
360
360
0
0
200
200
50
50
EOF

cmp images/shut_{serial,mpi}.bmp
cmp images/shut_{serial,pthread}.bmp
