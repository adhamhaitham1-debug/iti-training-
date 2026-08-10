cmake_minimum_required(VERSION 3.16)

project(calculator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Qml)

qt_standard_project_setup()

qt_add_executable(appcalculator
    main.cpp
)

qt_add_qml_module(appcalculator
    URI calculator
    VERSION 1.0
    QML_FILES
        Main.qml
    SOURCES
        calculatorbackend.h
        calculatorbackend.cpp
)


target_link_libraries(appcalculator PRIVATE Qt6::Quick Qt6::Qml)