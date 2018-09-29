enable_testing()
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})
include (GoogleTest)
add_executable(lexer_test LexerTest.cpp TestMain.cpp)
target_link_libraries(lexer_test gtest gtest_main ${CMAKE_THREAD_LIBS_INIT}) 
gtest_discover_tests(lexer_test)

