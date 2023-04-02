cmake_minimum_required(VERSION 3.0.0)
project(logic VERSION 0.1.0)

#enable_testing()
#add_subdirectory(tests)
#gtest_discover_tests(logic_tests)

add_executable(logic main.cpp LogicBlocks.cpp CPostfixLogicParser.cpp)


#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
