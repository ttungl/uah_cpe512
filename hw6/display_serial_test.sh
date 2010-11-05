#!/bin/bash

./display_mpi.exe images/shut << EOF
481
600
0
0
381
500
50
60
EOF

display images/shut.bmp &
display images/shut_serial.bmp &
