set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON) 
set (CMAKE_CXX_EXTENSIONS OFF)  
include_directories ("${PROJECT_SOURCE_DIR}/include")
add_library (lexer Lexer.cpp)
add_executable (kcalc Kcalc.cpp)
target_link_libraries (kcalc lexer)

