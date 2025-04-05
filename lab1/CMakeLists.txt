cmake_minimum_required(VERSION 3.22)
project(lab1)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} 
                main.cpp 
                Parser.cpp
                NFA.cpp
                DFA.cpp
)
