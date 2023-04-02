cmake_minimum_required(VERSION 3.0.0)
project(logic VERSION 0.1.0)

add_executable(logic main.cpp LogicBlocks.cpp CPostfixLogicParser.cpp CInfixToPostfixExpression.cpp)
