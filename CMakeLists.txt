cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 20)
project(STD_PRJ)
add_executable(STD_PRJ main.cpp Animator.cpp
        Button.cpp TextButton.cpp
        NumKeyBoard.cpp FuncWithDesc.h ChooseDialog.cpp
        AssetManager.cpp UserCursor.cpp
        LCDRowDisplay.cpp MessageBar.cpp
        KA_UI.h)
#add_executable(STD_PRJ test.cpp)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(STD_PRJ sfml-system sfml-network sfml-graphics sfml-window sfml-audio)
