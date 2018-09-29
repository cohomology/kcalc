cmake_minimum_required (VERSION 3.10)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads required)

project (kcalc)
add_subdirectory (src)
add_subdirectory (tests)

