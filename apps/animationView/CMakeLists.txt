set(SRCS 
    animationView.cpp
)

add_executable(animationView ${SRCS})
target_include_directories(animationView PRIVATE ${CMake_BINARY_DIR})