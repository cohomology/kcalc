include_directories (
  ${PROJECT_SOURCE_DIR}/include 
) 
add_executable (lexer_repl LexerRepl.cpp)
target_link_libraries (lexer_repl repl lexer Threads::Threads ${READLINE_LIBRARY})
add_executable (parser_repl ParserRepl.cpp)
target_link_libraries (parser_repl repl lexer parser ast exceptions Threads::Threads ${READLINE_LIBRARY} ${GMP_LIBRARIES}) 
