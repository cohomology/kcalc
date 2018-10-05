project (kcalc CXX)   
cmake_minimum_required (VERSION 3.10)
option(COVERAGE "CodeCoverage" OFF)
 
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set (THREADS_PREFER_PTHREAD_FLAG ON)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON) 
set (CMAKE_CXX_EXTENSIONS OFF)   

if (CMAKE_BUILD_TYPE MATCHES Debug)
  if (COVERAGE MATCHES ON)
    include(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()
  endif()
endif() 

find_package(Threads REQUIRED)
find_package(Boost 1.56 REQUIRED)
find_package(GMP 6.0.0 REQUIRED)
find_package(Readline REQUIRED)  

enable_testing()

add_subdirectory (src)
add_subdirectory (tests)
add_subdirectory (demo)

if (CMAKE_BUILD_TYPE MATCHES Debug)
  if (COVERAGE MATCHES ON)
    SETUP_TARGET_FOR_COVERAGE_GCOVR_HTML(NAME coverage EXECUTABLE ctest DEPENDENCIES ast_test lexer_test arith_test parser_test)
  endif()
endif()
