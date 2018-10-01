find_package(Boost 1.56)
find_package(GMP 6.0.0)
include_directories (
  ${PROJECT_SOURCE_DIR}/include
  ${GMP_INCLUDES}
) 
add_library (lexer Lexer.cpp)
add_library (parser Parser.cpp)
add_library (ast Ast.cpp)
add_executable (kcalc Kcalc.cpp)
target_link_libraries (kcalc lexer parser ast Threads::Threads ${GMP_LIBRARIES})
