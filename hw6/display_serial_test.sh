#!/bin/bash

./display_serial.exe images/shut << EOF
300
400
100
100
50
50
25
25
EOF

cmp images/shut_serial.bmp \
    images/shut_serial_baseline.bmp
