include(GoogleTest)

add_subdirectory(googletest)
include_directories(googletest/googletest/include)
include_directories(../src)

add_executable(logic_tests logic_tests.cpp ../src/LogicBlocks.cpp ../src/CPostfixLogicParser.cpp)
target_link_libraries(logic_tests GTest::gtest_main)
add_test(logic_tests logic_tests)
