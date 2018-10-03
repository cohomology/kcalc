include_directories (
  ${PROJECT_SOURCE_DIR}/include
  ${READLINE_INCLUDE_DIR} 
  ${GMP_INCLUDES}
) 
add_library (lexer Lexer.cpp)
add_library (parser Parser.cpp)
add_library (exceptions Exceptions.cpp)
add_library (ast Ast.cpp)
add_library (repl Repl.cpp)
add_executable (kcalc Kcalc.cpp)
target_link_libraries (kcalc lexer parser ast repl exceptions Threads::Threads ${GMP_LIBRARIES} ${READLINE_LIBRARY})
