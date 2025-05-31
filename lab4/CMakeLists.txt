cmake_minimum_required(VERSION 3.22)
project(lab3)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} 
                main.cpp 
                lexer.cpp
                tree.cpp
)
