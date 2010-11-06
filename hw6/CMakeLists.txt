cmake_minimum_required(VERSION 2.6)
project(cpe512_hw6)
enable_testing()

SET(CMAKE_CXX_COMPILER mpicxx)
SET(CMAKE_CXX_FLAGS "-openmp")

add_library(bmp bmp.cpp bmp.hpp)

add_executable(display_serial.exe display_serial.cpp)
target_link_libraries(display_serial.exe bmp)

add_executable(display_mpi.exe display_mpi.cpp)
target_link_libraries(display_mpi.exe bmp)

add_executable(display_pthread.exe display_pthread.cpp)
target_link_libraries(display_pthread.exe bmp pthread)

add_executable(display_omp.exe display_omp.cpp)
target_link_libraries(display_omp.exe bmp)

add_executable(mandelbrot_serial.exe mandelbrot_serial.cpp)
target_link_libraries(mandelbrot_serial.exe bmp)

add_executable(mandelbrot_mpi.exe mandelbrot_mpi.cpp)
target_link_libraries(mandelbrot_mpi.exe bmp)

add_executable(mandelbrot_pthread.exe mandelbrot_pthread.cpp)
target_link_libraries(mandelbrot_pthread.exe bmp pthread)

add_executable(mandelbrot_omp.exe mandelbrot_omp.cpp)
target_link_libraries(mandelbrot_omp.exe bmp)

add_test(test_display ./test_display.sh)
add_test(test_mandelbrot ./test_mandelbrot.sh)
