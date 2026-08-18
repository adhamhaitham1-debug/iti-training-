cmake_minimum_required(VERSION 3.16)
project(QtMediaPlayer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 1. Qt 6 Required Components (Added QuickControls2 for menus/dialogs)
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    QuickControls2
    Multimedia
    Quick Multimedia
)

# 2. Executable Target Sources
qt_add_executable(QtMediaPlayer
    main.cpp
    MediaPlayerBackend.h
    MediaPlayerBackend.cpp
    AppBackend.h
    resources.qrc
)

# 3. Link Qt 6 Libraries
target_link_libraries(QtMediaPlayer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Multimedia

)
