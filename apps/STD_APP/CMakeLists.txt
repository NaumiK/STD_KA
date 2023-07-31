set(SRCS 
    main.cpp
    Bank.cpp
    CoffeeChecker.cpp
)

add_executable(STD_APP ${SRCS})
target_include_directories(STD_APP PRIVATE ${CMake_BINARY_DIR})

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(nlohmann_json 3.11.2 REQUIRED)
target_link_libraries(STD_APP sfml-system sfml-network sfml-graphics sfml-window sfml-audio nlohmann_json::nlohmann_json)