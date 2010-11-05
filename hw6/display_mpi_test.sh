#!/bin/bash

mpirun -np 8 ./display_mpi.exe images/shut << EOF
400
500
0
0
400
500
0
0
EOF
