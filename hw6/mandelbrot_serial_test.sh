#!/bin/bash

./mandelbrot_serial.exe images/shut

cmp images/mandelbrot_serial.bmp \
    images/mandelbrot_serial_baseline.bmp
