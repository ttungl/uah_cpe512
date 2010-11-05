#!/bin/bash

./display_mpi.exe images/shut << EOF
481
600
0
0
481
600
0
0
EOF

display images/shut.bmp &
display images/shut_serial.bmp &
