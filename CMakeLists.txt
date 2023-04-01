cmake_minimum_required(VERSION 3.0.0)
project(logic VERSION 0.1.0)

include(GoogleTest)
enable_testing()
#gtest_discover_tests(logic_tests)
add_subdirectory(googletest)
include_directories(googletest/googletest/include)
add_executable(logic_tests logic_tests.cpp LogicBlocks.cpp CPostfixLogicParser.cpp)
add_executable(logic main.cpp LogicBlocks.cpp CPostfixLogicParser.cpp)
target_link_libraries(
    logic_tests
    GTest::gtest_main
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
