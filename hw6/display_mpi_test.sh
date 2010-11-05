#!/bin/bash

mpirun -np 4 ./display_mpi.exe images/red_green_256 << EOF
400
400
0
0
300
300
50
50
EOF

display images/red_green_256.bmp &
display images/red_green_256_mpi_?.bmp &

