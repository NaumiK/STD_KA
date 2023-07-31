project(KA_UI)

set(SRCS
    Animator.cpp
    AssetManager.cpp
    Button.cpp
    ChooseDialog.cpp
    LCDRowDisplay.cpp
    MessageBar.cpp
    NumKeyBoard.cpp
    TextButton.cpp
    UserCursor.cpp
)

add_library(KA_UI STATIC ${SRCS})

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(KA_UI sfml-system sfml-network sfml-graphics sfml-window sfml-audio)