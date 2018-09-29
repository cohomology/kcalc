cmake_minimum_required (VERSION 3.10)
project (kcalc CXX)  
 
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set (THREADS_PREFER_PTHREAD_FLAG ON)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON) 
set (CMAKE_CXX_EXTENSIONS OFF)   

find_package(Threads REQUIRED)
enable_testing()

add_subdirectory (src)
add_subdirectory (tests)
add_subdirectory (demo)
