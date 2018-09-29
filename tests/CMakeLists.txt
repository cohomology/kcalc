include_directories ("${PROJECT_SOURCE_DIR}/include") 
find_package(GTest REQUIRED)
include (GoogleTest)
add_executable(lexer_test LexerTest.cpp TestMain.cpp)
target_link_libraries(lexer_test GTest::GTest GTest::Main Threads::Threads lexer)
gtest_discover_tests(lexer_test) 
add_test(LexerTest lexer_test)

