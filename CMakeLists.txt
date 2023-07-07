cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 20)
project(STD_PRJ)
add_executable(STD_PRJ main.cpp Animator.cpp
        Button.cpp TextButton.cpp SoundAnimationChain.h
        NumKeyBoard.cpp FuncWithDesc.h ChooseDialog.cpp
        AssetManager.cpp UserCursor.cpp
        LCDRowDisplay.cpp MessageBar.cpp
        Bank.cpp Status.h CoffeeChecker.cpp)

#add_executable(STD_PRJ test.cpp)
#add_executable(STD_PRJ animationView.cpp Animator.cpp AssetManager.cpp)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(nlohmann_json 3.11.2 REQUIRED)
target_link_libraries(STD_PRJ sfml-system sfml-network sfml-graphics sfml-window sfml-audio nlohmann_json::nlohmann_json)
