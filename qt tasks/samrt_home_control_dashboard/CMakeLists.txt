cmake_minimum_required(VERSION 3.16)

project(samrt_home_control_dashboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick LinguistTools)

qt_standard_project_setup(
    REQUIRES 6.8
    I18N_SOURCE_LANGUAGE en
    I18N_TRANSLATED_LANGUAGES ar es fr de
)

qt_add_executable(appsamrt_home_control_dashboard
    main.cpp
)

qt_add_qml_module(appsamrt_home_control_dashboard
    URI samrt_home_control_dashboard
    VERSION 1.0
    QML_FILES
        Main.qml
    RESOURCES
        images/user_logo.png
        images/light.png
        images/ac.png
        images/fan.png
        images/garage.png
)

# Qt 6 Translation tools
qt_add_translations(appsamrt_home_control_dashboard
    TS_FILE_BASE qml
    TS_FILE_DIR i18n
    RESOURCE_PREFIX /qt/qml/samrt_home_control_dashboard/i18n
)

set_target_properties(appsamrt_home_control_dashboard PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appsamrt_home_control_dashboard
    PRIVATE Qt6::Quick
)