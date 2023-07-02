cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 20)
project(STD_PRJ)
add_executable(STD_PRJ Animator.cpp Button.cpp main.cpp NumKeyBoard.h NumKeyBoard.cpp)


find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(STD_PRJ sfml-system sfml-network sfml-graphics sfml-window sfml-audio)
