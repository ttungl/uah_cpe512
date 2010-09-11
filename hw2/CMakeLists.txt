cmake_minimum_required(VERSION 2.6)
project(cpe512_hw2)
enable_testing()

SET(CMAKE_CXX_COMPILER mpicxx)
SET(CMAKE_CXX_FLAGS "-g -O0")

# rv0:
add_executable(add_num_MPI.exe add_num_MPI.cpp)
#target_link_libraries(add_num_MPI.exe mpi)
add_test(add_num_MPI_5_procs_10_datasize 
   mpirun -np 5 ./add_num_MPI.exe 10)
set_tests_properties(add_num_MPI_5_procs_10_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 45")
add_test(add_num_MPI_1_proc_2483_datasize 
   mpirun -np 1 ./add_num_MPI.exe 2483)
set_tests_properties(add_num_MPI_1_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")

# rv1:
add_executable(add_num_MPI_rv1.exe add_num_MPI_rv1.cpp)
#target_link_libraries(add_num_MPI_rv1.exe mpi)
add_test(add_num_MPI_rv1_5_procs_10_datasize 
   mpirun -np 5 ./add_num_MPI_rv1.exe 10)
set_tests_properties(add_num_MPI_rv1_5_procs_10_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 45"
   PASS_REGULAR_EXPRESSION "Minimum number is 0"
   PASS_REGULAR_EXPRESSION "Maximum number is 9")
add_test(add_num_MPI_rv1_1_proc_2483_datasize 
   mpirun -np 1 ./add_num_MPI_rv1.exe 2483)
set_tests_properties(add_num_MPI_rv1_1_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403"
   PASS_REGULAR_EXPRESSION "Minimum number is 0"
   PASS_REGULAR_EXPRESSION "Maximum number is 2482")
