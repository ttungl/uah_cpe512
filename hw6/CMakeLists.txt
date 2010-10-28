cmake_minimum_required(VERSION 2.6)
project(cpe512_hw6)
enable_testing()

add_executable(display_serial.exe display_serial.cpp)
add_executable(mandelbrot_serial.exe mandelbrot_serial.cpp)

add_test(display_serial_test ./display_serial_test.sh)
add_test(mandelbrot_serial_test ./mandelbrot_serial_test.sh)
