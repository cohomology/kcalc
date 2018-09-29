include_directories ("${PROJECT_SOURCE_DIR}/include")
add_library (lexer Lexer.cpp)
add_executable (kcalc Kcalc.cpp)
target_link_libraries (kcalc lexer Threads::Threads)
