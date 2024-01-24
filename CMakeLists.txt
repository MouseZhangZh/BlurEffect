cmake_minimum_required(VERSION 3.16)
project(blur_widget)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(blur_widget main.cpp res.qrc)
target_link_libraries(blur_widget
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

