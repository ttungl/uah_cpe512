cmake_minimum_required(VERSION 2.6)
project(cpe512_hw2)
enable_testing()

SET(CMAKE_C_COMPILER icc)
SET(CMAKE_CXX_COMPILER icpc)
SET(CMAKE_CXX_FLAGS "-g -O0")

# rv0:
add_executable(add_num_MPI.exe add_num_MPI.cpp)
target_link_libraries(add_num_MPI.exe mpi)
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
target_link_libraries(add_num_MPI_rv1.exe mpi)
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

# rv2:
add_executable(add_num_MPI_rv2.exe add_num_MPI_rv2.cpp)
target_link_libraries(add_num_MPI_rv2.exe mpi)
add_test(add_num_MPI_rv2_1_proc_2483_datasize 
   mpirun -np 1 ./add_num_MPI_rv2.exe 2483)
set_tests_properties(add_num_MPI_rv2_1_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv2_2_proc_2483_datasize 
   mpirun -np 2 ./add_num_MPI_rv2.exe 2483)
set_tests_properties(add_num_MPI_rv2_2_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv2_3_proc_2483_datasize 
   mpirun -np 3 ./add_num_MPI_rv2.exe 2483)
set_tests_properties(add_num_MPI_rv2_3_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv2_4_proc_2483_datasize 
   mpirun -np 4 ./add_num_MPI_rv2.exe 2483)
set_tests_properties(add_num_MPI_rv2_4_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv2_5_proc_2483_datasize 
   mpirun -np 5 ./add_num_MPI_rv2.exe 2483)
set_tests_properties(add_num_MPI_rv2_5_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv2_6_proc_2483_datasize 
   mpirun -np 6 ./add_num_MPI_rv2.exe 2483)
set_tests_properties(add_num_MPI_rv2_6_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv2_7_proc_2483_datasize 
   mpirun -np 7 ./add_num_MPI_rv2.exe 2483)
set_tests_properties(add_num_MPI_rv2_7_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv2_8_proc_2483_datasize 
   mpirun -np 8 ./add_num_MPI_rv2.exe 2483)
set_tests_properties(add_num_MPI_rv2_8_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv2_9_proc_2483_datasize 
   mpirun -np 9 ./add_num_MPI_rv2.exe 2483)
set_tests_properties(add_num_MPI_rv2_9_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv2_10_proc_2483_datasize 
   mpirun -np 10 ./add_num_MPI_rv2.exe 2483)
set_tests_properties(add_num_MPI_rv2_10_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv2_11_proc_2483_datasize 
   mpirun -np 11 ./add_num_MPI_rv2.exe 2483)
set_tests_properties(add_num_MPI_rv2_11_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv2_12_proc_2483_datasize 
   mpirun -np 12 ./add_num_MPI_rv2.exe 2483)
set_tests_properties(add_num_MPI_rv2_12_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")

# rv3:
add_executable(add_num_MPI_rv3.exe add_num_MPI_rv3.cpp)
target_link_libraries(add_num_MPI_rv3.exe mpi)
add_test(add_num_MPI_rv3_1_proc_2483_datasize 
   mpirun -np 1 ./add_num_MPI_rv3.exe 2483)
set_tests_properties(add_num_MPI_rv3_1_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv3_2_proc_2483_datasize 
   mpirun -np 2 ./add_num_MPI_rv3.exe 2483)
set_tests_properties(add_num_MPI_rv3_2_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv3_3_proc_2483_datasize 
   mpirun -np 3 ./add_num_MPI_rv3.exe 2483)
set_tests_properties(add_num_MPI_rv3_3_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv3_4_proc_2483_datasize 
   mpirun -np 4 ./add_num_MPI_rv3.exe 2483)
set_tests_properties(add_num_MPI_rv3_4_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv3_5_proc_2483_datasize 
   mpirun -np 5 ./add_num_MPI_rv3.exe 2483)
set_tests_properties(add_num_MPI_rv3_5_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv3_6_proc_2483_datasize 
   mpirun -np 6 ./add_num_MPI_rv3.exe 2483)
set_tests_properties(add_num_MPI_rv3_6_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv3_7_proc_2483_datasize 
   mpirun -np 7 ./add_num_MPI_rv3.exe 2483)
set_tests_properties(add_num_MPI_rv3_7_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv3_8_proc_2483_datasize 
   mpirun -np 8 ./add_num_MPI_rv3.exe 2483)
set_tests_properties(add_num_MPI_rv3_8_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv3_9_proc_2483_datasize 
   mpirun -np 9 ./add_num_MPI_rv3.exe 2483)
set_tests_properties(add_num_MPI_rv3_9_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv3_10_proc_2483_datasize 
   mpirun -np 10 ./add_num_MPI_rv3.exe 2483)
set_tests_properties(add_num_MPI_rv3_10_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv3_11_proc_2483_datasize 
   mpirun -np 11 ./add_num_MPI_rv3.exe 2483)
set_tests_properties(add_num_MPI_rv3_11_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
add_test(add_num_MPI_rv3_12_proc_2483_datasize 
   mpirun -np 12 ./add_num_MPI_rv3.exe 2483)
set_tests_properties(add_num_MPI_rv3_12_proc_2483_datasize 
   PROPERTIES PASS_REGULAR_EXPRESSION "Sum of numbers is 3081403")
