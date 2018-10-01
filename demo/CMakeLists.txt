find_package(Readline REQUIRED)
include_directories (
  ${PROJECT_SOURCE_DIR}/include 
  ${READLINE_INCLUDE_DIR}
) 
add_executable (lexer_repl LexerRepl.cpp)
target_link_libraries (lexer_repl lexer Threads::Threads ${READLINE_LIBRARY})
add_executable (parser_repl ParserRepl.cpp)
target_link_libraries (parser_repl lexer parser ast Threads::Threads ${READLINE_LIBRARY} ${GMP_LIBRARIES}) 
