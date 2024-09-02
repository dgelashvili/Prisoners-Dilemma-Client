cmake_minimum_required(VERSION 3.29)
project(PrisonersDilemmaClient)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(PrisonersDilemmaClient main.cpp
                src/Client.cpp)

target_link_libraries(PrisonersDilemmaClient Ws2_32)