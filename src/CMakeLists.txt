include_directories (
  ${PROJECT_SOURCE_DIR}/include
)
find_package(Boost 1.56)
add_library (lexer Lexer.cpp Token.cpp Parser.cpp)
add_executable (kcalc Kcalc.cpp)
target_link_libraries (kcalc lexer Threads::Threads)
