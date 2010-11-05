#!/bin/bash

./display_serial.exe images/red_green_256 << EOF
400
400
0
0
400
400
0
0
EOF
cmp images/red_green_256.bmp images/red_green_256_serial.bmp

./display_serial.exe images/red_green_256 << EOF
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
display images/red_green_256_serial.bmp &
