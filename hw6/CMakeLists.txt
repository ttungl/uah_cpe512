cmake_minimum_required(VERSION 2.6)
project(cpe512_hw6)
enable_testing()

SET(CMAKE_CXX_COMPILER mpicxx)

add_library(bmp bmp.cpp bmp.hpp)

add_executable(display_serial.exe display_serial.cpp)
target_link_libraries(display_serial.exe bmp)

add_executable(display_mpi.exe display_mpi.cpp)
target_link_libraries(display_mpi.exe bmp)

add_executable(mandelbrot_serial.exe mandelbrot_serial.cpp)
target_link_libraries(mandelbrot_serial.exe bmp)

add_test(display_serial_test ./display_serial_test.sh)
add_test(display_mpi_test ./display_mpi_test.sh)
#add_test(mandelbrot_serial_test ./mandelbrot_serial_test.sh)
