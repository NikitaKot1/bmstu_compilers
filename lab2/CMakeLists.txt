cmake_minimum_required(VERSION 3.22)
project(lab2)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} 
                main.cpp 
                parser.cpp
                grammar.cpp
                fincs.cpp
)
