cmake_minimum_required(VERSION 3.23)
project(control_midi)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/home/dario/Qt/6.2.4/gcc_64/" )

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
add_executable(control_midi main.cpp
        ui/ventana_principal.cpp ui/ventana_principal.hpp ui/ventana_principal.ui)
target_link_libraries(control_midi
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )