cmake_minimum_required(VERSION 3.0.0)
project(logic VERSION 0.1.0)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 14)
include(CPack)
