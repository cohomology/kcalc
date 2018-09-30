include_directories (
  ${PROJECT_SOURCE_DIR}/include
) 
find_package(GTest REQUIRED)
find_package(GMP 6.0.0) 
include (GoogleTest)
add_executable(lexer_test LexerTest.cpp TestMain.cpp)
add_executable(ast_test AstTest.cpp TestMain.cpp) 
target_link_libraries(lexer_test GTest::GTest GTest::Main Threads::Threads lexer)
target_link_libraries(ast_test GTest::GTest GTest::Main Threads::Threads ast ${GMP_LIBRARIES}) 
gtest_discover_tests(lexer_test) 
gtest_discover_tests(ast_test)  
add_test(LexerTest lexer_test)
add_test(AstTest ast_test) 
